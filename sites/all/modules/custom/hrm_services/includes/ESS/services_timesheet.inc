<?php

function hrm_services_timesheet_get_timesheet_calendar($empid, $year, $month) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    $oid = $employee['oid'];

    $first_day = '01-' . $month . '-' . $year;
    $first_day_timestamp = strtotime($first_day . ' 00:00:01') - 1;
    $last_day = date('t-m-Y', $first_day_timestamp);
    $last_day_timestamp = strtotime($last_day . ' 23:59:59') + 1;


    $query = db_select('hrm_emp_timesheet', 'employee_timesheet')
      ->fields('employee_timesheet', array('day_date'));
    $query->addExpression('COUNT(id)', 'teaser');
    $query->condition('empid', $empid, '=');
    $query->condition('oid', $oid, '=');
    $query->condition('day_timestamp', array($first_day_timestamp, $last_day_timestamp), 'BETWEEN');
    $query->groupBy('day_date');
    $result = $query->execute()->fetchCol(0);

    $data = array();
    foreach ($result as $value) {
      $data[] = array('date' => $value);
    }
    $json->status = 200;
    $json->calendar = $data;
    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Time Sheet';
    $json->message = 'Account not found or blocked';
    return $json;
  }
}

function hrm_services_timesheet_get_timesheet_log($empid, $day) {
  $emp_filter = array(
    array('field' => 'empid', 'value' => $empid, 'operator' => '='),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '=')
  );
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    $oid = $employee['oid'];
    $branch_filters = array(
      array('field' => 'oid', 'value' => $oid, 'operator' => '='),
      array('field' => 'bid', 'value' => $employee['bid'], 'operator' => '=')
    );
    $projects = hrm_basic_project_get_projects_abbreviation($branch_filters);

    $timesheet_filters = array(
      array('field' => 'empid', 'value' => $empid, 'operator' => '='),
      array('field' => 'oid', 'value' => $oid, 'operator' => '='),
      array('field' => 'day_date', 'value' => $day, 'operator' => '='),
    );
    $timesheet_sort = array(array('field' => 'from_time', 'direction' => 'ASC'));
    $timesheet = hrm_basic_timesheet_get_data($timesheet_filters, $timesheet_sort);
    $org_time_format = variable_get(HRM_VAR_ORG_TIME_FORMAT . $oid . '_' . $employee['bid'], HRM_DATE_SIMPLE);

    $logs = array();
    foreach ($timesheet as $row) {
      $logs[] = array(
        'id' => $row['id'],
        'day' => $row['day_date'],
        'day_short' => date(HRM_DATE_DAY_NAME_DD, $row['day_timestamp']),
        'from' => date($org_time_format, $row['from_time']),
        'to' => date($org_time_format, $row['to_time']),
        'hours' => hrm_basic_time_timestamp_to_time(($row['to_time'] - $row['from_time'])),
        'description' => $row['description'],
        'project_name' => (isset($projects[$row['project']]) ? $projects[$row['project']] : ''),
        'project_id' => $row['project'],
      );
    }

    $json = new stdClass;
    $json->status = 200;
    $json->log = $logs;
    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Time Sheet';
    $json->message = 'Employee not found or blocked';
  }
  return $json;
}

function hrm_services_timesheet_add_timesheet_log($empid, $day, $from, $to, $description, $project) {
  $emp_filter = array(
    array('field' => 'empid', 'value' => $empid, 'operator' => '='),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '=')
  );
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    if (isset($employee['location']) && $employee['location']) {
      $oid = $employee['oid'];
      $location = $employee['location'];
      $emp_location = hrm_basic_location_get_data(array(array('field' => 'lid', 'value' => $location)), TRUE, TRUE);
      $timezone_offset = hrm_basic_timezone_get_offset($emp_location['timezone'], 'UTC');
      $now = time();
      $employee_time = $now + $timezone_offset;

      $employee_projects = hrm_basic_employee_get_job_projects($empid);
      if ($project && !in_array($project, $employee_projects)) {
        $json->status = 404;
        $json->message_title = 'Time Sheet';
        $json->message = 'Selected project is not assigned to you';
      }
      else {
        $day_timestamp = str_replace('/', '-', $day);
        $timesheet_id = db_insert('hrm_emp_timesheet')
          ->fields(array(
            'oid' => $oid,
            'empid' => $empid,
            'project' => $project,
            'day_date' => $day,
            'day_timestamp' => strtotime($day_timestamp . ' ' . $from),
            'from_time' => strtotime($day_timestamp . ' ' . $from),
            'to_time' => strtotime($day_timestamp . ' ' . $to),
            'description' => $description,
            'created' => $employee_time,
            'changed' => $employee_time,
          ))
          ->execute();
        if ($timesheet_id) {
          $json->status = 200;
          $json->message = 'Timesheet log has been added successfully';
          $json->log_id = $timesheet_id;
        }
        else {
          $json->status = 404;
          $json->message_title = 'Time Sheet';
          $json->message = 'An error occurred during the insert of timesheet log entry, please try again';
        }
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Time Sheet';
      $json->message = 'Your job has not been defined';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Time Sheet';
    $json->message = 'Employee not found or blocked';
  }
  return $json;
}

function hrm_services_timesheet_edit_timesheet_log($empid, $log_id, $day, $from, $to, $description, $project) {

  watchdog("project", serialize("empid is : " . $empid . " log_id is " . $log_id . " day is " . $day . " from is " . $from . " to is " . $to . " dersc is  " . $description . " project is  " . $project . " day timestampy is " . strtotime($day_timestamp . ' ' . $from)));

  $json = new stdClass;

  $emp_filter = array(
    array('field' => 'empid', 'value' => $empid, 'operator' => '='),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '=')
  );
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    if (isset($employee['location']) && $employee['location']) {
      $oid = $employee['oid'];
      $location = $employee['location'];
      $emp_location = hrm_basic_location_get_data(array(array('field' => 'lid', 'value' => $location)), TRUE, TRUE);
      $timezone_offset = hrm_basic_timezone_get_offset($emp_location['timezone'], 'UTC');
      $now = time();
      $employee_time = $now + $timezone_offset;

      $timesheet_filters = array(
        array('field' => 'id', 'value' => $log_id, 'operator' => '='),
        array('field' => 'oid', 'value' => $oid, 'operator' => '='),
        array('field' => 'empid', 'value' => $empid, 'operator' => '='),
      );
      $timesheet_log = hrm_basic_timesheet_get_data($timesheet_filters, array(), TRUE);
      if (!empty($timesheet_log)) {
        $employee_projects = hrm_basic_employee_get_job_projects($empid);

        if ($project && !in_array($project, $employee_projects)) {
          $json->status = 404;
          $json->message_title = 'Time Sheet';
          $json->message = 'Selected project is not assigned to you';
        }
        else {
          $day_timestamp = str_replace('/', '-', $day);

          watchdog("fff", serialize(strtotime($day_timestamp . ' ' . $from)));
          watchdog("aaa", serialize(strtotime($day_timestamp)));


          $affected_records = db_update('hrm_emp_timesheet')
//            ->fields(array(
//              'project' => $project,
//              'day_date' => $day,
//              'day_timestamp' => strtotime($day_timestamp . ' ' . $from),
//              'from_time' => strtotime($day_timestamp . ' ' . $from),
//              'to_time' => strtotime($day_timestamp . ' ' . $to),
//              'description' => $description,
//              'changed' => $employee_time,
//            ))
            ->fields(array(
            'project' => $project,
            'day_date' => $day,
            'day_timestamp' => strtotime($day_timestamp . ' ' . $from),
            'from_time' => strtotime($day_timestamp . ' ' . $from),
            'to_time' => strtotime($day_timestamp . ' ' . $to),
            'description' => $description,
            'changed' => $employee_time,
            ))
            ->condition('id', $log_id, '=')
            ->execute();

          if ($affected_records) {
            $json->status = 200;
            $json->message = 'Timesheet log has been edited successfully';
          }
          else {
            $json->status = 404;
            $json->message_title = 'Time Sheet';
            $json->message = 'An error occurred during the edit of timesheet log entry, please try again';
          }
        }
      }
      else {
        $json->status = 404;
        $json->message_title = 'Time Sheet';
        $json->message = 'Timesheet log does not exist';
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Time Sheet';
      $json->message = 'Your job has not been defined';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Time Sheet';
    $json->message = 'Employee not found or blocked';
  }
  return $json;
}

function hrm_services_timesheet_delete_timesheet_log($empid, $log_id) {
  $json = new stdClass;

  $emp_filter = array(
    array('field' => 'empid', 'value' => $empid, 'operator' => '='),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '=')
  );
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    $oid = $employee['oid'];

    $timesheet_filters = array(
      array('field' => 'id', 'value' => $log_id, 'operator' => '='),
      array('field' => 'oid', 'value' => $oid, 'operator' => '='),
      array('field' => 'empid', 'value' => $empid, 'operator' => '='),
    );
    $timesheet_log = hrm_basic_timesheet_get_data($timesheet_filters, array(), TRUE);

    if (!empty($timesheet_log)) {
      $deleted_records = db_delete('hrm_emp_timesheet')
        ->condition('id', $log_id, '=')
        ->execute();
      if ($deleted_records) {
        $json->status = 200;
        $json->message = 'Timesheet log has been deleted successfully';
      }
      else {
        $json->status = 404;
        $json->message_title = 'Time Sheet';
        $json->message = 'An error occurred during the delete of timesheet log entry, please try again';
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Time Sheet';
      $json->message = 'Timesheet log does not exist';
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Time Sheet';
    $json->message = 'Employee not found or blocked';
  }
  return $json;
}
