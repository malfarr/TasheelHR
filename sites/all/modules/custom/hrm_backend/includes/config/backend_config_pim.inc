<?php

/*
 * PIM
 */

function hrm_backend_config_pim_permissions($action, $account = array(), $data = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }

  $permissions = (isset($data['permissions']) && !empty($data['permissions']) ? $data['permissions'] : hrm_basic_role_permissions($account['role']));

  switch ($action) {

    /* identity document */
    case HRM_PERM_CONFIG_IDENTITY_DOCUMENT_ADD:
      if (isset($permissions[HRM_PERM_CONFIG_IDENTITY_DOCUMENT_ADD]) && $account['bid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_IDENTITY_DOCUMENT_EDIT:
      if (isset($permissions[HRM_PERM_CONFIG_IDENTITY_DOCUMENT_EDIT]) &&
        $data['document']['oid'] == $account['oid'] &&
        $data['document']['bid'] == $account['bid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_IDENTITY_DOCUMENT_DELETE:
      if (isset($permissions[HRM_PERM_CONFIG_IDENTITY_DOCUMENT_DELETE]) &&
        $data['document']['oid'] == $account['oid'] &&
        $data['document']['bid'] == $account['bid']) {
        return TRUE;
      }
      break;

      return FALSE;

    /* qualifications */
    case HRM_PERM_CONFIG_QUALIFICATIONS_ADD:
      if (isset($permissions[HRM_PERM_CONFIG_IDENTITY_DOCUMENT_ADD]) && $account['bid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_QUALIFICATIONS_EDIT:
      if (isset($permissions[HRM_PERM_CONFIG_QUALIFICATIONS_EDIT]) &&
        $data['qualification']['oid'] == $account['oid'] &&
        $data['qualification']['bid'] == $account['bid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_QUALIFICATIONS_DELETE:
      if (isset($permissions[HRM_PERM_CONFIG_QUALIFICATIONS_DELETE]) &&
        $data['qualification']['oid'] == $account['oid'] &&
        $data['qualification']['bid'] == $account['bid']) {
        return TRUE;
      }
      break;
      return FALSE;
  }
}

/*
 * Idenntity document
 */

function hrm_backend_config_identity_document() {
  $account = hrm_basic_account_get_info();
  $filters = array(array('field' => 'oid', 'value' => $account['oid']));
  if ($account['bid']) {
    $filters[] = array('field' => 'bid', 'value' => $account['bid']);
  }
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $documents = hrm_backend_config_identity_document_get_data($filters);
  $branch_names = hrm_basic_branch_names($filters);

  $role_permissions = hrm_basic_role_permissions($account['role']);
  $rows = array();
  foreach ($documents as $id => $document) {
    $permission_data = array('document' => $document, 'permissions' => $role_permissions);
    $access_edit_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_IDENTITY_DOCUMENT_EDIT, $account, $permission_data);

    $row_classes = array();
    $row_classes[] = ($access_edit_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_edit_permission ? url('config/pim/identity-document/edit/' . $id) : '');
    $rows[$id] = array(
      'data' => array(
        array('data' => ''),
        array('data' => $document['name']),
        array('data' => $branch_names[$document['bid']])),
      'class' => $row_classes, 'data-url' => $tr_data_url,
    );
  };

  $header = array(
    array('data' => '#', 'class' => array('nosort', 'nosearch', 'table-row-index-th')),
    array('data' => 'Identity Document'),
    array('data' => 'Branch')
  );

  $list = array(
    'attributes' => array('class' => array('table-vcenter', 'table-hover')),
    'rows' => $rows,
    'header' => $header
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all-index' : 'empty-datatable');

  $actions = array();
  if (hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_IDENTITY_DOCUMENT_ADD, $account)) {
    $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Identity document', 'config/pim/identity-document/add/0', array('html' => TRUE));
  }

  $variables = array(
    'title' => '<strong>Identity</strong> documents',
    'actions' => $actions,
    'list' => $list
  );

  return theme('page_wrapper_entity_list', $variables);
}

function hrm_backend_config_Identity_document_form_page($id) {
  $account = hrm_basic_account_get_info();

  $documents = array();
  if ($id) {
    $documents = hrm_backend_config_identity_document_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($documents)) {
      drupal_not_found();
      exit();
    }
    $edit_document_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_IDENTITY_DOCUMENT_EDIT, $account, array('document' => $documents));
    if (!$edit_document_permission) {
      drupal_access_denied();
      exit();
    }
    drupal_set_title('Edit ' . $documents['name'] . ' - ' . hrm_basic_branch_get_name($documents['bid']));
  }
  else {
    if (!hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_IDENTITY_DOCUMENT_ADD)) {
      drupal_access_denied();
      exit();
    }
    drupal_set_title('Add Identity document - ' . hrm_basic_branch_get_name($account['bid']));
  }

  $document_form = drupal_get_form('hrm_backend_config_identity_document_form', $documents, $account);

  $variables = array(
    'entity' => HRM_ENTITY_IDENTITY_DOCUMENT,
    'title' => 'Identity document information',
    'form' => $document_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_identity_document_form($form, &$form_state, $documents, $account) {

  $form['name'] = array(
    '#title' => 'Identity Document',
    '#type' => 'textfield',
    '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($documents['name']) ? $documents['name'] : '')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ' Save',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($documents['id']) && $documents['id']) {
    $delete_document_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_IDENTITY_DOCUMENT_DELETE, $account, array('document' => $documents));
    if ($delete_document_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#submit' => array('hrm_backend_config_identity_document_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'identity-document-add-form';
  $form['#attributes']['class'][] = 'js-validation-bootstrap';
  $form['#attributes']['class'][] = 'push';

  return $form;
}

function hrm_backend_config_identity_document_form_submit(&$form, &$form_state) {
  $documents = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  if (isset($documents['id']) && $documents['id']) {
    db_update('hrm_config_pim_document_type')
      ->fields(array(
        'oid' => $account['oid'],
        'name' => $form_state['values']['name'],
        'changed' => time(),
      ))
      ->condition('id', $documents['id'], '=')
      ->execute();
  }
  else {
    $documents['id'] = db_insert('hrm_config_pim_document_type')
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

  drupal_set_message($form_state['values']['name'] . ' Identity document saved successfully***');
  drupal_goto('config/pim/identity-document');
}

function hrm_backend_config_identity_document_form_delete(&$form, &$form_state) {
  $documents = $form_state['build_info']['args'][0];

  db_delete('hrm_config_pim_document_type')->condition('id', $documents['id'], '=')->execute();
  db_update('hrm_emp_document')
    ->fields(array('type' => 0))
    ->condition('type', $documents['id'], '=')
    ->execute();

  drupal_set_message($documents['name'] . ' identity document deleted successfully***');
  drupal_goto('config/pim/identity-document');
}

function hrm_backend_config_identity_document_get_data($filters = array(), $first_item = FALSE) {
  $query = db_select('hrm_config_pim_document_type', 'identity_document')
    ->fields('identity_document', array('id', 'oid', 'bid', 'name', 'uid', 'created', 'changed'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }
  $identity_documents = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  if ($first_item) {
    return reset($identity_documents);
  }

  return $identity_documents;
}

/*
 * Qualifications
 */

function hrm_backend_config_qualification_get_data($filters = array(), $first_item = FALSE) {
  $query = db_select('hrm_config_pim_qualification', 'qualification')
    ->fields('qualification', array('id', 'oid', 'bid', 'type', 'name', 'description', 'uid', 'created', 'changed'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }


  $qualifications = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  if ($first_item) {
    return reset($qualifications);
  }

  return $qualifications;
}

/*
 * Skills 
 */

function hrm_backend_config_skills() {
  $account = hrm_basic_account_get_info();
  $qualification_filters = array(array('field' => 'oid', 'value' => $account['oid'], 'field' => 'type', 'value' => HRM_QUALIFICATION_SKILLS));
  $filters = array(array('field' => 'oid', 'value' => $account['oid']));

  if ($account['bid']) {
    $filters[] = array('field' => 'bid', 'value' => $account['bid']);
    $qualification_filters[] = array('field' => 'bid', 'value' => $account['bid']);
  }
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $qualifications = hrm_backend_config_qualification_get_data($qualification_filters);
  $branch_names = hrm_basic_branch_names($filters);

  $role_permissions = hrm_basic_role_permissions($account['role']);


  $rows = array();
  foreach ($qualifications as $id => $qualification) {
    $row_classes = array();
    $permission_data = array('qualification' => $qualification, 'permissions' => $role_permissions);
    $access_edit_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_EDIT, $account, $permission_data);

    $row_classes[] = ($access_edit_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_edit_permission ? url('config/pim/skills/edit/' . $id) : '');
    $rows[$id] = array(
      'data' => array(
        array('data' => $qualification['name']),
        array('data' => check_plain($qualification['description'])),
        array('data' => $branch_names[$qualification['bid']]),
      ),
      'class' => $row_classes,
      'data-url' => $tr_data_url,
    );
  };

  $header = array(
    array('data' => 'Name'),
    array('data' => 'Description'),
    array('data' => 'Branch')
  );

  $list = array(
    'attributes' => array('class' => array('table-vcenter', 'table-hover')),
    'rows' => $rows,
    'header' => $header
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

  $actions = array();
  if (hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_ADD, $account)) {
    $actions[] = l(HRM_FA_PLUS_BLUE . 'Add skill', 'config/pim/skills/add/0', array('html' => TRUE));
  }

  $variables = array(
    'title' => '<strong>Skills</strong> list',
    'actions' => $actions,
    'list' => $list
  );

  return theme('page_wrapper_entity_list', $variables);
}

function hrm_backend_config_skills_form_page($id) {

  $account = hrm_basic_account_get_info();
  $qualifications = array();

  if ($id) {
    $qualifications = hrm_backend_config_qualification_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($qualifications)) {
      drupal_not_found();
      exit();
    }
    $edit_qualification_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_EDIT, $account, array('qualification' => $qualifications));
    if (!$edit_qualification_permission) {
      drupal_access_denied();
      exit();
    }
    drupal_set_title('Edit ' . $qualifications['name'] . ' - ' . hrm_basic_branch_get_name($qualifications['bid']));
  }
  else {
    $add_qualifications_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_ADD, $account);
    if (!$add_qualifications_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add qualification - ' . hrm_basic_branch_get_name($account['bid']));
  }

  $qualification_form = drupal_get_form('hrm_backend_config_skills_form', $qualifications, $account);

  $variables = array(
    'entity' => HRM_ENTITY_SKILLS,
    'title' => 'Qualification information',
    'form' => $qualification_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_skills_form($form, &$form_state, $qualifications, $account) {

  $form['name'] = array(
    '#title' => 'Skills',
    '#type' => 'textfield',
    '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($qualifications['name']) ? $qualifications['name'] : '')
  );

  $form['description'] = array('#title' => 'Description',
    '#type' => 'textarea',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#rows' => 5,
    '#empty_option' => '',
    '#attributes' => array('class' => array(''),),
    '#default_value' => (isset($qualifications['description']) ? $qualifications['description'] : '')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ' Save',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($qualifications['id']) && $qualifications['id']) {
    $delete_qualifications_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_DELETE, $account, array('qualification' => $qualifications));

    if ($delete_qualifications_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#submit' => array('hrm_backend_config_skills_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
    else {
      
    }
  }

  $form['#attributes']['class'][] = 'identity-document-add-form';
  $form['#attributes']['class'][] = 'js-validation-bootstrap';
  $form['#attributes']['class'][] = 'push';

  return $form;
}

function hrm_backend_config_skills_form_submit(&$form, &$form_state) {

  $qualifications = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  if (isset($qualifications['id']) && $qualifications['id']) {
    db_update('hrm_config_pim_qualification')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'changed' => time(),
      ))
      ->condition('id', $qualifications['id'], '=')
      ->execute();
  }
  else {
    $documents['id'] = db_insert('hrm_config_pim_qualification')
      ->fields(array(
        'oid' => $account['oid'],
        'bid' => $account['bid'],
        'type' => HRM_QUALIFICATION_SKILLS,
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  }

  drupal_set_message($form_state['values']['name'] . ' qualification saved successfully***');
  drupal_goto('config/pim/skills');
}

function hrm_backend_config_skills_form_delete(&$form, &$form_state) {
  $qualifications = $form_state['build_info']['args'][0];

  db_delete('hrm_config_pim_qualification')->condition('id', $qualifications['id'], '=')->execute();

  drupal_set_message($qualifications['name'] . ' qualification deleted successfully***');
  drupal_goto('config/pim/skills');
}

/*
 * Education
 */

function hrm_backend_config_education() {
  $account = hrm_basic_account_get_info();

  $qualification_filters = array(array('field' => 'oid', 'value' => $account['oid'], 'field' => 'type', 'value' => HRM_QUALIFICATION_EDUCATION));
  $filters = array(array('field' => 'oid', 'value' => $account['oid']));

  if ($account['bid']) {
    $filters[] = array('field' => 'bid', 'value' => $account['bid']);
    $qualification_filters[] = array('field' => 'bid', 'value' => $account['bid']);
  }
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $qualifications = hrm_backend_config_qualification_get_data($qualification_filters);
  $branch_names = hrm_basic_branch_names($filters);

  $role_permissions = hrm_basic_role_permissions($account['role']);

  $rows = array();
  foreach ($qualifications as $id => $qualification) {
    $row_classes = array();
    $permission_data = array('qualification' => $qualification, 'permissions' => $role_permissions);
    $access_edit_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_EDIT, $account, $permission_data);
    $row_classes[] = ($access_edit_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_edit_permission ? url('config/pim/education/edit/' . $id) : '');

    $rows[$id] = array(
      'data' => array(
        array('data' => $qualification['name']),
        array('data' => check_plain($qualification['description'])),
        array('data' => $branch_names[$qualification['bid']]),
      ),
      'class' => $row_classes,
      'data-url' => $tr_data_url,
    );
  };

  $header = array(
    array('data' => 'Name'),
    array('data' => 'Description'),
    array('data' => 'Branch')
  );

  $list = array(
    'attributes' => array('class' => array('table-vcenter', 'table-hover')),
    'rows' => $rows,
    'header' => $header
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

  $actions = array();
  if (hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_ADD, $account)) {
    $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Education', 'config/pim/education/add/0', array('html' => TRUE));
  }

  $variables = array(
    'title' => '<strong>Education</strong> list',
    'actions' => $actions,
    'list' => $list
  );

  return theme('page_wrapper_entity_list', $variables);
}

function hrm_backend_config_education_form_page($id) {

  $account = hrm_basic_account_get_info();
  $qualifications = array();

  if ($id) {
    $qualifications = hrm_backend_config_qualification_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($qualifications)) {
      drupal_not_found();
      exit();
    }
    $edit_qualification_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_EDIT, $account, array('qualification' => $qualifications));
    if (!$edit_qualification_permission) {
      drupal_access_denied();
      exit();
    }
    drupal_set_title('Edit ' . $qualifications['name'] . ' - ' . hrm_basic_branch_get_name($qualifications['bid']));
  }
  else {
    $add_qualification_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_ADD, $account);
    if (!$add_qualification_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add qualification - ' . hrm_basic_branch_get_name($account['bid']));
  }

  $qualification_form = drupal_get_form('hrm_backend_config_education_form', $qualifications, $account);

  $variables = array(
    'entity' => HRM_ENTITY_EDUCATION,
    'title' => 'Qualification information',
    'form' => $qualification_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_education_form($form, &$form_state, $qualifications, $account) {

  $form['name'] = array(
    '#title' => 'Education',
    '#type' => 'textfield',
    '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($qualifications['name']) ? $qualifications['name'] : '')
  );

  $form['description'] = array('#title' => 'Description',
    '#type' => 'textarea',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#rows' => 5,
    '#empty_option' => '',
    '#attributes' => array('class' => array(''),),
    '#default_value' => (isset($qualifications['description']) ? $qualifications['description'] : '')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ' Save',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($qualifications['id']) && $qualifications['id']) {
    $delete_qualifications_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_DELETE, $account, array('qualification' => $qualifications));

    if ($delete_qualifications_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#submit' => array('hrm_backend_config_education_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
    else {
      
    }
  }

  $form['#attributes']['class'][] = 'identity-document-add-form';
  $form['#attributes']['class'][] = 'js-validation-bootstrap';
  $form['#attributes']['class'][] = 'push';

  return $form;
}

function hrm_backend_config_education_form_submit(&$form, &$form_state) {

  $qualifications = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  if (isset($qualifications['id']) && $qualifications['id']) {
    db_update('hrm_config_pim_qualification')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'changed' => time(),
      ))
      ->condition('id', $qualifications['id'], '=')
      ->execute();
  }
  else {
    $documents['id'] = db_insert('hrm_config_pim_qualification')
      ->fields(array(
        'oid' => $account['oid'],
        'bid' => $account['bid'],
        'type' => HRM_QUALIFICATION_EDUCATION,
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  }

  drupal_set_message($form_state['values']['name'] . ' qualification saved successfully***');
  drupal_goto('config/pim/education');
}

function hrm_backend_config_education_form_delete(&$form, &$form_state) {
  $qualifications = $form_state['build_info']['args'][0];

  db_delete('hrm_config_pim_qualification')->condition('id', $qualifications['id'], '=')->execute();

  drupal_set_message($qualifications['name'] . ' qualification deleted successfully***');
  drupal_goto('config/pim/education');
}

/*
 * Licens
 */

function hrm_backend_config_licens() {
  $account = hrm_basic_account_get_info();

  $qualification_filters = array(array('field' => 'oid', 'value' => $account['oid'], 'field' => 'type', 'value' => HRM_QUALIFICATION_LICENSES));
  $filters = array(array('field' => 'oid', 'value' => $account['oid']));

  if ($account['bid']) {
    $filters[] = array('field' => 'bid', 'value' => $account['bid']);
    $qualification_filters[] = array('field' => 'bid', 'value' => $account['bid']);
  }
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $qualifications = hrm_backend_config_qualification_get_data($qualification_filters);
  $branch_names = hrm_basic_branch_names($filters);

  $role_permissions = hrm_basic_role_permissions($account['role']);


  $rows = array();
  foreach ($qualifications as $id => $qualification) {
    $row_classes = array();
    $permission_data = array('qualification' => $qualification, 'permissions' => $role_permissions);
    $access_edit_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_EDIT, $account, $permission_data);
    $row_classes[] = ($access_edit_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_edit_permission ? url('config/pim/licens/edit/' . $id) : '');

    $rows[$id] = array(
      'data' => array(
        array('data' => $qualification['name']),
        array('data' => check_plain($qualification['description'])),
        array('data' => $branch_names[$qualification['bid']]),
      ),
      'class' => $row_classes,
      'data-url' => $tr_data_url,
    );
  };

  $header = array(
    array('data' => 'Name'),
    array('data' => 'Description'),
    array('data' => 'Branch')
  );

  $list = array(
    'attributes' => array('class' => array('table-vcenter', 'table-hover')),
    'rows' => $rows,
    'header' => $header
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

  $actions = array();
  if (hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_ADD, $account)) {
    $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Licens', 'config/pim/licens/add/0', array('html' => TRUE));
  }

  $variables = array(
    'title' => '<strong>Licens</strong> list',
    'actions' => $actions,
    'list' => $list
  );

  return theme('page_wrapper_entity_list', $variables);
}

function hrm_backend_config_licens_form_page($id) {

  $account = hrm_basic_account_get_info();
  $qualifications = array();

  if ($id) {
    $qualifications = hrm_backend_config_qualification_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($qualifications)) {
      drupal_not_found();
      exit();
    }
    $edit_qualification_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_EDIT, $account, array('qualification' => $qualifications));
    if (!$edit_qualification_permission) {
      drupal_access_denied();
      exit();
    }
    drupal_set_title('Edit ' . $qualifications['name'] . ' - ' . hrm_basic_branch_get_name($qualifications['bid']));
  }
  else {
    $add_qualification_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_ADD, $account);
    if (!$add_qualification_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add qualification - ' . hrm_basic_branch_get_name($account['bid']));
  }

  $qualification_form = drupal_get_form('hrm_backend_config_licens_form', $qualifications, $account);

  $variables = array(
    'entity' => HRM_ENTITY_LICENS,
    'title' => 'Licens information',
    'form' => $qualification_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_licens_form($form, &$form_state, $qualifications, $account) {

  $form['name'] = array(
    '#title' => 'Licens',
    '#type' => 'textfield',
    '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($qualifications['name']) ? $qualifications['name'] : '')
  );

  $form['description'] = array('#title' => 'Description',
    '#type' => 'textarea',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#rows' => 5,
    '#empty_option' => '',
    '#attributes' => array('class' => array(''),),
    '#default_value' => (isset($qualifications['description']) ? $qualifications['description'] : '')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($qualifications['id']) && $qualifications['id']) {
    $delete_qualifications_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_DELETE, $account, array('qualification' => $qualifications));

    if ($delete_qualifications_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#submit' => array('hrm_backend_config_licens_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
    else {
      
    }
  }

  $form['#attributes']['class'][] = 'identity-document-add-form';
  $form['#attributes']['class'][] = 'js-validation-bootstrap';
  $form['#attributes']['class'][] = 'push';

  return $form;
}

function hrm_backend_config_licens_form_submit(&$form, &$form_state) {

  $qualifications = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  if (isset($qualifications['id']) && $qualifications['id']) {
    db_update('hrm_config_pim_qualification')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'changed' => time(),
      ))
      ->condition('id', $qualifications['id'], '=')
      ->execute();
  }
  else {
    $documents['id'] = db_insert('hrm_config_pim_qualification')
      ->fields(array(
        'oid' => $account['oid'],
        'bid' => $account['bid'],
        'type' => HRM_QUALIFICATION_LICENSES,
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  }

  drupal_set_message($form_state['values']['name'] . ' qualification saved successfully***');
  drupal_goto('config/pim/licens');
}

function hrm_backend_config_licens_form_delete(&$form, &$form_state) {
  $qualifications = $form_state['build_info']['args'][0];

  db_delete('hrm_config_pim_qualification')->condition('id', $qualifications['id'], '=')->execute();

  drupal_set_message($qualifications['name'] . ' qualification deleted successfully***');
  drupal_goto('config/pim/licens');
}

/*
 * Languages
 */

function hrm_backend_config_languages() {
  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $qualification_filters = array(array('field' => 'oid', 'value' => $account['oid'], 'field' => 'type', 'value' => HRM_QUALIFICATION_LANGUAGES));
  $filters = array(array('field' => 'oid', 'value' => $account['oid']));

  if ($account['bid']) {
    $filters[] = array('field' => 'bid', 'value' => $account['bid']);
    $qualification_filters[] = array('field' => 'bid', 'value' => $account['bid']);
  }

  $qualifications = hrm_backend_config_qualification_get_data($qualification_filters);
  $branch_names = hrm_basic_branch_names($filters);
  $role_permissions = hrm_basic_role_permissions($account['role']);


  $rows = array();
  foreach ($qualifications as $id => $qualification) {
    $row_classes = array();
    $permission_data = array('qualification' => $qualification, 'permissions' => $role_permissions);
    $access_edit_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_EDIT, $account, $permission_data);
    $row_classes[] = ($access_edit_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_edit_permission ? url('config/pim/languages/edit/' . $id) : '');

    $rows[$id] = array(
      'data' => array(
        array('data' => $qualification['name']),
        array('data' => check_plain($qualification['description'])),
        array('data' => $branch_names[$qualification['bid']]),
      ),
      'class' => $row_classes,
      'data-url' => $tr_data_url,
    );
  };

  $header = array(
    array('data' => 'Name'),
    array('data' => 'Description'),
    array('data' => 'Branch')
  );

  $list = array(
    'attributes' => array('class' => array('table-vcenter', 'table-hover')),
    'rows' => $rows,
    'header' => $header
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-sort-export' : 'empty-datatable');

  $actions = array();
  if (hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_ADD, $account)) {
    $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Language', 'config/pim/languages/add/0', array('html' => TRUE));
  }

  $variables = array(
    'title' => '<strong>Languages</strong> list',
    'actions' => $actions,
    'list' => $list
  );

  return theme('page_wrapper_entity_list', $variables);
}

function hrm_backend_config_languages_form_page($id) {

  $account = hrm_basic_account_get_info();
  $qualifications = array();

  if ($id) {
    $qualifications = hrm_backend_config_qualification_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($qualifications)) {
      drupal_not_found();
      exit();
    }
    $edit_qualification_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_EDIT, $account, array('qualification' => $qualifications));
    if (!$edit_qualification_permission) {
      drupal_access_denied();
      exit();
    }
    drupal_set_title('Edit ' . $qualifications['name'] . ' - ' . hrm_basic_branch_get_name($qualifications['bid']));
  }
  else {
    $add_qualification_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_ADD, $account);
    if (!$add_qualification_permission) {
      drupal_access_denied();
      exit();
    }

    drupal_set_title('Add qualification - ' . hrm_basic_branch_get_name($account['bid']));
  }

  $qualification_form = drupal_get_form('hrm_backend_config_languages_form', $qualifications, $account);

  $variables = array(
    'entity' => HRM_ENTITY_LICENS,
    'title' => 'Languages information',
    'form' => $qualification_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_languages_form($form, &$form_state, $qualifications, $account) {

  $form['name'] = array(
    '#title' => 'Language',
    '#type' => 'textfield',
    '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($qualifications['name']) ? $qualifications['name'] : '')
  );

  $form['description'] = array('#title' => 'Description',
    '#type' => 'textarea',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#rows' => 5,
    '#empty_option' => '',
    '#attributes' => array('class' => array(''),),
    '#default_value' => (isset($qualifications['description']) ? $qualifications['description'] : '')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($qualifications['id']) && $qualifications['id']) {
    $delete_qualifications_permission = hrm_backend_config_pim_permissions(HRM_PERM_CONFIG_QUALIFICATIONS_DELETE, $account, array('qualification' => $qualifications));

    if ($delete_qualifications_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#submit' => array('hrm_backend_config_languages_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
    else {
      
    }
  }

  $form['#attributes']['class'][] = 'identity-document-add-form';
  $form['#attributes']['class'][] = 'js-validation-bootstrap';
  $form['#attributes']['class'][] = 'push';

  return $form;
}

function hrm_backend_config_languages_form_submit(&$form, &$form_state) {

  $qualifications = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  if (isset($qualifications['id']) && $qualifications['id']) {
    db_update('hrm_config_pim_qualification')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'changed' => time(),
      ))
      ->condition('id', $qualifications['id'], '=')
      ->execute();
  }
  else {
    $documents['id'] = db_insert('hrm_config_pim_qualification')
      ->fields(array(
        'oid' => $account['oid'],
        'bid' => $account['bid'],
        'type' => HRM_QUALIFICATION_LANGUAGES,
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  }

  drupal_set_message($form_state['values']['name'] . ' qualification saved successfully***');
  drupal_goto('config/pim/languages');
}

function hrm_backend_config_languages_form_delete(&$form, &$form_state) {
  $qualifications = $form_state['build_info']['args'][0];

  db_delete('hrm_config_pim_qualification')->condition('id', $qualifications['id'], '=')->execute();

  drupal_set_message($qualifications['name'] . ' qualification deleted successfully***');
  drupal_goto('config/pim/languages');
}
