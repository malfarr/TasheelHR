<?php

function hrm_services_leave_form($empid) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    if (isset($employee['location']) && $employee['location']) {
      $lid = $employee['location'];
      $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));
      $leave_types_filters = array(
        array('field' => 'oid', 'value' => $employee['oid'], 'operator' => '='),
        array('field' => 'status', 'value' => HRM_STATUS_ACTIVE),
        array('field' => 'emp_apply', 'value' => HRM_FLAG_YES, 'operator' => '='),
        array('field' => 'bid', 'value' => $employee['bid'], 'operator' => '='),
      );
      $leave_types = hrm_basic_leave_type_get_data($leave_types_filters);

      $employee_entitlements_filters = array(
        array('field' => 'empid', 'value' => $empid, 'operator' => '='),
        array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
      );
      $employee_entitlements = hrm_basic_leave_get_leave_entitlements($employee_entitlements_filters);

      if (!empty($leave_types)) {
        $data = array();
        foreach ($leave_types as $leave_type_id => $leave_type) {
          $balance = 0;
          if (!empty($employee_entitlements)) {
            foreach ($employee_entitlements as $employee_entitlement) {
              if ($employee_entitlement['leave_type'] == $leave_type_id) {
                $balance = $employee_entitlement['days'];
                break;
              }
            }
          }
          if ($leave_type['beyond_balance'] == HRM_FLAG_NO) {
            if ($balance <= 0) {
              continue;
            }
          }
//          $leave_type_options = array();
          if ($leave_type['gender'] && $leave_type['gender'] != $employee['gender']) {
            continue;
          }
          $leave_type_options = $leave_type['name'];
          $duration = array();
          if ($leave_type['dur_full_day'] == HRM_FLAG_YES) {
            $duration[] = array(
              'id' => HRM_LEAVE_DURATION_FULL_DAY,
              'title' => 'Full Day'
            );
          }
          if ($leave_type['dur_half_day'] == HRM_FLAG_YES) {
            $duration[] = array(
              'id' => HRM_LEAVE_DURATION_HALF_DAY,
              'title' => 'Half Day'
            );
          }
          if ($leave_type['dur_time'] == HRM_FLAG_YES) {
            $duration[] = array(
              'id' => HRM_LEAVE_DURATION_TIME,
              'title' => 'Specify Time'
            );
          }

//          $json->aa = $leave_type_options;
//          return $json;
          $data[] = array(
            'id' => $leave_type_id,
            'name' => $leave_type_options,
            'balance' => round($balance, 2),
            'duration' => $duration,
          );
        }

        if (empty($data)) {
          $json->status = 404;
          $json->message_title = 'Apply Leave';
          $json->message = 'Sorry! you have insifficient leave balance.***';
          return $json;
        }
        $json->status = 200;
        $json->leave_types = $data;
        return $json;
      }
      else {
        $json->status = 404;
        $json->message_title = 'Apply Leave';
        $json->message = 'Leave types are not defined. Please contact your HR administrator';
        return $json;
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Apply Leave';
      $json->message = 'Your location has not been defined';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Apply Leave';
    $json->message = 'Employee not found or blocked';
  }
}

function hrm_services_leave_apply_leave($empid, $leave_type, $from_date, $to_date, $duration_data, $comment = '') {


  $start_date = hrm_basic_datetime_convert($from_date, HRM_DATE_SYSTEM_DATE, HRM_DATE_SHORT);
  $end_date = hrm_basic_datetime_convert($to_date, HRM_DATE_SYSTEM_DATE, HRM_DATE_SHORT);
  if (!is_numeric($from_date)) {
    $from_date = strtotime(str_replace('/', '-', $from_date) . ' 08:00');
  }
  if (!is_numeric($to_date)) {
    $to_date = strtotime(str_replace('/', '-', $to_date) . ' 08:00');
  }

  $json = new stdClass;

  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);

  if (!empty($employee)) {
    $oid = $employee['oid'];
    $leave_type_filters = array(
      array('field' => 'oid', 'value' => $oid, 'operator' => '='),
      array('field' => 'id', 'value' => $leave_type, 'operator' => '='),
      array('field' => 'emp_apply', 'value' => HRM_FLAG_YES, 'operator' => '='),
    );

    $selected_leave_type = hrm_basic_leave_type_get_data($leave_type_filters, TRUE);
    if (empty($selected_leave_type)) {
      $json->status = 404;
      $json->message_title = 'Apply Leave';
      $json->message = 'This leave type is not found. Please contact your HR administrator';
      return $json;
    }

    $beyond_balance = $selected_leave_type['beyond_balance'];
    $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));

    $balance_days = 0;
    $entitlement_filters = array(
      array('field' => 'empid', 'value' => $empid, 'operator' => '='),
      array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
      array('field' => 'leave_type', 'value' => $selected_leave_type['id'], 'operator' => '='),
    );

    $entitlement = hrm_basic_leave_get_leave_entitlements($entitlement_filters, TRUE);

    if (empty($entitlement)) {
      if (!$beyond_balance) {
        $json->status = 404;
        $json->message_title = 'Apply Leave';
        $json->message = 'Sorry! you have no entitltements.***';
        return $json;
      }
    }
    else {
      $balance_days = $entitlement['days'] - $entitlement['pending'];
      $entitlement_id = $entitlement['eid'];
    }
    $leave_status = HRM_STATUS_PENDING;

    $assigned_to = 0;
    if ($selected_leave_type['approval_method'] == HRM_LEAVE_APPROVAL_METHOD_REPORT_TO) {
      $reporting_method = $selected_leave_type['approval_value'];
      $report_to_employee = db_select('hrm_emp_reporting', 'hrm_emp_reporting')
          ->fields('hrm_emp_reporting', array('empid', 'supervisor_empid', 'method'))
          ->condition('method', $reporting_method, '=')
          ->condition('empid', $empid, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);

      if (empty($report_to_employee)) {
        $json->status = 404;
        $json->message_title = 'Apply Leave';
        $json->message = 'there is no approving manager/supervisor has been assigned for you. Please contact your HR administrator';
        return $json;
      }
      $assigned_to = $report_to_employee['supervisor_empid'];
    }
    elseif ($selected_leave_type['approval_method'] == HRM_LEAVE_APPROVAL_METHOD_EMPLOYEE) {
      $assigned_to = $selected_leave_type['approval_value'];
    }
    elseif ($selected_leave_type['approval_method'] == HRM_LEAVE_APPROVAL_METHOD_AUTO_APPROVAL) {
      $leave_status = HRM_STATUS_SCHEDULED;
    }

    $duration_data_object = json_decode($duration_data);
    $duration_data_array = object_to_array($duration_data_object);
    $duration = $duration_data_array['duration'];
    $half_day = $duration_data_array['half_day'];
    $from_time = $duration_data_array['from_time'];
    $to_time = $duration_data_array['to_time'];

    $org_date_format = variable_get(HRM_VAR_ORG_DATE_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_SIMPLE);
    $from_date_formatted = date(HRM_DATE_SIMPLE, $from_date);
    $to_date_formatted = date(HRM_DATE_SIMPLE, $to_date);

    if (isset($employee['location']) && $employee['location']) {
      $location = $employee['location'];
      $emp_location = hrm_basic_location_get_data(array(array('field' => 'lid', 'value' => $location)), TRUE, TRUE);
      $timezone_offset = hrm_basic_timezone_get_offset($emp_location['timezone'], 'UTC');
      $now = time();
      $employee_time = $now + $timezone_offset;

      $work_shift = hrm_basic_workshift_get_employee_workshift($empid, $employee);

      $work_shift_start_timestamp = strtotime($start_date . ' ' . $work_shift['from_time']);
      $work_shift_end_timestamp = strtotime($start_date . ' ' . $work_shift['to_time']);
      $work_shift_duration_timestamp = $work_shift_end_timestamp - $work_shift_start_timestamp;
      $work_shift_duration_half_day_timestamp = $work_shift_duration_timestamp / 2;

      $work_shift_hours_array = explode(':', $work_shift['duration']);
      $work_shift_hours_minutes = $work_shift_hours_array[1];
      $work_shift_minutes = $work_shift_hours_minutes / 60;
      $work_shift_hours_float = $work_shift_hours_array[0] + $work_shift_minutes;

      $all_days = hrm_basic_datetime_days_range($start_date, $end_date);

      switch ($duration) {
        case HRM_LEAVE_DURATION_FULL_DAY:

          $temp_from_time = $work_shift['from_time'];
          $temp_to_time = $work_shift['to_time'];
          $temp_duration = $work_shift['duration'];
          $temp_hours = $work_shift_hours_float;
          $temp_days = 1.00;

          break;
        case HRM_LEAVE_DURATION_HALF_DAY:
          $half_day_start_timestamp = 0;
          $half_day_end_timestamp = 0;

          if ($half_day == HRM_LEAVE_HALF_DAY_MORNING) {
            $half_day_start_timestamp = $work_shift_start_timestamp;
            $half_day_end_timestamp = $work_shift_start_timestamp + $work_shift_duration_half_day_timestamp;
          }
          elseif ($half_day == HRM_LEAVE_HALF_DAY_AFTERNOON) {
            $half_day_start_timestamp = $work_shift_start_timestamp + $work_shift_duration_half_day_timestamp;
            $half_day_end_timestamp = $work_shift_end_timestamp;
          }

          $temp_from_time = date(HRM_DATE_TIME_12, $half_day_start_timestamp);
          $temp_to_time = date(HRM_DATE_TIME_12, $half_day_end_timestamp);

          $temp_duration = hrm_basic_time_timestamp_to_time($work_shift_duration_half_day_timestamp);

          $temp_hours_array = explode(':', $temp_duration);
          $temp_hours_minutes = $temp_hours_array[1];
          $temp_minutes = $temp_hours_minutes / 60;
          $temp_hours_float = $temp_hours_array[0] + $temp_minutes;
          $temp_hours = $temp_hours_float;
          $temp_days = 0.50;
          break;
        case HRM_LEAVE_DURATION_TIME:

          $leave_start_timestamp = strtotime($from_date_formatted . ' ' . $from_time);
          $leave_end_timestamp = strtotime($from_date_formatted . ' ' . $to_time);
          $leave_duration_timestamp = $leave_end_timestamp - $leave_start_timestamp;

          $temp_from_time = $from_time;
          $temp_to_time = $to_time;

          $temp_duration = hrm_basic_time_timestamp_to_time($leave_duration_timestamp);

          $work_shift_from_time = date(HRM_DATE_SYSTEM_TIME, strtotime($work_shift['from_time']));
          $work_shift_to_time = date(HRM_DATE_SYSTEM_TIME, strtotime($work_shift['to_time']));
          $from_time_formatted = date(HRM_DATE_SYSTEM_TIME, strtotime($from_time));
          $to_time_formatted = date(HRM_DATE_SYSTEM_TIME, strtotime($to_time));

          if (($from_time_formatted < $work_shift_from_time) || ($to_time_formatted > $work_shift_to_time)) {
            $json->status = 404;
            $json->message_title = 'Apply Leave';
            $json->message = 'Sorry! you select duration time out of range your workshift ***';
            return $json;
          }
          if ($from_date_formatted > $to_time_formatted) {
            $json->status = 404;
            $json->message_title = 'Apply Leave';
            $json->message = 'Sorry! you select wrong duration time ***';
            return $json;
          }

          $temp_hours_array = explode(':', $temp_duration);
          $temp_hours_minutes = $temp_hours_array[1];
          $temp_minutes = $temp_hours_minutes / 60;
          $temp_hours_float = $temp_hours_array[0] + $temp_minutes;

          $temp_hours = $temp_hours_float;
          $temp_days = $leave_duration_timestamp / $work_shift_duration_timestamp;
          break;
      }
      $temp_from_time = hrm_basic_datetime_convert($temp_from_time, HRM_DATE_SYSTEM_TIME, HRM_DATE_TIME_12);
      $temp_to_time = hrm_basic_datetime_convert($temp_to_time, HRM_DATE_SYSTEM_TIME, HRM_DATE_TIME_12);
      if ($from_date == $to_date) {
        if (!hrm_basic_day_is_working_day($oid, $location, $from_date_formatted)) {
          $json->status = 404;
          $json->message_title = 'Apply Leave';
          $json->message = 'Selected date is not a working day***';
          return $json;
        }

        if (hrm_basic_day_is_apply_leave_before($empid, $start_date, $temp_from_time, $temp_to_time)) {
          $date = date($org_date_format, $from_date);
          $json->status = 404;
          $json->message_title = 'Apply Leave';
          $json->message = 'Sorry! you have applied for a leave at same day before and there is a time overlap, ***';
          return $json;
        }
      }
      else {
        $leave_working_days = array();
        $flag = TRUE;
        $date = $from_date_formatted;

        while ($flag) {
          if ($date == $to_date_formatted) {
            $flag = FALSE;
          }
          $date_formatted = date(HRM_DATE_SYSTEM_DATE, strtotime($date));

          if (hrm_basic_day_is_apply_leave_before($empid, $date_formatted, $temp_from_time, $temp_to_time)) {
            $json->status = 404;
            $json->message_title = 'Apply Leave';
            $json->message = 'Sorry! you have applied for a leave at same day before and there is a time overlap, on: ' . $date . '***';
            return $json;
          }
          if (hrm_basic_day_is_working_day($oid, $location, $date)) {
            $leave_working_days[] = $date;
          }

          $next_day_timestamp = strtotime($date . "+1 days");
          $date = date(HRM_DATE_SIMPLE, $next_day_timestamp);
        }

        if (empty($leave_working_days)) {
          $json->status = 404;
          $json->message_title = 'Apply Leave';
          $json->message = 'Sorry! selected dates are not a working days***';
          return $json;
        }
      }

      $timing = array();
      $total_hours = 0;
      $total_days = 0;

      foreach ($all_days as $day) {
        $day_date = hrm_basic_datetime_convert($day, HRM_DATE_SYSTEM_DATE, HRM_DATE_SIMPLE);

        $include_day = TRUE;
        $day_status = hrm_basic_day_status($oid, $location, $day);

        if ($day_status == HRM_DAY_STATUS_HOLIDAY && !$selected_leave_type['holidays_bd']) {
          $include_day = FALSE;
        }
        if ($day_status == HRM_DAY_STATUS_WEEKEND && !$selected_leave_type['weekend_bd']) {
          $include_day = FALSE;
        }

        if (!$include_day) {
          continue;
        }

        switch ($duration) {
          case HRM_LEAVE_DURATION_FULL_DAY:

            $total_hours += $work_shift['duration'];
            $total_days ++;

            break;
          case HRM_LEAVE_DURATION_HALF_DAY:
            $total_hours += hrm_basic_time_timestamp_to_time($work_shift_duration_half_day_timestamp);
            $total_days += 0.50;

            break;
          case HRM_LEAVE_DURATION_TIME:
            $total_hours += $temp_hours;
            $total_days += $temp_days;
            break;
        }
        $temp_start_time = $day_date . ' ' . $temp_from_time;
        $temp_end_time = $day_date . ' ' . $temp_to_time;

        $timing[] = array(
          'start_time' => $temp_start_time,
          'end_time' => $temp_end_time,
          'hours' => $temp_hours,
          'days' => $temp_days,
          'day_date' => $day_date,
        );
      }
      $from_timestamp = strtotime($from_date_formatted);
      $to_timestamp = strtotime($to_date_formatted);


      if (!empty($timing)) {
        if ($beyond_balance == HRM_FLAG_NO && ($balance_days < $total_days)) {
          $json->status = 404;
          $json->message_title = 'Apply Leave';
          $json->message = 'Sorry! you have insifficient leave balance.***';

          return $json;
        }

        $starting_date = date(HRM_DATE_SYSTEM_DATE, $from_timestamp);
        $starting_date .= ' at ' . date(HRM_DATE_SYSTEM_TIME, $from_timestamp);
        $leave_type_name = hrm_basic_leave_type_get_name($leave_type);
        $leave_owner_name = $employee['name'];

        if ($from_timestamp == '') {
          $json->status = 404;
          $json->message = 'Invalid from date';
          return $json;
        }
        if ($to_timestamp == '') {
          $json->status = 404;
          $json->message = 'Invalid to date';
          return $json;
        }

        hrm_basic_leave_entitlement_register_pending_leave($empid, $oid, $leave_type, $leave_period, $total_days, $leave_status);
        if (empty($entitlement)) {
          $entitlement_filters = array(
            array('field' => 'empid', 'value' => $empid, 'operator' => '='),
            array('field' => 'leave_type', 'value' => $leave_type, 'operator' => '='),
            array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
          );
          $entitlement = hrm_basic_leave_get_leave_entitlements($entitlement_filters, TRUE);
          $entitlement_id = $entitlement['eid'];
        }

        $lid = db_insert('hrm_leave')
          ->fields(array(
            'oid' => $employee['oid'],
            'empid' => $empid,
            'leave_type' => $leave_type,
            'hours' => $total_hours,
            'days' => $total_days,
            'start_date' => $start_date,
            'end_date' => $end_date,
            'assigned_to' => $assigned_to,
            'leave_entitlement' => $entitlement_id,
            'leave_period' => $leave_period,
            'source' => HRM_DATA_SOURCE_APP,
            'comment' => $comment,
            'status' => $leave_status,
            'uid' => 0,
            'created' => $employee_time,
            'changed' => $employee_time,
          ))
          ->execute();

        $first_start_date = 0;
        $last_end_date = 0;
        foreach ($timing as $leave_timing) {

          if ($first_start_date == 0) {
            $first_start_date = $leave_timing['start_time'];
          }
          if ($last_end_date == 0) {
            $last_end_date = $leave_timing['end_time'];
          }
          $first_start_date = (($leave_timing['start_time'] < $first_start_date) ? $leave_timing['start_time'] : $first_start_date);
          $last_end_date = (($leave_timing['end_time'] > $last_end_date) ? $leave_timing['end_time'] : $last_end_date);

          db_insert('hrm_leave_timing')
            ->fields(array(
              'lid' => $lid,
              'oid' => $employee['oid'],
              'empid' => $empid,
              'leave_type' => $leave_type,
              'day_date' => $leave_timing['day_date'],
              'start_time' => $leave_timing['start_time'],
              'end_time' => $leave_timing['end_time'],
              'hours' => $leave_timing['hours'],
              'days' => $leave_timing['days'],
              'duration_type' => $duration,
              'status' => $leave_status
            ))
            ->execute();
        }

        db_update('hrm_leave')
          ->fields(array(
            'start_date' => $first_start_date,
            'end_date' => $last_end_date))
          ->condition('lid', $lid, '=')
          ->execute();

        db_insert('hrm_leave_action')
          ->fields(array(
            'lid' => $lid,
            'action' => HRM_ACTION_APPLY,
            'comment' => $comment,
            'created_entity' => HRM_ENTITY_EMPLOYEE,
            'created_entity_id' => $empid,
            'created' => $employee_time,
          ))
          ->execute();
        $log_data = array(
          'title' => $lid,
          'total_days' => $total_days,
          'total_houres' => $total_hours,
          'start_date' => $start_date,
          'end_date' => $end_date,
          'justification' => $comment,
        );
        hrm_backend_entity_history_log_action(HRM_ENTITY_LEAVE, $lid, HRM_ACTION_APPLY, 0, $log_data, $employee_time, HRM_ENTITY_EMPLOYEE, $empid, $comment);

        if ($leave_status == HRM_STATUS_PENDING && $assigned_to) {
          $supervisor_notification_message = str_replace(array('@[leave_owner_name]', '@[leave_type_name]', '@[starting_date]'), array($leave_owner_name, $leave_type_name, $starting_date), HRM_GCM_MSG_LEAVE_SUBMIT_SUPERVISOR_NOTIFICATION);
          $notification_data = array(
            'key' => HRM_APP_NOTIF_KEY_LEAVE_APPROVAL,
            'message' => $supervisor_notification_message,
            'id' => $leave_type,
          );
          $supervisor_device_id = hrm_basic_gcm_get_device_id(array(array('field' => 'empid', 'value' => $assigned_to, 'operator' => '=')));
          hrm_services_sendGoogleCloudMessage($notification_data, $supervisor_device_id);
        }
        elseif ($leave_status == HRM_STATUS_SCHEDULED) {
          $leave_owner_notification_message = str_replace(array('@[leave_type_name]', '@[starting_date]'), array($leave_type_name, $starting_date), HRM_GCM_MSG_LEAVE_APPROVED_OWNER_NOTIFICATION);
          $notification_data = array(
            'key' => HRM_APP_NOTIF_KEY_LEAVE_APPROVED,
            'message' => $leave_owner_notification_message,
            'id' => $leave_type,
          );
          $leave_owner_device_id = hrm_basic_gcm_get_device_id(array(array('field' => 'empid', 'value' => $empid, 'operator' => '=')));
          hrm_services_sendGoogleCloudMessage($notification_data, $leave_owner_device_id);

          $employee_supervisors = hrm_basic_employee_get_supervisors($empid);
          if (!empty($employee_supervisors)) {
            $supervisors_notification_message = str_replace(array('@[leave_type_name]', '@[starting_date]', '@[leave_owner_name]'), array($leave_type_name, $starting_date, $leave_owner_name), HRM_GCM_MSG_LEAVE_APPROVED_SUPERVISOR_NOTIFICATION);
            $supervisors_gcm_data = array(
              'key' => HRM_APP_NOTIF_KEY_LEAVE_STAFF_APPROVED,
              'message' => $supervisors_notification_message,
              'id' => $leave_id,
            );
            $supervisors_devices = hrm_basic_gcm_get_device_id(array(array('field' => 'empid', 'value' => $employee_supervisors, 'operator' => '=')));
            hrm_services_sendGoogleCloudMessage($supervisors_gcm_data, $supervisors_devices);
          }
//            hrm_emails_notification_leave_auto_approved_leave($empid, $leave_id);
        }


        $json->status = 200;
        $json->message = 'Leave request has been submitted';
        return $json;
      }
      else {
        $json->status = 404;
        $json->message_title = 'Apply Leave';
        $json->message = 'Invalid Leave Data.***';
        return $json;
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Apply Leave';
      $json->message = 'Your location has not been defined';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Apply Leave';
    $json->message = 'This account is not found or blocked';
    return $json;
  }
}

function hrm_services_leave_my_leaves($empid) {
  $emp_filter = array(array('field' => 'empid', 'value' => 6),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee)) {


    $leaves_filters = array(array('field' => 'empid', 'value' => 6));
    $leaves = hrm_basic_leave_get_data($leaves_filters, FALSE);

    $data = hrm_services_leave_filter_date($leaves, $employee);

    $json->status = 200;
    $json->leaves = $data;
    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'My Leave';
    $json->message = 'Employee not found or blocked';
    return $json;
  }
}

function hrm_services_leave_staff_leaves($empid) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee)) {
//    $oid = $employee['oid'];
//    $leave_status = hrm_basic_leave_status_get_options();
//    $leave_types = hrm_basic_leave_type_names(array(array('field' => 'oid', 'value' => $oid), array('field' => 'bid', 'value' => $employee['bid'])));

    $data = array();
    $staff_uids = hrm_basic_employee_get_subordinates($empid);

    if (!empty($staff_uids)) {
      $logs_employees = array();
      $users = array();
//      $date_format = variable_get(HRM_VAR_ORG_DATE_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_SIMPLE);
//      $time_format = variable_get(HRM_VAR_ORG_TIME_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_TIME_24);
//      $date_time_format = $date_format . ' ' . $time_format;

      $filters = array(array('field' => 'empid', 'value' => $staff_uids, 'operator' => 'IN'),);
      $leaves = hrm_basic_leave_get_data($filters);
      $employees = hrm_services_employee_get_data($filters);
      $data = array();
      foreach ($employees as $employee) {

        $data[] = hrm_services_leave_filter_date($leaves, $employee);
      }

      /*
        foreach ($leaves as $leave) {
        $leave_employee = $employees[$leave['empid']];
        $start_date = date($date_format, strtotime($leave['start_date']));
        $end_date = date($date_format, strtotime($leave['end_date']));
        $leave_date = $start_date . ' - ' . $end_date;
        if ($start_date == $end_date) {
        $leave_date = $start_date . ' (' . date(HRM_DATE_TIME_12, $leave['start_timestamp']) . ' - ' . date(HRM_DATE_TIME_12, $leave['end_timestamp']) . ')';
        }
        $json->a = $end_date;
        return $json;
        $hours_duration = $leave['hours'] * (60 * 60);
        $hours = hrm_basic_time_timestamp_to_time($hours_duration);

        $leave_data = array(
        'id' => $leave['lid'],
        'employee' => $leave_employee['name'],
        'photo' => $leave_employee['photo_url'],
        'title' => $leave_employee['job_title_name'],
        'applied_on' => date(HRM_DATE_SHORT_TIME_12, $leave['created']),
        'leave_type_id' => $leave['leave_type'],
        'leave_type_name' => $leave_types[$leave['leave_type']],
        'leave_status_id' => $leave['status'],
        'leave_status_name' => $leave_status[$leave['status']],
        'leave_date' => $leave_date,
        'days' => number_format($leave['days'], 2),
        'hours' => $hours,
        'timing' => array(),
        'history' => array(),
        );

        //Timing
        $timing_rows = array();
        foreach ($leave['timing'] as $timing) {
        $timing_duration = $timing['length_hours'] * (60 * 60);
        $timing_hours = hrm_basic_time_timestamp_to_time($timing_duration);
        $timing_rows[] = array(
        'date' => date(HRM_DATE_SHORT, $timing['start_timestamp']),
        'time' => date(HRM_DATE_TIME_12, $timing['start_timestamp']) . ' - ' . date(HRM_DATE_TIME_12, $timing['end_timestamp']),
        'days' => number_format($timing['length_days'], 2),
        'hours' => $timing_hours,
        );
        }
        $leave_data['timing'] = $timing_rows;

        //History
        $logs = array();
        foreach ($leave['actions'] as $action) {
        switch ($action['action']) {
        case HRM_ACTION_ASSIGN:
        if (!isset($users[$action['created_entity_id']])) {
        $users[$action['created_entity_id']] = hrm_basic_account_get_name($action['created_entity_id']);
        }
        if (!isset($logs_employees[$leave['empid']])) {
        $logs_employees[$leave['empid']] = hrm_basic_employee_get_name($leave['empid']);
        }

        $description = theme('section_app_content_with_lines', array(
        'content' => 'Leave Assigned to ' . $logs_employees[$leave['empid']] . ' by ' . $users[$action['created_by']] . '\n\n' . $action['comment']));
        $logs[] = array(
        'title' => 'Leave Assigned',
        'date' => date(HRM_DATE_SHORT_TIME_12, $action['created']),
        'description' => rtrim($description),
        );
        break;
        case HRM_ACTION_APPLY:
        if (!isset($logs_employees[$action['created_entity_id']])) {
        $logs_employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
        }

        $description = theme('section_app_content_with_lines', array(
        'content' => 'Leave applied by ' . $logs_employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
        $logs[] = array(
        'title' => 'Leave Applied',
        'date' => date(HRM_DATE_SHORT_DAY_NAME_TIME_12, $action['created']),
        'description' => rtrim($description),
        );
        break;
        case HRM_ACTION_ASSIGN_TO_OTHER:
        $action_content = unserialize($action['comment']);

        if (!isset($logs_employees[$action['created_entity_id']])) {
        $logs_employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
        }
        if (!isset($logs_employees[$action_content['new_assigned_to']])) {
        $logs_employees[$action_content['new_assigned_to']] = hrm_basic_employee_get_name($action_content['new_assigned_to']);
        }

        $description = theme('section_app_content_with_lines', array(
        'content' => 'Leave approval assigned to ' . $logs_employees[$action_content['new_assigned_to']] . ' by ' . $logs_employees[$action['created_entity_id']] . '\n\n' . $action_content['comment']));
        $logs[] = array(
        'title' => 'Leave approval assign to other',
        'date' => date(HRM_DATE_SHORT_TIME_12, $action['created']),
        'description' => rtrim($description),
        );
        break;
        case HRM_ACTION_REJECT:
        if (!isset($logs_employees[$action['created_entity_id']])) {
        $logs_employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
        }

        $description = theme('section_app_content_with_lines', array(
        'content' => 'Leave rejected by ' . $logs_employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
        $logs[] = array(
        'title' => 'Leave Rejected',
        'date' => date(HR_DATETIME_FORMAT_LONG_SLASH, $action['created']),
        'description' => rtrim($description),
        );
        break;
        case HRM_ACTION_APPROVE:
        if (!isset($logs_employees[$action['created_entity_id']])) {
        $logs_employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
        }

        $description = theme('section_app_content_with_lines', array(
        'content' => 'Leave approved by ' . $logs_employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
        $logs[] = array(
        'title' => 'Leave Approved',
        'date' => date(HRM_DATE_SHORT_TIME_12, $action['created']),
        'description' => rtrim($description),
        );
        break;
        }
        }
        $leave_data['history'] = $logs;

        if ($leave['status'] == HRM_STATUS_PENDING) {
        $entitlement_filters = array(
        array('field' => 'empid', 'value' => $leave['empid'], 'operator' => '='),
        array('field' => 'leave_type', 'value' => $leave['leave_type'], 'operator' => '='),
        );
        $entitlement = hrm_basic_leave_get_leave_entitlements($entitlement_filters, TRUE);
        if (!empty($entitlement)) {
        $entitlement_data = array(
        'title' => 'Leave entitlement',
        'items' => array(
        array('title' => 'Pending', 'value' => number_format($entitlement['pending'], 2)),
        array('title' => 'Taken', 'value' => number_format($entitlement['taken'], 2)),
        array('title' => 'Currently allowed', 'value' => number_format($entitlement['balance'], 2)),
        array('title' => 'Future accrue', 'value' => number_format($entitlement['future_accrue'], 2)),
        ),
        );
        $leave_data['entitlement'] = $entitlement_data;
        }
        }

        $data[] = $leave_data;
        }
       */
    }

    $json->status = 200;
    $json->leaves = $data;
    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Staff Leave';
    $json->message = 'Employee not found or blocked';
    return $json;
  }
}

function hrm_services_leave_pending_leaves($empid) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee)) {
    $oid = $employee['oid'];
    $filters = array(array('field' => 'oid', 'value' => $oid),
      array('field' => 'oid', 'value' => $oid));

    $leave_status = hrm_basic_leave_status_get_options();
    $leave_types = hrm_basic_leave_type_get_data($filters);

    $date_format = variable_get(HRM_VAR_ORG_DATE_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_SIMPLE);
    $time_format = variable_get(HRM_VAR_ORG_TIME_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_TIME_24);
    $date_time_format = $date_format . ' ' . $time_format;
    $date_time_with_day_format = 'l ' . $date_time_format;

    $employee_supervisors_ids = hrm_basic_employee_get_supervisors($empid);
    $employee_supervisors = array();
    foreach ($employee_supervisors_ids as $supervisor_id) {
      $employee_supervisors[] = array(
        'id' => $supervisor_id,
        'name' => hrm_basic_employee_get_name($supervisor_id)
      );
    }

    $employees = array();
    $users = array();

    $leaves_filters = array(
      array('field' => 'assigned_to', 'value' => $empid, 'operator' => '='),
      array('field' => 'status', 'value' => HRM_STATUS_PENDING, 'operator' => '='),
    );
    $leaves = hrm_basic_leave_get_data($leaves_filters);
    $data = array();

    $is_hr = hrm_basic_employee_is_user($empid, $employee['email'], HRM_ROLE_HR);
    if ($is_hr) {
      $all_employees_filters = array(
        array('field' => 'oid', 'value' => $oid),
        array('field' => 'bid', 'value' => $employee['bid']),
        array('field' => 'status', 'value' => HRM_STATUS_ACTIVE,)
      );
      $employee_supervisors = array();
      $all_employees = hrm_basic_employee_names($all_employees_filters);
      foreach ($all_employees as $key => $value) {
        $employee_supervisors[] = array(
          'id' => $key,
          'name' => $value
        );
      }
    }

    $leave_owners = array();

    foreach ($leaves as $leave) {
      $leave_owner_empid = $leave['empid'];
      if (!isset($leave_owners[$leave_owner_empid])) {
        $leave_owner_filters = array(array('field' => 'empid', 'value' => $leave_owner_empid, 'operator' => '='));
        $leave_owner = hrm_services_employee_get_data($leave_owner_filters, TRUE);
        $leave_owners[$leave_owner_empid] = $leave_owner;
      }

      $start_date = date($date_format, strtotime($leave['start_date']));
      $end_date = date($date_format, strtotime($leave['end_date']));
      $leave_date = $start_date . ' - ' . $end_date;
      if ($start_date == $end_date) {
        $leave_date = $start_date;
      }

      $hours_duration = $leave['hours'] * (60 * 60);

      $hours = hrm_basic_time_timestamp_to_time($hours_duration);

      $leave_data = array(
        'id' => $leave['lid'],
        'employee' => $leave_owners[$leave_owner_empid]['name'],
        'photo' => $leave_owners[$leave_owner_empid]['photo_url'],
        'title' => $leave_owners[$leave_owner_empid]['job_title_name'],
        'applied_on' => date($date_time_format, $leave['created']),
        'leave_type_id' => $leave['leave_type'],
        'leave_type_name' => $leave_types[$leave['leave_type']]['name'],
        'leave_status_id' => $leave['status'],
        'leave_status_name' => $leave_status[$leave['status']],
        'leave_date' => $leave_date,
        'days' => number_format($leave['days'], 4),
        'hours' => $hours,
        'timing' => array(),
        'history' => array(),
      );

      //Timing
      $timing_rows = array();
      foreach ($leave['timing'] as $timing) {
//        $timing_duration = $timing['length_hours'] * (60 * 60);
//        $timing_hours = hrm_basic_time_timestamp_to_time($timing_duration);
        $timing_rows[] = array(
          'date' => date($date_format, strtotime($timing['start_time'])),
          'time' => date($date_time_format, strtotime($timing['start_time'])) . ' - ' . date($date_time_format, strtotime($timing['end_time'])),
          'days' => number_format($timing['days'], 2),
          'hours' => $timing['hours'],
        );
      }
      $leave_data['timing'] = $timing;

      //History
      $logs = array();

      $leave_data['history'] = $logs;

      /////////////////////////////////////////////////////////////////////////
      foreach ($leave['actions'] as $action) {
        switch ($action['action']) {
          case HRM_ACTION_ASSIGN:
            if (!isset($users[$action['created_entity_id']])) {
              $users[$action['created_entity_id']] = hrm_basic_account_get_name($action['created_entity_id']);
            }
            if (!isset($employees[$leave['empid']])) {
              $employees[$leave['empid']] = hrm_basic_employee_get_name($leave['empid']);
            }

            $description = theme('section_app_content_with_lines', array(
              'content' => 'Leave Assigned to ' . $employees[$leave['empid']] . ' by ' . $users[$action['created_entity_id']] . '\n\n' . $action['comment']));
            $logs[] = array(
              'title' => 'Leave Assigned',
              'date' => date($date_time_format, $action['created']),
              'description' => rtrim($description),
            );
            break;
          case HRM_ACTION_APPLY:
            if (!isset($employees[$action['created_entity_id']])) {
              $employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
            }

            $description = theme('section_app_content_with_lines', array(
              'content' => 'Leave applied by ' . $employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
            $logs[] = array(
              'title' => 'Leave Applied',
              'date' => date($date_time_with_day_format, $action['created']),
              'description' => rtrim($description),
            );
            break;
          case HRM_ACTION_ASSIGN_TO_OTHER:
            $action_content = unserialize($action['comment']);

            if (!isset($employees[$action['created_entity_id']])) {
              $employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
            }
            if (!isset($employees[$action_content['new_assigned_to']])) {
              $employees[$action_content['new_assigned_to']] = hrm_basic_employee_get_name($action_content['new_assigned_to']);
            }

            $description = theme('section_app_content_with_lines', array(
              'content' => 'Leave approval assigned to ' . $employees[$action_content['new_assigned_to']] . ' by ' . $employees[$action['created_entity_id']] . '\n\n' . $action_content['comment']));
            $logs[] = array(
              'title' => 'Leave approval assign to other',
              'date' => date($date_time_with_day_format, $action['created']),
              'description' => rtrim($description),
            );
            break;
          case HRM_ACTION_REJECT:
            if (!isset($employees[$action['created_entity_id']])) {
              $employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
            }

            $description = theme('section_app_content_with_lines', array(
              'content' => 'Leave rejected by ' . $employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
            $logs[] = array(
              'title' => 'Leave Rejected',
              'date' => date($date_time_format, $action['created']),
              'description' => rtrim($description),
            );
            break;
          case HRM_ACTION_APPROVE:
            if (!isset($employees[$action['created_entity_id']])) {
              $employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
            }

            $description = theme('section_app_content_with_lines', array(
              'content' => 'Leave approved by ' . $employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
            $logs[] = array(
              'title' => 'Leave Approved',
              'date' => date($date_time_format, $action['created']),
              'description' => rtrim($description),
            );
            break;
        }
      }
      $leave_data['history'] = $logs;

      if ($leave['status'] == HRM_STATUS_PENDING) {

        $entitlement_filters = array(
          array('field' => 'empid', 'value' => $employee['empid'], 'operator' => '='),
          array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
          array('field' => 'leave_type', 'value' => $leave['leave_type'], 'operator' => '='),
        );
        $entitlement = hrm_basic_leave_get_leave_entitlements($entitlement_filters, TRUE);

        if (!empty($entitlement)) {
          $entitlement_data = array(
            'title' => 'Leave entitlement',
            'items' => array(
              array('title' => 'Pending', 'value' => number_format($entitlement['pending'], 4)),
              array('title' => 'Used', 'value' => number_format($entitlement['used'], 4)),
              array('title' => 'Days', 'value' => number_format($entitlement['days'], 4)),
              array('title' => 'Future accrue', 'value' => number_format($entitlement['future_accrue'], 4)),
            ),
          );
          $leave_data['entitlement'] = $entitlement_data;
        }
      }
      /////////////////////////////////////////////////////////////////////////////

      $actions = array(
        array('id' => HRM_ACTION_APPROVE, 'label' => 'Approve', 'color' => '#4CAF50'),
        array('id' => HRM_ACTION_REJECT, 'label' => 'Reject', 'color' => '#F44336'),
      );
      if ($leave_types[$leave['leave_type']]['assign_to_other'] && !empty($employee_supervisors)) {
        $actions[] = array(
          'id' => HRM_ACTION_ASSIGN_TO_OTHER,
          'label' => 'Assign to other', 'color' => '#2196F3',
          'others' => $employee_supervisors
        );
      }
      $leave_data['actions'] = $actions;
      $data[] = $leave_data;
    }

    $json->status = 200;
    $json->leaves = $data;
    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Pending Leave';
    $json->message = 'Employee not found or blocked';
    return $json;
  }
}

function hrm_services_leave_action($empid, $lid, $action, $comment = '', $assign_to_other = '') {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee)) {
    if (isset($employee['location']) && $employee['location']) {

      $location = $employee['location'];
      $emp_location = hrm_basic_location_get_data(array(array('field' => 'lid', 'value' => $location)), TRUE, TRUE);
      $timezone_offset = hrm_basic_timezone_get_offset($emp_location['timezone'], 'UTC');
      $now = time();
      $employee_time = $now + $timezone_offset;

      $oid = $employee['oid'];
      $leave_filters = array(array('field' => 'lid', 'value' => $lid));
      $leave = hrm_basic_leave_get_data($leave_filters, TRUE);

      $date_format = variable_get(HRM_VAR_ORG_DATE_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_SIMPLE);

      if (!empty($leave)) {
        $leave_owner_id = $leave['empid'];
        $leave_owner = hrm_services_employee_get_data(array(array('field' => 'empid', 'value' => $leave_owner_id)), TRUE);

        if ($leave['status'] == HRM_STATUS_PENDING) {
          $leave_type = $leave['leave_type'];
          $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));
          $leave_days = $leave['days'];

          $leave_owner_name = $leave_owner['name'];
          $leave_type_name = hrm_basic_leave_type_get_name($leave_type);

          $starting_date = date($date_format, strtotime($leave['start_date']));
          $action_employee_name = $employee['name'];

          $leave_owner_message = '';
          $leave_owner_message_key = '';
          $action_message = '';

          switch ($action) {
            case HRM_ACTION_APPROVE:
// ============================================================================================================
// -------------------------- delete attendance if leave was full time ,  by MOHAMAD --------------------------
// ============================================================================================================
              if (!empty($leave['timing'])) {
                foreach ($leave['timing'] as $timing) {
                  $day_date = hrm_basic_datetime_convert($timing['day_date'], HRM_DATE_SIMPLE, HRM_DATE_SYSTEM_DATE);

                  $attendance_filters = array(
                    array('field' => 'oid', 'value' => $oid, 'operator' => '='),
                    array('field' => 'empid', 'value' => $leave['empid'], 'operator' => '='),
                    array('field' => 'date', 'value' => $day_date, 'operator' => '='),
                  );
                  $attendance_records = hrm_basic_attendance_get_data($attendance_filters, FALSE, TRUE);

                  if (!empty($attendance_records)) {
                    foreach ($attendance_records as $record) {
                      if ($timing['duration_type'] == HRM_LEAVE_DURATION_FULL_DAY) {
                        db_delete('hrm_attendance')
                          ->condition('aid', $record['aid'], '=')
                          ->execute();
                        db_delete('hrm_attendance_details')
                          ->condition('aid', $record['aid'], '=')
                          ->execute();
                        db_delete('hrm_attendance_adjustment')
                          ->condition('aid', $record['aid'], '=')
                          ->execute();
                        $message = 'your attendance has been deleted ***';
                      }
                    }
                  }
                }
              }
// ============================================================================================================
// ------------------------------------------------------------------------------------------------------------
// ============================================================================================================

              db_update('hrm_leave')
                ->fields(array(
                  'status' => HRM_STATUS_SCHEDULED,
                  'changed' => $employee_time
                ))
                ->condition('lid', $lid, '=')
                ->execute();

              db_update('hrm_leave_timing')
                ->fields(array(
                  'status' => HRM_STATUS_SCHEDULED,
                ))
                ->condition('lid', $lid, '=')
                ->execute();

              db_insert('hrm_leave_action')
                ->fields(array(
                  'lid' => $lid,
                  'action' => HRM_ACTION_APPROVE,
                  'comment' => $comment,
                  'created_entity' => HRM_ENTITY_EMPLOYEE,
                  'created_entity_id' => $empid,
                  'created' => $employee_time,
                ))->execute();

              $entitlement_filters = array(
                array('field' => 'empid', 'value' => $leave['empid'], 'operator' => '='),
                array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
                array('field' => 'leave_type', 'value' => $leave_type['id'], 'operator' => '='),
              );

              $entitlement = hrm_basic_leave_entitlement_get_data($entitlement_filters, TRUE);

              db_update('hrm_leave_entitlement')
                ->fields(array(
                  'days' => ($entitlement['days'] - $leave_days),
                  'changed' => $employee_time,
                ))
                ->condition('eid', $entitlement['eid'], '=')
                ->execute();

              $leave_owner_message = str_replace(array('@[leave_type_name]', '@[starting_date]'), array($leave_type_name, $starting_date), HRM_GCM_MSG_LEAVE_APPROVED_OWNER_NOTIFICATION);
              $leave_owner_message_key = HRM_APP_NOTIF_KEY_LEAVE_APPROVED;

              $action_message = 'Leave request has been approved';
              $log_data = array(
                'title' => $lid,
                'total_days' => $leave['days'],
                'total_hours' => $leave['hours'],
                'start_date' => $leave['start_date'],
                'end_date' => $leave['end_date'],
                'justification' => $comment,
              );
              hrm_backend_entity_history_log_action(HRM_ENTITY_LEAVE, $lid, HRM_ACTION_APPROVE, 0, $log_data, $employee_time, HRM_ENTITY_EMPLOYEE, $empid, $comment);

              //Notify supersisors
              $employee_supervisors = hrm_basic_employee_get_supervisors($leave['empid']);
              if (!empty($employee_supervisors)) {
                $supervisors_message = str_replace(array('@[leave_type_name]', '@[starting_date]', '@[leave_owner_name]'), array($leave_type_name, $starting_date, $leave_owner_name), HRM_GCM_MSG_LEAVE_APPROVED_SUPERVISOR_NOTIFICATION);
                $supervisors_gcm_data = array(
                  'key' => HRM_APP_NOTIF_KEY_LEAVE_APPROVED,
                  'message' => $supervisors_message,
                  'id' => $lid,
                );

                $supervisors_devices = hrm_basic_gcm_get_device_id(array(array('field' => 'empid', 'value' => $employee_supervisors, 'operator' => 'IN')));
                hrm_services_sendGoogleCloudMessage($supervisors_gcm_data, $supervisors_devices);
              }




              break;
            case HRM_ACTION_REJECT:
              db_update('hrm_leave')
                ->fields(array(
                  'status' => HRM_STATUS_REJECTED,
                  'changed' => $employee_time
                ))
                ->condition('lid', $lid, '=')
                ->execute();

              db_update('hrm_leave_timing')
                ->fields(array(
                  'status' => HRM_STATUS_REJECTED,
                ))
                ->condition('lid', $lid, '=')
                ->execute();

              db_insert('hrm_leave_action')
                ->fields(array(
                  'lid' => $lid,
                  'action' => HRM_ACTION_REJECT,
                  'comment' => $comment,
                  'created_entity' => HRM_ENTITY_EMPLOYEE,
                  'created_entity_id' => $empid,
                  'created' => $employee_time,
                ))->execute();


              hrm_basic_leave_entitlement_register_rejected_leave($leave_owner_id, $oid, $leave_type, $leave_period, $leave_days);
              $leave_owner_message = str_replace(array('@[leave_type_name]', '@[starting_date]', '@[rejected_by_name]'), array($leave_type_name, $starting_date, $action_employee_name), HRM_GCM_MSG_LEAVE_REJECTED_OWNER_NOTIFICATION);
              $leave_owner_message_key = HRM_APP_NOTIF_KEY_LEAVE_REJECTED;

              $action_message = 'Leave request has been rejected';
              $log_data = array(
                'title' => $lid,
                'total_days' => $leave['days'],
                'total_hours' => $leave['hours'],
                'start_date' => $leave['start_date'],
                'end_date' => $leave['end_date'],
                'justification' => $comment,
              );
              hrm_backend_entity_history_log_action(HRM_ENTITY_LEAVE, $lid, HRM_ACTION_REJECT, 0, $log_data, $employee_time, HRM_ENTITY_EMPLOYEE, $empid, $comment);

              break;
            case HRM_ACTION_ASSIGN_TO_OTHER:

              $previous_emp = db_select('hrm_leave')
                  ->fields('hrm_leave', array('assigned_to'))
                  ->condition('lid', $lid, '=')
                  ->execute()->fetchField();

              db_update('hrm_leave')
                ->fields(array(
                  'assigned_to' => $assign_to_other,
                  'changed' => $employee_time
                ))
                ->condition('lid', $lid, '=')
                ->execute();

              $content_array = array('new_assigned_to' => $assign_to_other, 'content' => $comment);
              db_insert('hrm_leave_action')
                ->fields(array(
                  'lid' => $lid,
                  'action' => HRM_ACTION_ASSIGN_TO_OTHER,
                  'comment' => serialize($content_array),
                  'created_entity' => HRM_ENTITY_EMPLOYEE,
                  'created_entity_id' => $empid,
                  'created' => $employee_time,
                ))->execute();
              $assigned_to_message = str_replace(array('@[transfered_by_name]', '@[leave_owner_name]', '@[leave_type_name]'), array($action_employee_name, $leave_owner_name, $leave_type_name), HRM_GCM_MSG_LEAVE_TRANSFERED_ASSIGNED_TO_NOTIFICATION);
              $gcm_data = array(
                'key' => HRM_APP_NOTIF_KEY_LEAVE_APPROVAL,
                'message' => $assigned_to_message,
                'id' => $lid,
              );
              $assigned_to_device_id = hrm_basic_gcm_get_device_id(array(array('field' => 'empid', 'value' => $assign_to_other, 'operator' => '=')));
              hrm_services_sendGoogleCloudMessage($gcm_data, $assigned_to_device_id);

              $assign_to_other_name = hrm_basic_employee_get_name($assign_to_other);

              $leave_owner_message = str_replace(array('@[leave_type_name]', '@[starting_date]', '@[assigned_to_name]'), array($leave_type_name, $starting_date, $assign_to_other_name), HRM_GCM_MSG_LEAVE_TRANSFERED_OWNER_NOTIFICATION);
              $leave_owner_message_key = HRM_APP_NOTIF_KEY_LEAVE_ASSIGNED;

              $action_message = 'Leave request has been transfered';
              $log_data = array(
                'title' => $lid,
                'total_days' => $leave['days'],
                'total_hours' => $leave['hours'],
                'start_date' => $leave['start_date'],
                'end_date' => $leave['end_date'],
                'prev_assign_to' => $previous_emp,
                'assign_to' => $assign_to_other,
                'justification' => $comment,
              );
              hrm_backend_entity_history_log_action(HRM_ENTITY_LEAVE, $lid, HRM_ACTION_ASSIGN_TO_OTHER, 0, $log_data, $employee_time, HRM_ENTITY_EMPLOYEE, $empid, $comment);
              break;
          }

          $gcm_data = array(
            'key' => $leave_owner_message_key,
            'message' => $leave_owner_message,
            'id' => $lid,
          );
          $leave_owner_device_id = hrm_basic_gcm_get_device_id(array(array('field' => 'empid', 'value' => $leave['empid'], 'operator' => '=')));
          hrm_services_sendGoogleCloudMessage($gcm_data, $leave_owner_device_id);

          $json->status = 200;
          $json->message = $action_message;
          return $json;
        }
        else {
          $message = 'Leave request status is ';
          switch ($leave['status']) {
            case HRM_STATUS_REJECTED:
              $message .= 'REJECTED';
              break;
            case HRM_STATUS_SCHEDULED:
              $message .= 'SCHEDULED';
              break;
            case HRM_STATUS_USED:
              $message .= 'TAKEN';
              break;
          }
          if ($message == 'Leave request status is ') {
            $message .= 'not pending approval';
          }

          $json->status = 404;
          $json->message_title = 'Leave Action';
          $json->message = $message;
          return $json;
        }
      }
      else {
        $json->status = 404;
        $json->message_title = 'Leave Action';
        $json->message = 'Leave request is not found';
        return $json;
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Leave Action';
      $json->message = 'Your location has not been defined';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Leave Action';
    $json->message = 'This account is not found or blocked';
    return $json;
  }
}

function hrm_services_leave_entitlements($empid) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee)) {
    $oid = $employee['oid'];
    $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));
    $entitlements_filters = array(
      array('field' => 'empid', 'value' => $empid, 'operator' => '='),
      array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
      array('field' => 'oid', 'value' => $oid, 'operator' => '=')
    );





    $employee_entitlements = hrm_basic_leave_get_leave_entitlements($entitlements_filters);


    $leave_type_filters = array(array('field' => 'oid', 'value' => $oid), array('field' => 'bid', 'value' => $employee['bid']));
    $leave_types = hrm_basic_leave_type_names($leave_type_filters);




    $data = array();
    foreach ($employee_entitlements as $entitlement) {
      
   
      $data[] = array(
        'label' => $leave_types[$entitlement['leave_type']],
        'year' => $entitlement['leave_period'],
        'days' => array(
          array('title' => 'Pending', 'value' => number_format($entitlement['pending'], 2) . ' days'),
          array('title' => 'Taken', 'value' => number_format($entitlement['used'], 2) . ' days'),
          array('title' => 'Balance', 'value' => number_format($entitlement['days'], 2) . ' days'),
          array('title' => 'Accrued Balance', 'value' => number_format($entitlement['future_accrue'], 2) . ' days'),
        ),
      );
    }

    $json->status = 200;
    $json->entitlements = $data;
    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Leave Entitlements';
    $json->message = 'Employee not found or blocked';
    return $json;
  }
}

function hrm_services_leave_filter_date($leaves, $employee) {
  $oid = $employee['oid'];

  $leave_status = hrm_basic_leave_status_get_options();
  $leave_types = hrm_basic_leave_type_names(array(array('field' => 'oid', 'value' => $oid), array('field' => 'bid', 'value' => $employee['bid'])));
  $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));

  $employees = array();
  $users = array();
  $data = array();
  $date_format = variable_get(HRM_VAR_ORG_DATE_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_SIMPLE);
  $time_format = variable_get(HRM_VAR_ORG_TIME_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_TIME_24);
  $date_time_format = $date_format . ' ' . $time_format;
  $date_time_with_day_format = 'l ' . $date_time_format;

  foreach ($leaves as $leave) {


    $start_date = date($date_format, strtotime($leave['start_date']));
    $end_date = date($date_format, strtotime($leave['end_date']));

    $leave_date = $start_date . ' - ' . $end_date;

    if ($start_date == $end_date) {
      $leave_date = $start_date;
    }

    $hours_duration = $leave['hours'] * (60 * 60);
    $hours = hrm_basic_time_timestamp_to_time($hours_duration);


    $leave_data = array(
      'id' => $leave['lid'],
      'employee' => $employee['name'],
      'applied_on' => date($date_time_format, $leave['created']),
      'leave_type_id' => $leave['leave_type'],
      'leave_type_name' => $leave_types[$leave['leave_type']],
      'leave_status_id' => $leave['status'],
      'leave_status_name' => $leave_status[$leave['status']],
      'leave_date' => $leave_date,
      'days' => number_format($leave['days'], 2),
      'hours' => $hours,
      'timing' => array(),
      'history' => array(),
    );

    //Timing
    $timing_rows = array();

    foreach ($leave['timing'] as $timing) {
      $timing_rows[] = array(
        'date' => date($date_format, strtotime($timing['day_date'])),
        'time' => date($time_format, strtotime($timing['start_time'])) . ' - ' . date($time_format, strtotime($timing['end_time'])),
        'days' => number_format($timing['days'], 2),
        'hours' => $timing['hours'],
      );
    }
    $leave_data['timing'] = $timing_rows;

    //History
    $logs = array();

    foreach ($leave['actions'] as $action) {
      switch ($action['action']) {
        case HRM_ACTION_ASSIGN:
          if (!isset($users[$action['created_entity_id']])) {
            $users[$action['created_entity_id']] = hrm_basic_account_get_name($action['created_entity_id']);
          }
          if (!isset($employees[$leave['empid']])) {
            $employees[$leave['empid']] = hrm_basic_employee_get_name($leave['empid']);
          }

          $description = theme('section_app_content_with_lines', array(
            'content' => 'Leave Assigned to ' . $employees[$leave['empid']] . ' by ' . $users[$action['created_entity_id']] . '\n\n' . $action['comment']));
          $logs[] = array(
            'title' => 'Leave Assigned',
            'date' => date($date_time_with_day_format, $action['created']),
            'description' => rtrim($description),
          );
          break;
        case HRM_ACTION_APPLY:
          if (!isset($employees[$action['created_entity_id']])) {
            $employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
          }

          $description = theme('section_app_content_with_lines', array(
            'content' => 'Leave applied by ' . $employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
          $logs[] = array(
            'title' => 'Leave Applied',
            'date' => date($date_time_with_day_format, $action['created']),
            'description' => rtrim($description),
          );
          break;
        case HRM_ACTION_ASSIGN_TO_OTHER:
          $action_content = unserialize($action['comment']);

          if (!isset($employees[$action['created_entity_id']])) {
            $employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
          }
          if (!isset($employees[$action_content['new_assigned_to']])) {
            $employees[$action_content['new_assigned_to']] = hrm_basic_employee_get_name($action_content['new_assigned_to']);
          }

          $description = theme('section_app_content_with_lines', array(
            'content' => 'Leave approval assigned to ' . $employees[$action_content['new_assigned_to']] . ' by ' . $employees[$action['created_entity_id']] . '\n\n' . $action_content['comment']));
          $logs[] = array(
            'title' => 'Leave approval assign to other',
            'date' => date($date_time_with_day_format, $action['created']),
            'description' => rtrim($description),
          );
          break;
        case HRM_ACTION_REJECT:
          if (!isset($employees[$action['created_entity_id']])) {
            $employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
          }

          $description = theme('section_app_content_with_lines', array(
            'content' => 'Leave rejected by ' . $employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
          $logs[] = array(
            'title' => 'Leave Rejected',
            'date' => date($date_time_with_day_format, $action['created']),
            'description' => rtrim($description),
          );
          break;
        case HRM_ACTION_APPROVE:
          if (!isset($employees[$action['created_entity_id']])) {
            $employees[$action['created_entity_id']] = hrm_basic_employee_get_name($action['created_entity_id']);
          }

          $description = theme('section_app_content_with_lines', array(
            'content' => 'Leave approved by ' . $employees[$action['created_entity_id']] . '\n\n' . $action['comment']));
          $logs[] = array(
            'title' => 'Leave Approved',
            'date' => date($date_time_with_day_format, $action['created']),
            'description' => rtrim($description),
          );
          break;
      }
    }
    $leave_data['history'] = $logs;

    if ($leave['status'] == HRM_STATUS_PENDING) {

      $entitlement_filters = array(
        array('field' => 'empid', 'value' => $employee['empid'], 'operator' => '='),
        array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
        array('field' => 'leave_type', 'value' => $leave['leave_type'], 'operator' => '='),
      );
      $entitlement = hrm_basic_leave_get_leave_entitlements($entitlement_filters, TRUE);

      if (!empty($entitlement)) {
        $entitlement_data = array(
          'title' => 'Leave entitlement',
          'items' => array(
            array('title' => 'Pending', 'value' => number_format($entitlement['pending'], 4)),
            array('title' => 'Used', 'value' => number_format($entitlement['used'], 4)),
            array('title' => 'Days', 'value' => number_format($entitlement['days'], 4)),
            array('title' => 'Future accrue', 'value' => number_format($entitlement['future_accrue'], 4)),
          ),
        );
        $leave_data['entitlement'] = $entitlement_data;
      }
    }
    $data[] = $leave_data;
  }
  return $data;
}
