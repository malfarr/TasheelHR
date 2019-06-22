<?php

function hrm_services_asset_get_assets($uid) {
  $json = new stdClass;
  $account = hrm_basic_account_get_info($uid);
  if (!empty($account) && $account['status'] == 1) {
    $branch_filters = array(array('field' => 'oid', 'value' => $account['oid']));
    if ($account['bid']) {
      $branch_filters[] = array('field' => 'bid', 'value' => $account['bid']);
    }
    $assets = hrm_basic_asset_get_data($branch_filters);
    $vendors = hrm_basic_asset_vendor_names($branch_filters);
    $brands = hrm_basic_asset_brand_names($branch_filters);
    $categories = hrm_basic_asset_category_names($branch_filters);

    $data = array();
    $status_names = hrm_basic_status_names();

    $locations = hrm_basic_location_names($branch_filters);
    foreach ($assets as $asset) {
      $photo_uri = hrm_basic_file_get_managed_file_uri($asset['picture_fid'], HRM_DEFAULT_IMAGES_ASSET);
      $photo_url = file_create_url($photo_uri);

      $asset_data = array(
        'aid' => $asset['aid'],
        'oid' => $asset['oid'],
        'bid' => $asset['bid'],
        'asset_no' => $asset['asset_no'],
        'serial_no' => $asset['serial_no'],
        'location' => $locations[$asset['location']],
        'model' => $asset['model'],
        'brand' => $brands[$asset['brand']],
        'category' => $categories[$asset['category']],
        'vendor' => $vendors[$asset['vendor']],
        'image' => $photo_url,
        'cost' => $asset['cost'],
        'current_value' => $asset['current_value'],
        'acquired' => $asset['acquired'],
        'warranty_start' => $asset['warranty_start'],
        'warranty_end' => $asset['warranty_end'],
        'description' => $asset['description'],
        'status' => $status_names[$asset['status']],
      );

      $data[] = $asset_data;
    }

    $json->status = 200;
    $json->assets = $data;
    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Get assets';
    $json->message = 'This account is not found or blocked***';
    return $json;
  }
}

function hrm_services_asset_get_barcode_data($uid, $barcode_data) {
  $json = new stdClass;
  $account = user_load($uid);

  if (!empty($account) && $account->status == 1) {
    $barcode_data_object = json_decode($barcode_data);
    $barcode_data_array = object_to_array($barcode_data_object);
    $aid = $barcode_data_array['aid'];
    $oid = $barcode_data_array['oid'];
    $asset = hrm_basic_asset_get_data(array(array('field' => 'aid', 'value' => $aid)), TRUE);
    $status_names = hrm_basic_status_names();


    if (!empty($asset)) {
      $photo_uri = hrm_basic_file_get_managed_file_uri($asset['picture_fid'], HRM_DEFAULT_IMAGES_ASSET);
      $photo_url = file_create_url($photo_uri);

      $json->status = 200;
      $asset_data = array(
        'aid' => $asset['aid'],
        'oid' => $asset['oid'],
        'asset_no' => $asset['asset_no'],
        'serial_no' => $asset['serial_no'],
        'location' => hrm_basic_location_get_name($asset['location']),
        'model' => $asset['model'],
        'brand' => hrm_basic_asset_brand_get_name($asset['brand']),
        'category' => hrm_basic_asset_category_get_name($asset['category']),
        'vendor' => hrm_basic_asset_vendor_get_name($asset['vendor']),
        'image' => $photo_url,
        'cost' => $asset['cost'],
        'current_value' => $asset['current_value'],
        'acquired' => $asset['acquired'],
        'warranty_start' => $asset['warranty_start'],
        'warranty_end' => $asset['warranty_end'],
        'description' => $asset['description'],
        'status' => $status_names[$asset['status']],
      );
      $json->asset = $asset_data;
    }
    else {
      $json->status = 404;
      $json->message_title = 'Get barcode';
      $json->message = 'Asset Not Found***';
    }
    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Get barcode';
    $json->message = 'This account is not found or blocked';
    return $json;
  }
}

function hrm_services_asset_send_tracking_data($uid, $aid, $coordinates, $note) {
  $json = new stdClass();
  $account = user_load($uid);

  if (!empty($account) && $account->status == HRM_FLAG_YES) {

    $asset = hrm_basic_asset_get_data(array(array('field' => 'aid', 'value' => $aid)), TRUE);

    if (!empty($asset)) {
      $tid = db_insert('hrm_asset_tracking')
        ->fields(array(
          'aid' => $aid,
          'uid' => $uid,
          'coordinates' => $coordinates,
          'note' => $note,
          'created' => time(),
        ))
        ->execute();

      $ressponse = FALSE;
      $image_path = strtr(HRM_DIR_ASSET, array('%oid' => $asset['oid'], '%aid' => $aid)) . '/tracking';
      file_prepare_directory($image_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

      if (count($_FILES) == 0) {
        $ressponse = TRUE;
      }
      else {
        foreach ($_FILES as $_file) {
          $filename_array = explode('.', $_file['name']);
          $original_filename = $filename_array[0];
          $original_extension = $filename_array[1];

//        watchdog('ASSET services - Upload image', 'Results: <pre>@results</pre>', array('@results' => print_r($filename_array, TRUE)), WATCHDOG_INFO);

          $new_filename = $original_filename . '_tracker' . $tid . '.' . $original_extension;

          $file = new stdClass();
          $file->uid = $uid;
          $file->status = 0;
          $file->filename = $new_filename;
          $file->uri = $_file['tmp_name'];
          $file->filemime = file_get_mimetype($file->filename);
          $file->filesize = $_file['size'];

          $photo = file_move($file, $image_path . '/' . $new_filename, FILE_EXISTS_RENAME);
          if (is_object($photo) && $photo->fid && $file->filesize) {
            $photo->status = 1;

            file_save($photo);
            file_usage_add($photo, HRM_MODULE_BACKEND, 'asset_tracker', $tid);

            db_update('hrm_asset_tracking')
              ->fields(array(
                'picture_fid' => $photo->fid,
              ))
              ->condition('id', $tid, '=')
              ->execute();

            $ressponse = TRUE;
          }
        }
      }

      $json->status = ($ressponse ? 200 : 404);
      $json->message = ($ressponse ? 'asset info send successfully***' : 'error send asset tracker');
      return $json;
    }
    else {
      $json->status = 404;
      $json->message_title = 'Asset Tracking';
      $json->message = 'Asset Not Found***';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Asset Tracking';
    $json->message = 'This acount is not found***';
    return $json;
  }
}
