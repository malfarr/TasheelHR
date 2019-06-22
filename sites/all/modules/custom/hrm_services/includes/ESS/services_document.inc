<?php

function hrm_services_document_get_documents($empid) {
  $emp_filter = array(
    array('field' => 'empid', 'value' => $empid, 'operator' => '='),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '=')
  );
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee)) {
    $oid = $employee['oid'];

    $documents_filters = array(
      array('field' => 'entity', 'value' => HRM_ENTITY_DOCUMENT, 'operator' => '='),
      array('field' => 'entity_id', 'value' => $oid, 'operator' => '='),
    );
    $documents = hrm_backend_entity_document_get_entity_documents($documents_filters);

    $data = array();
    foreach ($documents as $document) {
      $data[] = array(
        'id' => $document['id'],
        'title' => $document['title'],
        'description' => $document['description'],
        'url' => $document['url'],
        'date' => date(HRM_DATE_SHORT, $document['changed']),
      );
    }
    $json->status = 200;
    $json->documents = $data;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Documents';
    $json->message = 'Employee not found or blocked';
  }
  return $json;
}

function hrm_services_document_email_document($empid, $document_id) {
  $emp_filter = array(
    array('field' => 'empid', 'value' => $empid, 'operator' => '='),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '=')
  );
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;
  if (!empty($employee)) {
    $oid = $employee['oid'];

    $documents_filters = array(
      array('field' => 'id', 'value' => $document_id, 'operator' => '='),
      array('field' => 'entity_id', 'value' => $oid, 'operator' => '='),
    );
    $documents = hrm_backend_entity_document_get_entity_documents($documents_filters);


    if (!empty($documents)) {
      $document = reset($documents);
      $email_sent = hrm_emails_documents_email_document($employee, $document);
      if ($email_sent) {
        $json->status = 200;
        $json->message = 'Document has been emailed to you';
      }
      else {
        $json->status = 404;
        $json->message_title = 'Documents';
        $json->message = 'Document has not been emailed';
      }
    }
    else {
      $json->status = 404;
      $json->message_title = 'Documents';
      $json->message = 'Document does not exist';
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Documents';
    $json->message = 'Employee not found or blocked';
  }
  return $json;
}
