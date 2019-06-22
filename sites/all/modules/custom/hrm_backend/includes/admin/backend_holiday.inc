<?php

function hrm_backend_admin_holiday_permissions($hid, $action, $account = array(), $data = array()) {

  $holiday = (isset($data['holiday']) && !empty($data['holiday']) ? $data['holiday'] : hrm_basic_holiday_get_data(array(array('field' => 'hid', 'value' => $hid)), TRUE));
  $permissions = (isset($data['permissions']) && !empty($data['permissions']) ? $data['permissions'] : hrm_basic_role_permissions($account['role']));

  if (empty($holiday) || !isset($account['oid']) || !$account['oid'] || $account['oid'] != $holiday['oid']) {
    return FALSE;
  }
  if (isset($data['holiday']) && !empty($data['holiday'])) {
    if ($data['holiday']['oid'] != $account['oid']) {
      return FALSE;
    }
  }
  switch ($action) {

    /* Holiday */
    case HRM_PERM_ADMINISTRATION_HOLIDAY_EDIT:
      if (isset($permissions[HRM_PERM_ADMINISTRATION_HOLIDAY_EDIT])) {
        return TRUE;
      }
      break;
    case HRM_PERM_ADMINISTRATION_HOLIDAY_DELETE:
      if (isset($permissions[HRM_PERM_ADMINISTRATION_HOLIDAY_DELETE])) {
        return TRUE;
      }
      break;

      return FALSE;
  }
}

/*
 * Holiday
 */

function hrm_backend_admin_holidays() {

  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }
  $branch_filters = array(array('field' => 'oid', 'value' => $account['oid']));
  
  $holidays = hrm_basic_holiday_get_data($branch_filters);
  $locations = hrm_basic_location_names($branch_filters);
  $access_edit_permission = user_access(HRM_PERM_ADMINISTRATION_HOLIDAY_EDIT);

  $current_year = date(HRM_DATE_YEAR_YYYY, time());
  $rows = array();

//  print("<pre>");
//  print_r($holidays);
//  exit();
  foreach ($holidays as $id => $holiday) {
    $holiday_this_year = FALSE;
    $holiday_year = date(HRM_DATE_YEAR_YYYY, $holiday['start']);

    if ($holiday_year == $current_year) {
      $holiday_this_year = TRUE;
    }

    $date = '<strong>' . date(HRM_DATE_LONG, $holiday['start']) . '</strong>';
    $days = 1;
    if ($holiday['start'] != $holiday['end']) {
      $date .= ' -- <strong>' . date(HRM_DATE_LONG, $holiday['end']) . '</strong>';
      $days = ($holiday['end'] - $holiday['start']) / (24 * 60 * 60) + 1;
    }

    $annualy = 'No';
    if ($holiday['annually']) {
      $date = '<strong>' . date(HRM_DATE_MONTH_NAME_DAY_DD, $holiday['start']) . '</strong>';
      if ($holiday['start'] != $holiday['end']) {
        $date .= ' -- <strong>' . date(HRM_DATE_MONTH_NAME_DAY_DD, $holiday['end']) . '</strong>';
      }

      $annualy = 'Yes';
      if (!$holiday_this_year) {
        $holiday_this_year = TRUE;
      }
    }

 
    $row_classes = array();
    $row_classes[] = ($access_edit_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_edit_permission ? url('administration/holiday/edit/' . $id) : '');
    $rows[$id] = array(
      'data' => array(
        array('data' => ($holiday_this_year ? HRM_FA_CHECK_CIRCLE_GREEN_2x : HRM_FA_TIMES_CIRCLE_RED_2X), 'data-sort' => ($holiday_this_year ? 1 : 0), 'data-filter' => ($holiday_this_year ? 'this year' : ''), 'class' => array()),
        array('data' => ($holiday['name'])),
        array('data' => $date, 'data-sort' => $holiday['start']),
        array('data' => $days),
        array('data' => $annualy),
      ),
      'class' => $row_classes,
      'data-url' => $tr_data_url,
    );
  };

  $header = array(
    array('data' => 'This year'),
    array('data' => 'Name'),
    array('data' => 'Date'),
    array('data' => 'Days'),
    array('data' => 'Repeats Annually'),
  );

  $list = array(
    'attributes' => array('class' => array('table-hover', 'table-vcenter')),
    'rows' => $rows,
    'header' => $header
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

  $variables = array(
    'title' => '<strong>Hoidays</strong>',
    'list' => $list,
  );


  return theme('page_wrapper_entity_list_no_header', $variables);
}

function hrm_backend_admin_holiday_form_page($hid) {

  $account = hrm_basic_account_get_info();
  $holiday = array();
  if ($hid) {
    $holiday = hrm_basic_holiday_get_data(array(array('field' => 'hid', 'value' => $hid)), TRUE);

    if (empty($holiday)) {
      drupal_not_found();
      exit();
    }

    $edit_holiday_permission = hrm_backend_admin_holiday_permissions($hid, HRM_PERM_ADMINISTRATION_HOLIDAY_EDIT, $account, array('holiday' => $holiday));
    if (!$edit_holiday_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Edit ' . $holiday['name']);
  }
  else {
    if (!user_access(HRM_PERM_ADMINISTRATION_HOLIDAY_ADD) || !isset($account['oid']) || !$account['oid']) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add Holiday');
  }

  $holiday_form = drupal_get_form('hrm_backend_admin_holiday_form', $hid, $holiday, $account);

  $variables = array(
    'entity' => HRM_ENTITY_HOLIDAY,
    'title' => 'Holiday information',
    'form' => $holiday_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_admin_holiday_form($form, &$form_state, $hid, $holiday, $account) {

  $branch_filters = array(array('field' => 'oid', 'value' => $account['oid']));
//  $countries = hrm_basic_organization_get_countries($org_filters);

 

  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => (isset($holiday['name']) ? $holiday['name'] : ''),
    '#attributes' => array('class' => array()));

  $form['start'] = array(
    '#title' => 'From date',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#empty_option' => '',
    '#default_value' => ((isset($holiday['start']) && $holiday['start']) ? date(HRM_DATE_SHORT, $holiday['start']) : ''),
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
  );

  $form['end'] = array(
    '#title' => 'To date',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#empty_option' => '',
    '#default_value' => ((isset($holiday['end']) && $holiday['end']) ? date(HRM_DATE_SHORT, $holiday['end']) : ''),
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
  );

  $form['annually'] = array(
    '#title' => 'Repeats Annually',
    '#type' => 'checkbox',
    '#default_value' => (isset($holiday['annually']) ? $holiday['annually'] : ''),
    '#prefix' => '<div class="form-item form-group">',
    '#suffix' => '</div>',);
  

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ' Save',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($holiday['hid']) && $holiday['hid']) {
    $delete_holiday_permission = hrm_backend_admin_holiday_permissions($hid, HRM_PERM_ADMINISTRATION_HOLIDAY_DELETE, $account, array('holiday' => $holiday));
    if ($delete_holiday_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#submit' => array('hrm_backend_admin_holiday_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'administration-form';
  return $form;
}




function hrm_backend_admin_holiday_form_submit(&$form, &$form_state) {
  $hid = $form_state['build_info']['args'][0];
  $holiday = $form_state['build_info']['args'][1];
  $account = $form_state['build_info']['args'][2];


  $start = strtotime(str_replace('/', '-', $form_state['values']['start']));
  $end = strtotime(str_replace('/', '-', $form_state['values']['end']));

  if (isset($holiday['hid']) && $holiday['hid']) {
    db_update('hrm_holiday')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'start' => $start,
        'end' => $end,
        'annually' => $form_state['values']['annually'],
        'changed' => time(),
      ))
      ->condition('hid', $hid, '=')
      ->execute();

    db_delete('hrm_holiday_day')->condition('hid', $hid, '=')->execute();
    db_delete('hrm_holiday_location')->condition('hid', $hid, '=')->execute();
    // watchdog('locations', serialize($form_state['values']['locations']));



    $days_query = db_insert('hrm_holiday_day')
      ->fields(array('hid', 'day', 'month', 'year', 'day_date', 'day_timestamp'));
    $temp_start = $start;
    $flag = TRUE;
    while ($flag) {
      $day = date('d', $temp_start);
      $month = date('m', $temp_start);
      $year = date('Y', $temp_start);
      $day_date = date(HRM_DATE_SIMPLE, $temp_start);
      $days_query->values(array($hid, $day, $month, $year, $day_date, $temp_start));
      $temp_start = strtotime('+1 day', $temp_start);
      if ($temp_start > $end) {
        $flag = FALSE;
      }
    }
    $days_query->execute();
  }
  else {
    
 

      $hid = db_insert('hrm_holiday')
        ->fields(array(
          'oid' => $account['oid'],
          'name' => $form_state['values']['name'],
          'start' => $start,
          'end' => $end,
          'annually' => $form_state['values']['annually'],
          'uid' => $account['uid'],
          'created' => time(),
          'changed' => time(),
        ))
        ->execute();
      drupal_set_message($form_state['values']['name'] . ' saved successfully.');
      db_delete('hrm_holiday_day')->condition('hid', $hid, '=')->execute();
      // watchdog('locations', serialize($form_state['values']['locations']));

      $days_query = db_insert('hrm_holiday_day')
        ->fields(array('hid', 'day', 'month', 'year', 'day_date', 'day_timestamp'));
      $temp_start = $start;
      $flag = TRUE;
      while ($flag) {
        $day = date('d', $temp_start);
        $month = date('m', $temp_start);
        $year = date('Y', $temp_start);
        $day_date = date(HRM_DATE_SIMPLE, $temp_start);
        $days_query->values(array($hid, $day, $month, $year, $day_date, $temp_start));
        $temp_start = strtotime('+1 day', $temp_start);
        if ($temp_start > $end) {
          $flag = FALSE;
        }
      }
      $days_query->execute();
    
  }

  drupal_goto('administration/holiday');
}

function hrm_backend_admin_holiday_form_delete(&$form, &$form_state) {

  $hid = $form_state['build_info']['args'][0];
  $holidays = $form_state['build_info']['args'][1];
  $account = $form_state['build_info']['args'][2];

  db_delete('hrm_holiday')
    ->condition('hid', $hid, '=')
    ->execute();

  db_delete('hrm_holiday_location')
    ->condition('hid', $hid, '=')
    ->execute();

  db_delete('hrm_holiday_day')
    ->condition('hid', $hid, '=')
    ->execute();

  drupal_set_message('Holiday deleted successfully');


  drupal_goto('administration/holiday');
}
