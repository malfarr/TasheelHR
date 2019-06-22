<?php

function hrm_backend_leave_permissions($action, $account = array(), $data = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }
  $permissions = (isset($data['permissions']) && !empty($data['permissions']) ? $data['permissions'] : hrm_basic_role_permissions($account['role']));
  $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));
  switch ($action) {
    /* ----------- Entitlements ----------- */
    case HRM_PERM_LEAVE_ENTITLEMENT_ACCESS_ENTITLEMENTS:
      if (isset($permissions[HRM_PERM_LEAVE_ENTITLEMENT_ACCESS_ENTITLEMENTS]) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_ENTITLEMENT_ACCESS_ENTITLEMENT_DETAILS:
      if (isset($permissions[HRM_PERM_LEAVE_ENTITLEMENT_ACCESS_ENTITLEMENTS]) && $account['oid'] &&
        $data['entitlement']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_ENTITLEMENT_ADD_ENTITLEMENTS:

      if (isset($permissions[HRM_PERM_LEAVE_ENTITLEMENT_ADD_ENTITLEMENTS]) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_ENTITLEMENT_UPDATE_ENTITLEMENTS:
      if (isset($permissions[HRM_PERM_LEAVE_ENTITLEMENT_UPDATE_ENTITLEMENTS]) && $account['oid'] &&
        $data['entitlement']['oid'] == $account['oid'] && $data['entitlement']['leave_period'] == $leave_period) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_ENTITLEMENT_DELETE_ENTITLEMENTS:
      if (isset($permissions[HRM_PERM_LEAVE_ENTITLEMENT_DELETE_ENTITLEMENTS]) && $account['oid'] &&
        $data['entitlement']['oid'] == $account['oid'] && $data['entitlement']['leave_period'] == $leave_period) {
        return TRUE;
      }
      break;

    /* ----------- Leaves ----------- */
    case HRM_PERM_LEAVE_ACCESS_STAFF_LEAVES:
      if (hrm_basic_employee_is_supervisor($account['uid']) || $account['role'] == HRM_ROLE_ADMIN || $account['role'] == HRM_ROLE_HR) {
        return TRUE;
      }
    case HRM_PERM_LEAVE_ACCESS_LEAVES:
      if (isset($permissions[HRM_PERM_LEAVE_ACCESS_LEAVES]) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_ACCESS_DETAILS:
      if (isset($permissions[HRM_PERM_LEAVE_ACCESS_DETAILS])) {


        if ($account['role'] == HRM_ROLE_ADMIN || $account['role'] == HRM_ROLE_HR) {
          return TRUE;
        }
        if (in_array($data['leave']['empid'], hrm_basic_employee_get_staff_uids($account['uid']))) {
          return TRUE;
        }
        if ($data['leave']['empid'] == $account['uid']) {
          return TRUE;
        }
        return FALSE;
      }
      break;
    case HRM_PERM_LEAVE_ASSIGN_LEAVE:

      if (isset($permissions[HRM_PERM_LEAVE_ASSIGN_LEAVE]) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_CANCEL_LEAVE:
      if (isset($permissions[HRM_PERM_LEAVE_CANCEL_LEAVE]) && $account['oid'] &&
        $data['leave']['oid'] == $account['oid'] &&
        $data['leave']['leave_period'] == $leave_period &&
        $data['leave']['status'] != HRM_STATUS_CANCELED &&
        $data['leave']['status'] != HRM_STATUS_USED) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_ACCESS_OWN_LEAVES:
      return TRUE;
      break;
    case HRM_PERM_LEAVE_DELETE_LEAVE:
      if (isset($permissions[HRM_PERM_LEAVE_DELETE_LEAVE]) && $account['oid'] &&
        $data['leave']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_MANAGE_LEAVE:
      if (isset($permissions[HRM_PERM_LEAVE_MANAGE_LEAVE]) && $account['oid'] &&
        $data['leave']['oid'] == $account['oid'] && $data['leave']['status'] == HRM_STATUS_PENDING_TO_SUPERVISOR) {
        return TRUE;
      }
      break;

    case HRM_PERM_LEAVE_DOCUMENT_ACCESS:
      if (isset($permissions[HRM_PERM_LEAVE_DOCUMENT_ACCESS]) && $account['oid'] &&
        $data['leave']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_DOCUMENT_ADD:
      if (isset($permissions[HRM_PERM_LEAVE_DOCUMENT_ADD]) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_DOCUMENT_EDIT:
      if (isset($permissions[HRM_PERM_LEAVE_DOCUMENT_EDIT]) && $account['oid'] &&
        $data['leave']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_LEAVE_DOCUMENT_DELETE:
      if (isset($permissions[HRM_PERM_LEAVE_DOCUMENT_DELETE]) && $account['oid'] &&
        $data['leave']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;

    case HRM_PERM_LEAVE_APPLY_LEAVE:
      if (isset($permissions[HRM_PERM_LEAVE_APPLY_LEAVE])) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}

/*
 * ========================================
 * Entitlements
 * ========================================
 */

function hrm_backend_leave_entitlements_page() {
  $account = hrm_basic_account_get_info();

//  print("<pre>");
//  print_r(hrm_basic_role_permissions($account['role']));
//  exit();

  if (hrm_backend_leave_permissions(HRM_PERM_LEAVE_ENTITLEMENT_ACCESS_ENTITLEMENTS, $account)) {

    $oid = $account['oid'];
    $employee_filters = array(
      array('field' => 'oid', 'value' => $oid),
      array('field' => 'status', 'value' => HRM_STATUS_ACTIVE),
    );
    $entitlement_filters = array(
      array('field' => 'oid', 'value' => $oid)
    );
    $filters = array(
      array('field' => 'oid', 'value' => $account['oid'])
    );

    $leave_types = hrm_basic_leave_type_names($filters);

    $first_leave_period = variable_get(HRM_VAR_ORG_LEAVE_FIRST_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));
    $leave_period = variable_get(HRM_VAR_ORG_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));
    $selected_leave_period = $leave_period;

    $query_filters = drupal_get_query_parameters();
    $filters_count = 0;


//Update employees and entitlement filters
    if (TRUE) {
      if (isset($query_filters['lp']) && is_numeric($query_filters['lp']) &&
        $query_filters['lp'] >= $first_leave_period && $query_filters['lp'] <= $leave_period) {
        $selected_leave_period = $query_filters['lp'];

        $filters_count += 1;
      }

//Leave types
      if (isset($query_filters['lt']) && !empty($query_filters['lt'])) {
        $leave_type_ids = explode('+', $query_filters['lt']);
        foreach ($leave_type_ids as $key => $leave_type_id) {
          if (!is_numeric($leave_type_id)) {
            unset($leave_type_ids[$key]);
          }
        }
        $leave_type_ids[] = 0;
        $entitlement_filters[] = array('field' => 'leave_type', 'value' => $leave_type_ids, 'operator' => 'IN');
        $filters_count += 1;
      }

////Employees
      if (isset($query_filters['empid']) && !empty($query_filters['empid'])) {
        $empids = explode('+', $query_filters['empid']);
        foreach ($empids as $key => $empid) {
          if (!is_numeric($empid)) {
            unset($empids[$key]);
          }
        }
        $empids[] = 0;
        $employee_filters[] = array('field' => 'empid', 'value' => $empids, 'operator' => 'IN');
        $filters_count += 1;
      }
//
////Job title
//      if (isset($query_filters['jt']) && !empty($query_filters['jt'])) {
//        $job_title_empids = db_select('hrm_emp_job', 'hrm_emp_job')
//            ->fields('hrm_emp_job', array('empid'))
//            ->condition('job_title', $query_filters['jt'], '=')
//            ->execute()->fetchCol(0);
//
//        $employee_filters[] = array('field' => 'empid', 'value' => $job_title_empids, 'operator' => 'IN');
//        $filters_count += 1;
//      }
    }

    $entitlement_filters[] = array('field' => 'leave_period', 'value' => $selected_leave_period);

    $title = 'Leave Entitlements - <strong>' . $selected_leave_period . '</strong>';
    drupal_set_title($title, PASS_THROUGH);
    // backhere

    $employees = hrm_basic_employee_get_data($employee_filters, FALSE, TRUE);

    $rows = array();
    if (!empty($employees)) {
      $empids = array_keys($employees);

      $entitlement_filters[] = array('field' => 'empid', 'value' => $empids, 'operator' => 'IN');
      $entitlements = hrm_basic_leave_entitlement_get_data($entitlement_filters, FALSE, TRUE);

      $access_details = user_access(HRM_PERM_LEAVE_ENTITLEMENT_ACCESS_ENTITLEMENT_DETAILS);

      foreach ($entitlements as $eid => $entitlement) {
        $empid = $entitlement['empid'];
        if (!isset($leave_types[$entitlement['leave_type']])) {
          $leave_types[$entitlement['leave_type']] = 'LEAVE TYPE NOT EXIST';
        }


        $row_classes = array();
        $row_classes[] = ($entitlement['days'] < 0 ? 'danger' : '');
        $row_classes[] = ($access_details ? 'table-tr-link' : '');
        $tr_data_url = ($access_details ? url('leave/entitlement/details/' . $eid) : '');
        $rows[] = array(
          'data' => array(
            'photo' => array('data' => $employees[$empid]['photo_image'], 'class' => array('cell-photo', 'cell-photo-circle')),
            'name' => array('data' => $employees[$empid]['name'], 'class' => array('nowrap')),
            'leave' => array('data' => $leave_types[$entitlement['leave_type']]),
            'period' => array('data' => $entitlement['leave_period']),
            'transfered' => array('data' => number_format($entitlement['transfered'], 2)),
            'pending' => array('data' => number_format($entitlement['pending'], 2)),
            'used' => array('data' => number_format($entitlement['used'], 2)),
            'days' => array('data' => number_format($entitlement['days'], 2)),
          ),
          'class' => $row_classes,
          'data-url' => $tr_data_url,
        );
      }
    }

    $header = array(
      array('data' => HRM_FA_USER, 'class' => array('nosort', 'nosearch', 'noexport', 'text-center')),
      array('data' => 'Name'),
      array('data' => 'Leave type'),
      array('data' => 'Period'),
      array('data' => 'Transfered'),
      array('data' => 'Pending'),
      array('data' => 'Used'),
      array('data' => 'Balance'),
    );

    $list = array(
      'attributes' => array('class' => array('table-hover', 'table-vcenter')),
      'rows' => $rows,
      'header' => $header,
      'sticky' => FALSE,
    );
    $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all' : 'empty-datatable');

    $actions = array();
    if (hrm_backend_leave_permissions(HRM_PERM_LEAVE_ENTITLEMENT_ADD_ENTITLEMENTS, $account)) {
      $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Leave Entitlement', 'leave/entitlement/add/0', array('html' => TRUE));
    }

    $variables = array(
      'title' => $title,
      'list' => $list,
      'filter_form' => drupal_get_form('hrm_backend_leave_entitlementse_filter_form', $account),
      'filters_count' => $filters_count,
      'actions' => $actions
    );

    return theme('page_wrapper_entity_list', $variables);
  }

  drupal_access_denied();
  exit();
}

function hrm_backend_leave_entitlement_details_page($eid) {
  $account = hrm_basic_account_get_info();

  if (!isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $entitlement_filters = array(
    array('field' => 'eid', 'value' => $eid),
    array('field' => 'oid', 'value' => $account['oid']),
  );
  $entitlement = hrm_basic_leave_entitlement_get_data($entitlement_filters, TRUE);
  if (empty($entitlement)) {
    drupal_not_found();
    exit();
  }

  $role_permission = hrm_basic_role_permissions($account['role']);
  $permission_data = array('entitlement' => $entitlement, 'permissions' => $role_permission);

  $access_details_permission = hrm_backend_leave_permissions(HRM_PERM_LEAVE_ENTITLEMENT_ACCESS_ENTITLEMENT_DETAILS, $account, $permission_data);
  if (!$access_details_permission) {
    drupal_access_denied();
    exit();
  }

  $entity_filter = array(
    array('field' => 'entity', 'value' => HRM_ENTITY_LEAVE_ENTITLEMENT),
    array('field' => 'entity_id', 'value' => $eid),
  );

  $tabs = array();
  $leave_type_name = hrm_basic_leave_type_get_name($entitlement['leave_type']);
  $employee_name = hrm_basic_employee_get_name($entitlement['empid']);

//Entitlement details output
  if (TRUE) {
    $fields = array();

    $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Leave type', 'value_class' => array(),
      'value' => $leave_type_name, 'value_class' => array());

    $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Leave period', 'value_class' => array(),
      'value' => $entitlement['leave_period'], 'value_class' => array());

    $entitlement_days_output = '';
    $entitlement_days_output = '
      <div class="col-sm-4 col-lg-3 style-alt">
        <div class="widget">  
          <div class="widget-extra themed-background text-center"><h3 class="widget-content-light">Transfered</h3></div>
          <div class="widget-extra-full text-center h3 strong">' . number_format($entitlement['transfered'], 4) . '</div>
        </div>
        </div>';
    $entitlement_days_output .= '
      <div class="col-sm-4 col-lg-3 style-alt">
        <div class="widget">  
          <div class="widget-extra themed-background-spring text-center"><h3 class="widget-content-light">Pending</h3></div>
          <div class="widget-extra-full text-center h3 strong">' . number_format($entitlement['pending'], 4) . '</div>
        </div>
        </div>';
    $entitlement_days_output .= '
      <div class="col-sm-4 col-lg-3 style-alt">
        <div class="widget">  
          <div class="widget-extra themed-background-dark-fire text-center"><h3 class="widget-content-light">Used</h3></div>
          <div class="widget-extra-full text-center h3 strong">' . number_format($entitlement['used'], 4) . '</div>
        </div>
        </div>';
    $entitlement_days_output .= '
      <div class="col-sm-4 col-lg-3 style-alt">
        <div class="widget">  
          <div class="widget-extra themed-background-autumn text-center"><h3 class="widget-content-light">Balance</h3></div>
          <div class="widget-extra-full text-center h3 strong">' . number_format($entitlement['days'], 4) . '</div>
        </div>
        </div>';
    $fields[] = array('type' => 'raw', 'wrapper_class' => array('col-sm-12', 'padding-0'), 'row_class' => array('clearfix', 'noborder'),
      'value' => $entitlement_days_output, 'value_class' => array());

    $added_by = 'System auto-generated';
    if ($entitlement['uid']) {
      $added_by = hrm_basic_account_get_name($entitlement['uid']);
    }

    $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Added by', 'value_class' => array(),
      'value' => $added_by, 'value_class' => array());

    $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Created on', 'value_class' => array(),
      'value' => date(HRM_DATE_SIMPLE_TIME_12), 'value_class' => array());

    $tabs['details'] = array(
      'title' => 'Details',
      'content' => theme('section_entity_details', array('fields' => $fields))
    );
  }

  $history_logs = hrm_backend_entity_history_get_history($entity_filter);
  $entitlement_history = theme('section_entity_history', array('history' => $history_logs, 'title' => 'Leave entitlements hisory'));
  $tabs['history'] = array(
    'title' => 'History',
    'content' => $entitlement_history,
  );

  $actions = array();

  $update_entitlement_permission = hrm_backend_leave_permissions(HRM_PERM_LEAVE_ENTITLEMENT_UPDATE_ENTITLEMENTS, $account, $permission_data);
  if ($update_entitlement_permission) {
    $actions[] = l(HRM_FA_PENCIL_PURPLE . 'Update Leave Entitlement', 'leave/entitlement/edit/' . $eid, array('html' => TRUE));
  }
  $variables = array(
    'tabs' => $tabs,
    'actions' => $actions,
  );

  drupal_set_title($employee_name . ' - ' . $leave_type_name . ' ' . $entitlement['leave_period']);

  $output = theme('page_wrapper_entity_tabs_js', $variables);

  return $output;
}

function hrm_backend_leave_entitlementse_filter_form($form, &$form_state, $account) {
  $oid = $account['oid'];
  $filters = array(array('field' => 'oid', 'value' => $oid, 'operator' => '='));

  //branch test
  $employees = hrm_basic_employee_names();
  $job_titles = hrm_basic_job_title_names($filters);

  $query_filters = drupal_get_query_parameters();

  $first_leave_period = variable_get(HRM_VAR_ORG_LEAVE_FIRST_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));
  $leave_period = variable_get(HRM_VAR_ORG_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));
  $years = array();
  while ($leave_period >= $first_leave_period) {
    $years[$leave_period] = $leave_period;
    $leave_period--;
  }
  $form['lp'] = array(
    '#title' => 'Leave Period',
    '#type' => 'select',
    '#options' => $years,
    '#empty_option' => '',
    '#required' => TRUE,
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($query_filters['lp']) && isset($years[$query_filters['lp']]) ? $query_filters['lp'] : date(HRM_DATE_YEAR_YYYY)),
  );

  $leave_types = hrm_basic_leave_type_names($filters);
  $form['lt'] = array(
    '#title' => 'Leave Type',
    '#type' => 'select',
    '#options' => $leave_types,
    '#empty_option' => 'All',
    '#empty_value' => 'All',
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($query_filters['lt']) && isset($leave_types[$query_filters['lt']]) ? $query_filters['lt'] : ''),
  );

  $form['empid'] = array(
    '#title' => 'Employee',
    '#type' => 'select',
    '#options' => $employees,
    '#empty_option' => 'All',
    '#empty_value' => 'All',
    '#default_value' => (isset($query_filters['empid']) && isset($employees[$query_filters['empid']]) ? $query_filters['empid'] : ''),
    '#attributes' => array('class' => array('select-chosen')),
  );

  $form['jt'] = array(
    '#title' => 'Job Title',
    '#type' => 'select',
    '#options' => $job_titles,
    '#empty_option' => 'All',
    '#empty_value' => 'All',
    '#default_value' => (isset($query_filters['jt']) && isset($job_titles[$query_filters['jt']]) ? $query_filters['jt'] : ''),
    '#attributes' => array('class' => array('select-chosen')),
  );




  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'View',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm', 'btn-block'))
  );

  $form['#attributes']['class'][] = 'leave-entitlments-filter-form';

  return $form;
}

function hrm_backend_leave_entitlementse_filter_form_submit($form, &$form_state) {

  $account = $form_state['build_info']['args'][0];

  $query_filters = array();
  if (isset($form_state['values']['lp']) && !empty($form_state['values']['lp'])) {
    $query_filters['lp'] = $form_state['values']['lp'];
  }
  if (!empty($form_state['values']['lt']) && is_numeric($form_state['values']['lt'])) {
    $query_filters['lt'] = $form_state['values']['lt'];
  }
  if (!empty($form_state['values']['empid']) && is_numeric($form_state['values']['empid'])) {
    $query_filters['empid'] = $form_state['values']['empid'];
  }
  if (!empty($form_state['values']['jt']) && is_numeric($form_state['values']['jt'])) {
    $query_filters['jt'] = $form_state['values']['jt'];
  }
//    


  drupal_goto('leave/entitlement', array('query' => $query_filters));
}

//Entitlement Form
function hrm_backend_leave_entitlement_form_page($id) {

  $account = hrm_basic_account_get_info();

  $title = 'Add Leave Entitlement';
  $entitlement = array();

  $role_permissions = hrm_basic_role_permissions($account['role']);
  $permission_data = array('permissions' => $role_permissions);

  if ($id) {
    $entitlement = hrm_basic_leave_entitlement_get_data(array(array('field' => 'eid', 'value' => $id)), TRUE);
    if (empty($entitlement)) {
      drupal_not_found();
      exit();
    }

    $role_permissions = hrm_basic_role_permissions($account['role']);
    $permission_data['entitlement'] = $entitlement;

    $update_entitlement_permission = hrm_backend_leave_permissions(HRM_PERM_LEAVE_ENTITLEMENT_UPDATE_ENTITLEMENTS, $account, $permission_data);
    if (!$update_entitlement_permission) {
      drupal_access_denied();
    }
    $title = 'Update Leave Entitlement - ' . $entitlement['leave_period'];
  }
  else {
    $add_entitlement_permission = hrm_backend_leave_permissions(HRM_PERM_LEAVE_ENTITLEMENT_ADD_ENTITLEMENTS, $account, $permission_data);
    if (!$add_entitlement_permission) {
      drupal_access_denied();
      exit();
    }
  }

  $entitlement_form = drupal_get_form('hrm_backend_leave_entitlement_form', $account, $entitlement);

  $variables = array(
    'entity' => HRM_ENTITY_LEAVE_ENTITLEMENT,
    'title' => $title,
    'form' => $entitlement_form,
  );

  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_leave_entitlement_form($form, &$form_state, $account, $entitlement) {
  $oid = $account['oid'];
  $filters = array(array('field' => 'oid', 'value' => $oid));

  // branch test

  if (!isset($form_state['storage']['transfered'])) {
    $employee_filters = array(
      array('field' => 'oid', 'value' => $oid, 'operator' => '='),
      array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='),
    );
    $form_state['storage']['employees'] = hrm_basic_employee_names($employee_filters);
    $form_state['storage']['leave_types'] = hrm_basic_leave_type_names($filters);
  }

  $update_disable_flag = (empty($entitlement) ? FALSE : TRUE);

  $form['employee'] = array(
    '#title' => 'Employee',
    '#type' => 'select',
    '#options' => $form_state['storage']['employees'],
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($entitlement['empid']) && isset($form_state['storage']['employees'][$entitlement['empid']]) ? $entitlement['empid'] : ''),
    '#disabled' => $update_disable_flag,
  );
  if (empty($entitlement)) {
    $form['employee']['#multiple'] = TRUE;
  }

  $form['leave_type'] = array(
    '#title' => 'Leave type',
    '#type' => 'select',
    '#options' => $form_state['storage']['leave_types'],
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($entitlement['leave_type']) && isset($form_state['storage']['leave_types'][$entitlement['leave_type']]) ? $entitlement['leave_type'] : ''),
    '#disabled' => $update_disable_flag,
  );

  $form['days'] = array(
    '#title' => 'Entitlements (Days)',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 11,
    '#default_value' => (isset($entitlement['days']) ? $entitlement['days'] : ''),
    '#element_validate' => array('element_validate_number'),
  );

  $form['justification'] = array(
    '#title' => 'Justification',
    '#type' => 'textarea',
    '#required' => TRUE,
    '#resizable' => FALSE,
    '#attributes' => array('class' => array('ckeditor')),
  );

  $attachment_location = HRM_DIR_TMP;
  if (isset($entitlement['eid']) && $entitlement['eid']) {
    $attachment_location = strtr(HRM_DIR_ORG_LEAVE_ENTITLEMENTS_ADJUSTMENTS, array('%oid' => $entitlement['oid'])) . '/' . $entitlement['eid'];
  }
  file_prepare_directory($attachment_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $form['attachment'] = array(
    '#title' => 'Attachment',
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_DOCUMENT)),
    '#upload_location' => $attachment_location
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($entitlement['eid']) && $entitlement['eid']) {
    $role_permissions = hrm_basic_role_permissions($account['role']);
    $permission_data = array('entitlement' => $entitlement, 'permissions' => $role_permissions);

    $delete_entitlement_permission = hrm_backend_leave_permissions(HRM_PERM_LEAVE_ENTITLEMENT_DELETE_ENTITLEMENTS, $account, $permission_data);
    if ($delete_entitlement_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#submit' => array('hrm_backend_leave_entitlements_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'leave-entitlement-form';

  return $form;
}

function hrm_backend_leave_entitlement_form_validate(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $entitlement = $form_state['build_info']['args'][1];

  if (empty($entitlement)) {
    $oid = $account['oid'];
    $leave_type = $form_state['values']['leave_type'];
    $leave_period = date(HRM_DATE_YEAR_YYYY, time());
    $empids = $form_state['values']['employee'];
    if (!is_array($empids)) {
      $empids = array($empids);
    }
    if (!empty($empids) && $leave_type) {
      $already_applyed_employees = array();
      foreach ($empids as $empid) {
        $entitlement_filter = array(
          array('field' => 'oid', 'value' => $oid, 'operator' => '='),
          array('field' => 'empid', 'value' => $empid, 'operator' => '='),
          array('field' => 'leave_type', 'value' => $leave_type, 'operator' => '='),
          array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
        );
        $entitlement = hrm_basic_leave_get_leave_entitlements($entitlement_filter, TRUE);
        if (!empty($entitlement)) {
          $already_applyed_employees[$empid] = $empid;
        }
      }
    }

    if (!empty($already_applyed_employees)) {
      $employees = $form_state['storage']['employees'];
      $message = 'Below employees already has entitlement to the selected leave type and leave period****';
      $message .= '<ul>';
      foreach ($already_applyed_employees as $empid) {
        $message .= '<li>' . $form_state['storage']['employees'][$empid] . '</li>';
      }
      $message .= '</ul>';
      form_set_error('employee', $message);
    }
  }
}

function hrm_backend_leave_entitlement_form_submit(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $entitlement = $form_state['build_info']['args'][1];
  $eid = (isset($entitlement['eid']) && $entitlement['eid'] ? $entitlement['eid'] : 0);

  $adjustment_data = array();

  $history_action = HRM_ACTION_ADD;


  $history_action = HRM_ACTION_ADD;
  if (isset($entitlement['eid']) && $entitlement['eid']) {
    db_update('hrm_leave_entitlement')
      ->fields(array(
        'days' => $form_state['values']['days'],
        'changed' => time()
      ))
      ->condition('eid', $entitlement['eid'], '=')
      ->execute();

    $history_action = HRM_ACTION_EDIT;

    drupal_set_message('Entitlement updated successfully.');
  }
  else {
    $empids = $form_state['values']['employee'];
    if (!is_array($empids)) {
      $empids = array($form_state['values']['employee']);
    }
    foreach ($empids as $empid) {
      $eid = db_insert('hrm_leave_entitlement')
        ->fields(array(
          'oid' => $account['oid'],
          'empid' => $empid,
          'leave_type' => $form_state['values']['leave_type'],
          'leave_period' => date(HRM_DATE_YEAR_YYYY, time()),
          'days' => $form_state['values']['days'],
          'transfered' => 0,
          'pending' => 0,
          'used' => 0,
        ))
        ->execute();

      $log_data = array(
        'title' => $eid,
        'leave_type' => $form_state['values']['leave_type'],
        'justification' => $form_state['values']['justification'],
      );
      hrm_backend_entity_history_log_action(HRM_ENTITY_LEAVE_ENTITLEMENT, $eid, HRM_ACTION_ADD, $account['uid'], $log_data);
    }

    drupal_set_message('Entitlement added successfully.');
  }

  db_insert('hrm_leave_entitlement_adjustment')
    ->fields(array(
      'eid' => $eid,
      'justification' => $form_state['values']['justification'],
      'fid' => $form_state['values']['attachment'],
      'data' => serialize($adjustment_data),
      'uid' => $account['uid'],
      'created' => time(),
    ))
    ->execute();

  $history_data = array(
    'justification' => $form_state['values']['justification'],
    'attachments' => array($form_state['values']['attachment']),
    'prev' => $entitlement,
    'new' => hrm_basic_leave_entitlement_get_data(array(array('field' => 'eid', 'value' => $eid)), TRUE)
  );
  hrm_backend_entity_history_log_action(HRM_ENTITY_LEAVE_ENTITLEMENT, $eid, $history_action, $account['uid'], $history_data);

  drupal_goto('leave/entitlement');
}

function hrm_backend_leave_entitlements_form_delete(&$form, &$form_state) {
  $entitlement = $form_state['build_info']['args'][1];
  db_delete('hrm_leave_entitlement')
    ->condition('eid', $entitlement['eid'], '=')
    ->execute();

  drupal_set_message('Entitlement deleted successfully');
  drupal_goto('leave/entitlement');
}

/*
 * ========================================
 * Entitlements
 * ========================================
 */

//Leaves

function hrm_backend_leaves_my_leaves_page($status = NULL) {
  global $base_url;

  $accesssible_status = array();
  if (!$status || $status == 'all') {


    $accesssible_status[] = HRM_STATUS_PENDING_TO_HR;
    $accesssible_status[] = HRM_STATUS_PENDING_TO_SUPERVISOR;
    $accesssible_status[] = HRM_STATUS_REJECTED;
    $accesssible_status[] = HRM_STATUS_SCHEDULED;
    $accesssible_status[] = HRM_STATUS_USED;
  }
  else {
    $access_permission = '';
    $title = t('Leaves');

    if ($status == 'pending') {
      $title = t('Pending Leaves');
      $accesssible_status[] = HRM_STATUS_PENDING_TO_SUPERVISOR;
      $accesssible_status[] = HRM_STATUS_PENDING_TO_HR;
    }
    elseif ($status == 'rejected') {
      $title = t('Rejected Leaves');
      $accesssible_status[] = HRM_STATUS_REJECTED;
    }
    elseif ($status == 'scheduled') {
      $title = 'Scheduled Leaves';
      $accesssible_status[] = HRM_STATUS_SCHEDULED;
    }
    elseif ($status == 'used') {
      $title = 'Used Leaves';
      $accesssible_status[] = HRM_STATUS_USED;
    }
    elseif ($status == 'cancelled') {
      $title = 'Cancelled Leaves';
      $accesssible_status[] = HRM_STATUS_CANCELED;
    }
    drupal_set_title($title);
  }


  $account = hrm_basic_account_get_info();
  $role_permissions = hrm_basic_role_permissions($account['role']);
  $permission_data = array(
    'permissions' => $role_permissions,
  );

  $accessible_lids_filters = array(array('field' => 'status', 'value' => $accesssible_status, 'operator' => 'IN'));


  $oid = $account['oid'];
  $filters = array(array('field' => 'oid', 'value' => $oid));
//print("<pre>");
//print_r($account['uid']);
//exit();
  $employee_filters = array(
    array('field' => 'oid', 'value' => $oid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE),
    array('field' => 'empid', 'value' => $account['uid']),
  );


  $date_format = HRM_DATE_SIMPLE_MONTH_NAME;
  $title = t('My Leaves');

  $query_filters = drupal_get_query_parameters();
  if (empty($query_filters)) {
    $current_month = date('m');
    $current_year = date('Y');
    $last_day = date('t');
    $query_filters['fr'] = hrm_basic_datetime_convert('01-' . $current_month . '-' . $current_year, $date_format);
    $query_filters['to'] = hrm_basic_datetime_convert($last_day . '-' . $current_month . '-' . $current_year, $date_format);
  }
  $filters_count = 0;
  if (TRUE) {
//From To Filters
    if (isset($query_filters['fr']) && isset($query_filters['to'])) {
      $from_date = hrm_basic_datetime_convert($query_filters['fr'], HRM_DATE_SYSTEM_DATE, $date_format);
      $to_date = hrm_basic_datetime_convert($query_filters['to'], HRM_DATE_SYSTEM_DATE, $date_format);
      $leave_filters[] = array('field' => 'start_date', 'value' => array($from_date, $to_date), 'operator' => 'BETWEEN');
      $filters_count += 2;

      $title .= ' (' . $query_filters['fr'] . ' - ' . $query_filters['to'] . ')';
    }
    elseif (isset($query_filters['fr'])) {
      $from_date = hrm_basic_datetime_convert($query_filters['fr'], HRM_DATE_SYSTEM_DATE, $date_format);
      $leave_filters[] = array('field' => 'start_date', 'value' => $from_date, 'operator' => '>=');
      $filters_count += 1;

      $title .= ' - ' . $query_filters['fr'];
    }
    elseif (isset($query_filters['to'])) {
      $to_date = hrm_basic_datetime_convert($query_filters['to'], HRM_DATE_SYSTEM_DATE, $date_format);
      $leave_filters[] = array('field' => 'start_date', 'value' => $to_date, 'operator' => '<=');
      $filters_count += 1;

      $title .= ' - ' . $query_filters['to'];
    }

//Leave types
    if (isset($query_filters['lt']) && !empty($query_filters['lt'])) {
      $leave_type_ids = explode('+', $query_filters['lt']);
      foreach ($leave_type_ids as $key => $leave_type_id) {
        if (!is_numeric($leave_type_id)) {
          unset($leave_type_ids[$key]);
        }
      }
      $leave_type_ids[] = 0;
      $leave_filters[] = array('field' => 'leave_type', 'value' => $leave_type_ids, 'operator' => 'IN');
      $filters_count += 1;
    }

//Status
    if (isset($query_filters['st']) && !empty($query_filters['st'])) {
      $temp_statuses = explode('+', $query_filters['st']);
      foreach ($temp_statuses as $key => $value) {
        if ($value) {
          unset($temp_statuses[$key]);
        }
      }
      $temp_statuses[] = 0;

      $employee_filters['status'] = array('field' => 'status', 'value' => $temp_statuses, 'operator' => 'IN');
      $filters_count += 1;
    }

//Job title
    if (isset($query_filters['jt']) && !empty($query_filters['jt'])) {
      $job_title_empids = db_select('hrm_emp_job', 'hrm_emp_job')
          ->fields('hrm_emp_job', array('empid'))
          ->condition('job_title', $query_filters['jt'], '=')
          ->execute()->fetchCol(0);

      $employee_filters[] = array('field' => 'empid', 'value' => $job_title_empids, 'operator' => 'IN');
      $filters_count += 1;
    }
  }
  $employees = hrm_basic_employee_get_data($employee_filters, FALSE, TRUE);


  $leaves = array();
  if (!empty($employees)) {
    $leave_filters[] = array('field' => 'empid', 'value' => $account['uid'], 'operator' => '=');
    $leaves = hrm_basic_leave_get_data($leave_filters, FALSE, TRUE);
  }

  $rows = array();
  $events = array();

  if (!empty($leaves)) {
    $lids = array_keys($leaves);

    $leave_types = hrm_basic_leave_type_get_data($filters);
    $status_names = hrm_basic_status_names();

    $status_classes = hrm_basic_status_classes();

    $leaves_attachments_query = db_select('hrm_entity_document', 'hrm_entity_document')
      ->fields('hrm_entity_document', array('entity_id'));
    $leaves_attachments_query->addExpression('COUNT(*)', 'counter');
    $leaves_attachments_query->condition('entity', HRM_ENTITY_LEAVE, '=');
    $leaves_attachments_query->condition('entity_id', $lids, 'IN');
    $leaves_attachments_query->groupBy('entity_id');
    $leaves_attachments = $leaves_attachments_query->execute()->fetchAllKeyed(0, 1);
//print("<pre>");
//print_r($role_permissions);
//exit();

  }

    $list = hrm_basic_leave_get_list($filters);

//$filters = array_merge($events)
  $tabs = array();
  $tabs['list'] = array(
    'title' => 'Leaves List',
    'content' => '<div class="table-responsive">' . theme('table', $list) . '</div>'
  );

//  $tabs['calendar'] = array(
//    'title' => 'Calendar',
//    'content' => theme('section_calendar', array('events' => $events))
//  );
  

    
  $filter_form = drupal_get_form('hrm_backend_leaves_filter_form', $account, 'my-leavs');

  $actions = array();
  if (hrm_backend_leave_permissions(HRM_PERM_LEAVE_APPLY_LEAVE, $account, $permission_data)) {
    $actions[] = l(HRM_FA_PLUS_BLUE . t('Apply Leave'), 'leave/apply', array('html' => TRUE));
  }

  $variables = array(
    'tabs' => $tabs,
    'actions' => array(),
    'filter_form' => $filter_form,
    'filters_count' => $filters_count,
    'actions' => $actions
  );

  drupal_set_title($title, PASS_THROUGH);
  $output = theme('page_wrapper_entity_tabs_js', $variables);

  return $output;
}

function hrm_backend_staff_leaves_page() {
  global $base_url;

  $account = hrm_basic_account_get_info();
  $role_permissions = hrm_basic_role_permissions($account['role']);
  $permission_data = array(
    'permissions' => $role_permissions,
  );

  $access_leaves = hrm_backend_leave_permissions(HRM_PERM_LEAVE_ACCESS_STAFF_LEAVES, $account, $permission_data);
  if (!$access_leaves) {
    drupal_access_denied();
    exit();
  }

  $oid = $account['oid'];
  $filters = array(array('field' => 'oid', 'value' => $oid));

  $employee_filters = array(
    array('field' => 'oid', 'value' => $oid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE),
  );
  $manage_leaves = FALSE;
  if ($account['role'] == HRM_ROLE_ADMIN || hrm_basic_employee_is_supervisor($account['uid'])) {
    $manage_leaves = TRUE;
  }

  $date_format = HRM_DATE_SIMPLE_MONTH_NAME;

  $title = 'Leaves';

  $query_filters = drupal_get_query_parameters();
  if (empty($query_filters)) {
    $current_month = date('m');
    $current_year = date('Y');
    $last_day = date('t');
    $query_filters['fr'] = hrm_basic_datetime_convert('01-' . $current_month . '-' . $current_year, $date_format);
    $query_filters['to'] = hrm_basic_datetime_convert($last_day . '-' . $current_month . '-' . $current_year, $date_format);
  }
  $filters_count = 0;
//Update employees and leaves filters
  if (TRUE) {
//From To Filters
    if (isset($query_filters['fr']) && isset($query_filters['to'])) {
      $from_date = hrm_basic_datetime_convert($query_filters['fr'], HRM_DATE_SYSTEM_DATE, $date_format);
      $to_date = hrm_basic_datetime_convert($query_filters['to'], HRM_DATE_SYSTEM_DATE, $date_format);
      $leave_filters[] = array('field' => 'start_date', 'value' => array($from_date, $to_date), 'operator' => 'BETWEEN');
      $filters_count += 2;

      $title .= ' (' . $query_filters['fr'] . ' - ' . $query_filters['to'] . ')';
    }
    elseif (isset($query_filters['fr'])) {
      $from_date = hrm_basic_datetime_convert($query_filters['fr'], HRM_DATE_SYSTEM_DATE, $date_format);
      $leave_filters[] = array('field' => 'start_date', 'value' => $from_date, 'operator' => '>=');
      $filters_count += 1;

      $title .= ' - ' . $query_filters['fr'];
    }
    elseif (isset($query_filters['to'])) {
      $to_date = hrm_basic_datetime_convert($query_filters['to'], HRM_DATE_SYSTEM_DATE, $date_format);
      $leave_filters[] = array('field' => 'start_date', 'value' => $to_date, 'operator' => '<=');
      $filters_count += 1;

      $title .= ' - ' . $query_filters['to'];
    }

//Leave types
    if (isset($query_filters['lt']) && !empty($query_filters['lt'])) {
      $leave_type_ids = explode('+', $query_filters['lt']);
      foreach ($leave_type_ids as $key => $leave_type_id) {
        if (!is_numeric($leave_type_id)) {
          unset($leave_type_ids[$key]);
        }
      }
      $leave_type_ids[] = 0;
      $leave_filters[] = array('field' => 'leave_type', 'value' => $leave_type_ids, 'operator' => 'IN');
      $filters_count += 1;
    }

//Employees
    if (isset($query_filters['empid']) && !empty($query_filters['empid'])) {
      $temp_empids = explode('+', $query_filters['empid']);
      foreach ($temp_empids as $key => $empid) {
        if (!is_numeric($empid)) {
          unset($temp_empids[$key]);
        }
      }
      $temp_empids[] = 0;

      $employee_filters[] = array('field' => 'empid', 'value' => $temp_empids, 'operator' => 'IN');
      $filters_count += 1;
    }

//Status
    if (isset($query_filters['st']) && !empty($query_filters['st'])) {
      $temp_statuses = explode('+', $query_filters['st']);
      foreach ($temp_statuses as $key => $value) {
        if ($value) {
          unset($temp_statuses[$key]);
        }
      }
      $temp_statuses[] = 0;

      $employee_filters['status'] = array('field' => 'status', 'value' => $temp_statuses, 'operator' => 'IN');
      $filters_count += 1;
    }

//Job title
    if (isset($query_filters['jt']) && !empty($query_filters['jt'])) {
      $job_title_empids = db_select('hrm_emp_job', 'hrm_emp_job')
          ->fields('hrm_emp_job', array('empid'))
          ->condition('job_title', $query_filters['jt'], '=')
          ->execute()->fetchCol(0);

      $employee_filters[] = array('field' => 'empid', 'value' => $job_title_empids, 'operator' => 'IN');
      $filters_count += 1;
    }
  }
  $employees = hrm_basic_employee_get_data($employee_filters, FALSE, TRUE);

  $leaves = array();
  if (!empty($employees)) {
    $empids = array_keys($employees);
    $leave_filters[] = array('field' => 'empid', 'value' => $empids, 'operator' => 'IN');
    $leaves = hrm_basic_leave_get_data($leave_filters, FALSE, TRUE);
  }

  $rows = array();
  $events = array();

  if (!empty($leaves)) {
    $lids = array_keys($leaves);

    $leave_types = hrm_basic_leave_type_get_data($filters);
    $status_names = hrm_basic_status_names();
    $status_classes = hrm_basic_status_classes();

    $leaves_attachments_query = db_select('hrm_entity_document', 'hrm_entity_document')
      ->fields('hrm_entity_document', array('entity_id'));
    $leaves_attachments_query->addExpression('COUNT(*)', 'counter');
    $leaves_attachments_query->condition('entity', HRM_ENTITY_LEAVE, '=');
    $leaves_attachments_query->condition('entity_id', $lids, 'IN');
    $leaves_attachments_query->groupBy('entity_id');
    $leaves_attachments = $leaves_attachments_query->execute()->fetchAllKeyed(0, 1);

    $access_leave_details = (isset($role_permissions[HRM_PERM_LEAVE_ACCESS_DETAILS]) ? TRUE : FALSE);

    foreach ($leaves as $lid => $leave) {

      $start_date = hrm_basic_datetime_convert($leave['start_date'], $date_format, HRM_DATE_SYSTEM_DATE);
      $end_date = hrm_basic_datetime_convert($leave['end_date'], $date_format, HRM_DATE_SYSTEM_DATE);
      $days = number_format($leave['days'], 4);
      $hours = $leave['hours'];

      $warning_content = '';
      $row_class = (isset($status_classes[$leave['status']]) ? $status_classes[$leave['status']] : array());

      if ($leave_types[$leave['leave_type']]['attachment_required'] == HRM_FLAG_YES && (!isset($leaves_attachments[$lid]) || !$leaves_attachments[$lid])) {
        $warning_content = HRM_FA_EXCLAMATION_TRIANGLE_RED;
        $row_class[] = 'danger';
      }



      $leave_link = '';
      $data_trigger_click_class = 'leave-modal-' . $lid;
//      if ($access_leave_details_permission) {
//        $leave_link = l(HRM_FA_SEARCH, 'hrm-modal', array('query' => array('action' => HRM_MODAL_LEAVE_DETAILS, 'lid' => $lid),
//          'attributes' => array('class' => array('btn', 'btn-xs', 'btn-primary', $data_trigger_click_class)), 'html' => TRUE));
//
//        $row_class[] = 'table-tr-trigger-click';
//      }
      $row_classes = array();
      $row_classes[] = ($access_leave_details ? 'table-tr-link' : '');
      $tr_data_url = ($access_leave_details ? url('leave/details/' . $lid) : '');

      $rows[] = array(
        'data' => array(
          'warning' => array('data' => $warning_content, 'class' => array('text-center')),
          'photo' => array('data' => $employees[$leave['empid']]['photo_image'], 'class' => array('cell-photo', 'cell-photo-circle')),
          'name' => array('data' => $employees[$leave['empid']]['name'], 'class' => array('nowrap')),
          'type' => array('data' => $leave_types[$leave['leave_type']]['name'], 'class' => array()),
          'start' => array('data' => $start_date, 'class' => array(), 'data-sort' => strtotime($leave['start_date'])),
          'end' => array('data' => $end_date, 'class' => array(), 'data-sort' => strtotime($leave['end_date'])),
          'days' => array('data' => $days, 'class' => array(), 'data-sort' => $leave['days']),
          'hours' => array('data' => $hours, 'class' => array(), 'data-sort' => $leave['hours']),
          'status' => array('data' => isset($status_names[$leave['status']]) ? $status_names[$leave['status']] : '', 'class' => array()),
          'link' => array('data' => $leave_link, 'class' => array('hidden')),
        ),
        'class' => $row_classes,
        'data-url' => $tr_data_url,
      );

      $event_title = $employees[$leave['empid']]['name'] . '<span class ="leave-status">' . $status_names[$leave['status']] . '</span>';
      $events[] = array(
        'title' => hrm_basic_text_fix_java_script_text($event_title),
        'start' => strtotime($leave['start_date']),
        'end' => strtotime($leave['end_date']),
        'url' => $base_url . '/leave/details/' . $leave['lid'],
        'color' => HRM_COLOR_PIMARY,
        'class' => '',
      );
    }
  }

  $header = array(
    array('data' => HRM_FA_EXCLAMATION_TRIANGLE, 'class' => array('nosort', 'nosearch', 'text-center')),
    array('data' => HRM_FA_USER, 'class' => array('nosort', 'nosearch', 'text-center')),
    array('data' => 'Employee'),
    array('data' => 'Leave type'),
    array('data' => 'From'),
    array('data' => 'To'),
    array('data' => 'Days'),
    array('data' => 'Hours'),
    array('data' => 'Status'),
    array('data' => 'Link', 'class' => array('hidden')),
  );

  $list = array(
    'attributes' => array('class' => array('table-hover', 'table-vcenter')),
    'rows' => $rows,
    'header' => $header,
    'sticky' => FALSE,
  );
  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all' : 'empty-datatable');

  $tabs = array();
//  $tabs['list'] = array(
//    'title' => 'Leaves List',
//    'content' => '<div class="table-responsive">' . theme('table', $list) . '</div>'
//  );

//  $tabs['calendar'] = array(
//    'title' => 'Calendar',
//    'content' => theme('section_calendar', array('events' => $events))
//  );

  $filter_form = drupal_get_form('hrm_backend_leaves_filter_form', $account, 'staff-leaves');

//  $actions = array();
//  if (hrm_backend_leave_permissions(HRM_PERM_LEAVE_APPLY_LEAVE, $account, $permission_data)) {
//    $actions[] = l(HRM_FA_PLUS_BLUE . t('Apply Leave'), 'leave/apply', array('html' => TRUE));
//  }

//  $variables = array(
//    'tabs' => $tabs,
//    'actions' => array(),
//    'filter_form' => $filter_form,
//    'filters_count' => $filters_count,
////    'actions' => $actions
//  );
  
    $variables = array(
    'tabs' => $tabs,
    'content' => theme('table', $list),
    'filter_form' => $filter_form,
    'filters_count' => $filters_count
  );

  return theme('page_wrapper_entity_tabs_links', $variables);
  
  

  drupal_set_title($title, PASS_THROUGH);
  $output = theme('page_wrapper_entity_tabs_links', $variables);

  return $output;
}

function hrm_backend_leaves_get_accessible_transaction($uid, $account = array(), $permissions = array(), $filters = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info($uid);
  }
  if (empty($permissions)) {
    $permissions = hrm_basic_user_get_role_permissions($account['role']);
  }


  $extra_cond = db_and();
  $extra_cond->condition('hrm_leave.lid', 0, '>');
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $table = (isset($filter['table']) ? $filter['table'] : 'med_cash_assist');
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $extra_cond->condition($table . '.' . $filter['field'], $filter['value'], $operator);
    }
  }

  $all_lids = array();
  $all_lids = db_select('hrm_leave')
      ->fields('hrm_leave', array('lid'))
      ->condition($extra_cond)->execute()->fetchCol(0);
  $all_lids[] = 0;
  return $all_lids;

 
//  $accessible_caids = array_merge($all_caids, $project_caids, $own_caids);
//
//  $caids = array_unique($accessible_caids);
//  $caids[] = 0;
//  return $caids;
}

function hrm_backend_leaves_filter_form($form, &$form_state, $account, $page_type) {
  $oid = $account['oid'];
  $filters = array(array('field' => 'oid', 'value' => $oid, 'operator' => '='));
  $employees_filters = $filters;
  $status_names = hrm_basic_status_names();

  $leave_statuses = array(
    HRM_STATUS_PENDING_TO_SUPERVISOR => $status_names[HRM_STATUS_PENDING_TO_SUPERVISOR],
    HRM_STATUS_PENDING_TO_HR => $status_names[HRM_STATUS_PENDING_TO_HR],
    HRM_STATUS_REJECTED => $status_names[HRM_STATUS_REJECTED],
    HRM_STATUS_SCHEDULED => $status_names[HRM_STATUS_SCHEDULED],
    HRM_STATUS_USED => $status_names[HRM_STATUS_USED],
  );


  $employees = hrm_basic_employee_names($employees_filters);
  $job_titles = hrm_basic_job_title_names($filters);
  $leave_types = hrm_basic_leave_type_names($filters);

  $date_format = HRM_DATE_SIMPLE;
  $date_picker_format = 'dd-mm-yyyy';


  $query_filters = drupal_get_query_parameters();
  $form['fr'] = array(
    '#title' => 'From date',
    '#type' => 'textfield',
    '#default_value' => (isset($query_filters['fr']) && !empty($query_filters['fr']) ? $query_filters['fr'] : date($date_format)),
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => $date_picker_format),
  );


  $form['to'] = array(
    '#title' => 'To date',
    '#type' => 'textfield',
    '#default_value' => (isset($query_filters['to']) && !empty($query_filters['to']) ? $query_filters['to'] : date($date_format)),
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => $date_picker_format),
  );

  $default_leave_types = array();
  if (isset($query_filters['lt']) && !empty($query_filters['lt'])) {
    $default_leave_types = explode('+', $query_filters['lt']);
    foreach ($default_leave_types as $key => $value) {
      if (!$value || !isset($leave_types[$value])) {
        unset($default_leave_types[$key]);
      }
    }
  }

  $form['lt'] = array(
    '#title' => 'Leave Type',
    '#type' => 'select',
    '#options' => $leave_types,
    '#empty_option' => 'All',
    '#empty_value' => 'All',
    '#multiple' => TRUE,
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => $default_leave_types,
  );

  $default_status = array();
  if (isset($query_filters['st']) && !empty($query_filters['st'])) {
    $default_status = explode('+', $query_filters['st']);
    foreach ($default_status as $key => $value) {
      if (!$value || !isset($leave_statuses[$value])) {
        unset($default_status[$key]);
      }
    }
  }
  $form['st'] = array(
    '#title' => 'Leave Status',
    '#type' => 'checkboxes',
    '#options' => $leave_statuses,
    '#attributes' => array('class' => array()),
    '#default_value' => $default_status,
  );

  if ($page_type == 'staff-leaves') {
    $default_empids = array();
    if (isset($query_filters['empid']) && !empty($query_filters['empid'])) {
      $default_empids = explode('+', $query_filters['empid']);
      foreach ($default_empids as $key => $value) {
        if (!$value || !isset($employees[$value])) {
          unset($default_empids[$key]);
        }
      }
    }
    $form['empid'] = array(
      '#title' => 'Employee',
      '#type' => 'select',
      '#options' => $employees,
      '#empty_option' => 'All',
      '#empty_value' => 'All',
      '#multiple' => TRUE,
      '#default_value' => $default_empids,
      '#attributes' => array('class' => array('select-chosen')),
    );
  }



  $form['jt'] = array(
    '#title' => 'Job Title',
    '#type' => 'select',
    '#options' => $job_titles,
    '#empty_option' => 'All',
    '#empty_value' => 'All',
    '#default_value' => (isset($query_filters['jt']) && isset($job_titles[$query_filters['jt']]) ? $query_filters['jt'] : ''),
    '#attributes' => array('class' => array('select-chosen')),
  );





  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'View',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm', 'width-49'))
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => 'Reset',
    '#attributes' => array('class' => array('btn-warning', 'btn-sm', 'width-49')),
    '#limit_validation_errors' => array(),
    '#submit' => array('hrm_backend_leaves_filter_form_reset'),
  );

  $form['#attributes']['class'][] = 'leave-entitlments-filter-form';

  return $form;
}

function hrm_backend_leaves_filter_form_submit($form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $page_type = $form_state['build_info']['args'][1];

  $date_format = HRM_DATE_SIMPLE;
  $now_date = date($date_format);

  $query_filters = array();
  if (isset($form_state['values']['fr']) && !empty($form_state['values']['fr'])) {
    $query_filters['fr'] = (($form_state['values']['fr'] != '') ? $form_state['values']['fr'] : $now_date);
  }
  if (isset($form_state['values']['to']) && !empty($form_state['values']['to'])) {
    $query_filters['to'] = (($form_state['values']['to'] != '') ? $form_state['values']['to'] : $now_date);
  }
  if (isset($form_state['values']['lt']) && !empty($form_state['values']['lt'])) {
    $lts = array();
    foreach ($form_state['values']['lt'] as $lt) {
      if ($lt) {
        $lts[$lt] = $lt;
      }
    }
    if (!empty($lts)) {
      $query_filters['lt'] = implode('+', $lts);
    }
  }
  if (isset($form_state['values']['st']) && !empty($form_state['values']['st'])) {
    $sts = array();
    foreach ($form_state['values']['st'] as $st) {
      if ($st) {
        $sts[$st] = $st;
      }
    }
    if (!empty($sts)) {
      $query_filters['st'] = implode('+', $sts);
    }
  }
  if ($page_type == 'staff-leaves') {
    if (isset($form_state['values']['empid']) && !empty($form_state['values']['empid'])) {
      $empids = array();
      foreach ($form_state['values']['empid'] as $empid) {
        if ($empid) {
          $empids[$empid] = $empid;
        }
      }
      if (!empty($empids)) {
        $query_filters['empid'] = implode('+', $empids);
      }
    }
  }
  if (!empty($form_state['values']['jt']) && is_numeric($form_state['values']['jt'])) {
    $query_filters['jt'] = $form_state['values']['jt'];
  }


  drupal_goto('leave/leave', array('query' => $query_filters));
}

function hrm_backend_leaves_filter_form_reset($form, &$form_state) {
  drupal_goto('leave/leave');
}

////////////////////////////////////////////////////////////////////////

function hrm_backend_leave_entitlements_get_list($data, $employee = 0, $type = 0, $leave_period = 0) {
  $account = $data['account'];
  $employees = $data['employees'];
  $active_empids = array_keys($employees);
  $active_empids[] = 0;
  $types = $data['types'];
  $oid = $account['oid'];

  if (!$leave_period) {
    $leave_period = date(HRM_DATE_YEAR_YYYY, time());
  }

  $entitlement_filters = array(
    array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
    array('field' => 'oid', 'value' => $oid, 'operator' => '='),
    array('field' => 'empid', 'value' => $active_empids, 'operator' => 'IN'),
  );
  if ($employee) {
    $entitlement_filters[] = array('field' => 'empid', 'value' => $employee, 'operator' => '=');
  }
  if ($type) {
    $entitlement_filters[] = array('field' => 'leave_type', 'value' => $type, 'operator' => '=');
  }
  $entitlements = hr_basic_leave_get_leave_entitlements($entitlement_filters);
  $update_leave_entitlements = user_access(HR_PERM_LEAVE_EDIT_LEAVE_ENTITLEMENTS);
  $rows = array();
  foreach ($entitlements as $entitlement) {
    $empid = $entitlement['empid'];

    $edit_link = ($update_leave_entitlements ? l('<i class="fa fa-pencil"></i>', 'leave/entitlements/edit/' . $entitlement['id'], array('attributes' => array('class' => array('btn', 'btn-xs', 'btn-primary')), 'html' => TRUE)) : '');

    $row_class = '';
    if ($entitlement['balance'] < 0) {
      $row_class = 'danger';
    }
    $rows[] = array(
      'data' => array(
        'picture' => array('data' => $employees[$empid]['picture_image'], 'class' => array('text-center')),
        'employee' => array('data' => $employees[$empid]['name']),
        'leave' => array('data' => $types[$entitlement['leave_type']]),
        'period' => array('data' => $entitlement['leave_period']),
        'transfered' => array('data' => number_format($entitlement['transfered'], 2)),
        'pending' => array('data' => number_format($entitlement['pending'], 2)),
        'taken' => array('data' => number_format($entitlement['taken'], 2)),
        'currently_allowed' => array('data' => number_format($entitlement['balance'], 2) . ' ' . $edit_link),
        'future_accrue' => array('data' => number_format($entitlement['future_accrue'], 2)),
        'total' => array('data' => number_format(($entitlement['future_accrue'] + $entitlement['taken']), 2)),
      ),
      'class' => array($row_class)
    );
  }
  $header = array(
    array('data' => '<i class="fa fa-user-circle-o"></i>', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Employee', 'class' => array()),
    array('data' => 'Leave'),
    array('data' => 'Period'),
    array('data' => 'Transferred', 'title' => 'Transfered from preview year'),
    array('data' => 'Pending'),
    array('data' => 'Taken'),
    array('data' => 'Available balance'),
    array('data' => 'Accrued balance'),
    array('data' => 'Total'),
  );

  $list = array(
    'attributes' => array('class' => array('table', 'table-bordered', 'table-striped', 'table-hover')),
    'rows' => $rows,
    'header' => $header
  );

  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-custom-export';
  }


  $actions = array();
  $actions[] = l('<i class = "fa fa-plus"></i> Add Entitlement', 'leave/entitlements/add', array(
    'attributes' => array('class' => array('btn', 'btn-success', 'btn-sm', 'push-5-r'), 'data-action' => 'refresh_toggle', 'data-toggle' => 'block-option',),
    'html' => TRUE));

  $actions_output = '';
  $content = $actions_output;
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }
  else {
    $list['rows'][]['data'] = array(
      'leave_type' => array('data' => 'No Records Found'),
      'leave_period' => array('data' => ''),
      'days' => array('data' => ''),
      'balance' => array('data' => ''),
      'remaining' => array('data' => ''),
    );
  }
  $content .= theme('table', $list);
  $variables = array('content' => $content);
  return $variables;
}

/*
 * Leaves
 */

//// Mohamed

function hrm_backend_leave_apply_leave_page() {


  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $apply_leave_form = drupal_get_form('hrm_backend_leave_apply_leave_form', $account);

  $variables = array(
    'entity' => HRM_ENTITY_LEAVE,
    'title' => 'Apply Leave Information',
    'form' => $apply_leave_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_leave_apply_leave_form($form, &$form_state, $account) {


  $form['#attributes']['autocomplete'] = 'off';
  $leave_type_options = array();

  $empid = $account['uid'];
  $form_state['storage']['employee'] = hrm_basic_employee_get_data(array(array('field' => 'empid', 'value' => $empid, 'operator' => '=')), TRUE);
  $employee = $form_state['storage']['employee'];

//    print("<pre>");
//  print_r($employee);
//  exit();
  $leave_filters = array(
    array('field' => 'oid', 'value' => $account['oid']),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE),
//      array('field' => 'admin_apply', 'value' => HRM_FLAG_YES),
  );

  $form_state['storage']['leave_types'] = hrm_basic_leave_type_get_data($leave_filters);

  foreach ($form_state['storage']['leave_types'] as $lid => $leave_type) {
//      if ($leave_type['country'] == $employee['country']) {
    if ($leave_type['gender'] && $leave_type['gender'] != $employee['gender']) {
      continue;
    }
    $leave_type_options[$lid] = $leave_type['name'];
//      }


    $form['leave_type'] = array(
      '#title' => 'Leave Type',
      '#type' => 'select',
      '#options' => $leave_type_options,
      '#required' => TRUE,
      '#empty_option' => '',
      '#attributes' => array('class' => array('select-chosen'), 'data-placeholder' => 'select leave type'),
//      '#ajax' => array(
//        'callback' => 'hrm_backend_leave_apply_leave_form_callback',
//        'wrapper' => 'apply-leave-form-wrapper',
//      ),
    );
  }

  $form['start_date'] = array(
    '#title' => 'Start Date',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#empty_option' => '',
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => 'dd-mm-yyyy', 'placeholder' => 'dd-mm-yyyy'),
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'hrm_backend_leave_apply_leave_form_start_date_change_callback',
      'wrapper' => 'apply-leave-form-wrapper'
    ),
  );

  $form['end_date'] = array(
    '#title' => 'End Date',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#empty_option' => '',
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => 'dd-mm-yyyy', 'placeholder' => 'dd-mm-yyyy'),
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'hrm_backend_leave_apply_leave_form_end_date_change_callback',
      'wrapper' => 'apply-leave-form-wrapper'
    ),
  );

  if (isset($form_state['values']['leave_type']) && !empty($form_state['values']['leave_type'])) {

//    $leave_type_id = $form_state['values']['leave_type'];
//    $leave_type = $form_state['storage']['leave_types'][$leave_type_id];
//    $leave_type = hrm_basic_leave_type_get_data(array(array('field' => 'id', 'value' => $leave_type_id)), TRUE);
  }

  if (!empty($form_state['values']['start_date']) && !empty($form_state['values']['end_date'])) {
    $duration_options = array();
    if ($leave_type['dur_full_day'] == 1) {
      $duration_options[HRM_LEAVE_DURATION_FULL_DAY] = 'Full day';
    }
    if ($leave_type['dur_half_day'] == 1) {
      $duration_options[HRM_LEAVE_DURATION_HALF_DAY] = 'Half day';
    }
    if ($leave_type['dur_time'] == 1) {
      $duration_options[HRM_LEAVE_DURATION_TIME] = 'Specify time';
    }

    $half_day_options = array(
      HRM_LEAVE_HALF_DAY_MORNING => 'Morning',
      HRM_LEAVE_HALF_DAY_AFTERNOON => 'Afternoon',
    );

    $start_date = str_replace('/', '-', $form_state['values']['start_date']);
    $end_date = str_replace('/', '-', $form_state['values']['end_date']);

    $start_date_timestamp = strtotime($start_date);
    $end_date_timestamp = strtotime($end_date);

    if ($end_date_timestamp < $start_date_timestamp) {
      $form['duration']['start_date']['#value'] = $form_state['values']['end_date'];
      $form_state['values']['start_date'] = $form_state['values']['end_date'];
      $form_state['input']['start_date'] = $form_state['values']['end_date'];

      $start_date_timestamp = $end_date_timestamp;
      $start_date = $end_date;
    }

    $form['duration'] = array(
      '#title' => 'Duration',
      '#type' => 'select',
      '#required' => TRUE,
      '#options' => $duration_options,
      '#empty_option' => '',
      '#attributes' => array(
        'class' => array(
          'select-chosen'),
        'data-placeholder' => ''
      ),
      '#ajax' => array(
        'callback' => 'hrm_backend_leave_apply_leave_form_duration_change_callback',
        'wrapper' => 'apply-leave-form-wrapper'
      ),
    );


    if (isset($form_state['values']['duration']) && !empty($form_state['values']['duration'])) {
      $duration = $form_state['values']['duration'];
      if ($duration == HRM_LEAVE_DURATION_FULL_DAY) {
        
      }
      elseif ($duration == HRM_LEAVE_DURATION_HALF_DAY) {
        $form['duration_half_day'] = array(
          '#title' => 'Half day duration',
          '#type' => 'select',
          '#required' => TRUE,
          '#attributes' => array(
            'class' => array('select-chosen'),
            'data-placeholder' => ''
          ),
          '#default_value' => HRM_LEAVE_HALF_DAY_MORNING,
          '#options' => $half_day_options,
        );
      }
      elseif ($duration == HRM_LEAVE_DURATION_TIME) {

        if (isset($form_state['values']['employee']) && !empty($form_state['values']['employee'])) {
          $empid = $form_state['values']['employee'];
          $temp_employee = array();
          if (isset($form_state['storage']['employees'][$empid])) {
            $temp_employee = $form_state['storage']['employees'][$empid];
          }

          $employee_work_shift = hrm_basic_workshift_get_employee_workshift($employee['work_shift']);

          if (!empty($employee_work_shift)) {
            $default_from_time = $employee_work_shift['from_time'];
            $default_to_time = $employee_work_shift['to_time'];
            $default_duration = $employee_work_shift['duration'];
          }
        }
        $form['duration_time_from'] = array(
          '#type' => 'textfield',
          '#title' => 'From time',
          '#required' => TRUE,
          '#prefix' => '<div class="bootstrap-timepicker">',
          '#suffix' => '</div>',
          '#default_value' => $default_from_time,
          '#attributes' => array('class' => array($form_state['storage']['timepicker_class']), 'placeholder' => 'Duration time from'),
          '#class' => array('max-width-100-px'));

        $form['duration_time_to'] = array(
          '#type' => 'textfield',
          '#title' => 'To time',
          '#required' => TRUE,
          '#prefix' => '<div class="bootstrap-timepicker">',
          '#suffix' => '</div>',
          '#default_value' => $default_to_time,
          '#attributes' => array('class' => array($form_state['storage']['timepicker_class']), 'placeholder' => 'Duration time to'),
        );
      }
    }
  }

  $form['comment'] = array(
    '#title' => 'Comment',
    '#type' => 'textarea',
    '#required' => TRUE,
    '#resizable' => FALSE,
    '#attributes' => array('class' => array('ckeditor')),
  );

  $form['submit_suffix'] = array('#markup' => '<div class="form-group">');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Assign Leave',
    '#attributes' => array(
      'class' => array('btn-primary', 'btn-sm')
    )
  );
  $form['#prefix'] = '<div id="apply-leave-form-wrapper">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'form-bordered';

  return $form;
}

function hrm_backend_leave_apply_leave_form_validate(&$form, &$form_state) {
  if (!isset($form_state['values']['duration']) || empty($form_state['values']['duration'])) {
    form_set_error('duration', 'Duration field is required');
  }


  $account = $form_state['build_info']['args'][0];
  $oid = $account['oid'];
  $empid = $account['uid'];
  $employee = $form_state['storage']['employee'];

  $leave_type_id = $form_state['values']['leave_type'];
  $form_state['storage']['leave_type'] = hrm_basic_leave_type_get_data(array(array('field' => 'id', 'value' => $leave_type_id)), TRUE);
  $leave_type = $form_state['storage']['leave_type'];

  $emp_join_date = $employee['join_date'];
  $leave_due_date = $leave_type['due_date'];
  $time = time();




  $form_state['storage']['assigned_to'] = $employee['supervisor_id'];
  $org_date_format = HRM_DATE_SIMPLE;
  $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));

  $start_date = hrm_basic_datetime_convert($form_state['values']['start_date'], HRM_DATE_SYSTEM_DATE, $org_date_format);
  $end_date = hrm_basic_datetime_convert($form_state['values']['end_date'], HRM_DATE_SYSTEM_DATE, $org_date_format);

//  $from_date = hrm_basic_datetime_convert($form_state['values']['start_date'], HRM_DATE_SIMPLE, $org_date_format);
//  $to_date = hrm_basic_datetime_convert($form_state['values']['end_date'], HRM_DATE_SIMPLE, $org_date_format);

  $duration = $form_state['values']['duration'];

  switch ($duration) {
    case HRM_LEAVE_DURATION_FULL_DAY:
      $temp_from_time = '00:00:00';
      $temp_to_time = '00:00:00';

      break;
    case HRM_LEAVE_DURATION_TIME:
      $temp_from_time = date(HRM_DATE_SYSTEM_TIME, strtotime($form_state['values']['duration_time_from']));
      $temp_to_time = date(HRM_DATE_SYSTEM_TIME, strtotime($form_state['values']['duration_time_to']));
      if ($temp_from_time > $temp_to_time) {
        form_set_error('duration_time_from', 'Sorry! you select wrong duration time***');
      }
      break;
  }



//    $form_state['storage']['assigned_to'] = 0;
//    switch ($leave_type['approval_method']) {
//      case HRM_LEAVE_APPROVAL_METHOD_REPORT_TO :
//        $report_to_employee = db_select('hrm_emp_reporting')
//            ->fields('hrm_emp_reporting', array('supervisor_empid'))
//            ->condition('method', $leave_type['approval_value'], '=')
//            ->condition('empid', $empid, '=')
//            ->execute()->fetchField();
//        if (empty($report_to_employee)) {
//          form_set_error('', 'No approving supervisor has been applyed for the selected employee. Please contact your HR administrator***');
//        }
//        $form_state['storage']['assigned_to'] = $report_to_employee;
//
//        break;
//      case HRM_LEAVE_APPROVAL_METHOD_EMPLOYEE:
//        $form_state['storage']['assigned_to'] = $leave_type['approval_value'];
//        break;
//    }
  $entitlements_filters = array(
    array('field' => 'empid', 'value' => $account['uid'], 'operator' => '='),
    array('field' => 'leave_type', 'value' => $form_state['values']['leave_type'], 'operator' => '='),
    array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '=')
  );
  $form_state['storage']['entitlements'] = hrm_basic_leave_get_leave_entitlements($entitlements_filters, TRUE);
  $entitlements = $form_state['storage']['entitlements'];
//    $e = hrm_backend_leave_entitlement_

  if (empty($entitlements)) {
    if (!$leave_type['beyond_balance']) {
      form_set_error('', 'Sorry! you have no leave entitltmens for this leave type***');
    }
  }
  else {
    if (!$leave_type['beyond_balance']) {

      $all_days = hrm_basic_datetime_days_range($start_date, $end_date);

      $total_hours = 0;
      $total_days = 0;


      foreach ($all_days as $day) {
        $include_day = TRUE;
        $day_status = hrm_basic_day_status($oid, $day);
        if ($day_status == HRM_DAY_STATUS_HOLIDAY && !$leave_type['holidays_bd']) {
          $include_day = FALSE;
        }
        if ($day_status == HRM_DAY_STATUS_WEEKEND && !$leave_type['weekend_bd']) {
          $include_day = FALSE;
        }

        if (!$include_day) {
          continue;
        }

        switch ($duration) {
          case HRM_LEAVE_DURATION_FULL_DAY:
            $total_hours += '08:00';
            $total_days ++;
            break;
          case HRM_LEAVE_DURATION_TIME:
            $leave_start_timestamp = strtotime($start_date . ' ' . $form_state['values']['duration_time_from']);
            $leave_end_timestamp = strtotime($start_date . ' ' . $form_state['values']['duration_time_to']);
            $leave_duration_timestamp = $leave_end_timestamp - $leave_start_timestamp;

            $temp_duration = hrm_basic_time_timestamp_to_time($leave_duration_timestamp);
            $temp_hours_array = explode(':', $temp_duration);
            $temp_hours_minutes = $temp_hours_array[1];
            $temp_minutes = $temp_hours_minutes / 60;
            $temp_hours_float = $temp_hours_array[0] + $temp_minutes;

            $temp_hours = $temp_hours_float;
            $temp_days = $leave_duration_timestamp / 28800; // 8Ã—8Ã—60; backhere

            $total_hours += $temp_hours;
            $total_days += $temp_days;

            break;
        }
      }

      if ($entitlements['days'] - $entitlements['pending'] < $total_days) {
        form_set_error('employee', t('Sorry! you have insufficient leave balance'));
      }
    }
  }
}

function hrm_backend_leave_apply_leave_form_submit(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $oid = $account['oid'];
  $empid = $account['uid'];
  $leave_type = $form_state['values']['leave_type'];
  $leave_type_data = $form_state['storage']['leave_types'][$leave_type];
  $entitlements = $form_state['storage']['entitlements'];
  $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));

  $employee = hrm_basic_employee_get_data(array(array('field' => 'empid', 'value' => $empid)), TRUE);
  $org_date_format = HRM_DATE_SIMPLE;
  $org_time_format = HRM_DATE_TIME_12;

  $start_date = hrm_basic_datetime_convert($form_state['values']['start_date'], HRM_DATE_SYSTEM_DATE, $org_date_format);
  $end_date = hrm_basic_datetime_convert($form_state['values']['end_date'], HRM_DATE_SYSTEM_DATE, $org_date_format);

  $from_date = $start_date;
  $to_date = $end_date;

  $duration = $form_state['values']['duration'];

  $all_days = hrm_basic_datetime_days_range($start_date, $end_date);
  $timing = array();
  $total_hours = 0;
  $total_days = 0;

//  $work_shift_start_timestamp = strtotime($start_date . ' ' . $work_shift['from_time']);
//  $work_shift_end_timestamp = strtotime($start_date . ' ' . $work_shift['to_time']);
//  $work_shift_duration_timestamp = $work_shift_end_timestamp - $work_shift_start_timestamp;
//  $work_shift_duration_half_day_timestamp = $work_shift_duration_timestamp / 2;
//
//  $work_shift_hours_array = explode(':', $work_shift['duration']);
//  $work_shift_hours_minutes = $work_shift_hours_array[1];
//  $work_shift_minutes = $work_shift_hours_minutes / 60;
//  $work_shift_hours_float = $work_shift_hours_array[0] + $work_shift_minutes;

  switch ($duration) {
    case HRM_LEAVE_DURATION_FULL_DAY:
      $temp_from_time = '9:00 AM';
      $temp_to_time = '5:00 PM';
      $temp_duration = '08:00';
      $temp_hours = 8; // backhere Ø¹Ø¯Ø¯ Ø³Ø§Ø¹Ø§Øª Ø§Ù„Ø¯ÙˆØ§Ù…
      $temp_days = 1.00;
      $start_date = $start_date . ' ' . '9:00 AM';
      $end_date = $end_date . ' ' . '5:00 PM';
      $duration_type = HRM_LEAVE_DURATION_FULL_DAY;

      break;
    case HRM_LEAVE_DURATION_TIME:
      $leave_start_timestamp = strtotime($start_date . ' ' . $form_state['values']['duration_time_from']);
      $leave_end_timestamp = strtotime($start_date . ' ' . $form_state['values']['duration_time_to']);
      $leave_duration_timestamp = $leave_end_timestamp - $leave_start_timestamp;

      $start_date = $start_date . ' ' . $form_state['values']['duration_time_from'];
      $end_date = $end_date . ' ' . $form_state['values']['duration_time_to'];
      $duration_type = HRM_LEAVE_DURATION_TIME;

      $temp_from_time = $form_state['values']['duration_time_from'];
      $temp_to_time = $form_state['values']['duration_time_to'];

      $temp_duration = hrm_basic_time_timestamp_to_time($leave_duration_timestamp);

      $temp_hours_array = explode(':', $temp_duration);
      $temp_hours_minutes = $temp_hours_array[1];
      $temp_minutes = $temp_hours_minutes / 60;
      $temp_hours_float = $temp_hours_array[0] + $temp_minutes;

      $temp_hours = $temp_hours_float;
      $temp_days = $leave_duration_timestamp / 28800;
      break;
  }

  $temp_from_time = hrm_basic_datetime_convert($temp_from_time, HRM_DATE_SYSTEM_TIME, $org_time_format);
  $temp_to_time = hrm_basic_datetime_convert($temp_to_time, HRM_DATE_SYSTEM_TIME, $org_time_format);

  foreach ($all_days as $day) {
    $day_date = hrm_basic_datetime_convert($day, HRM_DATE_SYSTEM_DATE, HRM_DATE_SIMPLE);
    $include_day = TRUE;
    $day_status = hrm_basic_day_status($oid, $day);
    if ($day_status == HRM_DAY_STATUS_HOLIDAY && !$leave_type_data['holidays_bd']) {
      $include_day = FALSE;
    }
    if ($day_status == HRM_DAY_STATUS_WEEKEND && !$leave_type_data['weekend_bd']) {
      $include_day = FALSE;
    }

    if (!$include_day) {
      continue;
    }

    switch ($duration) {
      case HRM_LEAVE_DURATION_FULL_DAY:
        $total_hours += '08:00';
        $total_days ++;
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

  if (!empty($timing)) {

    hrm_basic_leave_entitlement_register_pending_leave($empid, $oid, $leave_type, $leave_period, $total_days, HRM_STATUS_PENDING_TO_SUPERVISOR);

    if (empty($entitlements)) {
      $entitlements_filters = array(
        array('field' => 'empid', 'value' => $empid, 'operator' => '='),
        array('field' => 'leave_type', 'value' => $leave_type, 'operator' => '='),
        array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
      );
      $entitlements = hrm_basic_leave_get_leave_entitlements($entitlements_filters, TRUE);
    }
    $assigned_to = 0;
    if ($form_state['storage']['assigned_to']) {
      $assigned_to = $form_state['storage']['assigned_to'];
    }

    $lid = db_insert('hrm_leave')
      ->fields(array(
        'oid' => $account['oid'],
        'empid' => $empid,
        'leave_type' => $leave_type,
        'hours' => $total_hours,
        'days' => $total_days,
        'start_date' => $from_date,
        'end_date' => $to_date,
        'assigned_to' => $assigned_to,
        'leave_entitlement' => $entitlements['eid'],
        'leave_period' => $leave_period,
        'source' => HRM_DATA_SOURCE_WEB,
        'color_code' => HRM_COLOR_PIMARY,
        'comment' => $form_state['values']['comment'],
        'status' => HRM_STATUS_PENDING_TO_SUPERVISOR,
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();

    foreach ($timing as $leave_timing) {

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
          'duration_type' => $duration_type,
          'status' => HRM_STATUS_PENDING_TO_SUPERVISOR
        ))
        ->execute();


// ============================================================================================================
// ------------------------------------------------------------------------------------------------------------
// ============================================================================================================
    }
    db_insert('hrm_leave_action')
      ->fields(array(
        'lid' => $lid,
        'action' => HRM_ACTION_APPLY,
        'comment' => $form_state['values']['comment'],
        'created_entity' => HRM_ENTITY_USER,
        'created_entity_id' => $account['uid'],
        'created' => time(),
      ))
      ->execute();

    $log_data = array(
      'title' => $lid,
      'empid' => $empid,
      'assigned_to' => $form_state['storage']['assigned_to'],
      'total_days' => $total_days,
      'total_hours' => $total_hours,
      'leave_type' => $leave_type,
      'justification' => $form_state['values']['comment'],
    );
    hrm_backend_entity_history_log_action(HRM_ENTITY_LEAVE, $lid, HRM_ACTION_APPLY, $account['uid'], $log_data, time());

    drupal_set_message('Leave Saved Successfully.***');
    drupal_goto('leave/leave');
  }
  else {
    drupal_set_message('Invalid leave data***', 'error');
  }
}

function hrm_backend_leave_apply_leave_form_callback($form, $form_state) {
  return $form;
}

function hrm_backend_leave_apply_leave_form_start_date_change_callback(&$form, &$form_state) {
  if (!empty($form_state['values']['start_date']) && !empty($form_state['values']['end_date'])) {
    $start_date = hrm_basic_datetime_convert($form_state['values']['start_date'], HRM_DATE_SYSTEM_DATE, $form_state['storage']['date_format']);
    $end_date = hrm_basic_datetime_convert($form_state['values']['end_date'], HRM_DATE_SYSTEM_DATE, $form_state['storage']['date_format']);

    if (strtotime($end_date) < strtotime($start_date)) {
      $form['end_date']['#value'] = $form_state['values']['start_date'];
      $form_state['values']['end_date'] = $form_state['values']['start_date'];
      $form_state['input']['end_date'] = $form_state['values']['start_date'];
    }
  }

  return $form;
}

function hrm_backend_leave_apply_leave_form_end_date_change_callback(&$form, &$form_state) {
  if (!empty($form_state['values']['start_date']) && !empty($form_state['values']['end_date'])) {
    $start_date = hrm_basic_datetime_convert($form_state['values']['start_date'], HRM_DATE_SYSTEM_DATE, $form_state['storage']['date_format']);
    $end_date = hrm_basic_datetime_convert($form_state['values']['end_date'], HRM_DATE_SYSTEM_DATE, $form_state['storage']['date_format']);

    if (strtotime($end_date) < strtotime($start_date)) {
      $form['start_date']['#value'] = $form_state['values']['end_date'];
      $form_state['values']['start_date'] = $form_state['values']['end_date'];
      $form_state['input']['start_date'] = $form_state['values']['end_date'];
    }
  }

  return $form;
}

function hrm_backend_leave_apply_leave_form_duration_change_callback(&$form, &$form_state) {
  return $form;
}

/*
 * Cancel Leave
 */

// mohamed
function hrm_backend_leave_cancel_leave_form($form, &$form_state, $leave, $account) {
  drupal_set_title('Cancel Leave');
  $role_permissions = hrm_basic_role_permissions($account['role']);
  $permission_data = array('leave' => $leave, 'permissions' => $role_permissions);

  $cancel_leave_permission = hrm_backend_leave_permissions(HRM_PERM_LEAVE_CANCEL_LEAVE, $account, $permission_data);

  if (!$cancel_leave_permission) {
    hrm_backend_modal_access_denied();
  }

  $form['justification'] = array(
    '#title' => 'Justification',
    '#type' => 'textarea',
    '#required' => TRUE,
    '#maxlength' => 400,
    '#rows' => 3,
    '#resizable' => FALSE,
    '#attributes' => array('class' => array('ckeditor')),
  );

  $form['submit_suffix'] = array('#markup' => '<div class="form-group">');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'submit',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#limit_validation_errors' => array(),
    '#submit' => array('hrm_backend_modal_cancel_submit'),
    '#attributes' => array('class' => array('btn-default', 'btn-sm')),
    '#suffix' => '</div>',
  );

  $form['#attributes']['class'][] = 'form-bordered';
  return $form;
}

function hrm_backend_leave_cancel_leave_form_submit($form, &$form_state) {
  $leave = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];
  $lid = $leave['lid'];
  hrm_basic_leave_entitlement_register_canceled_leave($leave);

  db_update('hrm_leave')
    ->fields(array(
      'status' => HRM_STATUS_CANCELED,
      'changed' => time(),
    ))
    ->condition('lid', $lid, '=')
    ->execute();

  db_update('hrm_leave_timing')
    ->fields(array(
      'status' => HRM_STATUS_CANCELED,
    ))
    ->condition('lid', $lid, '=')
    ->execute();

  db_insert('hrm_leave_action')
    ->fields(array(
      'lid' => $lid,
      'action' => HRM_ACTION_CANCEL,
      'comment' => $form_state['values']['justification'],
      'created_entity' => HRM_ENTITY_USER,
      'created_entity_id' => $account['uid'],
      'created' => time(),
    ))
    ->execute();

  $log_data = array(
    'title' => $lid,
    'justification' => $form_state['values']['justification'],
  );
  hrm_backend_entity_history_log_action(HRM_ENTITY_LEAVE, $lid, HRM_ACTION_CANCELED, $account['uid'], $log_data);

  drupal_set_message('Successfully saved***');
  $commands = array();
  $commands[] = ctools_ajax_command_redirect('leave/details/' . $lid, 0, array('query' => array('tab' => HRM_TAB_HISTORY)));
  print ajax_render($commands);
  exit;
}

/*
 * Leaves Calendare
 */

function hrm_backend_leave_calendar_page() {
  global $user;
  $account = hr_basic_account_get_info($user->uid);

  $calendar_content = hrm_backend_leave_calendar_get_content(array('account' => $account));
  return theme('page_wrapper_leave_calendar', array('account' => $account, 'calendar_content' => $calendar_content));
}

function hrm_backend_leave_calendar_form($form, &$form_state, $account) {
  $oid = $account['oid'];
  $filters = array(array('field' => 'oid', 'value' => $oid, 'operator' => '='));
  $emp_filters = $filters;
//  $emp_filters [] = array('field' => 'status', 'value' => HRM_FLAG_YES, 'operator' => '=');
  $employees = hrm_basic_employee_get_employees_names($emp_filters);
  $leave_types = hr_basic_config_leave_type_get_leave_types($filters);

  $leave_status = hr_basic_leave_status_get_options();

  $form['status'] = array(
    '#title' => t('Show Leave with Status'),
    '#type' => 'checkboxes',
    '#options' => $leave_status,
    '#attributes' => array('class' => array()),
    '#required' => FALSE,
  );

  $form['employee'] = array('#title' => t('Employee'), '#type' => 'select',
    '#options' => $employees,
    '#empty_option' => 'All',
    '#empty_value' => 'All',
    '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Employee'),
  );

  $form['type'] = array('#title' => t('Leave type'), '#type' => 'select',
    '#options' => $leave_types,
    '#empty_option' => 'All',
    '#empty_value' => 'All', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Leave Type'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('View'),
    "#ajax" => array(
      'callback' => 'hrm_backend_leave_calendar_form_callback',
      'event' => 'click',
      "effect" => "fade",
    ),
    '#attributes' => array('class' => array('btn-primary', 'btn-sm'))
  );

//$reset_link = 
  $form['#attributes']['class'][] = 'leave-list-form';

  return $form;
}

function hrm_backend_leave_calendar_form_callback($form, &$form_state) {
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

  $content = hrm_backend_leave_calendar_get_content($data);

  $commands[] = ajax_command_html('#leave-calendar-details', $content);

  $page = array('#type' => 'ajax', '#commands' => $commands);
  return $page;
}

function hrm_backend_leave_calendar_get_content($data) {
  global $base_url;

  $account = $data['account'];
  $oid = $account['oid'];

  $org_filterss = array(array('field' => 'oid', 'value' => $oid, 'operator' => '='));

  $employee_filters = array(
    array('field' => 'oid', 'value' => $oid, 'operator' => '='),
    array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '=')
  );
  $employees = hr_basic_employee_get_employees_names($employee_filters);
  $employees[0] = '';
  $empids = array_keys($employees);

  $leave_filters = array(
    array('field' => 'oid', 'value' => $oid, 'operator' => '='),
    array('field' => 'empid', 'value' => $empids, 'operator' => 'IN')
  );

  $leave_types = hr_basic_config_leave_type_get_leave_types($org_filterss);

  $leave_types_colors = hrm_basic_config_leave_type_get_leave_types_colors($org_filterss);

  $leave_status = hr_basic_leave_status_get_options_abbrevaition();

  if (isset($data['employee'])) {
    $leave_filters[] = array('field' => 'empid', 'value' => $data['employee'], 'operator' => '=');
  }

  if (isset($data['type'])) {
    $leave_filters[] = array('field' => 'leave_type', 'value' => $data['type'], 'operator' => '=');
  }
  if (isset($data['status']) && !empty($data['status'])) {
    $leave_filters[] = array('field' => 'status', 'value' => $data['status'], 'operator' => 'IN');
  }

  $leaves = hrm_backend_leave_get_data($leave_filters);

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

    $title = $employees[$leave['empid']] . '<span class ="leave-status">' . $leave_status[$leave['status']] . '</span>';
    $event = array(
      'title' => hr_basic_fix_js_string_format($title),
      'start' => $leave_start,
      'end' => $leave_end,
      'url' => $base_url . '/leave/details/' . $leave['leave_id'],
      'color' => $leave_types_colors[$leave['leave_type']],
      'class' => '',
    );

    $events[] = $event;
  }

  $content = theme('section_calendar', array('events' => $events));

  return $content;
}

/*
 * Leave Details
 */

// Mohamed
function hrm_backend_leave_details($lid) {
  if ($lid && is_numeric($lid)) {
    $leave = hrm_basic_leave_get_data(array(array('field' => 'lid', 'value' => $lid)), TRUE);
    if (!empty($leave)) {

      $account = hrm_basic_account_get_info();
      $actions = array();
      if (empty($account) || !isset($account['oid']) || !$account['oid']) {
        drupal_access_denied();
        exit();
      }

      $role_permission = hrm_basic_role_permissions($account['role']);
      $permission_data = array('leave' => $leave, 'permissions' => $role_permission);

      if (hrm_backend_leave_permissions(HRM_PERM_LEAVE_ACCESS_DETAILS, $account, $permission_data)) {
        $leave_type = hrm_basic_leave_type_get_data(array(array('field' => 'id', 'value' => $leave['leave_type'])), TRUE);
        $empid = $leave['empid'];

        $leave_period = variable_get(HRM_VAR_LEAVE_CURRENT_LEAVE_PERIOD, date(HRM_DATE_YEAR_YYYY));


        $entitlement_filters = array(
          array('field' => 'empid', 'value' => $empid, 'operator' => '='),
          array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
          array('field' => 'leave_type', 'value' => $leave_type['id'], 'operator' => '='),
        );

        $entitlement = hrm_basic_leave_entitlement_get_data($entitlement_filters);
        $tabs['details'] = array(
          'title' => 'Details',
          'content' => hrm_backend_leave_display_details($leave, $leave_type, $entitlement, $leave_period, $account)
        );

        $entity_filters = array(
          array('field' => 'entity', 'value' => HRM_ENTITY_LEAVE),
          array('field' => 'entity_id', 'value' => $lid),
        );

        $history_sort = array(array('field' => 'created', 'direction' => 'ASC'));
        $history_logs = hrm_backend_entity_history_get_history($entity_filters, $history_sort);
        $leave_history = theme('section_entity_history', array('history' => $history_logs, 'title' => 'Leave Log History'));
        $tabs['history'] = array(
          'title' => 'History',
          'content' => $leave_history
        );

        if ((hrm_backend_leave_permissions(HRM_PERM_LEAVE_CANCEL_LEAVE, $account, $permission_data)) && $leave['status'] != HRM_STATUS_USED) {
          $actions[] = l(HRM_FA_TRASH_RED . 'Cancel Leave', 'hrm-modal', array('query' => array('action' => HRM_MODAL_LEAVE_CANCEL_LEAVE, 'lid' => $lid), 'html' => TRUE));
        }

        if (hrm_backend_leave_permissions(HRM_PERM_LEAVE_DOCUMENT_ACCESS, $account, $permission_data)) {

          if (hrm_backend_leave_permissions(HRM_PERM_LEAVE_DOCUMENT_ADD, $account, $permission_data)) {
            $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Document', 'hrm-modal', array('query' => array('action' => HRM_MODAL_EN_DOCUMENT_ADD, 'entity' => HRM_ENTITY_LEAVE, 'entity_id' => $lid, 'oid' => $leave['oid']), 'html' => TRUE));
          }

          $edit_documents = hrm_backend_leave_permissions(HRM_PERM_LEAVE_DOCUMENT_EDIT, $account, $permission_data);
          $delete_documents = hrm_backend_leave_permissions(HRM_PERM_LEAVE_DOCUMENT_DELETE, $account, $permission_data);

          $documents_filters = array(
            array('field' => 'entity', 'value' => HRM_ENTITY_LEAVE),
            array('field' => 'entity_id', 'value' => $lid),
          );
          $documents_list = hrm_backend_entity_document_get_documents_list($documents_filters, $edit_documents, $delete_documents);
          $documets_fields = array();
          $documets_fields[] = array('type' => 'table', 'row_class' => array('noborder'),
            'value' => $documents_list, 'responsive' => TRUE);
          $documnts_content = theme('section_entity_details', array('fields' => $documets_fields));

          $tabs['documents'] = array(
            'title' => 'Documents',
            'content' => $documnts_content
          );
        }

        $variables = array(
          'tabs' => $tabs,
          'actions' => $actions
        );
        $org_date_format = HRM_DATE_SIMPLE;
        $org_time_format = HRM_DATE_TIME_12;
        $title = 'Leave Details: ' . hrm_basic_employee_get_name($empid) . ' - ';

        $start_date = hrm_basic_datetime_convert($leave['start_date'], $org_date_format, HRM_DATE_SYSTEM_DATE);
        $end_date = hrm_basic_datetime_convert($leave['end_date'], $org_date_format, HRM_DATE_SYSTEM_DATE);

        $leave_date = ' (<strong>' . $start_date . '</strong> - <strong>' . $end_date . '</strong>) ';
        if ($start_date == $end_date) {
          $leave_first_timing = reset($leave['timing']);

          $start_time = hrm_basic_datetime_convert($leave_first_timing['start_time'], $org_time_format, HRM_DATE_SYSTEM_TIME);
          $end_time = hrm_basic_datetime_convert($leave_first_timing['end_time'], $org_time_format, HRM_DATE_SYSTEM_TIME);
          $leave_date = $start_date . ' (<strong>' . $start_time . '</strong> - <strong>' . $end_time . '</strong>)';
        }
        $title .= $leave_date;

        drupal_set_title($title, PASS_THROUGH);

        return theme('page_wrapper_entity_tabs_js', $variables);
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
  else {
    drupal_not_found();
    exit();
  }
}

// Mohamed
function hrm_backend_leave_display_details($leave, $leave_type, $entitlement, $leave_period, $account) {

  $org_date_format = HRM_DATE_SIMPLE_MONTH_NAME_MMM;
  $org_time_format = HRM_DATE_TIME_12;

  $org_format = $org_date_format . ' ' . $org_time_format;
  $applied_on = date($org_format, $leave['created']);
  $statuses = hrm_basic_status_names();


  $fields = array();

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Leave type', 'value_class' => array(),
    'value' => $leave_type['name'], 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Status', 'value_class' => array(),
    'value' => $statuses[$leave['status']], 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Days', 'value_class' => array(),
    'value' => number_format($leave['days'], 3), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Hours', 'value_class' => array(),
    'value' => $leave['hours'], 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Applied on', 'value_class' => array(),
    'value' => $applied_on, 'value_class' => array());

//  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
//    'label' => 'Assigned By', 'value_class' => array(),
//    'value' => hrm_basic_account_get_name($leave['uid']), 'value_class' => array());
//  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
//    'label' => 'Approval Method', 'value_class' => array(),
//    'value' => $approval_method[$leave_type['approval_method']], 'value_class' => array());

  $applyed_to_name = '-';

  $applyed_to_name = hrm_basic_employee_get_name($leave['assigned_to']);


  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Assigned to', 'value_class' => array(),
    'value' => $applyed_to_name, 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Comments', 'value_class' => array(),
    'value' => check_markup($leave['comment'], 'filtered_html'), 'value_class' => array());


  if ($leave['status'] == HRM_STATUS_SCHEDULED || $leave['status'] == HRM_STATUS_USED || $leave['status'] == HRM_STATUS_PENDING_TO_SUPERVISOR || $leave['status'] == HRM_STATUS_PENDING_TO_HR || $leave['status'] == HRM_STATUS_CANCELED) {

    $entitlement_filters = array(
      array('field' => 'empid', 'value' => $leave['empid'], 'operator' => '='),
      array('field' => 'leave_period', 'value' => $leave_period, 'operator' => '='),
      array('field' => 'leave_type', 'value' => $leave['leave_type'], 'operator' => '='),
    );
    $entitlement = hrm_basic_leave_entitlement_get_data($entitlement_filters, TRUE);

    $fields[] = array('type' => 'block_header', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
      'label' => '', 'label_class' => array(),
      'value' => '<strong> Entitlements </strong>', 'value_class' => array());

//  print("<pre>");
//print_r($entitlement);
//exit();
    $entitlement_days_output = '';
    $entitlement_days_output = '
      <div class="col-sm-4 col-lg-3 style-alt">
        <div class="widget">  
          <div class="widget-extra themed-background text-center"><h3 class="widget-content-light">Transfered</h3></div>
          <div class="widget-extra-full text-center h3 strong">' . number_format($entitlement['transfered'], 4) . '</div>
        </div>
        </div>';
    $entitlement_days_output .= '
      <div class="col-sm-4 col-lg-3 style-alt">
        <div class="widget">  
          <div class="widget-extra themed-background-spring text-center"><h3 class="widget-content-light">Pending</h3></div>
          <div class="widget-extra-full text-center h3 strong">' . number_format($entitlement['pending'], 4) . '</div>
        </div>
        </div>';
    $entitlement_days_output .= '
      <div class="col-sm-4 col-lg-3 style-alt">
        <div class="widget">  
          <div class="widget-extra themed-background-dark-fire text-center"><h3 class="widget-content-light">Used</h3></div>
          <div class="widget-extra-full text-center h3 strong">' . number_format($entitlement['used'], 4) . '</div>
        </div>
        </div>';
    $entitlement_days_output .= '
      <div class="col-sm-4 col-lg-3 style-alt">
        <div class="widget">  
          <div class="widget-extra themed-background-autumn text-center"><h3 class="widget-content-light">Balance</h3></div>
          <div class="widget-extra-full text-center h3 strong">' . number_format($entitlement['days'], 4) . '</div>
        </div>
        </div>';
    $fields[] = array('type' => 'raw', 'wrapper_class' => array('col-sm-12', 'padding-0'), 'row_class' => array('clearfix', 'noborder'),
      'value' => $entitlement_days_output, 'value_class' => array());
    $fields[] = array('type' => 'block_footer');
  }

  $leave_start = 0;
  $leave_end = 0;
  $rows = array();
  foreach ($leave['timing'] as $timing) {
    if ($leave_start == 0) {
      $leave_start = $timing['start_time'];
    }
    if ($leave_end == 0) {
      $leave_end = $timing['end_time'];
    }

    if ($timing['start_time'] < $leave_start) {
      $leave_start = $timing['start_time'];
    }
    if ($timing['end_time'] > $leave_end) {
      $leave_end = $timing['end_time'];
    }

    $duration_type = array(
      HRM_LEAVE_DURATION_FULL_DAY => 'FULL DAY',
      HRM_LEAVE_DURATION_HALF_DAY => 'HALF DAY',
      HRM_LEAVE_DURATION_TIME => 'SPECIFIC TIME',
    );
    $timing_date = hrm_basic_datetime_convert($timing['day_date'], $org_date_format, HRM_DATE_SYSTEM_DATE);

    $rows[] = array(
      'data' => array(
        array('data' => $timing_date, 'class' => array()),
        array('data' => date($org_time_format, strtotime($timing['start_time']))),
        array('data' => date($org_time_format, strtotime($timing['end_time']))),
        array('data' => hrm_basic_time_timestamp_to_time(($timing['hours'] * 60 * 60))),
        array('data' => number_format($timing['days'], 3)),
        array('data' => isset($timing['duration_type']) ? $duration_type[$timing['duration_type']] : '')
      ),
    );
  }

  $header = array(
    array('data' => 'Day Date'),
    array('data' => 'Start Time'),
    array('data' => 'End Time'),
    array('data' => 'Duration (Hours)'),
    array('data' => 'Duration (Days)'),
    array('data' => 'Duration (Type)'),
  );



  $list = array(
    'attributes' => array('class' => array('table-hover', 'table-vcenter')),
    'rows' => $rows,
    'header' => $header
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

  $fields[] = array('type' => 'block_header', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => '', 'label_class' => array(),
    'value' => 'Leave <strong> Timing </strong>', 'value_class' => array());

  $fields[] = array('type' => 'table', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'value' => $list, 'value_class' => array());

  $fields[] = array('type' => 'block_footer');


  return theme('section_entity_details', array('fields' => $fields));
}

//Delete Leave
function hrm_backend_leave_delete($lid) {
  global $user;

  if ($lid && is_numeric($lid)) {
    $leave = hrm_backend_leave_get_data(array(array('field' => 'leave_id', 'value' => $lid, 'operator' => '=')), array(), TRUE);
    if (!empty($leave)) {
      $account = hr_basic_account_get_info($user->uid);

      if ($leave['oid'] == $account['oid']) {
        hr_basic_leave_entitlement_register_deleted_leave($leave['empid'], $leave['oid'], $lid);

        db_delete('hr_employee_leave')
          ->condition('leave_id', $leave['leave_id'], '=')
          ->execute();
        db_delete('hr_employee_leave_timing')
          ->condition('leave_id', $leave['leave_id'], '=')
          ->execute();
        db_delete('hr_employee_leave_actions')
          ->condition('leave_id', $leave['leave_id'], '=')
          ->execute();

        drupal_set_message('Leave deleted successfully');
        drupal_goto('leave/leave-list');
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
  else {
    drupal_not_found();
    exit();
  }
}

function hrm_backend_leave_get_data($filters = array(), $sort = array(), $first_item = FALSE) {
  $query = db_select('hr_employee_leave', 'employee_leave')
    ->fields('employee_leave', array('leave_id', 'empid', 'oid', 'leave_type', 'assigned_to', 'source',
    'from_date', 'to_date', 'hours', 'days', 'comment', 'status', 'uid', 'created', 'changed'));

  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $query->condition($filter['field'], $filter['value'], $filter['operator']);
    }
  }
  if (!empty($sort)) {
    foreach ($sort as $sort_field) {
      $query->orderBy($sort_field['field'], $sort_field['direction']);
    }
  }

  $leaves = $query->execute()->fetchAllAssoc('leave_id', PDO::FETCH_ASSOC);

  foreach ($leaves as $lid => $leave) {
    $leaves[$lid]['timing'] = db_select('hr_employee_leave_timing', 'employee_leave_timing')
        ->fields('employee_leave_timing', array('id', 'leave_id', 'from_time', 'to_time', 'hours', 'days'))
        ->condition('leave_id', $lid, '=')
        ->execute()->fetchAll(PDO::FETCH_ASSOC);

    $leaves[$lid]['hours_formatted'] = hr_basic_time_format_time(($leave['hours'] * 60 * 60));

    $leaves[$lid]['start'] = $leave['start_date'];
    $leaves[$lid]['end'] = $leave['end_date'];

    $leaves[$lid]['actions'] = db_select('hr_employee_leave_actions', 'employee_leave_actions')
        ->fields('employee_leave_actions', array('id', 'leave_id', 'action', 'content', 'created_by', 'created'))
        ->condition('leave_id', $lid, '=')
        ->orderBy('created')
        ->execute()->fetchAll(PDO::FETCH_ASSOC);
  }

  if ($first_item) {
    $leave = reset($leaves);

    return $leave;
  }

  return $leaves;
}

// ==================================================================================
// ---------------------------------- Apply Leave -----------------------------------
// ==================================================================================

function hrm_backend_my_leaves_page() {
  die('shoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo');
}
