<?php

/*
 * Users Management
 */
include("includes/password.inc");

function hrm_backend_users() {
  $account = hrm_basic_account_get_info();

  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }
  $filters = array(
    array('field' => 'oid', 'value' => $account['oid']),
        array('field' => 'type', 'value' => HRM_USER_TYPE_USER)

  );


  $users = hrm_basic_account_get_data($filters);
  $access_details_permission = user_access(HRM_PERM_USER_ACCESS_DETAILS);
  $rows = array();
  foreach ($users as $uid => $user) {
    $row_class = '';
    if (!$user['status']) {
      $row_class = 'danger';
    }



    $roles = hrm_basic_organization_get_department_name($account['oid']);
    $row_classes = array();
    $row_classes[] = ($access_details_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_details_permission ? url('hrm-user/details/' . $uid) : '');
    $rows[$uid] = array(
      'data' => array(
        array('data' => '', 'class' => array('table-row-index-td')),
        array('data' => ($user['name'])),
        array('data' => $user['photo_image'], 'class' => array('text-center')),
        array('data' => $user['email']),
        array('data' => $roles[$user['role']]),
        array('data' => ($user['status'] ? 'Active' : 'Blocked')),
      ),
      'class' => $row_classes,
      'data-url' => $tr_data_url,
    );
  }
  $header = array(
    array('data' => '#', 'class' => array('nosort', 'nosearch', 'table-row-index-th')),
    array('data' => 'Name'),
    array('data' => 'Picture', 'class' => array('nosort', 'nosearch', 'text-center')),
    array('data' => 'Email'),
    array('data' => 'Role'),
    array('data' => 'Status')
  );
  $list = array(
    'attributes' => array('class' => array('table-hover', 'table-vcenter')),
    'rows' => $rows,
    'header' => $header
  );
  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all-index' : 'empty-datatable');
  $variables = array(
    'title' => '<strong>Users</strong> Management',
    'list' => $list
  );
  return theme('page_wrapper_entity_list_no_header', $variables);
}

/*
 * user details
 */

function hrm_backend_user_details($uid) {
  if ($uid && is_numeric($uid)) {
    $user = hrm_basic_account_get_info($uid);
    if (!empty($user)) {
      $account = hrm_basic_account_get_info();

      $role_permission = hrm_basic_role_permissions($account['role']);
      $permission_data = array('user' => $user, 'permissions' => $role_permission);

      if (hrm_backend_user_permissions($uid, HRM_PERM_USER_ACCESS_DETAILS, $account, $permission_data)) {
        $fields = array();
        $roles = hrm_basic_role_names();
        $fields[] = array('type' => 'raw',
          'wrapper_class' => array('col-sm-12', 'text-center', 'strong'),
          'value' => $user['photo_image_medium'],
          'row_class' => array('no-border'));

        $fields[] = array('type' => 'row',
          'wrapper_class' => array('col-sm-6'),
          'row_class' => array(),
          'label' => 'Name',
          'value' => $user['name'],
          'value_class' => array());

        $fields[] = array('type' => 'row',
          'wrapper_class' => array('col-sm-6'),
          'row_class' => array(),
          'label' => 'Role',
          'value' => $roles[$user['role']],
          'value_class' => array());

        $fields[] = array('type' => 'row',
          'wrapper_class' => array('col-sm-6'),
          'row_class' => array(),
          'label' => 'Email',
          'value' => $user['email'],
          'value_class' => array());

        $fields[] = array('type' => 'row',
          'wrapper_class' => array('col-sm-6'),
          'row_class' => array(),
          'label' => 'Status',
          'value' => $user['status'] ? 'Active' : 'Blocked',
          'value_class' => array());

        $details = theme('section_entity_details', array('fields' => $fields));
        $actions = array();


        if (hrm_backend_user_permissions($uid, HRM_PERM_USER_MANAGE_USERS, $account, $permission_data)) {
          $actions[] = l(HRM_FA_PENCIL_YELLOW . 'Edit', 'hrm-user/edit/' . $uid, array('html' => TRUE));
        }

        $variables = array(
          'details' => $details,
          'actions' => $actions,
          'entity' => HRM_ENTITY_USER
        );
        return theme('page_wrapper_entity_details', $variables);
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

function hrm_backend_user_permissions($uid, $action, $account = array(), $data = array()) {
  $user = (isset($data['user']) && !empty($data['user']) ? $data['user'] : hrm_basic_account_get_data(array(array('field' => 'uid', 'value' => $uid)), TRUE));
  $permissions = (isset($data['permissions']) && !empty($data['permissions']) ? $data['permissions'] : hrm_basic_role_permissions($account['role']));

  if (empty($user) || !isset($account['oid']) || !$account['oid'] || $account['oid'] != $user['oid']) {
    return FALSE;
  }

  switch ($action) {
    /* User Roles */
    case HRM_PERM_USER_ACCESS_DETAILS:
      if (user_access(HRM_PERM_USER_ACCESS_DETAILS) && $account['oid'] &&
        $data['user']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_USER_MANAGE_USERS:
      if (isset($permissions[HRM_PERM_USER_MANAGE_USERS]) &&
        $data['user']['oid'] == $account['oid']) {

        if (isset($permissions[HRM_PERM_USER_MANAGE_ALL_USERS])) {
          return TRUE;
        }

        if ($data['user']['role'] == HRM_ROLE_ADMIN && isset($permissions[HRM_PERM_USER_MANAGE_ADMIN_USERS])) {
          return TRUE;
        }


        if ($data['user']['role'] == HRM_ROLE_HR && isset($permissions[HRM_PERM_USER_MANAGE_HR_USERS])) {
          return TRUE;
        }


        if ($data['user']['role'] == HRM_ROLE_LOGISTICS && isset($permissions[HRM_PERM_USER_MANAGE_LOGISTICS_USERS])) {
          return TRUE;
        }


        if ($data['user']['role'] == HRM_ROLE_FINANCE && isset($permissions[HRM_PERM_USER_MANAGE_FINANCE_USERS])) {
          return TRUE;
        }

        if (isset($permissions[HRM_PERM_USER_MANAGE_OWN_USER]) && $data['user']['uid'] == $account['uid']) {
          return TRUE;
        }
      }
  }
  return FALSE;
}

function hrm_backend_user_form_page($uid) {

  $account = hrm_basic_account_get_info();
  $user = array();
  if ($uid) {
    $user = hrm_basic_account_get_info($uid);

    if (empty($user)) {
      drupal_not_found();
      exit();
    }
    $manage_user_permission = hrm_backend_user_permissions($uid, HRM_PERM_USER_MANAGE_USERS, $account, array('user' => $user));
    if ((!$manage_user_permission) || !isset($account['oid']) || !$account['oid']) {
      drupal_access_denied();
      exit();
    }
    drupal_set_title('Edit ' . $user['name']);
  }
  else {
    if (!user_access(HRM_PERM_USER_ADD) || !isset($account['oid']) || !$account['oid']) {
      drupal_access_denied();
      exit();
    }
    drupal_set_title('Add User');
  }

  $user_form = drupal_get_form('hrm_backend_user_form', $user, $account);
  $variables = array(
    'entity' => HRM_ENTITY_USER,
    'title' => 'User information',
    'form' => $user_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_user_form($form, &$form_state, $user, $account) {

  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => (isset($user['name']) ? $user['name'] : ''),
  );

  $form['email'] = array(
    '#title' => 'Email',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => (isset($user['email']) ? $user['email'] : ''),
    '#attributes' => array('class' => array()),
  );
//$form['pass'] = array(
//  '#type' => 'password', 
//  '#title' => t('Password'), 
//  '#maxlength' => 64, 
//  '#size' => 15,
//      '#default_value' => (isset($user['password']) ? $user['passwoed'] : ''),
//);

  
  $form['role'] = array(
    '#title' => 'Role',
    '#type' => 'select',
    '#options' => hrm_basic_role_names(),
    '#required' => TRUE,
    '#default_value' => (isset($user['role']) ? $user['role'] : ''),
    '#empty_option' => '',
    '#attributes' => array('class' => array('select-chosen')),
  );

  $dest_location = str_replace(array('%entity', '%entity_id'), array(HRM_ENTITY_ORGANIZATION, $account['oid']), HRM_DIR_ENTITY) . '/users/picture';
  file_prepare_directory($dest_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $form['photo'] = array(
    '#title' => 'Photo',
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_PICTURE)),
    '#upload_location' => $dest_location,
    '#default_value' => (isset($user['photo']) ? $user['photo'] : ''),
  );


  $form['submit_suffix'] = array('#markup' => '<div class="form-group">'); // to make line border gone
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ' Save',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
  );


  if (isset($user['uid']) && $user['uid']) {
    $manage_user_permission = hrm_backend_user_permissions($user['uid'], HRM_PERM_USER_MANAGE_USERS, $account, array('user' => $user));
    if ($user['status'] == HRM_STATUS_ACTIVE) {
      if ($manage_user_permission) {
        $form['block'] = array(
          '#type' => 'submit',
          '#value' => 'Block',
          '#submit' => array('hrm_backend_user_form_block'),
          '#limit_validation_errors' => array(),
          '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        );
      }
    }
    if ($user['status'] == HRM_STATUS_DEACTIVETED) {
      if ($manage_user_permission) {
        $form['active'] = array(
          '#type' => 'submit',
          '#value' => 'Activate',
          '#submit' => array('hrm_backend_user_form_activate'),
          '#limit_validation_errors' => array(),
          '#attributes' => array('class' => array('btn-success', 'btn-sm')),
        );
      }
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';

  return $form;
}

function hrm_backend_user_form_validate(&$form, &$form_state) {
  $user = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];
  if (!empty($user)) {
    $uid = $user['uid'];
  }
  else {
    $uid = 0;
  }
  $email = $form_state['values']['email'];

  if (!valid_email_address($email)) {
    form_set_error('email', t('Invalid email address'));
  }

  if ($uid) {
    $email_exist = db_query('SELECT COUNT(*) FROM {hrm_users} WHERE email = :email AND uid <> :uid', array(':email' => $email, ':uid' => $user['uid']))->fetchField(0);
    if ($email_exist) {
      form_set_error('email', t('This already email exists.'));
    }
  }
  else {
    $email_exist = db_query('SELECT COUNT(*) FROM {hrm_users} WHERE email = :email', array(':email' => $email))->fetchField(0);
    if ($email_exist) {
      form_set_error('email', t('Email already in use'));
    }
  }
}

function hrm_backend_user_form_submit(&$form, &$form_state) {
  $user = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];
  $email = $form_state['values']['email'];
  $name = $form_state['values']['name'];
  $hrm_role = $form_state['values']['role'];
  $photo = $form_state['values']['photo'];
  $password = hrm_basic_text_generate_random_text(6);
//  $password = $form_state['values']['pass'];
  $encrypted_password = _password_crypt('sha512', $password, HRM_KEY_SHA512_SALT);

  $organization = $account['oid'];

  $role_name = hrm_basic_role_names($form_state['values']['role'], TRUE);
  $roles = array(
    HRM_ROLE_AUTHENTICATED => 'authenticated user',
    $form_state['values']['role'] => $role_name
  );

  if (isset($user['uid']) && $user['uid']) {
    $uid = $user['uid'];
    $edit = array(
      'name' => $email,
      'mail' => $email,
      'status' => HRM_STATUS_ACTIVE,
      'init' => $email,
      'roles' => $roles,
      'photo' => $photo,
      'field_user_type' => HRM_USER_TYPE_USER,
      'field_user_first_name' => $name,
      'field_user_organization' => $organization,
    );

    $updated_user = user_load($uid);
    $updated_account = user_save($updated_user, $edit);

    if ($updated_account) {
      db_update('hrm_users')
        ->fields(array(
          'name' => $name,
          'email' => $email,
          'role' => $hrm_role,
          'photo' => $photo,
          'username' => $email,
          'changed' => time(),
        ))
        ->condition('uid', $uid, '=')
        ->execute();
    }
    else {
      drupal_set_message("Information has not been correctly updated. Please resubmit the form", 'error***');
    }
  }
  else {
    $new_user_fields = array(
      'name' => $email,
      'pass' => $encrypted_password,
      'mail' => $email,
      'status' => 1,
      'init' => $email,
      'roles' => $roles,
      'photo' => $photo,
            'field_user_type' => HRM_USER_TYPE_USER,
      'field_user_first_name' => $name,
      'field_user_organization' => $organization,
    );
    $new_user = user_save(NULL, $new_user_fields);

    if ($new_user) {
      $data = get_object_vars($new_user);

      $insert_user = db_insert('hrm_users')
        ->fields(array(
          'uid' => $data['uid'],
          'oid' => $account['oid'],
          'username' => $email,
          'type' => HRM_USER_TYPE_USER,
          'password' => $encrypted_password,
          'email' => $email,
          'name' => $name,
          'role' => $hrm_role,
          'photo' => $photo,
          'status' => 1,
          'created' => time(),
          'changed' => time(),
        ))
        ->execute();

      if ($insert_user) {
        hrm_emails_notification_user_new($account, $password);
      }
    }
    else {
      drupal_set_message("Invalid user information. Please submit it again.", 'error');
    }
  }
  drupal_set_message($form_state['values']['name'] . 'User saved successfully***');
  drupal_goto('hrm-user');
}

function hrm_backend_user_form_block(&$form, &$form_state) {

  $user = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];
  $uid = $user['uid'];

  $edit = array(
    'status' => HRM_STATUS_DEACTIVETED,
  );

  $block_user = user_load($uid);
  $updated_account = user_save($block_user, $edit);

  if ($updated_account) {
    db_update('hrm_users')
      ->fields(array(
        'status' => HRM_STATUS_DEACTIVETED,
      ))
      ->condition('uid', $uid, '=')
      ->execute();
    drupal_set_message($user['name'] . ' User has been blocked successfully***');
  }
  else {
    drupal_set_message("user not blocked", 'error');
  }


  drupal_goto('hrm-user');
}

function hrm_backend_user_form_activate(&$form, &$form_state) {

  $user = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];
  $uid = $user['uid'];

  $edit = array(
    'status' => HRM_STATUS_ACTIVE,
  );

  $block_user = user_load($uid);
  $updated_account = user_save($block_user, $edit);

  if ($updated_account) {
    db_update('hrm_users')
      ->fields(array(
        'status' => HRM_STATUS_ACTIVE,
      ))
      ->condition('uid', $uid, '=')
      ->execute();
    drupal_set_message($user['name'] . ' User has been actived successfully***');
  }
  else {
    drupal_set_message("user not actived", 'error');
  }
  drupal_goto('hrm-user');
}
