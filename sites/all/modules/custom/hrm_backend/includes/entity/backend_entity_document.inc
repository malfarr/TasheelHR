<?php

/*
 * ========================================
 * Add documents
 * ========================================
 */

function hrm_backend_entity_document_add_documents_form($form, &$form_state, $entity, $oid, $entity_id, $data = array()) {
  $title = 'Add <strong> Documents</strong>';
  drupal_set_title($title, PASS_THROUGH);

  $form['documents'] = array(
    '#title' => t('Documents'),
    '#type' => 'plupload',
    '#autoupload' => FALSE,
    '#autosubmit' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_DOCUMENT),),
    '#plupload_settings' => array(
      'runtimes' => 'html5',
      'chunk_size' => '1mb',
    ),
    '#event_callbacks' => array(),
    '#required' => TRUE,
    '#event_callbacks' => array(
      'UploadComplete' => 'Drupal.behaviors.hrm_behavior_general.pluploadCompleted',
    ),
  );


  $document_max_size = variable_get(HRM_VAR_ADMIN_FILES_PLUPLOAD_DOCUMENT_MAX_SIZE, 0);
  if ($document_max_size) {
    $form['documents']['#plupload_settings']['max_file_size'] = $document_max_size . 'kb';
    $form['documents']['#description'] = 'Maximum document size is <strong>' . ($document_max_size / 1024) . ' MB</strong>';
  }


  $form['submit_suffix'] = array('#markup' => '<div class="form-group">'); // to make line border gone

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save documents'),
    '#attributes' => array('class' => array('btn-sm', 'plupload-submit-form'))
  );

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'entity-document-add-form';

  return $form;
}

function hrm_backend_entity_document_add_documents_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_entity_document_add_documents_form_submit(&$form, &$form_state) {
  $account = hrm_basic_account_get_info();

  $oid = $form_state['build_info']['args'][0];
  $entity = $form_state['build_info']['args'][1];
  $entity_id = $form_state['build_info']['args'][2];
  $data = $form_state['build_info']['args'][3];
  $ref1 = '';


  $usage = array('module' => HRM_MODULE_BACKEND, 'type' => 'entity_document_' . $entity, 'id' => $entity_id);
  $upload_location = strtr(HRM_DIR_ENTITY_DOCUMENT, array('%oid' => $oid, '%entity_id' => $entity_id));
  $documents = hrm_basic_plupload_form_files_submit($form, $form_state, 'documents', $upload_location, $usage);

  $now = time();
  $fields = array('oid', 'entity', 'entity_id', 'fid', 'title', 'description', 'ref_1', 'uid', 'created', 'changed');
  if (!empty($data)) {
    foreach ($data as $key => $value) {
      $fields[] = $key;
    }
  }
  $query = db_insert('hrm_entity_document')->fields($fields);
  foreach ($documents as $document) {
    $uri = hrm_basic_file_get_managed_file_uri($document);
    $file_info = pathinfo($uri);

    $values = array($oid, $entity, $entity_id, $document, $file_info['filename'], '', $ref1, $account['uid'], $now, $now);
    if (!empty($data)) {
      foreach ($data as $key => $value) {
        $values[] = $value;
      }
    }


    $query->values($values);
  }
  $query->execute();

  $tab = HRM_TAB_DOCUMENTS;
  $redirect_url = '';
  if (filter_input(INPUT_SERVER, 'HTTP_REFERER')) {
    $referer_url = explode('?', filter_input(INPUT_SERVER, 'HTTP_REFERER'));
    $redirect_url = $referer_url[0];
  }
  else {
    $redirect_url = str_replace('_', '-', $entity) . '/details/' . $entity_id;
  }

  drupal_set_message("Documents added successfully");
  $commands = array();
  $commands[] = ctools_ajax_command_redirect($redirect_url, 0, array('query' => array('tab' => $tab)));
  print ajax_render($commands);
  exit;
}

/*
 * ========================================
 * Edit document
 * ========================================
 */

function hrm_backend_entity_document_edit_document_form($form, &$form_state, $document_id, $entity, $entity_id, $data = array()) {
  $document = hrm_backend_entity_document_get_document_data($document_id);

  $title = 'Edit <strong> Document</strong>';
  drupal_set_title($title, PASS_THROUGH);

  $upload_location = strtr(HRM_DIR_ENTITY_DOCUMENT, array('%entity' => $entity, '%entity_id' => $entity_id));

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $document['title'],
  );

  $form['document'] = array(
    '#title' => t('Document'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#upload_validators' => array(
      'file_validate_extensions' => array(HRM_FILE_EXT_DOCUMENT),
    ),
    '#upload_location' => $upload_location,
    '#description' => HRM_FILE_EXT_DESC_DOCUMENT,
    '#default_value' => $document['fid'],);

  $document_max_size = variable_get(HRM_VAR_ADMIN_FILES_PLUPLOAD_DOCUMENT_MAX_SIZE, 0);
  if ($document_max_size) {
    $max_size_mb = ($document_max_size / 1024);
    $form['document']['#upload_validators']['file_validate_size'] = array($max_size_mb * 1024 * 1024);
    $form['document']['#description'] .= '<br />Maximum document size is <strong>' . $max_size_mb . ' MB</strong>';
  }

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#rows' => 2,
    '#default_value' => $document['description'],
  );
  $form['submit_suffix'] = array('#markup' => '<div class="form-group">'); // to make line border gone

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update document'),
    '#attributes' => array('class' => array('btn-primary', 'btn-sm',))
  );


  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'entity-document-edit-form';

  return $form;
}

function hrm_backend_entity_document_edit_document_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_entity_document_edit_document_form_submit(&$form, &$form_state) {
  $document_id = $form_state['build_info']['args'][0];
  $entity = $form_state['build_info']['args'][1];
  $entity_id = $form_state['build_info']['args'][2];
  $data = $form_state['build_info']['args'][3];

  $document = hrm_backend_entity_document_get_document_data($document_id);

  db_update('hrm_entity_document')
    ->fields(array(
      'fid' => $form_state['values']['document'],
      'title' => $form_state['values']['title'],
      'description' => $form_state['values']['description'],
      'changed' => time()
    ))
    ->condition('id', $document_id, '=')
    ->execute();
  if ($form_state['values']['document'] != $document['fid']) {
    hrm_basic_file_delete_file_data($document['fid'], HRM_MODULE_BACKEND);
    hrm_basic_file_save_file_data($form_state['values']['document'], HRM_MODULE_BACKEND, 'entity_document_' . $entity, $entity_id);
  }

  $redirect_url = '';
  $tab = HRM_TAB_DOCUMENTS;
  if (isset($data['redirect']) && isset($data['tab'])) {
    $redirect_url = $data['redirect'];
    $tab = $data['tab'];
  }
  elseif (filter_input(INPUT_SERVER, 'HTTP_REFERER')) {
    $referer_url = explode('?', filter_input(INPUT_SERVER, 'HTTP_REFERER'));
    $redirect_url = $referer_url[0];
  }
  else {
    $redirect_url = str_replace('_', '-', $entity) . '/details/' . $entity_id;
  }

  drupal_set_message("Documents updated successfully");
  $commands = array();
  $commands[] = ctools_ajax_command_redirect($redirect_url, 0, array('query' => array('tab' => $tab)));
  print ajax_render($commands);
  exit;
}

/*
 * ========================================
 * Delete document
 * ========================================
 */

function hrm_backend_entity_document_delete_document($document_id) {
  $document = hrm_backend_entity_document_get_document_data($document_id);
  db_delete('hrm_entity_document')
    ->condition('id', $document_id, '=')
    ->execute();
  hrm_basic_file_delete_file_data($document['fid'], HRM_MODULE_BACKEND);


  $redirect_url = '';
  $tab = HRM_TAB_DOCUMENTS;
  if (filter_input(INPUT_SERVER, 'HTTP_REFERER')) {
    $referer_url = explode('?', filter_input(INPUT_SERVER, 'HTTP_REFERER'));
    $redirect_url = $referer_url[0];
  }
  else {
    $redirect_url = str_replace('_', '-', $document['entity']) . '/details/' . $document['entity_id'];
  }

  drupal_set_message($document['title'] . ' document deleted successfully');
  drupal_goto($redirect_url, array('query' => array('tab' => $tab)));
}

/*
 * ========================================
 * Documents data functions
 * ========================================
 */

function hrm_backend_entity_document_get_entity_documents($filters = array()) {
  $query = db_select('hrm_entity_document', 'entity_document')
    ->fields('entity_document', array('id', 'entity', 'entity_id', 'fid', 'title', 'description', 'type',
    'ref_1', 'ref_2', 'uid', 'created', 'changed'));

  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }

  $documents = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  foreach ($documents as $id => $document) {
    $uri = hrm_basic_file_get_managed_file_uri($document['fid']);
    $documents[$id]['uri'] = $uri;
    $documents[$id]['url'] = file_create_url($uri);
  }

  return $documents;
}

function hrm_backend_entity_document_get_document_data($document_id) {
  $document = db_select('hrm_entity_document', 'entity_document')
      ->fields('entity_document', array('id', 'entity', 'entity_id', 'fid', 'title', 'description', 'type',
        'uid', 'created', 'changed'))
      ->condition('id', $document_id, '=')
      ->execute()->fetch(PDO::FETCH_ASSOC);

  return $document;
}

function hrm_backend_entity_document_get_documents_list($filters = array(), $edit = FALSE, $delete = FALSE) {
  $account = hrm_basic_account_get_info();
  $query = db_select('hrm_entity_document', 'entity_document')
    ->fields('entity_document', array('id', 'oid' ,'entity', 'entity_id', 'fid', 'ref_1', 'ref_2', 'title', 'description', 'type',
    'uid', 'created', 'changed'));

  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }
  $documents = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  $rows = array();
  $staff_names = array();

  foreach ($documents as $id => $document) {
    $file = file_load($document['fid']);
    if ($file) {
      $file_info = pathinfo($file->uri);
      $file_url = file_create_url($file->uri);
      $title = l($document['title'], $file_url, array('attributes' => array('target' => '_blank')));

      $size = hrm_basic_file_get_filesize((file_exists($file->uri) ? $file->uri : ''));

      if (!isset($staff_names[$document['uid']])) {
        if ($document['uid']) {
          $staff_names[$document['uid']] = 'added by <strong>' . hrm_basic_account_get_name($document['uid']) . '</strong>';
        }
        else {
          $staff_names[$document['uid']] = 'System auto generated';
        }
      }

      $action_buttons = array('<div class="btn-group btn-group-sm">');
      $action_buttons[] = l('<i class="fa fa-download"></i>', $file_url, array('html' => TRUE, 'attributes' => array('class' => array('btn', 'btn-xs', 'btn-primary'), 'title' => 'Download', 'download' => 'download')));
      if ($document['description'] != '') {
        $action_buttons[] = l(HRM_FA_INFO, 'hrm-modal', array('query' => array('action' => 'description', 'title' => $document['title'], 'content' => $document['description']), 'attributes' => array('class' => array('btn', 'btn-xs', 'btn-success'), 'title' => 'Document description'), 'html' => TRUE));
      }
      if ($edit) {
        $action_buttons[] = l(HRM_FA_PENCIL, 'hrm-modal', array('query' => array('action' => HRM_MODAL_EN_DOCUMENT_EDIT, 'document_id' => $id, 'entity' => $document['entity'], 'entity_id' => $document['entity_id']), 'attributes' => array('class' => array('btn', 'btn-xs', 'btn-warning'), 'title' => 'Edit document'), 'html' => TRUE));
      }
      if ($delete) {
        $action_buttons[] = l(HRM_FA_TRASH_ALT, 'entity/document/delete/' . $id . '/delete', array('attributes' => array('class' => array('btn', 'btn-xs', 'btn-danger', 'confirm-delete'), 'title' => 'Delete document', 'data-text' => 'Are you sure you want to delete ' . $document['title'] . ' document?'), 'html' => TRUE));
      }
      $action_buttons[] = '</div>';

      $rows[$id] = array(
        'data' => array(
          array('data' => $document['title']),
          array('data' => $title),
          array('data' => check_markup($document['description'], 'filtered_html')),
          array('data' => $file_info['extension'] . ' - ' . $size . ' MB<br /><span class="file-added-by-field">' . $staff_names[$document['uid']] . '</span>'),
          array('data' => date(HRM_DATE_SHORT, $document['created'])),
          array('data' => implode(' ', $action_buttons), 'class' => array()),
        ),
      );
 
    }
  }

  $header = array(
    array('data' => '#', 'class' => array('nosort', 'nosearch', 'noexport')),
    array('data' => 'Document'),
    array('data' => 'Description'),
    array('data' => 'Information'),
    array('data' => 'Added On'),
    array('data' => 'Action', 'class' => array('nosort', 'nosearch', 'noexport')),
  );
  

  $list = array(
    'attributes' => array('class' => array('table-vcenter', 'table-hover')),
    'rows' => $rows,
    'header' => $header
  );

  $list['attributes']['class'][] = (!empty($rows) ? 'datatable-all-index' : 'empty-datatable');

  return $list;
}

/*
 * ========================================
 * ENTITY FILES
 * ========================================
 */

function hrm_backend_entity_document_get_entity_files_raw($filters = array()) {
  $entity_files_query = db_select('hrm_entity_file', 'entity_file')
    ->fields('entity_file', array('id', 'entity', 'entity_id', 'type', 'fid', 'title', 'extension', 'uri',
    'ref_1', 'ref_2', 'weight', 'created'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $entity_files_query->condition($filter['field'], $filter['value'], $operator);
    }
  }
  $entity_files_query->orderBy('weight');

  $entity_files = $entity_files_query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);


  return $entity_files;
}

function hrm_backend_entity_document_get_entity_files($filters = array()) {
  $entity_files_query = db_select('hrm_entity_file', 'entity_file')
    ->fields('entity_file', array('id', 'entity', 'entity_id', 'type', 'fid', 'title', 'extension', 'uri',
    'ref_1', 'ref_2', 'weight', 'created'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $entity_files_query->condition($filter['field'], $filter['value'], $operator);
    }
  }
  $entity_files_query->orderBy('weight');

  $entity_files = $entity_files_query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  $files_icons = array('txt' => 'txt', 'doc' => 'doc', 'rtf' => 'rtf', 'log' => 'log', 'tex' => 'tex', 'msg' => 'msg', 'text' => 'text', 'wpd' => 'wpd',
    'wps' => 'wps', 'docx' => 'docx', 'page' => 'page', 'csv' => 'csv', 'dat' => 'dat', 'tar' => 'tar', 'xml' => 'xml', 'vcf' => 'vcf', 'pps' => 'pps', 'key' => 'key',
    'ppt' => 'ppt', 'pptx' => 'pptx', 'sdf' => 'sdf', 'gbr' => 'gbr', 'ged' => 'ged', 'mp3' => 'mp3', 'm4a' => 'm4a', 'waw' => 'waw', 'wma' => 'wma', 'mpa' => 'mpa',
    'iff' => 'iff', 'aif' => 'aif', 'ra' => 'ra', 'mid' => 'mid', 'm3v' => 'm3v', 'e_3gp' => 'e_3gp', 'shf' => 'shf', 'avi' => 'avi', 'asx' => 'asx', 'mp4' => 'mp4',
    'e_3g2' => 'e_3g2', 'mpg' => 'mpg', 'asf' => 'asf', 'vob' => 'vob', 'wmv' => 'wmv', 'mov' => 'mov', 'srt' => 'srt', 'm4v' => 'm4v', 'flv' => 'flv', 'rm' => 'rm',
    'png' => 'png', 'psd' => 'psd', 'psp' => 'psp', 'jpg' => 'jpg', 'tif' => 'tif', 'tiff' => 'tiff', 'gif' => 'gif', 'bmp' => 'bmp', 'tga' => 'tga', 'thm' => 'thm',
    'yuv' => 'yuv', 'dds' => 'dds', 'ai' => 'ai', 'eps' => 'eps', 'ps' => 'ps', 'svg' => 'svg', 'pdf' => 'pdf', 'pct' => 'pct', 'indd' => 'indd', 'xlr' => 'xlr', 'xls' => 'xls',
    'xlsx' => 'xlsx', 'db' => 'db', 'dbf' => 'dbf', 'mdb' => 'mdb', 'pdb' => 'pdb', 'sql' => 'sql', 'aacd' => 'aacd', 'app' => 'app', 'exe' => 'exe', 'com' => 'com',
    'bat' => 'bat', 'apk' => 'apk', 'jar' => 'jar', 'hsf' => 'hsf', 'pif' => 'pif', 'vb' => 'vb', 'cgi' => 'cgi', 'css' => 'css', 'js' => 'js', 'php' => 'php', 'xhtml' => 'xhtml',
    'htm' => 'htm', 'html' => 'html', 'asp' => 'asp', 'cer' => 'cer', 'jsp' => 'jsp', 'cfm' => 'cfm', 'aspx' => 'aspx', 'rss' => 'rss', 'csr' => 'csr', 'less' => 'less',
    'otf' => 'otf', 'ttf' => 'ttf', 'font' => 'font', 'fnt' => 'fnt', 'eot' => 'eot', 'woff' => 'woff', 'zip' => 'zip', 'zipx' => 'zipx', 'rar' => 'rar', 'targ' => 'targ',
    'sitx' => 'sitx', 'deb' => 'deb', 'e_7z' => 'e_7z', 'pkg' => 'pkg', 'rpm' => 'rpm', 'cbr' => 'cbr', 'gz' => 'gz', 'dmg' => 'dmg', 'cue' => 'cue', 'bin' => 'bin',
    'iso' => 'iso', 'hdf' => 'hdf', 'vcd' => 'vcd', 'bak' => 'bak', 'tmp' => 'tmp', 'ics' => 'ics', 'msi' => 'msi', 'cfg' => 'cfg', 'ini' => 'ini', 'prf' => 'prf',
  );

  $files = array();
  foreach ($entity_files as $id => $entity_file) {
    if (file_exists($entity_file['uri'])) {
      $file_url = file_create_url($entity_file['uri']);
      $size = hrm_basic_file_get_filesize($entity_file['uri']);

      $caption = $size . ' MB';
      if ($entity_file['extension'] == HRM_FILE_EXTENSION_PDF) {
        $pages_count = hrm_basic_file_get_pdf_pages_counter($entity_file['uri']);
        $caption = $pages_count . ' Pages | ' . $size . ' MB';
      }
      $caption .= '<br /><span class="file-added-by-field">System auto generated</span>';
      $files[] = array(
        'id' => 'entity-file-' . $entity_file['entity'] . '-' . $entity_file['entity_id'] . '-' . $id,
        'color_class' => 'warning',
        'url' => $file_url,
        'name' => $entity_file['title'],
        'extension' => $entity_file['extension'],
        'icon' => 'fi fi-' . $files_icons[$entity_file['extension']],
        'caption' => $caption,
        'links' => array(
          l(HRM_FA_DOWNLOAD, $file_url, array('html' => TRUE, 'attributes' => array('class' => array('btn', 'btn-xs', 'btn-primary'), 'title' => 'Download', 'download' => 'download'))),
        ),
        'type' => $entity_file['type'],
        'ref_1' => $entity_file['ref_1'],
        'ref_2' => $entity_file['ref_2']
      );
    }
  }

  return $files;
}

function hrm_backend_entity_document_generate_entity_file($entity, $entity_id, $type, $name, $title, $delete_prev = FALSE, $uid = 0, $ref_1 = 0, $ref_2 = 0) {
  if ($delete_prev) {
    $delete_filters = array(
      array('field' => 'entity', 'value' => $entity),
      array('field' => 'entity_id', 'value' => $entity_id),
      array('field' => 'type', 'value' => $type),
    );
    hrm_backend_entity_document_delete_entity_file($delete_filters);
  }

  $uri = strtr(HRM_DIR_ENTITY_PDF, array('%entity' => $entity, '%entity_id' => $entity_id));
  $uri .= '/' . $name;

  $fid = hrm_basic_file_save_unmanged_file($uri, $name, HRM_MODULE_BACKEND, $type, $entity_id, $uid);
  hrm_backend_entity_document_save_entity_file($entity, $entity_id, $type, $fid, $title, HRM_FILE_EXTENSION_PDF, $ref_1, $ref_2);
}

function hrm_backend_entity_document_save_entity_file($entity, $entity_id, $type, $fid, $title, $extension, $ref_1 = 0, $ref_2 = 0, $weight = 0) {
  $uri = hrm_basic_file_get_managed_file_uri($fid);

  if ($type == HRM_EN_FILE_TYPE_NFI_DISTRIBUTION_FORM && !$weight) {
    $weight = 1;
  }

  db_insert('hrm_entity_file')
    ->fields(array(
      'entity' => $entity,
      'entity_id' => $entity_id,
      'type' => $type,
      'fid' => $fid,
      'title' => $title,
      'extension' => $extension,
      'uri' => $uri,
      'ref_1' => $ref_1,
      'ref_2' => $ref_2,
      'weight' => $weight,
      'created' => time(),
      )
    )->execute();
}

function hrm_backend_entity_document_delete_entity_file($filters = array()) {
  $entity_files_query = db_select('hrm_entity_file', 'entity_file')
    ->fields('entity_file', array('id', 'entity', 'entity_id', 'type', 'fid', 'title', 'extension', 'uri', 'weight', 'created'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $entity_files_query->condition($filter['field'], $filter['value'], $operator);
    }
  }
  $entity_files = $entity_files_query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  $entity_files_ids = array_keys($entity_files);
  if (!empty($entity_files_ids)) {
    db_delete('hrm_entity_file')->condition('id', $entity_files_ids, 'IN')->execute();
    foreach ($entity_files as $entity_file) {
      hrm_basic_file_delete_file_data($entity_file['fid'], HRM_MODULE_BACKEND);
    }
  }
}


