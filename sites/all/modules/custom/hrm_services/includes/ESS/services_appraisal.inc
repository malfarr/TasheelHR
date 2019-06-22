<?php

function hrm_service_appraisals_get_appraisal($empid) {

  $emp_filter = array(array('field' => 'empid', 'value' => $empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);


  $json = new stdClass;
  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    $branch_filters = array(array('field' => 'oid', 'value' => $employee['oid']), array('field' => 'bid', 'value' => $employee['bid']));
    $emp_filters = array(array('field' => 'oid', 'value' => $employee['oid']), array('field' => 'bid', 'value' => $employee['bid']), array('field' => 'status', 'value' => HRM_STATUS_ACTIVE));
    $appraisals = hrm_basic_perf_appraisal_get_data($branch_filters);




    $branch_employees = hrm_services_employee_get_data($emp_filters);
    $competencies = hrm_basic_perf_competency_get_data($branch_filters);
    $subordinates = array();


    $data = array();



    foreach ($appraisals as $appraisal) {
      if ($appraisal['status'] == HRM_STATUS_ACTIVE) {


        $employees = array();
        $emp_reviewed = array();

        foreach ($appraisal['review'] as $review) {
          if ($review['eval_empid'] == $empid) {
            $emp_reviewed[] = $review['empid'];
          }
        }


        if (!empty($appraisal['evaluator'])) {
          foreach ($appraisal['evaluator'] as $eval_id => $evaluator) {

            if ($evaluator['eval_empid'] == $empid) {
              $evaluated_employee = $branch_employees[$evaluator['empid']];

              
              if (in_array($evaluated_employee['empid'], $emp_reviewed)) {
                continue;
              }

              $emp_data = array(
                'empid' => $evaluated_employee['empid'],
                'name' => $evaluated_employee['name'],
                'photo' => $evaluated_employee['photo_url'],
                'title' => $evaluated_employee['job_title_name'],
              );

              $employees[] = $emp_data;
            }
          }
        }

        if (empty($employees)) {
          continue;
        }


        $competency_group = array();
        $competency = $competencies[$appraisal['cid']];
        foreach ($competency['group'] as $group) {
          $group_data = array(
            'gid' => $group['gid'],
            'title' => $group['title'],
          );
          $question_data = array();
          foreach ($group['questions'] as $question) {
            $question_data[] = array(
              'id' => $question['qid'],
              'text' => $question['text'],
            );
          }
          $group_data['questions'] = $question_data;
          $competency_group[] = $group_data;
        }
        if (empty($competency_group)) {
          continue;
        }


        $data[] = array(
          'aid' => $appraisal['aid'],
          'name' => $appraisal['name'],
          'employees' => $employees,
          'reviews' => $emp_reviewed,
          'competency' => $competency_group,
          'status' => $appraisal['status'],
        );
      }
    }


    $json->appraisals = $data;
    $json->status = 200;

    return $json;
  }
  else {
    $json->status = 404;
    $json->message_title = 'Get appraisals';
    $json->message = 'This account is not found or blocked';
    return $json;
  }
}

function hrm_service_appraisals_send_review($eval_empid, $review_data) {
  $emp_filter = array(array('field' => 'empid', 'value' => $eval_empid),
    array('field' => 'status', 'value' => HRM_STATUS_ACTIVE, 'operator' => '='));
  $employee = hrm_services_employee_get_data($emp_filter, TRUE);

  $json = new stdClass;
  if (!empty($employee) && $employee['status'] == HRM_STATUS_ACTIVE) {
    $emp_location = hrm_basic_location_get_data(array(array('field' => 'lid', 'value' => $employee['location'])), TRUE, TRUE);
    if (!empty($emp_location)) {
      $timezone_offset = hrm_basic_timezone_get_offset($emp_location['timezone'], 'UTC');
      $utc_now = time();
      $timestamp = $utc_now + $timezone_offset;

      $review_data_object = json_decode($review_data);
      $review_data_array = object_to_array($review_data_object);
      $aid = $review_data_array['aid'];
      $empid = $review_data_array['empid'];
      $review_answers = $review_data_array['review_answers'];

      $review_filter = array(array('field' => 'eval_empid', 'value' => $eval_empid),
        array('field' => 'empid', 'value' => $empid, 'operator' => '='),
        array('field' => 'aid', 'value' => $aid, 'operator' => '='));

      $review = hrm_basic_perf_appraisal_get_review($review_filter, TRUE);

      $appraisal_review_fields = array(
        'aid' => $aid,
        'eval_empid' => $eval_empid,
        'empid' => $empid,
        'rate' => 0,
      );


      if (empty($review)) {
        $rid = db_insert('hrm_perf_appraisal_review')
          ->fields($appraisal_review_fields)
          ->execute();

        $rate = 0;
        foreach ($review_answers as $review_data) {
          db_insert('hrm_perf_appraisal_review_details')
            ->fields(array(
              'rid' => $rid,
              'qid' => $review_data['qid'],
              'answer' => $review_data['answer'],
              'rate' => $review_data['rate'],
            ))
            ->execute();

          $rate += $review_data['rate'];
        }

        $rate_avg = $rate / count($review_answers);
        db_update('hrm_perf_appraisal_review')
          ->fields(array(
            'rate' => $rate_avg,
            'created' => $timestamp,
          ))
          ->condition('rid', $rid, '=')
          ->execute();

        if ($rid) {
          $json->status = 200;
          $json->message = 'Appraisal review has supmitted successfully***';
        }
        else {
          $json->status = 404;
          $json->message = 'An error occurred during the review , please try again***';
        }
      }
      else {
        $rid = $review['rid'];

        $rate = 0;
        foreach ($review_answers as $review_data) {
          db_update('hrm_perf_appraisal_review_details')
            ->fields(array(
              'answer' => $review_data['answer'],
              'rate' => $review_data['rate'],
            ))
            ->condition('rid', $rid, '=')
            ->condition('qid', $review_data['qid'], '=')
            ->execute();

          $rate += $review_data['rate'];
        }

        $rate_avg = $rate / count($review_answers);
        db_update('hrm_perf_appraisal_review')
          ->fields(array(
            'rate' => $rate_avg,
          ))
          ->condition('rid', $rid, '=')
          ->execute();

        if ($rid) {
          $json->status = 200;
          $json->message = 'Appraisal review has updated successfully***';
        }
        else {
          $json->status = 404;
          $json->message = 'An error occurred during the review , please try again***';
        }
      }

      return $json;
    }
    else {
      $json->status = 404;
      $json->message_title = 'Appraisal review';
      $json->message = 'Your job has not been defined***';
      return $json;
    }
  }
  else {
    $json->status = 404;
    $json->message_title = 'Appraisal review';
    $json->message = 'This account is not found or blocked';
    return $json;
  }
}
