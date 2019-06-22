<?php

function hrm_services_get_announcements($empid) {
  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);
  $json = new stdClass;

  if (!empty($employee)) {
    $announcements = hrm_basic_announcement_get_employee_announcements($empid, $employee);
    $data = array();
    foreach ($announcements as $announcement) {
      $data[] = array(
        'id' => $announcement['aid'],
        'topic' => $announcement['title'],
        'description' => $announcement['description'],
        'publish_date' => date(HRM_DATE_SHORT, $announcement['publish_date']),
      );
    }
    $json->status = 200;
    $json->announcements = $data;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Announcments';
    $json->message = 'Account not found or blocked';
    return $json;
  }
  return $json;
}
