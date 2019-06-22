<?php

function hrm_services_app_check_version($version) {

  $json = new stdClass;

  $app_version = variable_get(HRM_VAR_ADMIN_APP_VERSION, '');

  if ($version == $app_version) {
    $json->status = 200;
  }
  else {
    $json->message_title = 'ResponseHRM Updates';
    $json->message = 'New updates available';
    $json->status = 404;
  }
  return $json;
}

function hrm_services_app_check_device($empid, $device_id) {
  db_update('hrm_emp')
    ->fields(array(
      'device_id' => $device_id,
    ))
    ->condition('empid', $empid, '=')
    ->execute();
  $json = new stdClass;
  $json->message = 'Successfully device updated';
  $json->status = 200;
  return $json;
}
