<?php

function hrm_backend_config_job_permission($action, $account = array(), $data = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }

  if (!isset($account['oid']) || !$account['oid']) {
    return FALSE;
  }

  switch ($action) {
    /* Job titles */
    case HRM_PERM_CONFIG_JOB_TITLE_ACCESS:
      if (user_access(HRM_PERM_CONFIG_JOB_TITLE_ACCESS) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_JOB_TITLE_ACCESS_DETAILS:
      if (user_access(HRM_PERM_CONFIG_JOB_TITLE_ACCESS_DETAILS) && $account['oid'] &&
        $data['title']['oid'] == $account['oid'] ) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_JOB_TITLE_ADD:
      if (user_access(HRM_PERM_CONFIG_JOB_TITLE_ADD) && $account['oid']) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_JOB_TITLE_EDIT:
      if (user_access(HRM_PERM_CONFIG_JOB_TITLE_EDIT) && $account['oid'] &&
        $data['title']['oid'] == $account['oid'] ) {
        return TRUE;
      }
      break;
    case HRM_PERM_CONFIG_JOB_TITLE_DELETE:
      if (user_access(HRM_PERM_CONFIG_JOB_TITLE_EDIT) && $account['oid'] &&
        $data['title']['oid'] == $account['oid']) {
        return TRUE;
      }
      break;


  }
  return FALSE;
}

/* -----------------------JOB CONFIGURATION----------------------- */
/*
 * Job Titles
 */

function hrm_backend_config_job_titles() {
  $account = hrm_basic_account_get_info();

  if (hrm_backend_config_job_permission(HRM_PERM_CONFIG_JOB_TITLE_ACCESS, $account)) {

    $filters = array(array('field' => 'oid', 'value' => $account['oid']));

    $titles = hrm_backend_config_job_title_get_data($filters);
    $role_permissions = hrm_basic_role_permissions($account['role']);
    $rows = array();
    foreach ($titles as $id => $title) {
      $description = substr(check_markup($title['description'], 'filtered_html'), 0, 200);
      $permission_data = array('title' => $title, 'permissions' => $role_permissions);
      $access_details_permission = hrm_backend_config_job_permission(HRM_PERM_CONFIG_JOB_TITLE_ACCESS_DETAILS, $account, $permission_data);

      $row_classes = array();
      $row_classes[] = ($access_details_permission ? 'table-tr-link' : '');
      $tr_data_url = ($access_details_permission ? url('config/job/title/details/' . $id) : '');
      $rows[$id] = array(
        'data' => array(
          array('data' => '', 'class' => array('table-row-index-td')),
          array('data' => ($title['name'])),
          array('data' => $description),
        ),
        'class' => $row_classes,
        'data-url' => $tr_data_url,
      );
    }
    $header = array(
      array('data' => '#', 'class' => array('nosort', 'nosearch', 'table-row-index-th')),
      array('data' => 'Name'),
      array('data' => 'Description'),
    );

    $list = array(
      'attributes' => array('class' => array('table-hover', 'table-vcenter')),
      'rows' => $rows,
      'header' => $header
    );
    $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all-index' : 'empty-datatable');

    $actions = array();
    if (hrm_backend_config_job_permission(HRM_PERM_CONFIG_JOB_TITLE_ADD, $account)) {
      $actions[] = l(HRM_FA_PLUS_BLUE . 'Add Job Title', 'config/job/title/add/0', array('html' => TRUE));
    }

    $variables = array(
      'title' => 'Job Title',
      'list' => $list,
      'actions' => $actions
    );

    return theme('page_wrapper_entity_list', $variables);
  }
  else {
    drupal_access_denied();
    exit();
  }
}

function hrm_backend_config_job_title_details($id) {
  if ($id && is_numeric($id)) {
    $title = hrm_backend_config_job_title_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (!empty($title)) {
      $account = hrm_basic_account_get_info();
      $permission_data = array('title' => $title);

      if (hrm_backend_config_job_permission(HRM_PERM_CONFIG_JOB_TITLE_ACCESS_DETAILS, $account, $permission_data)) {
        $fields = array();

        $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
          'label' => 'Name', 'label_class' => array(),
          'value' => $title['name'], 'value_class' => array());

        $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
          'label' => 'Description', 'label_class' => array(),
          'value' => check_markup($title['description'], 'full_html'), 'value_class' => array());

        $attachment = file_load($title['attachment']);
        if (isset($title['attachment']) && $title['attachment']) {
          $attachment = file_load($title['attachment']);
          if ($attachment) {
            $attachment_url = file_create_url($attachment->uri);
            $attachment_link = l($title['name'], $attachment_url, array('attributes' => array('target' => '_blank')));

            $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
              'label' => 'Attachment', 'label_class' => array(),
              'value' => $attachment_link, 'value_class' => array());
          }
        }



        $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
          'label' => 'Notes', 'label_class' => array(),
          'value' => (isset($title['note']) && !empty($title['note']) ? check_markup($title['note'], 'filtered_html') : ''), 'value_class' => array());

        $details = theme('section_entity_details', array('fields' => $fields));

        $actions = array();
        if (hrm_backend_config_job_permission(HRM_PERM_CONFIG_JOB_TITLE_EDIT, $account, $permission_data)) {
          $actions[] = l(HRM_FA_PENCIL_YELLOW . '  Edit job title', 'config/job/title/edit/' . $id, array('html' => TRUE));
        }


        $variables = array(
          'details' => $details,
          'actions' => $actions,
          'entity' => HRM_ENTITY_JOB_TITLE
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

function hrm_backend_config_job_title_get_data($filters = array(), $first_item = FALSE) {
  $query = db_select('hrm_config_job_title', 'job_title')
    ->fields('job_title', array('id', 'oid',  'name', 'description', 'attachment', 'note', 'uid', 'created', 'changed'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }

  $titles = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  if ($first_item) {
    return reset($titles);
  }

  return $titles;
}

function hrm_backend_config_job_title_form_page($id) {
  $account = hrm_basic_account_get_info();

  $title = array();
  if ($id) {
    $title = hrm_backend_config_job_title_get_data(array(array('field' => 'id', 'value' => $id)), TRUE);
    if (empty($title)) {
      drupal_not_found();
      exit();
    }

    $edit_title_permission = hrm_backend_config_job_permission(HRM_PERM_CONFIG_JOB_TITLE_EDIT, $account, array('title' => $title));
    if (!$edit_title_permission) {
      drupal_access_denied();
      exit();
    }

  }
  else {
    $add_title_permission = hrm_backend_config_job_permission(HRM_PERM_CONFIG_JOB_TITLE_ADD, $account);
    if (!$add_title_permission) {
      drupal_access_denied();
      exit();
    }

  }

  $title_form = drupal_get_form('hrm_backend_config_job_title_form', $title, $account);

  $variables = array(
    'entity' => HRM_ENTITY_JOB_TITLE,
    'title' => 'Job title information',
    'form' => $title_form,
  );
  return theme('page_wrapper_entity_form', $variables);
}

function hrm_backend_config_job_title_form($form, &$form_state, $title, $account) {
  $oid = $account['oid'];

  $form['name'] = array(
    '#title' => 'Title',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($title['name']) ? $title['name'] : '')
  );
  $form['description'] = array(
    '#title' => 'Description',
    '#type' => 'textarea',
    '#required' => TRUE,
    '#resizable' => FALSE,
    '#attributes' => array('class' => array('ckeditor')),
    '#default_value' => (isset($title['description']) ? $title['description'] : '')
  );

//  $dest_location = str_replace(array('%entity', '%entity_id'), array(HRM_ENTITY_ORGANIZATION, $oid), HRM_DIR_ENTITY) . '/config/job/title';
  $dest_location = strtr(HRM_DIR_ENTITY, array('%entity' => HRM_ENTITY_ORGANIZATION, '%entity_id' => $oid)) . '/config/job/title';
  file_prepare_directory($dest_location, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $form['attachment'] = array(
    '#title' => t('Specification attachment'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_DOCUMENT),),
    '#upload_location' => $dest_location,
    '#default_value' => (isset($title['attachment']) ? $title['attachment'] : '')
  );
  $form['note'] = array(
    '#title' => 'Notes',
    '#type' => 'textarea',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#attributes' => array('class' => array('ckeditor')),
    '#default_value' => (isset($title['note']) ? $title['note'] : '')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ' Save Job Title',
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  if (isset($title['id']) && $title['id']) {
    $delete_title_permission = hrm_backend_config_job_permission(HRM_PERM_CONFIG_JOB_TITLE_DELETE, $account, array('title' => $title));
    if ($delete_title_permission) {
      unset($form['submit']['#suffix']);

      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => 'Delete Job Title',
        '#submit' => array('hrm_backend_config_job_title_form_delete'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('btn-danger', 'btn-sm')),
        '#suffix' => '</div>',
      );
    }
  }

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'config-job-title-form';

  return $form;
}

function hrm_backend_config_job_title_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_config_job_title_form_submit(&$form, &$form_state) {
  $title = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];

  if (isset($title['id']) && $title['id']) {
    db_update('hrm_config_job_title')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'attachment' => $form_state['values']['attachment'],
        'note' => $form_state['values']['note'],
        'changed' => time(),
      ))
      ->condition('id', $title['id'], '=')
      ->execute();
    if ($form_state['values']['attachment'] != $title['attachment']) {
      hrm_basic_file_save_file_data($form_state['values']['attachment'], HRM_MODULE_BACKEND, 'job_title_attachment', $title['id']);
      if ($title['attachment']) {
        hrm_basic_file_delete_file_data($title['attachment'], HRM_MODULE_BACKEND);
      }
    }
  }
  else {
    $title['id'] = db_insert('hrm_config_job_title')
      ->fields(array(
        'oid' => $account['oid'],
        'name' => $form_state['values']['name'],
        'description' => $form_state['values']['description'],
        'attachment' => $form_state['values']['attachment'],
        'note' => $form_state['values']['note'],
        'uid' => $account['uid'],
        'created' => time(),
        'changed' => time(),
      ))
      ->execute();
    if ($form_state['values']['attachment']) {
      hrm_basic_file_save_file_data($form_state['values']['attachment'], HRM_MODULE_BACKEND, 'job_title_attachment', $title['id']);
    }

    db_insert('hrm_config_job_structure')
      ->fields(array(
        'oid' => $account['oid'],
        'job_id' => $title['id'],
        'parent' => 0,
        'weight' => 0,
      ))
      ->execute();
  }

  drupal_set_message($form_state['values']['name'] . ' job title saved successfully***');
  drupal_goto('config/job/title/details/' . $title['id']);
}

function hrm_backend_config_job_title_form_delete(&$form, &$form_state) {
  $title = $form_state['build_info']['args'][0];

  db_delete('hrm_config_job_title')
    ->condition('id', $title['id'], '=')
    ->execute();

  if ($title['attachment']) {
    hrm_basic_file_delete_file_data($title['attachment'], HRM_MODULE_BACKEND);
  }


  db_update('hrm_emp_job')->fields(array('job_title' => 0))
    ->condition('job_title', $title['id'], '=')
    ->execute();
  db_update('hrm_emp_job_history')->fields(array('job_title' => 0))
    ->condition('job_title', $title['id'], '=')
    ->execute();

  db_delete('hrm_config_job_structure')
    ->condition('job_id', $title['id'], '=')->execute();
  db_update('hrm_config_job_structure')
    ->fields(array('parent' => 0))
    ->condition('parent', $title['id'], '=')
    ->execute();

  drupal_set_message($title['name'] . ' job title deleted successfully***');
  drupal_goto('config/job/title');
}



/*
 * Employment Status
 */

/*
 * Recruitment source
 */


/*
 * Reporting methods
 */




