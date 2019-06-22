<?php

//COMPLETED - Othman
function hrm_backend_pim_employees() {
  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $emp_filters = array(
    array('field' => 'oid', 'value' => $account['oid']),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE)
  );
  $branch_filters = array(array('field' => 'oid', 'value' => $account['oid']));

  $employees = hrm_basic_employee_get_data($emp_filters);
  $emp_ids = array_keys($employees);

  $uncompleted_termination = array();
  if (!empty($emp_ids)) {
    $uncompleted_termination = db_select('hrm_emp_terminated')
        ->fields('hrm_emp_terminated', array('empid'))
        ->condition('empid', $emp_ids, 'IN')
        ->execute()->fetchCol(0);
  }
  $access_detail_permission = user_access(HRM_PERM_PIM_EMP_ACCESS_EMPLOYEE_DETAILS);
  $job_titles = hrm_basic_job_title_names($branch_filters);
  $employee_names = hrm_basic_employee_names();
  $rows = array();
  foreach ($employees as $id => $employee) {
    $tags = '';
    if (in_array($id, $uncompleted_termination)) {
      $tags .= '<i class="fa fa-exclamation-triangle themed-color-red animated bounceIn" title="Employment termination is not completed"></i>';
    }
//print("<pre>");
//print_r($employees);
//exit();

    $row_classes = array();
    $row_classes[] = ($access_detail_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_detail_permission ? url('pim/employee/details/' . $id) : '');
    $rows[$id] = array(
      'data' => array(
        array('data' => '', 'class' => array()),
        array('data' => $tags, 'class' => array()),
        array('data' => $employee['photo_image'], 'class' => array('cell-photo', 'cell-photo-circle')),
        array('data' => ($employee['emp_no'])),
        array('data' => ($employee['name'])),
        array('data' => (isset($employee['job_title']) && isset($job_titles[$employee['job_title']]) ? $job_titles[$employee['job_title']] : '')),
        array('data' => (isset($employee['supervisor_id']) ? $employee_names[$employee['supervisor_id']] : '')),
      ),
      'class' => $row_classes,
      'data-url' => $tr_data_url,
    );
  }

  $header = array(
    array('data' => '#', 'class' => array('nosort', 'nosearch', 'noexport')),
    array('data' => HRM_FA_EXCLAMATION_TRIANGLE, 'class' => array('noexport')),
    array('data' => HRM_FA_USER, 'class' => array('nosort', 'nosearch', 'noexport', 'text-center')),
    array('data' => 'ID'),
    array('data' => 'Name'),
    array('data' => 'Job Title'),
    array('data' => 'Supervisor'),
  );
  $list = array(
    'attributes' => array('class' => array('table-hover', 'table-vcenter')),
    'rows' => $rows,
    'header' => $header,
    'sticky' => FALSE,
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all-index' : 'empty-datatable');

  $variables = array(
    'title' => '<strong>EMPLOYEE </strong> LIST',
    'list' => $list
  );

  return theme('page_wrapper_entity_list_no_header', $variables);
}

function hrm_backend_pim_employee_details($empid) {
  if ($empid && is_numeric($empid)) {
    $employee = hrm_basic_employee_get_data(array(array('field' => 'empid', 'value' => $empid)), TRUE);
    if (!empty($employee)) {
      drupal_set_title($employee['name']);

      $account = hrm_basic_account_get_info();
      $role_permissions = hrm_basic_role_permissions($account['role']);

      $permission_data = array('employee' => $employee, 'permissions' => $role_permissions);

      if (hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_ACCESS_EMPLOYEE_DETAILS, $account, $permission_data)) {
        $edit_employee_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_EDIT_EMPLOYEE, $account, $permission_data);

        $variables = array(
          'entity' => HRM_ENTITY_EMPLOYEE,
          'details' => hrm_backend_pim_employee_display_details($employee, $account, $edit_employee_permission)
        );

        //Job
        if (hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_JOB_ACCESS, $account, $permission_data)) {
          $manage_job_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_JOB_MANAGE, $account, $permission_data);
//          $variables['job'] = hrm_backend_pim_employee_job($employee, $account, $manage_job_permission);
        }


        //Emergency Contacts
        //Report-To
        if (hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_REPORT_TO_ACCESS, $account, $permission_data)) {
          $manage_report_to_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_REPORT_TO_MANAGE, $account, $permission_data);
//          $variables['report_to'] = hrm_backend_pim_employee_report_to($employee, $account, $manage_report_to_permission);
        }

        //Identity Documents
        //Qualification
        //Documents
        //Account
        if (hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_ACCOUNT_ACCESS, $account, $permission_data)) {
          $manage_account_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_ACCOUNT_MANAGE, $account, $permission_data);
          $variables['account'] = hrm_backend_pim_employee_account($employee, $account);
        }

        //Assets
        //Attendance
        //Leave
        if (hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_LEAVES_ACCESS, $account, $permission_data)) {
          $variables['leave']['content'] = '<p>CHECk AGAIN</p>';
        }

//        $assets = hrm_backend_pim_employee_assets($employee, $account);
//        $employee_account = hrm_backend_pim_employee_account($employee, $account);
//        $variables['actions'] = array();

        return theme('page_wrapper_pim_employee_details', $variables);
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

function hrm_backend_pim_employee_details_get_page_title($empid, $employee = array()) {
  if ($empid && is_numeric($empid) && empty($employee)) {
    $employee = hrm_basic_employee_get_data(array(array('field' => 'empid', 'value' => $empid)), TRUE);
  }

  $title = '';
  if (!empty($employee)) {
    $job_title = (isset($employee['job']['job_title']) && $employee['job']['job_title'] ? hrm_basic_job_title_names(array(array('field' => 'id', 'value' => $employee['job']['job_title'])), TRUE) : '');
    $title = '<div class="pim-employee-title clearfix">';

    $title .= '<div class="gallery custom-gallery" data-toggle="lightbox-gallery">
        <a href="' . $employee['photo_url'] . '" class="gallery-link" title="' . $employee['name'] . '">
          ' . theme('image_style', array('style_name' => HRM_IMAGE_STYLE_150_150, 'path' => $employee['photo_uri'], 'attributes' => array('class' => array('employee-photo', 'img-circle', 'img-50')))) . '
        </a>
    </div>';



    $title .= '<div class="employee-desc"><span class="employee-desc-name">' . $employee['name'] . '</span><br /><span class="employee-desc-title">' . $job_title . '</span></div>';
    $title .= '</div>';
  }

  return $title;
}

//COMPLETED - Othman
function hrm_backend_pim_employee_permission($empid, $action, $account = array(), $data = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }
  $employee = (isset($data['employee']) && !empty($data['employee']) ? $data['employee'] : hrm_basic_employee_get_data(array(array('field' => 'empid', 'value' => $empid)), TRUE));
  return TRUE;
  $permissions = (isset($data['permissions']) && !empty($data['permissions']) ? $data['permissions'] : hrm_basic_role_permissions($account['role']));

  if (empty($employee) || !isset($account['oid']) || !$account['oid'] || $account['oid'] != $employee['oid']) {
    return FALSE;
  }
  switch ($action) {
    case HRM_PERM_PIM_EMP_ACCESS_EMPLOYEE_DETAILS:
      if (isset($permissions[HRM_PERM_PIM_EMP_ACCESS_EMPLOYEE_DETAILS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_EDIT_EMPLOYEE:
      if (isset($permissions[HRM_PERM_PIM_EMP_EDIT_EMPLOYEE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_TERMINATE_EMPLOYEE:
      if (isset($permissions[HRM_PERM_PIM_EMP_TERMINATE_EMPLOYEE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_ACTIVATE_EMPLOYEE:
      if (isset($permissions[HRM_PERM_PIM_EMP_ACTIVATE_EMPLOYEE]) && $employee['status'] == HRM_STATUS_DEACTIVETED) {
        return TRUE;
      }
      break;

    //Job
    case HRM_PERM_PIM_EMP_JOB_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_JOB_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_JOB_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_JOB_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Salary
    case HRM_PERM_PIM_EMP_SALARY_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_SALARY_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_SALARY_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_SALARY_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Contact Details
    case HRM_PERM_PIM_EMP_CONTACT_DETAILS_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_CONTACT_DETAILS_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_CONTACT_DETAILS_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_CONTACT_DETAILS_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Emergency Contacts
    case HRM_PERM_PIM_EMP_EMERGENCY_CONTACTS_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_EMERGENCY_CONTACTS_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_EMERGENCY_CONTACTS_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_EMERGENCY_CONTACTS_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Report To
    case HRM_PERM_PIM_EMP_REPORT_TO_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_REPORT_TO_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_REPORT_TO_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_REPORT_TO_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Qualifications
    case HRM_PERM_PIM_EMP_QUALIFICATIONS_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_QUALIFICATIONS_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_QUALIFICATIONS_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_QUALIFICATIONS_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Identity documents
    case HRM_PERM_PIM_EMP_IDENTITY_DOCUMENTS_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_IDENTITY_DOCUMENTS_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_IDENTITY_DOCUMENTS_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_IDENTITY_DOCUMENTS_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Documents
    case HRM_PERM_PIM_EMP_DOCUMENTS_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_DOCUMENTS_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_DOCUMENTS_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_DOCUMENTS_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Account
    case HRM_PERM_PIM_EMP_ACCOUNT_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_ACCOUNT_ACCESS])) {
        return TRUE;
      }
      break;
    case HRM_PERM_PIM_EMP_ACCOUNT_MANAGE:
      if (isset($permissions[HRM_PERM_PIM_EMP_ACCOUNT_MANAGE]) && $employee['status'] == HRM_STATUS_ACTIVE) {
        return TRUE;
      }
      break;

    //Attendance
    case HRM_PERM_PIM_EMP_ATTENDANCE_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_ATTENDANCE_ACCESS])) {
        return TRUE;
      }
      break;
    //Leaves
    case HRM_PERM_PIM_EMP_LEAVES_ACCESS:
      if (isset($permissions[HRM_PERM_PIM_EMP_LEAVES_ACCESS])) {
        return TRUE;
      }
      break;
    //Assets
    case HRM_PERM_PIM_EMP_ACCESS_ASSETS:
      if (isset($permissions[HRM_PERM_PIM_EMP_ACCESS_ASSETS])) {
        return TRUE;
      }
      break;
  }
}

/* -----------------------PIM----------------------- */

function hrm_backend_pim_employee_add_page() {

  $account = hrm_basic_account_get_info();
  drupal_set_title('Add Employee');

//  if (!user_access(HRM_PERM_PIM_EMP_ADD_EMPLOYEE) || !isset($account['oid']) || !$account['oid']) {
//    drupal_access_denied();
//    exit();
//  }
//  print("<pre>");
//print_r($account);
//exit();
  $employee_form = drupal_get_form('hrm_backend_pim_employee_add_form', $account);
//print("<pre>");
//print_r($account);
//exit();
  $variables = array(
    'entity' => HRM_ENTITY_EMPLOYEE,
    'title' => 'Add Employee',
    'form' => $employee_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_pim_employee_add_form($form, &$form_state, $account) {
//  $oid = $account['oid'];
//

  $form['fname'] = array(
    '#title' => 'First Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
  );

  $form['mname'] = array(
    '#title' => 'Middle Name',
    '#type' => 'textfield',
    '#required' => FALSE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
  );

  $form['lname'] = array(
    '#title' => 'Last Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
  );
  $oid = $account['oid'];
  $emp_no = variable_get(HRM_VAR_ORG_EMPLOYEE_ID_SERIAL . $oid, '0001');

  if ($emp_no == '0000') {
    $emp_no = '0001';
  }

  $form['emp_no'] = array(
    '#title' => t('Employee ID'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#disabled' => FALSE,
    '#attributes' => array('class' => array()),
    '#default_value' => $emp_no,);

  $form['email'] = array(
    '#title' => t('Email'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
  );

  $role_names = hrm_basic_role_names();

  $form['role'] = array(
    '#title' => 'Role',
    '#type' => 'select',
    '#options' => $role_names,
    '#required' => TRUE,
//    '#default_value' => (isset($emp['role']) ? $role_names[$user['role']] : ''),
    '#empty_option' => '',
    '#attributes' => array('class' => array('select-chosen')),
  );


  $form['photo'] = array(
    '#title' => 'Photo',
    '#type' => 'managed_file',
//    '#required' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_PICTURE)),
    '#upload_location' => HRM_DIR_TMP,
  );
  $date_picker_format = 'dd-mm-yyyy';

  $form['join_date'] = array(
    '#title' => 'Join date',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => $date_picker_format),
  );

  $org_filters = array(array('field' => 'oid', 'value' => $account['oid']));
  $emp_filters = array(
    array('field' => 'oid', 'value' => $account['oid']),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE),
  );

  $form['job_title'] = array(
    '#title' => 'Job Title',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => hrm_basic_job_title_names($org_filters),
//    '#default_value' => (isset($em['job_title']) ? $user['role'] : ''),
    '#empty_option' => '',
    '#attributes' => array('class' => array('select-chosen')),
    '#states' => array(
      'visible' => array(
        array(':input[name="role"]' => array(array('value' => HRM_ROLE_EMPLOYEE)),),
      ),
//      'required' => array(
//        ':input[name="accrue"]' => array('checked' => TRUE),
//        array(':input[name="accrue_frequency"]' => array(array('value' => HRM_LEAVE_ACCRUE_FREQUENCY_MONTHLY),
//            array('value' => HRM_LEAVE_ACCRUE_FREQUENCY_YEARLY)),),
//      ),
    ),
  );

  $form['supervisor'] = array(
    '#title' => 'Supervisor',
    '#type' => 'select',
    '#options' => hrm_basic_employee_names($emp_filters),
    '#required' => TRUE,
//    '#default_value' => (isset($user['role']) ? $user['role'] : ''),
    '#empty_option' => '',
    '#attributes' => array('class' => array('select-chosen')),
    '#states' => array(
      'invisible' => array(
        array(':input[name="role"]' => array(array('value' => HRM_ROLE_ADMIN)),),
      ),
    ),
  );


  $gender_options = hrm_basic_lov_get_options_list(HRM_LOV_GENDER);
  $martial_status_terms = hrm_basic_lov_get_terms(HRM_LOV_MARITAL_STATUS);
  $nationality_terms = hrm_basic_lov_get_terms(HRM_LOV_COUNTRY);


  $form['nationality'] = array(
    '#title' => 'Nationality',
    '#type' => 'select',
    '#options' => $nationality_terms,
    '#required' => TRUE,
    '#attributes' => array('class' => array('select-chosen')),
//    '#default_value' => (isset($employee['nationality']) ? $employee['nationality'] : ''),
  );

  $form['marital_status'] = array(
    '#title' => 'Marital status',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $martial_status_terms,
    '#attributes' => array('class' => array('select-chosen')),
//    '#default_value' => (isset($employee['marital_status']) ? $employee['marital_status'] : ''),
  );
  $form['gender'] = array(
    '#title' => 'Gender',
    '#type' => 'radios',
    '#required' => TRUE,
    '#attributes' => array('class' => array('inline-radios')),
    '#options' => $gender_options,
  );

  $form['phone'] = array(
    '#title' => 'Home phone',
    '#type' => 'textfield',
    '#required' => FALSE,
    '#maxlength' => 255,
//    '#default_value' => (isset($contact['phone']) ? $contact['phone'] : ''),
  );




  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Add Employee',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions form-actions-btn-primary">',
    '#suffix' => '</div>',
  );

//  $form['#attributes']['class'][] = 'form-bordered';
//  $form['#attributes']['class'][] = 'pim-employee-form';


  return $form;
}

function hrm_backend_pim_employee_add_form_validate(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $oid = $account['oid'];

  $email = $form_state['values']['email'];
  $email_exist_in_emp = db_query('SELECT COUNT(*) FROM {hrm_emp} WHERE oid = :oid AND email = :email', array(':oid' => $oid, ':email' => $email))->fetchField(0);

  if ($email_exist_in_emp) {
    form_set_error('email', t('This email already exists.'));
  }
  $email_exist_in_user = db_query('SELECT COUNT(*) FROM {users} WHERE name = :email', array(':email' => $email))->fetchField(0);

  if ($email_exist_in_user) {
    form_set_error('email', t('This email already exists.'));
  }

  $emp_no = $form_state['values']['emp_no'];
  $emp_no_exist = db_query('SELECT COUNT(*) FROM {hrm_emp} WHERE oid = :oid AND emp_no = :emp_no', array(':oid' => $oid, ':emp_no' => $emp_no))->fetchField(0);
  if ($emp_no_exist) {
    form_set_error('emp_no', t('Employee ID already exists.'));
  }

  if (isset($form_state['values']['phone']) && !is_numeric($form_state['values']['phone'])) {
    form_set_error('phone', t('Invalid phone number.'));
  }
}

function hrm_backend_pim_employee_add_form_submit(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $oid = $account['oid'];

  $emp_no = variable_get(HRM_VAR_ORG_EMPLOYEE_ID_SERIAL . $oid, '0001');
  if ($emp_no == '0000') {
    $emp_no = '0001';
  }

  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
  $password = hrm_basic_text_generate_random_text(6);
  $encrypted_password = _password_crypt('sha512', $password, HRM_KEY_SHA512_SALT);
  $role_name = hrm_basic_role_names($form_state['values']['role'], TRUE);

  $roles = array(
    HRM_ROLE_AUTHENTICATED => 'authenticated user',
    $form_state['values']['role'] => $role_name
  );

  $job_title = 0;
  if ($form_state['values']['job_title']) {
    $job_title = $form_state['values']['job_title'];
  }

  //CHECK AGAIN
  //hrm_emails_reports_pim_montly_contact_list(array($oid), array($empid));
  // add employee to users tables
  if (TRUE) {
    $new_user_fields = array(
      'name' => $form_state['values']['email'],
      'pass' => $encrypted_password,
      'mail' => $form_state['values']['email'],
      'status' => 1,
      'init' => $form_state['values']['email'],
      'roles' => $roles,
//      'photo' => $form_state['values']['email'],
      'field_user_first_name' => $form_state['values']['fname'],
      'field_user_organization' => $account['oid'],
    );
    $new_user = user_save(NULL, $new_user_fields);
    $data = get_object_vars($new_user);

    $uid = $data['uid'];
  }

  $supervisor_id = 0;
  if ($form_state['values']['supervisor']) {
    $supervisor_id = $form_state['values']['supervisor'];
  }
  $empid = $data['uid'];
  db_insert('hrm_emp')
    ->fields(array(
      'empid' => $uid,
      'user_id' => $uid,
      'oid' => $oid,
      'emp_no' => $form_state['values']['emp_no'],
      'fname' => trim($form_state['values']['fname']),
      'mname' => trim($form_state['values']['mname']),
      'lname' => trim($form_state['values']['lname']),
      'role' => $form_state['values']['role'],
      'email' => $form_state['values']['email'],
      'photo' => $form_state['values']['photo'],
      'username' => $form_state['values']['email'],
      'supervisor_id' => $supervisor_id,
      'job_title' => $job_title,
      'join_date' => strtotime($form_state['values']['join_date']),
      'password' => $encrypted_password,
      'logged_in' => 0,
      'gender' => $form_state['values']['gender'],
      'marital_status' => $form_state['values']['marital_status'],
      'nationality' => $form_state['values']['nationality'],
      'birthday' => 0,
      'phone' => $form_state['values']['phone'],
      'status' => HRM_STATUS_ACTIVE,
      'uid' => $account['uid'],
      'created' => time(),
      'changed' => time(),
    ))
    ->execute();

//  $token = hrm_basic_employee_account_generate_token($empid, $form_state['values']['email']);
//  db_update('hrm_emp')
//    ->fields(array(
//      'token' => $token,
//    ))
//    ->condition('empid', $empid, '=')
//    ->execute();


  if ($form_state['values']['photo']) {
    $photo_file = file_load($form_state['values']['photo']);
    $dest_location = strtr(HRM_DIR_ORG_EMPLOYEES, array('%oid' => $oid, '%empid' => $empid)) . '/photo';
    file_prepare_directory($dest_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

    file_move($photo_file, $dest_location, FILE_EXISTS_RENAME);
    hrm_basic_file_save_file_data($form_state['values']['photo'], HRM_MODULE_BACKEND, 'employee_picture', $empid);
  }

  $employee = array(
    'emp_id' => $empid,
    'oid' => $oid,
    'emp_no' => $form_state['values']['emp_no'],
    'name' => trim($form_state['values']['fname']) . ' ' . trim($form_state['values']['lname']),
    'email' => $form_state['values']['email'],
  );

  $emp_no += 1;
  variable_set(HRM_VAR_ORG_EMPLOYEE_ID_SERIAL . $oid, str_pad($emp_no, 4, "0", STR_PAD_LEFT));

//  hrm_emails_notification_new_employee($employee, $password);

  drupal_set_message('Successfully Saved');
  drupal_goto('pim/employee/details/' . $empid);
}

/*
 * Personal Details
 */

//COMPLETED - Othman
function hrm_backend_pim_employee_display_details($employee, $account, $edit_permission) {
  $fields = array();
  //Personal details content
  if (TRUE) {
    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'First Name', 'label_class' => array(),
      'value' => (isset($employee['fname']) ? $employee['fname'] : ''), 'value_class' => array());

    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Middle Name', 'label_class' => array(),
      'value' => (isset($employee['mname']) ? $employee['mname'] : ''), 'value_class' => array());

    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Last Name', 'label_class' => array(),
      'value' => (isset($employee['lname']) ? $employee['lname'] : ''), 'value_class' => array());

    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Employee ID', 'label_class' => array(),
      'value' => (isset($employee['emp_no']) ? $employee['emp_no'] : ''), 'value_class' => array());

    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Email', 'label_class' => array(),
      'value' => (isset($employee['email']) ? $employee['email'] : ''), 'value_class' => array());

    $gender = (!empty($employee['gender']) && $employee['gender'] ? hrm_basic_lov_get_term_field_data(HRM_LOV_GENDER, $employee['gender']) : '');
    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Gender', 'label_class' => array(),
      'value' => $gender, 'value_class' => array());

    $join_date = '';
    if ($employee['join_date']) {
      $join_date = date(HRM_DATE_SIMPLE, $employee['join_date']);
    }
    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Join Date', 'label_class' => array(),
      'value' => (!empty($employee['join_date']) ? $join_date : ''), 'value_class' => array());


    $marital_status = (!empty($employee['marital_status']) && $employee['marital_status'] ? hrm_basic_lov_get_term_field_data(HRM_LOV_MARITAL_STATUS, $employee['marital_status']) : '');
    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Marital Status', 'label_class' => array(),
      'value' => $marital_status, 'value_class' => array());

    $nationality = (!empty($employee['nationality']) && $employee['nationality'] ? hrm_basic_lov_get_term_field_data(HRM_LOV_COUNTRY, $employee['nationality']) : '');
    $fields[] = array('type' => 'row',
      'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
      'label' => 'Nationality', 'label_class' => array(),
      'value' => $nationality, 'value_class' => array());
  }
  $content = theme('section_entity_details', array('fields' => $fields));
  $role_permissions = hrm_basic_role_permissions($account['role']);
  $permission_data = array('employee' => $employee, 'permissions' => $role_permissions);


  $buttons = array();
  if ($edit_permission) {
    $buttons[] = l(HRM_FA_PENCIL . 'Update Personal Details', 'pim/employee/edit/' . $employee['empid'], array('attributes' => array('class' => array('btn', 'btn-primary')), 'html' => TRUE));
  }

  $terminate_employee = hrm_backend_pim_employee_permission($employee['empid'], HRM_PERM_PIM_EMP_TERMINATE_EMPLOYEE, $account, $permission_data);
  $activate_employee = hrm_backend_pim_employee_permission($employee['empid'], HRM_PERM_PIM_EMP_ACTIVATE_EMPLOYEE, $account, $permission_data);

  if ($terminate_employee) {
    $buttons[] = l(HRM_FA_CLOSE . ' Terminate Employee', 'hrm-modal', array('query' => array('action' => HRM_MODAL_PIM_EMP_TERMINATE_EMPLOYEE, 'empid' => $employee['empid']), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-danger')), 'html' => TRUE));
  }

  if ($activate_employee) {
    $buttons[] = l(HRM_FA_CHECK . ' Activate Employee', 'hrm-modal', array('query' => array('action' => HRM_MODAL_PIM_EMP_ACTIVATE_EMPLOYEE, 'empid' => $employee['empid']), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-success')), 'html' => TRUE));
  }

  return array('content' => $content, 'buttons' => $buttons);
}

function hrm_backend_pim_employee_edit_page($empid) {
  $account = hrm_basic_account_get_info();
  if ($empid && is_numeric($empid)) {
    $employee = hrm_basic_employee_get_data(array(array('field' => 'empid', 'value' => $empid)), TRUE);
    if (!empty($employee)) {
      $title = 'Update ' . $employee['name'] . ' Details';
      drupal_set_title($title);

      $permission_data = array('employee' => $employee);
      $edit_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_EDIT_EMPLOYEE, $account, $permission_data);
      if (!$edit_permission) {
        drupal_access_denied();
        exit();
      }

      $edit_form = drupal_get_form('hrm_backend_pim_employee_edit_form', $employee);
      $variables = array(
        'entity' => HRM_ENTITY_EMPLOYEE,
        'title' => $title,
        'form' => $edit_form,
      );
      return theme('page_wrapper_entity_form', $variables);
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

function hrm_backend_pim_employee_edit_form($form, &$form_state, $employee) {
  $gender_options = hrm_basic_lov_get_options_list(HRM_LOV_GENDER);

  $martial_status_terms = hrm_basic_lov_get_terms(HRM_LOV_MARITAL_STATUS);
  $nationality_terms = hrm_basic_lov_get_terms(HRM_LOV_COUNTRY);

  $form['fname'] = array(
    '#title' => 'First Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => (isset($employee['fname']) ? $employee['fname'] : ''),
  );
  $form['mname'] = array(
    '#title' => 'Middle Name',
    '#type' => 'textfield',
    '#required' => FALSE,
    '#maxlength' => 255,
    '#default_value' => (isset($employee['mname']) ? $employee['mname'] : ''),
  );
  $form['lname'] = array(
    '#title' => 'Last Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => (isset($employee['lname']) ? $employee['lname'] : ''),
  );



  $form['emp_no'] = array(
    '#title' => 'Employee ID',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => (isset($employee['emp_no']) ? $employee['emp_no'] : ''),
  );
  $form['email'] = array(
    '#title' => 'Email',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => (isset($employee['email']) ? $employee['email'] : ''),
  );


  // backhere
  $org_filters = array(array('field' => 'oid', 'value' => $account['oid']));
  $emp_filters = array(
    array('field' => 'oid', 'value' => $account['oid']),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE),
  );

  $form['job_title'] = array(
    '#title' => 'Job Title',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => hrm_basic_job_title_names($org_filters),
    '#default_value' => (isset($employee['job_title']) ? $employee['job_title'] : ''),
    '#empty_option' => '',
    '#attributes' => array('class' => array('select-chosen')),
  );

  $form['supervisor'] = array(
    '#title' => 'Supervisor',
    '#type' => 'select',
    '#options' => hrm_basic_employee_names($emp_filters),
    '#required' => TRUE,
    '#default_value' => (isset($employee['supervisor_id']) ? $employee['supervisor_id'] : ''),
    '#empty_option' => '',
    '#attributes' => array('class' => array('select-chosen')),
  );



  $dest_location = strtr(HRM_DIR_ORG_EMPLOYEES, array('%oid' => $employee['oid'], '%empid' => $employee['empid'])) . '/photo';
  file_prepare_directory($dest_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['photo'] = array(
    '#title' => 'Photo',
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#attributes' => array('class' => array()),
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_PICTURE)),
    '#upload_location' => $dest_location,
    '#default_value' => (isset($employee['photo']) && file_exists($employee['photo_uri']) ? $employee['photo'] : ''),
  );

  $date_picker_format = 'dd-mm-yyyy';

  $form['join_date'] = array(
    '#title' => 'Join date',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => (isset($employee['join_date']) ? $employee['join_date'] : ''),
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => $date_picker_format),
  );


  $form['nationality'] = array(
    '#title' => 'Nationality',
    '#type' => 'select',
    '#options' => $nationality_terms,
    '#required' => TRUE,
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($employee['nationality']) ? $employee['nationality'] : ''),
  );
  $form['marital_status'] = array(
    '#title' => 'Marital status',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $martial_status_terms,
    '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($employee['marital_status']) ? $employee['marital_status'] : ''),
  );
  $form['gender'] = array(
    '#title' => 'Gender',
    '#type' => 'radios',
    '#required' => TRUE,
    '#attributes' => array('class' => array('inline-radios')),
    '#options' => $gender_options,
  );
  if (isset($employee['gender']) && $employee['gender']) {
    $form['gender']['#default_value'] = $employee['gender'];
  }

  $birthday = '';
  if ($employee['birthday']) {
    $birthday = date(HRM_DATE_SHORT, $employee['birthday']);
  }
  $form['birthday'] = array(
    '#title' => 'Date of Birth',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#attributes' => array('class' => array('custom-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => $birthday,
  );

  /*
    if (isset($form_state['values']['country'])) {
    $cid = $form_state['values']['country'];
    if ($cid == HRM_STATUS_GLOBAL) {
    $locations = hrm_basic_location_names(array(array('field' => 'oid', 'value' => $employee['oid'])));
    }
    else {
    $locations = hrm_basic_location_names(array(array('field' => 'oid', 'value' => $employee['oid']), array('field' => 'cid', 'value' => $cid, 'operator' => '=')));
    }
    $form['location'] = array(
    '#title' => 'Location',
    '#type' => 'select',
    '#options' => $locations,
    '#required' => FALSE,
    '#attributes' => array('class' => array('select-chosen')),
    '#prefix' => '<div id="branch-location">',
    '#suffix' => '</div>',
    '#default_value' => (isset($employee['location']) ? $employee['location'] : ''),
    );
    }
   */
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Changes',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'employee-edit-form';
  $form['#prefix'] = '<div id="branch-location">';
  $form['#suffix'] = '</div>';

  return $form;
}

function hrm_backend_pim_employee_edit_form_validate(&$form, &$form_state) {
  $employee = $form_state['build_info']['args'][0];
  $empid = $employee['empid'];
  $oid = $employee['oid'];

  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', 'Invalid email address');
  }
  else {
    $email_exist = db_query('SELECT COUNT(*) FROM {hrm_emp} WHERE oid = :oid AND empid <> :empid AND email = :email', array(':empid' => $empid, ':oid' => $oid, ':email' => $form_state['values']['email']))->fetchField(0);
    if ($email_exist) {
      form_set_error('email', 'This email already exists***');
    }
  }

  $emp_no_exist = db_query('SELECT COUNT(*) FROM {hrm_emp} WHERE oid = :oid AND empid <> :empid AND emp_no = :emp_no', array(':empid' => $empid, ':oid' => $oid, ':emp_no' => $form_state['values']['emp_no']))->fetchField(0);
  if ($emp_no_exist) {
    form_set_error('emp_no', t('Employee ID already exists.'));
  }
}

function hrm_backend_pim_employee_edit_form_submit(&$form, &$form_state) {
  $employee = $form_state['build_info']['args'][0];

  $birthday = 0;
  if (isset($form_state['values']['birthday']) && !empty($form_state['values']['birthday'])) {
    $birthday = strtotime(str_replace('/', '-', $form_state['values']['birthday']));
  }

  $updated_fields = array(
    'emp_no' => $form_state['values']['emp_no'],
    'fname' => $form_state['values']['fname'],
    'mname' => $form_state['values']['mname'],
    'lname' => $form_state['values']['lname'],
    'email' => $form_state['values']['email'],
    'photo' => $form_state['values']['photo'],
    'gender' => $form_state['values']['gender'],
    'marital_status' => $form_state['values']['marital_status'],
    'nationality' => $form_state['values']['nationality'],
    'join_date' => strtotime($form_state['values']['join_date']),
    'birthday' => $birthday,
    'changed' => time(),
  );

  db_update('hrm_emp')
    ->fields($updated_fields)
    ->condition('empid', $employee['empid'], '=')
    ->execute();

  if ($form_state['values']['photo'] != $employee['photo']) {
    hrm_basic_file_save_file_data($form_state['values']['photo'], HRM_MODULE_BACKEND, 'employee_picture', $employee['empid']);
    if ($employee['photo']) {
      hrm_basic_file_delete_file_data($employee['photo'], HRM_MODULE_BACKEND);
    }
  }

  drupal_set_message('Successfully Saved');
  drupal_goto('pim/employee/details/' . $employee['empid']);
}

/*
 * Job JOB
 */



/*
 * Contact Detail
 */

function hrm_backend_pim_employee_account($employee, $account) {
  $fields = array();


  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Username', 'value_class' => array(),
    'value' => $employee['username'], 'value_class' => array());

  $logged_in = 0;
  if (isset($employee['logged_in']) && $employee['logged_in']) {
    $logged_in = date(HRM_DATE_SHORT_TIME_12, $employee['logged_in']);
  }

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Logged in', 'value_class' => array(),
    'value' => (!empty($logged_in) ? HRM_FLAG_TEXT_YES : HRM_FLAG_TEXT_NO), 'value_class' => array());

  if ($logged_in) {
    $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
      'label' => 'Login time', 'value_class' => array(),
      'value' => $logged_in, 'value_class' => array());
  }

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Device', 'value_class' => array(),
    'value' => isset($employee['device_type']) ? $employee['device_type'] : '', 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Device ID', 'value_class' => array(),
    'value' => isset($employee['device_id']) ? $employee['device_id'] : '', 'value_class' => array());

  $details = theme('section_entity_details', array('fields' => $fields));

  return array('content' => $details);
}

function hrm_backend_pim_employee_account_logout_form($form, &$form_state, $empid) {
  drupal_set_title('LOGOUT ACCOUNT');

  $employee = hrm_basic_employee_get_data(array(array('field' => 'empid', 'value' => $empid)), TRUE, TRUE);
  $account = hrm_basic_account_get_info();

  $form_state['build_info']['args'][1] = $account;
  $form_state['build_info']['args'][2] = $employee;

  $manage_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_ACCOUNT_MANAGE, $account, array('employee' => $employee));
  if (!$manage_permission) {
    hrm_backend_modal_access_denied();
  }

  $form['description'] = array('#markup' => '<div class="form-item form-group">Are you sure want to logout this account? ***</div>');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Yes',
    '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
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
  $form['#attributes']['class'][] = 'employee-account-logout-form';
  return $form;
}

function hrm_backend_pim_employee_account_logout_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_pim_employee_account_logout_form_submit(&$form, &$form_state) {
  $empid = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];
  $employee = $form_state['build_info']['args'][2];

  $gcm_data = array(
    'key' => HRM_APP_NOTIF_KEY_ACCOUNT_LOGOUT,
    'message' => 'Login is required***',
  );
  $devices = hrm_basic_gcm_get_device_id(array(array('field' => 'empid', 'value' => $empid, 'operator' => '=')));
  hrm_services_sendGoogleCloudMessage($gcm_data, $devices);

  db_update('hrm_emp')
    ->fields(array(
      'device_id' => '',
      'device_type' => '',
      'logged_in' => 0,
    ))
    ->condition('empid', $empid, '=')
    ->execute();

  drupal_set_message($employee['name'] . ' has been logged out from the HRM App');
  $commands = array();
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $empid, 0, array('query' => array('tab' => HRM_TAB_Account)));
  print ajax_render($commands);
  exit;
}

function hrm_backend_pim_employee_account_reset_password_form($form, &$form_state, $empid) {
  drupal_set_title('RESET PASSWORD');

  $employee = hrm_basic_employee_get_data(array(array('field' => 'empid', 'value' => $empid)), TRUE, TRUE);
  $account = hrm_basic_account_get_info();

  $form_state['build_info']['args'][1] = $account;
  $form_state['build_info']['args'][2] = $employee;

  $manage_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_ACCOUNT_MANAGE, $account, array('employee' => $employee));
  if (!$manage_permission) {
    hrm_backend_modal_access_denied();
  }

  $form['description'] = array('#markup' => '<div class="form-item form-group">Are you sure want to reset the password for this account? ***</div>');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Yes',
    '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
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
  $form['#attributes']['class'][] = 'employee-account-logout-form';

  return $form;
}

function hrm_backend_pim_employee_account_reset_password_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_pim_employee_account_reset_password_form_submit(&$form, &$form_state) {
  $empid = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];
  $employee = $form_state['build_info']['args'][2];

  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
  $password = hrm_basic_text_generate_random_text(6);
  $encrypted_password = _password_crypt('sha512', $password, HRM_KEY_SHA512_SALT);

  $devices = hrm_basic_gcm_get_device_id(array(array('field' => 'empid', 'value' => $empid)));
  if (!empty($devices)) {
    $gcm_data = array(
      'key' => HRM_APP_NOTIF_KEY_ACCOUNT_LOGOUT,
      'message' => 'Login is required***',
    );
    hrm_services_sendGoogleCloudMessage($gcm_data, $devices);
  }

  db_update('hrm_emp')
    ->fields(array(
      'password' => $encrypted_password,
    ))
    ->condition('empid', $empid, '=')
    ->execute();

  drupal_set_message("must sending email with the password: " . $password);
  // hrm_emails_notification_pim_employee_reset_password($employee, $password);  

  drupal_set_message($employee['name'] . ' password has been reset');
  $commands = array();
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $empid, 0, array('query' => array('tab' => HRM_TAB_Account)));
  print ajax_render($commands);
  exit;
}

/*
 * Assets
 */

/////////////////////////////////////
// Mohamed 
// Blocked Employees List && Activate and Terminate employees

function hrm_backend_pim_employees_bloked_list_page() {
  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $branch_filters = array(array('field' => 'oid', 'value' => $account['oid']));
  $emp_filters = array(
    array('field' => 'oid', 'value' => $account['oid']),
    array('field' => 'status', 'value' => HRM_STATUS_DEACTIVETED)
  );

  $employees = hrm_basic_employee_get_data($emp_filters);

  $access_details_permission = user_access(HRM_PERM_PIM_EMP_ACCESS_EMPLOYEE_DETAILS);

  $job_titles = hrm_basic_job_title_names($branch_filters);
  $locations = hrm_basic_location_names($branch_filters);
  $employment_statuses = hrm_basic_employment_statuse_names($branch_filters);

  $rows = array();
  foreach ($employees as $id => $employee) {


    $supervisors = array();
    if (isset($employee['supervisors']) && !empty($employee['supervisors'])) {
      foreach ($employee['supervisors'] as $supervisor) {
        $supervisor_name = '';
        if (isset($supervisor['supervisor_empid']) && $supervisor['supervisor_empid']) {
          $supervisor_name = (isset($employees[$supervisor['supervisor_empid']]['name']) ? $employees[$supervisor['supervisor_empid']]['name'] : hrm_basic_employee_get_name($supervisor['supervisor_empid']));
        }
        if (!empty($supervisor_name)) {
          $supervisors[] = $supervisor_name;
        }
      }
    }
    $row_classes = array();
    $row_classes[] = ($access_details_permission ? 'table-tr-link' : '');
    $tr_data_url = ($access_details_permission ? url('pim/employee/details/' . $id) : '');
    $rows[] = array(
      'data' => array(
        array('data' => '', 'class' => array()),
        array('data' => $employee['photo_image'], 'class' => array('cell-photo', 'cell-photo-circle')),
        array('data' => ($employee['emp_no'])),
        array('data' => ($employee['name'])),
        array('data' => (isset($employee['job']['job_title']) && isset($job_titles[$employee['job']['job_title']]) ? $job_titles[$employee['job']['job_title']] : '')),
        array('data' => (isset($employee['employment_status']) && isset($employment_statuses[$employee['employment_status']]) ? $employment_statuses[$employee['employment_status']] : '')),
        array('data' => (isset($employee['location']) && isset($locations[$employee['location']]) ? $locations[$employee['location']] : '')),
        array('data' => implode(', ', $supervisors),),
      ),
      'class' => $row_classes,
      'data-url' => $tr_data_url,
    );
  }

  $header = array(
    array('data' => '#', 'class' => array('nosort', 'nosearch', 'noexport')),
    array('data' => HRM_FA_USER, 'class' => array('nosort', 'nosearch', 'noexport', 'text-center')),
    array('data' => 'ID'),
    array('data' => 'Name'),
    array('data' => 'Job Title'),
    array('data' => 'Employment Status'),
    array('data' => 'Location'),
    array('data' => 'Supervisor'),
  );

  $list = array(
    'attributes' => array('class' => array('table-hover', 'table-vcenter')),
    'rows' => $rows,
    'header' => $header,
    'sticky' => FALSE,
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all-index' : 'empty-datatable');

  $variables = array(
    'title' => '<strong>EMPLOYEE </strong> LIST',
    'list' => $list
  );

  return theme('page_wrapper_entity_list_no_header', $variables);
}

function hrm_backend_pim_employee_terminate_form($form, &$form_state, $employee, $empid, $account) {
  $permission_data = array('employee' => $employee);
  $terminate_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_TERMINATE_EMPLOYEE, $account, $permission_data);
  if (!$terminate_permission) {
    hrm_backend_modal_access_denied();
  }
  drupal_set_title('Terminate Employee');

  $form['description'] = array('#markup' => '<div class="form-item form-group">Are you sure to Terminate Employee ?***</div>');

  $form['submit_suffix'] = array('#markup' => '<div class="form-group">'); // to make line border gone

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Yes, Terminate',
    '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
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

function hrm_backend_pim_employee_terminate_form_validate($form, &$form_state) {
  $empid = $form_state['build_info']['args'][1];
  $account = $form_state['build_info']['args'][2];

  $assign_to_empid = db_select('hrm_asset_assignment', 'assignment')
      ->fields('assignment', array('oid', 'empid'))
      ->condition('oid', $account['oid'], '=')
      ->condition('empid', $empid, '=')
      ->execute()->fetch();

  if ($assign_to_empid) {
    form_set_error('', 'The Employee Cannot be Terminated now, he has asset not returned yet***');
  }
}

function hrm_backend_pim_employee_terminate_form_submit($form, &$form_state) {

  $empid = $form_state['build_info']['args'][1];

  db_update('hrm_emp')
    ->fields(array(
      'status' => HRM_STATUS_DEACTIVETED,
      'changed' => time(),
    ))
    ->condition('empid', $empid, '=')
    ->execute();


  drupal_set_message('Successfully Saved***');
  $commands = array();
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $empid);
  print ajax_render($commands);
  exit;
}

function hrm_backend_pim_employee_activate_form($form, &$form_state, $employee, $empid, $account) {
  $permission_data = array('employee' => $employee);
  $activate_permission = hrm_backend_pim_employee_permission($empid, HRM_PERM_PIM_EMP_ACTIVATE_EMPLOYEE, $account, $permission_data);
  if (!$activate_permission) {
    hrm_backend_modal_access_denied();
  }

  drupal_set_title('Activate Employee');

  $form['description'] = array('#markup' => '<div class="form-item form-group">Are you sure to Activate Employee ?***</div>');

  $form['submit_suffix'] = array('#markup' => '<div class="form-group">'); // to make line border gone

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Yes, Activate',
    '#attributes' => array('class' => array('btn-success', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
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

function hrm_backend_pim_employee_activate_form_submit($form, &$form_state) {

  $empid = $form_state['build_info']['args'][1];

  db_update('hrm_emp')
    ->fields(array(
      'status' => HRM_STATUS_ACTIVE,
      'changed' => time(),
    ))
    ->condition('empid', $empid, '=')
    ->execute();


  drupal_set_message('Successfully Saved***');
  $commands = array();
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $empid);
  print ajax_render($commands);
  exit;
}
