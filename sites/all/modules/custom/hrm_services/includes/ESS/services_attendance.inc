<?php

function hrm_services_attendance_punchin($empid, $lat, $long, $note) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    if ($employee['work_shift']) {
      $emp_location = hrm_basic_location_get_data(array(array('field' => 'lid', 'value' => $employee['location'])), TRUE, TRUE);
      if (!empty($emp_location)) {
        $timezone_offset = hrm_basic_timezone_get_offset($emp_location['timezone'], 'UTC');
        $date_format = variable_get(HRM_VAR_ORG_DATE_FORMAT . $employee['oid'] . '_' . $employee['bid'], HRM_DATE_SIMPLE);
        $time_format = variable_get(HRM_VAR_ORG_TIME_FORMAT . $employee['oid'] . '_' . $employee['bid'], HRM_DATE_TIME_12);
        $utc_now = time();
        $timestamp = $utc_now + $timezone_offset;

        $in_time = date(HRM_DATE_SYSTEM_TIME, $timestamp);
//        $in_date = date(HRM_DATE_SIMPLE, $timestamp);
        $date = date(HRM_DATE_SYSTEM_DATE, $timestamp);
        $in_time_formatted = date($time_format, $timestamp);

        $apply_leave_before = hrm_basic_attendance_is_apply_leave($empid, $date, $in_time);
        if ($apply_leave_before) {
          $json->status = 404;
          $json->message = 'you have been applied leave today***';
          return $json;
        }


        $in_timestamp = $utc_now;
        $in_coordinates = $lat . ',' . $long;
        $in_near_loc = hrm_basic_map_calculate_nearest_location_distance($employee['oid'], $in_coordinates);
        $in_status = HRM_STATUS_PRESENT;

        $emp_work_shift = hrm_basic_workshift_get_data(array(array('field' => 'id', 'value' => $employee['work_shift'])), TRUE);
        $workshift_from_timestamp = strtotime($date . ' ' . $emp_work_shift['from_time']);

        $diff = $workshift_from_timestamp - $in_timestamp;

        $diff_minute = $diff / 60;
        if ($diff_minute > 0) {
          $in_status = HRM_STATUS_EARLY_IN;
        }
        else if ($diff_minute < 0) {
          $in_status = HRM_STATUS_LATE_IN;
        }

        $punched_in_before = hrm_basic_attendance_is_punched_in_before($empid, $date);

        $aid = 0;
        if (empty($punched_in_before)) {
          $attendance_fields = array(
            'oid' => $employee['oid'],
            'bid' => $employee['bid'],
            'empid' => $empid,
            'date' => $date,
            'in_time' => $in_time,
            'in_date' => $date,
            'in_coordinates' => $in_coordinates,
            'in_near_loc' => $in_near_loc,
            'in_comment' => $note,
            'in_status' => $in_status,
            'timezone' => $emp_location['timezone'],
            'source' => HRM_DATA_SOURCE_APP,
            'status' => HRM_ATTENDANCE_LOG_UNCOMPLETED,
          );

          $aid = db_insert('hrm_attendance')
            ->fields($attendance_fields)
            ->execute();
        }
        else {
          $aid = $punched_in_before['aid'];
        }

        $last_attendance_details = db_select('hrm_attendance_details')
            ->fields('hrm_attendance_details')
            ->condition('aid', $aid, '=')
            ->orderBy('id', 'DESC')
            ->range(0, 1)
            ->execute()->fetch(PDO::FETCH_ASSOC);

        if (!empty($last_attendance_details) && $last_attendance_details['status'] == HRM_ATTENDANCE_LOG_UNCOMPLETED) {
          $json->status = 404;
          $json->message_title = 'Punch in';
          $json->message = 'You are already punched in. Return to the home page and pull the page to synchronize****';
          return $json;
        }

        $attendance_details_fields = array(
          'aid' => $aid,
          'empid' => $empid,
          'in_time' => $in_time,
          'in_date' => $date,
          'in_coordinates' => $in_coordinates,
          'in_near_loc' => $in_near_loc,
          'in_comment' => $note,
          'status' => HRM_ATTENDANCE_LOG_UNCOMPLETED
        );

        db_insert('hrm_attendance_details')
          ->fields($attendance_details_fields)
          ->execute();

        $employee['punch_status'] = 'in';
        $employee['punch_time'] = $in_time_formatted . ' on ' . date($date_format, $in_timestamp);

        $json = hrm_services_employee_generate_employee_object($employee);
        $json->status = 200;
        $json->message = 'Successefully punched in***';
        return $json;
      }
      else {
        $json->status = 404;
        $json->message_title = 'Punch in';
        $json->message = 'Your job has not been defined***';
        return $json;
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Punch in';
      $json->message = 'You dont hava workshift yet***';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Punch in';
    $json->message = 'This account is not found or blocked***';
    return $json;
  }
}

function hrm_services_attendance_punchout($empid, $lat, $long, $note) {
  $employee = hrm_services_employee_get_data(array(array('field' => 'empid', 'value' => $empid)), TRUE);
  $json = new stdClass();

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    if ($employee['work_shift']) {
      $emp_location = hrm_basic_location_get_data(array(array('field' => 'lid', 'value' => $employee['location'])), TRUE, TRUE);
      if (!empty($emp_location)) {
        $timezone_offset = hrm_basic_timezone_get_offset($emp_location['timezone'], 'UTC');

        $utc_now = time();
        $timestamp = $utc_now + $timezone_offset;
        $date_format = variable_get(HRM_VAR_ORG_DATE_FORMAT . $employee['oid'] . '_' . $employee['bid'], HRM_DATE_SIMPLE);
        $time_format = variable_get(HRM_VAR_ORG_TIME_FORMAT . $employee['oid'] . '_' . $employee['bid'], HRM_DATE_TIME_12);

        $out_time = date(HRM_DATE_SYSTEM_TIME, $timestamp);
        $out_time_formatted = date($time_format, $timestamp);
        $out_date = date(HRM_DATE_SYSTEM_DATE, $timestamp);
        $out_date_formatted = date($date_format, $timestamp);

        $out_timestamp = $utc_now;
        $out_coordinates = $lat . ',' . $long;
        $out_near_loc = hrm_basic_map_calculate_nearest_location_distance($employee['oid'], $out_coordinates);
        $out_status = HRM_STATUS_PRESENT;
        $duration_timestamp = 0;

        $current_attendance_details = db_select('hrm_attendance_details')
            ->fields('hrm_attendance_details', array('id', 'aid',
              'in_time', 'in_date', 'in_coordinates', 'in_near_loc', 'in_comment',
              'out_time', 'out_date', 'out_coordinates', 'out_near_loc', 'out_comment',
              'duration', 'status'))
            ->condition('empid', $empid, '=')
            ->condition('status', HRM_ATTENDANCE_LOG_UNCOMPLETED, '=')
            ->orderBy('aid', 'DESC')
            ->range(0, 1)
            ->execute()->fetch(PDO::FETCH_ASSOC);

        $emp_work_shift = hrm_basic_workshift_get_data(array(array('field' => 'id', 'value' => $employee['work_shift'])), TRUE);
        $workshift_to_timestamp = strtotime($current_attendance_details['in_date'] . ' ' . $emp_work_shift['to_time']);

        $diff = $workshift_to_timestamp - strtotime($out_time);
        $diff_minute = $diff / 60;

        if ($diff_minute > 0) {
          $out_status = HRM_STATUS_EARLY_OUT;
        }
        else if ($diff_minute < 0) {
          $out_status = HRM_STATUS_LATE_OUT;
        }

        $aid = $current_attendance_details['aid'];
        $attendance = hrm_basic_attendance_get_data(array(array('field' => 'aid', 'value' => $aid)), TRUE, TRUE);

        if (empty($current_attendance_details) || empty($attendance)) {
          $json->status = 404;
          $json->message_title = 'Punch out';
          $json->message = 'You dont punch in. Return to the home page and pull the page to synchronize***';
          return $json;
        }

//        $duration_timestamp = $out_timestamp - $current_attendance_details['in_timestamp'];
//        $duration_time = hrm_basic_time_timestamp_to_time($duration_timestamp);
//        $attendance_duration_timestamp = ($attendance['duration_timestamp'] + $duration_timestamp);
//        $attendance_duration_time = hrm_basic_time_timestamp_to_time($attendance_duration_timestamp);

        $in_timestamp = hrm_basic_datetime_get_timezone_timestamp($attendance['in_date'] . ' ' . $attendance['in_time'], '+0000');

        $attendance_fields = array(
          'out_time' => $out_time,
          'out_date' => $out_date,
//          'out_timestamp' => $out_timestamp,
//          'out_diff' => $timezone_offset,
          'out_coordinates' => $out_coordinates,
          'out_near_loc' => $out_near_loc,
          'out_comment' => $note,
          'out_status' => $out_status,
          'duration' => $duration_timestamp,
          'shift_duration' => $emp_work_shift['duration_timestamp'],
          'status' => HRM_ATTENDANCE_LOG_COMPLETED,
        );

        db_update('hrm_attendance')
          ->fields($attendance_fields)
          ->condition('aid', $aid, '=')
          ->execute();

        $attendance_details_fields = array(
          'out_time' => $out_time,
          'out_date' => $out_date,
//          'out_timestamp' => $out_timestamp,
//          'out_diff' => $timezone_offset,
          'out_coordinates' => $out_coordinates,
          'out_near_loc' => $out_near_loc,
          'out_comment' => $note,
          'duration' => $duration_timestamp,
//          'duration_timestamp' => $duration_timestamp,
          'status' => HRM_ATTENDANCE_LOG_COMPLETED,
        );
        db_update('hrm_attendance_details')
          ->fields($attendance_details_fields)
          ->condition('id', $current_attendance_details['id'], '=')
          ->execute();

        $employee['punch_status'] = 'out';
        $employee['punch_time'] = $out_time_formatted . ' on: ' . $out_date_formatted;

        $json = hrm_services_employee_generate_employee_object($employee);
        $json->status = 200;
        $json->message = 'Successefully punched out***';
        return $json;
      }
      else {
        $json->status = 404;
        $json->message_title = 'Punch in';
        $json->message = 'Your job has not been defined***';
        return $json;
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Punch out';
      $json->message = 'You dont hava workshift yet***';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Punch out';
    $json->message = 'Account not found or blocked***';
    return $json;
  }
}

//check_again
function hrm_services_attendance_staff_attendance($empid, $day) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $date_format = variable_get(HRM_VAR_ORG_DATE_FORMAT . $employee['oid'] . '_' . $employee['bid'], HRM_DATE_SIMPLE);
  $time_format = variable_get(HRM_VAR_ORG_TIME_FORMAT . $employee['oid'] . '_' . $employee['bid'], HRM_DATE_TIME_12);

  $day_date = hrm_basic_datetime_convert($day, $date_format, HRM_DATE_SHORT);
  $day_system_formatted = hrm_basic_datetime_convert($day, HRM_DATE_SYSTEM_DATE, HRM_DATE_SHORT);
  $day_simple_formatted = hrm_basic_datetime_convert($day, HRM_DATE_SIMPLE, HRM_DATE_SHORT);


  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    $oid = $employee['oid'];
    $branch_filters = array(
      array('field' => 'oid', 'value' => $oid, 'operator' => '='),
      array('field' => 'bid', 'value' => $employee['bid'], 'operator' => '=')
    );

    $staff_empids = array($empid);
    $employee_staff = hrm_basic_employee_get_subordinates($empid);
    if (!empty($employee_staff)) {
      $staff_empids = array_merge($staff_empids, $employee_staff);
    }

    $attendance_filters = array(
      array('field' => 'oid', 'value' => $oid, 'operator' => '='),
      array('field' => 'empid', 'value' => $staff_empids, 'operator' => 'IN'),
      array('field' => 'date', 'value' => $day_system_formatted, 'operator' => '='),
    );

    $attendance_records = hrm_basic_attendance_get_data($attendance_filters, FALSE, TRUE);
    $employees_filter = array(
      array('field' => 'oid', 'value' => $oid, 'operator' => '='),
      array('field' => 'empid', 'value' => $staff_empids, 'operator' => 'IN'),
      array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='),
      array('field' => 'attendance', 'value' => HRM_FLAG_YES, 'operator' => '='),
    );
    $employees = hrm_services_employee_get_data($employees_filter);

    $records = array();
    foreach ($employees as $employee) {
      $records[$employee['empid']] = array(
        'empid' => $employee['empid'],
        'name' => $employee['name'],
        'title' => $employee['job_title_name'],
        'photo' => $employee['photo_url'],
        'work_shift' => (isset($employee['work_shift']) ? $employee['work_shift'] : 0),
        'attendance' => array(),
        'attended' => 0,
        'leave' => array('duration' => 0, 'type' => array()),
      );

      $employee_leaves_timing_query = db_select('hrm_leave_timing', 'employee_leave_timing');
      $employee_leaves_timing_query->join('hrm_leave', 'employee_leave', 'employee_leave_timing.lid = employee_leave.lid');
//      $employee_leaves_timing_query->fields('employee_leave_timing', array('length_hours'));
      $employee_leaves_timing_query->fields('employee_leave_timing', array('hours'));
      $employee_leaves_timing_query->fields('employee_leave', array('leave_type'));
      $employee_leaves_timing_query->condition('employee_leave.empid', $employee['empid'], '=');
      $employee_leaves_timing_query->condition('employee_leave.status', array(HRM_STATUS_USED, HRM_STATUS_PENDING, HRM_STATUS_SCHEDULED), 'IN');
      $employee_leaves_timing_query->condition('employee_leave_timing.day_date', $day_system_formatted, '=');
//      $employee_leaves_timing_query->condition('employee_leave_timing.end_timestamp', $to_day_timestamp, '<=');
      $employee_leaves_timing = $employee_leaves_timing_query->execute()->fetchAll(PDO::FETCH_ASSOC);

      if (!empty($employee_leaves_timing)) {
        foreach ($employee_leaves_timing as $employee_leave_timing) {
          $records[$employee['empid']]['leave']['duration'] += ($employee_leave_timing['hours'] * 60 * 60);
          $records[$employee['empid']]['leave']['type'][] = $employee_leave_timing['leave_type'];
        }
      }
    }
    $work_shifts = hrm_basic_workshift_get_data($branch_filters);
    foreach ($work_shifts as $key => $row) {

      $work_shifts[$key]['today_from_time'] = strtotime($day_simple_formatted . ' ' . $row['from_time']);
      $work_shifts[$key]['today_to_time'] = strtotime($day_simple_formatted . ' ' . $row['to_time']);
    }
    $leave_types = hrm_basic_leave_type_names($branch_filters);
    foreach ($attendance_records as $attendance_record) {
      $temp_empid = $attendance_record['empid'];
      if (empty($records[$temp_empid]['attendance'])) {
        $records[$temp_empid]['attended'] = 1;
        $records[$temp_empid]['attendance'] = array(
          'status' => 'Absent',
          'in' => $attendance_record['in_time'],
          'out' => $attendance_record['out_time'],
          'duration' => 0,
        );
      }
      $records[$temp_empid]['attendance']['duration'] += $attendance_record['duration'];

      if ($attendance_record['in_timestamp'] < $records[$temp_empid]['attendance']['in']) {
        $records[$temp_empid]['attendance']['in'] = $attendance_record['in_time'];
      }
      if ($attendance_record['out_timestamp'] > $records[$temp_empid]['attendance']['out']) {
        $records[$temp_empid]['attendance']['out'] = $attendance_record['out_time'];
      }
    }

    $data = array();
    foreach ($records as $record) {
      $status = array();
      $working_duration = 0;
      $leave_duration = 0;

      $in_color = '#212121';
      $out_color = '#212121';
      $duration_color = '#212121';

      $punch_in_time = $record['attendance']['in'];
      $punch_out_time = $record['attendance']['out'];

      $work_shift_from_timestamp = strtotime($attendance_record['in_date'] . ' ' . $work_shifts[$record['work_shift']]['from_time']);
      $work_shift_to_timestamp = strtotime($attendance_record['in_date'] . ' ' . $work_shifts[$record['work_shift']]['to_time']);

      if ($record['attended']) {
        if ($record['work_shift']) {
          /*
            if ($record['attendance']['in'] && $record['attendance']['in'] <= $work_shifts[$record['work_shift']]['today_from_time']) {
            if (!in_array('Early IN', $status)) {
            $status[] = 'Early IN';
            }
            $in_color = '#388E3C';
            }
           */

          if ($punch_in_time && $punch_in_timestamp <= $work_shift_from_timestamp) {
            if (!in_array('Early IN', $status)) {
              $status[] = 'Early IN';
            }
            $in_color = '#388E3C';
          }
          else {
            if (!in_array('Late IN', $status)) {
              $status[] = 'Late IN';
            }
            $in_color = '#F44336';
          }

          if ($punch_out_time && $punch_out_timestamp >= $work_shift_to_timestamp) {
            if (!in_array('Late OUT', $status)) {
              $status[] = 'Late OUT';
            }
            $out_color = '#388E3C';
          }
          elseif ($punch_out_time) {
            if (!in_array('Early OUT', $status)) {
              $status[] = 'Early OUT';
            }
            $out_color = '#F44336';
          }
        }
        $working_duration += $record['attendance']['duration'];

      }

      if ($record['leave']['duration']) {
        $leave_duration = $record['leave']['duration'];
        $working_duration += $record['leave']['duration'];

        if (is_array($record['leave']['type']) && !empty($record['leave']['type'])) {
          foreach ($record['leave']['type'] as $leave_type) {
            if (!in_array($leave_types[$leave_type], $status)) {
              $status[] = $leave_types[$leave_type];
            }
          }
        }
      }

      if ($record['work_shift'] && $working_duration) {

        $org_working_duration = ($work_shifts[$record['work_shift']]['today_to_time'] - $work_shifts[$record['work_shift']]['today_from_time']);
        if ($working_duration >= $org_working_duration) {
          $duration_color = '#388E3C';
        }
        else {
          $out_color = '#F44336';
        }
      }

      $in = (isset($record['attendance']['in']) ? $record['attendance']['in'] : 0);
      $out = (isset($record['attendance']['out']) ? $record['attendance']['out'] : 0);

      if (empty($status)) {
        $in = '00:00';
        $out = '00:00';
        $status[] = 'Absent';
      }
      if ($in == 0) {
        $in = '00:00';
        $in_color = '#212121';
      }
      if ($out == 0) {
        $out = '00:00';
        $out_color = '#212121';
      }

      $data_row = array(
        'name' => $record['name'],
        'title' => $record['title'],
        'photo' => $record['photo'],
        'duration' => hrm_basic_time_timestamp_to_time($working_duration),
        'duration_color' => $duration_color,
        'leave_duration' => hrm_basic_time_timestamp_to_time($leave_duration),
        'in' => $in,
        'in_color' => $in_color,
        'out' => $out,
        'out_color' => $out_color,
        'status' => implode('<br />', $status),
      );
      $data[] = $data_row;
    }

    $json = new stdClass;
    $json->status = 200;
    $json->attendance = $data;
    return $json;
  }
  else {
    $json = new stdClass;
    $json->status = 404;
    $json->message_title = 'Staff attendance';
    $json->message = 'Account not found or blocked';
    return $json;
  }
}
