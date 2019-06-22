<?php

/*
 * Organization chart
 */
/*
 * Leaves List
 */

function hr_backend_other_org_chart_page() {
  global $user;
  $account = hr_basic_account_get_info($user->uid);

  return theme('page_wrapper_org_chart', array('account' => $account));
}

function hr_backend_other_org_chart_form($form, &$form_state, $account) {
  $org_id = $account['organization'];
  $organization_filter = array(array('field' => 'org_id', 'value' => $org_id, 'operator' => '='));
  
  if (!isset($form_state['storage']['reporting_methods'])) {
    $form_state['storage']['reporting_methods'] = hr_basic_reporting_method_get_reporting_methods($org_id);
  }

  
  if (!isset($form_state['storage']['projects'])) {
    $form_state['storage']['projects'] = hr_basic_project_get_project_info($organization_filter);
  }
  
  $reporting_methods = $form_state['storage']['reporting_methods'];
  $form['reporting_method'] = array('#title' => t('Reporting Method'), '#type' => 'select',
    '#options' => $reporting_methods,
    '#empty_option' => '',
    '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Reporting Method'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate Organizational charts'),
    "#ajax" => array(
      'callback' => 'hr_backend_other_org_chart_form_callback',
      'event' => 'click',
      "effect" => "fade",
    ),
    '#attributes' => array('class' => array('btn-success', 'btn-sm'))
  );

  //$reset_link = 
  $form['#attributes']['class'][] = 'org-chart-form';

  return $form;
}

function hr_backend_other_org_chart_form_callback($form, &$form_state) {
  $commands = array();
  if (!empty($form_state['values']['reporting_method'])) {
    $account = $form_state['build_info']['args'][0];
    $data = array(
      'account' => $account,
      'reporting_method' => $form_state['values']['reporting_method'],
      'projects' => $form_state['storage']['projects'],
    );

    $content = hr_backend_other_org_chart_view($data);
    $commands[] = ajax_command_html('#org-chart-block-content', $content);
  }
  else {
    $error_message = '
    <div class="alert alert-block alert-danger messages error">
      <a href="#" data-dismiss="alert" class="close">×</a>
      Reporting method field is required
    </div>';
    $commands[] = ajax_command_html('#org-chart-block-content', $error_message);
  }

  $page = array('#type' => 'ajax', '#commands' => $commands);
  return $page;
}

function hr_backend_other_org_chart_view($data) {
  $error_message = '';
  $datasource = array();
  
  $account = $data['account'];  
  $org_id = $account['organization'];
  $reporting_method = $data['reporting_method'];  
  $projects = $data['projects'];  
  
  $terminated_emp_ids = db_select('hr_terminated_employee')
          ->fields('hr_terminated_employee', array('emp_id'))
          ->condition('org_id', $org_id, '=')
          ->execute()->fetchCol(0);
  $terminated_emp_ids[] = 0;
  
  $employee_filters = array(
    array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
    array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '='),
    array('field' => 'emp_id', 'value' => $terminated_emp_ids, 'operator' => 'NOT IN'),
  );
  $employees = hr_basic_employee_get_employees_info($employee_filters);
  $emp_ids = array_keys($employees);
  $reports_to_values = db_select('hr_employee_report_to', 'employee_report_to')
      ->fields('employee_report_to', array('emp_id', 'supervisor_id'))
      ->condition('emp_id', $emp_ids, 'IN')
      ->condition('reporting_method', $reporting_method, '=')
      ->execute()->fetchAllAssoc('emp_id', PDO::FETCH_ASSOC);
  
  $job_titles = hr_basic_job_title_get_job_titles($org_id);
  $locations = hr_basic_location_get_locations($org_id);
  
  $nodes = array();
  foreach ($employees as $emp_id => $employee){
    $supervisor_id = 0;
    if(isset($reports_to_values[$emp_id])){
      $supervisor_id = $reports_to_values[$emp_id]['supervisor_id'];
    }
    $picture_file = file_load($employee['picture']);
    $picture_uri = HR_DEFAULT_IMAGES_EMPLOYEE_PICTURE;
    if($picture_file){
      $picture_uri = $picture_file->uri;
      $picture_url = image_style_url('image_style_resize_64_64', $picture_uri);      
    }    
    
    $assigned_projects = array();
    if(isset($employee['job']['projects']) && !empty($employee['job']['projects'])){
      foreach ($employee['job']['projects'] as $project) {
        $assigned_projects[] = '<span class="label" style="background-color:' . $projects[$project]['color_code'] . '">' . $projects[$project]['abbrev'] . '</span> ';
      }
    }
    
    $picture = '<div class="org-chart-picture-wrapper">' . theme('image_style', array('style_name' => 'image_style_resize_64_64', 'path' => $picture_uri, 'attributes' => array('class' => array('org-chart-picture-image')))) . '</div>';
    $title = '<div class="org-chart-title content-div">' . (isset($job_titles[$employee['job']['job_title']]) ? $job_titles[$employee['job']['job_title']] : '') . '</div>';
    $location = '<div class="org-chart-office content-div">' . (isset($locations[$employee['job']['location']]) ? $locations[$employee['job']['location']] : '') . '</div>';
    $projects_content = '<div class="org-chart-office content-div">' . implode(' ', $assigned_projects) . '</div>';
    
    $content = $picture . $title . $location . $projects_content;    
    $nodes[$emp_id] = array(
      'emp_id' => $emp_id,
      'supervisor_id' => $supervisor_id,
      'name' => $employee['name'],
      'content' => $content
    );
  }
  
  $tree_nodes = hr_basic_build_tree($nodes, 'supervisor_id', 'emp_id');
  
  if (count($nodes) == count($tree_nodes) && count($nodes) > 1) {
    $error_message .= '<div class="text-left"><p>Reports-to data is not configured properly for all employees</p></div>';
  }
  elseif (count($tree_nodes) > 1) {
    $error_message .= '<div class="text-left"><p>Reports-to data is  not configured properly for the folloing employees: </p>';
    $error_message .= '<ul>';
    foreach ($nodes as $node) {
      if ($node['supervisor_id'] == 0) {
        $error_message .= '<li>' . $node['name'] . '</li>';
      }
    }
    $error_message .= '</ul>';
    $error_message .= '<p>Organizational chart must contain only one root</p></div>';
  }
  
  if(!empty($tree_nodes)){
    $datasource = reset($tree_nodes);
  }
  
  $reporting_method_name = hr_basic_reporting_method_get_reporting_methods($org_id, $reporting_method);
  $org_chart_data = array(
    'datasource' => $datasource,
    'error' => $error_message,
    'export_url' => 'org-chart/export/' . $reporting_method,
    'title' => 'organizational chart - ' . $reporting_method_name,
  );
  
  return theme('section_org_chart', $org_chart_data);
}

function hr_backend_other_org_chart_export_page($reporting_method) {
  global $user;
  $account = hr_basic_account_get_info($user->uid);
  $org_id = $account['organization'];
  $organization_filter = array(array('field' => 'org_id', 'value' => $org_id, 'operator' => '='));
  
  $job_titles = hr_basic_job_title_get_job_titles($org_id);
  $locations = hr_basic_location_get_locations($org_id);
  $projects = hr_basic_project_get_project_info($organization_filter);
  
  $organization_reporting_methods = hr_basic_reporting_method_get_reporting_methods($org_id);
  if(!isset($organization_reporting_methods[$reporting_method])){
    drupal_not_found();
  }
  
  $terminated_emp_ids = db_select('hr_terminated_employee')
          ->fields('hr_terminated_employee', array('emp_id'))
          ->condition('org_id', $org_id, '=')
          ->execute()->fetchCol(0);
  $terminated_emp_ids[] = 0;
  
  $employee_filters = array(
    array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
    array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '='),
    array('field' => 'emp_id', 'value' => $terminated_emp_ids, 'operator' => 'NOT IN'),
  );
  $employees = hr_basic_employee_get_employees_info($employee_filters);
  $emp_ids = array_keys($employees);
  $reports_to_values = db_select('hr_employee_report_to', 'employee_report_to')
      ->fields('employee_report_to', array('emp_id', 'supervisor_id'))
      ->condition('emp_id', $emp_ids, 'IN')
      ->condition('reporting_method', $reporting_method, '=')
      ->execute()->fetchAllAssoc('emp_id', PDO::FETCH_ASSOC);  

  $nodes = array();
  foreach ($employees as $emp_id => $employee){
    $supervisor_id = 0;
    if(isset($reports_to_values[$emp_id])){
      $supervisor_id = $reports_to_values[$emp_id]['supervisor_id'];
    }
    $picture_file = file_load($employee['picture']);
    $picture_uri = HR_DEFAULT_IMAGES_EMPLOYEE_PICTURE;
    if($picture_file){
      $picture_uri = $picture_file->uri;
      $picture_url = image_style_url('image_style_resize_64_64', $picture_uri);      
    }    
    
    $assigned_projects = array();
    if(isset($employee['job']['projects']) && !empty($employee['job']['projects'])){
      foreach ($employee['job']['projects'] as $project) {
        $assigned_projects[] = '<span class="label" style="background-color:' . $projects[$project]['color_code'] . '">' . $projects[$project]['abbrev'] . '</span> ';
      }
    }    
    
    $picture = '<div class="org-chart-picture-wrapper">' . theme('image_style', array('style_name' => 'image_style_resize_64_64', 'path' => $picture_uri, 'attributes' => array('class' => array('org-chart-picture-image')))) . '</div>';
    $title = '<div class="org-chart-title content-div">' . (isset($job_titles[$employee['job']['job_title']]) ? $job_titles[$employee['job']['job_title']] : '') . '</div>';
    $location = '<div class="org-chart-office content-div">' . (isset($locations[$employee['job']['location']]) ? $locations[$employee['job']['location']] : '') . '</div>';
    $projects_content = '<div class="org-chart-office content-div">' . implode(' ', $assigned_projects) . '</div>';
    
    $content = $picture . $title . $location . $projects_content;    
    $nodes[$emp_id] = array(
      'emp_id' => $emp_id,
      'supervisor_id' => $supervisor_id,
      'name' => $employee['name'],
      'content' => $content
    );
  }
  
  $tree_nodes = hr_basic_build_tree($nodes, 'supervisor_id', 'emp_id');
  
  $error_message = '';
  if (count($nodes) == count($tree_nodes) && count($nodes) > 1) {
    $error_message .= '<div class="text-left"><p>Reports-to data is not configured properly for all employees</p></div>';
  }
  elseif (count($tree_nodes) > 1) {
    $error_message .= '<div class="text-left"><p>Reports-to data is  not configured properly for the folloing employees: </p>';
    $error_message .= '<ul>';
    foreach ($nodes as $node) {
      if ($node['supervisor_id'] == 0) {
        $error_message .= '<li>' . $node['name'] . '</li>';
      }
    }
    $error_message .= '</ul>';
    $error_message .= '<p>Organizational chart must contain only one root</p></div>';
  }
  
  if(!empty($tree_nodes)){
    $datasource = reset($tree_nodes);
  }
  
  $reporting_method_name = hr_basic_reporting_method_get_reporting_methods($org_id, $reporting_method);
  $org_chart_data = array(
    'datasource' => $datasource,
    'error' => $error_message,
    'export_file_name' => 'organizational chart - ' . $reporting_method_name,
    'title' => 'organizational chart - ' . $reporting_method_name,
  );
  
  $content = theme('section_org_chart', $org_chart_data);
  return $content;
}