<?php

function hrm_backend_organization_details_page() {
  $account = hrm_basic_account_get_info();

  if (user_access(HRM_PERM_ORG_ACCESS_ORGANIZATION)) {
    $oid = (isset($account['oid']) && !empty($account['oid']) ? $account['oid'] : 0);
    $organization = hrm_basic_organization_get_data(array(array('field' => 'oid', 'value' => $oid)), TRUE);
    if (!empty($organization)) {
      drupal_set_title($organization['name']);
      $entity_filters = array(
        array('field' => 'entity', 'value' => HRM_ENTITY_ORGANIZATION),
        array('field' => 'entity_id', 'value' => $oid),
      );
      $details_output = hrm_backend_organization_display_details($organization);
      $details_buttons = hrm_backend_organization_get_details_buttons($organization, $account);
      $history_sort = array(array('field' => 'created', 'direction' => 'ASC'));
      $history_logs = hrm_backend_entity_history_get_history($entity_filters, $history_sort);
      $organization_history = theme('section_entity_history', array('history' => $history_logs, 'title' => 'Organization history log'));

      $variables = array(
        'entity' => HRM_ENTITY_ORGANIZATION,
        'details' => $details_output,
        'history' => $organization_history,
        'buttons' => $details_buttons
      );

      if (user_access(HRM_PERM_ORG_MEDIA_ACCESS)) {
        $add_documents = user_access(HRM_PERM_ORG_MEDIA_ADD);
        $edit_documents = user_access(HRM_PERM_ORG_MEDIA_EDIT);
        $delete_documents = user_access(HRM_PERM_ORG_MEDIA_DELETE);
        $documents_filters = array(
          array('field' => 'entity', 'value' => HRM_ENTITY_ORGANIZATION, 'operator' => '='),
          array('field' => 'entity_id', 'value' => $oid, 'operator' => '='),
        );
        $documets_fields = array();
        $documets_fields[] = array('type' => 'table', 'row_class' => array('noborder'),
          'value' => hrm_backend_entity_document_get_documents_list($documents_filters, $edit_documents, $delete_documents), 'responsive' => TRUE);
        $documnts_content = theme('section_entity_details', array('fields' => $documets_fields));
        $documents_buttons = array();
        if ($add_documents) {
          $documents_buttons[] = l(HRM_FA_PLUS . 'Add documents', 'hrm-modal', array('query' => array('action' => HRM_MODAL_EN_DOCUMENT_ADD, 'entity' => HRM_ENTITY_ORGANIZATION, 'entity_id' => $oid, 'oid' => $oid), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-success')), 'html' => TRUE));
        }
        $variables['documents'] = array('content' => $documnts_content, 'buttons' => $documents_buttons);
      }

      $status_output = hrm_basic_status_icons($organization['status']) . ' ' . hrm_basic_status_names($organization['status']);
      $variables['status'] = $status_output;
      return theme('page_wrapper_entity_details', $variables);
    }
    else {
      drupal_not_found();
      exit();
    }
  }
  else {
    drupal_access_denied();
    exit();
  }
}

function hrm_backend_organization_display_details($organization) {
  $fields = array();

  $logo_uri = hrm_basic_file_get_managed_file_uri($organization['logo'], HRM_DEFAULT_IMAGES_ORGANIZATION_LOGO);
  $logo_url = file_create_url($logo_uri);
  $logo_image = theme('image_style', array('style_name' => HRM_IMAGE_STYLE_150_150, 'path' => $logo_uri, 'alt' => $organization['name'], 'attributes' => array('class' => array('img-circle'))));
  $organization_header = '
    <div class="gallery custom-gallery widget text-center" data-toggle="lightbox-gallery">
        <a href="' . $logo_url . '" class="gallery-link" title="' . $organization['name'] . '">
          ' . $logo_image . '
        </a>
    </div>';
  $organization_header .= '<h2>' . $organization['name'] . '</h2>';
  $fields[] = array('type' => 'raw', 'wrapper_class' => array('col-sm-12', 'photos-row', 'text-center', 'strong'),
    'value' => $organization_header, 'row_class' => array('no-border'));

  

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
    'label' => 'Number of locations', 'value_class' => array(),
    'value' => count($organization['locations']), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-4'), 'row_class' => array(),
    'label' => 'Number of active employees', 'value_class' => array(),
    'value' => count($organization['employees']), 'value_class' => array());


  $fields[] = array('type' => 'header', 'row_class' => array('col-sm-12'),
    'title' => 'Address', 'value_class' => array(),);


  $address = $organization['address'];

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Phone', 'value_class' => array(),
    'value' => (isset($address['phone']) ? $address['phone'] : ''), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Mobile', 'value_class' => array(),
    'value' => (isset($address['mobile']) ? $address['mobile'] : ''), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Fax', 'value_class' => array(),
    'value' => (isset($address['fax']) ? $address['fax'] : ''), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Email', 'value_class' => array(),
    'value' => (isset($address['email']) ? $address['email'] : ''), 'value_class' => array());

  $street_1 = (isset($address['street_1']) ? $address['street_1'] : '');
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Street 1', 'value_class' => array(),
    'value' => $street_1, 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Street 2', 'value_class' => array(),
    'value' => (isset($address['street_2']) ? $address['street_2'] : ''), 'value_class' => array());

  $city = (isset($address['city']) ? $address['city'] : '');
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'City', 'value_class' => array(),
    'value' => $city, 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Region', 'value_class' => array(),
    'value' => (isset($address['region']) ? $address['region'] : ''), 'value_class' => array());

  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Zip/Postal code', 'value_class' => array(),
    'value' => (isset($address['postal_code']) ? $address['postal_code'] : ''), 'value_class' => array());

  $country = (isset($address['country']) ? hrm_basic_lov_get_term_field_data(HRM_LOV_COUNTRY, $address['country']) : '');
  $fields[] = array('type' => 'row', 'wrapper_class' => array('col-sm-6'), 'row_class' => array(),
    'label' => 'Country', 'value_class' => array(),
    'value' => $country, 'value_class' => array());

  if (isset($address['coordinates'])) {
    $info_box_content = '<div>' . $street_1 . '</div>';
    $info_box_content .= '<div>' . $city . '</div>';
    $info_box_content .= '<div>' . $country . '</div>';
    $info_box_content .= '<div>' . $address['coordinates'] . '</div>';
    $map_marker_content = '<div class="infobox-wrapper">';
    $map_marker_content .= '<div class="infobox-title-wrapper" style="color: #fff; background-color: ' . HRM_COLOR_PIMARY . '"> ' . $organization['name'] . '</div>';
    $map_marker_content .= '<div class="infobox-content-wrapper"><div class="infobox-text-wrapper">' . hrm_basic_text_fix_java_script_text($info_box_content) . '</div></div>';
    $map_marker_content .= '</div>';
    $markers[] = array(
      'coordinates' => $address['coordinates'],
      'content' => $map_marker_content,
      'icon' => file_create_url(HRM_MARKER_BLUE),
    );

    $map_output = theme('map_view_locations', array(
      'div_id' => 'organization-location-map',
      'center' => $address['coordinates'],
      'zoom' => 12,
      'markers' => json_encode($markers),
      'class' => array('map-infobox-style')));

    $fields[] = array('type' => 'raw', 'wrapper_class' => array('col-sm-12'), 'row_class' => array(),
      'value' => $map_output, 'value_class' => array());
  }

  return theme('section_entity_details', array('fields' => $fields));
}

function hrm_backend_organization_get_details_buttons($organization, $account = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }
  $oid = $organization['oid'];

  $buttons = array();
  if (user_access(HRM_PERM_ORG_EDIT_ORGANIZATION) && isset($account['oid']) && $account['oid'] == $oid) {
    $buttons[] = l(HRM_FA_PENCIL . 'Edit Organization', 'organization/edit', array('attributes' => array('class' => array('btn', 'btn-primary', 'btn-sm')), 'html' => TRUE));
  }

  return $buttons;
}

function hrm_backend_organization_form_page() {
  $account = hrm_basic_account_get_info();

  if (user_access(HRM_PERM_ORG_ACCESS_ORGANIZATION)) {
    $oid = (isset($account['oid']) && !empty($account['oid']) ? $account['oid'] : 0);
    $organization = hrm_basic_organization_get_data(array(array('field' => 'oid', 'value' => $oid)), TRUE);
    if (!empty($organization)) {
      $organization_form = drupal_get_form('hrm_backend_organization_form', $organization);
      $variables = array(
        'entity' => HRM_ENTITY_ORGANIZATION,
        'title' => 'Organization information',
        'form' => $organization_form,
      );
      return theme('page_wrapper_entity_form', $variables);
    }
    else {
      drupal_not_found();
      exit();
    }
  }
  else {
    drupal_access_denied();
    exit();
  }
}

function hrm_backend_organization_form($form, &$form_state, $organization) {

  $oid = $organization['oid'];

  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 400,
    '#attributes' => array('class' => array()),
    '#default_value' => $organization['name']
  );
  $form['logo'] = array(
    '#title' => 'Logo',
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#upload_validators' => array('file_validate_extensions' => array(HRM_FILE_EXT_PICTURE),),
    '#upload_location' => strtr(HRM_DIR_ENTITY_LOGO, array('%entity' => HRM_ENTITY_ORGANIZATION, '%entity_id' => $oid)),
    '#default_value' => $organization['logo']
  );

  $address = (isset($organization['address']) ? $organization['address'] : array());

  $form['phone'] = array(
    '#title' => 'Phone',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['phone']) ? $address['phone'] : '')
  );
  $form['mobile'] = array(
    '#title' => 'Mobile',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['mobile']) ? $address['mobile'] : '')
  );
  $form['fax'] = array(
    '#title' => 'Fax',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['fax']) ? $address['fax'] : '')
  );
  $form['email'] = array(
    '#title' => 'Email',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['email']) ? $address['email'] : '')
  );
  $form['street_1'] = array(
    '#title' => 'Street address',
    '#type' => 'textfield', '#required' => TRUE,
    '#maxlength' => 400,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['street_1']) ? $address['street_1'] : '')
  );
  $form['street_2'] = array(
    '#title' => 'Street address 2',
    '#type' => 'textfield',
    '#required' => FALSE,
    '#maxlength' => 400,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['street_2']) ? $address['street_2'] : '')
  );
  $form['city'] = array(
    '#title' => 'City',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['city']) ? $address['city'] : '')
  );
  $form['region'] = array(
    '#title' => 'Region/State',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['region']) ? $address['region'] : '')
  );
  $form['postal_code'] = array(
    '#title' => 'Postal/Zip code',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#attributes' => array('class' => array()),
    '#default_value' => (isset($address['postal_code']) ? $address['postal_code'] : '')
  );


  $countries_list = hrm_basic_lov_get_terms(HRM_LOV_COUNTRY);
  $form['branch'] = array(
    '#title' => 'Branch',
    '#type' => 'select',
    '#options' => $countries_list,
    '#required' => TRUE,
    '#empty_option' => '', '#attributes' => array('class' => array('select-chosen')),
    '#default_value' => (isset($address['country']) ? $address['country'] : '')
  );

  $location_map_variables = array(
    'title' => 'Location Coordinates <span title="This field is required." class="form-required">*</span>',
    'hidden_id' => 'organization-coordinates',
    'center' => '0,0',
    'form_item' => TRUE
  );
  if (isset($address['coordinates']) && !empty($address['coordinates'])) {
    $location_map_variables['marker'] = $address['coordinates'];
    $location_map_variables['center'] = $address['coordinates'];
  }
  $form['location'] = array('#markup' => theme('map_set_location', $location_map_variables));
  $form['coordinates'] = array(
    '#type' => 'hidden',
    '#title' => 'Organization coordinates',
    '#default_value' => (isset($address['coordinates']) ? $address['coordinates'] : ''),
    '#required' => TRUE,
    '#attributes' => array('id' => 'organization-coordinates'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Organization'),
    '#attributes' => array('class' => array('btn-primary', 'btn-sm')),
    '#prefix' => '<div class="form-group form-actions">',
    '#suffix' => '</div>',
  );

  $form['#attributes']['class'][] = 'form-bordered';
  $form['#attributes']['class'][] = 'organization-edit-form';

  return $form;
}

function hrm_backend_organization_form_validate(&$form, &$form_state) {
  
}

function hrm_backend_organization_form_submit(&$form, &$form_state) {
  global $user;

  $organization = $form_state['build_info']['args'][0];

  db_update('hrm_org')
    ->fields(array(
      'name' => $form_state['values']['name'],
      'logo' => $form_state['values']['logo'],
    ))
    ->condition('oid', $organization['oid'], '=')
    ->execute();

  if ($organization['logo'] != $form_state['values']['logo']) {
    hrm_basic_file_save_file_data($form_state['values']['logo'], HRM_MODULE_BACKEND, strtr(HRM_FILE_USAGE_ENTITY_LOGO, array('%entity' => HRM_ENTITY_ORGANIZATION)), $organization['oid']);

    if ($organization['logo']) {
      hrm_basic_file_delete_file_data($organization['logo'], HRM_MODULE_BACKEND);
    }
  }

  $coordinates = str_replace(array('(', ')', ' '), array('', '', ''), $form_state['values']['coordinates']);
  $address_fields = array(
    'phone' => $form_state['values']['phone'],
    'mobile' => $form_state['values']['mobile'],
    'fax' => $form_state['values']['fax'],
    'email' => $form_state['values']['email'],
    'street_1' => $form_state['values']['street_1'],
    'street_2' => $form_state['values']['street_2'],
    'city' => $form_state['values']['city'],
    'region' => $form_state['values']['region'],
    'postal_code' => $form_state['values']['postal_code'],
    'country' => $form_state['values']['branch'],
    'coordinates' => $coordinates,
    'changed' => time(),
  );

  if (isset($organization['address']) && !empty($organization['address'])) {
    db_update('hrm_entity_address')
      ->fields($address_fields)
      ->condition('oid', $organization['oid'], '=')
      ->condition('entity', HRM_ENTITY_ORGANIZATION, '=')
      ->condition('entity_id', $organization['oid'], '=')
      ->execute();
  }
  else {
    $address_fields['oid'] = $organization['oid'];
    $address_fields['entity'] = HRM_ENTITY_ORGANIZATION;
    $address_fields['entity_id'] = $organization['oid'];

    $address_fields['uid'] = $user->uid;
    $address_fields['created'] = time();
    db_insert('hrm_entity_address')->fields($address_fields)->execute();
  }

  hrm_backend_entity_history_log_action(HRM_ENTITY_ORGANIZATION, $organization['oid'], HRM_ACTION_EDIT, $user->uid);

  drupal_set_message('Organization updated successfully***');
  drupal_goto('organization');
}
