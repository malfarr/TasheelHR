<?php

/*
 * Assets
 */

function hr_backend_dashboard_page() {
  global $user;
  $account = hr_basic_account_get_info($user->uid);
  $org_id = $account['organization'];

  $employees = db_select('hr_employee', 'hr_employee')
          ->fields('hr_employee', array('emp_id', 'org_id', 'status'))
          ->condition('org_id', $org_id, '=')
          ->condition('status', HR_EMPLOYEE_STATUS_ACTIVE, '=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $locations = db_select('hr_location', 'hr_location')
          ->fields('hr_location', array('loc_id', 'org_id'))
          ->condition('org_id', $org_id, '=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $qualifications = db_select('hr_qualification', 'hr_qualification')
          ->fields('hr_qualification', array('id', 'org_id'))
          ->condition('org_id', $org_id, '=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $last_week_timestamp = strtotime("-1 week");
  $attendance = db_select('hr_attendance', 'hr_attendance')
          ->fields('hr_attendance', array('id', 'org_id'))
          ->condition('org_id', $org_id, '=')
          ->condition('day_timestamp', $last_week_timestamp, '>=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $leaves = db_select('hr_employee_leave', 'hr_employee_leave')
          ->fields('hr_employee_leave', array('leave_id', 'org_id', 'status'))
          ->condition('org_id', $org_id, '=')
          ->condition('status', HR_LEAVE_STATUS_TAKEN, '>=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $assets = db_select('hr_asset', 'hr_asset')
          ->fields('hr_asset', array('asset_id', 'org_id'))
          ->condition('org_id', $org_id, '=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $announcements = db_select('hr_announcement', 'hr_announcement')
          ->fields('hr_announcement', array('id', 'org_id'))
          ->condition('org_id', $org_id, '=')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $variables = array(
    'org_id' => $org_id,
    'employees' => count($employees),
    'locations' => count($locations),
    'qualification' => count($qualifications),
    'attendance' => count($attendance),
    'leaves' => count($leaves),
    'assets' => count($assets),
    'announcements' => count($announcements),
    'organization_structure' => 1,
  );
  if (!user_access(HR_PERM_PIM_ACCESS_EMPLOYEE_LIST)) {
    unset($variables['employees']);
  }
  if (!user_access(HR_PERM_LOCATION_ACCESS_LOCATIONS)) {
    unset($variables['locations']);
  }
  if (!user_access(HR_PERM_CONFIG_QUALIFICATION_ACCESS_QUALIFICATIONS)) {
    unset($variables['qualification']);
  }
  if (!user_access(HR_PERM_ATTENDANCE_ACCESS_EMPLOYEE_ATTENDANCE_RECORD)) {
    unset($variables['attendance']);
  }
  if (!user_access(HR_PERM_LEAVE_ACCESS_LEAVE_LIST)) {
    unset($variables['leaves']);
  }
  if (!user_access(HR_PERM_ASSET_ACCESS_ASSETS)) {
    unset($variables['assets']);
  }
  if (!user_access(HR_PERM_ANNOUNCEMENT_ACCESS_ANNOUNCEMENTS)) {
    unset($variables['announcements']);
  }
  if (!user_access(HR_PERM_CONFIG_ORGANIZATION_STRUCTURE_ACCESS_ORGANIZATION_STRUCTURE)) {
    unset($variables['organization_structure']);
  }


  if (user_access(HR_PERM_ATTENDANCE_MANAGE_ATTENDANCE)) {
    $attendance_dashboard = hr_backend_dashboard_attendance_section_content($org_id);

    $variables['dashboard_attendance_in'] = $attendance_dashboard['in'];
    $variables['dashboard_attendance_out'] = $attendance_dashboard['out'];
  }

  if (user_access(HR_PERM_LEAVE_ACCESS_LEAVE_LIST)) {
    $variables['dashboard_leaves'] = hr_backend_dashboard_leave_calendar_page($org_id);
  }
  if (user_access(HR_PERM_ASSET_ACCESS_ASSETS)) {
    $variables['dashboard_assets'] = hr_backend_dashboard_assets_section_content($org_id);
  }
  return theme('page_wrapper_dashboard', $variables);
}

function hr_backend_dashboard_attendance_section_content($org_id) {
  $content = array();

  $day = date(HR_DATETIME_FORMAT_SHORT_DASH, time());
  $day_starts = strtotime($day . ' 00:00:01');
  $day_ends = strtotime($day . ' 23:59:59');

  $organization_filters = array(array('field' => 'org_id', 'value' => $org_id, 'operator' => '='));
  $employee_filters = array(
    array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
    array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '='),
    array('field' => 'attendance_applicable', 'value' => HR_EMPLOYEE_ATTENDANCE_APPLICABLE, 'operator' => '='),
  );
  $employees = hr_basic_employee_get_employees_info($employee_filters);
  $emp_ids = array_keys($employees);
  $absent_emp_ids = array_combine($emp_ids, $emp_ids);

  $workshifts = hr_basic_work_shift_get_basic_info($organization_filters);
  $override_work_shift_id = variable_get(HR_VAR_WORK_SHIFT_OVERRIDE, 0);
  $default_workshift_id = 0;
  $default_workshift_starts = 0;
  $default_workshift_ends = 0;
  
  foreach ($workshifts as $workshift) {
    if ($workshift['active']) {
      $default_workshift_id = $workshift['id'];
      $default_workshift_starts = $workshift['today_from_time'];
      $default_workshift_ends = $workshift['today_from_time'];
    }
  }
  if($override_work_shift_id){
    $default_workshift_id = $workshifts[$override_work_shift_id]['id'];
    $default_workshift_starts = $workshifts[$override_work_shift_id]['today_from_time'];
    $default_workshift_ends = $workshifts[$override_work_shift_id]['today_from_time'];
  }
  
  $attendance_data = array(
    'total' => 0,
    'early_in' => array(
      'total' => 0,
      'employees' => array(),
    ),
    'late_in' => array(
      'total' => 0,
      'employees' => array(),
    ),
    'leave' => array(
      'total' => 0,
      'employees' => array(),
    ),
    'absent' => array(
      'total' => 0,
      'employees' => array(),
    ),
    'attendees' => array(),
    'absents' => array(),
  );


  if (!empty($emp_ids)) {
    $attendance_in_result = db_select('hr_attendance', 'attendance')
            ->fields('attendance', array('id', 'emp_id', 'day_date', 'punch_in_time', 'punch_in_lat', 'punch_in_long'))
            ->condition('emp_id', $emp_ids, 'IN')
            ->condition('day_date', $day, '=')
            ->orderBy('punch_in_time', 'ASC')
            ->groupBy('emp_id')
            ->groupBy('day_date')
            ->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($attendance_in_result as $row) {
      $emp_id = $row['emp_id'];
      $attendance_data['total'] += 1;
      $attendance_data['attendees'][$emp_id] = $employees[$emp_id];
      unset($absent_emp_ids[$emp_id]);

      $emp_workshift = ((isset($employees[$emp_id]['job']['work_shift']) && $employees[$emp_id]['job']['work_shift']) ? $employees[$emp_id]['job']['work_shift'] : $default_workshift_id);
      if($override_work_shift_id){
        $emp_workshift = $override_work_shift_id;
      }
      
      $workshift_starts = $workshifts[$emp_workshift]['today_from_time'] + 59;      
      if ($row['punch_in_time'] > $workshift_starts) {
        $attendance_data['late_in']['total'] += 1;
        $attendance_data['late_in']['employees'][$emp_id] = $employees[$emp_id];
        $attendance_data['late_in']['employees'][$emp_id]['punch_in'] = $row['punch_in_time'];
      }
      else {
        $attendance_data['early_in']['total'] += 1;
        $attendance_data['early_in']['employees'][$emp_id] = $employees[$emp_id];
        $attendance_data['early_in']['employees'][$emp_id]['punch_in'] = $row['punch_in_time'];
      }
    }

    if (!empty($absent_emp_ids)) {
      $absent_leaves_query = db_select('hr_employee_leave_timing', 'hr_employee_leave_timing');
      $absent_leaves_query->join('hr_employee_leave', 'hr_employee_leave', 'hr_employee_leave_timing.leave_id = hr_employee_leave.leave_id');
      $absent_leaves_query->fields('hr_employee_leave', array('leave_type', 'emp_id'));
      $absent_leaves_query->fields('hr_employee_leave_timing', array('leave_id', 'hours'));
      $absent_leaves_query->condition('hr_employee_leave.emp_id', $absent_emp_ids, 'IN');
      $absent_leaves_query->condition('hr_employee_leave.status', array(HR_LEAVE_STATUS_SCHEDULED, HR_LEAVE_STATUS_TAKEN), 'IN');
      $absent_leaves_query->condition('from_time', $day_starts, '>=');
      $absent_leaves_query->condition('to_time', $day_ends, '<=');
      $absent_leaves = $absent_leaves_query->execute()->fetchAll(PDO::FETCH_ASSOC);

      if (!empty($absent_leaves)) {
        foreach ($absent_leaves as $absent_leave) {
          if (!isset($attendance_data['leave']['employees'][$absent_leave['emp_id']])) {
            $attendance_data['total'] += 1;
            $attendance_data['leave']['total'] += 1;
            $attendance_data['leave']['employees'][$absent_leave['emp_id']] = $employees[$absent_leave['emp_id']];
            $attendance_data['leave']['employees'][$absent_leave['emp_id']]['punch_in'] = 0;
            unset($absent_emp_ids[$absent_leave['emp_id']]);
          }
          $attendance_data['leave']['employees'][$absent_leave['emp_id']]['punch_in'] += ($absent_leave['hours'] * 60 * 60);
        }
      }
      if (!empty($absent_emp_ids)) {
        foreach ($absent_emp_ids as $absent_emp_id) {
          $attendance_data['total'] += 1;
          $attendance_data['absent']['total'] += 1;
          $attendance_data['absent']['employees'][$absent_emp_id] = $employees[$absent_emp_id];
          $attendance_data['absent']['employees'][$absent_emp_id]['punch_in'] = 0;
        }
      }
    }
  }

  $early_in_perecentage = (!$attendance_data['early_in']['total'] ? 0 : (($attendance_data['early_in']['total'] / $attendance_data['total']) * 100));
  $late_in_perecentage = (!$attendance_data['late_in']['total'] ? 0 : (($attendance_data['late_in']['total'] / $attendance_data['total']) * 100));
  $leave_in_perecentage = (!$attendance_data['leave']['total'] ? 0 : (($attendance_data['leave']['total'] / $attendance_data['total']) * 100));
  $absent_perecentage = (!$attendance_data['absent']['total'] ? 0 : (($attendance_data['absent']['total'] / $attendance_data['total']) * 100));

  $series_data = array(
    array(
      'name' => 'Check in',
      'data' => array(
        array('name' => 'Early In', 'y' => $early_in_perecentage, 'count' => $attendance_data['early_in']['total'], 'color' => '#46c37b'),
        array('name' => 'Late In', 'y' => $late_in_perecentage, 'count' => $attendance_data['late_in']['total'], 'color' => '#d26a5c'),
        array('name' => 'On Leave', 'y' => $leave_in_perecentage, 'count' => $attendance_data['leave']['total'], 'color' => '#f3b760'),
        array('name' => 'Absent', 'y' => $absent_perecentage, 'count' => $attendance_data['absent']['total'], 'color' => '#5c90d2'),
      )
    ),
  );
  $chart_data = array(
    'container_id' => 'check-in-chart',
    'container_class' => array('check-in-chart'),
    'title' => 'Attendance - Check In ',
    'subtitle' => '',
    'series_data' => $series_data,
  );

  $content['in']['chart'] = hr_backend_highcharts_pie_chart($chart_data);

  $header = array(
    array('data' => 'Employee'),
    array('data' => 'IN')
  );

 $list = array(
  'attributes' => array('class' => array('table-hover', 'table-vcenter')),
  'rows' => $rows,
  'header' => $header,
  'footer' => $footer,
  );
  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

  $early_in_rows = array();
  foreach ($attendance_data['early_in']['employees'] as $employee) {
    $name = theme('image', array('path' => $employee['picture_url'], 'attributes' => array('class' => array('img-avatar', 'img-avatar24', 'margin-5-r'))));
    $name .= $employee['name'];
    $link = l($name, 'pim/employee/details/' . $employee['emp_id'], array('html' => TRUE));
    $early_in_rows[]['data'] = array(
      array('data' => $link, 'class' => array()),
      array('data' => date(HR_DATETIME_FORMAT_HOUR_12, $employee['punch_in']), 'class' => array()),
    );
  }
  $list['rows'] = $early_in_rows;
  $content['in']['tables']['early_in'] = $list;

  $late_in_rows = array();
  foreach ($attendance_data['late_in']['employees'] as $employee) {
    $name = theme('image', array('path' => $employee['picture_url'], 'attributes' => array('class' => array('img-avatar', 'img-avatar24', 'margin-5-r'))));
    $name .= $employee['name'];
    $link = l($name, 'pim/employee/details/' . $employee['emp_id'], array('html' => TRUE));
    $late_in_rows[]['data'] = array(
      array('data' => $link, 'class' => array()),
      array('data' => date(HR_DATETIME_FORMAT_HOUR_12, $employee['punch_in']), 'class' => array()),
    );
  }
  $list['rows'] = $late_in_rows;
  $content['in']['tables']['late_in'] = $list;

  $leave_rows = array();
  foreach ($attendance_data['leave']['employees'] as $employee) {
    $name = theme('image', array('path' => $employee['picture_url'], 'attributes' => array('class' => array('img-avatar', 'img-avatar24', 'margin-5-r'))));
    $name .= $employee['name'];
    $link = l($name, 'pim/employee/details/' . $employee['emp_id'], array('html' => TRUE));
    $leave_rows[]['data'] = array(
      array('data' => $link, 'class' => array()),
      array('data' => hr_basic_time_format_time($employee['punch_in']), 'class' => array()),
    );
  }
  $list['rows'] = $leave_rows;
  $list['header'][1]['data'] = 'Hours on leave';
  $content['in']['tables']['leave'] = $list;

  $absent_rows = array();
  foreach ($attendance_data['absent']['employees'] as $employee) {
    $name = theme('image', array('path' => $employee['picture_url'], 'attributes' => array('class' => array('img-avatar', 'img-avatar24', 'margin-5-r'))));
    $name .= $employee['name'];
    $link = l($name, 'pim/employee/details/' . $employee['emp_id'], array('html' => TRUE));
    $absent_rows[]['data'] = array(
      array('data' => $link, 'class' => array()),
      array('data' => '00:00', 'class' => array()),
    );
  }
  $list['rows'] = $absent_rows;
  $list['header'][1]['data'] = 'IN';
  $content['in']['tables']['absent'] = $list;


  /*
   * Attendance Out
   */
  $absent_out_emp_ids = array_combine($emp_ids, $emp_ids);
  $attendance_out_data = array(
    'total' => 0,
    'early_out' => array(
      'total' => 0,
      'employees' => array(),
    ),
    'late_out' => array(
      'total' => 0,
      'employees' => array(),
    ),
    'leave' => array(
      'total' => 0,
      'employees' => array(),
    ),
    'absent' => array(
      'total' => 0,
      'employees' => array(),
    ),
  );
  if (!empty($emp_ids)) {
    $attendance_out_result = db_select('hr_attendance', 'attendance')
            ->fields('attendance', array('emp_id', 'day_date', 'punch_out_time', 'punch_out_lat', 'punch_out_long'))
            ->condition('emp_id', $emp_ids, 'IN')
            ->condition('day_date', $day, '=')
            ->condition('punch_out_time', 0, '<>')
            ->orderBy('punch_out_time', 'DESC')
            ->groupBy('emp_id')
            ->groupBy('day_date')
            ->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($attendance_out_result as $row) {
      $emp_id = $row['emp_id'];
      $attendance_out_data['total'] += 1;
      unset($absent_out_emp_ids[$emp_id]);

      $emp_workshift = ((isset($employees[$emp_id]['job']['work_shift']) && $employees[$emp_id]['job']['work_shift']) ? $employees[$emp_id]['job']['work_shift'] : 0);      
      if($override_work_shift_id){
        $emp_workshift = $override_work_shift_id;
      }
      if ($emp_workshift) {
        $workshift_ends = $workshifts[$emp_workshift]['today_to_time'] + 59;
        if ($row['punch_out_time'] > $workshift_ends) {
          $attendance_out_data['late_out']['total'] += 1;
          $attendance_out_data['late_out']['employees'][$emp_id] = $employees[$emp_id];
          $attendance_out_data['late_out']['employees'][$emp_id]['punch_out'] = $row['punch_out_time'];
        }
        else {
          $attendance_out_data['early_out']['total'] += 1;
          $attendance_out_data['early_out']['employees'][$emp_id] = $employees[$emp_id];
          $attendance_out_data['early_out']['employees'][$emp_id]['punch_out'] = $row['punch_out_time'];
        }
      }
      else {
        $attendance_out_data['unknown']['total'] += 1;
        $attendance_out_data['unknown']['employees'][$emp_id] = $employees[$emp_id];
        $attendance_out_data['unknown']['employees'][$emp_id]['punch_out'] = $row['punch_out_time'];
      }
    }

    
    if (!empty($absent_out_emp_ids)) {
      $absent_leaves_query = db_select('hr_employee_leave_timing', 'hr_employee_leave_timing');
      $absent_leaves_query->join('hr_employee_leave', 'hr_employee_leave', 'hr_employee_leave_timing.leave_id = hr_employee_leave.leave_id');
      $absent_leaves_query->fields('hr_employee_leave', array('leave_type', 'emp_id'));
      $absent_leaves_query->fields('hr_employee_leave_timing', array('leave_id', 'hours'));
      $absent_leaves_query->condition('hr_employee_leave.emp_id', $absent_out_emp_ids, 'IN');
      $absent_leaves_query->condition('hr_employee_leave.status', array(HR_LEAVE_STATUS_SCHEDULED, HR_LEAVE_STATUS_TAKEN), 'IN');
      $absent_leaves_query->condition('from_time', $day_starts, '>=');
      $absent_leaves_query->condition('to_time', $day_ends, '<=');
      $absent_leaves = $absent_leaves_query->execute()->fetchAll(PDO::FETCH_ASSOC);

      if (!empty($absent_leaves)) {
        foreach ($absent_leaves as $absent_leave) {
          if (!isset($attendance_out_data['leave']['employees'][$absent_leave['emp_id']])) {
            $attendance_out_data['total'] += 1;
            $attendance_out_data['leave']['total'] += 1;
            $attendance_out_data['leave']['employees'][$absent_leave['emp_id']] = $employees[$absent_leave['emp_id']];
            $attendance_out_data['leave']['employees'][$absent_leave['emp_id']]['punch_out'] = 0;
            unset($absent_out_emp_ids[$absent_leave['emp_id']]);
          }
          $attendance_out_data['leave']['employees'][$absent_leave['emp_id']]['punch_out'] += ($absent_leave['hours'] * 60 * 60);
        }
      }
      
      if (!empty($absent_out_emp_ids)) {
        foreach ($absent_out_emp_ids as $absent_out_emp_id) {
          $attendance_out_data['total'] += 1;
          $attendance_out_data['absent']['total'] += 1;
          $attendance_out_data['absent']['employees'][$absent_out_emp_id] = $employees[$absent_out_emp_id];
          $attendance_out_data['absent']['employees'][$absent_out_emp_id]['punch_out'] = 0;
        }
      }
    }
  }

  $early_out_perecentage = (!$attendance_out_data['early_out']['total'] ? 0 : (($attendance_out_data['early_out']['total'] / $attendance_out_data['total']) * 100));
  $late_out_perecentage = (!$attendance_out_data['late_out']['total'] ? 0 : (($attendance_out_data['late_out']['total'] / $attendance_out_data['total']) * 100));
  $leave_out_perecentage = (!$attendance_out_data['leave']['total'] ? 0 : (($attendance_out_data['leave']['total'] / $attendance_out_data['total']) * 100));
  $absent_out_perecentage = (!$attendance_out_data['absent']['total'] ? 0 : (($attendance_out_data['absent']['total'] / $attendance_out_data['total']) * 100));

  $series_out_data = array(
    array(
      'name' => 'Check out',
      'data' => array(
        array('name' => 'Late Out', 'y' => $late_out_perecentage, 'count' => $attendance_out_data['early_out']['total'], 'color' => '#46c37b'),
        array('name' => 'Early Out', 'y' => $early_out_perecentage, 'count' => $attendance_out_data['late_out']['total'], 'color' => '#d26a5c'),
        array('name' => 'On leave', 'y' => $leave_out_perecentage, 'count' => $attendance_out_data['leave']['total'], 'color' => '#f3b760'),
        array('name' => 'Absent', 'y' => $absent_out_perecentage, 'count' => $attendance_out_data['absent']['total'], 'color' => '#5c90d2'),
      )
    ),
  );
  $chart_out_data = array(
    'container_id' => 'check-out-chart',
    'container_class' => array('check-out-chart'),
    'title' => 'Attendance - Check Out ',
    'subtitle' => '',
    'series_data' => $series_out_data,
  );

  $content['out']['chart'] = hr_backend_highcharts_pie_chart($chart_out_data);

  $out_header = array(
    array('data' => 'Employee'),
    array('data' => 'Out')
  );
  $out_list = array(
    'attributes' => array('class' => array('table', 'table-hover')),
    'rows' => array(),
    'header' => $out_header
  );

  $late_out_rows = array();
  foreach ($attendance_out_data['late_out']['employees'] as $employee) {
    $name = theme('image', array('path' => $employee['picture_url'], 'attributes' => array('class' => array('img-avatar', 'img-avatar24', 'margin-5-r'))));
    $name .= $employee['name'];
    $link = l($name, 'pim/employee/details/' . $employee['emp_id'], array('html' => TRUE));
    $late_out_rows[]['data'] = array(
      array('data' => $link, 'class' => array()),
      array('data' => date(HR_DATETIME_FORMAT_HOUR_12, $employee['punch_out']), 'class' => array()),
    );
  }
  $out_list['rows'] = $late_out_rows;
  $content['out']['tables']['late_out'] = $out_list;

  $early_out_rows = array();
  foreach ($attendance_out_data['early_out']['employees'] as $employee) {
    $name = theme('image', array('path' => $employee['picture_url'], 'attributes' => array('class' => array('img-avatar', 'img-avatar24', 'margin-5-r'))));
    $name .= $employee['name'];
    $link = l($name, 'pim/employee/details/' . $employee['emp_id'], array('html' => TRUE));
    $early_out_rows[]['data'] = array(
      array('data' => $link, 'class' => array()),
      array('data' => date(HR_DATETIME_FORMAT_HOUR_12, $employee['punch_out']), 'class' => array()),
    );
  }
  $out_list['rows'] = $early_out_rows;
  $content['out']['tables']['early_out'] = $out_list;

  $leave_out_rows = array();
  foreach ($attendance_out_data['leave']['employees'] as $employee) {
    $name = theme('image', array('path' => $employee['picture_url'], 'attributes' => array('class' => array('img-avatar', 'img-avatar24', 'margin-5-r'))));
    $name .= $employee['name'];
    $link = l($name, 'pim/employee/details/' . $employee['emp_id'], array('html' => TRUE));
    $leave_out_rows[]['data'] = array(
      array('data' => $link, 'class' => array()),
      array('data' => hr_basic_time_format_time($employee['punch_out']), 'class' => array()),
    );
  }
  $out_list['rows'] = $leave_out_rows;
  $out_list['header'][1]['data'] = 'Hours on leave';
  $content['out']['tables']['leave'] = $out_list;

  $absent_out_rows = array();
  foreach ($attendance_out_data['absent']['employees'] as $employee) {
    $name = theme('image', array('path' => $employee['picture_url'], 'attributes' => array('class' => array('img-avatar', 'img-avatar24', 'margin-5-r'))));
    $name .= $employee['name'];
    $link = l($name, 'pim/employee/details/' . $employee['emp_id'], array('html' => TRUE));
    $absent_out_rows[]['data'] = array(
      array('data' => $link, 'class' => array()),
      array('data' => '00:00', 'class' => array()),
    );
  }
  $out_list['rows'] = $absent_out_rows;
  $out_list['header'][1]['data'] = 'Out';
  $content['out']['tables']['absent'] = $out_list;

  return $content;
}

function hr_backend_dashboard_leaves_section_content($org_id) {

  $organization_filters = array(array('field' => 'org_id', 'value' => $org_id, 'operator' => '='));

  $first_month_day = date('Y-m-01 00:01');
  $last_month_day = date('Y-m-t 23:59');
  $from_time = strtotime($first_month_day);
  $to_time = strtotime($last_month_day);

  $leave_filters = array(
    array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
    array('field' => 'from_date', 'value' => $from_time, 'operator' => '>='),
    array('field' => 'to_date', 'value' => $to_time, 'operator' => '<=')
  );
  $leaves_sort = array(array('field' => 'from_date', 'direction' => 'DESC'));
  $leaves = hr_basic_leave_get_leave_basic_information($leave_filters, $leaves_sort);
  $leave_ids = array_keys($leaves);
  $leave_ids[] = 0;

  $leaves_attachments_query = db_select('hr_attachment', 'hr_attachment')
      ->fields('hr_attachment', array('entity_id'));
  $leaves_attachments_query->addExpression('COUNT(*)', 'counter');
  $leaves_attachments_query->condition('entity', HRM_ENTITY_LEAVE, '=');
  $leaves_attachments_query->condition('entity_id', $leave_ids, 'IN');
  $leaves_attachments_query->groupBy('entity_id');
  $leaves_attachments = $leaves_attachments_query->execute()->fetchAllKeyed(0, 1);

  $leave_types = hr_basic_config_leave_type_get_basic_info($organization_filters);
  $leave_status = hr_basic_leave_status_get_options();

  $emp_ids = db_select('hr_employee_leave', 'employee_leave')
          ->distinct()
          ->fields('employee_leave', array('emp_id'))
          ->condition('leave_id', $leave_ids, 'IN')
          ->condition('org_id', $org_id, '=')
          ->execute()->fetchCol();
  $emp_ids[] = 0;
  $employee_filters = array(
    array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '='),
    array('field' => 'emp_id', 'value' => $emp_ids, 'operator' => 'IN')
  );
  $employees = hr_basic_employee_get_employees_info($employee_filters);

  $access_leave_details_permission = user_access(HR_PERM_LEAVE_ACCESS_LEAVE_DETAILS);
  $rows = array();

  foreach ($leaves as $leave_id => $leave) {
    if (!isset($employees[$leave['emp_id']])) {
      continue;
    }
    $from_date = date(HR_DATETIME_FORMAT_SHORT_SPACE_MONTH_SHORT_NAME, $leave['from_date']);
    $to_date = date(HR_DATETIME_FORMAT_SHORT_SPACE_MONTH_SHORT_NAME, $leave['to_date']);
    if ($from_date == $to_date) {
      $from_date = date(HR_DATETIME_FORMAT_LONG_SPACE_MONTH_SHORT_NAME, $leave['from_date']);
      $to_date = date(HR_DATETIME_FORMAT_LONG_SPACE_MONTH_SHORT_NAME, $leave['to_date']);
    }

    $warning_content = '';
    $row_class = '';
    if ($leave['status'] == HR_LEAVE_STATUS_TAKEN &&
        $leave_types[$leave['leave_type']]['attachments'] == HR_OPTION_ANSWER_YES &&
        !isset($leaves_attachments[$leave_id])) {
      $warning_content = '<i class="fa fa-warning text-danger" title="Attachments is required"></i>';
      $row_class = 'danger';
    }

    $leave_details = '';
    if ($access_leave_details_permission) {
      $leave_details = l('<i class="fa fa-search"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_LEAVE_DETAILS, 'leave_id' => $leave['leave_id']),
        'attributes' => array('class' => array('btn', 'btn-xs', 'btn-primary')), 'html' => TRUE));
    }

    $rows[] = array(
      'data' => array(
        'link' => array('data' => $leave_details, 'class' => array('text-center')),
        'picture' => array('data' => $employees[$leave['emp_id']]['picture_image'], 'class' => array('text-center')),
        'name' => array('data' => $employees[$leave['emp_id']]['name'], 'class' => array()),
        'from' => array('data' => $from_date, 'class' => array()),
        'to' => array('data' => $to_date, 'class' => array()),
        'type' => array('data' => $leave_types[$leave['leave_type']]['name'], 'class' => array()),
        'status' => array('data' => $leave_status[$leave['status']] . ' ' . $warning_content, 'class' => array()),
        'days' => array('data' => number_format($leave['days'], 2), 'class' => array()),
        'hours' => array('data' => $leave['hours_formatted'], 'class' => array()),
        'created' => array('data' => date(HR_DATETIME_FORMAT_LONG_SPACE_MONTH_SHORT_NAME, $leave['created']), 'class' => array()),
      ),
      'class' => array($row_class),
    );
  }

  $header = array(
    array('data' => '<i class="fa fa-link"></i>', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => '<i class="fa fa-user-circle-o"></i>', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Employee'),
    array('data' => 'From'),
    array('data' => 'To'),
    array('data' => 'Leave type'),
    array('data' => 'Status'),
    array('data' => 'Days'),
    array('data' => 'Hours'),
    array('data' => 'Applied on'),
  );


  $list = array(
    'attributes' => array('class' => array('table', 'table-hover')),
    'rows' => $rows,
    'header' => $header
  );

  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }

  $content = theme('table', $list);

  return $content;
}

function hr_backend_dashboard_assets_section_content($org_id) {
  $filters = array(
    array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
    array('field' => 'status', 'value' => HR_TERM_ASSET_STATUS_ASSIGNED, 'operator' => '='),
  );
  $assets = hr_basic_asset_get_asset_basic_information($filters);

  $status = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_ASSET_STATUS);
  $categories = hr_basic_asset_category_get_categories($org_id);
  $categories[0] = '';
  $brands = hr_basic_asset_brand_get_brands($org_id);
  $brands[0] = '';
  $employees = array();

  $rows = array();
  foreach ($assets as $asset_id => $asset) {
    $assignment = $asset['assignment'];
    $assigned_to_emp_id = $assignment['emp_id'];
    if (!isset($employees[$assigned_to_emp_id])) {
      $employees[$assigned_to_emp_id] = hr_basic_employee_get_name($assigned_to_emp_id);
    }
    $assign_to = $employees[$assigned_to_emp_id];
    $warning_content = '';
    if (!$assignment['receipt_fid'] || !$assignment['photo_fid']) {
      $warning_content = '<i class="fa fa-warning text-danger animated bounceIn" title="Signed Reciept Form or/and assignment photo is not uploded"></i>';
      $rows[$asset_id] = array(
        'data' => array(
          'warning' => array('data' => $warning_content, 'class' => array('text-center')),
          'asset_no' => array('data' => l($asset['asset_no'], 'asset/asset/details/' . $asset_id)),
          'asset_serial' => array('data' => l($asset['serial'], 'asset/asset/details/' . $asset_id)),
          'category' => array('data' => (isset($categories[$asset['category']]) ? $categories[$asset['category']] : '')),
          'brand' => array('data' => (isset($brands[$asset['brand']]) ? $brands[$asset['brand']] : '')),
          'model' => array('data' => $asset['model']),
          'status' => array('data' => $status[$asset['status']]),
          'assigned_to' => array('data' => $assign_to), //Assign to          
        ),
        'class' => array()
      );
    }
  }

  $content = '<p>All the assigned assets have been completed</p>';
  if (!empty($rows)) {
    $header = array(
      array('data' => '<i class="fa fa-warning"></i>', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
      array('data' => 'Asset ID'),
      array('data' => 'Serial Number'),
      array('data' => 'Category'),
      array('data' => 'Brand'),
      array('data' => 'Model'),
      array('data' => 'Status'),
      array('data' => 'Assigned To'),
    );
    $list = array(
      'attributes' => array('class' => array('table', 'table-hover', 'datatable-all')),
      'rows' => $rows,
      'header' => $header
    );
    $content = theme('table', $list);
  }

  return $content;
}


/*
 * Leave Calendar
 */
function hr_backend_dashboard_leave_calendar_page() {
  global $user;
  $account = hr_basic_account_get_info($user->uid);

  $calendar_content = hr_backend_dashboard_leave_calendar_get_content(array('account' => $account));
  $leave_calendar_variables = array(
    'content' => $calendar_content,
    'filter_form' => drupal_get_form('hr_backend_dashboard_leave_calendar_form', $account),
  );
  return theme('section_leave_calendar', $leave_calendar_variables);
}

function hr_backend_dashboard_leave_calendar_form($form, &$form_state, $account) {
  $org_id = $account['organization'];
  $organization_filter = array(array('field' => 'org_id', 'value' => $org_id, 'operator' => '='));

  $employees = hr_basic_employee_get_employees_names($organization_filter);
  $leave_types = hr_basic_config_leave_type_get_leave_types($organization_filter);

  $leave_status = hr_basic_leave_status_get_options_bold_abbrev();

  $form['status'] = array(
    '#title' => t('Show Leave with Status'),
    '#type' => 'checkboxes',
    '#options' => $leave_status,
    '#attributes' => array('class' => array()),
    '#required' => FALSE,
    '#prefix' => '<div class="col-sm-4 remove-padding checkbox-bigger-text">',
    '#suffix' => '</div>',
  );

  $form['employee'] = array('#title' => t('Employee'), '#type' => 'select',
    '#options' => $employees,
    '#empty_option' => 'All',
    '#empty_value' => 'All',
    '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Employee'),
    '#prefix' => '<div class="col-sm-4 remove-padding">',
    '#suffix' => '</div>',
  );

  $form['type'] = array('#title' => t('Leave type'), '#type' => 'select',
    '#options' => $leave_types,
    '#empty_option' => 'All',
    '#empty_value' => 'All', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Leave Type'),
    '#prefix' => '<div class="col-sm-4 remove-padding">',
    '#suffix' => '</div>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('View'),
    "#ajax" => array(
      'callback' => 'hr_backend_dashboard_leave_calendar_form_callback',
      'event' => 'click',
      "effect" => "fade",
    ),
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="col-sm-12 remove-padding">',
    '#suffix' => '</div>',
  );

  //$reset_link = 
  $form['#attributes']['class'][] = 'leave-list-form';

  return $form;
}

function hr_backend_dashboard_leave_calendar_form_callback($form, &$form_state) {
  $account = $form_state['build_info']['args'][0];


  $data = array(
    'account' => $account,
  );

  foreach ($form_state['values']['status'] as $status) {
    if ($status) {
      $data['status'][] = $status;
    }
  }

  if (!empty($form_state['values']['employee']) && is_numeric($form_state['values']['employee'])) {
    $data['employee'] = $form_state['values']['employee'];
  }

  if (!empty($form_state['values']['type']) && is_numeric($form_state['values']['type'])) {
    $data['type'] = $form_state['values']['type'];
  }

  $content = hr_backend_dashboard_leave_calendar_get_content($data);

  $commands[] = ajax_command_html('#leave-calendar-details', $content);

  $page = array('#type' => 'ajax', '#commands' => $commands);
  return $page;
}

function hr_backend_dashboard_leave_calendar_get_content($data) {
  global $base_url;
  
  $account = $data['account'];
  $org_id = $account['organization'];
  
  $organization_filters = array(array('field' => 'org_id', 'value' => $org_id, 'operator' => '='));
  
  $employee_filters = array(
    array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
    array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '=')
  );
  $employees = hr_basic_employee_get_employees_info($employee_filters);
  $employees[0]['name'] = '';
  $emp_ids = array_keys($employees);
  
  $leave_filters = array(
    array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
    array('field' => 'emp_id', 'value' => $emp_ids, 'operator' => 'IN')
  );
  
  $leave_types = hr_basic_config_leave_type_get_leave_types($organization_filters);

  $leave_types_colors = hr_basic_config_leave_type_get_leave_types_colors($organization_filters);

  $leave_status = hr_basic_leave_status_get_options_abbrevaition();

  if (isset($data['employee'])) {
    $leave_filters[] = array('field' => 'emp_id', 'value' => $data['employee'], 'operator' => '=');
  }

  if (isset($data['type'])) {
    $leave_filters[] = array('field' => 'leave_type', 'value' => $data['type'], 'operator' => '=');
  }
  if (isset($data['status']) && !empty($data['status'])) {
    $leave_filters[] = array('field' => 'status', 'value' => $data['status'], 'operator' => 'IN');
  }

  $leaves = hr_basic_leave_get_leave_basic_information($leave_filters);

  $events = array();
  foreach ($leaves as $leave) {
    $leave_start = 0;
    $leave_end = 0;
    if (count($leave['timing']) == 1) {
      $timing = reset($leave['timing']);
      $leave_start = $timing['from_time'];
      $leave_end = $timing['to_time'];
    }
    else {
      $first_timing = reset($leave['timing']);
      $leave_start = $first_timing['from_time'];
      $leave_end = $first_timing['to_time'];

      foreach ($leave['timing'] as $timing) {
        if ($timing['from_time'] < $leave_start) {
          $leave_start = $timing['from_time'];
        }
        if ($timing['to_time'] > $leave_end) {
          $leave_end = $timing['to_time'];
        }
      }
    }

    $title = $employees[$leave['emp_id']]['picture_image'] . ' ' . $employees[$leave['emp_id']]['name'] . '<span class ="leave-status">' . $leave_status[$leave['status']] . '</span>';
    $event = array(
      'title' => hr_basic_fix_js_string_format($title),
      'start' => $leave_start,
      'end' => $leave_end,
      'url' => $base_url . '/leave/details/' . $leave['leave_id'],
      'color' => $leave_types_colors[$leave['leave_type']],
      'class' => 'overflow-visible',
    );

    $events[] = $event;
  }

  $content = theme('section_calendar', array('events' => $events));

  return $content;
}
