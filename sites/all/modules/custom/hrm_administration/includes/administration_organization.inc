<?php

/*
 * LOVs functions
 */

function hrm_administration_organizations() {
  drupal_set_title('Organizations');
  $organizations = hrm_basic_organization_get_data();

  $rows = array();
  foreach ($organizations as $oid => $organization) {
    $image_uri = hrm_basic_file_get_managed_file_uri($organization['logo'], HRM_DEFAULT_IMAGES_ORGANIZATION_LOGO);
    $image = theme('image', array('path' => $image_uri, 'attributes' => array('class' => array('img-48', 'img-circle'))));
    $rows[]['data'] = array(
      array('data' => $oid),
      array('data' => $image, 'class' => array('text-center')),
      array('data' => l($organization['name'], 'admin/hrm-administration/organization/' . $oid)),
      array('data' => count($organization['locations'])),
      array('data' => count($organization['users'])),
      array('data' => count($organization['employees'])),
      array('data' => count($organization['deactivated_employees'])),
    );
  }

  $header = array(
    array('data' => 'ID'),
    array('data' => 'Logo'),
    array('data' => 'Name'),
    array('data' => 'Locations #'),
    array('data' => 'Users #'),
    array('data' => 'Employees #'),
    array('data' => 'Blocked Employees #'),
  );
  $list = array(
    'attributes' => array('class' => array()),
    'header' => $header,
    'rows' => $rows
  );

  return theme('table', $list);
}

//Organization details
function hrm_administration_organization_details($oid) {
  $organization = hrm_basic_organization_get_data(array(array('field' => 'oid', 'value' => $oid)), TRUE, TRUE);

  $image_uri = hrm_basic_file_get_managed_file_uri($organization['logo'], HRM_DEFAULT_IMAGES_ORGANIZATION_LOGO);
  $image = theme('image', array('path' => $image_uri, 'attributes' => array('class' => array('img-height-72'))));

  $details_items = array(
    array('label' => 'ID', 'value' => $organization['oid']),
    array('label' => 'Name', 'value' => $organization['name']),
    array('label' => 'Logo', 'value' => $image),
    array('label' => 'Created by', 'value' => hrm_basic_account_get_name($organization['uid'])),
    array('label' => 'Created', 'value' => date(HRM_DATE_SHORT_TIME_12, $organization['created'])),
    array('label' => 'Changed', 'value' => date(HRM_DATE_SHORT_TIME_12, $organization['changed'])),
  );

  $output = hrm_administration_generate_output_detials($details_items, 'Organization Details');

  return $output;
}

//Organization form
function hrm_administration_organization_form($form, &$form_state, $oid = 0) {
  $organization = array();
  $logo_dest = HRM_DIR_TMP;

  if ($oid) {
    $organization = hrm_basic_organization_get_data(array(array('field' => 'oid', 'value' => $oid)), TRUE, TRUE);

    $logo_dest = strtr(HRM_DIR_ENTITY_LOGO, array('%entity' => HRM_ENTITY_ORGANIZATION, '%entity_id' => $oid));
    file_prepare_directory($logo_dest, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  }
  $form_state['build_info']['args'][1] = $organization;

  drupal_set_title('Add Organization');

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#maxlength' => 400,
    '#required' => TRUE,
    '#default_value' => (isset($organization['name']) ? $organization['name'] : '')
  );

  $form['logo'] = array(
    '#title' => 'Logo',
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_PICTURE),),
    '#upload_location' => $logo_dest,
    '#default_value' => (isset($organization['logo']) ? $organization['logo'] : '')
  );
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save'
  );
  return $form;
}

function hrm_administration_organization_form_validate($form, &$form_state) {
  
}

function hrm_administration_organization_form_submit($form, &$form_state) {
  global $user;

  $oid = $form_state['build_info']['args'][0];
  $organization = $form_state['build_info']['args'][1];

  if (!$oid) {
    $oid = db_insert('hrm_org')
        ->fields(array(
          'name' => $form_state['values']['name'],
          'logo' => $form_state['values']['logo'],
          'status' => HRM_STATUS_ACTIVE,
          'uid' => $user->uid,
          'created' => time(),
          'changed' => time(),
        ))
        ->execute();

    if ($form_state['values']['logo']) {
      $logo_dest = strtr(HRM_DIR_ENTITY_LOGO, array('%entity' => HRM_ENTITY_ORGANIZATION, '%entity_id' => $oid));
      file_prepare_directory($logo_dest, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $logo_file = file_load($form_state['values']['logo']);
      file_move($logo_file, $logo_dest, FILE_EXISTS_RENAME);
      hrm_basic_file_save_file_data($form_state['values']['logo'], HRM_MODULE_BACKEND, strtr(HRM_FILE_USAGE_ENTITY_LOGO, array('%entity' => HRM_ENTITY_ORGANIZATION)), $oid);
    }

    variable_set(HRM_VAR_ORG_DATA_FIRST_YEAR . $oid, date(HRM_DATE_YEAR_YYYY));
    variable_set(HRM_VAR_ORG_LEAVE_FIRST_LEAVE_PERIOD . $oid, date(HRM_DATE_YEAR_YYYY));
    variable_set(HRM_VAR_ORG_LEAVE_CURRENT_LEAVE_PERIOD . $oid, date(HRM_DATE_YEAR_YYYY));

    drupal_set_message($form_state['values']['name'] . ' Organization added successfully');
  }
  else {
    db_update('hrm_org')
        ->fields(array(
          'name' => $form_state['values']['name'],
          'logo' => $form_state['values']['logo'],
          'changed' => time(),
        ))
        ->condition('oid', $oid, '=')
        ->execute();
    if ($organization['logo'] != $form_state['values']['logo']) {
      hrm_basic_file_save_file_data($form_state['values']['logo'], HRM_MODULE_BACKEND, strtr(HRM_FILE_USAGE_ENTITY_LOGO, array('%entity' => HRM_ENTITY_ORGANIZATION)), $oid);

      if ($organization['logo']) {
        hrm_basic_file_delete_file_data($organization['logo'], HRM_MODULE_BACKEND);
      }
    }

    drupal_set_message($form_state['values']['name'] . ' Organization updated successfully');
  }

  $form_state['redirect'] = 'admin/hrm-administration/organization/' . $oid;
}

//Organization Users
function hrm_administration_organization_users($oid) {
  drupal_set_title('Organization Users');

  $roles = hrm_basic_role_names();
  $organization = hrm_basic_organization_get_data(array(array('field' => 'oid', 'value' => $oid)), TRUE);

  $rows = array();
  foreach ($organization['users'] as $uid => $user) {
    $image_uri = hrm_basic_file_get_managed_file_uri($organization['logo'], HRM_DEFAULT_IMAGES_USER);
    $image = theme('image', array('path' => $image_uri, 'attributes' => array('class' => array('img-48', 'img-circle'))));

    $row_class = (!$user['status'] ? 'themed-background-danger' : '');
    $rows[] = array(
      'data' => array(array('data' => $uid),
        array('data' => $image, 'class' => array('text-center')),
        array('data' => l($user['name'], 'admin/hrm-administration/organization/' . $oid . '/users/edit/' . $uid)),
        array('data' => $user['email']),
        array('data' => $roles[$user['role']]),
        array('data' => ($user['status']) ? 'Active' : 'Blocked')),
      'class' => array($row_class)
    );
  }

  $header = array(
    array('data' => 'UID'),
    array('data' => 'Photo'),
    array('data' => 'Name'),
    array('data' => 'Email'),
    array('data' => 'Role'),
    array('data' => 'Status'),
  );
  $list = array(
    'attributes' => array('class' => array()),
    'header' => $header,
    'rows' => $rows
  );

  return theme('table', $list);
}

function hrm_administration_organization_user_form($form, &$form_state, $oid, $uid = 0) {
  $roles = user_roles();
  unset($roles[HRM_ROLE_ANONYMOUS]);
  unset($roles[HRM_ROLE_AUTHENTICATED]);

  $account = array();
  $photo_dest = HRM_DIR_TMP;
  if ($uid) {
    $account = hrm_basic_account_get_info($uid);

    $photo_dest = strtr(HRM_DIR_ENTITY_SUB, array('%entity' => HRM_ENTITY_ORGANIZATION, '%entity_id' => $oid, '%sub_entity' => HRM_ENTITY_USER, '%sub_entity_id' => $uid)) . '/photo';
    file_prepare_directory($photo_dest, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  }
  $form_state['build_info']['args'][2] = $account;

  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => (isset($account['name']) ? $account['role'] : '')
  );

  $form['email'] = array(
    '#title' => 'Email Address',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => (isset($account['email']) ? $account['email'] : '')
  );

  $form['role'] = array(
    '#title' => 'Role',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $roles,
    '#empty_option' => '',
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($account['role']) ? $account['role'] : '')
  );
  $form['photo'] = array(
    '#title' => 'Photo',
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_PICTURE),),
    '#upload_location' => $photo_dest,
    '#default_value' => (isset($account['photo']) ? $account['photo'] : '')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Add User',
    '#attributes' => array(
      'class' => array('btn-primary', 'btn-sm')
    )
  );
  if ($uid) {
    $form['submit']['#value'] = 'Save User';
    if ($account['status'] == HRM_STATUS_ACTIVE) {
      $form['block'] = array(
        '#type' => 'submit',
        '#value' => 'Block User',
        '#submit' => array('hrm_administration_organization_user_form_submit'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array())
      );
    }
    else {
      $form['activate'] = array(
        '#type' => 'submit',
        '#value' => 'Activate User',
        '#submit' => array('hrm_administration_organization_user_form_submit'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array())
      );
    }
  }

  return $form;
}

function hrm_administration_organization_user_form_validate(&$form, &$form_state) {
  $uid = $form_state['build_info']['args'][1];
  $email = $form_state['values']['email'];

  if (!valid_email_address($email)) {
    form_set_error('email', 'Invalid email address');
  }

  $email_exist = db_query('SELECT COUNT(*) FROM {users} WHERE mail = :mail AND uid <> :uid', array(':mail' => $email, ':uid' => $uid))->fetchField(0);
  if ($email_exist) {
    form_set_error('email', 'Email already in use');
  }
}

function hrm_administration_organization_user_form_submit(&$form, &$form_state) {
  $oid = $form_state['build_info']['args'][0];
  $uid = $form_state['build_info']['args'][1];
  $account = $form_state['build_info']['args'][2];

  if (isset($form_state['values']['block']) && $form_state['values']['op'] == $form_state['values']['block']) {
    $edit = array(
      'status' => HRM_USER_STATUS_BLOCKED,
    );

    $updated_user = user_load($uid);
    $updated_account = user_save($updated_user, $edit);

    db_update('hrm_users')
        ->fields(array(
          'status' => HRM_USER_STATUS_BLOCKED,
          'changed' => time(),
        ))
        ->condition('uid', $uid, '=')
        ->execute();

    drupal_set_message('User blocked successfully');
  }
  elseif (isset($form_state['values']['activate']) && $form_state['values']['op'] == $form_state['values']['activate']) {
    $edit = array(
      'status' => HRM_STATUS_ACTIVE,
    );

    $updated_user = user_load($uid);
    $updated_account = user_save($updated_user, $edit);

    db_update('hrm_users')
        ->fields(array(
          'status' => HRM_STATUS_ACTIVE,
          'changed' => time(),
        ))
        ->condition('uid', $uid, '=')
        ->execute();

    drupal_set_message('User activated successfully');
  }
  else {
    $email = $form_state['values']['email'];
    $username = $email;
    $password = hrm_basic_text_generate_random_text(6);
    $roles = array(
      HRM_ROLE_AUTHENTICATED => hrm_basic_role_names(HRM_ROLE_AUTHENTICATED, TRUE),
      $form_state['values']['role'] => hrm_basic_role_names($form_state['values']['role'], TRUE),
    );

    if (!$uid) {
      $new_user_fields = array(
        'name' => $username,
        'pass' => $password,
        'mail' => $email,
        'status' => HRM_STATUS_ACTIVE,
        'init' => $email,
        'roles' => $roles,
      );
      $new_user = user_save(NULL, $new_user_fields);
      if ($new_user) {
        $uid = $new_user->uid;
        db_insert('hrm_users')
            ->fields(array(
              'uid' => $uid,
              'oid' => $oid,
//              'bid' => $bid,
              'username' => $username,
              'password' => $new_user->pass,
              'email' => $email,
              'name' => $form_state['values']['name'],
              'photo' => (isset($form_state['values']['photo']) && !empty($form_state['values']['photo']) ? $form_state['values']['photo'] : 0),
              'role' => $form_state['values']['role'],
              'status' => HRM_STATUS_ACTIVE,
              'created' => time(),
              'changed' => time(),
            ))
            ->execute();

        $account = hrm_basic_account_get_info($uid);
        hrm_emails_notification_user_new($account, $password);

        drupal_set_message('User added succefully');
      }
      else {
        drupal_set_message("Invalid user information. Please submit it again.", 'error');
      }
    }
    else {
      $edit = array(
        'name' => $username,
        'mail' => $email,
        'init' => $email,
        'roles' => $roles,
      );

      $updated_user = user_load($uid);
      $updated_account = user_save($updated_user, $edit);

      if ($updated_account) {
        db_update('hrm_users')
            ->fields(array(
              'username' => $username,
              'email' => $email,
              'name' => $form_state['values']['name'],
              'photo' => (isset($form_state['values']['photo']) && !empty($form_state['values']['photo']) ? $form_state['values']['photo'] : 0),
              'role' => $form_state['values']['role'],
              'changed' => time(),
            ))
            ->condition('uid', $uid, '=')
            ->execute();

        drupal_set_message('User updated successfully');
      }
      else {
        drupal_set_message('Information has not been correctly updated. Please resubmit the form', 'error');
      }
    }
  }

  drupal_goto('admin/hrm-administration/organization/' . $oid . '/users');
}

//Organization Resources
function hrm_administration_organization_resources_form($form, &$form_state, $oid) {
  $logo_dest = strtr(HRM_DIR_ENTITY_LOGO, array('%entity' => HRM_ENTITY_ORGANIZATION, '%entity_id' => $oid)) . '/sidebar';
  file_prepare_directory($logo_dest, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $form['sidebar_logo'] = array(
    '#title' => 'Logo',
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_PICTURE),),
    '#upload_location' => $logo_dest,
    '#default_value' => variable_get(HRM_VAR_ORG_SIDEBAR_LOGO . $oid, '')
  );

  $form['data_first_year'] = array(
    '#type' => 'textfield',
    '#title' => 'Data first year',
    '#maxlength' => 4,
    '#required' => TRUE,
    '#default_value' => variable_get(HRM_VAR_ORG_DATA_FIRST_YEAR . $oid, date(HRM_DATE_YEAR_YYYY)),
  );
  $form['first_leave_period'] = array(
    '#type' => 'textfield',
    '#title' => 'First leave period',
    '#maxlength' => 4,
    '#required' => TRUE,
    '#default_value' => variable_get(HRM_VAR_ORG_LEAVE_FIRST_LEAVE_PERIOD . $oid, date(HRM_DATE_YEAR_YYYY)),
  );
  $form['current_leave_period'] = array(
    '#type' => 'textfield',
    '#title' => 'Current leave period',
    '#maxlength' => 4,
    '#required' => TRUE,
    '#default_value' => variable_get(HRM_VAR_ORG_LEAVE_CURRENT_LEAVE_PERIOD . $oid, date(HRM_DATE_YEAR_YYYY)),
  );

  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save'
  );
  return $form;
}

function hrm_administration_organization_resources_form_validate($form, &$form_state) {
  
}

function hrm_administration_organization_resources_form_submit($form, &$form_state) {
  $oid = $form_state['build_info']['args'][0];

  $sidebar_logo = variable_get(HRM_VAR_ORG_SIDEBAR_LOGO . $oid, '');
  if ($form_state['values']['sidebar_logo'] != $sidebar_logo) {
    if ($form_state['values']['sidebar_logo']) {
      variable_set(HRM_VAR_ORG_SIDEBAR_LOGO . $oid, $form_state['values']['sidebar_logo']);
      hrm_basic_file_save_file_data($form_state['values']['sidebar_logo'], HRM_MODULE_ADMINISTRATION, strtr(HRM_FILE_USAGE_ENTITY_LOGO, array('%entity' => HRM_ENTITY_ORGANIZATION)), $oid);
    }

    if ($sidebar_logo) {
      hrm_basic_file_delete_file_data($sidebar_logo, HRM_MODULE_ADMINISTRATION);
    }
  }

  variable_set(HRM_VAR_ORG_DATA_FIRST_YEAR . $oid, $form_state['values']['data_first_year']);
  variable_set(HRM_VAR_ORG_LEAVE_FIRST_LEAVE_PERIOD . $oid, $form_state['values']['first_leave_period']);
  variable_set(HRM_VAR_ORG_LEAVE_CURRENT_LEAVE_PERIOD . $oid, $form_state['values']['current_leave_period']);

  drupal_set_message('Organization resources updated successfully');
  $form_state['redirect'] = 'admin/hrm-administration/organization/' . $oid . '/resources';
}

//Organization Resources
function hrm_administration_organization_assets_form($form, &$form_state, $oid) {

//  $generate_receipt = variable_get(HRM_VAR_ORG_ASSET_GENERATE_ASSIGN_RECIEPT . $oid);
//  $receipt_content = variable_get(HRM_VAR_ORG_ASSET_ASSIGN_RECIEPT_CONTENT . $oid);
//  $generate_return = variable_get(HRM_VAR_ORG_ASSET_GENERATE_RETURN_RECIEPT . $oid);
//  $return_content = variable_get(HRM_VAR_ORG_ASSET_RETURN_RECIEPT_CONTENT . $oid);
//
//  $form['assign'] = array(
//    '#type' => 'fieldset',
//    '#title' => 'Asset Assign',
//    '#collapsible' => TRUE,
//    '#attributes' => array('class' => array()),
//  );
//
//
//  $form['assign']['generate_assign_reciept'] = array (
//    '#type' => 'checkbox',
//    '#title' => 'Generate Asset Assign Receipt',
//    '#default_value' => variable_get($generate_receipt, 0),
//  );
//  $form['assign']['assign_reciept_content'] = array(
//    '#type' => 'textarea',
//    '#title' => 'Asset Assign Receipt Content',
//    '#default_value' => $receipt_content,
//  );
//
//  $form['assign']['generate_return_reciept'] = array(
//    '#type' => 'checkbox',
//    '#title' => 'Generate Asset Return Receipt',
//    '#default_value' => variable_get($generate_return, 0),
//  );
//  $form['assign']['return_reciept_content'] = array(
//    '#type' => 'textarea',
//    '#title' => 'Asset Return Receipt Content',
//    '#default_value' => $return_content,
//  );


  $form['assets'] = array(
    '#type' => 'fieldset',
    '#title' => 'Asset Assign',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array()),
  );

  $generate_receipt = HRM_VAR_ORG_ASSET_GENERATE_ASSIGN_RECIEPT . $oid;
  $receipt_content = HRM_VAR_ORG_ASSET_ASSIGN_RECIEPT_CONTENT . $oid;
  $generate_return = HRM_VAR_ORG_ASSET_GENERATE_RETURN_RECIEPT . $oid;
  $return_content = HRM_VAR_ORG_ASSET_RETURN_RECIEPT_CONTENT . $oid;


  $generate_asset_receipt = variable_get($generate_receipt, 0);
  $asset_receipt_form = variable_get($receipt_content, '');
  $generate_asset_return = variable_get($generate_return, 0);
  $asset_return_form = variable_get($return_content, '');



  $form['assets']['generate_assign_reciept'] = array(
    '#type' => 'checkbox',
    '#title' => 'Generate Asset Receipt Form',
    '#default_value' => $generate_asset_receipt,
  );
  $form['assets']['assign_reciept_content'] = array(
    '#type' => 'textarea',
    '#title' => 'Asset Receipt Form Content',
    '#default_value' => $asset_receipt_form,
  );

  $form['assets']['generate_return_reciept'] = array(
    '#type' => 'checkbox',
    '#title' => 'Generate Asset Return Form',
    '#default_value' => $generate_asset_return,
  );
  $form['assets']['return_reciept_content'] = array(
    '#type' => 'textarea',
    '#title' => 'Asset Return Form Content',
    '#default_value' => $asset_return_form,
  );

  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['assets']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save'
  );

  return $form;
}

function hrm_administration_organization_assets_form_validate($form, &$form_state) {
  
}

function hrm_administration_organization_assets_form_submit($form, &$form_state) {
// CHECK AGAIN
// MOHAMED
  $oid = $form_state['build_info']['args'][0];

  variable_set(HRM_VAR_ORG_ASSET_GENERATE_ASSIGN_RECIEPT . $oid, $form_state['values']['generate_assign_reciept']);
  variable_set(HRM_VAR_ORG_ASSET_ASSIGN_RECIEPT_CONTENT . $oid, $form_state['values']['assign_reciept_content']);
  variable_set(HRM_VAR_ORG_ASSET_GENERATE_RETURN_RECIEPT . $oid, $form_state['values']['generate_return_reciept']);
  variable_set(HRM_VAR_ORG_ASSET_RETURN_RECIEPT_CONTENT . $oid, $form_state['values']['return_reciept_content']);

  drupal_set_message('Organization assets updated successfully');
  $form_state['redirect'] = 'admin/hrm-administration/organization/' . $oid . '/assets';
}

//Departments
function hrm_administration_organization_departments_form($form, &$form_state, $oid) {
  $roles = hrm_basic_role_names();

  $department_names = variable_get(HRM_VAR_ORG_DEPARTMENTS_NAME . $oid, array());
  $form['names'] = array(
    '#type' => 'fieldset',
    '#title' => 'Departments Name',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes' => array('class' => array()),
  );
  foreach ($roles as $role_id => $role_name) {
    $form['names'][$role_id] = array(
      '#type' => 'textfield',
      '#title' => 'Name',
      '#maxlength' => 400,
      '#required' => TRUE,
      '#default_value' => (isset($department_names[$role_id]) ? $department_names[$role_id] : $role_name)
    );
  }

  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save'
  );

  return $form;
}

function hrm_administration_organization_departments_form_validate($form, &$form_state) {
  
}

function hrm_administration_organization_departments_form_submit($form, &$form_state) {
  $oid = $form_state['build_info']['args'][0];

  variable_set(HRM_VAR_ORG_DEPARTMENTS_NAME . $oid, $form_state['values']['names']);

  drupal_set_message('Departments configuration updated successfully');
  $form_state['redirect'] = 'admin/hrm-administration/organization/' . $oid . '/departments';
}
