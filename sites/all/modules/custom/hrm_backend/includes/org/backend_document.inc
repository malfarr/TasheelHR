<?php

function hrm_backend_document() {
  $account = hrm_basic_account_get_info();
  if (empty($account) || !isset($account['oid']) || !$account['oid']) {
    drupal_access_denied();
    exit();
  }

  $add_documents = hrm_backend_documents_permission(HRM_PERM_DOCUMENT_ADD, $account);
  $edit_documents = hrm_backend_documents_permission(HRM_PERM_DOCUMENT_EDIT, $account);
  $delete_documents = hrm_backend_documents_permission(HRM_PERM_DOCUMENT_DELETE, $account);
  $document_buttons = array();

  $documents_filters = array(
    array('field' => 'entity', 'value' => HRM_ENTITY_DOCUMENT),
    array('field' => 'entity_id', 'value' => $account['oid']),
  );
  if ($account['bid']) {
    $documents_filters[] = array('field' => 'ref_1', 'value' => array(0, $account['bid']), 'operator' => 'IN');
  }

  $documents_list = hrm_backend_entity_document_get_documents_list($documents_filters, $edit_documents, $delete_documents);

  if ($add_documents) {
    $document_buttons[] = l(HRM_FA_PLUS . ' Add Document', 'hrm-modal', array('query' => array('action' => HRM_MODAL_EN_DOCUMENT_ADD, 'entity' => HRM_ENTITY_DOCUMENT, 'entity_id' => $account['oid'], 'oid' => $account['oid']), 'attributes' => array('class' => array('btn', 'btn-sm', 'btn-primary', 'pull-right')), 'html' => TRUE));
  }
  $variables = array(
    'title' => '<strong> d</strong> ',
    'list' => $documents_list,
    'buttons' => $document_buttons
  );

  return theme('page_wrapper_entity_list_no_header', $variables);
}

function hrm_backend_documents_permission($action, $account = array()) {
  if (empty($account)) {
    $account = hrm_basic_account_get_info();
  }

  $permissions = (isset($data['permissions']) && !empty($data['permissions']) ? $data['permissions'] : hrm_basic_role_permissions($account['role']));

  if (!isset($account['oid']) || !$account['oid']) {
    return FALSE;
  }
  switch ($action) {
    case HRM_PERM_DOCUMENT_ADD:
      if (isset($permissions[HRM_PERM_DOCUMENT_ADD])) {
        return TRUE;
      }
      break;
    case HRM_PERM_DOCUMENT_EDIT:
      if (isset($permissions[HRM_PERM_DOCUMENT_EDIT])) {
        return TRUE;
      }
      break;
    case HRM_PERM_DOCUMENT_DELETE:
      if (isset($permissions[HRM_PERM_DOCUMENT_DELETE])) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}
