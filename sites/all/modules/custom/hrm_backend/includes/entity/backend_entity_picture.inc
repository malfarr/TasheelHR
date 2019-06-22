<?php

function hrm_backend_entity_picture_display_pictures($filters = array(), $edit = FALSE, $delete = FALSE) {
  $result = hrm_backend_get_entity_picture_data($filters);
  $fields = array();

  $pictures = array();
  foreach ($result as $id => $row) {
    $file = file_load($row['picture_fid']);
    if ($file && file_exists($file->uri)) {
      $pictures[$row['id']] = array(
        'id' => $row['id'],
        'fid' => $row['picture_fid'],
        'uri' => $file->uri,
        'url' => file_create_url($file->uri),
        'caption' => date(HRM_DATE_SIMPLE_TIME_12, $row['created']) . ' - ' . (empty($row['caption']) ? $file->filename : $row['caption']),
        'uid' => $row['uid'],
        'created' => $row['created'],
        'changed' => $row['changed'],
      );

      if ($edit) {
        $pictures[$row['id']]['links'][] = array(
          'link' => l('<i class="fa fa-pencil"></i>', 'hr-modal', array('query' => array('action' => 'entity-photo-edit', 'photo_id' => $row['id']), 'attributes' => array('class' => array('btn', 'btn-xs', 'btn-warning')), 'html' => TRUE)),
        );
      }

      if ($delete) {
        $pictures[$row['id']]['links'][] = array(
          'link' => l(HRM_FA_TRASH, 'entity/photo/delete/' . $row['id'], array('attributes' => array('class' => array('btn', 'btn-xs', 'btn-danger'), 'title' => 'Delete Picture'), 'html' => TRUE)),
        );
      }
      $actions = '';

      if (!empty($pictures[$row['id']]['links'])) {
        foreach ($pictures[$row['id']]['links'] as $link) {
          $actions .= implode('', $link);
        }
      }


      $row['picture'] = theme('image_style', array('style_name' => HRM_IMAGE_STYLE_150_150, 'path' => $row['picture_uri'], 'alt' => $row['id'], 'title' => $row['caption'], 'attributes' => array('class' => array('img-circle'))));
      $picture = '
    <div class="gallery custom-gallery widget text-center" data-toggle="lightbox-gallery">
        <a href="' . $pictures[$id]['url'] . '" class="gallery-link" title="' . ('added by <strong>' . hrm_basic_account_get_name($row['uid']) . '</strong> on : ' . date(HRM_DATE_SIMPLE_TIME_12, $row['created'])) . '">
          ' . $row['picture'] . $actions . '
        </a>
    </div>';


      $fields[] = array('type' => 'raw', 'wrapper_class' => array('col-sm-12', 'photos-row', 'text-center', 'strong'),
        'value' => $picture, 'row_class' => array('no-border'));
    }
    else {
      watchdog('HRM Backend', 'File: ' . $file->fid . ' not exists.', array(), WATCHDOG_ERROR);
    }
  }
  $pictures = '<p>No Pictures</p>';
  if (!empty($pictures)) {
    $pictures = theme('section_entity_details', array('fields' => $fields));
  }


  return $pictures;
}

/*
 * ========================================
 * Add picture
 * ========================================
 */

function hrm_backend_entity_picture_add_picture_form($form, &$form_state, $entity, $oid, $entity_id, $data = array()) {
  $title = 'Add <strong> pictures</strong>';
  drupal_set_title($title, PASS_THROUGH);

  $form['pictures'] = array(
    '#title' => t('Picture'),
    '#type' => 'plupload',
    '#autoupload' => FALSE,
    '#autosubmit' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_PICTURE),),
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

  $picture_max_size = variable_get(HRM_VAR_ADMIN_FILES_PLUPLOAD_PICTURE_MAX_SIZE, 0);
  if ($picture_max_size) {
    $form['pictures']['#plupload_settings']['max_file_size'] = $picture_max_size . 'kb';
    $form['pictures']['#description'] = 'Maximum picture size is <strong>' . ($picture_max_size / 1024) . ' MB</strong>';
  }

  $form['submit_suffix'] = array('#markup' => '<div class="form-group">'); // to make line border gone

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save pictures'),
    '#attributes' => array('class' => array('btn-sm', 'plupload-submit-form'))
  );

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'entity-picture-add-form';

  return $form;
}

function hrm_backend_entity_picture_add_picture_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_entity_picture_add_picture_form_submit(&$form, &$form_state) {
  global $user;

  $oid = $form_state['build_info']['args'][0];
  $entity = $form_state['build_info']['args'][1];
  $entity_id = $form_state['build_info']['args'][2];
  $data = $form_state['build_info']['args'][3];
  $ref_1 = 0;
  $ref_2 = 0;

  if (isset($data['ref_1'])) {
    $ref_1 = $data['ref_1'];
  }
  if (isset($data['ref_2'])) {
    $ref_2 = $data['ref_2'];
  }

  $usage = array('module' => HRM_MODULE_BACKEND, 'type' => 'entity_picture_' . $entity, 'id' => $entity_id);
  $upload_location = strtr(HRM_DIR_ENTITY_PHOTO, array('%oid' => $oid, '%entity_id' => $entity_id));

  $pictures = hrm_basic_plupload_form_files_submit($form, $form_state, 'pictures', $upload_location, $usage);

  $now = time();
  $fields = array('oid', 'entity', 'entity_id', 'picture_fid', 'caption', 'ref_1', 'ref_2', 'uid', 'created', 'changed');
  if (!empty($data)) {
    foreach ($data as $key => $value) {
      $fields[] = $key;
    }
  }
  $query = db_insert('hrm_entity_picture')->fields($fields);
  foreach ($pictures as $picture) {
    $uri = hrm_basic_file_get_managed_file_uri($picture);
    $file_info = pathinfo($uri);

    $values = array($oid, $entity, $entity_id, $picture, '', $ref_1, $ref_2, $user->uid, $now, $now);
    if (!empty($data)) {
      foreach ($data as $key => $value) {
        $values[] = $value;
      }
    }

    $query->values($values);
  }
  $query->execute();

  $tab = HRM_TAB_PICTURES;
  $redirect_url = '';
  if (filter_input(INPUT_SERVER, 'HTTP_REFERER')) {
    $referer_url = explode('?', filter_input(INPUT_SERVER, 'HTTP_REFERER'));
    $redirect_url = $referer_url[0];
  }
  else {
    switch ($entity) {
      case 'asset':
        $redirect_url = 'asset/asset/details/' . $entity_id;
        break;
      case HR_ENTITY_DISCIPLINE:
        $redirect_url = 'discipline/details/' . $entity_id;
        break;
    }
  }

  drupal_set_message("pictures saved successfully");
  $commands = array();
  $commands[] = ctools_ajax_command_redirect($redirect_url, 0, array('query' => array('tab' => $tab)));
  print ajax_render($commands);
  exit;
}

/*
 * ========================================
 * Edit picture
 * ========================================
 */

function hrm_backend_entity_picture_edit_picture_form($form, &$form_state, $pid) {
  $title = 'Edit <strong> picture</strong>';
  drupal_set_title($title, PASS_THROUGH);

  $picture = hrm_backend_entity_pictures_get_data(array(array('field' => 'id', 'value' => $pid, 'operator' => '=')), TRUE);
  if (empty($picture)) {
    hrm_backend_modal_not_found();
  }
  $form_state['build_info']['args'][0] = $picture;

  $picture_file = file_load($picture['picture_fid']);
  $caption = (empty($picture['caption']) ? $picture_file->filename : $picture['caption']);

  $form['picture'] = array('#markup' => '<div class="text-center">' . theme('image_style', array('style_name' => 'image_style_edit_form', 'path' => $picture_file->uri, 'attributes' => array('alt' => $caption, 'class' => array('picture-edit-image')))) . '</div>');

  $form['caption'] = array(
    '#title' => t('picture caption'),
    '#type' => 'textarea',
    '#required' => FALSE,
    '#resizable' => FALSE,
    '#rows' => 2,
    '#default_value' => $caption,
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update picture'),
    '#attributes' => array('class' => array('btn-primary', 'btn-sm',))
  );

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'entity-picture-edit-form';

  return $form;
}

function hrm_backend_entity_picture_edit_picture_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_entity_picture_edit_picture_form_submit(&$form, &$form_state) {
  $picture = $form_state['build_info']['args'][0];
  db_update('hrm_entity_picture')
    ->fields(array(
      'caption' => $form_state['values']['caption'],
      'changed' => time()
    ))
    ->condition('id', $picture['id'], '=')
    ->execute();

  if (filter_input(INPUT_SERVER, 'HTTP_REFERER')) {
    $referer_url = explode('?', filter_input(INPUT_SERVER, 'HTTP_REFERER'));
    $redirect_link = $referer_url[0];
    $tab = 'pictures';
  }
  else {
    switch ($picture['entity']) {
      case 'asset':
        $redirect_link = 'asset/asset/details/' . $picture['entity_id'];
        $tab = 'pictures';
        break;
      case HR_ENTITY_DISCIPLINE:
        $redirect_link = 'discipline/details/' . $picture['entity_id'];
        $tab = 'pictures';
        break;
    }
  }

  drupal_set_message("picture has been updated");
  $commands = array();
  $commands[] = ctools_ajax_command_redirect($redirect_link, 0, array('query' => array('tab' => $tab)));
  print ajax_render($commands);
  exit;
}

/*
 * ========================================
 * Delete picture
 * ========================================
 */

function hrm_backend_entity_picture_delete_picture($picture_id) {
  $account = hrm_basic_account_get_info();
  $picture = hrm_backend_entity_picture_get_pictures_data(array(array('field' => 'id', 'value' => $picture_id, 'operator' => '=')), TRUE);
  db_delete('hrm_entity_picture')->condition('id', $picture_id, '=')->execute();
  hrm_basic_file_delete_file_data($picture['picture_fid'], 'hrm_backend');

  switch ($picture['entity']) {
    case 'activity':
      $fields = array('aid' => $picture['entity_id'], 'action' => RTP_ACTIVITY_HISTORY_ACTION_PHOTO_DELETE, 'uid' => $user->uid, 'created' => time(), 'data' => serialize($picture));
      db_insert('hrm_activity_history')
        ->fields($fields)
        ->execute();
      break;
    case 'partner':
      $fields = array('pid' => $picture['entity_id'], 'action' => RTP_PARTNER_HISTORY_ACTION_PHOTO_DELETE, 'uid' => $user->uid, 'created' => time(), 'data' => serialize($picture));
      db_insert('hrm_partner_history')
        ->fields($fields)
        ->execute();
      break;
    case 'rehab':
      $fields = array('rid' => $picture['entity_id'], 'action' => RTP_REHAB_HISTORY_ACTION_PHOTO_DELETE, 'uid' => $user->uid, 'created' => time(), 'data' => serialize($picture));
      db_insert('hrm_rehab_history')
        ->fields($fields)
        ->execute();
      break;
    case 'workshop':
      $fields = array('wid' => $picture['entity_id'], 'action' => RTP_WORKSHOP_HISTORY_ACTION_PHOTO_DELETE, 'uid' => $user->uid, 'created' => time(), 'data' => serialize($picture));
      db_insert('hrm_workshop_history')
        ->fields($fields)
        ->execute();
      break;
  }

  if (filter_input(INPUT_SERVER, 'HTTP_REFERER')) {
    $referer_url = explode('?', filter_input(INPUT_SERVER, 'HTTP_REFERER'));
    $redirect_link = $referer_url[0];
    $tab = 'pictures';
  }
  else {
    switch ($picture['entity']) {
      case 'asset':
        $redirect_link = 'asset/asset/details/' . $picture['entity_id'];
        $tab = 'pictures';
        break;
      case HR_ENTITY_DISCIPLINE:
        $redirect_link = 'discipline/details/' . $picture['entity_id'];
        $tab = 'pictures';
        break;
    }
  }

  drupal_set_message('picture deleted successfully');
  drupal_goto($redirect_link, array('query' => array('tab' => $tab)));
}

/*
 * ========================================
 * Entity picture data function
 * ========================================
 */

function hrm_backend_get_entity_picture_data($filters = array(), $first_item = FALSE) {

  $query = db_select('hrm_entity_picture', 'hrm_entity_picture')
    ->fields('hrm_entity_picture', array('id', 'oid', 'source', 'entity', 'entity_id', 'picture_fid', 'caption', 'ref_1', 'ref_2', 'uid', 'created', 'changed'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }

  $pictures = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  foreach ($pictures as $id => $picture) {
    $pictures[$id]['picture_uri'] = hrm_basic_file_get_managed_file_uri($picture['picture_fid'], HRM_DEFAULT_IMAGES_ASSET);
    $pictures[$id]['picture_url'] = file_create_url($pictures[$id]['picture_uri']);
//    $pictures[$id]['picture'] = theme('image_style', array('style_name' => HRM_IMAGE_STYLE_150_150, 'path' => $pictures[$id]['picture_url'], 'alt' => 'caption', 'title' => 'XX', 'attributes' => array('class' => array('img-circle'))));
  }



  if ($first_item) {
    $picture = reset($pictures);
    return $picture;
  }
  return $pictures;
}

/* Delete Entity Picture */

// CHECK AGAIN 
// Mohamed
function hrm_backend_entity_picture_delete_picture_form($form, &$form_state, $pid) {
  drupal_set_title('Delete Picture');

  $form['description'] = array('#markup' => '<div class="form-item form-group">Are you sure to delete Picture ?***</div>');

  $form['submit_suffix'] = array('#markup' => '<div class="form-group">'); // to make line border gone

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Yes, Delete',
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


function hrm_backend_entity_picture_delete_picture_form_submit($form, &$form_state){
  
}

