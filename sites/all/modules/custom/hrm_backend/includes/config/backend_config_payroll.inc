<?php

function hrm_backend_config_payroll_permission($action, $account = array(), $data = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }

  switch ($action) {
    // Grade
    case HRM_PERM_CONFIG_PAYROLL_GRADE_ACCESS:
      if (user_access(HRM_PERM_CONFIG_PAYROLL_GRADE_ACCESS) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_PAYROLL_GRADE_ADD:
      if (user_access(HRM_PERM_CONFIG_PAYROLL_GRADE_ADD) && $account['oid'] && $account['bid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_PAYROLL_GRADE_EDIT:
      if (user_access(HRM_PERM_CONFIG_PAYROLL_GRADE_EDIT) && $account['oid'] &&
        $data['grade']['oid'] == $account['oid'] &&
        $data['grade']['bid'] == $account['bid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_PAYROLL_GRADE_DELETE:
      if (user_access(HRM_PERM_CONFIG_PAYROLL_GRADE_DELETE) && $account['oid'] &&
        $data['grade']['oid'] == $account['oid'] &&
        $data['grade']['bid'] == $account['bid']) {
        return TRUE;
      }
      break;
    // Salary Component
    case HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_ACCESS:
      if (user_access(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_ACCESS) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_ADD:
      if (user_access(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_ADD) && $account['oid'] && $account['bid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_EDIT:
      if (user_access(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_EDIT) && $account['oid'] &&
        $data['salary_component']['oid'] == $account['oid'] &&
        $data['salary_component']['bid'] == $account['bid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_DELETE:
      if (user_access(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_DELETE) && $account['oid'] &&
        $data['salary_component']['oid'] == $account['oid'] &&
        $data['salary_component']['bid'] == $account['bid']) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}

/*
 * Pay Grades
 */

function hrm_backend_config_payroll_grades() {
  $account = hrm_basic_account_get_info();

  if (hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_GRADE_ACCESS, $account)) {
    $filters = array(array('field' => 'oid', 'value' => $account['oid']));
    $currencies = hrm_basic_lov_get_terms(HRM_LOV_CURRENCY);

    if ($account['bid']) {
      $filters[] = array('field' => 'bid', 'value' => $account['bid']);
    }
    $grades = hrm_backend_config_payroll_grade_get_data($filters);
    $role_permissions = hrm_basic_role_permissions($account['role']);

    $rows = array();
    foreach ($grades as $id => $grade) {
      $grade_currency = array();
      $permission_data = array('grade' => $grade, 'permissions' => $role_permissions);
      if (isset($grade['currency']) && !empty($grade['currency'])) {
        foreach ($grade['currency'] as $row) {
          $grade_currency[] = $currencies[$row['currency']] . ' <strong>' . $row['min_salary'] . ' - ' . $row['max_salary'] . '</strong>';
        }
      }
      $row_classes = array();
      $edit_payroll_permission = hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_GRADE_EDIT, $account, $permission_data);
      $row_classes[] = ($edit_payroll_permission ? 'table-tr-link' : '');
      $tr_data_url = ($edit_payroll_permission ? url('config/payroll/grade/edit/' . $id) : '');
      $rows[] = array(
        'data' => array(
          array('data' => $grade['name']),
          array('data' => implode('<br />', $grade_currency)),
          array('data' => hrm_basic_branch_get_name($grade['bid'])),
        ),
        'class' => $row_classes,
        'data-url' => $tr_data_url,
      );
    }

    $header = array(
      array('data' => 'Name'),
      array('data' => 'Currency'),
      array('data' => 'Branch'),
    );

    $list = array(
      'attributes' => array('class' => array('table-hover', 'table-vcenter')),
      'rows' => $rows,
      'header' => $header
    );

    $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

    $actions = array();
    if (hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_GRADE_ADD, $account)) {
      $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Pay Grade', 'config/payroll/grade/add/0', array('html' => TRUE));
    }

    $variables = array(
      'title' => 'Pay <strong>Grades</strong>',
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

function hrm_backend_config_payroll_grade_get_data($filters = array(), $first_item = FALSE) {
  $query = db_select('hrm_config_payroll_grade', 'payroll_grade')
    ->fields('payroll_grade', array('id', 'oid', 'bid', 'name', 'uid', 'created', 'changed'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }

  $grades = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  $grade_ids = array_keys($grades);
  if (!empty($grade_ids)) {
    $grades_currency = db_select('hrm_config_payroll_grade_currency', 'payroll_grade_currency')
        ->fields('payroll_grade_currency', array('id', 'grade', 'currency', 'min_salary', 'max_salary'))
        ->condition('grade', $grade_ids, 'IN')
        ->execute()->fetchAll(PDO::FETCH_ASSOC);
    foreach ($grades_currency as $grade_caurrency) {
      if (!isset($grades[$grade_caurrency['grade']]['currency'])) {
        $grades[$grade_caurrency['grade']]['currency'] = array();
      }
      $grades[$grade_caurrency['grade']]['currency'][] = $grade_caurrency;
    }

    foreach ($grades as $key => $grade) {
      if (!isset($grade['currency'])) {
        $grades[$key]['currency'] = array();
      }
    }
  }

  if ($first_item) {
    return reset($grades);
  }

  return $grades;
}

function hrm_backend_config_payroll_grade_form_page($id) {
  $account = hrm_basic_account_get_info();

  $grade = array();
  if ($id && is_numeric($id)) {
    $grade = hrm_backend_config_payroll_grade_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($grade)) {
      drupal_not_found();
      exit();
    }

    $edit_grade_permission = hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_GRADE_EDIT, $account, array('grade' => $grade));
    if (!$edit_grade_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Edit ' . $grade['name'] . ' - ' . hrm_basic_branch_get_name($grade['bid']));
  }
  else {
    $add_grade_permission = hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_GRADE_ADD, $account);
    if (!$add_grade_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add Grade - ' . hrm_basic_branch_get_name($account['bid']));
  }

  $grade_form = drupal_get_form('hrm_backend_config_payroll_grade_form', $grade, $account);

  $variables = array(
    'entity' => HRM_ENTITY_GRADE,
    'title' => 'Pay grade information',
    'form' => $grade_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_payroll_grade_form($form, &$form_state, $grade, $account) {
  if (!isset($form_state['storage']['currency'])) {
    $currency_data = array(array(array(), array(), array(), array(), array()));
    if (isset($grade['currency']) && !empty($grade['currency'])) {
      $currency_data = array();
      foreach ($grade['currency'] as $row) {
        $currency_data[] = $row;
      }
    }

    $form_state['storage']['currency'] = $currency_data;
    $form_state['storage']['lov']['currency'] = hrm_basic_lov_get_terms(HRM_LOV_CURRENCY);
  }

  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($grade['name']) ? $grade['name'] : ''),
  );

  $currency_header = array(
    'index' => array('data' => '#', 'class' => array('table-field-header', 'table-field-header-grade-index')),
    'currency' => array('data' => 'Currency', 'class' => array('table-field-header', 'table-field-header-grade-currency')),
    'min_salary' => array('data' => 'Minimum', 'class' => array('table-field-header', 'table-field-header-grade-min-salary')),
    'max_salary' => array('data' => 'Minimum', 'class' => array('table-field-header', 'table-field-header-grade-max-salary')),
    'actions' => array('data' => 'Actions', 'class' => array('table-field-header', 'table-field-agenda-items-actions')),
  );

  $form['currency'] = array(
    '#theme' => 'hrm_table_field',
    '#header' => $currency_header,
    '#title' => 'Pay Grade Currency',
    '#attributes' => array('class' => array('table-form')),
    '#prefix' => '<div id="grade-currency-wrapper" class="form-item form-group grade-currency-wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    'rows' => array(
      '#tree' => TRUE,
    ),
  );

  $currency_data = $form_state['storage']['currency'];
  $currency_index = 1;
  foreach ($currency_data as $currency_key => $currency) {
    $currency_required = FALSE;
    if ($currency_index == 1) {
      $currency_required = TRUE;
    }
    $row = array();
    $row['index'] = array(
      '#markup' => $currency_index . '.',
    );
    $row['currency'] = array(
      '#type' => 'select',
      '#title' => 'Currency',
      '#title_display' => 'invisible',
      '#options' => $form_state['storage']['lov']['currency'],
      '#required' => $currency_required,
      '#empty_option' => '',
      '#default_value' => (isset($currency['currency']) ? $currency['currency'] : ''),
      '#attributes' => array('class' => array('select-chosen'), 'placeholder' => 'currency'),
    );
    $row['min_salary'] = array(
      '#type' => 'textfield',
      '#title' => 'Minimum',
      '#title_display' => 'invisible',
      '#required' => $currency_required,
      '#default_value' => (isset($currency['min_salary']) ? $currency['min_salary'] : ''),
      '#attributes' => array('class' => array('currency-min'), 'placeholder' => 'minimum'),
      '#element_validate' => array('element_validate_number'),
      '#class' => array('max-width-100-px')
    );
    $row['max_salary'] = array(
      '#type' => 'textfield',
      '#title' => 'Maximum',
      '#title_display' => 'invisible',
      '#required' => $currency_required,
      '#default_value' => (isset($currency['max_salary']) ? $currency['max_salary'] : ''),
      '#attributes' => array('class' => array('currency-max'), 'placeholder' => 'maximum'),
      '#element_validate' => array('element_validate_number'),
      '#class' => array('max-width-100-px')
    );
    $row['remove'] = array('#markup' => '');
    if (count($form_state['storage']['currency']) > 1) {
      $row['remove'] = array(
        '#type' => 'submit',
        '#value' => 'DELETE',
        '#limit_validation_errors' => array(),
        '#submit' => array('hrm_backend_config_payroll_grade_form_currency_remove'),
        '#ajax' => array(
          'callback' => 'hrm_backend_config_payroll_grade_form_currency_callback',
          'wrapper' => 'grade-currency-wrapper',
        ),
        '#name' => 'currency_remove_' . $currency_key,
        '#attributes' => array('class' => array('btn-danger', 'btn-xs', 'remove-btn', 'btn-2-clicks', 'strong'), 'title' => 'Click twice to remove'),
        '#prefix' => '<span class="remove-currency-wrapper cursor-pointer btn-2-clicks-wrapper">',
        '#suffix' => '</span>',
        '#class' => array('text-center')
      );
    }
    $form['currency']['rows'][$currency_key] = $row;
    $currency_index += 1;
  }

  $form['currency']['add_more'] = array(
    '#type' => 'submit',
    '#value' => '+ ADD MORE',
    '#limit_validation_errors' => array(),
    '#submit' => array('hrm_backend_config_payroll_grade_form_currency_add'),
    '#ajax' => array(
      'callback' => 'hrm_backend_config_payroll_grade_form_currency_callback',
      'wrapper' => 'grade-currency-wrapper',
    ),
    '#attributes' => array('class' => array('btn-xs', 'btn-link', 'strong')),
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Pay Grade',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($grade['id']) && $grade['id']) {
    $delete_grade_permission = hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_GRADE_DELETE, $account, array('grade' => $grade));
    if ($delete_grade_permission) {
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete Pay Grade',
        '#submit' => array('hrm_backend_config_payroll_grade_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );

      unset($form['submit']['#suffix']);
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'config-grade-form';

  return $form;
}

function hrm_backend_config_payroll_grade_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_config_payroll_grade_form_submit(&$form, &$form_state) {
  $grade = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  if (isset($grade['id']) && $grade['id']) {
    db_update('hrm_config_payroll_grade')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'changed' => time(),
      ))
      ->condition('id', $grade['id'], '=')
      ->execute();
  }
  else {
    $grade['id'] = db_insert('hrm_config_payroll_grade')
      ->fields(array(
        'oid' => $account['oid'],
        'bid' => $account['bid'],
        'name' => $form_state['values']['name'],
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  }

  db_delete('hrm_config_payroll_grade_currency')
    ->condition('grade', $grade['id'], '=')
    ->execute();

  $grade_currencies = $form_state['values']['currency']['rows'];
  foreach ($grade_currencies as $currency) {
    db_insert('hrm_config_payroll_grade_currency')
      ->fields(array(
        'grade' => $grade['id'],
        'currency' => $currency['currency'],
        'min_salary' => $currency['min_salary'],
        'max_salary' => $currency['max_salary'],
      ))
      ->execute();
  }

  drupal_set_message('Pay grade saved successfully***');
  drupal_goto('config/payroll/grade');
}

function hrm_backend_config_payroll_grade_form_delete(&$form, &$form_state) {
  $grade = $form_state['build_info']['args'][0];

  db_delete('hrm_config_payroll_grade')
    ->condition('id', $grade['id'], '=')
    ->execute();

  db_delete('hrm_config_payroll_grade_currency')
    ->condition('grade', $grade['id'], '=')
    ->execute();

  drupal_set_message('Pay grade deleted successfully');
  drupal_goto('config/payroll/grade');
}

function hrm_backend_config_payroll_grade_form_currency_add($form, &$form_state) {
  $new_currency_index = (max(array_keys($form_state['storage']['currency']))) + 1;
  $form_state['storage']['currency'][$new_currency_index] = array();
  $form_state['rebuild'] = TRUE;
}

function hrm_backend_config_payroll_grade_form_currency_remove($form, &$form_state) {
  $trigger_element = explode('_', $form_state['triggering_element']['#name']);
  watchdog('remove', $trigger_element);

  $currency_index = $trigger_element[2];

  watchdog('currency before', serialize($form_state['storage']['currency']));
  unset($form_state['storage']['currency'][$currency_index]);
  watchdog('currency after', serialize($form_state['storage']['currency']));

  $form_state['rebuild'] = TRUE;
}

function hrm_backend_config_payroll_grade_form_currency_callback($form, $form_state) {
  return $form['currency'];
}

/*
 * Salary Components
 */

function hrm_backend_config_payroll_salary_component() {
  $account = hrm_basic_account_get_info();

  if (hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_ACCESS, $account)) {
    $filters = array(array('field' => 'oid', 'value' => $account['oid']));
    if ($account['bid']) {
      $filters[] = array('field' => 'bid', 'value' => $account['bid']);
    }
    $salary_components = hrm_backend_config_payroll_salary_component_get_data($filters);
    $rows = array();

    $branch_names = hrm_basic_branch_names($filters);
    $role_permissions = hrm_basic_role_permissions($account['role']);

    foreach ($salary_components as $id => $salary_component) {
      $type = HRM_SALARY_COMPONENT_EARNING_NAME;
      if ($salary_component['type'] == HRM_SALARY_COMPONENT_DEDUCTION) {
        $type = HRM_SALARY_COMPONENT_DEDUCTION_NAME;
      }
      $permission_data = array('salary_component' => $salary_component, 'permissions' => $role_permissions);
      $edit_salary_component_permission = hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_EDIT, $account, $permission_data);
      $row_classes = array();
      $row_classes[] = ($edit_salary_component_permission ? 'table-tr-link' : '');
      $tr_data_url = ($edit_salary_component_permission ? url('config/payroll/salary-component/edit/' . $id) : '');
      $rows[] = array(
        'data' => array(
          array('data' => $salary_component['name']),
          array('data' => $type),
          array('data' => ($salary_component['payable'] ? HRM_FLAG_TEXT_YES : HRM_FLAG_TEXT_NO), 'class' => 'strong'),
          array('data' => ($salary_component['cost'] ? HRM_FLAG_TEXT_YES : HRM_FLAG_TEXT_NO), 'class' => 'strong'),
          array('data' => ($salary_component['value_amount'] ? HRM_FLAG_TEXT_YES : HRM_FLAG_TEXT_NO), 'class' => 'strong'),
          array('data' => ($salary_component['value_percentage'] ? HRM_FLAG_TEXT_YES : HRM_FLAG_TEXT_NO), 'class' => 'strong'),
          array('data' => $branch_names[$salary_component['bid']]),
        ),
        'class' => $row_classes,
        'data-url' => $tr_data_url,
      );
    }
    $header = array(
      array('data' => 'Name'),
      array('data' => 'Type'),
      array('data' => 'Payable'),
      array('data' => 'Cost'),
      array('data' => 'Value Ammount'),
      array('data' => 'Value Percentage'),
      array('data' => 'Branch'),
    );

    $list = array(
      'attributes' => array('class' => array('table-hover', 'table-vcenter')),
      'rows' => $rows,
      'header' => $header
    );

    $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

    $actions = array();
    if (hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_ADD, $account)) {
      $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Salary Component', 'config/payroll/salary-component/add/0', array('html' => TRUE));
    }


    $variables = array(
      'title' => 'Salary <strong>Component</strong>',
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

function hrm_backend_config_payroll_salary_component_get_data($filters = array(), $first_item = FALSE) {
  $query = db_select('hrm_config_payroll_salary_component', 'salary_component')
    ->fields('salary_component', array('id', 'oid', 'bid', 'name', 'type', 'payable', 'cost', 'value_amount', 'value_percentage', 'uid', 'created', 'changed'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $query->condition($filter['field'], $filter['value'], isset($filter['operator']) ? $filter['operator'] : '=');
    }
  }

  $salary_components = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  if ($first_item) {
    return reset($salary_components);
  }

  return $salary_components;
}

function hrm_backend_config_payroll_salary_component_form_page($id) {
  $account = hrm_basic_account_get_info();

  $salary_component = array();
  if ($id && is_numeric($id)) {
    $salary_component = hrm_backend_config_payroll_salary_component_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($salary_component)) {
      drupal_not_found();
      exit();
    }

    $edit_salary_component_permission = hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_EDIT, $account, array('salary_component' => $salary_component));
    if (!$edit_salary_component_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Edit ' . $salary_component['name'] . ' - ' . hrm_basic_branch_get_name($salary_component['bid']));
  }
  else {
    $add_salary_component_permission = hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_ADD, $account);
    if (!$add_salary_component_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add Salary Component - ' . hrm_basic_branch_get_name($account['bid']));
  }

  $salary_component_form = drupal_get_form('hrm_backend_config_payroll_salary_component_form', $account, $salary_component);

  $variables = array(
    'entity' => HRM_ENTITY_SALARY_COMPONENT,
    'title' => 'Salary component information',
    'form' => $salary_component_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_payroll_salary_component_form($form, &$form_state, $account, $salary_component) {
  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => isset($salary_component['name']) ? $salary_component['name'] : '',
  );

  $type_options = array(
    HRM_SALARY_COMPONENT_EARNING => HRM_SALARY_COMPONENT_EARNING_NAME,
    HRM_SALARY_COMPONENT_DEDUCTION => HRM_SALARY_COMPONENT_DEDUCTION_NAME
  );

  $add_to_options = array(
    'payable' => 'Total Payable',
    'cost' => 'Cost to Company'
  );

  $value_type_options = array(
    'amount' => 'Amount',
    'percentage' => 'Percentage'
  );


  $form['type'] = array(
    '#type' => 'radios',
    '#title' => 'Type',
    '#options' => $type_options,
    '#required' => TRUE,
    '#default_value' => isset($salary_component['type']) ? $salary_component['type'] : '',
  );

  $add_to_default_values = array();
  if (isset($salary_component['payable']) && $salary_component['payable']) {
    $add_to_default_values[] = 'payable';
  }
  if (isset($salary_component['cost']) && $salary_component['cost']) {
    $add_to_default_values[] = 'cost';
  }
  $form['add_to'] = array(
    '#type' => 'checkboxes',
    '#options' => $add_to_options,
    '#title' => 'Add to',
    '#default_value' => $add_to_default_values,
  );

  $value_type_default_values = '';
  if (isset($salary_component['value_amount']) && $salary_component['value_amount']) {
    $value_type_default_values = 'amount';
  }
  if (isset($salary_component['value_percentage']) && $salary_component['value_percentage']) {
    $value_type_default_values = 'percentage';
  }
  $form['value_type'] = array(
    '#type' => 'radios',
    '#title' => 'Value Type',
    '#options' => $value_type_options,
    '#required' => TRUE,
    '#default_value' => $value_type_default_values,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Salary Component',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($salary_component['id']) && $salary_component['id']) {
    $delete_salary_component_permission = hrm_backend_config_payroll_permission(HRM_PERM_CONFIG_PAYROLL_SALARY_COMPONENT_DELETE, $account, array('salary_component' => $salary_component));
    if ($delete_salary_component_permission) {
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete Salary Component',
        '#submit' => array('hrm_backend_config_payroll_salary_component_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );

      unset($form['submit']['#suffix']);
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'config-grade-form';

  return $form;
}

function hrm_backend_config_payroll_salary_component_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_config_payroll_salary_component_form_submit(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $salary_component = $form_state['build_info']['args'][1]; //  $account['oid'];

  $name = $form_state['values']['name'];
  $type = $form_state['values']['type'];
  $payable = (($form_state['values']['add_to']['payable']) ? 1 : 0);
  $cost = (($form_state['values']['add_to']['cost']) ? 1 : 0);
  $value_type = $form_state['values']['value_type'];
  $value_amount = (($value_type == 'amount') ? 1 : 0);
  $value_percentage = (($value_type == 'percentage') ? 1 : 0);

  if (isset($salary_component['id']) && $salary_component['id']) {
    db_update('hrm_config_payroll_salary_component')
      ->fields(array(
        'name' => $name,
        'type' => $type,
        'payable' => $payable,
        'cost' => $cost,
        'value_amount' => $value_amount,
        'value_percentage' => $value_percentage,
        'changed' => time(),
      ))
      ->condition('id', $salary_component['id'], '=')
      ->execute();
  }
  else {
    db_insert('hrm_config_payroll_salary_component')
      ->fields(array(
        'oid' => $account['oid'],
        'bid' => $account['bid'],
        'name' => $name,
        'type' => $type,
        'payable' => $payable,
        'cost' => $cost,
        'value_amount' => $value_amount,
        'value_percentage' => $value_percentage,
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  }

  drupal_set_message($form_state['values']['name'] . ' saved successfully***');
  drupal_goto('config/payroll/salary-component');
}

function hrm_backend_config_payroll_salary_component_form_delete(&$form, &$form_state) {
  $salary_component = $form_state['build_info']['args'][1];

  db_delete('hrm_config_payroll_salary_component')
    ->condition('id', $salary_component['id'], '=')
    ->execute();

  drupal_set_message($salary_component['name'] . ' deleted successfully');
  drupal_goto('config/payroll/salary-component');
}
