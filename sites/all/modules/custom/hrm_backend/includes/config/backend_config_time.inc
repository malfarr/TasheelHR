<?php

function hrm_backend_config_time_permission($action, $account = array(), $data = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }

  switch ($action) {
    /* Work week */
    case HRM_PERM_CONFIG_TIME_WORK_WEEK_ACCESS:
      if (user_access(HRM_PERM_CONFIG_TIME_WORK_WEEK_ACCESS) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_TIME_WORK_WEEK_EDIT:
      if (user_access(HRM_PERM_CONFIG_TIME_WORK_WEEK_EDIT) && $account['oid']) {
        return TRUE;
      }
      break;

    /* Work shift */
    case HRM_PERM_CONFIG_TIME_WORK_SHIFT_ACCESS:
      if (user_access(HRM_PERM_CONFIG_TIME_WORK_SHIFT_ACCESS) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_TIME_WORK_SHIFT_ADD:
      if (user_access(HRM_PERM_CONFIG_TIME_WORK_SHIFT_ADD) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_TIME_WORK_SHIFT_EDIT:
      if (user_access(HRM_PERM_CONFIG_TIME_WORK_SHIFT_EDIT) && $account['oid'] &&
        $data['work_shift']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_TIME_WORK_SHIFT_DELETE:
      if (user_access(HRM_PERM_CONFIG_TIME_WORK_SHIFT_DELETE) && $account['oid'] &&
        $data['work_shift']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;

    /* Date/Time */
    case HRM_PERM_CONFIG_TIME_DATE_TIME_ACCESS:
      if (user_access(HRM_PERM_CONFIG_TIME_DATE_TIME_ACCESS) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_TIME_DATE_TIME_EDIT:
      if (user_access(HRM_PERM_CONFIG_TIME_DATE_TIME_EDIT) && $account['oid']) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}

/*
 * Work week
 */

function hrm_backend_config_time_work_week() {
  global $user;
  $account = hrm_basic_account_get_info($user->uid);
  $filters = array(array('field' => 'oid', 'value' => $account['oid']));
 

  if (hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_WEEK_ACCESS, $account)) {
    $work_week_data = hrm_backend_config_time_work_week_get_data($filters);
    $edit_work_week = hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_WEEK_EDIT, $account, $work_week_data);

//    unset($work_week_data['id']);
//        unset($work_week_data['oid']);

    $rows = array();

      $row_classes = array();
      $row_classes[] = ($edit_work_week ? 'table-tr-link' : '');
      $tr_data_url = ($edit_work_week ? url('config/time/work-week/edit/' .$account['oid']) : '');
      $rows[] = array(
        'data' => array(          
          'sun' => array('data' => $work_week_data['sun'] ? HRM_FA_CHECK_CIRCLE_GREEN_3x : HRM_FA_TIMES_CIRCLE_RED_3x),
          'mon' => array('data' => $work_week_data['mon'] ? HRM_FA_CHECK_CIRCLE_GREEN_3x : HRM_FA_TIMES_CIRCLE_RED_3x),
          'tue' => array('data' => $work_week_data['tue'] ? HRM_FA_CHECK_CIRCLE_GREEN_3x : HRM_FA_TIMES_CIRCLE_RED_3x),
          'wed' => array('data' => $work_week_data['wed'] ? HRM_FA_CHECK_CIRCLE_GREEN_3x : HRM_FA_TIMES_CIRCLE_RED_3x),
          'thu' => array('data' => $work_week_data['thu'] ? HRM_FA_CHECK_CIRCLE_GREEN_3x : HRM_FA_TIMES_CIRCLE_RED_3x),
          'fri' => array('data' => $work_week_data['fri'] ? HRM_FA_CHECK_CIRCLE_GREEN_3x : HRM_FA_TIMES_CIRCLE_RED_3x),
          'sat' => array('data' => $work_week_data['sat'] ? HRM_FA_CHECK_CIRCLE_GREEN_3x : HRM_FA_TIMES_CIRCLE_RED_3x),
        ),
        'class' => $row_classes,
        'data-url' => $tr_data_url,
      );
    
//      print("<pre>");
//      print_r($work_week_data);
//      exit();



    $header = array(
      array('data' => 'SUN'),
      array('data' => 'MON'),
      array('data' => 'TUE'),
      array('data' => 'WED'),
      array('data' => 'THU'),
      array('data' => 'FRI'),
      array('data' => 'SAT'),
    );

    $list = array(
      'attributes' => array('class' => array('table-hover', 'table-vcenter')),
      'rows' => $rows,
      'header' => $header
    );
    $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

    $variables = array(
      'title' => '<strong>Work</strong> Week',
      'list' => $list
    );
    return theme('page_wrapper_entity_list_no_header', $variables);
  }
  else {
    drupal_access_denied();
    exit();
  }
}

function hrm_backend_config_time_work_week_get_data($filters = array(), $first_item = FALSE) {
  $query = db_select('hrm_config_time_work_week', 'time_work_week')
    ->fields('time_work_week', array('id', 'oid', 'sun', 'mon', 'tue', 'wed', 'thu', 'fri', 'sat'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }

  $work_weeks = $query->execute()->fetchAllAssoc('oid', PDO::FETCH_ASSOC);

  if ($first_item) {
    return reset($work_weeks);
  }

  return array_shift($work_weeks);
}

function hrm_backend_config_time_work_week_form_page($oid) {
  if ($oid && is_numeric($oid)) {
    $account = hrm_basic_account_get_info();
    //
    if (!hrm_basic_organization_get_name($oid)) {
      drupal_not_found();
      exit();
    }
    if (hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_WEEK_EDIT, $account)) {
      $filters = array(
        array('field' => 'oid', 'value' => $account['oid']),
      );

      drupal_set_title(' Work week');

      $work_week = hrm_backend_config_time_work_week_get_data($filters, TRUE);

      $work_week_form = drupal_get_form('hrm_backend_config_time_work_week_form', $oid, $work_week, $account);

      $variables = array(
        'entity' => HRM_ENTITY_WORK_SHIFT,
        'title' => 'Work week',
        'form' => $work_week_form,
      );
      return theme('page_wrapper_entity_form', $variables);
    }
    else {
      drupal_access_denied();
      exit();
    }
  }
  else {
    drupal_not_found();
    exit();
  }
}

function hrm_backend_config_time_work_week_form($form, &$form_state, $oid, $work_week, $account) {



  $form['working_days'] = array(
    '#type' => 'container',
  );

  $form['working_days']['sun'] = array(
    '#type' => 'checkbox',
    '#title' => 'Sunday',
    '#default_value' => (isset($work_week['sun']) ? $work_week['sun'] : 0),
    '#prefix' => '<label for="edit-country">Days <span class="form-required" title="This field is required.">*</span></label>',
  );

  $form['working_days']['mon'] = array(
    '#type' => 'checkbox',
    '#title' => 'Monday',
    '#default_value' => (isset($work_week['mon']) ? $work_week['mon'] : 0),
  );

  $form['working_days']['tue'] = array(
    '#type' => 'checkbox',
    '#title' => 'Tuesday',
    '#default_value' => (isset($work_week['tue']) ? $work_week['tue'] : 0),
  );

  $form['working_days']['wed'] = array(
    '#type' => 'checkbox',
    '#title' => 'Wednesday',
    '#default_value' => (isset($work_week['wed']) ? $work_week['wed'] : 0),
  );

  $form['working_days']['thu'] = array(
    '#type' => 'checkbox',
    '#title' => 'Thursday',
    '#default_value' => (isset($work_week['thu']) ? $work_week['thu'] : 0),
  );
  $form['working_days']['fri'] = array(
    '#type' => 'checkbox',
    '#title' => 'Friday',
    '#default_value' => (isset($work_week['fri']) ? $work_week['fri'] : 0),
  );

  $form['working_days']['sat'] = array(
    '#type' => 'checkbox',
    '#title' => 'Saturday',
    '#default_value' => (isset($work_week['sat']) ? $work_week['sat'] : 0),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ' Save Work Week',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  $form['#attributes']['class'][] = 'block-checkboxes';
  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'work-week-form';

  return $form;
}

function hrm_backend_config_time_work_week_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_config_time_work_week_form_submit(&$form, &$form_state) {
  $work_week = $form_state['build_info']['args'][1];
  $account = $form_state['build_info']['args'][2];

  if (empty($work_week)) {
    db_insert('hrm_config_time_work_week')
      ->fields(array(
        'oid' => $account['oid'],
        'sun' => $form_state['values']['sun'],
        'mon' => $form_state['values']['mon'],
        'tue' => $form_state['values']['tue'],
        'wed' => $form_state['values']['wed'],
        'thu' => $form_state['values']['thu'],
        'fri' => $form_state['values']['fri'],
        'sat' => $form_state['values']['sat'],
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  }
  else {
    db_update('hrm_config_time_work_week')
      ->fields(array(
        'sun' => $form_state['values']['sun'],
        'mon' => $form_state['values']['mon'],
        'tue' => $form_state['values']['tue'],
        'wed' => $form_state['values']['wed'],
        'thu' => $form_state['values']['thu'],
        'fri' => $form_state['values']['fri'],
        'sat' => $form_state['values']['sat'],
        'changed' => time(),
      ))
      ->condition('oid', $account['oid'], '=')
      ->execute();
  }
  drupal_set_message('Work week saved successfully***');
  drupal_goto('config/time/work-week');
}

/*
 * Work shift
 */

function hrm_backend_config_time_work_shifts() {
  $account = hrm_basic_account_get_info();

  if (hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_SHIFT_ACCESS, $account)) {
    $filters = array(array('field' => 'oid', 'value' => $account['oid']));
    $work_shifts = hrm_backend_config_time_work_shift_get_data($filters);
    $role_permissions = hrm_basic_role_permissions($account['role']);

    $rows = array();
    foreach ($work_shifts as $id => $work_shift) {
      $row_classes = array();
      $permission_data = array('work_shift' => $work_shift, 'permissions' => $role_permissions);
      $edit_permission = hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_SHIFT_EDIT, $account, $permission_data);
      $row_classes[] = ($edit_permission ? 'table-tr-link' : '');
      $tr_data_url = ($edit_permission ? url('config/time/work-shift/edit/' . $id) : '');
      $rows[$id] = array(
        'data' => array(
          array('data' => ($work_shift['name'])),
          array('data' => $work_shift['from_time']),
          array('data' => $work_shift['to_time']),
          array('data' => $work_shift['duration']),
          array('data' => ($work_shift['default_shift'] == HRM_FLAG_YES ? HRM_FA_CHECK_CIRCLE_GREEN_3x : '')),
          array('data' => ($work_shift['override'] == HRM_FLAG_YES ? HRM_FA_CHECK_CIRCLE_GREEN_3x : '')),
        ),
        'class' => $row_classes,
        'data-url' => $tr_data_url,
      );
    }
    $header = array(
      array('data' => 'Name'),
      array('data' => 'From'),
      array('data' => 'To'),
      array('data' => 'Duration'),
      array('data' => 'Default'),
      array('data' => 'Override all'),
    );

    $list = array(
      'attributes' => array('class' => array('table-hover', 'table-vcenter')),
      'rows' => $rows,
      'header' => $header
    );
    $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

    $actions = array();
    if (hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_SHIFT_ADD, $account)) {
      $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Work Shift', 'config/time/work-shift/add/0', array('html' => TRUE));
    }

    $variables = array(
      'title' => '<strong>Work</strong> shifts',
      'actions' => $actions,
      'list' => $list
    );
    return theme('page_wrapper_entity_list', $variables);
  }
  else {
    drupal_access_denied();
    exit();
  }
}

function hrm_backend_config_time_work_shift_get_data($filters = array(), $first_item = FALSE) {
  $query = db_select('hrm_config_time_work_shift', 'time_work_shift')
    ->fields('time_work_shift', array('id', 'oid', 'name', 'from_time', 'to_time', 'duration', 'default_shift', 'override',
    'uid', 'created', 'changed'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : ' = ');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }

  $work_shifts = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  if ($first_item) {
    return reset($work_shifts);
  }

  return $work_shifts;
}

function hrm_backend_config_time_work_shift_form_page($id) {
  $account = hrm_basic_account_get_info();

  $work_shift = array();
  if ($id) {
    $work_shift = hrm_backend_config_time_work_shift_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($work_shift)) {
      drupal_not_found();
      exit();
    }
    $permission_data = array('work_shift' => $work_shift, 'permissions' => hrm_basic_account_get_role($account['role']));
    $edit_title_permission = hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_SHIFT_EDIT, $account, $permission_data);
    if (!$edit_title_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Edit ' . $work_shift['name']);
  }
  else {

    $add_title_permission = hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_SHIFT_ADD, $account);
    if (!$add_title_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add Work Shift');
  }

  $work_shift_form = drupal_get_form('hrm_backend_config_time_work_shift_form', $work_shift, $account);

  $variables = array(
    'entity' => HRM_ENTITY_WORK_SHIFT,
    'title' => 'Work Shift information',
    'form' => $work_shift_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_time_work_shift_form($form, &$form_state, $work_shift, $account) {
  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($work_shift['name']) ? $work_shift['name'] : '')
  );
  $form['from_time'] = array(
    '#type' => 'textfield',
    '#title' => 'From',
    '#required' => TRUE,
    '#prefix' => '<div class="bootstrap-timepicker">',
    '#suffix' => '</div>',
    '#default_value' => (isset($work_shift['from_time']) ? $work_shift['from_time'] : '09:00 AM'),
    '#attributes' => array('class' => array('bootstrap-timepicker', 'input-timepicker', 'work-shift-from-time')),
  );

  $form['to_time'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#required' => TRUE,
    '#prefix' => '<div class="bootstrap-timepicker">',
    '#suffix' => '</div>',
    '#default_value' => (isset($work_shift['to_time']) ? $work_shift['to_time'] : '05:00 PM'),
    '#attributes' => array('class' => array('input-timepicker', 'work-shift-to-time')),
  );
  $form['default_shift'] = array(
    '#type' => 'checkbox',
    '#title' => 'Default Work shift',
    '#default_value' => (isset($work_shift['default_shift']) ? $work_shift['default_shift'] : 0),
    '#prefix' => '<div class="form-item form-group">',
    '#suffix' => '</div>'
  );
  $form['override'] = array(
    '#type' => 'checkbox',
    '#title' => 'Overrides all workshifts',
    '#default_value' => (isset($work_shift['override']) ? $work_shift['override'] : 0),
    '#prefix' => '<div class="form-item form-group">',
    '#suffix' => '</div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Work Shift',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($work_shift['id']) && $work_shift['id']) {
    $delete_work_shift_permission = hrm_backend_config_time_permission(HRM_PERM_CONFIG_TIME_WORK_SHIFT_DELETE, $account, array('work_shift' => $work_shift));
    if ($delete_work_shift_permission) {
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete Work Shift',
        '#submit' => array('hrm_backend_config_time_work_shift_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );

      unset($form['submit']['#suffix']);
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'config-work-shift-form';

  return $form;
}

function hrm_backend_config_time_work_shift_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_config_time_work_shift_form_submit(&$form, &$form_state) {
  $work_shift = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  $today_date = date(HRM_DATE_SIMPLE, time());
  $from_timestamp = strtotime($today_date . ' ' . $form_state['values']['from_time']);
  $to_timestamp = strtotime($today_date . ' ' . $form_state['values']['to_time']);
  $duration_timestamp = $to_timestamp - $from_timestamp;
  $duration = hrm_basic_time_timestamp_to_time($duration_timestamp);

  if ($form_state['values']['default_shift']) {
    db_update('hrm_config_time_work_shift')
      ->fields(array(
        'default_shift' => HRM_FLAG_NO
      ))
      ->condition('oid', $account['oid'], '=')
      ->execute();
  }
  if ($form_state['values']['override']) {
    db_update('hrm_config_time_work_shift')
      ->fields(array(
        'override' => HRM_FLAG_NO
      ))
      ->condition('oid', $account['oid'], '=')
      ->execute();
  }
  if (isset($work_shift['id']) && $work_shift['id']) {
    db_update('hrm_config_time_work_shift')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'from_time' => $form_state['values']['from_time'],
        'to_time' => $form_state['values']['to_time'],
        'duration' => $duration,
        'default_shift' => $form_state['values']['default_shift'],
        'override' => $form_state['values']['override'],
        'changed' => time(),
      ))
      ->condition('id', $work_shift['id'], '=')
      ->execute();
  }
  else {
    db_insert('hrm_config_time_work_shift')
      ->fields(array(
        'oid' => $account['oid'],
        'name' => $form_state['values']['name'],
        'from_time' => $form_state['values']['from_time'],
        'to_time' => $form_state['values']['to_time'],
        'duration' => $duration,
        'default_shift' => $form_state['values']['default_shift'],
        'override' => $form_state['values']['override'],
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  }

  drupal_set_message($form_state['values']['name'] . ' work shift saved successfully***');
  drupal_goto('config/time/work-shift');
}

function hrm_backend_config_time_work_shift_form_delete(&$form, &$form_state) {
  $work_shift = $form_state['build_info']['args'][0];

  db_delete('hrm_config_time_work_shift')
    ->condition('id', $work_shift['id'], '=')
    ->execute();

  drupal_set_message($work_shift['name'] . ' work shift deleted successfully***');
  drupal_goto('config/time/work-shift');
}

