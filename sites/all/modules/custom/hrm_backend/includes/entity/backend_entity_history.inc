<?php

function hrm_backend_entity_history_log_action($entity, $entity_id, $action, $uid, $data = '', $created = NULL, $sub_entity = NULL, $sub_entity_id = 0) {
  if ($data != '') {
    if (is_array($data)) {
      $data = serialize($data);
    }
  }
  if (!$created) {
    $created = time();
  }
  db_insert('hrm_entity_history')
    ->fields(array(
      'entity' => $entity,
      'entity_id' => $entity_id,
      'sub_entity' => $sub_entity,
      'sub_entity_id' => $sub_entity_id,
      'action' => $action,
      'data' => $data,
      'uid' => $uid,
      'created' => $created,
    ))
    ->execute();
}

function hrm_backend_entity_history_get_history_action_data($entity, $entity_id, $action, $sort_direction = 'DESC', $sub_entity = NULL, $sub_entity_id = 0) {
  $action_operator = '=';
  if (is_array($action)) {
    $action_operator = 'IN';
  }
  $log_query = db_select('hrm_entity_history', 'entity_history')
    ->fields('entity_history', array('id', 'entity', 'entity_id', 'action', 'data', 'uid', 'created'))
    ->condition('entity', $entity, '=')
    ->condition('entity_id', $entity_id, '=')
    ->condition('action', $action, $action_operator);
  if ($sub_entity) {
    $log_query->condition('sub_entity', $sub_entity, '=');
  }
  if ($sub_entity_id) {
    $log_query->condition('sub_entity_id', $sub_entity_id, '=');
  }
  $log_query->orderBy('created', $sort_direction);
  $log_query->range(0, 1);
  $log = $log_query->execute()->fetch(PDO::FETCH_ASSOC);

  return $log;
}

function hrm_backend_entity_history_get_history_data($filters = array(), $sort = array(), $first_item = FALSE) {
  $query = db_select('hrm_entity_history', 'entity_history')
    ->fields('entity_history', array('id', 'entity', 'entity_id', 'action', 'data', 'uid', 'created'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }
  if (!empty($sort)) {
    foreach ($sort as $sort_field) {
      $query->orderBy($sort_field['field'], $sort_field['direction']);
    }
  }
  $result = $query->execute()->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  if ($first_item) {
    return reset($result);
  }

  return $result;
}

function hrm_backend_entity_history_get_history($filters = array(), $sort = array()) {
  $query = db_select('hrm_entity_history', 'entity_history')
    ->fields('entity_history', array('id', 'entity', 'entity_id', 'sub_entity', 'sub_entity_id', 'action', 'data', 'uid', 'created'));
  if (!empty($filters)) {
    foreach ($filters as $filter) {
      $operator = (isset($filter['operator']) ? $filter['operator'] : '=');
      $query->condition($filter['field'], $filter['value'], $operator);
    }
  }
  if (!empty($sort)) {
    foreach ($sort as $sort_field) {
      $query->orderBy($sort_field['field'], $sort_field['direction']);
    }
  }
  else {
    $query->orderBy('created', 'ASC');
  }
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $users = array();
  $users[0] = 'System';
  $users_roles = array();
  $users_roles[0] = 'System';

  $icons = hrm_basic_actions_icons();
  $titles = hrm_basic_actions_title();
  $contents = hrm_basic_actions_content();
  $entity_names = hrm_basic_entity_names();
  $entity_icons = hrm_basic_entity_icons();
  $roles = hrm_basic_role_names();
  $roles[0] = 'SYSTEM';

  $history = array();
  foreach ($result as $row) {
    $action = $row['action'];
    $owner_uid = $row['uid'];
    $entity = $row['entity'];
    $sub_entity = $row['sub_entity'];

    $data = $row['data'];
    $is_data_serialized = @unserialize($row['data']);
    if ($is_data_serialized !== false) {
      $data = $is_data_serialized;
    }
    if ($owner_uid && !isset($users[$owner_uid])) {
      $users[$owner_uid] = hrm_basic_account_get_name($owner_uid);
    }
    if ($owner_uid && !isset($users_roles[$owner_uid])) {

      $rid = hrm_basic_account_get_role($owner_uid);
      $users_roles[$owner_uid] = $roles[$rid];
    }

    $icon = $icons[$action];

    $title = str_replace(array('@[entity]', '@[name]'), array($entity_names[$entity], $users[$owner_uid]), $titles[$action]);


    $time = date(HRM_DATE_SHORT_DAY_NAME_TIME_12, $row['created']);

    if ($sub_entity) {
      $entity = $sub_entity;
      $icon = $entity_icons[$sub_entity] . ' ' . $icon;
      $title = str_replace(array('@[entity]', '@[name]'), array($entity_names[$sub_entity], $users[$owner_uid]), $titles[$action]);
    }

    $content_raw = '';
//print("<pre>");
//print_r($data);
//exit();
    switch ($action) {
      case HRM_ACTION_ADD:
        //@[entity] created by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_EDIT:
        //@[entity] edited by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_DRAFT:
        //Saved as draft by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_SAVE:
        //@[entity] saved by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        break;

      case HRM_ACTION_SUBMIT:
        //Submitted by <strong>@[action_owner_name]</strong> for approval
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_REJECT:
        //Rejected by <strong>@[action_owner_name]</strong> for the following reason:<br /> @[justification]
        if ($row['sub_entity'] == HRM_ENTITY_EMPLOYEE) {
          $rejected_by = hrm_basic_employee_get_name($row['sub_entity_id']);
        }
        else {
          $rejected_by = $users[$owner_uid];
        }
        $content_raw = str_replace(
          array('@[action_owner_name]', '@[justification]'), array($rejected_by, $data['justification']), $contents[$action]);
 
        break;
      case HRM_ACTION_APPROVE:
        //Approved by <strong>@[action_owner_name]</strong> 
        if ($row['sub_entity'] == HRM_ENTITY_EMPLOYEE) {
          $approved_by = hrm_basic_employee_get_name($row['sub_entity_id']);
        }
        else {
          $approved_by = $users[$owner_uid];
        }
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($approved_by), $contents[$action]);

        if (!empty($data['justification'])) {
          $content_raw .= '<p>' . $data['justification'] . '</p>';
        }

        break;

      case HRM_ACTION_APPLY:
        //@[entity] applied by <strong>@[action_owner_name]</strong> @[justification]
        if ($owner_uid == 0) {
          $content_raw = $entity_names[$entity] . ' applied automatically by the <strong>System</strong>';
        }
        else {
          $justification = ((isset($row['data']) && !empty($row['data'])) ? '<br />' . $row['data'] : '');
          $content_raw = str_replace(
            array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        }

        break;
      case HRM_ACTION_ACTIVATE:
        //@[entity] activated by <strong>@[action_owner_name]</strong> @[justification]
        if ($owner_uid == 0) {
          $content_raw = $entity_names[$entity] . ' activated automatically by the <strong>System</strong>';
        }
        else {
          $justification = ((isset($row['data']) && !empty($row['data'])) ? '<br />' . $row['data'] : '');
          $content_raw = str_replace(
            array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        }

        break;
      case HRM_ACTION_START:
        //@[entity] started by <strong>@[action_owner_name]</strong> @[justification]
        if ($owner_uid == 0) {
          $content_raw = $entity_names[$entity] . ' started automatically by the <strong>System</strong>';
        }
        else {
          $justification = ((isset($row['data']) && !empty($row['data'])) ? '<br />' . $row['data'] : '');
          $content_raw = str_replace(
            array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        }

        break;
      case HRM_ACTION_BLOCKED:
        //@[entity] blocked by <strong>@[action_owner_name]</strong> @[justification]
        $justification = ((isset($row['data']) && !empty($row['data'])) ? '<br />' . $row['data'] : '');
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        break;

      case HRM_ACTION_OPEN:
        //@[entity] opened by <strong>@[action_owner_name]</strong> @[justification]
        $justification = ((isset($row['data']) && !empty($row['data'])) ? '<br />' . $row['data'] : '');
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        break;
      case HRM_ACTION_CLOSE:
        //@[entity] closed by <strong>@[action_owner_name]</strong> @[justification]
        $justification = ((isset($row['data']) && !empty($row['data'])) ? '<br />' . $row['data'] : '');
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        break;

      case HRM_ACTION_PUBLISHED:
        //@[entity] published by <strong>@[action_owner_name]</strong> @[justification]
        $justification = ((isset($row['data']) && !empty($row['data'])) ? '<br />' . $row['data'] : '');
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        break;
      case HRM_ACTION_UNPUBLISHED:
        //@[entity] unpublished by <strong>@[action_owner_name]</strong> @[justification]
        $justification = ((isset($row['data']) && !empty($row['data'])) ? '<br />' . $row['data'] : '');
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        break;
      case HRM_ACTION_ARCHIVED:
        //Activity has been archived by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_REVERT:
        //@[entity] reverted by <strong>@[action_owner_name]</strong>
        //<br />Reverted revision: @[reverted_revision]
        //<br />New revision: @[new_revision]
        //<br /><p>For the following justification 
        //<br /> @[justification]</p>

        $data = unserialize($row['data']);

        $justification = '';
        if (isset($data['justification']) && !empty($data['justification'])) {
          $justification = $data['justification'];
        }
        elseif (isset($data['notes']) && !empty($data['notes'])) {
          $justification = $data['notes'];
        }
        $reverted_revision = ((isset($data['reverted_revision']) && !empty($data['reverted_revision'])) ? $data['reverted_revision'] : '');
        $new_revision = ((isset($data['new_revision']) && !empty($data['new_revision'])) ? $data['new_revision'] : '');

        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]', '@[reverted_revision]', '@[new_revision]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $reverted_revision, $new_revision, $justification), $contents[$action]);
        break;
      case HRM_ACTION_CANCELED:
        //Canceled by <strong>@[action_owner_name]</strong> for the following reason:<br /> @[justification]
        $content_raw = str_replace(
          array('@[action_owner_name]', '@[justification]'), array($users[$owner_uid], $row['data']), $contents[$action]);
        break;

      case HRM_ACTION_SUSPEND:
        $data = unserialize($row['data']);
        $reason = (isset($data['reason']) ? hrm_basic_taxonomy_term_name($data['reason']) : '');
        $justification = (isset($data['justification']) ? $data['justification'] : '');
        if ($justification == '') {
          $justification = (isset($data['description']) ? $data['description'] : '');
        }

        $content_raw = str_replace(
          array('@[action_owner_name]', '@[reason]', '@[justification]'), array($users[$owner_uid], $reason, $justification), $contents[$action]);
        break;
      case HRM_ACTION_DROP_OUT:
        $data = unserialize($row['data']);

        $drop_out_document = '';
        if (isset($data['document'])) {
          $document = file_load($data['document']);
          if ($document) {
            $drop_out_document = '<a href="' . file_create_url($document->uri) . '" download>Drop out document</a>';
          }
        }

        $justification = (isset($data['justification']) ? $data['justification'] : '');
        if ($justification == '') {
          $justification = (isset($data['description']) ? $data['description'] : '');
        }

        $content_raw = str_replace(
          array('@[action_owner_name]', '@[document]', '@[justification]'), array($users[$owner_uid], $drop_out_document, $justification), $contents[$action]);
        break;
      case HRM_ACTION_DELETE:
        $data = unserialize($row['data']);
        $justification = ((isset($data['justification']) && !empty($data['justification'])) ? $data['justification'] : '');
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]', '@[justification]'), array($entity_names[$entity], $users[$owner_uid], $justification), $contents[$action]);
        break;
      case HRM_ACTION_TRANSFER:
        $data = unserialize($row['data']);
        $justification = ((isset($data['justification']) && !empty($data['justification'])) ? $data['justification'] : '');
        $from = hrm_basic_entity_get_entity_name($data['from']['entity'], $data['from']['entity_id'], HRM_DB_FIELD_ENGLISH_NAME);
        $to = hrm_basic_entity_get_entity_name($data['from']['entity'], $data['from']['entity_id'], HRM_DB_FIELD_ENGLISH_NAME);

        $content_raw = str_replace(
          array('@[action_owner_name]', '@[from]', '@[to]', '@[justification]'), array($users[$owner_uid], $from, $to, $justification), $contents[$action]);
        break;
      case HRM_ACTION_ASSIGN:
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_ASSIGN_TO_OTHER:
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_RETURN:
        //@[entity] return by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        break;

      case HRM_ACTION_REASSIGN:
        //@[entity] re-assigned by <strong>@[action_owner_name]</strong>
        $data = unserialize($row['data']);
        $justification = (isset($data['justification']) ? $data['justification'] : '');

        $content_raw = str_replace(
          array('@[entity]', '@[action_owner_name]'), array($entity_names[$entity], $users[$owner_uid]), $contents[$action]);
        if (!empty($justification)) {
          $content_raw .= ' for the following reason: <br />' . $justification;
        }
        break;
      case HRM_ACTION_ENABLE:
        //'@[entity] @[description] enabled by <strong>@[action_owner_name]</strong>'        
        $data = unserialize($row['data']);
        $title = str_replace(array('@[entity]', '@[title]'), array($entity_names[$entity], $data['title']), $titles[$action]);
        $content_raw = str_replace(
          array('@[entity]', '@[description]', '@[action_owner_name]'), array($entity_names[$entity], $data['description'], $users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_DISABLE:
        //'@[entity] @[description] disabled by <strong>@[action_owner_name]</strong>'
        $data = unserialize($row['data']);
        $title = str_replace(array('@[entity]', '@[title]'), array($entity_names[$entity], $data['title']), $titles[$action]);
        $content_raw = str_replace(
          array('@[entity]', '@[description]', '@[action_owner_name]'), array($entity_names[$entity], $data['description'], $users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_ADJUST_VALUE:
        $project_text = (isset($data['project']) && !empty($data['project']) ? '<br /><strong>Project</strong>: ' . hrm_basic_project_get_name_formatted($data['project']) : '');
        $description_text = (isset($data['description']) && !empty($data['description']) ? '<br />' . $data['description'] : '');

        $title = str_replace(array('@[entity]', '@[value_label]'), array($entity_names[$entity], ucfirst($data['value_label'])), $titles[$action]);
        $content_raw = str_replace(
          array('@[entity]', '@[value_label]', '@[action_owner_name]', '@[old_value]', '@[new_value]', '@[project]', '@[description]'), array($entity_names[$entity], $data['value_label'], $users[$owner_uid], $data['old_value'], $data['new_value'], $project_text, $description_text), $contents[$action]);
        break;
      case HRM_ACTION_STATUS_UPDATE:
        //@[entity] @[title] status has been updated by '@[action_owner_name]'.
        //prev status: @[prev_status]
        //new status: @[new_status]
//        $data = unserialize($row['data']);
        $title = str_replace(array('@[entity]', '@[title]'), array($entity_names[$entity], ucfirst($data['title'])), $titles[$action]);
        $content_raw = str_replace(
            array('@[entity]', '@[title]', '@[action_owner_name]', '@[prev_status]', '@[new_status]'), array($entity_names[$entity], $data['title'], $users[$owner_uid], $data['prev_status']['label'], $data['new_status']['label']), $contents[$action]) . '<br />';
        break;
      //Document
      case HRM_ACTION_DOCUMENT_ADD:
        //Documents added by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_DOCUMENT_EDIT:
        //Document edited by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_DOCUMENT_DELETE:
        //Documents deleted by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      //Pictures
      case HRM_ACTION_PICTURE_ADD:
        //Pictures added by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_PICTURE_EDIT:
        //Picture edited by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_PICTURE_DELETE:
        //Picture deleted by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      //Video
      case HRM_ACTION_VIDEO_ADD:
        //Video added by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_VIDEO_EDIT:
        //Video edited by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;
      case HRM_ACTION_VIDEO_DELETE:
        //Video deleted by <strong>@[action_owner_name]</strong>
        $content_raw = str_replace(
          array('@[action_owner_name]'), array($users[$owner_uid]), $contents[$action]);
        break;

      //App reports      
      case HRM_ACTION_PUNCH_IN:
        $content_raw = 'Punch in';
        break;
      case HRM_ACTION_PUNCH_OUT:
        $content_raw = 'Punch out';
        break;
    }

    if (is_array($data) && isset($data['justification']) && !empty($data['justification'])) {
      $content_raw .= 'for the following reason: ' . $data['justification'];
    }

    if (is_array($data) && isset($data['attachments']) && !empty($data['attachments'])) {
      $content_raw .= '<div class="strong">Attachments:</div>';
      $content_raw .= '<ul>';
      foreach ($data['attachments'] as $fid) {
        $file = file_load($fid);
        if ($file) {
          $file_url = file_create_url($file->uri);
          $content_raw .= '<li>' . l($file->filename, $file_url, array('attributes' => array('target' => '_blank'), 'html' => TRUE)) . '</li>';
        }
      }
      $content_raw .= '</ul>';
    }

    if ($content_raw != '') {
      $history[] = array(
        'icon' => '<div class="history-log-icon">' . $icon . '</div>',
        'time' => $time,
        'title' => $title,
        'content' => check_markup($content_raw, 'full_html'),
      );
    }
  }

  return $history;
}
