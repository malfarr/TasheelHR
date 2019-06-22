<?php

function hrm_services_account_get_permissions($empid) {
//  $account = user_load($uid);
//  $permissions = array();
//
//  if ($account) {
//    if (user_access(RTP_PERM_LEADER_EDIT_LEADER, $account) || user_access(RTP_PERM_ASSOCIATE_EDIT_ASSOCIATE, $account)) {
//      $permissions[] = RTP_APP_PERM_ACCESS_BENEF_ID_PHOTO;
//    }
//    if (user_access(RTP_PERM_EN_AFR_CLOSE_AFR, $account)) {
//      $permissions[] = RTP_APP_PERM_ACCESS_AFR;
//    }
//  }
//
//  $log_data = array(
//    'Arguments' => array('uid' => $uid),
//    'Results' => $permissions,
//  );
//  watchdog('RTP SERVICES', 'Account - Get permissions <pre>@arguments</pre>', array('@arguments' => print_r($log_data, TRUE)), WATCHDOG_DEBUG);
//
//  return $permissions;
  return 0;
}

function hrm_services_employee_login($email, $password, $device_id, $device_type) {
  $emp_filter = array(array('field' => 'email', 'value' => $email));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
    $hashed_password = _password_crypt('sha512', $password, HRM_KEY_SHA512_SALT);
    $success_login = FALSE;
   
    if ($hashed_password == $employee['password']) {
      $success_login = TRUE;
    }
    if ($success_login) {
      $json = hrm_services_employee_generate_employee_object($employee);
      db_update('hrm_emp')
        ->fields(array(
          'device_id' => $device_id,
          'device_type' => $device_type,
          'logged_in' => time()
        ))
        ->condition('empid', $employee['empid'], '=')
        ->execute();

      $json->message = 'Successfully logged in';
      $json->status = 200;
      return $json;
    }
    else {
      $json = new stdClass;
      $json->status = 404;
      $json->message_title = 'Login';
      $json->message = 'Invalid password';
      return $json;
    }
  }
  else {
    $json = new stdClass;
    $json->status = 404;
    $json->message_title = 'Login';
    $json->message = 'Account not found or Blocked';
    return $json;
  }
}

function hrm_services_employee_logout($empid) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    if ($employee['logged_in']) {
      db_update('hrm_emp')
        ->fields(array(
          'device_id' => '',
          'device_type' => '',
          'logged_in' => 0
        ))
        ->condition('empid', $empid, '=')
        ->execute();

      $json = new stdClass;
      $json->message = 'Successfully logged out';
      $json->message_title = 'Logout';
      $json->status = 200;
      return $json;
    }
    else {
      $json = new stdClass;
      $json->status = 404;
      $json->message_title = 'Logout';
      $json->message = 'You are not logged in';
      return $json;
    }
  }
  else {
    $json = new stdClass;
    $json->status = 404;
    $json->message_title = 'Logout';
    $json->message = 'This account is not found or blocked';
    return $json;
  }
}

function hrm_services_employee_change_password($empid, $old_password, $new_password) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
    $hashed_old_password = _password_crypt('sha512', $old_password, HRM_KEY_SHA512_SALT);
    $hashed_new_password = _password_crypt('sha512', $new_password, HRM_KEY_SHA512_SALT);

    if ($employee['password'] != $hashed_old_password) {
      $json = new stdClass;
      $json->status = 404;
      $json->message_title = 'Password';
      $json->message = 'Invalid old password';
      return $json;
    }
    else {
      db_update('hrm_emp')
        ->fields(array(
          'password' => $hashed_new_password,
        ))
        ->condition('empid', $empid, '=')
        ->execute();

      $json = new stdClass;
      $json->status = 200;
      $json->message_title = 'Password';
      $json->message = 'Your password has been changed successfully***';
      return $json;
    }
  }
  else {
    $json = new stdClass;
    $json->status = 404;
    $json->message_title = 'Password';
    $json->message = 'Account not found or Blocked';
    return $json;
  }
}

function hrm_services_employee_reset_password($email) {
  $emp_filter = array(array('field' => 'email', 'value' => $email));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
    $password = hrm_basic_text_generate_random_text(6);
    $encrypted_password = _password_crypt('sha512', $password, HRM_KEY_SHA512_SALT);

    db_update('hrm_emp')
      ->fields(array(
        'password' => $encrypted_password,
      ))
      ->condition('empid', $employee['empid'], '=')
      ->execute();

    watchdog("backend-pim", '$password <pre> pass <pre>', array('pass' => $password), WATCHDOG_INFO);
    hrm_emails_notification_pim_employee_reset_password($employee, $password);
    $json = new stdClass;
    $json->message = 'New password has been sent to your email address';
    $json->status = 200;
    return $json;
  }
  else {
    $json = new stdClass;
    $json->status = 404;
    $json->message_title = 'Reset Password';
    $json->message = 'This account is not found or blocked';
    return $json;
  }
}

function hrm_services_employee_get_info($empid) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);

  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    $json = hrm_services_employee_generate_employee_object($employee);
    $json->status = 200;
    return $json;
  }
  else {
    $json = new stdClass;
    $json->status = 404;
    $json->message_title = 'Data information';
    $json->message = 'This account is not found or blocked';
    return $json;
  }
}





