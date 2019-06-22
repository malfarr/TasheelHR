<?php

function hr_backend_pim_employees_list_page() {
  global $user;
  $account = hr_basic_account_get_info($user->uid);

  if (user_access(HR_PERM_PIM_ACCESS_EMPLOYEE_LIST)) {
    $job_titles = hr_basic_job_title_get_job_titles($account['organization']);
    $locations = hr_basic_location_get_locations($account['organization']);
    $gender = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_GENDER);

    $filters = array(
      array('field' => 'org_id', 'value' => $account['organization'], 'operator' => '='),
      array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '=')
    );
    $employees = hr_basic_employee_get_employees_info($filters);
    $emp_ids = array_keys($employees);
    $uncompleted_termination = array();
    if (!empty($emp_ids)) {
      $uncompleted_termination = db_select('hr_terminated_employee')
              ->fields('hr_terminated_employee', array('emp_id'))
              ->condition('emp_id', $emp_ids, 'IN')
              ->execute()->fetchCol(0);
    }

    $rows = array();
    foreach ($employees as $id => $employee) {
      $tags = '';
      if (in_array($id, $uncompleted_termination)) {
        $tags .= '<i class="fa fa-warning text-danger animated bounceIn" title="Employment termination is not completed"></i>';
      }
      $emp_gender = (isset($gender[$employee['gender']]) ? $gender[$employee['gender']] : '');
      $job_title = (isset($employee['job']['job_title']) ? $job_titles[$employee['job']['job_title']] : '');
      $location = (isset($employee['job']['location']) ? $locations[$employee['job']['location']] : '');
      $join_date = ((isset($employee['job']['join_date']) && $employee['job']['join_date']) ? date(HR_DATETIME_FORMAT_SHORT_SPACE_PRE_MONTH_NAME, $employee['job']['join_date']) : '');
      $rows[$id] = array(
        'data' => array(
          array('data' => $tags, 'class' => array('text-center')),
          array('data' => l($employee['emp_no'], 'pim/employee/details/' . $id)),
          array('data' => $employee['picture_image'], 'class' => array('text-center')),
          array('data' => l($employee['name'], 'pim/employee/details/' . $id)),
          array('data' => $emp_gender),
          array('data' => $job_title,),
          array('data' => $location,),
          array('data' => $join_date,),
        ),
      );
    }

    $header = array(
      array('data' => '<i class="fa fa-warning"></i>', 'class' => array('nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
      array('data' => 'ID'),
      array('data' => '<i class="fa fa-user-circle-o"></i>', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
      array('data' => 'Name'),
      array('data' => 'Gender'),
      array('data' => 'Job Title'),
      array('data' => 'Location'),
      array('data' => 'Join date'),
    );

    $list = array(
      'attributes' => array('class' => array('table', 'table-bordered', 'table-striped', 'table-condensed')),
      'rows' => $rows,
      'header' => $header
    );

    if (!empty($rows)) {
      $list['attributes']['class'][] = 'datatable-all';
    }

    $buttons = array();
    if (user_access(HR_PERM_PIM_ADD_EMPLOYEE)) {
      $add_link = l('<i class="fa fa-plus"></i> Add', 'pim/employee/add', array('attributes' => array('class' => array('btn', 'btn-primary'), 'data-toggle' => 'block-option', 'data-action' => 'refresh_toggle'), 'html' => TRUE));

      $buttons[] = $add_link;
    }

    return theme('page_wrapper_pim_employees', array('list' => $list, 'buttons' => $buttons));
  }
  else {
    drupal_access_denied();
    exit();
  }
}

function hr_backend_pim_employees_bloked_list_page() {
  global $user;
  $account = hr_basic_account_get_info($user->uid);

  if (user_access(HR_PERM_PIM_ACCESS_EMPLOYEE_LIST)) {
    $job_titles = hr_basic_job_title_get_job_titles($account['organization']);
    $locations = hr_basic_location_get_locations($account['organization']);
    $gender = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_GENDER);

    $filters = array(
      array('field' => 'org_id', 'value' => $account['organization'], 'operator' => '='),
      array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_INACTIVE, 'operator' => '=')
    );
    $employees = hr_basic_employee_get_employees_info($filters);

    $rows = array();
    foreach ($employees as $id => $employee) {
      $emp_gender = (isset($gender[$employee['gender']]) ? $gender[$employee['gender']] : '');
      $job_title = (isset($employee['job']['job_title']) ? $job_titles[$employee['job']['job_title']] : '');
      $location = (isset($employee['job']['location']) ? $locations[$employee['job']['location']] : '');
      $join_date = ((isset($employee['job']['join_date']) && $employee['job']['join_date']) ? date(HR_DATETIME_FORMAT_SHORT_SPACE_PRE_MONTH_NAME, $employee['job']['join_date']) : '');
      $rows[$id] = array(
        'data' => array(
          array('data' => l($employee['emp_no'], 'pim/employee/details/' . $id)),
          array('data' => $employee['picture_image'], 'class' => array('text-center')),
          array('data' => l($employee['name'], 'pim/employee/details/' . $id)),
          array('data' => $emp_gender),
          array('data' => $job_title,),
          array('data' => $location,),
          array('data' => $join_date,),
        ),
      );
    }

    $header = array(
      array('data' => 'ID'),
      array('data' => '<i class="fa fa-user-circle-o"></i>', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
      array('data' => 'Name'),
      array('data' => 'Gender'),
      array('data' => 'Job Title'),
      array('data' => 'Location'),
      array('data' => 'Join date'),
    );

    $list = array(
      'attributes' => array('class' => array('table', 'table-bordered', 'table-striped', 'table-condensed')),
      'rows' => $rows,
      'header' => $header
    );

    if (!empty($rows)) {
      $list['attributes']['class'][] = 'datatable-all-index';
    }

    return theme('page_wrapper_pim_employees', array('list' => $list, 'buttons' => array()));
  }
  else {
    drupal_access_denied();
    exit();
  }
}

function hr_backend_pim_get_employee_data($filters = array(), $sort = array(), $first_item = FALSE) {
  $query = db_select('hr_employee', 'employee')
      ->fields('employee', array('emp_id', 'org_id', 'emp_no', 'fname', 'mname', 'lname', 'email', 'picture',
    'gender', 'martial_status', 'nationality', 'birth_date', 'ssn', 'tax_no', 'join_date', 'permanency_date',
    'attendance_applicable', 'status', 'note', 'uid', 'created', 'changed'));
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

  $employees = $query->execute()->fetchAllAssoc('emp_id', PDO::FETCH_ASSOC);

  foreach ($employees as $emp_id => $employee) {
    $employees[$emp_id]['contact'] = db_select('hr_employee_contact', 'employee_contact')
            ->fields('employee_contact', array('id', 'emp_id', 'address_id', 'work_mobile', 'other_mobile', 'home_phone',
              'work_phone', 'work_email', 'other_email', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetch(PDO::FETCH_ASSOC);

    if (isset($employees[$emp_id]['contact']['address_id']) && $employees[$emp_id]['contact']['address_id']) {
      $employees[$emp_id]['contact']['address'] = db_select('hr_address', 'address')
              ->fields('address', array('street_1', 'street_2', 'city', 'region', 'postal_code', 'country'))
              ->condition('address_id', $employees[$emp_id]['contact']['address_id'], '=')
              ->execute()->fetch(PDO::FETCH_ASSOC);
    }

    $employees[$emp_id]['identity_document'] = db_select('hr_employee_identity_document', 'employee_identity_document')
            ->fields('employee_identity_document', array('id', 'org_id', 'emp_id', 'type', 'identity_no', 'identity_image', 'issue_place',
              'issue_date', 'expiry_date', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['emergency_contacts'] = db_select('hr_employee_emergency_contact', 'employee_emergency_contact')
            ->fields('employee_emergency_contact', array('id', 'emp_id', 'name', 'relationship', 'mobile', 'home_phone',
              'work_phone', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['job'] = db_select('hr_employee_job', 'employee_job')
            ->fields('employee_job', array('id', 'emp_id', 'org_id', 'join_date', 'permanency_date', 'job_title',
              'job_no', 'employment_status', 'employment_type', 'recruitment_source', 'location', 'work_shift',
              'contract_start_date', 'contract_end_date', 'contract_attachement', 'review_attachment',
              'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetch(PDO::FETCH_ASSOC);

    if (!empty($employees[$emp_id]['job'])) {
      $employees[$emp_id]['job']['projects'] = db_select('hr_employee_job_projects', 'employee_job_projects')
              ->fields('employee_job_projects', array('project'))
              ->condition('emp_id', $emp_id, '=')
              ->execute()->fetchCol(0);
    }

    $employees[$emp_id]['job_history'] = db_select('hr_employee_job_history', 'employee_job_history')
            ->fields('employee_job_history', array('id', 'emp_id', 'org_id', 'job_title', 'job_no', 'employment_status',
              'employment_type', 'recruitment_source', 'location', 'work_shift', 'contract_start_date', 'contract_end_date',
              'contract_attachement', 'review_attachment', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['salary'] = db_select('hr_employee_salary', 'employee_salary')
            ->fields('employee_salary', array('id', 'emp_id', 'org_id', 'grade', 'currency', 'regulation',
              'basic', 'earnings', 'deductions', 'addition_cost', 'total_cost', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetch(PDO::FETCH_ASSOC);

    $employees[$emp_id]['salary']['items'] = db_select('hr_employee_salary_item', 'employee_salary_item')
            ->fields('employee_salary_item', array('id', 'emp_id', 'org_id', 'salary_component',
              'salary_component_type', 'value', 'exemption', 'total_value'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('salary_component', PDO::FETCH_ASSOC);

    $employees[$emp_id]['salary_history'] = db_select('hr_employee_salary_history', 'employee_salary_history')
            ->fields('employee_salary_history', array('id', 'emp_id', 'org_id', 'formatted_data', 'raw_data',
              'uid', 'created'))
            ->condition('emp_id', $emp_id, '=')
            ->orderBy('created', 'DESC')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['salary_allocation'] = db_select('hr_employee_salary_allocation', 'employee_salary_allocation')
            ->fields('employee_salary_allocation', array('id', 'org_id', 'emp_id', 'project_id', 'percentage_value',
              'total_value'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('project_id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['supervisors'] = db_select('hr_employee_report_to', 'employee_report_to')
            ->fields('employee_report_to', array('id', 'org_id', 'emp_id', 'supervisor_id', 'reporting_method',
              'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['education'] = db_select('hr_employee_education', 'employee_education')
            ->fields('employee_education', array('id', 'emp_id', 'org_id', 'qualification_id', 'institute',
              'major', 'year', 'score', 'start_date', 'end_date', 'attachment', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['experience'] = db_select('hr_employee_experience', 'employee_experience')
            ->fields('employee_experience', array('id', 'emp_id', 'org_id', 'company', 'job_title',
              'from_date', 'to_date', 'attachment', 'comment', 'attachment', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['skills'] = db_select('hr_employee_skills', 'employee_skills')
            ->fields('employee_skills', array('id', 'emp_id', 'org_id', 'qualification_id', 'years',
              'comment', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['language'] = db_select('hr_employee_language', 'employee_language')
            ->fields('employee_language', array('id', 'emp_id', 'org_id', 'qualification_id', 'reading', 'writing', 'speaking',
              'comment', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['license'] = db_select('hr_employee_license', 'employee_license')
            ->fields('employee_license', array('id', 'emp_id', 'org_id', 'qualification_id', 'license_number',
              'issue_date', 'expiry_date', 'attachment', 'uid', 'created', 'changed'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    $employees[$emp_id]['user'] = db_select('hr_employee_user', 'employee_user')
            ->fields('employee_user', array('emp_id', 'org_id', 'email', 'username', 'password', 'second_password',
              'token', 'device_id', 'device_type', 'active', 'status', 'login'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetch(PDO::FETCH_ASSOC);

    $employees[$emp_id]['name'] = trim($employee['fname']) . ' ' . trim($employee['lname']);
    $employees[$emp_id]['picture_uri'] = hr_basic_file_get_managed_file_uri($employee['picture'], HR_DEFAULT_IMAGES_EMPLOYEE_PICTURE);
    $employees[$emp_id]['picture_url'] = hr_basic_file_get_managed_file_url($employee['picture'], HR_DEFAULT_IMAGES_EMPLOYEE_PICTURE);
    $employees[$emp_id]['picture_url_64'] = image_style_url('image_style_scale_64x64', $employees[$emp_id]['picture_uri']);
    $employees[$emp_id]['picture_image'] = theme('image_style', array('style_name' => 'image_style_scale_64x64', 'path' => $employees[$emp_id]['picture_uri'], 'attributes' => array('class' => array('img-avatar', 'img-avatar48'))));
    $employees[$emp_id]['organization'] = ((isset($employee['org_id']) && !empty($employee['org_id'])) ? hr_basic_organization_get_name($employee['org_id']) : '');
    $employees[$emp_id]['title'] = ((isset($employees[$emp_id]['job']['job_title']) && !empty($employees[$emp_id]['job']['job_title'])) ? hr_basic_job_title_get_job_titles($employee['org_id'], $employees[$emp_id]['job']['job_title']) : '');
    $employees[$emp_id]['location'] = ((isset($employees[$emp_id]['job']['location']) && !empty($employees[$emp_id]['job']['location'])) ? hr_basic_location_get_locations($employee['org_id'], $employees[$emp_id]['job']['location']) : '');
    $employees[$emp_id]['country'] = 0;
    if (isset($employees[$emp_id]['job']['location']) && $employees[$emp_id]['job']['location']) {
      $employees[$emp_id]['country'] = hr_basic_location_get_location_country($employees[$emp_id]['job']['location']);
    }
  }

  if ($first_item) {
    $employee = reset($employees);

    return $employee;
  }
  return $employees;
}

function hr_backend_pim_permission($action, $emp_id, $employee = array(), $account = array(), $data = array()) {
  global $user;
  if (empty($account)) {
    $account = hr_basic_account_get_info($user->uid);
  }
  if (empty($employee)) {
    $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
    $employee = hr_backend_pim_get_employee_data($filters, array(), FALSE);
  }

  switch ($action) {
    //PIM Employee
    case HR_PERM_PIM_ACCESS_EMPLOYEE_DETAILS:
      if (user_access(HR_PERM_PIM_ACCESS_EMPLOYEE_DETAILS) && $account['organization'] &&
          $account['organization'] == $employee['org_id']) {
        return TRUE;
      }
      break;
    case HR_PERM_PIM_TERMINATE_EMPLOYEE:
      if (user_access(HR_PERM_PIM_TERMINATE_EMPLOYEE) && $account['organization'] &&
          $account['organization'] == $employee['org_id'] &&
          $employee['status'] == HR_EMPLOYEE_STATUS_ACTIVE) {
        return TRUE;
      }
      break;
    case HR_PERM_PIM_ACTIVATE_EMPLOYEE_EMPLOYMENT:
      if (user_access(HR_PERM_PIM_ACTIVATE_EMPLOYEE_EMPLOYMENT) && $account['organization'] &&
          $account['organization'] == $employee['org_id'] &&
          $employee['status'] == HR_EMPLOYEE_STATUS_INACTIVE) {
        return TRUE;
      }
      break;
    case HR_PERM_PIM_EMPLOYEE_MANAGE_DOCUMENTS:
      if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_DOCUMENTS) &&
          $account['organization'] && $account['organization'] == $employee['org_id']) {
        return TRUE;
      }
      break;

    //PIM Employee
    case HR_PERM_PIM_EMPLOYEE_ACCESS_ACCOUNT:
      if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_ACCOUNT) && $account['organization'] &&
          $account['organization'] == $employee['org_id']) {
        return TRUE;
      }
      break;
    case HR_PERM_PIM_EMPLOYEE_MANAGE_ACCOUNT:
      if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_ACCOUNT) && $account['organization'] &&
          $account['organization'] == $employee['org_id']) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}

/*
 * Add Employee Form
 */

function hr_backend_pim_add_employee_page() {
  global $user;
  $account = hr_basic_account_get_info($user->uid);
  $org_id = $account['organization'];
  if (hr_basic_organization_employees_add_check($org_id)) {
    return theme('page_wrapper_pim_employee_add', array('account' => $account));
  }
  else {
    drupal_set_message('You have reached the maximum number of employees for your account.', 'error');
    drupal_goto('pim/employee');
  }
}

function hr_backend_pim_add_employee_form($form, &$form_state, $account) {
  $org_id = $account['organization'];

  $form['fname'] = array('#title' => t('First Name'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()));
  $form['mname'] = array('#title' => t('Middle Name'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#attributes' => array('class' => array()));
  $form['lname'] = array('#title' => t('Last Name'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()));
  $emp_no = variable_get('organization_employees_id_' . $org_id, '0001');
  if ($emp_no == '0000') {
    $emp_no = '0001';
  }
  $form['emp_no'] = array('#title' => t('Employee ID.'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => $emp_no, '#disabled' => FALSE, '#attributes' => array('class' => array()));

  $form['email'] = array('#title' => t('Email'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()));

  $upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $org_id . '/employees/picture';
  file_prepare_directory($upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['picture'] = array('#title' => t('Picture'), '#type' => 'managed_file', '#required' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array('gif png jpg jpeg'),), '#upload_location' => $upload_location);


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Employee'),
    '#attributes' => array('class' => array('btn-primary', 'btn-sm'))
  );

  $form['#attributes']['class'][] = 'employee-add-form';

  return $form;
}

function hr_backend_pim_add_employee_form_validate(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $org_id = $account['organization'];

  $email = $form_state['values']['email'];
  $email_exist = db_query('SELECT COUNT(*) FROM {hr_employee} WHERE org_id = :org_id AND email = :email', array(':org_id' => $org_id, ':email' => $email))->fetchField(0);
  if ($email_exist) {
    form_set_error('email', t('This email already exists.'));
  }

  $emp_no = $form_state['values']['emp_no'];
  $emp_no_exist = db_query('SELECT COUNT(*) FROM {hr_employee} WHERE org_id = :org_id AND emp_no = :emp_no', array(':org_id' => $org_id, ':emp_no' => $emp_no))->fetchField(0);
  if ($emp_no_exist) {
    form_set_error('emp_no', t('Employee ID already exists.'));
  }
}

function hr_backend_pim_add_employee_form_submit(&$form, &$form_state) {
  $account = $form_state['build_info']['args'][0];
  $org_id = $account['organization'];

  $emp_no = variable_get('organization_employees_id_' . $org_id, '0001');
  if ($emp_no == '0000') {
    $emp_no = '0001';
  }

  $emp_id = db_insert('hr_employee')
      ->fields(array(
        'org_id' => $org_id,
        'emp_no' => $form_state['values']['emp_no'],
        'fname' => trim($form_state['values']['fname']),
        'mname' => trim($form_state['values']['mname']),
        'lname' => trim($form_state['values']['lname']),
        'email' => $form_state['values']['email'],
        'picture' => $form_state['values']['picture'],
        'status' => HR_EMPLOYEE_STATUS_ACTIVE,
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
  hr_basic_file_save_file_data($form_state['values']['picture'], 'hr_backend', 'employee_picture', $emp_id);

  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
  $password = hr_basic_generate_random_string(6);
  $encrypted_password = _password_crypt('sha512', $password, HR_SHA512_SALT);

  $token = hr_basic_pim_employee_user_generate_token($emp_id, $form_state['values']['email']);
  db_insert('hr_employee_user')
      ->fields(array(
        'emp_id' => $emp_id,
        'org_id' => $org_id,
        'email' => $form_state['values']['email'],
        'username' => $form_state['values']['email'],
        'password' => $encrypted_password,
        'token' => $token,
        'active' => HR_EMPLOYEE_USER_STATUS_INACTIVE,
        'status' => HR_EMPLOYEE_STATUS_ACTIVE,
        'login' => 0,
      ))
      ->execute();

  $employee = array(
    'emp_id' => $emp_id,
    'org_id' => $org_id,
    'emp_no' => $form_state['values']['emp_no'],
    'name' => trim($form_state['values']['fname']) . ' ' . trim($form_state['values']['lname']),
    'email' => $form_state['values']['email'],
  );
  hr_emails_notification_new_employee($employee, $password);

  hr_emails_reports_pim_montly_contact_list(array($org_id), array($emp_id));

  $emp_no += 1;
  variable_set('organization_employees_id_' . $org_id, str_pad($emp_no, 4, "0", STR_PAD_LEFT));

  drupal_set_message('Successfully Saved');
  drupal_goto('pim/employee/details/' . $emp_id);
}

/*
 * Employee Details
 */

function hr_backend_pim_employee_details_page($emp_id) {
  global $user;
  if ($emp_id && is_numeric($emp_id)) {
    $account = hr_basic_account_get_info($user->uid);

    $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
    $employee = hr_backend_pim_get_employee_data($filters, array(), TRUE);

    if (!empty($employee)) {
      if (hr_backend_pim_permission(HR_PERM_PIM_ACCESS_EMPLOYEE_DETAILS, $emp_id, $employee, $account)) {
        drupal_set_title($employee['fname'] . ' ' . $employee['lname']);

        $variables = array();
        $sidebar_picture_block = hr_backend_pim_employee_display_sidebar_picture($employee);
        $sidebar_nav_block = hr_backend_pim_employee_display_sidebar_nav($employee);
        $variables['sidebar'] = $sidebar_picture_block . $sidebar_nav_block;
        $variables['content'] = '';
        $variables['content_title'] = '';
        $variables['blocks'] = array();

        if (arg(4) != NULL) {
          $section = arg(4);
          switch ($section) {
            case 'picture':
              if (user_access(HR_PERM_PIM_EMPLOYEE_CHANGE_PICTURE)) {
                $variables['content'] = hr_backend_pim_employee_details_content_picture($employee);
                $variables['content_title'] = 'Picture';
              }
              else {
                drupal_access_denied();
                exit();
              }
              break;
            case 'contact':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_CONTACT_DETAILS)) {
                $variables['content'] = hr_backend_pim_employee_details_content_contact($employee);
                $variables['content_title'] = 'Contact Details';
              }
              else {
                drupal_access_denied();
                exit();
              }
              break;
            case 'identity-document':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_IDENTITY_DOCUMENTS)) {
                $identity_document_content = hr_backend_pim_employee_identity_document_content($employee);
                $variables['content'] = $identity_document_content['content'];
                $variables['content_title'] = $identity_document_content['title'];
              }
              else {
                drupal_access_denied();
                exit();
              }
              break;
            case 'emergency-contacts':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_EMERGENCY_CONTACTS)) {
                $variables['content'] = hr_backend_pim_employee_details_content_emergency_contacts($employee);
                $variables['content_title'] = 'Assigned Emergency Contacts';
              }
              else {
                drupal_access_denied();
                exit();
              }
              break;
            case 'job':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_JOB)) {
                $job = hr_backend_pim_employee_details_content_job($employee);
                $variables['content'] = $job['content'];
                $variables['content_title'] = 'Job';

                $variables['blocks'][] = array(
                  'title' => 'Job History',
                  'content' => $job['history']
                );
              }
              else {
                drupal_access_denied();
                exit();
              }
              break;
            case 'salary':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_SALARY)) {
                $salary = hr_backend_pim_employee_details_content_salary($employee);
                $variables['content'] = $salary['content'];
                $variables['content_title'] = 'Salary';

                $variables['blocks'][] = array(
                  'title' => 'Salary History',
                  'content' => $salary['history'],
                );
              }
              else {
                drupal_access_denied();
                exit();
              }
              break;
            case 'report-to':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_REPORT_TO)) {
                $variables['content'] = hr_backend_pim_employee_details_content_report_to($employee);
                $variables['content_title'] = 'Assigned Supervisors';
              }
              else {
                drupal_access_denied();
                exit();
              }
              break;
            case 'qualifications':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_QUALIFICATIONS)) {
                $qualifications = hr_backend_pim_employee_details_content_qualification($employee);
                $variables['content'] = $qualifications['experience']['content'];
                $variables['content_title'] = $qualifications['experience']['title'];
                $variables['content_id'] = $qualifications['experience']['id'];

                $variables['blocks'][] = array(
                  'title' => $qualifications['education']['title'],
                  'content' => $qualifications['education']['content'],
                  'id' => $qualifications['education']['id']
                );

                $variables['blocks'][] = array(
                  'title' => $qualifications['skills']['title'],
                  'content' => $qualifications['skills']['content'],
                  'id' => $qualifications['skills']['id']
                );

                $variables['blocks'][] = array(
                  'title' => $qualifications['language']['title'],
                  'content' => $qualifications['language']['content'],
                  'id' => $qualifications['language']['id']
                );

                $variables['blocks'][] = array(
                  'title' => $qualifications['license']['title'],
                  'content' => $qualifications['license']['content'],
                  'id' => $qualifications['license']['id']
                );
              }
              else {
                drupal_access_denied();
                exit();
              }
              break;
            case 'documents':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_DOCUMENTS)) {
                $documents = hr_backend_pim_employee_details_content_documents($employee, $account);
                $variables['content'] = $documents['content'];
                $variables['content_title'] = $documents['title'];
                break;
              }
              else {
                drupal_access_denied();
                exit();
              }
            case 'assets':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_ASSETS)) {
                $assets = hr_backend_pim_employee_assigned_assets_content($employee, $account);
                $variables['content'] = $assets['content'];
                $variables['content_title'] = $assets['title'];
                $variables['id'] = $assets['id'];

                $prev_assets = hr_backend_pim_employee_prev_assigned_assets_content($employee, $account);
                $variables['blocks'][] = array(
                  'title' => $prev_assets['title'],
                  'content' => $prev_assets['content'],
                  'id' => $prev_assets['id'],
                );

                break;
              }
              else {
                drupal_access_denied();
                exit();
              }
            case 'account':
              if (user_access(HR_PERM_PIM_EMPLOYEE_ACCESS_ACCOUNT)) {
                $employee_account = hr_backend_pim_employee_account_details_content($employee, $account);

                $variables['content'] = $employee_account['content'];
                $variables['content_title'] = $employee_account['title'];
                $variables['id'] = $employee_account['id'];
                break;
              }
              else {
                drupal_access_denied();
                exit();
              }
            default:

              $variables['content'] = hr_backend_pim_employee_details_content_personal($employee);
              $variables['content_title'] = 'Personal Details';
              break;
          }
        }
        else {
          $variables['content'] = hr_backend_pim_employee_details_content_personal($employee);
          $variables['content_title'] = 'Personal Details';
        }
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

function hr_backend_pim_employee_display_sidebar_picture($employee) {
  $name = $employee['fname'] . ' ' . $employee['lname'];
  $picture_uri = HR_DEFAULT_IMAGES_EMPLOYEE_PICTURE;
  if ($employee['picture']) {
    $picture = file_load($employee['picture']);
    $picture_uri = $picture->uri;
  }

  $picture_url = file_create_url($picture_uri);
  $variables = array('emp_id' => $employee['emp_id'], 'name' => $name, 'picture_uri' => $picture_uri, 'picture_url' => $picture_url);
  $output = theme('section_pim_employee_sidebar_picture', $variables);

  return $output;
}

function hr_backend_pim_employee_display_sidebar_nav($employee) {
  $name = $employee['fname'] . ' ' . $employee['lname'];
  $variables = array('emp_id' => $employee['emp_id'], 'name' => $name);
  $output = theme('section_pim_employee_sidebar_nav', $variables);
  return $output;
}

/*
 * Personal Details
 */

/*
 * Details Contact
 */

function hr_backend_pim_employee_details_content_personal($employee) {
  $contact_form = drupal_get_form('hr_backend_pim_employee_personal_form', $employee);
  $output = drupal_render($contact_form);

  return $output;
}

function hr_backend_pim_employee_personal_form($form, &$form_state, $employee) {
  $disabled = TRUE;
  if (isset($form_state['storage']['edit'])) {
    $disabled = FALSE;
  }

  $gender_terms = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_GENDER);
  $martial_status_terms = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_MARTIAL_STATUS);
  $nationality_terms = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_COUNTRY);

  $form['wrapper_start'] = array('#markup' => '<div class="row">');
  $form['fname'] = array('#title' => t('First name'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($employee['fname']) ? $employee['fname'] : ''),
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-4">',
    '#suffix' => '</div>');
  $form['mname'] = array('#title' => t('Middle name'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($employee['mname']) ? $employee['mname'] : ''),
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-4">',
    '#suffix' => '</div>');
  $form['lname'] = array('#title' => t('Last name'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($employee['lname']) ? $employee['lname'] : ''),
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-4">',
    '#suffix' => '</div>');

  $form['emp_no'] = array('#title' => t('Employee ID.'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($employee['emp_no']) ? $employee['emp_no'] : ''),
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-6">',
    '#suffix' => '</div>');

  $form['email'] = array('#title' => t('Email'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($employee['email']) ? $employee['email'] : ''),
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-6">',
    '#suffix' => '</div>');

  $form['sep_1'] = array('#markup' => '<hr />', '#prefix' => '<div class="col-sm-12">',
    '#suffix' => '</div>');

  $gender = '';
  if ($employee['gender']) {
    $gender = $employee['gender'];
  }
  $form['gender'] = array('#title' => t('Gender'), '#type' => 'radios', '#required' => TRUE,
    '#attributes' => array('class' => array('inline-radios')),
    '#options' => $gender_terms,
    '#default_value' => $gender,
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-6">',
    '#suffix' => '</div>');

  $form['martial_status'] = array('#title' => t('Martial status'), '#type' => 'select', '#required' => TRUE,
    '#options' => $martial_status_terms,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Select martial status'),
    '#default_value' => (isset($employee['martial_status']) ? $employee['martial_status'] : ''),
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-6">',
    '#suffix' => '</div>');

  $form['nationality'] = array('#title' => t('Nationality'), '#type' => 'select', '#options' => $nationality_terms, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Select nationality'),
    '#default_value' => (isset($employee['nationality']) ? $employee['nationality'] : ''),
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-6">',
    '#suffix' => '</div>');

  $birth_date = '';
  if ($employee['birth_date']) {
    $birth_date = date(HR_DATETIME_FORMAT_SHORT_SLASH, $employee['birth_date']);
  }
  $form['birth_date'] = array('#title' => t('Date of Birth'), '#type' => 'textfield', '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => $birth_date,
    '#disabled' => $disabled,
    '#prefix' => '<div class="col-sm-6">',
    '#suffix' => '</div>'
  );
  $form['wrapper_end'] = array('#markup' => '</div>');

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_PERSONAL_DETAILS)) {
    if ($disabled) {
      $form['preview'] = array(
        '#type' => 'submit',
        '#value' => t('Edit'),
        '#submit' => array('hr_backend_pim_employee_personal_form_edit'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_personal_form_edit_callback',
          'wrapper' => 'employee-personal-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_personal_form_view'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_personal_form_view_callback',
          'wrapper' => 'employee-personal-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }

  $form['#prefix'] = '<div id="employee-personal-form-wrapper">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-personal-form';

  return $form;
}

function hr_backend_pim_employee_personal_form_validate(&$form, &$form_state) {
  $employee = $form_state['build_info']['args'][0];
  $emp_id = $employee['emp_id'];
  $org_id = $employee['org_id'];

  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Invalid email address.'));
  }
  else {
    $email = $form_state['values']['email'];
    $email_exist = db_query('SELECT COUNT(*) FROM {hr_employee} WHERE emp_id <> :emp_id AND org_id = :org_id AND email = :email', array(':emp_id' => $emp_id, ':org_id' => $org_id, ':email' => $email))->fetchField(0);
    if ($email_exist) {
      form_set_error('email', t('This email already exists.'));
    }
  }

  $emp_no = $form_state['values']['emp_no'];
  $emp_no_exist = db_query('SELECT COUNT(*) FROM {hr_employee} WHERE emp_id <> :emp_id AND org_id = :org_id AND emp_no = :emp_no', array(':emp_id' => $emp_id, ':org_id' => $org_id, ':emp_no' => $emp_no))->fetchField(0);
  if ($emp_no_exist) {
    form_set_error('emp_no', t('Employee ID already exists.'));
  }
}

function hr_backend_pim_employee_personal_form_submit(&$form, &$form_state) {
  $employee = $form_state['build_info']['args'][0];

  $birth_date = 0;
  if (isset($form_state['values']['birth_date']) && !empty($form_state['values']['birth_date'])) {
    $birth_date = strtotime(str_replace('/', '-', $form_state['values']['birth_date']));
  }

  $updated_fields = array(
    'emp_no' => $form_state['values']['emp_no'],
    'fname' => $form_state['values']['fname'],
    'mname' => $form_state['values']['mname'],
    'lname' => $form_state['values']['lname'],
    'email' => $form_state['values']['email'],
    'gender' => $form_state['values']['gender'],
    'martial_status' => $form_state['values']['martial_status'],
    'nationality' => $form_state['values']['nationality'],
    'birth_date' => $birth_date,
    'changed' => time(),
  );
  db_update('hr_employee')
      ->fields($updated_fields)
      ->condition('emp_id', $employee['emp_id'], '=')
      ->execute();

  db_update('hr_employee_user')
      ->fields(array(
        'email' => $form_state['values']['email'],
        'username' => $form_state['values']['email'],
      ))
      ->condition('emp_id', $employee['emp_id'], '=')
      ->execute();

  drupal_set_message('Successfully Saved');
  drupal_goto('pim/employee/details/' . $employee['emp_id']);
}

function hr_backend_pim_employee_personal_form_edit($form, &$form_state) {
  $form_state['storage']['edit'] = TRUE;
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_personal_form_edit_callback($form, $form_state) {
  return $form;
}

function hr_backend_pim_employee_personal_form_view($form, &$form_state) {
  unset($form_state['storage']['edit']);
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_personal_form_view_callback($form, $form_state) {
  return $form;
}

/*
 * Details Picture
 */

function hr_backend_pim_employee_details_content_picture($employee) {
  $picture_form = drupal_get_form('hr_backend_pim_employee_picture_form', $employee);
  $output = drupal_render($picture_form);

  return $output;
}

function hr_backend_pim_employee_picture_form($form, &$form_state, $employee) {
  $upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $employee['org_id'] . '/employees/picture';
  file_prepare_directory($upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['picture'] = array(
    '#title' => t('Picture'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#default_value' => $employee['picture'],
    '#upload_validators' => array('file_validate_extensions' => array('gif png jpg jpeg'),),
    '#upload_location' => $upload_location,
    '#description' => 'Accepts gif png jpg jpeg. Recommended dimensions: 200px X 200px');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
  );

  $form['#attributes']['class'][] = 'employee-picture-form';
  return $form;
}

function hr_backend_pim_employee_picture_form_validate(&$form, &$form_state) {
  
}

function hr_backend_pim_employee_picture_form_submit(&$form, &$form_state) {
  $employee = $form_state['build_info']['args'][0];
  if ($form_state['values']['picture'] != $employee['picture']) {
    db_update('hr_employee')
        ->fields(array(
          'picture' => $form_state['values']['picture'],
          'changed' => time(),
        ))
        ->condition('emp_id', $employee['emp_id'], '=')
        ->execute();

    hr_basic_file_save_file_data($form_state['values']['picture'], 'hr_backend', 'employee_picture', $employee['emp_id']);

    hr_basic_file_delete_file_data($employee['picture'], 'hr_backend');

    drupal_set_message('Successfully Saved');
    drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/picture');
  }
}

/*
 * Identity Documents
 */

function hr_backend_pim_employee_identity_document_content($employee) {
  $org_id = $employee['org_id'];
  $organization_filter = array(array('field' => 'org_id', 'value' => $org_id, 'operator' => '='));

  $identity_document_form = drupal_get_form('hr_backend_pim_employee_identity_document_content_form', $employee);

  $identity_document_types = hr_basic_config_identity_document_get_identity_documents($organization_filter);

  $rows = array();
  foreach ($employee['identity_document'] as $row) {
    $checkbox = '<label class="css-input css-checkbox css-checkbox-primary">
          <input type="checkbox" value="' . $row['id'] . '" id="identity_document_' . $row['id'] . '" name="identity_document_' . $row['id'] . '"><span></span>
        </label>';

    $file_content = '';
    if ($row['identity_image']) {
      $file = file_load($row['identity_image']);
      if ($file) {
        $file_url = file_create_url($file->uri);
        $file_content = l('Identity file', $file_url, array('attributes' => array('target' => '_blank')));

//        $file_content = '
//        <center><div class="js-gallery-advanced">
//          <div class="img-container fx-img-zoom-in">
//            <a class="img-lightbox" href="' . $image_url . '">
//              <img src="' . $image_url . '" class="img-responsive img-64"/>
//            </a>  
//          </div>
//        </div></center>';
      }
    }

    $rows[]['data'] = array(
      'checkbox' => array('data' => $checkbox, 'class' => array('text-center')),
      'type' => array('data' => $identity_document_types[$row['type']]),
      'no' => array('data' => $row['identity_no']),
      'image' => array('data' => $file_content),
      'issue_place' => array('data' => $row['issue_place']),
      'issue_date' => array('data' => ($row['issue_date'] ? date(HR_DATETIME_FORMAT_SHORT_SPACE_PRE_MONTH_NAME, $row['issue_date']) : '')),
      'expiry_date' => array('data' => ($row['expiry_date'] ? date(HR_DATETIME_FORMAT_SHORT_SPACE_PRE_MONTH_NAME, $row['expiry_date']) : '')),
    );
  }
  $header = array(
    array('data' => '', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Type'),
    array('data' => 'NO.'),
    array('data' => 'File'),
    array('data' => 'Issue place'),
    array('data' => 'Issue date'),
    array('data' => 'Expiry date'),
  );

  $list = array(
    'attributes' => array('class' => array('employee-identity-document-list', 'table', 'table-bordered', 'table-striped', 'js-table-checkable')),
    'rows' => $rows,
    'header' => $header
  );
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }

  $content = drupal_render($identity_document_form);
  $content .= theme('table', $list);

  $variables = array(
    'content' => $content,
    'title' => 'Identity Documents',
  );


  return $variables;
}

function hr_backend_pim_employee_identity_document_content_form($form, &$form_state, $employee) {
  $access = FALSE;
  if (isset($form_state['storage']['add'])) {
    $access = TRUE;
  }

  $org_id = $employee['org_id'];
  $organization_filter = array(array('field' => 'org_id', 'value' => $org_id, 'operator' => '='));

  $identity_document_types = hr_basic_config_identity_document_get_identity_documents($organization_filter);


  $form['type'] = array('#title' => t('Type'), '#type' => 'select', '#options' => $identity_document_types, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Identity document type'),
    '#access' => $access,);

  $form['identity_no'] = array('#title' => t('Identity No.'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#access' => $access,);

  $identity_document_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $employee['org_id'] . '/employees/identity_document';
  file_prepare_directory($identity_document_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $form['identity_file'] = array('#title' => t('Scaned File'), '#type' => 'managed_file',
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png txt doc xls docx xlsx pdf ppt pps'),),
    '#upload_location' => $identity_document_upload_location,
    '#access' => $access,
  );

  $form['issue_place'] = array('#title' => t('Issue place'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#access' => $access,);


  $form['issue_date'] = array('#title' => t('Issue date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#access' => $access,
  );

  $form['expiry_date'] = array('#title' => t('Expiry date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#access' => $access,
  );

  $form['delete_identity_document'] = array('#type' => 'hidden', '#default_value' => '', '#attributes' => array('id' => 'delete-identity-document-ids'));
  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_CONTACT_DETAILS)) {
    if (!$access) {
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_add'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_add_callback',
          'wrapper' => 'employee-identity-document-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#submit' => array('hr_backend_pim_employee_identity_document_content_form_submit'),
        '#limit_validation_errors' => array(),
        '#attributes' => array(
          'class' => array('btn-danger', 'delete-identity-document'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_cancel'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_cancel_callback',
          'wrapper' => 'employee-identity-document-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }

  $form['#prefix'] = '<div id="employee-identity-document-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-identity-document-form';

  return $form;
}

function hr_backend_pim_employee_identity_document_content_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    if (empty($form_state['values']['delete_identity_document'])) {
      form_set_error('delete_experience', t('At least select one identity document to be deleted.'));
    }
  }
}

function hr_backend_pim_employee_identity_document_content_form_submit(&$form, &$form_state) {
  global $user;
  $employee = $form_state['build_info']['args'][0];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $issue_date = 0;
    if (!empty($form_state['values']['issue_date'])) {
      $issue_date = strtotime(str_replace('/', '-', $form_state['values']['issue_date']));
    }

    $expiry_date = 0;
    if (!empty($form_state['values']['expiry_date'])) {
      $expiry_date = strtotime(str_replace('/', '-', $form_state['values']['expiry_date']));
    }

    $identity_file = 0;
    if ($form_state['values']['identity_file']) {
      $identity_file = $form_state['values']['identity_file'];
    }
    $identity_document_id = db_insert('hr_employee_identity_document')
            ->fields(array(
              'emp_id' => $employee['emp_id'],
              'org_id' => $employee['org_id'],
              'type' => $form_state['values']['type'],
              'identity_no' => $form_state['values']['identity_no'],
              'identity_image' => $identity_file,
              'issue_place' => $form_state['values']['issue_place'],
              'issue_date' => $issue_date,
              'expiry_date' => $expiry_date,
              'uid' => $user->uid,
              'created' => time(),
              'changed' => time(),
            ))->execute();

    if ($identity_file) {
      hr_basic_file_save_file_data($form_state['values']['identity_file'], 'hr_backend', 'employee_identity_document', $identity_document_id);
    }


    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {

    $deleted_ids = explode(',', $form_state['input']['delete_identity_document']);
    if (!empty($deleted_ids)) {
      foreach ($deleted_ids as $key => $value) {
        if (!is_numeric($value)) {
          unset($deleted_ids[$key]);
        }
      }

      if (!empty($deleted_ids)) {
        $deleted_identity_images = db_select('hr_employee_identity_document', 'hr_employee_identity_document')
                ->fields('hr_employee_identity_document', array('identity_image'))
                ->condition('id', $deleted_ids, 'IN')
                ->condition('identity_image', 0, '<>')
                ->execute()->fetchCol(0);
        foreach ($deleted_identity_images as $deleted_identity_image) {

          hr_basic_file_delete_file_data($deleted_identity_image, 'hr_backend');
        }

        db_delete('hr_employee_identity_document')
            ->condition('id', $deleted_ids, 'IN')
            ->execute();
      }

      drupal_set_message('Successfully Deleted');
    }
  }


  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/identity-document');
}

/*
 * Details Contact
 */

function hr_backend_pim_employee_details_content_contact($employee) {
  $contact_form = drupal_get_form('hr_backend_pim_employee_contact_form', $employee);
  $output = drupal_render($contact_form);

  return $output;
}

function hr_backend_pim_employee_contact_form($form, &$form_state, $employee) {
  $disabled = TRUE;
  if (isset($form_state['storage']['edit'])) {
    $disabled = FALSE;
  }

  $contact = $employee['contact'];
  $address = array();
  if (isset($contact['address'])) {
    $address = $contact['address'];
  }

  $form['street_1'] = array('#title' => t('Address street 1'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($address['street_1']) ? $address['street_1'] : ''),
    '#disabled' => $disabled);

  $form['street_2'] = array('#title' => t('Address street 2'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['street_2']) ? $address['street_2'] : ''),
    '#disabled' => $disabled);

  $form['city'] = array('#title' => t('City'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['city']) ? $address['city'] : ''),
    '#disabled' => $disabled);

  $form['region'] = array('#title' => t('Region/State'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['region']) ? $address['region'] : ''),
    '#disabled' => $disabled);

  $form['postal_code'] = array('#title' => t('Postal/Zip code'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['postal_code']) ? $address['postal_code'] : ''),
    '#disabled' => $disabled);

  $form['country'] = array('#title' => t('Country'), '#type' => 'select', '#options' => hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_COUNTRY, FALSE), '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Select country'),
    '#default_value' => (isset($address['country']) ? $address['country'] : ''),
    '#disabled' => $disabled);

  $form['sep_1'] = array('#markup' => '<hr />');

  $form['work_mobile'] = array('#title' => t('Work mobile'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($contact['work_mobile']) ? $contact['work_mobile'] : ''),
    '#disabled' => $disabled);

  $form['other_mobile'] = array('#title' => t('Other mobile'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($contact['other_mobile']) ? $contact['other_mobile'] : ''),
    '#disabled' => $disabled);

  $form['home_phone'] = array('#title' => t('Home phone'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#default_value' => (isset($contact['home_phone']) ? $contact['home_phone'] : ''),
    '#disabled' => $disabled);

  $form['work_phone'] = array('#title' => t('Work phone'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($contact['work_phone']) ? $contact['work_phone'] : ''),
    '#disabled' => $disabled);

  $form['work_email'] = array('#title' => t('Work email'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($contact['work_email']) ? $contact['work_email'] : ''),
    '#disabled' => $disabled);

  $form['other_email'] = array('#title' => t('Other email'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => (isset($contact['other_email']) ? $contact['other_email'] : ''),
    '#disabled' => $disabled);

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_CONTACT_DETAILS)) {
    if ($disabled) {
      $form['preview'] = array(
        '#type' => 'submit',
        '#value' => t('Edit'),
        '#submit' => array('hr_backend_pim_employee_contact_form_edit'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_contact_form_edit_callback',
          'wrapper' => 'employee-contact-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_contact_form_view'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_contact_form_view_callback',
          'wrapper' => 'employee-contact-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }



  $form['#prefix'] = '<div id="employee-contact-form-wrapper">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-contact-form';

  return $form;
}

function hr_backend_pim_employee_contact_form_validate(&$form, &$form_state) {
  if (!valid_email_address($form_state['values']['work_email'])) {
    form_set_error('work_email', t('Invalid work email address.'));
  }
  if (!valid_email_address($form_state['values']['other_email'])) {
    form_set_error('other_email', t('Invalid other email address.'));
  }
}

function hr_backend_pim_employee_contact_form_submit(&$form, &$form_state) {
  global $user;

  $employee = $form_state['build_info']['args'][0];

  $address_id = 0;
  if (isset($employee['contact']['address_id'])) {
    $address_id = $employee['contact']['address_id'];
  }
  $address_fields = array(
    'street_1' => $form_state['values']['street_1'],
    'street_2' => $form_state['values']['street_2'],
    'city' => $form_state['values']['city'],
    'region' => $form_state['values']['region'],
    'postal_code' => $form_state['values']['postal_code'],
    'country' => $form_state['values']['country'],
  );

  if ($address_id) {
    $address_fields['changed'] = time();
    db_update('hr_address')
        ->fields($address_fields)
        ->condition('address_id', $address_id, '=')
        ->execute();
  }
  else {
    $address_fields['entity'] = 'employee';
    $address_fields['entity_id'] = $employee['emp_id'];

    $address_fields['uid'] = $user->uid;
    $address_fields['created'] = time();
    $address_fields['changed'] = time();
    $address_id = db_insert('hr_address')->fields($address_fields)->execute();
  }

  $contact_fields = array(
    'emp_id' => $employee['emp_id'],
    'address_id' => $address_id,
    'work_mobile' => $form_state['values']['work_mobile'],
    'other_mobile' => $form_state['values']['other_mobile'],
    'home_phone' => $form_state['values']['home_phone'],
    'work_phone' => $form_state['values']['work_phone'],
    'work_email' => $form_state['values']['work_email'],
    'other_email' => $form_state['values']['other_email'],
  );

  if (isset($employee['contact']['id'])) {
    $contact_fields['changed'] = time();
    db_update('hr_employee_contact')
        ->fields($contact_fields)
        ->condition('id', $employee['contact']['id'], '=')
        ->execute();
  }
  else {
    $contact_fields['uid'] = $user->uid;
    $contact_fields['created'] = time();
    $contact_fields['changed'] = time();
    db_insert('hr_employee_contact')->fields($contact_fields)->execute();
  }

  db_update('hr_employee')
      ->fields(array(
        'email' => $form_state['values']['work_email']
      ))
      ->condition('emp_id', $employee['emp_id'], '=')
      ->execute();
  db_update('hr_employee_user')
      ->fields(array(
        'email' => $form_state['values']['work_email'],
        'username' => $form_state['values']['work_email'],
      ))
      ->condition('emp_id', $employee['emp_id'], '=')
      ->execute();

  drupal_set_message('Successfully Saved');
  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/contact');
}

function hr_backend_pim_employee_contact_form_edit($form, &$form_state) {
  $form_state['storage']['edit'] = TRUE;
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_contact_form_edit_callback($form, $form_state) {
  return $form;
}

function hr_backend_pim_employee_contact_form_view($form, &$form_state) {
  unset($form_state['storage']['edit']);
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_contact_form_view_callback($form, $form_state) {
  return $form;
}

/*
 * Details Emergency Contacts
 */

function hr_backend_pim_employee_details_content_emergency_contacts($employee) {
  $emergency_contacts_form = drupal_get_form('hr_backend_pim_employee_emergency_contact_form', $employee);
  $output = drupal_render($emergency_contacts_form);

  $rows = array();
  foreach ($employee['emergency_contacts'] as $emergency_contact) {
    $checkbox = '<label class="css-input css-checkbox css-checkbox-primary">
          <input type="checkbox" value="' . $emergency_contact['id'] . '" id="emergency_contact_' . $emergency_contact['id'] . '" name="emergency_contact_' . $emergency_contact['id'] . '"><span></span>
        </label>';
    $rows[]['data'] = array(
      'checkbox' => array('data' => $checkbox, 'class' => array('text-center')),
      'name' => array('data' => $emergency_contact['name']),
      'relationship' => array('data' => $emergency_contact['relationship']),
      'mobile' => array('data' => $emergency_contact['mobile']),
      'home_phone' => array('data' => $emergency_contact['home_phone']),
      'work_phone' => array('data' => $emergency_contact['work_phone']),
    );
  }
  $header_checkbox = '<label class="css-input css-checkbox css-checkbox-primary remove-margin-t remove-margin-b">
    <input type="checkbox" id="check-all" name="check-all"><span></span>
    </label>';
  $header = array(
    array('data' => $header_checkbox, 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Name'),
    array('data' => 'Relationship'),
    array('data' => 'Mobile'),
    array('data' => 'Home phone'),
    array('data' => 'Work phone'),
  );

  $list = array(
    'attributes' => array('class' => array('employee-emergency-contacts-list', 'table', 'table-bordered', 'table-striped', 'js-table-checkable')),
    'rows' => $rows,
    'header' => $header
  );
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }
  $output .= theme('table', $list);

  return $output;
}

function hr_backend_pim_employee_emergency_contact_form($form, &$form_state, $employee) {
  $access = FALSE;
  if (isset($form_state['storage']['add'])) {
    $access = TRUE;
  }

  $form['name'] = array('#title' => t('Name'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#access' => $access);

  $form['relationship'] = array('#title' => t('Relationship'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#access' => $access);

  $form['mobile'] = array('#title' => t('Mobile'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#access' => $access);

  $form['home_phone'] = array('#title' => t('Home phone'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#access' => $access);

  $form['work_phone'] = array('#title' => t('Work phone'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#access' => $access);

  $form['delete_emergency_contacts'] = array('#type' => 'hidden', '#default_value' => '', '#attributes' => array('id' => 'delete-emergency-contacts-ids'));

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_EMERGENCY_CONTACTS)) {
    if (!$access) {
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('hr_backend_pim_employee_emergency_contact_form_add'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_emergency_contact_form_add_callback',
          'wrapper' => 'employee-emergency-contact-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array(
          'class' => array('btn-danger', 'delete-emergency-contacts'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_emergency_contact_form_cancel'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_emergency_contact_form_cancel_callback',
          'wrapper' => 'employee-emergency-contact-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }



  $form['#prefix'] = '<div id="employee-emergency-contact-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-emergency-contacts-form';

  return $form;
}

function hr_backend_pim_employee_emergency_contact_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    if (empty($form_state['values']['mobile']) && empty($form_state['values']['home_phone']) && empty($form_state['values']['work_phone'])) {
      form_set_error('mobile', t('At least one phone number is required.'));
    }
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    if (empty($form_state['values']['delete_emergency_contacts'])) {
      form_set_error('delete_emergency_contacts', t('At least select one emergency contacts to be deleted.'));
    }
  }
}

function hr_backend_pim_employee_emergency_contact_form_submit(&$form, &$form_state) {
  global $user;
  $employee = $form_state['build_info']['args'][0];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    db_insert('hr_employee_emergency_contact')
        ->fields(array(
          'emp_id' => $employee['emp_id'],
          'name' => $form_state['values']['name'],
          'relationship' => $form_state['values']['relationship'],
          'mobile' => $form_state['values']['mobile'],
          'home_phone' => $form_state['values']['home_phone'],
          'work_phone' => $form_state['values']['work_phone'],
          'uid' => $user->uid,
          'created' => time(),
          'changed' => time(),
        ))->execute();

    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    $deleted_ids = explode(',', $form_state['values']['delete_emergency_contacts']);
    if (!empty($deleted_ids)) {
      foreach ($deleted_ids as $key => $value) {
        if (!is_numeric($value)) {
          unset($deleted_ids[$key]);
        }
      }

      if (!empty($deleted_ids)) {
        db_delete('hr_employee_emergency_contact')
            ->condition('id', $deleted_ids, 'IN')
            ->execute();
      }

      drupal_set_message('Successfully Deleted');
    }
  }


  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/emergency-contacts');
}

function hr_backend_pim_employee_emergency_contact_form_add($form, &$form_state) {
  $form_state['storage']['add'] = TRUE;
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_emergency_contact_form_add_callback($form, $form_state) {
  return $form;
}

function hr_backend_pim_employee_emergency_contact_form_cancel($form, &$form_state) {
  unset($form_state['storage']['add']);
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_emergency_contact_form_cancel_callback($form, $form_state) {
  return $form;
}

/*
 * Details JOB
 */

function hr_backend_pim_employee_details_content_job($employee) {
  $job_form = drupal_get_form('hr_backend_pim_employee_job_form', $employee);
  $output = drupal_render($job_form);

  $job_history_list = hr_backend_pim_employee_get_job_history_list($employee);
  $job_history_output = theme('table', $job_history_list);

  return array('content' => $output, 'history' => $job_history_output);
}

function hr_backend_pim_employee_get_job_history_list($employee) {
  $org_id = $employee['org_id'];
  $locations = hr_basic_location_get_locations($org_id);
  $employment_status = hr_basic_emp_status_get_emp_statuses($org_id);
  $jobs_titles = hr_basic_job_title_get_job_titles($org_id);

  $rows = array();
  foreach ($employee['job_history'] as $job) {
    $contract = file_load($job['contract_attachement']);
    $contract_output = '';
    if ($contract) {
      $contract_output = l('Contract', file_create_url($contract->uri), array('attributes' => array('target' => '_blank')));
    }

    $review = file_load($job['review_attachment']);
    $review_output = '';
    if ($review) {
      $review_output = l('Probation Review', file_create_url($review->uri), array('attributes' => array('target' => '_blank')));
    }

    $rows[] = array(
      'data' => array(
        array('data' => (isset($jobs_titles[$job['job_title']]) ? $jobs_titles[$job['job_title']] : '')),
        array('data' => $job['job_no']),
        array('data' => ($job['contract_start_date'] ? date(HR_DATETIME_FORMAT_SHORT_SPACE_PRE_MONTH_NAME, $job['contract_start_date']) : '')),
        array('data' => ($job['contract_end_date'] ? date(HR_DATETIME_FORMAT_SHORT_SPACE_PRE_MONTH_NAME, $job['contract_end_date']) : '')),
        array('data' => (isset($employment_status[$job['employment_status']]) ? $employment_status[$job['employment_status']] : '')),
        array('data' => (isset($locations[$job['location']]) ? $locations[$job['location']] : '')),
        array('data' => $contract_output),
        array('data' => $review_output),
        array('data' => date(HR_DATETIME_FORMAT_LONG_SPACE_PRE_MONTH_NAME, $job['changed'])),
      ),
      'class' => array()
    );
  }

  $header = array(
    array('data' => 'Job Title'),
    array('data' => 'Job ID'),
    array('data' => 'Contract Start'),
    array('data' => 'Contract End'),
    array('data' => 'Employment Status'),
    array('data' => 'Location'),
    array('data' => 'Contract'),
    array('data' => 'Probation Review'),
    array('data' => 'Changed'),
  );

  $list = array(
    'attributes' => array('class' => array('table', 'table-bordered', 'table-striped')),
    'rows' => $rows,
    'header' => $header
  );

  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }

  return $list;
}

//Job Form
function hr_backend_pim_employee_job_form($form, &$form_state, $employee) {
  $organization_filter = array(array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='));
  $countries = array();

  $disabled = TRUE;
  if (isset($form_state['storage']['edit'])) {
    $disabled = FALSE;
  }
  $job = $employee['job'];

  $job_options = hr_basic_job_title_get_job_titles($employee['org_id']);
  $employment_status_options = hr_basic_emp_status_get_emp_statuses($employee['org_id']);
  $employment_type_options = hr_basic_employment_type_get_employment_types($employee['org_id']);
  $recruitment_source_options = hr_basic_recruitment_source_get_recruitment_sources($employee['org_id']);

  $location_options = array();
  $locations = hr_basic_location_get_locations_info($organization_filter);
  foreach ($locations as $loc_id => $location) {
    if (!isset($countries[$location['country']])) {
      $countries[$location['country']] = hr_basic_taxonomy_term_name($location['country']);
    }
    $location_options[$countries[$location['country']]][$loc_id] = $location['name'];
  }

  $work_shifts = hr_basic_work_shift_get_work_shift($organization_filter);

  $projects = hr_basic_project_get_projects($organization_filter);

  $contract_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $employee['org_id'] . '/employees/contract';
  file_prepare_directory($contract_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $probation_review_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $employee['org_id'] . '/employees/review';
  file_prepare_directory($probation_review_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $form['commencement'] = array(
    '#type' => 'fieldset',
    '#title' => 'Employment Commencement',
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="commencement-details" class="">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('commencement-details')),
  );

  $join_date = '';
  if ($job['join_date']) {
    $join_date = date(HR_DATETIME_FORMAT_SHORT_SLASH, $job['join_date']);
  }
  $form['commencement']['join_date'] = array(
    '#title' => t('Join Date'), '#type' => 'textfield', '#empty_option' => '',
    '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => $join_date, '#disabled' => $disabled,
  );

  $permanency_date = '';
  if ($job['permanency_date']) {
    $permanency_date = date(HR_DATETIME_FORMAT_SHORT_SLASH, $job['permanency_date']);
  }
  $form['commencement']['permanency_date'] = array(
    '#title' => t('Date of Permanency'), '#type' => 'textfield', '#empty_option' => '',
    '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => $permanency_date, '#disabled' => $disabled,
  );


  $form['job'] = array(
    '#type' => 'fieldset', '#title' => 'Job Details',
    '#collapsible' => FALSE, '#tree' => TRUE,
    '#prefix' => '<div id="job-details" class="">', '#suffix' => '</div>',
    '#attributes' => array('class' => array('job-details')),
  );

  $form['job']['job_title'] = array(
    '#title' => t('Job Title'), '#type' => 'select', '#options' => $job_options, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Job title'),
    '#default_value' => (isset($job['job_title']) ? $job['job_title'] : ''),
    '#disabled' => $disabled);

  $form['job']['job_no'] = array(
    '#title' => t('Job ID'), '#type' => 'textfield', '#required' => TRUE,
    '#default_value' => (isset($job['job_no']) ? $job['job_no'] : ''), '#attributes' => array('class' => array()),
    '#disabled' => $disabled,
  );

  $form['job']['employment_status'] = array(
    '#title' => t('Employment Status'), '#type' => 'select', '#options' => $employment_status_options, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Employment status'),
    '#default_value' => (isset($job['employment_status']) ? $job['employment_status'] : ''),
    '#disabled' => $disabled);

  $form['job']['employment_type'] = array(
    '#title' => t('Employment Type'), '#type' => 'select', '#options' => $employment_type_options, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Employment type'),
    '#default_value' => (isset($job['employment_type']) ? $job['employment_type'] : ''),
    '#disabled' => $disabled);

  $form['job']['recruitment_source'] = array(
    '#title' => t('Rectuitment source'), '#type' => 'select', '#options' => $recruitment_source_options, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Recruitment source'),
    '#default_value' => (isset($job['recruitment_source']) ? $job['recruitment_source'] : ''),
    '#disabled' => $disabled);

  $form['job']['location'] = array(
    '#title' => t('Location'), '#type' => 'select', '#options' => $location_options, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Location'),
    '#default_value' => (isset($job['location']) ? $job['location'] : ''),
    '#disabled' => $disabled);

  $form['job']['work_shift'] = array(
    '#title' => t('Work shift'), '#type' => 'select', '#options' => $work_shifts, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Work shift'),
    '#default_value' => (isset($job['work_shift']) ? $job['work_shift'] : ''),
    '#disabled' => $disabled);

  $form['job']['projects'] = array(
    '#title' => t('Projects'), '#type' => 'select',
    '#options' => $projects, '#required' => FALSE, '#multiple' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Project'),
    '#default_value' => (isset($job['projects']) && !empty($job['projects']) ? $job['projects'] : ''),
    '#disabled' => $disabled);

  $form['contract'] = array(
    '#type' => 'fieldset', '#title' => 'Employment Contract',
    '#collapsible' => FALSE, '#tree' => TRUE,
    '#prefix' => '<div id="contract-details" class="">', '#suffix' => '</div>',
    '#attributes' => array('class' => array('contract-details')),
  );
  $form['contract']['new_contract'] = array(
    '#type' => 'checkbox', '#title' => t('New Contract'),
    '#prefix' => '<div class="form-item form-group">', '#suffix' => '</div>',
    '#default_value' => 0,
    '#disabled' => $disabled,
    '#access' => (!$disabled),
  );

  $contract_start_date = '';
  if ($job['contract_start_date']) {
    $contract_start_date = date(HR_DATETIME_FORMAT_SHORT_SLASH, $job['contract_start_date']);
  }
  $form['contract']['contract_start_date'] = array('#title' => t('Start date'), '#type' => 'textfield',
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => $contract_start_date,
    '#disabled' => $disabled,
  );

  $contract_end_date = '';
  if ($job['contract_end_date']) {
    $contract_end_date = date(HR_DATETIME_FORMAT_SHORT_SLASH, $job['contract_end_date']);
  }
  $form['contract']['contract_end_date'] = array('#title' => t('End Date'), '#type' => 'textfield',
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => $contract_end_date,
    '#disabled' => $disabled,
  );

  $form['contract']['contract_attachement'] = array('#title' => t('Contract'), '#type' => 'managed_file',
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png txt doc xls docx xlsx pdf ppt pps'),),
    '#upload_location' => $contract_upload_location,
    '#default_value' => $job['contract_attachement'],
    '#disabled' => $disabled,
  );
  $form['contract']['review_attachment'] = array('#title' => t('Probation performance review'), '#type' => 'managed_file',
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png txt doc xls docx xlsx pdf ppt pps'),),
    '#upload_location' => $probation_review_upload_location,
    '#default_value' => $job['review_attachment'],
    '#disabled' => $disabled,
  );

  $form['attendance_applicable'] = array(
    '#type' => 'checkbox', '#title' => t('Attendance applicable'),
    '#prefix' => '<div class="form-item form-group">', '#suffix' => '</div>',
    '#default_value' => (isset($employee['attendance_applicable']) ? $employee['attendance_applicable'] : HR_EMPLOYEE_ATTENDANCE_APPLICABLE),
    '#disabled' => $disabled,
  );

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_JOB)) {
    if ($disabled) {
      $form['preview'] = array(
        '#type' => 'submit',
        '#value' => t('Edit'),
        '#submit' => array('hr_backend_pim_employee_job_form_edit'),
        '#limit_validation_errors' => array(),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_job_form_edit_callback',
          'wrapper' => 'employee-job-form-wrapper',
          'effect' => 'fade',
        ),
        '#attributes' => array('class' => array('btn-info', 'btn-sm'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success', 'btn-sm'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_job_form_view'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_job_form_view_callback',
          'wrapper' => 'employee-job-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning', 'btn-sm'))
      );
    }

    if ($employee['status'] == HR_EMPLOYEE_STATUS_ACTIVE) {
      $termination_date = db_select('hr_terminated_employee')
              ->fields('hr_terminated_employee', array('termination_date'))
              ->condition('emp_id', $employee['emp_id'], '=')
              ->orderBy('termination_date', 'DESC')
              ->execute()->fetchField(0);
      if ($termination_date) {
        unset($form['preview']);
        $terminate_description_link = l('Terminated on: ' . date(HR_DATETIME_FORMAT_SHORT_SLASH, $termination_date) . '&nbsp&nbsp', 'hrm-modal', array('query' => array('action' => 'terminate-employment-description', 'emp_id' => $employee['emp_id']), 'attributes' => array('class' => array()), 'html' => TRUE));
        $form['terminate_description'] = array('#markup' => $terminate_description_link);

        $terminate_link = l('Complete Employment Termination', 'hrm-modal', array('query' => array('action' => 'complete-terminate-employment', 'emp_id' => $employee['emp_id']), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-danger')), 'html' => TRUE));
        $form['terinate_employment'] = array('#markup' => $terminate_link);
      }
      else {
        $terminate_link = l('Terminate Employment', 'hrm-modal', array('query' => array('action' => 'terminate-employment', 'emp_id' => $employee['emp_id']), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-danger')), 'html' => TRUE));
        $form['terinate_employment'] = array('#markup' => $terminate_link);
      }
    }
    else {
      unset($form['preview']);

      $terminate_link = l('Activate Employment', 'pim/employee/activate-employment/' . $employee['emp_id'], array('attributes' => array(
          'class' => array('btn', 'btn-success', 'btn-sm', 'push-5-r', 'confirm-delete'),
          'data-text' => 'Are you sure want to activate employment?',
          'data-title' => "Activate Employement"
        ), 'html' => TRUE));
      $form['activate_employment'] = array('#markup' => $terminate_link);

      $termination_date = db_select('hr_terminated_employee')
              ->fields('hr_terminated_employee', array('termination_date'))
              ->condition('emp_id', $employee['emp_id'], '=')
              ->orderBy('termination_date', 'DESC')
              ->execute()->fetchField(0);

      $terminate_description_link = l('Terminated on: ' . date(HR_DATETIME_FORMAT_SHORT_SLASH, $termination_date), 'hrm-modal', array('query' => array('action' => 'terminate-employment-description', 'emp_id' => $employee['emp_id']), 'attributes' => array('class' => array()), 'html' => TRUE));
      $form['terminate_description'] = array('#markup' => $terminate_description_link);
    }
  }

  $form['#prefix'] = '<div id="employee-job-form-wrapper">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-job-form';

  return $form;
}

function hr_backend_pim_employee_job_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  if ($values['contract']['new_contract']) {
    if (empty($values['contract']['contract_start_date'])) {
      form_set_error('contract][contract_start_date', 'Contract start date field id required');
    }
    if (empty($values['contract']['contract_end_date'])) {
      form_set_error('contract][contract_end_date', 'Contract end date field id required');
    }
    if (empty($values['contract']['contract_attachement'])) {
      form_set_error('contract][contract_attachement', 'Contract field id required');
    }
  }

  if (!empty($values['contract']['contract_start_date']) && !empty($values['contract']['contract_end_date'])) {
    $contract_start_time = strtotime(str_replace('/', '-', $values['contract']['contract_start_date']));
    $contract_end_time = strtotime(str_replace('/', '-', $values['contract']['contract_end_date']));

    if ($contract_start_time > $contract_end_time) {
      form_set_error('contract][contract_start_date', 'Contract start date should be less than end date');
    }
  }
}

function hr_backend_pim_employee_job_form_submit(&$form, &$form_state) {
  global $user;

  $employee = $form_state['build_info']['args'][0];
  $current_job = $employee['job'];

  $join_date = (!empty($form_state['values']['commencement']['join_date']) ? strtotime(str_replace('/', '-', $form_state['values']['commencement']['join_date']) . ' 08:00') : 0);
  $permanency_date = (!empty($form_state['values']['commencement']['permanency_date']) ? strtotime(str_replace('/', '-', $form_state['values']['commencement']['permanency_date']) . ' 08:00') : 0);

  $contract_start_date = (!empty($form_state['values']['contract']['contract_start_date']) ? strtotime(str_replace('/', '-', $form_state['values']['contract']['contract_start_date']) . ' 08:00') : 0);
  $contract_end_date = (!empty($form_state['values']['contract']['contract_end_date']) ? strtotime(str_replace('/', '-', $form_state['values']['contract']['contract_end_date']) . ' 08:00') : 0);

  if (empty($current_job)) {
    $job_fields = array(
      'emp_id' => $employee['emp_id'],
      'org_id' => $employee['org_id'],
      'join_date' => $join_date,
      'permanency_date' => $permanency_date,
      'job_title' => $form_state['values']['job']['job_title'],
      'job_no' => $form_state['values']['job']['job_no'],
      'employment_status' => $form_state['values']['job']['employment_status'],
      'employment_type' => $form_state['values']['job']['employment_type'],
      'recruitment_source' => $form_state['values']['job']['recruitment_source'],
      'location' => $form_state['values']['job']['location'],
      'work_shift' => $form_state['values']['job']['work_shift'],
      'contract_start_date' => $contract_start_date,
      'contract_end_date' => $contract_end_date,
      'contract_attachement' => $form_state['values']['contract']['contract_attachement'],
      'review_attachment' => $form_state['values']['contract']['review_attachment'],
      'uid' => $user->uid,
      'created' => time(),
      'changed' => time(),
    );
    $employee_job_id = db_insert('hr_employee_job')->fields($job_fields)->execute();

    if ($form_state['values']['contract']['contract_attachement']) {
      hr_basic_file_save_file_data($form_state['values']['contract']['contract_attachement'], 'hr_backend', 'pim_job_contract', $employee_job_id);
    }
    if ($form_state['values']['contract']['review_attachment']) {
      hr_basic_file_save_file_data($form_state['values']['contract']['review_attachment'], 'hr_backend', 'pim_job_review', $employee_job_id);
    }

    //Create leave types entitlements autamatically
    hr_basic_leave_entitlements_generate_employee_entitlemnts($employee['emp_id']);

    drupal_set_message('Successfully Saved');
  }
  else {
    $updated_job_fields = array(
      'join_date' => $join_date,
      'permanency_date' => $permanency_date,
      'job_title' => $form_state['values']['job']['job_title'],
      'job_no' => $form_state['values']['job']['job_no'],
      'employment_status' => $form_state['values']['job']['employment_status'],
      'employment_type' => $form_state['values']['job']['employment_type'],
      'recruitment_source' => $form_state['values']['job']['recruitment_source'],
      'location' => $form_state['values']['job']['location'],
      'work_shift' => $form_state['values']['job']['work_shift'],
      'contract_start_date' => $contract_start_date,
      'contract_end_date' => $contract_end_date,
      'contract_attachement' => $form_state['values']['contract']['contract_attachement'],
      'review_attachment' => $form_state['values']['contract']['review_attachment'],
      'uid' => $user->uid,
      'changed' => time(),
    );
    $emp_job_id = $current_job['id'];
    db_update('hr_employee_job')
        ->fields($updated_job_fields)
        ->condition('id', $emp_job_id, '=')
        ->execute();

    if ($form_state['values']['contract']['contract_attachement'] && $form_state['values']['contract']['contract_attachement'] != $current_job['contract_attachement']) {
      hr_basic_file_save_file_data($form_state['values']['contract']['contract_attachement'], 'hr_backend', 'pim_job_contract', $emp_job_id);
    }

    if ($form_state['values']['contract']['review_attachment'] && $form_state['values']['contract']['review_attachment'] != $current_job['review_attachment']) {
      hr_basic_file_save_file_data($form_state['values']['contract']['review_attachment'], 'hr_backend', 'pim_job_review', $emp_job_id);
    }

    if ($form_state['values']['contract']['new_contract']) {
      $job_history_fields = array(
        'emp_id' => $current_job['emp_id'],
        'org_id' => $current_job['org_id'],
        'job_title' => $current_job['job_title'],
        'job_no' => $current_job['job_no'],
        'employment_status' => $current_job['employment_status'],
        'location' => $current_job['location'],
        'work_shift' => $current_job['work_shift'],
        'contract_start_date' => $current_job['contract_start_date'],
        'contract_end_date' => $current_job['contract_end_date'],
        'contract_attachement' => $current_job['contract_attachement'],
        'review_attachment' => $current_job['review_attachment'],
        'uid' => $current_job['uid'],
        'created' => $current_job['created'],
        'changed' => time(),
      );
      $employee_job_history_id = db_insert('hr_employee_job_history')->fields($job_history_fields)->execute();
      if ($current_job['contract_attachement']) {
        hr_basic_file_save_file_data($current_job['contract_attachement'], 'hr_backend', 'pim_job_contract_history', $employee_job_history_id);
      }
      if ($current_job['review_attachment']) {
        hr_basic_file_save_file_data($current_job['review_attachment'], 'hr_backend', 'pim_job_review_history', $employee_job_history_id);
      }
    }

    drupal_set_message('Successfully Saved');
  }

  db_delete('hr_employee_job_projects')
      ->condition('emp_id', $employee['emp_id'], '=')
      ->execute();
  if (!empty($form_state['values']['job']['projects'])) {
    $projects_insert_query = db_insert('hr_employee_job_projects')
        ->fields(array('emp_id', 'org_id', 'project'));
    foreach ($form_state['values']['job']['projects'] as $project) {
      $projects_insert_query->values(array($employee['emp_id'], $employee['org_id'], $project));
    }
    $projects_insert_query->execute();
  }

  db_update('hr_employee')
      ->fields(array(
        'join_date' => $join_date,
        'permanency_date' => $permanency_date,
        'attendance_applicable' => $form_state['values']['attendance_applicable'],
      ))
      ->condition('emp_id', $employee['emp_id'], '=')
      ->execute();

  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/job');
}

function hr_backend_pim_employee_job_form_edit($form, &$form_state) {
  $form_state['storage']['edit'] = TRUE;
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_job_form_edit_callback($form, $form_state) {
  return $form;
}

function hr_backend_pim_employee_job_form_view($form, &$form_state) {
  unset($form_state['storage']['edit']);
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_job_form_view_callback($form, $form_state) {
  return $form;
}

//Terminate Form
function hr_backend_pim_employee_job_terminate_form($form, &$form_state, $emp_id, $termination_data = array()) {
  global $user;
  if ($emp_id && is_numeric($emp_id)) {
    $account = hr_basic_account_get_info($user->uid);

    $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
    $employee = hr_backend_pim_get_employee_data($filters, array(), TRUE);

    if (!empty($employee)) {
      if (hr_backend_pim_permission(HR_PERM_PIM_TERMINATE_EMPLOYEE, $emp_id, $employee, $account)) {
        drupal_set_title('Terminate Employment');

        $assigned_assets = hr_basic_employee_get_assigned_assets($emp_id);
        if (!empty($assigned_assets)) {
          $assets_filters = array(array('field' => 'asset_id', 'value' => $assigned_assets, 'operator' => 'IN'));
          $assets = hr_basic_asset_get_asset_basic_information($assets_filters);

          $assigned_assets_content = '<p>The assets detailed below are still assigned to this employee.</p>';
          foreach ($assets as $asset) {
            $assigned_assets_content .= '<p>' . hr_basic_asset_category_get_name($asset['category']) . ' - ' . hr_basic_asset_brand_get_name($asset['brand']) . ' - ' . $asset['model'] . '</p>';
          }
          $assigned_assets_content .= '<p>All assets must be released before completing this process.</p>';
          $form['assets_assigned'] = array('#markup' => $assigned_assets_content);
        }
        else {
          form_load_include($form_state, 'inc', 'hr_backend', 'includes/backend_pim');
          $attachment_upload_location = strtr(HR_DIRECTORY_EMPLOYEE_TERMINATION_DIR, array('%org_id' => $employee['org_id'], '%emp_id' => $emp_id));
          file_prepare_directory($attachment_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);


          $termination_reasons_filters = array(array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='));
          $termination_reasons = hr_basic_config_termination_reason_get_termination_reasons($termination_reasons_filters);
          $form['reason'] = array(
            '#title' => t('Reason'),
            '#type' => 'select',
            '#required' => TRUE,
            '#options' => $termination_reasons,
            '#empty_option' => '',
            '#default_value' => (isset($termination_data['reason']) ? $termination_data['reason'] : ''),
            '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Reason'),
          );
          $form['termination_date'] = array(
            '#title' => t('Termination date'),
            '#type' => 'textfield',
            '#required' => TRUE,
            '#empty_option' => '',
            '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
            '#default_value' => (isset($termination_data['termination_date']) ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $termination_data['termination_date']) : date(HR_DATETIME_FORMAT_SHORT_SLASH, time())),
          );
          $form['letter'] = array(
            '#title' => t('Letter'),
            '#type' => 'managed_file',
            '#required' => TRUE,
            '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf msg'),),
            '#upload_location' => $attachment_upload_location,
            '#default_value' => (isset($termination_data['letter_fid']) ? $termination_data['letter_fid'] : ''),
          );
          $form['separation'] = array(
            '#title' => t('Separation Form'),
            '#type' => 'managed_file',
            '#required' => TRUE,
            '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf'),),
            '#upload_location' => $attachment_upload_location,
            '#default_value' => (isset($termination_data['separation_form_fid']) ? $termination_data['separation_form_fid'] : ''),
          );
          $form['settlement'] = array(
            '#title' => t('Final Settlement Receipt'),
            '#type' => 'managed_file',
            '#required' => FALSE,
            '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf'),),
            '#upload_location' => $attachment_upload_location,
            '#default_value' => (isset($termination_data['settlement_receipt_fid']) ? $termination_data['settlement_receipt_fid'] : ''),
          );
          $form['note'] = array(
            '#title' => t('Note'),
            '#type' => 'textarea',
            '#required' => FALSE,
            '#maxlength' => 400,
            '#rows' => 3,
            '#resizable' => FALSE,
            '#default_value' => (isset($termination_data['note']) ? $termination_data['note'] : ''),
          );

          $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Confirm'),
            '#attributes' => array('class' => array('btn-success'))
          );
          $form['cancel'] = array(
            '#type' => 'submit',
            '#value' => t('Cancel'),
            '#submit' => array('hr_backend_pim_employee_job_terminate_form_submit'),
            '#limit_validation_errors' => array(),
            '#attributes' => array('class' => array('btn-info'))
          );

          $form['#attributes']['class'][] = 'employee-termination-form';
        }

        return $form;
      }
      else {
        hr_backend_modal_access_denied();
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

function hr_backend_pim_employee_job_terminate_form_validate(&$form, &$form_state) {
  
}

function hr_backend_pim_employee_job_terminate_form_submit(&$form, &$form_state) {
  global $user;

  $emp_id = $form_state['build_info']['args'][0];
  $termination_data = (isset($form_state['build_info']['args'][1]) && !empty($form_state['build_info']['args'][1]) ? $form_state['build_info']['args'][1] : array());

  $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
  $employee = hr_backend_pim_get_employee_data($filters, array(), TRUE);

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $termination_date = strtotime(str_replace('/', '-', $form_state['values']['termination_date']));

    $fields = array(
      'reason' => $form_state['values']['reason'],
      'letter_fid' => $form_state['values']['letter'],
      'separation_form_fid' => $form_state['values']['separation'],
      'settlement_receipt_fid' => $form_state['values']['settlement'],
      'termination_date' => $termination_date,
      'note' => $form_state['values']['note'],
      'changed' => time(),
    );

    $id = 0;
    if (empty($termination_data)) {
      $fields['emp_id'] = $emp_id;
      $fields['org_id'] = $employee['org_id'];
      $fields['uid'] = $user->uid;
      $fields['created'] = time();

      $id = db_insert('hr_terminated_employee')
          ->fields($fields)
          ->execute();
    }
    else {
      $id = $termination_data['id'];
      db_update('hr_terminated_employee')
          ->fields($fields)
          ->condition('id', $id, '=')
          ->execute();
    }

    if ($form_state['values']['letter']) {
      if (!isset($termination_data['letter_fid'])) {
        hr_basic_file_save_file_data($form_state['values']['letter'], 'hr_backend', HR_FILE_USAGE_EMPLOYEE_TERMINATION_LETTER, $id);
      }
      elseif ($termination_data['letter_fid'] != $form_state['values']['letter']) {
        hr_basic_file_delete_file_data($termination_data['letter_fid'], 'hr_backend');

        hr_basic_file_save_file_data($form_state['values']['letter'], 'hr_backend', HR_FILE_USAGE_EMPLOYEE_TERMINATION_LETTER, $id);
      }
    }
    if ($form_state['values']['separation']) {
      if (!isset($termination_data['separation_form_fid'])) {
        hr_basic_file_save_file_data($form_state['values']['separation'], 'hr_backend', HR_FILE_USAGE_EMPLOYEE_TERMINATION_SEPERATION_FORM, $id);
      }
      elseif ($termination_data['separation_form_fid'] != $form_state['values']['separation']) {
        hr_basic_file_delete_file_data($termination_data['separation_form_fid'], 'hr_backend');

        hr_basic_file_save_file_data($form_state['values']['separation'], 'hr_backend', HR_FILE_USAGE_EMPLOYEE_TERMINATION_SEPERATION_FORM, $id);
      }
    }


    if ($form_state['values']['settlement']) {
      if (!isset($termination_data['settlement_receipt_fid'])) {
        hr_basic_file_save_file_data($form_state['values']['settlement'], 'hr_backend', HR_FILE_USAGE_EMPLOYEE_TERMINATION_SETTLEMENT_RECEIPT, $id);
      }
      elseif ($termination_data['settlement_receipt_fid'] != $form_state['values']['settlement']) {
        hr_basic_file_delete_file_data($termination_data['settlement_receipt_fid'], 'hr_backend');
        hr_basic_file_save_file_data($form_state['values']['settlement'], 'hr_backend', HR_FILE_USAGE_EMPLOYEE_TERMINATION_SETTLEMENT_RECEIPT, $id);
      }


      db_update('hr_employee')
          ->fields(array(
            'status' => HR_EMPLOYEE_STATUS_INACTIVE,
            'changed' => time(),
          ))
          ->condition('emp_id', $emp_id, '=')
          ->execute();
    }

    drupal_set_message('Updated Successfully');
  }
  elseif (isset($form_state['values']['cancel']) && $form_state['values']['op'] == $form_state['values']['cancel']) {
    
  }

  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands = array();
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $emp_id . '/job');
  print ajax_render($commands);
  exit;
}

function hr_backend_pim_employee_job_complete_terminate_form($form, &$form_state, $emp_id) {
  global $user;
  if ($emp_id && is_numeric($emp_id)) {
    $account = hr_basic_account_get_info($user->uid);

    $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
    $employee = hr_backend_pim_get_employee_data($filters, array(), TRUE);

    if (!empty($employee)) {
      if (hr_backend_pim_permission(HR_PERM_PIM_TERMINATE_EMPLOYEE, $emp_id, $employee, $account)) {
        drupal_set_title('Complete Employment Termination');

        form_load_include($form_state, 'inc', 'hr_backend', 'includes/backend_pim');
        $attachment_upload_location = strtr(HR_DIRECTORY_EMPLOYEE_TERMINATION_DIR, array('%org_id' => $employee['org_id'], '%emp_id' => $emp_id));
        file_prepare_directory($attachment_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

        $form['settlement'] = array('#title' => t('Final Settlement Receipt'), '#type' => 'managed_file', '#required' => FALSE,
          '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf'),),
          '#upload_location' => $attachment_upload_location,);

        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Confirm'),
          '#attributes' => array('class' => array('btn-success'))
        );
        $form['cancel'] = array(
          '#type' => 'submit',
          '#value' => t('Cancel'),
          '#submit' => array('hr_backend_pim_employee_job_terminate_form_submit'),
          '#limit_validation_errors' => array(),
          '#attributes' => array('class' => array('btn-info'))
        );


        $form['#attributes']['class'][] = 'employee-termination-form';

        return $form;
      }
      else {
        hr_backend_modal_access_denied();
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

function hr_backend_pim_employee_job_complete_terminate_form_validate(&$form, &$form_state) {
  
}

function hr_backend_pim_employee_job_complete_terminate_form_submit(&$form, &$form_state) {
  global $user;

  $emp_id = $form_state['build_info']['args'][0];

  $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
  $employee = hr_backend_pim_get_employee_data($filters, array(), TRUE);

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    db_update('hr_terminated_employee')
        ->fields(array(
          'settlement_receipt_fid' => $form_state['values']['settlement'],
          'changed' => time(),
        ))
        ->condition('emp_id', $emp_id, '=')
        ->execute();

    $id = db_select('hr_terminated_employee')
            ->fields('hr_terminated_employee', array('id'))
            ->condition('emp_id', $emp_id, '=')
            ->execute()->fetchField(0);

    hr_basic_file_save_file_data($form_state['values']['settlement'], 'hr_backend', HR_FILE_USAGE_EMPLOYEE_TERMINATION_SETTLEMENT_RECEIPT, $id);

    db_update('hr_employee')
        ->fields(array(
          'status' => HR_EMPLOYEE_STATUS_INACTIVE,
          'changed' => time(),
        ))
        ->condition('emp_id', $emp_id, '=')
        ->execute();

    drupal_set_message('Updated Successfully');
  }
  elseif (isset($form_state['values']['cancel']) && $form_state['values']['op'] == $form_state['values']['cancel']) {
    
  }
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands = array();
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $emp_id . '/job');
  print ajax_render($commands);
  exit;
}

function hr_backend_pim_employee_job_activate_employment($emp_id) {
  global $user;
  $account = hr_basic_account_get_info($user->uid);

  $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
  $employee = hr_backend_pim_get_employee_data($filters, array(), TRUE);

  if (!empty($employee)) {
    if (hr_backend_pim_permission(HR_PERM_PIM_ACTIVATE_EMPLOYEE_EMPLOYMENT, $emp_id, $employee, $account)) {
      db_update('hr_employee')
          ->fields(array(
            'attendance_applicable' => 1,
            'status' => HR_EMPLOYEE_STATUS_ACTIVE,
            'changed' => time(),
          ))
          ->condition('emp_id', $emp_id, '=')
          ->execute();

      db_delete('hr_terminated_employee')
          ->condition('emp_id', $emp_id, '=')
          ->execute();
      drupal_set_message('Updated Successfully');
      drupal_goto('pim/employee/details/' . $emp_id . '/job');
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

/*
 * Details Salary
 */

function hr_backend_pim_employee_details_content_salary($employee) {
  $contact_form = drupal_get_form('hr_backend_pim_employee_salary_form', $employee);
  $output = drupal_render($contact_form);

  $salary_history_list = hr_backend_pim_employee_get_salary_history_list($employee);
  $salary_history_output = theme('table', $salary_history_list);

  return array('content' => $output, 'history' => $salary_history_output);
}

function hr_backend_pim_employee_get_salary_history_list($employee) {
  $rows = array();

  foreach ($employee['salary_history'] as $history) {
    $data = unserialize($history['formatted_data']);

    $currency = explode(' - ', $data['currency']);

    $row = array(
      'changed' => date(HR_DATETIME_FORMAT_SHORT_SLASH, $history['created']),
      'grade' => $data['grade'],
      'currency' => (isset($currency[0]) ? $currency[0] : $data['currency']),
      'basic_salary' => number_format($data['basic_salary'], 2),
      'earning' => '',
      'deduction' => '',
      'total_payable' => number_format($data['total_payable'], 2),
      'total_cost' => number_format($data['total_cost'], 2),
      'updated_by' => hr_basic_account_get_name($history['uid']),
      'updated_on' => date(HR_DATETIME_FORMAT_LONG_SLASH, $history['created']),
    );

    $total_earning = 0;
    $earning_content = '<ul class="minimal-list">';
    foreach ($data['earning'] as $earning) {
      $total_earning += $earning['value'];
      $earning_content .= '<li>' . $earning['name'] . ': ' . $earning['value'] . '</li>';
    }
    $earning_content .= '</ul>';

    $deduction_content = '<ul class="minimal-list">';
    foreach ($data['deduction'] as $deduction) {
      $deduction_content .= '<li>' . $deduction['name'] . ': ' . $deduction['value'] . '</li>';
    }
    $deduction_content .= '</ul>';

    $row['earning'] = '<strong>' . number_format($total_earning, 2) . '</strong><br />' . $earning_content;
    $row['deduction'] = '<strong>' . number_format($data['total_deduction'], 2) . '</strong><br />' . $deduction_content;

    $rows[]['data'] = $row;
  }

  $header = array(
    array('data' => 'Changed'),
    array('data' => 'Grade'),
    array('data' => 'Curr'),
    array('data' => 'Basic'),
    array('data' => 'Earnings'),
    array('data' => 'Deductions'),
    array('data' => 'Payable'),
    array('data' => 'Cost'),
    array('data' => 'Updated by'),
    array('data' => 'Updated on'),
  );

  $list = array(
    'attributes' => array('class' => array('table', 'table-bordered', 'table-striped')),
    'rows' => $rows,
    'header' => $header
  );

  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-custom-export';
  }

  return $list;
}

function hr_backend_pim_employee_salary_form($form, &$form_state, $employee) {
  $organization_filter = array(array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='));

  if (!isset($form_state['storage']['first_time'])) {
    $form_state['storage']['first_time'] = 1;
  }
  else {
    $form_state['storage']['first_time'] = 0;
  }

  $first_time = $form_state['storage']['first_time'];

  if (!isset($form_state['storage']['grades'])) {
    $form_state['storage']['grades'] = hr_basic_pay_grade_get_basic_info($organization_filter);

    $grade_currencies_options = array();
    foreach ($form_state['storage']['grades'] as $grade) {
      foreach ($grade['currency'] as $currency) {
        $grade_currencies_options[$grade['pay_grade_id']][$currency['id']] = hr_basic_taxonomy_term_name($currency['currency']) . ' (' . $currency['max_salary'] . ' - ' . $currency['min_salary'] . ')';
      }
    }
    $form_state['storage']['grade_currencies'] = $grade_currencies_options;
  }
  if (!isset($form_state['storage']['salary_components'])) {
    $form_state['storage']['salary_components'] = hr_basic_salary_component_get_basic_info($organization_filter);
  }

  $grades = $form_state['storage']['grades'];

  $grade_currencies_options = $form_state['storage']['grade_currencies'];
  $grades_options = array();
  foreach ($grades as $grade) {
    $grades_options[$grade['pay_grade_id']] = $grade['name'];
  }

  $salary_components = $form_state['storage']['salary_components'];
  $salary_components_options = array();
  foreach ($salary_components as $salary_component) {
    $salary_components_options[$salary_component['id']] = $salary_component['name'];
  }

  $salary = $employee['salary'];

  $form['ssn'] = array(
    '#title' => t('SSN'),
    '#type' => 'textfield',
    '#attributes' => array('class' => array(''),),
    '#default_value' => $employee['ssn'],
    '#maxlength' => 255,
    '#required' => TRUE,
    '#prefix' => '<div class="row"><div class="col-sm-6">',
    '#suffix' => '</div>',
  );
  $form['tax_no'] = array(
    '#title' => t('Tax No.'),
    '#type' => 'textfield',
    '#attributes' => array('class' => array(''),),
    '#default_value' => $employee['tax_no'],
    '#maxlength' => 255,
    '#prefix' => '<div class="col-sm-6">',
    '#suffix' => '</div></div>',
  );

  $form['payment'] = array(
    '#type' => 'fieldset',
    '#title' => 'Payment Details',
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#prefix' => '<div id="payment-details" class="">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('payment-details')),
  );

  $default_grade = '';
  if (isset($form_state['values']['payment']['grade']) && !empty($form_state['values']['payment']['grade'])) {
    $default_grade = $form_state['values']['payment']['grade'];
  }
  elseif (isset($salary['grade']) && !empty($salary['grade'])) {
    $default_grade = $salary['grade'];
  }

  $form['payment']['grade'] = array('#title' => t('Pay Grade'), '#type' => 'select', '#options' => $grades_options, '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Pay Grade'),
    '#default_value' => $default_grade,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'hr_backend_pim_employee_salary_form_callback',
      'wrapper' => 'employee-salary-form-wrapper',
      'method' => 'replace',
    ),);

  $default_currency = '';
  if (isset($form_state['values']['payment']['currency']) && !empty($form_state['values']['payment']['currency'])) {
    $default_currency = $form_state['values']['payment']['currency'];
  }
  elseif (isset($salary['currency']) && !empty($salary['currency'])) {
    $default_currency = $salary['currency'];
  }

  if (($default_currency != '' && is_numeric($default_currency)) &&
      ($default_grade != '' && is_numeric($default_grade))) {
    
  }
  $form['payment']['currency'] = array('#title' => t('Currency'), '#type' => 'select',
    '#options' => (isset($grade_currencies_options[$default_grade]) ? $grade_currencies_options[$default_grade] : array()),
    '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Currency'),
    '#default_value' => $default_currency,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'hr_backend_pim_employee_salary_form_callback',
      'wrapper' => 'employee-salary-form-wrapper',
      'method' => 'replace',
    ),);

  $default_basic_salary = '';
  if (isset($form_state['values']['payment']['basic_salary']) && !empty($form_state['values']['payment']['basic_salary'])) {
    $default_basic_salary = $form_state['values']['payment']['basic_salary'];
  }
  elseif (isset($salary['basic']) && !empty($salary['basic'])) {
    $default_basic_salary = $salary['basic'];
  }
  $form['payment']['basic_salary'] = array('#title' => t('Basic Salary'), '#type' => 'textfield', '#required' => TRUE,
    '#attributes' => array('class' => array(''),),
    '#default_value' => $default_basic_salary,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'hr_backend_pim_employee_salary_form_callback',
      'wrapper' => 'employee-salary-form-wrapper',
      'method' => 'replace',
    ),
  );

  $form['earning'] = array(
    '#type' => 'fieldset',
    '#title' => 'All Earnings',
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#prefix' => '<div id="earning-details" class="">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('earning-details')),
  );

  $earning_count = 0;
  foreach ($salary_components as $salary_component) {
    if ($salary_component['type'] == HR_SALARY_COMPONENT_EARNING) {
      $display_label = 'invisible';
      $class = 'row-' . $earning_count;
      if ($earning_count == 0) {
        $display_label = '';
      }
      $salary_component_id = $salary_component['id'];
      $form['earning'][$salary_component_id] = array(
        '#type' => 'container',
        '#tree' => TRUE,
        '#attributes' => array('class' => array($class, 'col-sm-12', 'salary-item-row')),
      );

      $selected_suffix = ' (<b>Amount</b>)';
      if ($salary_component['value_percentage']) {
        $selected_suffix = ' (<b>Percentage</b>)';
      }
      $selected = 0;
      $value = '';
      $exemption = '';

      if (isset($salary['items'][$salary_component_id])) {
        $value = $salary['items'][$salary_component_id]['value'];
        $exemption = $salary['items'][$salary_component_id]['exemption'];
        $selected = 1;
      }

      $form['earning'][$salary_component_id]['value_type'] = array(
        '#type' => 'hidden',
        '#default_value' => HR_SALARY_COMPONENT_VALUE_TYPE_AMOUNT,
      );

      $form['earning'][$salary_component_id]['selected'] = array(
        '#type' => 'checkbox', '#title' => $salary_component['name'] . $selected_suffix,
        '#default_value' => $selected,
        '#prefix' => '<div class="col-sm-4 salary-tem-row-field first">',
        '#suffix' => '</div>',
        '#ajax' => array(
          'event' => 'change',
          'callback' => 'hr_backend_pim_employee_salary_form_callback',
          'wrapper' => 'employee-salary-form-wrapper',
          'method' => 'replace',
        ),
      );


      $form['earning'][$salary_component_id]['value'] = array(
        '#title' => t('Value'), '#type' => 'textfield',
        '#attributes' => array('class' => array(''),),
        '#default_value' => $value,
        '#ajax' => array(
          'event' => 'change',
          'callback' => 'hr_backend_pim_employee_salary_form_callback',
          'wrapper' => 'employee-salary-form-wrapper',
          'method' => 'replace',
        ),
        '#states' => array(
          'enabled' => array(
            ':input[name="earning[' . $salary_component_id . '][selected]"]' => array('checked' => TRUE),
          ),
        ),
        '#title_display' => $display_label,
        '#prefix' => '<div class="col-sm-4 salary-tem-row-field">',
        '#suffix' => '</div>',
      );

      if ($salary_component['value_percentage']) {
        $form['earning'][$salary_component_id]['value_type']['#default_value'] = HR_SALARY_COMPONENT_VALUE_TYPE_PERCENTAGE;
        $form['earning'][$salary_component_id]['exemption'] = array(
          '#title' => t('Exemption'), '#type' => 'textfield',
          '#attributes' => array('class' => array(''),),
          '#default_value' => $exemption,
          '#ajax' => array(
            'event' => 'change',
            'callback' => 'hr_backend_pim_employee_salary_form_callback',
            'wrapper' => 'employee-salary-form-wrapper',
            'method' => 'replace',
          ),
          '#states' => array(
            'enabled' => array(
              ':input[name="earning[' . $salary_component_id . '][selected]"]' => array('checked' => TRUE),
            ),
          ),
          '#title_display' => $display_label,
          '#prefix' => '<div class="col-sm-4 salary-tem-row-field last">',
          '#suffix' => '</div>',
        );
      }
      else {
        $form['earning'][$salary_component_id]['exemption'] = array(
          '#title' => t('Exemption'), '#type' => 'textfield',
          '#attributes' => array('class' => array(''),),
          '#default_value' => $exemption,
          '#title_display' => $display_label,
          '#prefix' => '<div class="col-sm-4 salary-tem-row-field last">',
          '#suffix' => '</div>',
          '#disabled' => TRUE,
        );
      }

      $earning_count++;
    }
  }


  $form['deduction'] = array(
    '#type' => 'fieldset',
    '#title' => 'All Deductions',
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#prefix' => '<div id="deduction-details" class="">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('deduction-details')),
  );

  $deduction_count = 0;
  foreach ($salary_components as $salary_component) {
    if ($salary_component['type'] == HR_SALARY_COMPONENT_DEDUCTION) {
      $display_label = 'invisible';
      $class = 'row-' . $deduction_count;
      if ($deduction_count == 0) {
        $display_label = '';
      }
      $salary_component_id = $salary_component['id'];
      $form['deduction'][$salary_component_id] = array(
        '#type' => 'container',
        '#tree' => TRUE,
        '#attributes' => array('class' => array($class, 'col-sm-12', 'salary-item-row')),
        '#prefix' => '<div class="">',
        '#suffix' => '</div>',
      );

      $selected_suffix = ' (<b>Amoun</b>t)';
      if ($salary_component['value_percentage']) {
        $selected_suffix = ' (<b>Percentage</b>)';
      }
      $selected = 0;
      $value = '';
      $exemption = '';
      if (isset($salary['items'][$salary_component_id])) {
        $selected = 1;
        $value = $salary['items'][$salary_component_id]['value'];
        $exemption = $salary['items'][$salary_component_id]['exemption'];
      }
      $form['deduction'][$salary_component_id]['value_type'] = array(
        '#type' => 'hidden',
        '#default_value' => HR_SALARY_COMPONENT_VALUE_TYPE_AMOUNT,
      );
      $form['deduction'][$salary_component_id]['selected'] = array(
        '#type' => 'checkbox', '#title' => $salary_component['name'] . $selected_suffix,
        '#default_value' => $selected,
        '#prefix' => '<div class="col-sm-4 salary-tem-row-field first">',
        '#suffix' => '</div>',
        '#ajax' => array(
          'event' => 'change',
          'callback' => 'hr_backend_pim_employee_salary_form_callback',
          'wrapper' => 'employee-salary-form-wrapper',
          'method' => 'replace',
        ),
      );


      $form['deduction'][$salary_component_id]['value'] = array(
        '#title' => t('Value'), '#type' => 'textfield',
        '#attributes' => array('class' => array(''),),
        '#default_value' => $value,
        '#ajax' => array(
          'event' => 'change',
          'callback' => 'hr_backend_pim_employee_salary_form_callback',
          'wrapper' => 'employee-salary-form-wrapper',
          'method' => 'replace',
        ),
        '#states' => array(
          'enabled' => array(
            ':input[name="deduction[' . $salary_component_id . '][selected]"]' => array('checked' => TRUE),
          ),
        ),
        '#title_display' => $display_label,
        '#prefix' => '<div class="col-sm-4 salary-tem-row-field">',
        '#suffix' => '</div>',
      );

      if ($salary_component['value_percentage']) {
        $form['deduction'][$salary_component_id]['value_type']['#default_value'] = HR_SALARY_COMPONENT_VALUE_TYPE_PERCENTAGE;
        $form['deduction'][$salary_component_id]['exemption'] = array(
          '#title' => t('Exemption'), '#type' => 'textfield',
          '#attributes' => array('class' => array(''),),
          '#default_value' => $exemption,
          '#ajax' => array(
            'event' => 'change',
            'callback' => 'hr_backend_pim_employee_salary_form_callback',
            'wrapper' => 'employee-salary-form-wrapper',
            'method' => 'replace',
          ),
          '#states' => array(
            'enabled' => array(
              ':input[name="deduction[' . $salary_component_id . '][selected]"]' => array('checked' => TRUE),
            ),
          ),
          '#title_display' => $display_label,
          '#prefix' => '<div class="col-sm-4 salary-tem-row-field last">',
          '#suffix' => '</div>',
        );
      }
      else {
        $form['deduction'][$salary_component_id]['exemption'] = array(
          '#title' => t('Exemption'), '#type' => 'textfield',
          '#attributes' => array('class' => array(''),),
          '#default_value' => $exemption,
          '#title_display' => $display_label,
          '#prefix' => '<div class="col-sm-4 salary-tem-row-field last">',
          '#suffix' => '</div>',
          '#disabled' => TRUE,
        );
      }

      $deduction_count++;
    }
  }


  /*
   * Calculations
   */
  $basic_salary = (isset($salary['basic']) ? $salary['basic'] : 0);
  $earnings = (isset($salary['earnings']) ? $salary['earnings'] : 0);
  $deductions = (isset($salary['deductions']) ? $salary['deductions'] : 0);
  $addition_cost = (isset($salary['addition_cost']) ? $salary['addition_cost'] : 0);
  $total_cost = (isset($salary['total_cost']) ? $salary['total_cost'] : 0);  

  $form_state['storage']['data']['basic'] = $basic_salary;
  $form_state['storage']['data']['earnings'] = $earnings;
  $form_state['storage']['data']['deductions'] = $deductions;
  $form_state['storage']['data']['addition_cost'] = $addition_cost;
  $form_state['storage']['data']['total_cost'] = $total_cost;

  if (!empty($form_state['values']['payment']['basic_salary']) && is_numeric($form_state['values']['payment']['basic_salary'])) {
    $basic_salary = $form_state['values']['payment']['basic_salary'];
    $earnings = 0;
    $deductions = 0;
    $addition_cost = 0;
    $total_cost = $basic_salary;

    foreach ($form_state['values']['earning'] as $salary_compnent_id => $earning) {
      if ($earning['selected'] && !empty($earning['value']) && is_numeric($earning['value'])) {
        $temp_value = $earning['value'];
        if ($salary_components[$salary_compnent_id]['value_percentage']) {
          $basic_salary_after_exepmption = $basic_salary;
          if (!empty($earning['exemption']) && is_numeric($earning['exemption'])) {
            $basic_salary_after_exepmption = $basic_salary - $earning['exemption'];
          }
          $temp_value = $basic_salary_after_exepmption * ($earning['value'] / 100);
        }
        if ($salary_components[$salary_compnent_id]['payable']) {
          $earnings += $temp_value;
        }
        if ($salary_components[$salary_compnent_id]['cost']) {
          $total_cost += $temp_value;
        }
      }
    }

    foreach ($form_state['values']['deduction'] as $salary_compnent_id => $deduction) {
      if ($deduction['selected'] && !empty($deduction['value']) && is_numeric($deduction['value'])) {
        $temp_value = $deduction['value'];
        if ($salary_components[$salary_compnent_id]['value_percentage']) {
          $basic_salary_after_exepmption = $basic_salary;
          if (!empty($deduction['exemption']) && is_numeric($deduction['exemption'])) {
            $basic_salary_after_exepmption = $basic_salary - $deduction['exemption'];
          }
          $temp_value = $basic_salary_after_exepmption * ($deduction['value'] / 100);
        }
        if ($salary_components[$salary_compnent_id]['payable']) {
          $deductions += $temp_value;
        }
        if ($salary_components[$salary_compnent_id]['cost']) {
          $total_cost += $temp_value;
        }
      }
    }
  }

  $net_salary = $basic_salary + $earnings;

  if (!isset($form_state['storage']['regulation'])) {
    $form_state['storage']['regulation'] = 0;
    if (isset($salary['regulation']) && $salary['regulation'] == HR_TERM_COUNTRY_JORDAN) {
      $form_state['storage']['regulation'] = 1;
    }
  }
  else {
    if (isset($form_state['values']['regulations']['jordan']) && $form_state['values']['regulations']['jordan']) {
      $form_state['storage']['regulation'] = 1;
    }
    else {
      $form_state['storage']['regulation'] = 0;
    }
  }

  $form['regulations'] = array(
    '#type' => 'fieldset',
    '#title' => 'Jordan Regulations',
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#prefix' => '<div id="regulations-details" class="">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('regulations-details')),
  );

  $jordan_regulation = $form_state['storage']['regulation'];


  $form['regulations']['jordan'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable Jordan Regulations',
    '#default_value' => $jordan_regulation,
    '#prefix' => '<div class="form-item form-group">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'hr_backend_pim_employee_salary_form_callback',
      'wrapper' => 'employee-salary-form-wrapper',
      'method' => 'replace',
    ),
  );

  $income_tax = 0;
  $social_by_employee = 0;
  $social_by_organization = 0;
  if ($jordan_regulation) {
    $jordan_summary = '';
    $martial_status = $employee['martial_status'];
    if ($martial_status == 0) {
      $jordan_summary = '<div class="push-5 response-red">Martial status not exists</div>';
    }
    else {
      $income_tax_exemption = 1000;
      if ($martial_status == HR_TERM_MARTIAL_STATUS_MARRIED) {
        $income_tax_exemption = 2000;
      }

      $yearly_basic_salary = $net_salary * 12;
      $yearly_income_tax_exemption = $income_tax_exemption * 12;

      $taxable_basic_salary = $yearly_basic_salary - $yearly_income_tax_exemption;
      if ($taxable_basic_salary > 0) {
        if ($taxable_basic_salary <= 10000) {
          $income_tax = ($taxable_basic_salary * .07) / 12;
        }
        elseif ($taxable_basic_salary <= 20000) {
          $first_income_tax = (10000 * .07) / 12;
          $second_income_tax = (($taxable_basic_salary - 10000) * .14) / 12;
          $income_tax = $first_income_tax + $second_income_tax;
        }
        else {
          $first_income_tax = (10000 * .07) / 12;
          $second_income_tax = (10000 * .14) / 12;
          $third_income_tax = (($taxable_basic_salary - 20000) * .20) / 12;
          $income_tax = $first_income_tax + $second_income_tax + $third_income_tax;
        }
      }

      $ssn_emp_percentage = variable_get(HR_VAR_COUNTRY_REGU_JO_SSN_EMP_PERCENTAGE, 0);
      $ssn_org_percentage = variable_get(HR_VAR_COUNTRY_REGU_JO_SSN_ORG_PERCENTAGE, 0);

      $social_by_employee = $basic_salary * $ssn_emp_percentage;
      $social_by_organization = $basic_salary * $ssn_org_percentage;

      $jordan_summary .= '<div class="push-5"><strong>Income Tax: </strong>' . round($income_tax, 2) . ' JOD</div>';
      $jordan_summary .= '<div class="push-5"><strong>Social Security by Employee: </strong>' . round($social_by_employee, 2) . ' JOD</div>';
      $jordan_summary .= '<div class="push-5"><strong>Social Security by Organization: </strong>' . round($social_by_organization, 2) . ' JOD</div>';

      if (!$first_time) {
        $deductions += $income_tax;
        $deductions += $social_by_employee;
        $addition_cost += $social_by_organization;
        $total_cost += $social_by_organization;
      }
    }

    $form['regulations']['jordan_summary'] = array('#markup' => $jordan_summary);
  }

  $form_state['storage']['data']['basic'] = $basic_salary;
  $form_state['storage']['data']['earnings'] = $earnings;
  $form_state['storage']['data']['deductions'] = $deductions;
  $form_state['storage']['data']['addition_cost'] = $addition_cost;
  $form_state['storage']['data']['total_cost'] = $total_cost;


  $paypal = ($basic_salary + $earnings - $deductions);

  $form_state['storage']['summary']['basic_salary'] = round($basic_salary, 2);
  $form_state['storage']['summary']['deduction'] = round($deductions, 2);
  $form_state['storage']['summary']['earning'] = round($earnings, 2);
  $form_state['storage']['summary']['payable'] = round($paypal, 2);
  $form_state['storage']['summary']['cost'] = round($total_cost, 2);

  $form['allocation'] = array(
    '#type' => 'fieldset',
    '#title' => 'Salary Project Allocation',
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#prefix' => '<div id="salary-allocation" class="">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('salary-allocation')),
  );

  $projects = hr_basic_project_get_project_info($organization_filter);
  $salary_allocation = $employee['salary_allocation'];
  $project_count = 0;

  $covered_percentage = 0;
  $covered_total = 0;

  foreach ($projects as $project_id => $project) {
    $selected = 0;
    $percentage = '';
    $total = '';
    if (isset($form_state['values']['allocation'][$project_id]) &&
        $form_state['values']['allocation'][$project_id]['selected'] &&
        !empty($form_state['values']['allocation'][$project_id]['percentage'])) {
      $selected = 1;
      $percentage = $form_state['values']['allocation'][$project_id]['percentage'];
      $total = ($percentage / 100) * $total_cost;

      $covered_percentage += $percentage;
      $covered_total += $total;
    }
    elseif (isset($salary_allocation[$project_id]) && $form_state['storage']['first_time']) {
      $selected = 1;
      $percentage = $salary_allocation[$project_id]['percentage_value'];
      $total = ($percentage / 100) * $total_cost;

      $covered_percentage += $salary_allocation[$project_id]['percentage_value'];
      $covered_total += $total;
    }

    $display_label = 'invisible';
    $class = 'row-' . $project_count;
    if ($project_count == 0) {
      $display_label = '';
    }
    $form['allocation'][$project_id]['selected'] = array(
      '#type' => 'checkbox', '#title' => $project['name'],
      '#default_value' => $selected,
      '#prefix' => '<div class="col-sm-12"><div class="col-sm-4 ' . $class . ' allocaion-project-row-field ">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'event' => 'change',
        'callback' => 'hr_backend_pim_employee_salary_form_callback',
        'wrapper' => 'employee-salary-form-wrapper',
        'method' => 'replace',
      ),
    );


    $form['allocation'][$project_id]['percentage'] = array(
      '#title' => t('Percentage'),
      '#type' => 'textfield',
      '#attributes' => array('class' => array(''),),
      '#default_value' => $percentage,
      '#ajax' => array(
        'event' => 'change',
        'callback' => 'hr_backend_pim_employee_salary_form_callback',
        'wrapper' => 'employee-salary-form-wrapper',
        'method' => 'replace',
      ),
      '#states' => array(
        'enabled' => array(
          ':input[name="allocation[' . $project_id . '][selected]"]' => array('checked' => TRUE),
        ),
      ),
      '#title_display' => $display_label,
      '#prefix' => '<div class="col-sm-4 ' . $class . ' allocaion-project-row-field">',
      '#suffix' => '</div>',
    );

    $form['allocation'][$project_id]['total'] = array(
      '#title' => t('Covered Value'), '#type' => 'textfield',
      '#attributes' => array('class' => array(''),),
      '#default_value' => round($total, 2),
      '#title_display' => $display_label,
      '#prefix' => '<div class="col-sm-4 ' . $class . ' allocaion-project-row-field last">',
      '#suffix' => '</div></div>',
      '#disabled' => TRUE,
    );

    $project_count++;
  }


  $form['summary'] = array(
    '#type' => 'fieldset',
    '#title' => 'Salary Summary',
    '#collapsible' => TRUE,
    '#prefix' => '<div id="summary-details" class="">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('summary-details')),
  );
  $form['summary']['deduction'] = array(
    '#markup' => '<div class="push-5"><strong>Total Deductions: </strong>' .
    (isset($form_state['storage']['summary']['deduction']) ? $form_state['storage']['summary']['deduction'] : 0)
    . '</div>'
  );
  $form['summary']['payable'] = array(
    '#markup' => '<div class="push-5"><strong>Total Payable: </strong>' .
    (isset($form_state['storage']['summary']['payable']) ? $form_state['storage']['summary']['payable'] : 0)
    . '</div>'
  );
  $form['summary']['cost'] = array(
    '#markup' => '<div class="push-5"><strong>Cost to the Organization: </strong>' .
    (isset($form_state['storage']['summary']['cost']) ? $form_state['storage']['summary']['cost'] : 0)
    . '</div>'
  );

  $form['summary']['covered_percentage'] = array(
    '#markup' => '<hr /><div class="push-5"><strong>Covered Salary Percentage: </strong>' .
    $covered_percentage
    . '%</div>'
  );
  $form['summary']['covered_total'] = array(
    '#markup' => '<div class="push-5"><strong>Covered Salary: </strong>' .
    $covered_total
    . '</div>'
  );
  if ($total_cost > $covered_total) {
    $form['summary']['uncovered'] = array(
      '#markup' => '<div class="push-5 text-danger"><strong>Un-Covered Salary: </strong>' .
      ($total_cost - $covered_total)
      . '</div>'
    );
  }

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_SALARY)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#attributes' => array('class' => array('btn-success', 'btn-sm'))
    );
  }
  $form['#prefix'] = '<div id="employee-salary-form-wrapper">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-salary-form';

  return $form;
}

function hr_backend_pim_employee_salary_form_validate(&$form, &$form_state) {
  if (!empty($form_state['values']['payment']['basic_salary'])) {
    if (!is_numeric($form_state['values']['payment']['basic_salary'])) {
      form_set_error('payment[basic_salary', 'Basic salary field must be numeric');
    }
    else {
      if (!empty($form_state['values']['payment']['currency'])) {
        $currency_data = db_select('hr_pay_grade_currency')
                ->fields('hr_pay_grade_currency', array('currency', 'max_salary', 'min_salary'))
                ->condition('id', $form_state['values']['payment']['currency'], '=')
                ->execute()->fetch(PDO::FETCH_ASSOC);
        if (!empty($currency_data)) {
          if ($form_state['values']['payment']['basic_salary'] < $currency_data['min_salary'] ||
              $form_state['values']['payment']['basic_salary'] > $currency_data['max_salary']) {
            form_set_error('payment[basic_salary', 'Basic salary field must be with the range of pay grade currency');
          }
        }
      }
    }
  }

  $salary_components = $form_state['storage']['salary_components'];

  if (isset($form_state['values']['earning']) && !empty($form_state['values']['earning'])) {
    foreach ($form_state['values']['earning'] as $salary_compnent_id => $earning) {
      if ($earning['selected']) {
        if (!empty($earning['value'])) {
          if (!is_numeric($earning['value'])) {
            $form_state['values']['earning'][$salary_compnent_id]['value'] = '';
            $form_state['input']['earning'][$salary_compnent_id]['value'] = '';
            form_set_error('earning[' . $salary_compnent_id . '][value', 'Value of ' . $salary_components[$salary_compnent_id]['name'] . ' field must be numeric');
          }
        }
        else {
          form_set_error('earning[' . $salary_compnent_id . '][value', 'Value of ' . $salary_components[$salary_compnent_id]['name'] . ' is required');
        }
        if ($salary_components[$salary_compnent_id]['value_percentage']) {
          if (!empty($earning['exemption'])) {
            if (!is_numeric($earning['exemption'])) {
              $form_state['values']['earning'][$salary_compnent_id]['exemption'] = '';
              $form_state['input']['earning'][$salary_compnent_id]['exemption'] = '';
              form_set_error('earning[' . $salary_compnent_id . '][exemption', 'Exemption of ' . $salary_components[$salary_compnent_id]['name'] . '  field must be numeric');
            }
          }
        }
      }
    }
  }
  if (isset($form_state['values']['deduction']) && !empty($form_state['values']['deduction'])) {
    foreach ($form_state['values']['deduction'] as $salary_compnent_id => $deduction) {
      if ($deduction['selected']) {
        if (!empty($deduction['value'])) {
          if (!is_numeric($deduction['value'])) {
            $form_state['values']['deduction'][$salary_compnent_id]['value'] = '';
            $form_state['input']['deduction'][$salary_compnent_id]['value'] = '';
            form_set_error('deduction[' . $salary_compnent_id . '][value', 'Value of ' . $salary_components[$salary_compnent_id]['name'] . ' field must be numeric');
          }
        }
        else {
          form_set_error('deduction[' . $salary_compnent_id . '][value', 'Value of ' . $salary_components[$salary_compnent_id]['name'] . ' is required');
        }
        if ($salary_components[$salary_compnent_id]['value_percentage']) {
          if (!empty($deduction['exemption'])) {
            if (!is_numeric($deduction['exemption'])) {
              $form_state['values']['deduction'][$salary_compnent_id]['exemption'] = '';
              $form_state['input']['deduction'][$salary_compnent_id]['exemption'] = '';
              form_set_error('deduction[' . $salary_compnent_id . '][exemption', 'Exemption of ' . $salary_components[$salary_compnent_id]['name'] . '  field must be numeric');
            }
          }
        }
      }
    }
  }

  if (isset($form_state['values']['allocation']) && !empty($form_state['values']['allocation'])) {

    $percentage = 0;
    foreach ($form_state['values']['allocation'] as $project_id => $allocation) {

      if ($allocation['selected']) {
        if (!empty($allocation['percentage'])) {
          if (!is_numeric($allocation['percentage'])) {
            $form_state['values']['allocation'][$project_id]['percentage'] = '';
            $form_state['input']['allocation'][$project_id]['percentage'] = '';
            form_set_error('allocation[' . $project_id . '][percentage', 'Invalid salary allocation value');
          }
          else {
            if (($percentage + $allocation['percentage']) > 100) {
              $form_state['values']['allocation'][$project_id]['percentage'] = 100 - $percentage;
              $form_state['input']['allocation'][$project_id]['percentage'] = 100 - $percentage;
              $allocation['percentage'] = 100 - $percentage;
            }

            $percentage += $allocation['percentage'];
          }
        }
      }
    }
  }
}

function hr_backend_pim_employee_salary_form_submit(&$form, &$form_state) {
  global $user;

  $employee = $form_state['build_info']['args'][0];
  $emp_id = $employee['emp_id'];
  $org_id = $employee['org_id'];

  $martial_status = $employee['martial_status'];

  $salary_components = $form_state['storage']['salary_components'];
  $grades = $form_state['storage']['grades'];

  $salary_id = 0;
  if (!isset($employee['salary']['id'])) {
    $salary_fields = array(
      'emp_id' => $emp_id,
      'org_id' => $org_id,
      'grade' => $form_state['values']['payment']['grade'],
      'currency' => $form_state['values']['payment']['currency'],
      'basic' => $form_state['values']['payment']['basic_salary'],
      'earnings' => 0,
      'deductions' => 0,
      'addition_cost' => 0,
      'uid' => $user->uid,
      'created' => time(),
      'changed' => time(),
    );

    $salary_id = db_insert('hr_employee_salary')->fields($salary_fields)->execute();
  }
  else {
    $salary_id = $employee['salary']['id'];
  }

  $salary_items_query = db_insert('hr_employee_salary_item')
      ->fields(array('emp_id', 'org_id', 'salary_component', 'salary_component_type', 'value', 'exemption',
    'total_value'));

  $basic_salary = $form_state['storage']['data']['basic'];
  $earnings = $form_state['storage']['data']['earnings'];
  $deductions = $form_state['storage']['data']['deductions'];
  $addition_cost = $form_state['storage']['data']['addition_cost'];
  $total_cost = $form_state['storage']['data']['total_cost'];

  if (isset($form_state['values']['earning']) && !empty($form_state['values']['earning'])) {
    foreach ($form_state['values']['earning'] as $salary_compnent_id => $earning) {
      if ($earning['selected'] && !empty($earning['value']) && is_numeric($earning['value'])) {
        $temp_basic_salary = $basic_salary;
        $temp_value = $earning['value'];
        $temp_exemption = 0;
        $temp_total_value = 0;

        if ($salary_components[$salary_compnent_id]['value_percentage']) {
          if (!empty($earning['exemption']) && is_numeric($earning['exemption'])) {
            $temp_exemption = $earning['exemption'];
            $temp_basic_salary = $basic_salary - $temp_exemption;
          }
          $temp_total_value = $temp_basic_salary * ($temp_value / 100);
        }
        else {
          $temp_total_value = $temp_value;
        }

        $salary_items_query->values(array(
          $emp_id, $org_id, $salary_compnent_id, HR_SALARY_COMPONENT_EARNING, $temp_value, $temp_exemption, $temp_total_value
        ));
      }
    }
  }
  if (isset($form_state['values']['deduction']) && !empty($form_state['values']['deduction'])) {
    foreach ($form_state['values']['deduction'] as $salary_compnent_id => $deduction) {
      if ($deduction['selected'] && !empty($deduction['value']) && is_numeric($deduction['value'])) {
        $temp_basic_salary = $basic_salary;
        $temp_value = $deduction['value'];
        $temp_exemption = 0;
        $temp_total_value = 0;
        if ($salary_components[$salary_compnent_id]['value_percentage']) {
          if (!empty($deduction['exemption']) && is_numeric($deduction['exemption'])) {
            $temp_exemption = $deduction['exemption'];
            $temp_basic_salary = $basic_salary - $temp_exemption;
          }
          $temp_total_value = $temp_basic_salary * ($temp_value / 100);
        }
        else {
          $temp_total_value = $temp_value;
        }

        $salary_items_query->values(array(
          $emp_id, $org_id, $salary_compnent_id, HR_SALARY_COMPONENT_DEDUCTION, $temp_value, $temp_exemption, $temp_total_value
        ));
      }
    }
  }

  $income_tax = 0;
  $social_by_employee = 0;
  $social_by_organization = 0;
  $regulation_history = array();
  $regulation = 0;
  if (isset($form_state['values']['regulations']['jordan']) && $form_state['values']['regulations']['jordan']) {
    if ($martial_status == 0) {
      
    }
    else {
      $regulation = HR_TERM_COUNTRY_JORDAN;
      $income_tax_exemption = 1000;
      if ($martial_status == HR_TERM_MARTIAL_STATUS_MARRIED) {
        $income_tax_exemption = 2000;
      }

      $yearly_basic_salary = $basic_salary * 12;
      $yearly_income_tax_exemption = $income_tax_exemption * 12;

      $taxable_basic_salary = $yearly_basic_salary - $yearly_income_tax_exemption;
      if ($taxable_basic_salary > 0) {
        if ($taxable_basic_salary <= 10000) {
          $income_tax = ($taxable_basic_salary * .07) / 12;
        }
        elseif ($taxable_basic_salary <= 20000) {
          $first_income_tax = (10000 * .07) / 12;
          $second_income_tax = (($taxable_basic_salary - 10000) * .14) / 12;
          $income_tax = $first_income_tax + $second_income_tax;
        }
        else {
          $first_income_tax = (10000 * .07) / 12;
          $second_income_tax = (10000 * .14) / 12;
          $third_income_tax = (($taxable_basic_salary - 20000) * .20) / 12;
          $income_tax = $first_income_tax + $second_income_tax + $third_income_tax;
        }
      }

      $social_by_employee = $basic_salary * .0725;
      $social_by_organization = $basic_salary * .1375;

      $regulation_history[] = array('name' => 'Incom Tax', 'value' => $income_tax,);
      $regulation_history[] = array('name' => 'Social Security - Employee %7.25', 'value' => $social_by_employee,);
      $regulation_history[] = array('name' => 'Social Security - Organization %13.75', 'value' => $social_by_organization,);
    }
  }

  db_delete('hr_employee_salary_item')
      ->condition('emp_id', $emp_id, '=')
      ->condition('org_id', $org_id, '=')
      ->execute();

  $salary_items_query->execute();

  db_update('hr_employee')
      ->fields(array(
        'ssn' => $form_state['values']['ssn'],
        'tax_no' => $form_state['values']['tax_no'],
        'changed' => time(),
      ))
      ->condition('emp_id', $emp_id, '=')
      ->condition('org_id', $org_id, '=')
      ->execute();

  db_update('hr_employee_salary')
      ->fields(array(
        'emp_id' => $emp_id,
        'org_id' => $org_id,
        'grade' => $form_state['values']['payment']['grade'],
        'currency' => $form_state['values']['payment']['currency'],
        'regulation' => $regulation,
        'basic' => $basic_salary,
        'earnings' => $earnings,
        'deductions' => $deductions,
        'addition_cost' => $addition_cost,
        'total_cost' => $total_cost,
        'uid' => $user->uid,
        'changed' => time(),
      ))
      ->condition('id', $salary_id, '=')
      ->execute();

  db_delete('hr_employee_salary_allocation')
      ->condition('emp_id', $emp_id, '=')
      ->condition('org_id', $org_id, '=')
      ->execute();

  $allocation_projects_query = db_insert('hr_employee_salary_allocation')
      ->fields(array('org_id', 'emp_id', 'project_id', 'percentage_value', 'total_value'));
  if (isset($form_state['values']['allocation']) && !empty($form_state['values']['allocation'])) {
    foreach ($form_state['values']['allocation'] as $project_id => $allocation) {
      if ($allocation['selected'] && !empty($allocation['percentage']) && is_numeric($allocation['percentage'])) {
        $total_value = ($allocation['percentage'] / 100) * $total_cost;
        $allocation_projects_query->values(array($org_id, $emp_id, $project_id, $allocation['percentage'], $total_value));
      }
    }
  }
  $allocation_projects_query->execute();

  if (isset($employee['salary']['id'])) {
    $salary = $employee['salary'];


    $currency_tid = db_select('hr_pay_grade_currency')
            ->fields('hr_pay_grade_currency', array('currency'))
            ->condition('id', $salary['currency'], '=')
            ->execute()->fetchField(0);

    $history_data = array(
      'id' => $employee['salary'],
      'emp_id' => $emp_id,
      'org_id' => $org_id,
      'grade' => $grades[$salary['grade']]['name'],
      'currency' => hr_basic_taxonomy_term_name($currency_tid),
      'basic_salary' => $salary['basic'],
      'total_deduction' => $salary['deductions'],
      'total_payable' => ($salary['basic'] + $salary['earnings'] - $salary['deductions']),
      'total_cost' => $salary['total_cost'],
      'earning' => array(),
      'deduction' => array(),
    );

    foreach ($salary['items'] as $item) {
      if ($item['salary_component_type'] == HR_SALARY_COMPONENT_EARNING) {
        $history_data['earning'][] = array(
          'name' => $salary_components[$item['salary_component']]['name'],
          'value' => $item['total_value'],
        );
      }
      if ($item['salary_component_type'] == HR_SALARY_COMPONENT_DEDUCTION) {
        $history_data['deduction'][] = array(
          'name' => $salary_components[$item['salary_component']]['name'],
          'value' => $item['total_value'],
        );
      }
    }

    foreach ($regulation_history as $item) {
      $history_data['deduction'][] = array(
        'name' => $item['name'],
        'value' => $item['value'],
      );
    }

    $raw_data = serialize($salary);
    $formatted_data = serialize($history_data);

    db_insert('hr_employee_salary_history')
        ->fields(array(
          'emp_id' => $emp_id,
          'org_id' => $org_id,
          'formatted_data' => $formatted_data,
          'raw_data' => $raw_data,
          'uid' => $user->uid,
          'created' => time(),
        ))
        ->execute();
  }
  drupal_set_message('Salary saved successfully');
  drupal_goto('pim/employee/details/' . $emp_id . '/salary');
}

function hr_backend_pim_employee_salary_form_callback(&$form, &$form_state) {
  return $form;
}

/*
 * Report To
 */

function hr_backend_pim_employee_details_content_report_to($employee) {
  $report_to_form = drupal_get_form('hr_backend_pim_employee_report_to_form', $employee);
  $output = drupal_render($report_to_form);

  $employees = array();
  $employees_pictures = array();
  $reporting_methods = array();

  $rows = array();
  foreach ($employee['supervisors'] as $supervisor) {
    if (!isset($employees[$supervisor['supervisor_id']])) {
      $employees[$supervisor['supervisor_id']] = hr_basic_employee_get_name($supervisor['supervisor_id']);
    }
    if (!isset($employees_pictures[$supervisor['supervisor_id']])) {
      $employees_pictures[$supervisor['supervisor_id']] = hr_basic_employee_get_picture_uri($supervisor['supervisor_id']);
    }
    if (!isset($reporting_methods[$supervisor['reporting_method']])) {
      $reporting_methods[$supervisor['reporting_method']] = hr_basic_reporting_method_get_reporting_methods($employee['org_id'], $supervisor['reporting_method']);
    }

    $checkbox = '<label class="css-input css-checkbox css-checkbox-primary">
          <input type="checkbox" value="' . $supervisor['id'] . '" id="supervisor_' . $supervisor['id'] . '" name="supervisor_' . $supervisor['id'] . '"><span></span>
        </label>';
    $rows[]['data'] = array(
      'checkbox' => array('data' => $checkbox, 'class' => array('text-center')),
      'name' => array('data' => $employees[$supervisor['supervisor_id']]),
      'picture' => array('data' => theme('image_style', array('style_name' => 'image_style_organization_logo_small', 'path' => $employees_pictures[$supervisor['supervisor_id']], 'attributes' => array('class' => array('img-avatar', 'img-avatar48')))), 'class' => array('text-center')),
      'reporting_method' => array('data' => $reporting_methods[$supervisor['reporting_method']]),
    );
  }
  $header_checkbox = '<label class="css-input css-checkbox css-checkbox-primary remove-margin-t remove-margin-b">
    <input type="checkbox" id="check-all" name="check-all"><span></span>
    </label>';
  $header = array(
    array('data' => $header_checkbox, 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Employee'),
    array('data' => 'Picture', 'class' => array('nosort', 'nosearch')),
    array('data' => 'Reporting Method'),
  );

  $list = array(
    'attributes' => array('class' => array('employee-supervisors-list', 'table', 'table-bordered', 'table-striped', 'js-table-checkable')),
    'rows' => $rows,
    'header' => $header
  );
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }
  $output .= theme('table', $list);

  return $output;
}

function hr_backend_pim_employee_report_to_form($form, &$form_state, $employee) {
  $access = FALSE;
  if (isset($form_state['storage']['add'])) {
    $access = TRUE;
  }
  if (!isset($form_state['storage']['employees'])) {
    $exclude_employees[] = $employee['emp_id'];
    foreach ($employee['supervisors'] as $supervisor) {
      $exclude_employees[] = $supervisor['supervisor_id'];
    }
    $employees_filters = array(
      array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='),
      array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '='),
      array('field' => 'emp_id', 'value' => $exclude_employees, 'operator' => 'NOT IN')
    );
    $form_state['storage']['employees'] = hr_basic_employee_get_employees_names($employees_filters);
  }

  if (!isset($form_state['storage']['reporting_methods'])) {
    $employees_filters = array(
      array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='),
      array('field' => 'status', 'value' => HR_EMPLOYEE_STATUS_ACTIVE, 'operator' => '='),
      array('field' => 'emp_id', 'value' => $employee['emp_id'], 'operator' => '<>')
    );
    $form_state['storage']['reporting_methods'] = hr_basic_reporting_method_get_reporting_methods($employee['org_id']);
  }

  $form['supervisor'] = array('#title' => t('Supervisor'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['employees'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Supervisor'),
    '#access' => $access,);

  $form['reporting_method'] = array('#title' => t('Reporting method'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['reporting_methods'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Reporting method'),
    '#access' => $access,);

  $form['delete_supervisors'] = array('#type' => 'hidden', '#default_value' => '', '#attributes' => array('id' => 'delete-supervisors-ids'));

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_REPORT_TO)) {
    if (!$access) {
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('hr_backend_pim_employee_report_to_form_add'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_report_to_form_add_callback',
          'wrapper' => 'employee-report-to-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array(
          'class' => array('btn-danger', 'delete-supervisors'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_report_to_form_cancel'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_report_to_form_cancel_callback',
          'wrapper' => 'employee-report-to-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }

  $form['#prefix'] = '<div id="employee-report-to-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-report-to-form';

  return $form;
}

function hr_backend_pim_employee_report_to_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    if (empty($form_state['values']['delete_supervisors'])) {
      form_set_error('delete_supervisors', t('At least select one supervisor to be deleted.'));
    }
  }
}

function hr_backend_pim_employee_report_to_form_submit(&$form, &$form_state) {
  global $user;
  $employee = $form_state['build_info']['args'][0];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    db_insert('hr_employee_report_to')
        ->fields(array(
          'org_id' => $employee['org_id'],
          'emp_id' => $employee['emp_id'],
          'supervisor_id' => $form_state['values']['supervisor'],
          'reporting_method' => $form_state['values']['reporting_method'],
          'uid' => $user->uid,
          'created' => time(),
          'changed' => time(),
        ))->execute();

    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    $deleted_ids = explode(',', $form_state['values']['delete_supervisors']);
    if (!empty($deleted_ids)) {
      foreach ($deleted_ids as $key => $value) {
        if (!is_numeric($value)) {
          unset($deleted_ids[$key]);
        }
      }

      if (!empty($deleted_ids)) {
        db_delete('hr_employee_report_to')
            ->condition('id', $deleted_ids, 'IN')
            ->execute();
      }

      drupal_set_message('Successfully Deleted');
    }
  }


  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/report-to');
}

function hr_backend_pim_employee_report_to_form_add($form, &$form_state) {
  $form_state['storage']['add'] = TRUE;
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_report_to_form_add_callback($form, $form_state) {
  return $form;
}

function hr_backend_pim_employee_report_to_form_cancel($form, &$form_state) {
  unset($form_state['storage']['add']);
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_report_to_form_cancel_callback($form, $form_state) {
  return $form;
}

/*
 * Qualifications
 */

function hr_backend_pim_employee_details_content_qualification($employee) {
  $qualifications = hr_basic_qualification_get_qualfications(array(array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '=')));

  /*
   * Education
   */
  $education = hr_backend_pim_employee_qualification_education_content($employee, $qualifications);
  $experience = hr_backend_pim_employee_qualification_experience_content($employee);
  $skills = hr_backend_pim_employee_qualification_skills_content($employee, $qualifications);
  $language = hr_backend_pim_employee_qualification_language_content($employee, $qualifications);
  $license = hr_backend_pim_employee_qualification_license_content($employee, $qualifications);

  $variables = array(
    'education' => $education,
    'experience' => $experience,
    'skills' => $skills,
    'language' => $language,
    'license' => $license
  );
  return $variables;
}

//Experince
function hr_backend_pim_employee_qualification_experience_content($employee) {
  $experience_form = drupal_get_form('hr_backend_pim_employee_qualification_experience_form', $employee);

  $manage_qualifications = user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS);
  $rows = array();
  foreach ($employee['experience'] as $row) {
    $checkbox = '<label class="css-input css-checkbox css-checkbox-primary">
          <input type="checkbox" value="' . $row['id'] . '" id="experience_' . $row['id'] . '" name="experience_' . $row['id'] . '"><span></span>
        </label>';
    $actions = array('<div class="btn-group btn-group-sm">');
    $actions[] = l('<i class="fa fa-search"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_EXPERIENCE_DETAILS, 'emp_id' => $employee['emp_id'], 'experiance_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-info',)), 'html' => TRUE));
    if ($manage_qualifications) {
      $actions[] = l('<i class="fa fa-pencil"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_EXPERIENCE_EDIT, 'emp_id' => $employee['emp_id'], 'experiance_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-warning',)), 'html' => TRUE));
    }
    $actions[] = '</div>';
    $from_to = '';
    $duration = '';
    if ($row['from_date'] && $row['to_date']) {
      $from_to = date(HR_DATETIME_FORMAT_MONTH_NAME_YEAR_4_DIGITS, $row['from_date']) . '<br />' . date(HR_DATETIME_FORMAT_MONTH_NAME_YEAR_4_DIGITS, $row['to_date']);
      $duration = hr_basic_date_dates_diff_years_months($row['from_date'], $row['to_date']);
    }
    $rows[]['data'] = array(
      'checkbox' => array('data' => $checkbox, 'class' => array('text-center')),
      'company' => array('data' => $row['company']),
      'job_title' => array('data' => $row['job_title']),
      'from_date' => array('data' => $from_to, 'class' => array('nowrap')),
      'duration' => array('data' => $duration),
      'actions' => array('data' => implode(' ', $actions), 'class' => array('text-center')),
    );
  }
  $disabled = '';
  if (empty($rows)) {
    $disabled = 'disabled="disabled"';
  }
  $header_checkbox = '<label class="css-input css-checkbox css-checkbox-primary remove-margin-t remove-margin-b">
    <input type="checkbox" id="check-all-experience" name="check-all-experience" ' . $disabled . '><span></span>
    </label>';
  $header = array(
    array('data' => $header_checkbox, 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Comapny'),
    array('data' => 'Job Title'),
    array('data' => 'From - To'),
    array('data' => 'Duration'),
    array('data' => 'Actions', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
  );

  $list = array(
    'attributes' => array('class' => array('employee-experience-list', 'table', 'js-table-checkable', 'table-condensed', 'table-bordered')),
    'rows' => $rows,
    'header' => $header
  );
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }

  $content = drupal_render($experience_form);
  $content .= '<div class="table-responsive">' . theme('table', $list) . '</div>';

  $variables = array(
    'content' => $content,
    'title' => 'Work Experience',
    'id' => 'qualification-experience-wrapper',
  );

  return $variables;
}

function hr_backend_pim_employee_qualification_experience_details($emp_id, $experience_id) {
  $experience = db_select('hr_employee_experience', 'employee_experience')
          ->fields('employee_experience', array('id', 'emp_id', 'org_id', 'company', 'job_title',
            'from_date', 'to_date', 'attachment', 'comment', 'uid', 'created', 'changed'))
          ->condition('id', $experience_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);
  if (empty($experience)) {
    hr_backend_modal_not_found();
  }
  drupal_set_title('Experience: ' . $experience['company'] . ' - ' . $experience['job_title']);

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Company', 'value_class' => array(),
    'value' => $experience['company'], 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Job title', 'value_class' => array(),
    'value' => $experience['job_title'], 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'From', 'value_class' => array(),
    'value' => (($experience['from_date']) ? date(HR_DATETIME_FORMAT_MONTH_NAME_YEAR_4_DIGITS, $experience['from_date']) : ''), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'To', 'value_class' => array(),
    'value' => (($experience['to_date']) ? date(HR_DATETIME_FORMAT_MONTH_NAME_YEAR_4_DIGITS, $experience['to_date']) : ''), 'value_class' => array());
  $duration = '';
  if ($experience['from_date'] && $experience['to_date']) {
    $duration = hr_basic_date_dates_diff_years_months($experience['from_date'], $experience['to_date']);
  }
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Duration', 'value_class' => array(),
    'value' => $duration, 'value_class' => array());

  $attachment = '';
  if ($experience['attachment']) {
    $file = file_load($experience['attachment']);
    if ($file) {
      $attachment_url = file_create_url($file->uri);
      $attachment = l($file->filename, $attachment_url, array('attributes' => array('target' => '_blank')));
    }
  }
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Attachment', 'value_class' => array(),
    'value' => $attachment, 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Comment', 'value_class' => array(),
    'value' => check_markup($experience['comment'], 'filtered_html'), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Added by', 'value_class' => array(),
    'value' => hr_basic_account_get_name($experience['uid']), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Created', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $experience['created']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Updated', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $experience['changed']), 'value_class' => array());

  return theme('section_entity_details', array('fields' => $fields));
}

function hr_backend_pim_employee_qualification_experience_form($form, &$form_state, $employee) {
  $access = FALSE;
  if (isset($form_state['storage']['add'])) {
    $access = TRUE;
  }

  $form['company'] = array('#title' => t('Company'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#access' => $access,);

  $form['job_title'] = array('#title' => t('Job Title'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#access' => $access,);
  $form['from_date'] = array('#title' => t('From date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#access' => $access,
  );
  $form['to_date'] = array('#title' => t('To date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#access' => $access,
  );
  $attachment_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $employee['org_id'] . '/employees/' . $employee['emp_id'] . '/qualification/experience';
  file_prepare_directory($attachment_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['attachment'] = array('#title' => t('Attachment'), '#type' => 'managed_file', '#required' => FALSE,
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf ppt pps'),),
    '#upload_location' => $attachment_upload_location,
    '#access' => $access,);
  $form['comment'] = array('#title' => t('Comment'), '#type' => 'textarea', '#required' => FALSE, '#maxlength' => 400,
    '#access' => $access, '#rows' => 3, '#resizable' => FALSE);


  $form['delete_experience'] = array('#type' => 'hidden', '#default_value' => '', '#attributes' => array('id' => 'delete-experience-ids'));

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS)) {
    if (!$access) {
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_add'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_add_callback',
          'wrapper' => 'employee-qualification-experience-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array(
          'class' => array('btn-danger', 'delete-experience'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_cancel'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_cancel_callback',
          'wrapper' => 'employee-qualification-experience-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }

  $form['#prefix'] = '<div id="employee-qualification-experience-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-experience-form';

  return $form;
}

function hr_backend_pim_employee_qualification_experience_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    if (empty($form_state['values']['delete_experience'])) {
      form_set_error('delete_experience', t('At least select one experience to be deleted.'));
    }
  }
}

function hr_backend_pim_employee_qualification_experience_form_submit(&$form, &$form_state) {
  global $user;
  $employee = $form_state['build_info']['args'][0];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $from_date = 0;
    if (!empty($form_state['values']['from_date'])) {
      $from_date = strtotime(str_replace('/', '-', $form_state['values']['from_date']));
    }

    $to_date = 0;
    if (!empty($form_state['values']['to_date'])) {
      $to_date = strtotime(str_replace('/', '-', $form_state['values']['to_date']));
    }

    $attachment = 0;
    if ($form_state['values']['attachment']) {
      $attachment = $form_state['values']['attachment'];
    }

    $experience_id = db_insert('hr_employee_experience')
            ->fields(array(
              'emp_id' => $employee['emp_id'],
              'org_id' => $employee['org_id'],
              'company' => $form_state['values']['company'],
              'job_title' => $form_state['values']['job_title'],
              'from_date' => $from_date,
              'to_date' => $to_date,
              'attachment' => $attachment,
              'comment' => $form_state['values']['comment'],
              'uid' => $user->uid,
              'created' => time(),
              'changed' => time(),
            ))->execute();

    if ($attachment) {
      hr_basic_file_save_file_data($attachment, 'hr_backend', 'employee_qualification_experience', $experience_id);
    }

    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    $deleted_ids = explode(',', $form_state['values']['delete_experience']);
    if (!empty($deleted_ids)) {
      foreach ($deleted_ids as $key => $value) {
        if (!is_numeric($value)) {
          unset($deleted_ids[$key]);
        }
      }

      if (!empty($deleted_ids)) {
        $attachments = db_select('hr_employee_experience')
                ->fields('hr_employee_experience', array('attachment'))
                ->condition('id', $deleted_ids, 'IN')
                ->execute()->fetchCol(0);
        if (!empty($attachments)) {
          foreach ($attachments as $attachment) {
            if ($attachment) {
              hr_basic_file_delete_file_data($attachment, 'hr_backend');
            }
          }
        }

        db_delete('hr_employee_experience')
            ->condition('id', $deleted_ids, 'IN')
            ->execute();
      }

      drupal_set_message('Successfully Deleted');
    }
  }

  $redirect_options = array(
    'query' => array('anchor' => 'qualification-experience-wrapper'),
  );
  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/qualifications', $redirect_options);
}

function hr_backend_pim_employee_qualification_experience_edit_form($form, &$form_state, $emp_id, $experience_id) {
  form_load_include($form_state, 'inc', 'hr_backend', 'includes/backend_pim');

  $experience = db_select('hr_employee_experience', 'employee_experience')
          ->fields('employee_experience', array('id', 'emp_id', 'org_id', 'company', 'job_title',
            'from_date', 'to_date', 'attachment', 'comment', 'uid', 'created', 'changed'))
          ->condition('id', $experience_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);

  if (empty($experience)) {
    hr_backend_modal_not_found();
  }

  $form_state['build_info']['args'][2] = $experience;

  drupal_set_title('Experience: ' . $experience['company'] . ' - ' . $experience['job_title']);

  $form['company'] = array('#title' => t('Company'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => $experience['company'],);

  $form['job_title'] = array('#title' => t('Job Title'), '#type' => 'textfield', '#required' => TRUE, '#maxlength' => 255,
    '#default_value' => $experience['job_title'],);


  $from_date = (($experience['from_date']) ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $experience['from_date']) : '');
  $form['from_date'] = array('#title' => t('From date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => $from_date,
  );

  $to_data = (($experience['to_date']) ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $experience['to_date']) : '');
  $form['to_date'] = array('#title' => t('To date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => $to_data,
  );

  $attachment_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $experience['org_id'] . '/employees/' . $emp_id . '/qualification/experience';
  file_prepare_directory($attachment_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['attachment'] = array('#title' => t('Attachment'), '#type' => 'managed_file', '#required' => FALSE,
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf ppt pps'),),
    '#upload_location' => $attachment_upload_location,
    '#default_value' => $experience['attachment'],);

  $form['comment'] = array('#title' => t('Comment'), '#type' => 'textarea', '#required' => FALSE, '#maxlength' => 400,
    '#default_value' => $experience['comment'], '#rows' => 3, '#resizable' => FALSE);


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => array('btn-info'))
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#attributes' => array('class' => array('btn-danger'))
  );

  $form['#prefix'] = '<div id="employee-qualification-experience-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-experience-form';

  return $form;
}

function hr_backend_pim_employee_qualification_experience_edit_form_validate(&$form, &$form_state) {
  
}

function hr_backend_pim_employee_qualification_experience_edit_form_submit(&$form, &$form_state) {
  global $user;
  $emp_id = $form_state['build_info']['args'][0];
  $experience_id = $form_state['build_info']['args'][1];
  $experience = $form_state['build_info']['args'][2];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $from_date = 0;
    if (!empty($form_state['values']['from_date'])) {
      $from_date = strtotime(str_replace('/', '-', $form_state['values']['from_date']));
    }

    $to_date = 0;
    if (!empty($form_state['values']['to_date'])) {
      $to_date = strtotime(str_replace('/', '-', $form_state['values']['to_date']));
    }

    $attachment = 0;
    if ($form_state['values']['attachment']) {
      $attachment = $form_state['values']['attachment'];
    }

    db_update('hr_employee_experience')
        ->fields(array(
          'company' => $form_state['values']['company'],
          'job_title' => $form_state['values']['job_title'],
          'from_date' => $from_date,
          'to_date' => $to_date,
          'attachment' => $attachment,
          'comment' => $form_state['values']['comment'],
          'changed' => time(),
        ))
        ->condition('id', $experience_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();

    if ($attachment) {
      if ($attachment != $experience['attachment']) {
        hr_basic_file_save_file_data($attachment, 'hr_backend', 'employee_qualification_experience', $experience_id);
        if ($experience['attachment']) {
          hr_basic_file_delete_file_data($attachment, 'hr_backend');
        }
      }
    }
    else {
      if ($experience['attachment']) {
        hr_basic_file_delete_file_data($experience['attachment'], 'hr_backend');
      }
    }

    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    db_delete('hr_employee_experience')
        ->condition('id', $experience_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();
    if ($experience['attachment']) {
      hr_basic_file_delete_file_data($experience['attachment'], 'hr_backend');
    }
    drupal_set_message('Successfully Deleted');
  }

  $redirect_options = array(
    'query' => array('anchor' => 'qualification-experience-wrapper'),
  );
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $emp_id . '/qualifications', 0, $redirect_options);
  print ajax_render($commands);
  exit;
}

//Education
function hr_backend_pim_employee_qualification_education_content($employee, $qualifications) {
  $education_form = drupal_get_form('hr_backend_pim_employee_qualification_education_form', $employee);

  $manage_qualifications = user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS);

  $rows = array();
  foreach ($employee['education'] as $row) {
    $checkbox = '<label class="css-input css-checkbox css-checkbox-primary">
          <input type="checkbox" value="' . $row['id'] . '" id="education_' . $row['id'] . '" name="education_' . $row['id'] . '"><span></span>
        </label>';

    $actions = array('<div class="btn-group btn-group-sm">');
    $actions[] = l('<i class="fa fa-search"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_EDUCATION_DETAILS, 'emp_id' => $employee['emp_id'], 'education_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-info',)), 'html' => TRUE));
    if ($manage_qualifications) {
      $actions[] = l('<i class="fa fa-pencil"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_EDUCATION_EDIT, 'emp_id' => $employee['emp_id'], 'education_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-warning',)), 'html' => TRUE));
    }
    $actions[] = '</div>';

    $rows[]['data'] = array(
      'checkbox' => array('data' => $checkbox, 'class' => array('text-center')),
      'level' => array('data' => $qualifications[$row['qualification_id']]),
      'institute' => array('data' => $row['institute']),
      'major' => array('data' => $row['major']),
      'year' => array('data' => ($row['year'] ? $row['year'] : '')),
      'score' => array('data' => $row['score']),
      'actions' => array('data' => implode('', $actions), 'class' => array('text-center')),
    );
  }

  $disabled = '';
  if (empty($rows)) {
    $disabled = 'disabled="disabled"';
  }
  $header_checkbox = '<label class="css-input css-checkbox css-checkbox-primary remove-margin-t remove-margin-b">
    <input type="checkbox" id="check-all-education" name="check-all-education" ' . $disabled . '><span></span>
    </label>';
  $header = array(
    array('data' => $header_checkbox, 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Level'),
    array('data' => 'Institute'),
    array('data' => 'Major'),
    array('data' => 'Year'),
    array('data' => 'Score'),
    array('data' => 'Actions', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
  );

  $list = array(
    'attributes' => array('class' => array('employee-education-list', 'table', 'js-table-checkable', 'table-condensed', 'table-bordered')),
    'rows' => $rows,
    'header' => $header
  );
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }

  $content = drupal_render($education_form);
  $content .= '<div class="table-responsive">' . theme('table', $list) . '</div>';

  $variables = array(
    'content' => $content,
    'title' => 'Education',
    'id' => 'qualification-education-wrapper',
  );

  return $variables;
}

function hr_backend_pim_employee_qualification_education_details($emp_id, $education_id) {
  $education = db_select('hr_employee_education', 'employee_education')
          ->fields('employee_education', array('id', 'emp_id', 'org_id', 'qualification_id', 'institute',
            'major', 'year', 'score', 'start_date', 'end_date', 'attachment', 'uid', 'created', 'changed'))
          ->condition('id', $education_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);
  if (empty($education)) {
    hr_backend_modal_not_found();
  }

  $level = hr_basic_qualification_get_name($education['qualification_id']);
  drupal_set_title('Education: ' . $level);

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Level', 'value_class' => array(),
    'value' => $level, 'value _class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Institute', 'value_class' => array(),
    'value' => $education['institute'], 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Major', 'value_class' => array(),
    'value' => $education['major'], 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Year', 'value_class' => array(),
    'value' => $education['year'], 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'GPA/Score', 'value_class' => array(),
    'value' => $education['score'], 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Start date', 'value_class' => array(),
    'value' => (($education['start_date']) ? date(HR_DATETIME_FORMAT_MONTH_NAME_YEAR_4_DIGITS, $education['start_date']) : ''), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'End date', 'value_class' => array(),
    'value' => (($education['end_date']) ? date(HR_DATETIME_FORMAT_MONTH_NAME_YEAR_4_DIGITS, $education['end_date']) : ''), 'value_class' => array());

  $attachment = '';
  if ($education['attachment']) {
    $file = file_load($education['attachment']);
    if ($file) {
      $attachment_url = file_create_url($file->uri);
      $attachment = l($file->filename, $attachment_url, array('attributes' => array('target' => '_blank')));
    }
  }
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Attachment', 'value_class' => array(),
    'value' => $attachment, 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Added by', 'value_class' => array(),
    'value' => hr_basic_account_get_name($education['uid']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Created', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $education['created']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Updated', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $education['changed']), 'value_class' => array());

  return theme('section_entity_details', array('fields' => $fields));
}

function hr_backend_pim_employee_qualification_education_form($form, &$form_state, $employee) {
  $access = FALSE;
  if (isset($form_state['storage']['add'])) {
    $access = TRUE;
  }

  if (!isset($form_state['storage']['qualifications'])) {
    $qualifications_filters = array(
      array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='),
      array('field' => 'type', 'value' => HR_QUALIFICATION_EDUCATION, 'operator' => '='),
    );
    $form_state['storage']['qualifications'] = hr_basic_qualification_get_qualfications($qualifications_filters);
  }

  $form['qualification'] = array('#title' => t('Level'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['qualifications'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Level'),
    '#access' => $access,);

  $form['institute'] = array('#title' => t('Institute'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#access' => $access,);

  $form['major'] = array('#title' => t('Major/Specialization'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#access' => $access,);

  $form['year'] = array('#title' => t('Year'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#element_validate' => array('element_validate_integer_positive'),
    '#access' => $access,);

  $form['score'] = array('#title' => t('GPA/Score'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#access' => $access,);

  $form['start_date'] = array('#title' => t('Start date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#access' => $access,
  );

  $form['end_date'] = array('#title' => t('End date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#access' => $access,
  );

  $attachment_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $employee['org_id'] . '/employees/' . $employee['emp_id'] . '/qualification/education';
  file_prepare_directory($attachment_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['attachment'] = array('#title' => t('Attachment'), '#type' => 'managed_file', '#required' => FALSE,
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf ppt pps'),),
    '#upload_location' => $attachment_upload_location,
    '#access' => $access,);

  $form['delete_education'] = array('#type' => 'hidden', '#default_value' => '', '#attributes' => array('id' => 'delete-education-ids'));

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS)) {
    if (!$access) {
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_add'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_add_callback',
          'wrapper' => 'employee-qualification-education-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array(
          'class' => array('btn-danger', 'delete-education'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_cancel'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_cancel_callback',
          'wrapper' => 'employee-qualification-education-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }

  $form['#prefix'] = '<div id="employee-qualification-education-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-education-form';

  return $form;
}

function hr_backend_pim_employee_qualification_education_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    if (empty($form_state['values']['delete_education'])) {
      form_set_error('delete_education', t('At least select one education to be deleted.'));
    }
  }
}

function hr_backend_pim_employee_qualification_education_form_submit(&$form, &$form_state) {
  global $user;
  $employee = $form_state['build_info']['args'][0];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $year = 0;
    if (!empty($form_state['values']['year'])) {
      $year = $form_state['values']['year'];
    }

    $start_date = 0;
    if (!empty($form_state['values']['start_date'])) {
      $start_date = strtotime(str_replace('/', '-', $form_state['values']['start_date']));
    }

    $end_date = 0;
    if (!empty($form_state['values']['end_date'])) {
      $end_date = strtotime(str_replace('/', '-', $form_state['values']['end_date']));
    }

    $attachment = 0;
    if ($form_state['values']['attachment']) {
      $attachment = $form_state['values']['attachment'];
    }

    $education_id = db_insert('hr_employee_education')
            ->fields(array(
              'emp_id' => $employee['emp_id'],
              'org_id' => $employee['org_id'],
              'qualification_id' => $form_state['values']['qualification'],
              'institute' => $form_state['values']['institute'],
              'major' => $form_state['values']['major'],
              'year' => $year,
              'score' => $form_state['values']['score'],
              'start_date' => $start_date,
              'end_date' => $end_date,
              'attachment' => $attachment,
              'uid' => $user->uid,
              'created' => time(),
              'changed' => time(),
            ))->execute();

    if ($attachment) {
      hr_basic_file_save_file_data($attachment, 'hr_backend', HR_FILE_USAGE_QUALIFICATION_EDUCATION_ATTACHMENT, $education_id);
    }

    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    $deleted_ids = explode(',', $form_state['values']['delete_education']);
    if (!empty($deleted_ids)) {
      foreach ($deleted_ids as $key => $value) {
        if (!is_numeric($value)) {
          unset($deleted_ids[$key]);
        }
      }

      if (!empty($deleted_ids)) {
        db_delete('hr_employee_education')
            ->condition('id', $deleted_ids, 'IN')
            ->execute();
      }

      drupal_set_message('Successfully Deleted');
    }
  }

  $redirect_options = array(
    'query' => array('anchor' => 'qualification-education-wrapper'),
  );
  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/qualifications', $redirect_options);
}

function hr_backend_pim_employee_qualification_education_edit_form($form, &$form_state, $emp_id, $education_id) {
  form_load_include($form_state, 'inc', 'hr_backend', 'includes/backend_pim');

  $education = db_select('hr_employee_education', 'employee_education')
          ->fields('employee_education', array('id', 'emp_id', 'org_id', 'qualification_id', 'institute',
            'major', 'year', 'score', 'start_date', 'end_date', 'attachment', 'uid', 'created', 'changed'))
          ->condition('id', $education_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);

  if (empty($education)) {
    hr_backend_modal_not_found();
  }

  $form_state['build_info']['args'][2] = $education;

  if (!isset($form_state['storage']['qualifications'])) {
    $qualifications_filters = array(
      array('field' => 'org_id', 'value' => $education['org_id'], 'operator' => '='),
      array('field' => 'type', 'value' => HR_QUALIFICATION_EDUCATION, 'operator' => '='),
    );
    $form_state['storage']['qualifications'] = hr_basic_qualification_get_qualfications($qualifications_filters);
  }

  drupal_set_title('Education: ' . $form_state['storage']['qualifications'][$education['qualification_id']]);


  $form['qualification'] = array('#title' => t('Level'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['qualifications'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Level'),
    '#default_value' => $education['qualification_id'],);

  $form['institute'] = array('#title' => t('Institute'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => $education['institute'],);

  $form['major'] = array('#title' => t('Major/Specialization'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#default_value' => $education['major'],);

  $form['year'] = array('#title' => t('Year'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $education['year'],);

  $form['score'] = array('#title' => t('GPA/Score'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#default_value' => $education['score'],);

  $form['start_date'] = array('#title' => t('Start date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => ($education['start_date'] ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $education['start_date']) : ''),
  );

  $form['end_date'] = array('#title' => t('End date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => ($education['end_date'] ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $education['end_date']) : ''),
  );

  $attachment_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $education['org_id'] . '/employees/' . $emp_id . '/qualification/education';
  file_prepare_directory($attachment_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['attachment'] = array('#title' => t('Attachment'), '#type' => 'managed_file', '#required' => FALSE,
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf ppt pps'),),
    '#upload_location' => $attachment_upload_location,
    '#default_value' => $education['attachment'],);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => array('btn-success'))
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#attributes' => array('class' => array('btn-danger'))
  );

  $form['#prefix'] = '<div id="employee-qualification-education-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-education-form';

  return $form;
}

function hr_backend_pim_employee_qualification_education_edit_form_validate(&$form, &$form_state) {
  
}

function hr_backend_pim_employee_qualification_education_edit_form_submit(&$form, &$form_state) {
  $emp_id = $form_state['build_info']['args'][0];
  $education_id = $form_state['build_info']['args'][1];
  $education = $form_state['build_info']['args'][2];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $year = 0;
    if (!empty($form_state['values']['year'])) {
      $year = $form_state['values']['year'];
    }

    $start_date = 0;
    if (!empty($form_state['values']['start_date'])) {
      $start_date = strtotime(str_replace('/', '-', $form_state['values']['start_date']));
    }

    $end_date = 0;
    if (!empty($form_state['values']['end_date'])) {
      $end_date = strtotime(str_replace('/', '-', $form_state['values']['end_date']));
    }

    $attachment = 0;
    if ($form_state['values']['attachment']) {
      $attachment = $form_state['values']['attachment'];
    }

    db_update('hr_employee_education')
        ->fields(array(
          'qualification_id' => $form_state['values']['qualification'],
          'institute' => $form_state['values']['institute'],
          'major' => $form_state['values']['major'],
          'year' => $year,
          'score' => $form_state['values']['score'],
          'start_date' => $start_date,
          'end_date' => $end_date,
          'attachment' => $attachment,
          'changed' => time(),
        ))
        ->condition('id', $education_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();

    if ($attachment) {
      if ($attachment != $education['attachment']) {
        hr_basic_file_save_file_data($attachment, 'hr_backend', HR_FILE_USAGE_QUALIFICATION_EDUCATION_ATTACHMENT, $education_id);
        if ($education['attachment']) {
          hr_basic_file_delete_file_data($attachment, 'hr_backend');
        }
      }
    }
    else {
      if ($education['attachment']) {
        hr_basic_file_delete_file_data($education['attachment'], 'hr_backend');
      }
    }

    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    db_delete('hr_employee_education')
        ->condition('id', $education_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();
    if ($education['attachment']) {
      hr_basic_file_delete_file_data($education['attachment'], 'hr_backend');
    }
    drupal_set_message('Successfully Deleted');
  }


  $redirect_options = array(
    'query' => array('anchor' => 'qualification-education-wrapper'),
  );
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $emp_id . '/qualifications', 0, $redirect_options);
  print ajax_render($commands);
  exit;
}

//Skills
function hr_backend_pim_employee_qualification_skills_content($employee, $qualifications) {
  $skills_form = drupal_get_form('hr_backend_pim_employee_qualification_skills_form', $employee);

  $manage_qualifications = user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS);

  $rows = array();
  foreach ($employee['skills'] as $row) {
    $checkbox = '<label class="css-input css-checkbox css-checkbox-primary">
          <input type="checkbox" value="' . $row['id'] . '" id="skills_' . $row['id'] . '" name="skills_' . $row['id'] . '"><span></span>
        </label>';

    $actions = array('<div class="btn-group btn-group-sm">');
    $actions[] = l('<i class="fa fa-search"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_SKILLS_EDIT, 'emp_id' => $employee['emp_id'], 'skill_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-info',)), 'html' => TRUE));
    if ($manage_qualifications) {
      $actions[] = l('<i class="fa fa-pencil"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_SKILLS_DETAILS, 'emp_id' => $employee['emp_id'], 'skill_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-warning',)), 'html' => TRUE));
    }
    $actions[] = '</div>';

    $rows[]['data'] = array(
      'checkbox' => array('data' => $checkbox, 'class' => array('text-center')),
      'skills' => array('data' => $qualifications[$row['qualification_id']]),
      'years' => array('data' => $row['years']),
      'actions' => array('data' => implode('', $actions), 'class' => array('text-center')),
    );
  }

  $disabled = '';
  if (empty($rows)) {
    $disabled = 'disabled="disabled"';
  }
  $header_checkbox = '<label class="css-input css-checkbox css-checkbox-primary remove-margin-t remove-margin-b">
    <input type="checkbox" id="check-all-skills" name="check-all-skills" ' . $disabled . '><span></span>
    </label>';
  $header = array(
    array('data' => $header_checkbox, 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Skill'),
    array('data' => 'Years of Experience'),
    array('data' => 'Actions', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
  );

  $list = array(
    'attributes' => array('class' => array('employee-skills-list', 'table', 'table-bordered', 'table-striped', 'js-table-checkable')),
    'rows' => $rows,
    'header' => $header
  );
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }

  $content = drupal_render($skills_form);
  $content .= '<div class="table-responsive">' . theme('table', $list) . '</div>';

  $variables = array(
    'content' => $content,
    'title' => 'Skills',
    'id' => 'qualification-skills-wrapper',
  );

  return $variables;
}

function hr_backend_pim_employee_qualification_skills_details($emp_id, $skill_id) {
  $skill = db_select('hr_employee_skills', 'employee_skills')
          ->fields('employee_skills', array('id', 'emp_id', 'org_id', 'qualification_id', 'years',
            'comment', 'uid', 'created', 'changed'))
          ->condition('id', $skill_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);
  if (empty($skill)) {
    hr_backend_modal_not_found();
  }

  $skill_name = hr_basic_qualification_get_name($skill['qualification_id']);
  drupal_set_title('Skills: ' . $skill_name);

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Level', 'value_class' => array(),
    'value' => $skill_name, 'value _class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Years of Experience', 'value_class' => array(),
    'value' => $skill['years'], 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Comment', 'value_class' => array(),
    'value' => check_markup($skill['comment'], 'filtered_html'), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Added by', 'value_class' => array(),
    'value' => hr_basic_account_get_name($skill['uid']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Created', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $skill['created']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Updated', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $skill['changed']), 'value_class' => array());

  return theme('section_entity_details', array('fields' => $fields));
}

function hr_backend_pim_employee_qualification_skills_form($form, &$form_state, $employee) {
  $access = FALSE;
  if (isset($form_state['storage']['add'])) {
    $access = TRUE;
  }

  if (!isset($form_state['storage']['qualifications'])) {
    $qualifications_filters = array(
      array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='),
      array('field' => 'type', 'value' => HR_QUALIFICATION_SKILLS, 'operator' => '='),
    );
    $form_state['storage']['qualifications'] = hr_basic_qualification_get_qualfications($qualifications_filters);
  }

  $form['qualification'] = array('#title' => t('Skill'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['qualifications'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Skill'),
    '#access' => $access,);

  $form['years'] = array('#title' => t('Years of Experience'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#element_validate' => array('element_validate_integer_positive'),
    '#access' => $access,);

  $form['comment'] = array('#title' => t('Comment'), '#type' => 'textarea', '#required' => FALSE, '#maxlength' => 400,
    '#access' => $access, '#rows' => 3, '#resizable' => FALSE);

  $form['delete_skills'] = array('#type' => 'hidden', '#default_value' => '', '#attributes' => array('id' => 'delete-skills-ids'));

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS)) {
    if (!$access) {
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_add'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_add_callback',
          'wrapper' => 'employee-qualification-skills-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array(
          'class' => array('btn-danger', 'delete-skills'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_cancel'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_cancel_callback',
          'wrapper' => 'employee-qualification-skills-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }

  $form['#prefix'] = '<div id="employee-qualification-skills-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-skills-form';

  return $form;
}

function hr_backend_pim_employee_qualification_skills_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    if (empty($form_state['values']['delete_skills'])) {
      form_set_error('delete_skills', t('At least select one skill to be deleted.'));
    }
  }
}

function hr_backend_pim_employee_qualification_skills_form_submit(&$form, &$form_state) {
  global $user;
  $employee = $form_state['build_info']['args'][0];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $years = 0;
    if (!empty($form_state['values']['years'])) {
      $years = $form_state['values']['years'];
    }

    db_insert('hr_employee_skills')
        ->fields(array(
          'emp_id' => $employee['emp_id'],
          'org_id' => $employee['org_id'],
          'qualification_id' => $form_state['values']['qualification'],
          'years' => $years,
          'comment' => $form_state['values']['comment'],
          'uid' => $user->uid,
          'created' => time(),
          'changed' => time(),
        ))->execute();

    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    $deleted_ids = explode(',', $form_state['values']['delete_skills']);
    if (!empty($deleted_ids)) {
      foreach ($deleted_ids as $key => $value) {
        if (!is_numeric($value)) {
          unset($deleted_ids[$key]);
        }
      }

      if (!empty($deleted_ids)) {
        db_delete('hr_employee_skills')
            ->condition('id', $deleted_ids, 'IN')
            ->execute();
      }

      drupal_set_message('Successfully Deleted');
    }
  }

  $redirect_options = array(
    'query' => array('anchor' => 'qualification-skills-wrapper'),
  );
  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/qualifications', $redirect_options);
}

function hr_backend_pim_employee_qualification_skills_edit_form($form, &$form_state, $emp_id, $skill_id) {
  $skill = db_select('hr_employee_skills', 'employee_skills')
          ->fields('employee_skills', array('id', 'emp_id', 'org_id', 'qualification_id', 'years',
            'comment', 'uid', 'created', 'changed'))
          ->condition('id', $skill_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);
  if (empty($skill)) {
    hr_backend_modal_not_found();
  }
  $form_state['build_info']['args'][2] = $skill;

  $skill_name = hr_basic_qualification_get_name($skill['qualification_id']);
  drupal_set_title('Edit skill: ' . $skill_name . ' ');

  if (!isset($form_state['storage']['qualifications'])) {
    $qualifications_filters = array(
      array('field' => 'org_id', 'value' => $skill['org_id'], 'operator' => '='),
      array('field' => 'type', 'value' => HR_QUALIFICATION_SKILLS, 'operator' => '='),
    );
    $form_state['storage']['qualifications'] = hr_basic_qualification_get_qualfications($qualifications_filters);
  }

  $form['qualification'] = array('#title' => t('Skill'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['qualifications'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Skill'),
    '#default_value' => $skill['qualification_id'],);

  $form['years'] = array('#title' => t('Years of Experience'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $skill['years'],);

  $form['comment'] = array('#title' => t('Comment'), '#type' => 'textarea', '#required' => FALSE, '#maxlength' => 400,
    '#default_value' => $skill['comment'], '#rows' => 3, '#resizable' => FALSE);

  //Form Actions
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => array('btn-success'))
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#attributes' => array('class' => array('btn-danger'))
  );

  $form['#prefix'] = '<div id="employee-qualification-skills-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-skills-form';

  return $form;
}

function hr_backend_pim_employee_qualification_skills_edit_form_validate(&$form, &$form_state) {
  
}

function hr_backend_pim_employee_qualification_skills_edit_form_submit(&$form, &$form_state) {
  global $user;
  $emp_id = $form_state['build_info']['args'][0];
  $skill_id = $form_state['build_info']['args'][1];
  $skill = $form_state['build_info']['args'][2];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $years = 0;
    if (!empty($form_state['values']['years'])) {
      $years = $form_state['values']['years'];
    }

    db_update('hr_employee_skills')
        ->fields(array(
          'qualification_id' => $form_state['values']['qualification'],
          'years' => $years,
          'comment' => $form_state['values']['comment'],
          'changed' => time(),
        ))
        ->condition('id', $skill_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();

    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    db_delete('hr_employee_skills')
        ->condition('id', $skill_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();
    if ($skill['attachment']) {
      hr_basic_file_delete_file_data($skill['attachment'], 'hr_backend');
    }
    drupal_set_message('Successfully Deleted');
  }

  $redirect_options = array(
    'query' => array('anchor' => 'qualification-skills-wrapper'),
  );
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $emp_id . '/qualifications', 0, $redirect_options);
  print ajax_render($commands);
  exit;
}

//Language
function hr_backend_pim_employee_qualification_language_content($employee, $qualifications) {
  $language_form = drupal_get_form('hr_backend_pim_employee_qualification_language_form', $employee);
  $language_fluency_levels = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_LANGUAGE_FLUENCY_LEVEL);
  $language_fluency_levels[0] = '';

  $manage_qualifications = user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS);

  $rows = array();
  foreach ($employee['language'] as $row) {
    $checkbox = '<label class="css-input css-checkbox css-checkbox-primary">
          <input type="checkbox" value="' . $row['id'] . '" id="language_' . $row['id'] . '" name="language_' . $row['id'] . '"><span></span>
        </label>';

    $actions = array('<div class="btn-group btn-group-sm">');
    $actions[] = l('<i class="fa fa-search"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_LANGUAGE_DETAILS, 'emp_id' => $employee['emp_id'], 'language_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-info',)), 'html' => TRUE));
    if ($manage_qualifications) {
      $actions[] = l('<i class="fa fa-pencil"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_LANGUAGE_EDIT, 'emp_id' => $employee['emp_id'], 'language_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-warning',)), 'html' => TRUE));
    }
    $actions[] = '</div>';

    $rows[]['data'] = array(
      'checkbox' => array('data' => $checkbox, 'class' => array('text-center')),
      'language' => array('data' => $qualifications[$row['qualification_id']]),
      'reading' => array('data' => (isset($language_fluency_levels[$row['reading']]) ? $language_fluency_levels[$row['reading']] : '')),
      'writing' => array('data' => (isset($language_fluency_levels[$row['writing']]) ? $language_fluency_levels[$row['writing']] : '')),
      'speaking' => array('data' => (isset($language_fluency_levels[$row['speaking']]) ? $language_fluency_levels[$row['speaking']] : '')),
      'actions' => array('data' => implode('', $actions), 'class' => array('text-center')),
    );
  }

  $disabled = '';
  if (empty($rows)) {
    $disabled = 'disabled="disabled"';
  }
  $header_checkbox = '<label class="css-input css-checkbox css-checkbox-primary remove-margin-t remove-margin-b">
    <input type="checkbox" id="check-all-language" name="check-all-language" ' . $disabled . '><span></span>
    </label>';
  $header = array(
    array('data' => $header_checkbox, 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'Language'),
    array('data' => 'Reading'),
    array('data' => 'Writing'),
    array('data' => 'Speaking'),
    array('data' => 'Actions', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
  );

  $list = array(
    'attributes' => array('class' => array('employee-language-list', 'table', 'table-bordered', 'table-striped', 'js-table-checkable')),
    'rows' => $rows,
    'header' => $header
  );
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }

  $content = drupal_render($language_form);
  $content .= '<div class="table-responsive">' . theme('table', $list) . '</div>';

  $variables = array(
    'content' => $content,
    'title' => 'Languages',
    'id' => 'qualification-language-wrapper',
  );

  return $variables;
}

function hr_backend_pim_employee_qualification_language_details($emp_id, $language_id) {
  $language = db_select('hr_employee_language', 'employee_language')
          ->fields('employee_language', array('id', 'emp_id', 'org_id', 'qualification_id', 'reading', 'writing', 'speaking',
            'comment', 'uid', 'created', 'changed'))
          ->condition('id', $language_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);
  if (empty($language)) {
    hr_backend_modal_not_found();
  }
  $language_fluency_levels = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_LANGUAGE_FLUENCY_LEVEL);

  $language_name = hr_basic_qualification_get_name($language['qualification_id']);
  drupal_set_title('Language: ' . $language_name);

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Language', 'value_class' => array(),
    'value' => $language_name, 'value _class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Reading', 'value_class' => array(),
    'value' => (isset($language_fluency_levels[$language['reading']]) ? $language_fluency_levels[$language['reading']] : ''), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Writing', 'value_class' => array(),
    'value' => (isset($language_fluency_levels[$language['writing']]) ? $language_fluency_levels[$language['writing']] : ''), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Speaking', 'value_class' => array(),
    'value' => (isset($language_fluency_levels[$language['speaking']]) ? $language_fluency_levels[$language['speaking']] : ''), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Comment', 'value_class' => array(),
    'value' => check_markup($language['comment'], 'filtered_html'), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Added by', 'value_class' => array(),
    'value' => hr_basic_account_get_name($language['uid']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Created', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $language['created']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Updated', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $language['changed']), 'value_class' => array());

  return theme('section_entity_details', array('fields' => $fields));
}

function hr_backend_pim_employee_qualification_language_form($form, &$form_state, $employee) {
  $access = FALSE;
  if (isset($form_state['storage']['add'])) {
    $access = TRUE;
  }

  if (!isset($form_state['storage']['qualifications'])) {
    $qualifications_filters = array(
      array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='),
      array('field' => 'type', 'value' => HR_QUALIFICATION_LANGUAGES, 'operator' => '='),
    );
    $form_state['storage']['qualifications'] = hr_basic_qualification_get_qualfications($qualifications_filters);
  }

  if (!isset($form_state['storage']['language_fluency_levels'])) {
    $form_state['storage']['language_fluency_levels'] = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_LANGUAGE_FLUENCY_LEVEL, FALSE);
  }

  $form['qualification'] = array('#title' => t('Language'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['qualifications'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Language'),
    '#access' => $access,);

  $form['reading'] = array('#title' => t('Reading'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['language_fluency_levels'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Reading level'),
    '#access' => $access,);

  $form['writing'] = array('#title' => t('Writing'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['language_fluency_levels'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Writing level'),
    '#access' => $access,);

  $form['speaking'] = array('#title' => t('Speaking'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['language_fluency_levels'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Speaking level'),
    '#access' => $access,);

  $form['comment'] = array('#title' => t('Comment'), '#type' => 'textarea', '#required' => FALSE, '#maxlength' => 400,
    '#access' => $access, '#rows' => 3, '#resizable' => FALSE);

  $form['delete_language'] = array('#type' => 'hidden', '#default_value' => '', '#attributes' => array('id' => 'delete-language-ids'));

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS)) {
    if (!$access) {
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_add'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_add_callback',
          'wrapper' => 'employee-qualification-language-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array(
          'class' => array('btn-danger', 'delete-language'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_cancel'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_cancel_callback',
          'wrapper' => 'employee-qualification-language-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }

  $form['#prefix'] = '<div id="employee-qualification-language-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-language-form';

  return $form;
}

function hr_backend_pim_employee_qualification_language_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    if (empty($form_state['values']['delete_language'])) {
      form_set_error('delete_language', t('At least select one language to be deleted.'));
    }
  }
}

function hr_backend_pim_employee_qualification_language_form_submit(&$form, &$form_state) {
  global $user;
  $employee = $form_state['build_info']['args'][0];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {

    $language_exists = db_query('SELECT COUNT(*) FROM {hr_employee_language} WHERE emp_id = :emp_id AND qualification_id = :qualification_id', array(':emp_id' => $employee['emp_id'], ':qualification_id' => $form_state['values']['qualification']))->fetchField(0);
    if ($language_exists) {
      db_update('hr_employee_language')
          ->fields(array(
            'reading' => $form_state['values']['reading'],
            'writing' => $form_state['values']['writing'],
            'speaking' => $form_state['values']['speaking'],
            'comment' => $form_state['values']['comment'],
            'changed' => time(),
          ))
          ->condition('emp_id', $employee['emp_id'], '=')
          ->condition('qualification_id', $form_state['values']['qualification'], '=')
          ->execute();
    }
    else {
      db_insert('hr_employee_language')
          ->fields(array(
            'emp_id' => $employee['emp_id'],
            'org_id' => $employee['org_id'],
            'qualification_id' => $form_state['values']['qualification'],
            'reading' => $form_state['values']['reading'],
            'writing' => $form_state['values']['writing'],
            'speaking' => $form_state['values']['speaking'],
            'comment' => $form_state['values']['comment'],
            'uid' => $user->uid,
            'created' => time(),
            'changed' => time(),
          ))->execute();
    }


    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    $deleted_ids = explode(',', $form_state['values']['delete_language']);
    if (!empty($deleted_ids)) {
      foreach ($deleted_ids as $key => $value) {
        if (!is_numeric($value)) {
          unset($deleted_ids[$key]);
        }
      }

      if (!empty($deleted_ids)) {
        db_delete('hr_employee_language')
            ->condition('id', $deleted_ids, 'IN')
            ->execute();
      }

      drupal_set_message('Successfully Deleted');
    }
  }

  $redirect_options = array(
    'query' => array('anchor' => 'qualification-language-wrapper'),
  );
  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/qualifications', $redirect_options);
}

function hr_backend_pim_employee_qualification_language_edit_form($form, &$form_state, $emp_id, $language_id) {
  $language = db_select('hr_employee_language', 'employee_language')
          ->fields('employee_language', array('id', 'emp_id', 'org_id', 'qualification_id', 'reading', 'writing', 'speaking',
            'comment', 'uid', 'created', 'changed'))
          ->condition('id', $language_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);
  if (empty($language)) {
    hr_backend_modal_not_found();
  }

  $form_state['build_info']['args'][2] = $language;

  if (!isset($form_state['storage']['qualifications'])) {
    $qualifications_filters = array(
      array('field' => 'org_id', 'value' => $language['org_id'], 'operator' => '='),
      array('field' => 'type', 'value' => HR_QUALIFICATION_LANGUAGES, 'operator' => '='),
    );
    $form_state['storage']['qualifications'] = hr_basic_qualification_get_qualfications($qualifications_filters);
  }

  if (!isset($form_state['storage']['language_fluency_levels'])) {
    $form_state['storage']['language_fluency_levels'] = hr_basic_taxonomy_vocabulary_terms(HR_VOCABULARY_LANGUAGE_FLUENCY_LEVEL, FALSE);
  }

  drupal_set_title('Language: ' . $form_state['storage']['qualifications'][$language['qualification_id']]);

  $form['qualification'] = array('#title' => t('Language'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['qualifications'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Language'),
    '#default_value' => $language['qualification_id'],);

  $form['reading'] = array('#title' => t('Reading'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['language_fluency_levels'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Reading level'),
    '#default_value' => $language['reading'],);

  $form['writing'] = array('#title' => t('Writing'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['language_fluency_levels'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Writing level'),
    '#default_value' => $language['writing'],);

  $form['speaking'] = array('#title' => t('Speaking'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['language_fluency_levels'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Speaking level'),
    '#default_value' => $language['speaking'],);

  $form['comment'] = array('#title' => t('Comment'), '#type' => 'textarea', '#required' => FALSE, '#maxlength' => 400,
    '#default_value' => $language['comment'], '#rows' => 3, '#resizable' => FALSE);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => array('btn-success'))
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#attributes' => array('class' => array('btn-danger'))
  );

  $form['#prefix'] = '<div id="employee-qualification-language-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-language-form';

  return $form;
}

function hr_backend_pim_employee_qualification_language_edit_form_validate(&$form, &$form_state) {
  
}

function hr_backend_pim_employee_qualification_language_edit_form_submit(&$form, &$form_state) {
  global $user;
  $emp_id = $form_state['build_info']['args'][0];
  $language_id = $form_state['build_info']['args'][1];
  $language = $form_state['build_info']['args'][2];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    db_update('hr_employee_language')
        ->fields(array(
          'reading' => $form_state['values']['reading'],
          'writing' => $form_state['values']['writing'],
          'speaking' => $form_state['values']['speaking'],
          'comment' => $form_state['values']['comment'],
          'changed' => time(),
        ))
        ->condition('id', $language_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();
    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    db_delete('hr_employee_language')
        ->condition('id', $language_id, '=')
        ->execute();
    drupal_set_message('Successfully Deleted');
  }

  $redirect_options = array(
    'query' => array('anchor' => 'qualification-language-wrapper'),
  );
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $emp_id . '/qualifications', 0, $redirect_options);
  print ajax_render($commands);
  exit;
}

//License
function hr_backend_pim_employee_qualification_license_content($employee, $qualifications) {
  $license_form = drupal_get_form('hr_backend_pim_employee_qualification_license_form', $employee);

  $manage_qualifications = user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS);

  $rows = array();
  foreach ($employee['license'] as $row) {
    $checkbox = '<label class="css-input css-checkbox css-checkbox-primary">
          <input type="checkbox" value="' . $row['id'] . '" id="license_' . $row['id'] . '" name="license_' . $row['id'] . '"><span></span>
        </label>';

    $actions = array('<div class="btn-group btn-group-sm">');
    $actions[] = l('<i class="fa fa-search"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_LICENSE_DETAILS, 'emp_id' => $employee['emp_id'], 'license_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-info',)), 'html' => TRUE));
    if ($manage_qualifications) {
      $actions[] = l('<i class="fa fa-pencil"></i>', 'hrm-modal', array('query' => array('action' => HR_MODAL_PIM_QUALIFICATION_LICENSE_EDIT, 'emp_id' => $employee['emp_id'], 'license_id' => $row['id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-warning',)), 'html' => TRUE));
    }
    $actions[] = '</div>';

    $rows[]['data'] = array(
      'checkbox' => array('data' => $checkbox, 'class' => array('text-center')),
      'license' => array('data' => $qualifications[$row['qualification_id']]),
      'license_number' => array('data' => $row['license_number']),
      'issue_date' => array('data' => ($row['issue_date'] ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $row['issue_date']) : '')),
      'expiry_date' => array('data' => ($row['expiry_date'] ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $row['expiry_date']) : '')),
      'actions' => array('data' => implode('', $actions), 'class' => array('text-center')),
    );
  }

  $disabled = '';
  if (empty($rows)) {
    $disabled = 'disabled="disabled"';
  }
  $header_checkbox = '<label class="css-input css-checkbox css-checkbox-primary remove-margin-t remove-margin-b">
    <input type="checkbox" id="check-all-license" name="check-all-license" ' . $disabled . '><span></span>
    </label>';
  $header = array(
    array('data' => $header_checkbox, 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
    array('data' => 'License'),
    array('data' => 'Number'),
    array('data' => 'Issue Date'),
    array('data' => 'Expiry Date'),
    array('data' => 'Actions', 'class' => array('nosort', 'nosearch', 'text-center', 'remove-padding-r', 'remove-padding-l')),
  );

  $list = array(
    'attributes' => array('class' => array('employee-license-list', 'table', 'table-bordered', 'table-striped', 'js-table-checkable')),
    'rows' => $rows,
    'header' => $header
  );
  if (!empty($rows)) {
    $list['attributes']['class'][] = 'datatable-all';
  }

  $content = drupal_render($license_form);
  $content .= '<div class="table-responsive">' . theme('table', $list) . '</div>';

  $variables = array(
    'content' => $content,
    'title' => 'License',
    'id' => 'qualification-license-wrapper',
  );

  return $variables;
}

function hr_backend_pim_employee_qualification_license_details($emp_id, $license_id) {
  $license = db_select('hr_employee_license', 'employee_license')
          ->fields('employee_license', array('id', 'emp_id', 'org_id', 'qualification_id', 'license_number',
            'issue_date', 'expiry_date', 'attachment', 'uid', 'created', 'changed'))
          ->condition('id', $license_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);
  if (empty($license)) {
    hr_backend_modal_not_found();
  }

  $license_type = hr_basic_qualification_get_name($license['qualification_id']);
  drupal_set_title('License: ' . $license_type);

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'License', 'value_class' => array(),
    'value' => $license_type, 'value _class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'License number', 'value_class' => array(),
    'value' => $license['license_number'], 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Issue date', 'value_class' => array(),
    'value' => (($license['issue_date']) ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $license['issue_date']) : ''), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Expiry date', 'value_class' => array(),
    'value' => (($license['expiry_date']) ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $license['expiry_date']) : ''), 'value_class' => array());

  $attachment = '';
  if ($license['attachment']) {
    $file = file_load($license['attachment']);
    if ($file) {
      $attachment_url = file_create_url($file->uri);
      $attachment = l($file->filename, $attachment_url, array('attributes' => array('target' => '_blank')));
    }
  }
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Attachment', 'value_class' => array(),
    'value' => $attachment, 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Added by', 'value_class' => array(),
    'value' => hr_basic_account_get_name($license['uid']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Created', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $license['created']), 'value_class' => array());
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Updated', 'value_class' => array(),
    'value' => date(HR_DATETIME_FORMAT_LONG_SLASH, $license['changed']), 'value_class' => array());

  return theme('section_entity_details', array('fields' => $fields));
}

function hr_backend_pim_employee_qualification_license_form($form, &$form_state, $employee) {
  $access = FALSE;
  if (isset($form_state['storage']['add'])) {
    $access = TRUE;
  }

  if (!isset($form_state['storage']['qualifications'])) {
    $qualifications_filters = array(
      array('field' => 'org_id', 'value' => $employee['org_id'], 'operator' => '='),
      array('field' => 'type', 'value' => HR_QUALIFICATION_LICENSES, 'operator' => '='),
    );
    $form_state['storage']['qualifications'] = hr_basic_qualification_get_qualfications($qualifications_filters);
  }

  $form['qualification'] = array('#title' => t('License'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['qualifications'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Type'),
    '#access' => $access,);

  $form['license_number'] = array('#title' => t('License Number'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#access' => $access,);

  $form['issue_date'] = array('#title' => t('Issue date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#access' => $access,
  );

  $form['expiry_date'] = array('#title' => t('Expiry date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#access' => $access,
  );

  $attachment_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $employee['org_id'] . '/employees/' . $employee['emp_id'] . '/qualification/license';
  file_prepare_directory($attachment_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['attachment'] = array('#title' => t('Attachment'), '#type' => 'managed_file', '#required' => FALSE,
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf ppt pps'),),
    '#upload_location' => $attachment_upload_location,
    '#access' => $access,);

  $form['delete_license'] = array('#type' => 'hidden', '#default_value' => '', '#attributes' => array('id' => 'delete-license-ids'));

  //Form Actions
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_QUALIFICATIONS)) {
    if (!$access) {
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_add'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_add_callback',
          'wrapper' => 'employee-qualification-license-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-info'))
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array(
          'class' => array('btn-danger', 'delete-license'))
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn-success'))
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('hr_backend_pim_employee_qualification_form_cancel'),
        '#ajax' => array(
          'callback' => 'hr_backend_pim_employee_qualification_form_cancel_callback',
          'wrapper' => 'employee-qualification-license-form-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-warning'))
      );
    }
  }

  $form['#prefix'] = '<div id="employee-qualification-license-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-license-form';

  return $form;
}

function hr_backend_pim_employee_qualification_license_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    if (empty($form_state['values']['delete_license'])) {
      form_set_error('delete_license', t('At least select one license to be deleted.'));
    }
  }
}

function hr_backend_pim_employee_qualification_license_form_submit(&$form, &$form_state) {
  global $user;
  $employee = $form_state['build_info']['args'][0];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $issue_date = 0;
    if (!empty($form_state['values']['issue_date'])) {
      $issue_date = strtotime(str_replace('/', '-', $form_state['values']['issue_date']));
    }

    $expiry_date = 0;
    if (!empty($form_state['values']['expiry_date'])) {
      $expiry_date = strtotime(str_replace('/', '-', $form_state['values']['expiry_date']));
    }

    $attachment = 0;
    if ($form_state['values']['attachment']) {
      $attachment = $form_state['values']['attachment'];
    }

    $license_id = db_insert('hr_employee_license')
            ->fields(array(
              'emp_id' => $employee['emp_id'],
              'org_id' => $employee['org_id'],
              'qualification_id' => $form_state['values']['qualification'],
              'license_number' => $form_state['values']['license_number'],
              'issue_date' => $issue_date,
              'expiry_date' => $expiry_date,
              'attachment' => $attachment,
              'uid' => $user->uid,
              'created' => time(),
              'changed' => time(),
            ))->execute();

    if ($attachment) {
      hr_basic_file_save_file_data($attachment, 'hr_backend', HR_FILE_USAGE_QUALIFICATION_LICENSE_ATTACHMENT, $license_id);
    }
    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    $deleted_ids = explode(',', $form_state['values']['delete_license']);
    if (!empty($deleted_ids)) {
      foreach ($deleted_ids as $key => $value) {
        if (!is_numeric($value)) {
          unset($deleted_ids[$key]);
        }
      }

      if (!empty($deleted_ids)) {
        db_delete('hr_employee_license')
            ->condition('id', $deleted_ids, 'IN')
            ->execute();
      }

      drupal_set_message('Successfully Deleted');
    }
  }


  $redirect_options = array(
    'query' => array('anchor' => 'qualification-license-wrapper'),
  );
  drupal_goto('pim/employee/details/' . $employee['emp_id'] . '/qualifications', $redirect_options);
}

function hr_backend_pim_employee_qualification_license_edit_form($form, &$form_state, $emp_id, $license_id) {
  form_load_include($form_state, 'inc', 'hr_backend', 'includes/backend_pim');

  $license = db_select('hr_employee_license', 'employee_license')
          ->fields('employee_license', array('id', 'emp_id', 'org_id', 'qualification_id', 'license_number',
            'issue_date', 'expiry_date', 'attachment', 'uid', 'created', 'changed'))
          ->condition('id', $license_id, '=')
          ->condition('emp_id', $emp_id, '=')
          ->execute()->fetch(PDO::FETCH_ASSOC);
  if (empty($license)) {
    hr_backend_modal_not_found();
  }

  $form_state['build_info']['args'][2] = $license;

  $license_type = hr_basic_qualification_get_name($license['qualification_id']);
  drupal_set_title('License: ' . $license_type);

  if (!isset($form_state['storage']['qualifications'])) {
    $qualifications_filters = array(
      array('field' => 'org_id', 'value' => $license['org_id'], 'operator' => '='),
      array('field' => 'type', 'value' => HR_QUALIFICATION_LICENSES, 'operator' => '='),
    );
    $form_state['storage']['qualifications'] = hr_basic_qualification_get_qualfications($qualifications_filters);
  }

  $form['qualification'] = array('#title' => t('License'), '#type' => 'select', '#required' => TRUE,
    '#options' => $form_state['storage']['qualifications'],
    '#empty_option' => '', '#attributes' => array('class' => array('js-select2'), 'data-placeholder' => 'Type'),
    '#default_value' => $license['qualification_id'],);

  $form['license_number'] = array('#title' => t('License Number'), '#type' => 'textfield', '#required' => FALSE, '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => $license['license_number'],);

  $form['issue_date'] = array('#title' => t('Issue date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => ($license['issue_date'] ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $license['issue_date']) : ''),
  );

  $form['expiry_date'] = array('#title' => t('Expiry date'), '#type' => 'textfield', '#required' => FALSE,
    '#empty_option' => '', '#attributes' => array('class' => array('js-datepicker'), 'data-date-format' => 'dd/mm/yyyy', 'placeholder' => 'dd/mm/yyyy'),
    '#default_value' => ($license['expiry_date'] ? date(HR_DATETIME_FORMAT_SHORT_SLASH, $license['expiry_date']) : ''),
  );

  $attachment_upload_location = HR_DIRECTORY_ORGANIZATION . '/' . $license['org_id'] . '/employees/' . $emp_id . '/qualification/license';
  file_prepare_directory($attachment_upload_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $form['attachment'] = array('#title' => t('Attachment'), '#type' => 'managed_file', '#required' => FALSE,
    '#upload_validators' => array('file_validate_extensions' => array('jpg jpeg gif png doc docx pdf ppt pps'),),
    '#upload_location' => $attachment_upload_location,
    '#default_value' => $license['attachment'],);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => array('btn-success'))
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#attributes' => array('class' => array('btn-danger'))
  );

  $form['#prefix'] = '<div id="employee-qualification-license-form-wrapper" class="push-15">';
  $form['#suffix'] = '</div>';
  $form['#attributes']['class'][] = 'employee-qualification-license-form';

  return $form;
}

function hr_backend_pim_employee_qualification_license_edit_form_validate(&$form, &$form_state) {
  
}

function hr_backend_pim_employee_qualification_license_edit_form_submit(&$form, &$form_state) {
  global $user;
  $emp_id = $form_state['build_info']['args'][0];
  $license_id = $form_state['build_info']['args'][1];
  $license = $form_state['build_info']['args'][2];

  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $issue_date = 0;
    if (!empty($form_state['values']['issue_date'])) {
      $issue_date = strtotime(str_replace('/', '-', $form_state['values']['issue_date']));
    }

    $expiry_date = 0;
    if (!empty($form_state['values']['expiry_date'])) {
      $expiry_date = strtotime(str_replace('/', '-', $form_state['values']['expiry_date']));
    }

    $attachment = 0;
    if ($form_state['values']['attachment']) {
      $attachment = $form_state['values']['attachment'];
    }

    db_update('hr_employee_license')
        ->fields(array(
          'qualification_id' => $form_state['values']['qualification'],
          'license_number' => $form_state['values']['license_number'],
          'issue_date' => $issue_date,
          'expiry_date' => $expiry_date,
          'attachment' => $attachment,
          'changed' => time(),
        ))
        ->condition('id', $license_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();

    if ($attachment) {
      if ($attachment != $license['attachment']) {
        hr_basic_file_save_file_data($attachment, 'hr_backend', HR_FILE_USAGE_QUALIFICATION_LICENSE_ATTACHMENT, $license_id);
        if ($license['attachment']) {
          hr_basic_file_delete_file_data($license, 'hr_backend');
        }
      }
    }
    else {
      if ($license['attachment']) {
        hr_basic_file_delete_file_data($license['attachment'], 'hr_backend');
      }
    }
    drupal_set_message('Successfully Saved');
  }
  elseif (isset($form_state['values']['delete']) && $form_state['values']['op'] == $form_state['values']['delete']) {
    db_delete('hr_employee_license')
        ->condition('id', $license_id, '=')
        ->condition('emp_id', $emp_id, '=')
        ->execute();
    if ($license['attachment']) {
      hr_basic_file_delete_file_data($license['attachment'], 'hr_backend');
    }
  }


  $redirect_options = array(
    'query' => array('anchor' => 'qualification-license-wrapper'),
  );
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands[] = ctools_ajax_command_redirect('pim/employee/details/' . $emp_id . '/qualifications', 0, $redirect_options);
  print ajax_render($commands);
  exit;
}

//Global Functions
function hr_backend_pim_employee_qualification_form_add($form, &$form_state) {
  $form_state['storage']['add'] = TRUE;
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_qualification_form_add_callback($form, $form_state) {
  return $form;
}

function hr_backend_pim_employee_qualification_form_cancel($form, &$form_state) {
  unset($form_state['storage']['add']);
  $form_state['rebuild'] = TRUE;
}

function hr_backend_pim_employee_qualification_form_cancel_callback($form, $form_state) {
  return $form;
}

/*
 * Documents
 */

function hr_backend_pim_employee_details_content_documents($employee, $account) {
  $edit_permission = FALSE;
  $delete_permission = FALSE;

  $output = '';
  if (hr_backend_pim_permission(HR_PERM_PIM_EMPLOYEE_MANAGE_DOCUMENTS, $employee, $employee, $account)) {
    $edit_permission = TRUE;
    $delete_permission = TRUE;
    $output .= '<div class="push">' . l('<i class="fa fa-plus"></i> Add documents', 'hrm-modal', array('query' => array('action' => HRM_MODAL_EN_DOCUMENT_ADD, 'entity' => HR_ENTITY_EMPLOYEE, 'entity_id' => $employee['emp_id'],), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-success')), 'html' => TRUE)) . '</div>';
  }

  $documents_filters = array(
    array('field' => 'entity', 'value' => HR_ENTITY_EMPLOYEE, 'operator' => '='),
    array('field' => 'entity_id', 'value' => $employee['emp_id'], 'operator' => '='),
  );
  $documents_list = hr_backend_entity_document_get_documents_list($documents_filters, array(), TRUE, TRUE);
  $output .= theme('table', $documents_list);

  return array(
    'title' => 'Documents',
    'content' => $output,
  );
}

/*
 * Assets
 */

function hr_backend_pim_employee_assigned_assets_content($employee, $account) {
  $emp_id = $employee['emp_id'];
  $org_id = $employee['org_id'];

  $assigned_assets = hr_basic_employee_get_assigned_assets($emp_id);

  $content = '<p>There are no assets assigned to ' . $employee['name'] . '***</p>';
  if (!empty($assigned_assets)) {
    $asset_filters = array(
      array('field' => 'asset_id', 'value' => $assigned_assets, 'operator' => 'IN'),
      array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
      array('field' => 'status', 'value' => HR_TERM_ASSET_STATUS_ASSIGNED, 'operator' => '='),
    );

    $list = hr_basic_asset_get_asset_list($asset_filters, array(), TRUE, TRUE);
    $content = theme('table', $list);
  }

  $variables = array(
    'content' => $content,
    'title' => 'Assigned Assets',
    'id' => 'assigned-assets-wrapper',
  );

  return $variables;
}

function hr_backend_pim_employee_prev_assigned_assets_content($employee, $account) {
  $emp_id = $employee['emp_id'];
  $org_id = $employee['org_id'];

  $assigned_assets = hr_basic_employee_get_prev_assigned_assets($emp_id);

  $content = '<p>There are no assets assignment history for ' . $employee['name'] . '</p>';
  if (!empty($assigned_assets)) {
    $asset_filters = array(
      array('field' => 'asset_id', 'value' => $assigned_assets, 'operator' => 'IN'),
      array('field' => 'org_id', 'value' => $org_id, 'operator' => '='),
    );

    $list = hr_basic_asset_get_asset_list($asset_filters, array(), TRUE, TRUE, FALSE);
    $content = theme('table', $list);
  }

  $variables = array(
    'content' => $content,
    'title' => 'Asset assinmgment log',
    'id' => 'previous-assigned-assets-wrapper',
  );

  return $variables;
}

/*
 * Employee Accounts
 */

function hr_backend_pim_employee_account_details_content($employee, $account) {
  $emp_id = $employee['emp_id'];
  $employee_user = $employee['user'];

  $buttons = array();
  if (user_access(HR_PERM_PIM_EMPLOYEE_MANAGE_ACCOUNT)) {
    if (isset($employee_user['active']) && $employee_user['active']) {
      $buttons[] = l('<i class = "si si-logout"></i> Logout employee account', 'pim/account/logout/' . $emp_id, array(
        'attributes' => array(
          'class' => array('btn', 'btn-warning', 'btn-sm', 'push-5-r', 'confirm-delete'),
          'data-text' => 'Are you sure want to logout this account?',
          'data-title' => "Logout Account",),
        'html' => TRUE));
    }
    $buttons[] = l('<i class = "fa fa-asterisk"></i> Reset Password', 'pim/account/reset-pass/' . $emp_id, array(
      'attributes' => array(
        'class' => array('btn', 'btn-danger', 'btn-sm', 'push-5-r', 'confirm-delete'),
        'data-text' => 'Are you sure want to reset the password for this account?',
        'data-title' => "Reset Password",),
      'html' => TRUE));
  }

  if (!empty($buttons)) {
    $fields[] = array('type' => 'controls', 'wrapper_class' => array(), 'row_class' => array(),
      'value' => $buttons, 'controls_class' => array('col-sm-12', 'push'));
  }

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Username', 'value_class' => array(),
    'value' => $employee_user['username'], 'value_class' => array());

  $logged_in = '';
  if (isset($employee_user['active'])) {
    $logged_in = ($employee_user['active'] ? 'Yes' : 'No');
  }

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Logged in', 'value_class' => array(),
    'value' => $logged_in, 'value_class' => array());

  $login_time = '';
  if (isset($employee_user['login']) && $employee_user['login']) {
    $login_time = date(HR_DATETIME_FORMAT_LONG_SLASH, $employee_user['login']);
  }
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Login time', 'value_class' => array(),
    'value' => $login_time, 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Device', 'value_class' => array(),
    'value' => $employee_user['device_type'], 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
    'label' => 'Device ID', 'value_class' => array(),
    'value' => $employee_user['device_id'], 'value_class' => array());

  $content = theme('section_entity_details', array('fields' => $fields));

  $variables = array(
    'content' => $content,
    'title' => 'Account',
    'id' => 'account-wrapper',
  );

  return $variables;
}

function hr_backend_pim_account_logout($emp_id) {
  global $user;

  if ($emp_id && is_numeric($emp_id)) {
    $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
    $employee = hr_backend_pim_get_employee_data($filters, array(), TRUE);
    if (!empty($employee)) {
      $account = hr_basic_account_get_info($user->uid);

      if (hr_backend_pim_permission(HR_PERM_PIM_EMPLOYEE_MANAGE_ACCOUNT, $emp_id, $employee, $account)) {
        $gcm_data = array(
          'key' => HR_GCM_MESSAGE_KEY_ACCOUNT_LOGOUT,
          'message' => 'Login is required',
        );
        $devices = hr_basic_gcm_get_device_id(array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '=')));
        hr_services_sendGoogleCloudMessage($gcm_data, $devices);

        db_update('hr_employee_user')
            ->fields(array(
              'device_id' => '',
              'device_type' => '',
              'active' => HR_EMPLOYEE_USER_STATUS_INACTIVE,
              'login' => 0,
            ))
            ->condition('emp_id', $emp_id, '=')
            ->execute();

        drupal_set_message($employee['name'] . ' has been logged out from the HR App');
        drupal_goto('pim/employee/details/' . $emp_id . '/account');
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

function hr_backend_pim_account_reset_password($emp_id) {
  global $user;

  if ($emp_id && is_numeric($emp_id)) {
    $filters = array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '='));
    $employee = hr_backend_pim_get_employee_data($filters, array(), TRUE);
    if (!empty($employee)) {
      $account = hr_basic_account_get_info($user->uid);

      if (hr_backend_pim_permission(HR_PERM_PIM_EMPLOYEE_MANAGE_ACCOUNT, $emp_id, $employee, $account)) {
        require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
        $password = hr_basic_generate_random_string(6);
        $encrypted_password = _password_crypt('sha512', $password, HR_SHA512_SALT);


        $devices = hr_basic_gcm_get_device_id(array(array('field' => 'emp_id', 'value' => $emp_id, 'operator' => '=')));
        if (!empty($devices)) {
          $gcm_data = array(
            'key' => HR_GCM_MESSAGE_KEY_ACCOUNT_LOGOUT,
            'message' => 'Login is required',
          );
          hr_services_sendGoogleCloudMessage($gcm_data, $devices);
        }

        db_update('hr_employee_user')
            ->fields(array(
              'second_password' => $encrypted_password,
            ))
            ->condition('emp_id', $emp_id, '=')
            ->execute();

        hr_emails_notification_pim_employee_reset_password($employee, $password);

        drupal_set_message($employee['name'] . ' password has been reset');
        drupal_goto('pim/employee/details/' . $emp_id . '/account');
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
