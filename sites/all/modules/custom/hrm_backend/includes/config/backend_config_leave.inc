<?php

function hrm_backend_config_leave_permission($action, $account = array(), $data = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }

  if (!isset($account['oid']) || !$account['oid']) {
    return FALSE;
  }
  switch ($action) {
    /* Job titles */
    case HRM_PERM_CONFIG_LEAVE_TYPE_ACCESS:
      if (user_access(HRM_PERM_CONFIG_LEAVE_TYPE_ACCESS) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_LEAVE_TYPE_ADD:
      if (user_access(HRM_PERM_CONFIG_LEAVE_TYPE_ADD) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_LEAVE_TYPE_EDIT:
      if (user_access(HRM_PERM_CONFIG_LEAVE_TYPE_EDIT) && $account['oid'] &&
        $data['leave_type']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;

    case HRM_PERM_CONFIG_LEAVE_TYPE_DELETE:
      if (user_access(HRM_PERM_CONFIG_LEAVE_TYPE_DELETE) && $account['oid'] &&
        $data['leave_type']['oid'] == $account['oid']) {
        $leave_type_id = $data['leave_type']['id'];
        $submitted_leaves = db_select('hrm_leave')
            ->fields('hrm_leave', array('lid'))
            ->condition('leave_type', $leave_type_id)
            ->execute()->fetchCol();
        $assigned_entitlements = db_select('hrm_leave_entitlement')
            ->fields('hrm_leave_entitlement', array('eid'))
            ->condition('leave_type', $leave_type_id)
            ->execute()->fetchCol();
        if (empty($submitted_leaves) && empty($assigned_entitlements)) {
          return TRUE;
        }
      }
      break;

      return FALSE;
  }
}

/* -----------------------LEAVE CONFIGURATION----------------------- */
/*
 * Leave type
 */

function hrm_backend_config_leave_types() {
  $account = hrm_basic_account_get_info();
  $branch_filters = array(array('field' => 'oid', 'value' => $account['oid']));
//  if ($account['bid']) {
//    $branch_filters[] = array('field' => 'bid', 'value' => $account['bid']);
//  }
  if (hrm_backend_config_leave_permission(HRM_PERM_CONFIG_LEAVE_TYPE_ACCESS, $account)) {
    $leave_types = hrm_backend_config_leave_type_get_data($branch_filters);
    $status_names = hrm_basic_status_names();
    $status_classes = hrm_basic_status_classes();

    $access_edit_permission = user_access(HRM_PERM_CONFIG_LEAVE_TYPE_EDIT);

//    $countries = hrm_basic_organization_get_countries($branch_filters);

    $answer_options = array(
      HRM_FLAG_YES => 'Yes',
      HRM_FLAG_NO => 'No'
    );

    $rows = array();
    foreach ($leave_types as $id => $leave_type) {
      $row_classes = array();
      $row_classes[] = ($access_edit_permission ? 'table-tr-link' : '');
      $tr_data_url = ($access_edit_permission ? url('config/leave/leave-type/edit/' . $id) : '');
      $rows[$id] = array(
        'data' => array(
          array('data' => '', 'class' => array()),
          array('data' => $leave_type['name']),
//          array('data' => $branches[$leave_type['bid']]),
//          array('data' => $answer_options[$leave_type['accrue']]),
//          array('data' => $answer_options[$leave_type['carried']]),
          array('data' => number_format($leave_type['days'], 2)),
          array('data' => isset($status_names[$leave_type['status']]) ? $status_names[$leave_type['status']] : ''),
        ),
        'class' => $row_classes,
        'data-url' => $tr_data_url,
      );
    };

    $header = array(
      array('data' => '#', 'class' => array('nosort', 'nosearch', 'noexport')),
      array('data' => 'Name'),
//      array('data' => 'Leave Accrue Enabled'),
//      array('data' => 'Leave Carried Forward'),
      array('data' => 'Days'),
      array('data' => 'Status'),
    );

    $list = array(
      'attributes' => array('class' => array('table-vcenter', 'table-hover')),
      'rows' => $rows,
      'header' => $header
    );

    $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all-index' : 'empty-datatable');

    $actions = array();
    if (hrm_backend_config_leave_permission(HRM_PERM_CONFIG_LEAVE_TYPE_ADD, $account)) {
      $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Leave Type', 'config/leave/leave-type/add/0', array('html' => TRUE));
    }

    $variables = array(
      'title' => '<strong>Leave</strong> types',
      'list' => $list,
      'actions' => $actions,
    );

    return theme('page_wrapper_entity_list', $variables);
  }
  else {
    drupal_access_denied();
    exit();
  }
}

function hrm_backend_config_leave_type_get_data($filters = array(), $first_item = FALSE) {
  $query = db_select('hrm_config_leave_type', 'leave_type')
    ->fields('leave_type', array('id', 'oid', 'name', 'days', 'gender',
    'beyond_balance', 'attachment', 'attachment_required', 'dur_full_day', 'dur_time', 'holidays_bd', 'weekend_bd',
    'status', 'uid', 'created', 'changed'));

  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }

  $leave_types = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  if ($first_item) {
    return reset($leave_types);
  }

  return $leave_types;
}

function hrm_backend_config_leave_type_form_page($id) {

  $account = hrm_basic_account_get_info();
  $leave_types = array();

  if ($id) {
    $leave_type_filter = array(
      array('field' => 'oid', 'value' => $account['oid']),
      array('field' => 'id', 'value' => $id)
    );
    $leave_types = hrm_backend_config_leave_type_get_data($leave_type_filter, true);
    if (empty($leave_types)) {
      drupal_not_found();
      exit();
    }
    $edit_leave_permission = hrm_backend_config_leave_permission(HRM_PERM_CONFIG_LEAVE_TYPE_EDIT, $account, array('leave_type' => $leave_types));
    if (!$edit_leave_permission) {

      drupal_access_denied();
      exit();
    }

    drupal_set_title('Edit ' . $leave_types['name']);
  }
  else {
    $add_title_permission = hrm_backend_config_leave_permission(HRM_PERM_CONFIG_LEAVE_TYPE_ADD, $account);
    if (!$add_title_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add leave type');
  }

  $title_form = drupal_get_form('hrm_backend_config_leave_type_form', $leave_types, $account);

  $variables = array(
    'entity' => HRM_ENTITY_LEAVE,
    'title' => 'Leave type information',
    'form' => $title_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_leave_type_form($form, &$form_state, $leave_type, $account) {
  $oid = $account['oid'];
  $branch_filters = array(array('field' => 'oid', 'value' => $oid));
//  if ($account['bid']) {
//    $branch_filters[] = array('field' => 'bid', 'value' => $account['bid']);
//  }
  $emp_filters = $branch_filters;
  $emp_filters[] = array('field' => 'status', 'value' => HRM_FLAG_YES);
//  $countries = hrm_basic_organization_get_countries($org_filters);
//  $branches = hrm_basic_branch_names($branch_filters);
//  if (empty($branches)) {
//    $form['message'] = array('#markup' => '<div class="alert alert-block alert-danger messages error">
//          At lease one location must be defined before updating leave type. ' . l('Add location', 'location/add') . '</div>');
//    return $form;
//  }

  $gender_options = hrm_basic_lov_get_options_list(HRM_LOV_GENDER);

  $accrue_frequency_options = array(
    HRM_LEAVE_ACCRUE_FREQUENCY_DAILY => 'Daily',
    HRM_LEAVE_ACCRUE_FREQUENCY_MONTHLY => 'Monthly',
    HRM_LEAVE_ACCRUE_FREQUENCY_YEARLY => 'Yearly',
  );

  $accrue_credit_day_options = array(
    HRM_LEAVE_ACCRUE_CREDIT_DAY_FIRST => 'First',
    HRM_LEAVE_ACCRUE_CREDIT_DAY_LAST => 'Last',
  );

  $carried_type_options = array(
    HRM_LEAVE_CARRIED_TYPE_ALL => 'All',
    HRM_LEAVE_CARRIED_TYPE_SPECIFIC_DAYS => 'specific days',
  );

  $duration_options = array(
    HRM_LEAVE_DURATION_FULL_DAY => 'Full Day',
//    HRM_LEAVE_DURATION_HALF_DAY => 'Half Day',
    HRM_LEAVE_DURATION_TIME => 'Time',
  );

  $duration_default = array();
  if (!empty($leave_type)) {
    if ($leave_type['dur_full_day'] == HRM_FLAG_YES) {
      $duration_default[] = HRM_LEAVE_DURATION_FULL_DAY;
    }
//    if ($leave_type['dur_half_day'] == HRM_FLAG_YES) {
//      $duration_default[] = HRM_LEAVE_DURATION_HALF_DAY;
//    }
    if ($leave_type['dur_time'] == HRM_FLAG_YES) {
      $duration_default[] = HRM_LEAVE_DURATION_TIME;
    }
  }

//  $approval_methods = array(
//    HRM_LEAVE_APPROVAL_METHOD_REPORT_TO => 'Report-To',
//    HRM_LEAVE_APPROVAL_METHOD_EMPLOYEE => 'Specific Employee',
//    HRM_LEAVE_APPROVAL_METHOD_AUTO_APPROVAL => 'Auto Approval'
//  );

  $status_options = array(
    HRM_STATUS_ACTIVE => hrm_basic_status_names(HRM_STATUS_ACTIVE),
    HRM_STATUS_DEACTIVETED => hrm_basic_status_names(HRM_STATUS_DEACTIVETED),
  );

  $report_to_methods = hrm_basic_reporting_method_names($branch_filters);
  $employees = hrm_basic_employee_names($emp_filters);

  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($leave_type['name']) ? $leave_type['name'] : ''),
  );
//
//  $form['branch'] = array(
//    '#title' => 'Branch',
//    '#type' => 'select',
//    '#required' => TRUE,
//    '#options' => $branches,
//    '#attributes' => array('class' => array('select-chosen')),
//    '#default_value' => (isset($leave_type['bid']) ? $leave_type['bid'] : ''),
//  );

  $form['days'] = array(
    '#title' => 'Leaves per period',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 11,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($leave_type['days']) ? $leave_type['days'] : ''));

  $form['gender'] = array(
    '#title' => 'Applicable Gender****',
    '#type' => 'select',
    '#options' => $gender_options,
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($leave_type['gender']) && isset($gender_options[$leave_type['gender']]) ? $leave_type['gender'] : 0),
    '#empty_option' => 'Any',
    '#empty_value' => 0,
  );

  $form['beyond_balance'] = array(
    '#type' => 'checkbox',
    '#title' => 'Employees can apply beyond the current leave balance',
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($leave_type['beyond_balance']) ? $leave_type['beyond_balance'] : ''),
    '#prefix' => '<div class="form-group">',
    '#suffix' => '</div>',
  );

  $form['entitlement_auto'] = array(
    '#type' => 'checkbox',
    '#title' => 'Auto Entitlement***',
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($leave_type['entitlement_auto']) ? $leave_type['entitlement_auto'] : ''),
    '#prefix' => '<div class="form-group">',
    '#suffix' => '</div>',
    '#description' => 'Entitlement added during employee creation***'
  );

  $form['attachment_prefix'] = array('#markup' => '<div class="form-group">'); // to make line border gone

  $form['attachment'] = array(
    '#type' => 'checkbox',
    '#title' => 'Attachment',
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($leave_type['attachment']) ? $leave_type['attachment'] : ''),
    '#prefix' => '<div class="display-block">', // to make (attachment) filed and  (attachment_required) field under each other
    '#suffix' => '</div>',
  );

  $form['attachment_required'] = array(
    '#type' => 'checkbox',
    '#title' => 'Attachment required',
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($leave_type['attachment_required']) ? $leave_type['attachment_required'] : ''),
    '#states' => array(
      'visible' => array(
        ':input[name="attachment"]' => array('checked' => TRUE),
      ),
    ),
    '#prefix' => '<div class="display-block">',
    '#suffix' => '</div>',
  );
  $form['attachment_suffix'] = array('#markup' => '</div>');

  $form['duration'] = array(
    '#title' => 'Applicable Leave durations',
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#options' => $duration_options,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($duration_default) ? $duration_default : ''),
  );

  $form['holidays_bd'] = array(
    '#type' => 'checkbox',
    '#title' => 'Holidays included***',
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($leave_type['holidays_bd']) ? $leave_type['holidays_bd'] : ''),
    '#prefix' => '<div class="form-group">',
    '#suffix' => '</div>',
  );

  $form['weekend_bd'] = array(
    '#type' => 'checkbox',
    '#title' => 'Weekend included***',
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($leave_type['weekend_bd']) ? $leave_type['weekend_bd'] : ''),
    '#prefix' => '<div class="form-group">',
    '#suffix' => '</div>',
  );

//    $date_picker_format = 'dd-mm-yyyy';

  $due_date_options = array(
    0 => 'Immediatly',
    1 => 'After 1 Month',
    2 => 'After 2 Months',
    3 => 'After 3 Months',
    4 => 'After 4 Months',
    5 => 'After 5 Months',
    6 => 'After 6 Months',
    7 => 'After 7 Months',
    8 => 'After 8 Months',
    9 => 'After 9 Months',
    10 => 'After 10 Months',
    11 => 'After 11 Months',
    12 => 'After 12 Months',
  );
  $form['due_date'] = array(
    '#title' => 'Due Date',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $due_date_options,
    '#empty_option' => '',
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($leave_type['due_date']) ? $due_date_options[$leave_type['due_date']] : ''),
  );
  $form['paid'] = array(
    '#title' => 'Paid',
    '#type' => 'select',
    '#options' => array(HRM_FLAG_YES => 'Yes', HRM_FLAG_NO => 'No'),
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($leave_type['paid']) ? $leave_type['paid'] : ''),
    '#required' => TRUE,
  );
  $form['status'] = array(
    '#title' => 'Status',
    '#type' => 'select',
    '#options' => $status_options,
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($leave_type['status']) && isset($status_options[$leave_type['status']]) ? $leave_type['status'] : HRM_STATUS_ACTIVE),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Leave Type',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($leave_type['id']) && $leave_type['id']) {
    $delete_leave_type_permission = hrm_backend_config_leave_permission(HRM_PERM_CONFIG_LEAVE_TYPE_DELETE, $account, array('leave_type' => $leave_type));
    if ($delete_leave_type_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete Leave type',
        '#submit' => array('hrm_backend_config_leave_type_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
    elseif (user_access(HRM_PERM_CONFIG_LEAVE_TYPE_DELETE)) {
      unset($form['submit']['#suffix']);
      $form['delete_message'] = array(
        '#markup' => '<span class="text-danger">Leave type is not deletable because it is used in Leave and Entitlements***</span></div>'
      );
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'config-leave_type-form';
  return $form;
}

function hrm_backend_config_leave_type_form_validate(&$form, &$form_state) {

  if ($form_state['values']['days'] && !is_numeric($form_state['values']['days'])) {
    form_set_error('days', 'Days should be a number');
  }
}

function hrm_backend_config_leave_type_form_submit(&$form, &$form_state) {

  $leave_type = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  $dur_full_day = HRM_FLAG_NO;
  $dur_time = HRM_FLAG_NO;
  if ($form_state['values']['duration'][HRM_LEAVE_DURATION_FULL_DAY]) {
    $dur_full_day = HRM_FLAG_YES;
  }
//  if ($form_state['values']['duration'][HRM_LEAVE_DURATION_HALF_DAY]) {
//    $dur_half_day = HRM_FLAG_YES;
//  }
  if ($form_state['values']['duration'][HRM_LEAVE_DURATION_TIME]) {
    $dur_time = HRM_FLAG_YES;
  }



  $leave_type_fields = array(
//    'bid' => $form_state['values']['branch'],
    'name' => $form_state['values']['name'],
    'days' => $form_state['values']['days'],
//    'emp_apply' => $form_state['values']['emp_apply'],
//    'admin_assign' => $form_state['values']['admin_assign'],
    'gender' => $form_state['values']['gender'],
    'beyond_balance' => $form_state['values']['beyond_balance'],
//    'accrue' => $form_state['values']['accrue'],
//    'accrue_frequency' => (!empty($form_state['values']['accrue']) ? $form_state['values']['accrue_frequency'] : 0),
//    'accrue_credit_day' => (!empty($form_state['values']['accrue']) ? $form_state['values']['accrue_credit_day'] : 0),
//    'carried' => $form_state['values']['carried'],
//    'carried_type' => (!empty($form_state['values']['carried']) ? $form_state['values']['carried_type'] : 0),
//    'carried_max' => $carried_max,
    'entitlement_auto' => $form_state['values']['entitlement_auto'],
    'attachment' => $form_state['values']['attachment'],
    'attachment_required' => (!empty($form_state['values']['attachment']) ? $form_state['values']['attachment_required'] : 0),
    'dur_full_day' => $dur_full_day,
//    'dur_half_day' => $dur_half_day,
    'dur_time' => $dur_time,
    'holidays_bd' => $form_state['values']['holidays_bd'],
    'weekend_bd' => $form_state['values']['weekend_bd'],
//    'approval_method' => $form_state['values']['approval_method'],
    'paid' => $form_state['values']['paid'],
    'due_date' => $form_state['values']['due_date'],
//    'color_code' => $color_code,
    'status' => $form_state['values']['status'],
    'changed' => time(),
  );

  if (isset($leave_type['id']) && $leave_type['id']) {
    db_update('hrm_config_leave_type')
      ->fields($leave_type_fields)
      ->condition('id', $leave_type['id'], '=')
      ->execute();
    drupal_set_message($form_state['values']['name'] . ' updated successfully***');
  }
  else {
    $leave_type_fields['oid'] = $account['oid'];
    $leave_type_fields['uid'] = $account['uid'];
    $leave_type_fields['created'] = time();

    $leave_type_id = db_insert('hrm_config_leave_type')
      ->fields($leave_type_fields)
      ->execute();
    drupal_set_message($form_state['values']['name'] . ' saved successfully.***');
  }

  drupal_goto('config/leave/leave-type');
}

function hrm_backend_config_leave_type_form_delete(&$form, &$form_state) {

  $leave_types = $form_state['build_info']['args'][0];
  db_delete('hrm_config_leave_type')
    ->condition('id', $leave_types['id'], '=')
    ->execute();

  drupal_set_message($leave_types['name'] . ' deleted successfully***');
  drupal_goto('config/leave/leave-type');
}
