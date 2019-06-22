<?php

/*
 * Employee Timesheet
 */

function hrm_backend_timesheet_employee_timesheet() {
  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }
  $filters = array(array('field' => 'oid', 'value' => $account['oid']));
  if ($account['bid']) {
    $filters[] = array('field' => 'bid', 'value' => $account['bid']);
  }
  $projects = hrm_basic_project_get_data($filters);
  $form = drupal_get_form('hrm_backend_timesheet_employee_timesheet_form', $account, $projects);
  $variables = array(
    'entity' => HRM_ENTITY_TIMESHEET,
    'title' => 'Employee Timesheet',
    'form' => $form,
  );
  return theme('page_wrapper_content-no-header', $variables);
}

function hrm_backend_timesheet_employee_timesheet_form($form, &$form_state, $account, $projects) {
  $oid = $account['oid'];
  if (!isset($form_state['storage']['employees'])) {
    $employee_filters = array(
      array('field' => 'oid', 'value' => $oid, '='),
      array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, '='),
    );
    if ($account['bid']) {
      $employee_filters[] = array('field' => 'bid', 'value' => $account['bid']);
    }
    $form_state['storage']['employees'] = hrm_basic_employee_names($employee_filters);
  }

  $form['employee'] = array(
    '#title' => 'Employee',
    '#type' => 'select',
    '#options' => $form_state['storage']['employees'],
    '#attributes' => array(
      'class' => array('select-chosen')),
    '#empty_option' => '',
    '#ajax' => array(
      'callback' => 'hrm_backend_timesheet_employee_timesheet_form_callback',
      'event' => 'change',
      'effect' => 'fade',
    ),
  );

  return $form;
}

function hrm_backend_timesheet_employee_timesheet_form_callback(&$form, &$form_state) {

  $account = $form_state['build_info']['args'][0];
  $projects = $form_state['build_info']['args'][1];
  $data = array(
    'account' => $account,
    'projects' => $projects,
    'empid' => $form_state['values']['employee'],
  );

  $content = hrm_backend_timesheet_employee_timesheet_content($data);
  $commands[] = ajax_command_html('#timesheet-container', $content);
  $page = array('#type' => 'ajax', '#commands' => $commands);
  return $page;
}

function hrm_backend_timesheet_employee_timesheet_content($data) {
  $account = $data['account'];
  $projects = $data['projects'];
  $empid = $data['empid'];
  $timesheet = hrm_basic_timesheet_get_data(array(array('field' => 'empid', 'value' => $empid)));

  $events = array();
  foreach ($timesheet as $row) {
    $description = $row['description'];
    if (isset($projects[$row['project']])) {
      $description = '<span class="font-bold">' . $projects[$row['project']]['abbrev'] . ':</span> ' . $row['description'];
    }
    $qtip_title = date(HRM_DATE_TIME_12, $row['from_time']) . ' - ' . date(HRM_DATE_TIME_12, $row['to_time']) . ' (' . hrm_basic_time_timestamp_to_time($row['to_time'] - $row['from_time']) . ')';
    $qtip_content = $description;

    $event = array(
      'title' => $description,
      'start' => $row['from_time'],
      'end' => $row['to_time'],
      'color' => (isset($projects[$row['project']]['color_code']) ? $projects[$row['project']]['color_code'] : ''),
      'class' => '',
      'qtip_title' => $qtip_title,
      'qtip_content' => $qtip_content,
    );

    $events[] = $event;
  }
  watchdog('backend -timesheet', '<pre>_events</pre>', array('_events' => serialize($events)), WATCHDOG_INFO);
  
  return theme('section_timesheet_calendar', array('events' => $events));
}

/*
 * Project Timesheet
 */

function hrm_backend_timesheet_project_timesheet() {
  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }
  $filters = array(array('field' => 'oid', 'value' => $account['oid']));
  if ($account['bid']) {
    $filters[] = array('field' => 'bid', 'value' => $account['bid']);
  }
  $projects = hrm_basic_project_get_data($filters);

  $form = drupal_get_form('hrm_backend_timesheet_project_timesheet_form', $account, $projects);
  $variables = array(
    'entity' => HRM_ENTITY_TIMESHEET,
    'title' => 'Project Timesheet',
    'form' => $form,
  );
  return theme('page_wrapper_content-no-header', $variables);
}

function hrm_backend_timesheet_project_timesheet_form($form, &$form_state, $account, $projects) {
  $oid = $account['oid'];
  if (!isset($form_state['storage']['employees'])) {
    $employee_filters = array(
      array('field' => 'oid', 'value' => $oid, '='),
    );
    if ($account['bid']) {
      $employee_filters[] = array('field' => 'bid', 'value' => $account['bid']);
    }

    $form_state['storage']['employees'] = hrm_basic_employee_names($employee_filters);
  }
  if (!isset($form_state['storage']['projects_list'])) {
    $projects_list = array();
    foreach ($projects as $id => $project) {
      $projects_list[$id] = $project['name'];
    }
    $form_state['storage']['projects_list'] = $projects_list;
  }

  $form['project'] = array(
    '#title' => 'Project',
    '#type' => 'select',
    '#options' => $form_state['storage']['projects_list'],
    '#attributes' => array(
      'class' => array(
        'select-chosen')),
    '#empty_option' => '',
    '#ajax' => array(
      'callback' => 'hrm_backend_timesheet_project_timesheet_form_callback',
      'event' => 'change',
      'effect' => 'fade',
    ),
  );
  return $form;
}

function hrm_backend_timesheet_project_timesheet_form_callback(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $projects = $form_state['build_info']['args'][1];
  $employees = $form_state['storage']['employees'];

  $data = array(
    'account' => $account,
    'employees' => $employees,
    'projects' => $projects,
    'project_id' => $form_state['values']['project'],
  );

  $content = hrm_backend_timesheet_project_timesheet_content($data);
  $commands[] = ajax_command_html('#timesheet-container', $content);
  $page = array('#type' => 'ajax', '#commands' => $commands);
  return $page;
}

function hrm_backend_timesheet_project_timesheet_content($data) {
  $account = $data['account'];
  $oid = $account['oid'];
  $projects = $data['projects'];
  $employees = $data['employees'];
  $project_id = $data['project_id'];

  $random_classes = array('bg-success', 'bg-warning', 'bg-danger', 'bg-info', 'bg-default', 'bg-City', 'bg-amethyst', 'bg-flat', 'bg-modern', 'bg-smooth');
  $timesheet_filters = array(
    array('field' => 'oid', 'value' => $oid, 'operator' => '='),
    array('field' => 'project', 'value' => $project_id, 'operator' => '=')
  );
  $timesheet = hrm_basic_timesheet_get_data($timesheet_filters);

  $employee_badges = array();

  $events = array();
  foreach ($timesheet as $row) {
    $empid = $row['empid'];
    if (!isset($employees[$empid])) {
      $employees[$empid] = hrm_basic_employee_names(array(array('field' => 'empid', 'value' => $empid, 'operator' => '=')), array(), TRUE);
    }
    $employee = $employees[$empid];

    $event_content = '<img src="' . $employee['picture_url'] . '" class="img-avatar img-avatar24 margin-5-r"/>';
    if (isset($projects[$row['project']])) {
      $event_content .= '<span class="font-bold">' . $projects[$row['project']]['abbrev'] . ':</span> ';
    }
    $event_content .= $row['description'];


    if (!isset($employee_badges[$empid])) {
      $employee_badges[$empid] = '<div class="col-sm-12 remove-padding">';
      $employee_badges[$empid] .= '<img src="' . $employee['picture_url'] . '" class="img-avatar margin-5-r pull-left"/>';
      $employee_badges[$empid] .= $employee['name'] . ' <br />' . $employee['title'];
      if (isset($employee['job']['projects']) && !empty($employee['job']['projects'])) {
        $employee_badges[$empid] .= '<br />';
        foreach ($employee['job']['projects'] as $emp_project_id) {
          $employee_badges[$empid] .= '<span class="label label-success" style="background-color:' . $projects[$emp_project_id]['color_code'] . '">' . $projects[$emp_project_id]['abbrev'] . '</span> ';
        }
      }
      $employee_badges[$empid] .= '</div>';
    }

    $qtip_title = date(HRM_DATE_TIME_12, $row['from_time']) . ' <span class="font-bold">-</span> ' . date(HRM_DATE_TIME_12, $row['to_time']) . ' (<span class="font-bold">' . hrm_basic_time_timestamp_to_time($row['to_time'] - $row['from_time']) . '</span>)';
    $qtip_content = $employee_badges[$empid];
    if (isset($projects[$row['project']])) {
      $qtip_content .= '<span class="font-bold">' . $projects[$row['project']]['abbrev'] . ':</span> ';
    }
    $qtip_content .= $row['description'];

    $event = array(
      'title' => $event_content,
      'start' => $row['from_time'],
      'end' => $row['to_time'],
      'color' => (isset($projects[$row['project']]['color_code']) ? $projects[$row['project']]['color_code'] : ''),
      'class' => $random_classes[array_rand($random_classes)],
      'qtip_title' => $qtip_title,
      'qtip_content' => $qtip_content,
    );

    $events[] = $event;
  }

  return theme('section_timesheet_calendar', array('events' => $events));
}

/*
 * Summary
 */

function hrm_backend_timesheet_summary_timesheet() {
  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $filters = array(array('field' => 'oid', 'value' => $account['oid']));
  if ($account['bid']) {
    $filters[] = array('field' => 'bid', 'value' => $account['bid']);
  }
  $projects = hrm_basic_project_get_data($filters);
  $employees = hrm_basic_employee_get_data($filters);

  $form = drupal_get_form('hrm_backend_timesheet_summary_timesheet_form', $account, $projects, $employees);
  $variables = array(
    'entity' => HRM_ENTITY_TIMESHEET,
    'title' => 'Timesheet Summary',
    'form' => $form,
  );
  return theme('page_wrapper_content-no-header', $variables);
}

function hrm_backend_timesheet_summary_timesheet_form($form, &$form_state, $account, $projects, $employees) {
  $oid = $account['oid'];
  if (!isset($form_state['storage']['employees_list'])) {
    foreach ($employees as $empid => $employee) {
      $form_state['storage']['employees_list'][$empid] = $employee['name'];
    }
  }

  $form['from'] = array(
    '#title' => 'From',
    '#type' => 'textfield',
    '#required' => FALSE,
    '#empty_option' => ''
    , '#attributes' => array(
      'class' => array('custom-datepicker'),
      'data-date-format' => 'dd/mm/yyyy',
      'placeholder' => 'dd/mm/yyyy'),
  );
  $form['to'] = array(
    '#title' => 'To',
    '#type' => 'textfield',
    '#required' => FALSE,
    '#empty_option' => '',
    '#attributes' => array(
      'class' => array('custom-datepicker'),
      'data-date-format' => 'dd/mm/yyyy',
      'placeholder' => 'dd/mm/yyyy'),
  );
  $form['employee'] = array(
    '#title' => 'Employees',
    '#type' => 'select',
    '#options' => $form_state['storage']['employees_list'],
    '#empty_option' => 'All',
    '#multiple' => TRUE,
    '#empty_value' => 0,
    '#attributes' => array('class' => array('select-chosen')),
  );

  if (!isset($form_state['storage']['projects_list'])) {
    foreach ($projects as $id => $project) {
      $form_state['storage']['projects_list'][$id] = $project['name'];
    }
  }
  $form['project'] = array(
    '#title' => 'Project',
    '#type' => 'select',
    '#options' => $form_state['storage']['projects_list'],
    '#multiple' => TRUE,
    '#attributes' => array('class' => array('select-chosen')),
    '#empty_option' => 'All',
    '#empty_value' => 'All',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'View',
    "#ajax" => array(
      'callback' => 'hrm_backend_timesheet_summary_timesheet_form_callback',
      'event' => 'click',
      "effect" => "fade",
    ),
    '#attributes' => array('class' => array('btn-primary', 'btn-sm'))
  );

  return $form;
}

function hrm_backend_timesheet_summary_timesheet_form_callback(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $projects = $form_state['build_info']['args'][1];
  $employees = $form_state['build_info']['args'][2];

  $oid = $account['oid'];

  $filters = array(
    array('field' => 'oid', 'value' => $oid, 'operator' => '=')
  );
  if (!empty($form_state['values']['from'])) {
    $from_time = strtotime(str_replace('/', '-', $form_state['values']['from']) . ' 00:01');
    $filters[] = array('field' => 'from_time', 'value' => $from_time, 'operator' => '>=');
  }
  if (!empty($form_state['values']['to'])) {
    $to_time = strtotime(str_replace('/', '-', $form_state['values']['to']) . ' 23:59');
    $filters['to_time'] = array('field' => 'to_time', 'value' => $to_time, 'operator' => '<=');
  }
  if (!empty($form_state['values']['employee'])) {
    $empids = array();
    foreach ($form_state['values']['employee'] as $value) {
      if ($value) {
        $empids[] = $value;
      }
    }
    if (!empty($empids)) {
      $filters['employee'] = array('field' => 'empid', 'value' => $empids, 'operator' => 'IN');
    }
  }
  if (!empty($form_state['values']['project'])) {
    $project_ids = array();
    foreach ($form_state['values']['project'] as $value) {
      if ($value) {
        $project_ids[] = $value;
      }
    }
    if (!empty($project_ids)) {
      $filters['project'] = array('field' => 'project', 'value' => $project_ids, 'operator' => 'IN');
    }
  }

  $data = array(
    'account' => $account,
    'employees' => $employees,
    'projects' => $projects,
    'filters' => $filters
  );

  $content = hrm_backend_timesheet_summary_timesheet_content($data);
  $commands[] = ajax_command_html('#timesheet-container', $content);
  $page = array('#type' => 'ajax', '#commands' => $commands);
  return $page;
}

function hrm_backend_timesheet_summary_timesheet_content($data) {

  $account = $data['account'];
  $oid = $account['oid'];
  $projects = $data['projects'];
  $employees = $data['employees'];
  $filters = $data['filters'];

  $selected_projects = array();
  if (isset($filters['project'])) {
    $selected_projects = $filters['project']['value'];
  }
  else {
    $selected_projects = array_keys($projects);
  }

  $selected_employees = array();
  if (isset($filters['employee'])) {
    $selected_employees = $filters['employee']['value'];
  }
  else {
    $selected_employees = array_keys($employees);
  }


  $records = array();
  foreach ($selected_employees as $empid) {
    $assigned_projects = array();
    foreach ($employees[$empid]['job']['projects'] as $project) {
      $assigned_projects[] = '<span class="label" style="background-color:' . $projects[$project]['color_code'] . '">' . $projects[$project]['abbrev'] . '</span> ';
    }
    $records[$empid] = array(
      'name' => $employees[$empid]['name'],
      'picture' => $employees[$empid]['picture_image'],
      'projects' => implode(' ', $assigned_projects)
    );
    foreach ($selected_projects as $project_id) {
      $records[$empid]['project_' . $project_id] = 0;
    }
    $records[$empid]['total'] = 0;
  }

  $records['total'] = array(
    'name' => 'TOTAL',
    'picture' => '',
    'projects' => ''
  );
  foreach ($selected_projects as $project_id) {
    $records['total']['project_' . $project_id] = 0;
  }
  $records['total']['total'] = 0;


  $timesheet = hrm_basic_timesheet_get_data($filters);
  foreach ($timesheet as $row) {
    $log_empid = $row['empid'];
    $log_project_id = $row['project'];
    $duration = $row['to_time'] - $row['from_time'];

    $records[$log_empid]['project_' . $log_project_id] += $duration;
    $records[$log_empid]['total'] += $duration;

    $records['total']['project_' . $log_project_id] += $duration;
    $records['total']['total'] += $duration;
  }

  $rows = array();
  foreach ($records as $key => $record) {
    $row = array(
      array('data' => $record['picture'], 'class' => array('text-center')),
      array('data' => $record['name'], 'class' => array()),
      array('data' => $record['projects'], 'class' => array()),
    );
    foreach ($selected_projects as $project_id) {
      $row[] = array('data' => hrm_basic_time_timestamp_to_time($record['project_' . $project_id]), 'class' => array());
    }
    $row[] = array('data' => hrm_basic_time_timestamp_to_time($record['total']), 'class' => array());

    $classes = array();
    if ($key == 'total') {
      $classes[] = 'total-row';
    }
    $rows[$key] = array('data' => $row, 'class' => $classes);
  }
  $rows['total']['class'][] = 'warning';
  $footer[] = $rows['total'];
  unset($rows['total']);

  $header = array(
    array('data' => '<i class="fa fa-user-circle-o"></i>', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Employee'),
    array('data' => 'Assigned Projects'),
  );
  foreach ($selected_projects as $project_id) {
    $header[] = array('data' => $projects[$project_id]['abbrev'], 'class' => array());
  }
  $header[] = array('data' => 'Total');


  $list = array(
    'attributes' => array('class' => array('table-hover', 'table-vcenter')),
    'rows' => $rows,
    'header' => $header,
    'footer' => $footer,
  );
  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');


  return theme('table', $list);
}
