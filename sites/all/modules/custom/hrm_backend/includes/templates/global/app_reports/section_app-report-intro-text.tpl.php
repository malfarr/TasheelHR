<?php
$type = $variables['type'];
$entity = (isset($variables['entity']) ? $variables['entity'] : '');
?>
Hello @[reporter],
<?php if ($type == MED_APP_REPORT_TYPE_FINAL) { ?>  
  <?php if ($variables['entity'] == MED_ENTITY_ACTIVITY) { ?>
    You are reporting on the @[activity_type] activity entitled "@[title]", taking place at @[venue] on @[start_date] at @[start_time]. Please enter accurate information only.  
  <?php } ?>

<?php }
elseif ($type == MED_APP_REPORT_TYPE_PARTICIPANTS) { ?>
  You are reporting on the participants of the @[entity_name] entitled "@[title]", taking place at @[venue] on @[start_date]. Please enter accurate information only.

<?php }
elseif ($type == MED_APP_REPORT_TYPE_E_ATTENDANCE) { ?>
  You are reporting on the attendance day @[day_index] (@[day_date]) of the workshop entitled "@[title]", taking place at @[venue]. Please enter accurate information only.

<?php }
elseif ($type == MED_APP_REPORT_TYPE_TRACKER_REPORT) { ?>  
  You are reporting on the @[entity] tracker entitled "@[title]", taking place at @[venue]. Please enter accurate information only.  
<?php } 
elseif ($type == MED_APP_REPORT_TYPE_HOUSEHOLD_ASSESSMENT_REPORT) { ?>  
  <?php if(!isset($variables['lang']) || $variables['lang'] == MED_LANGUAGE_EN){ ?>
My name is @[case_manager_name]. I am working with Medair Swiss Organization which provides humanitarian services to the host community.
In this project, we focus on refugees and Jordanians living in Amman, 
I would like to ask you some questions about your family with the aim of having a better understanding of your living conditions. The survey usually takes about 45 minutes to complete. Please note that any information that you provide will be kept strictly confidential and will not be shared with any third party without your approval. This is voluntary and you can choose not to answer any or all of the questions. However we hope that you will participate since the information you will provide is essential to evaluate your individual situation. If you don’t have any questions, may I begin now? 
  <?php } 
  elseif ($variables['lang'] == MED_LANGUAGE_AR) { ?>  
أنا إسمي @[case_manager_name] بشتغل في منظمة ميدير السويسرية، بنقدم خدمات إنسانية للمجتمع المحلي. بهذا المشروع بنركز على اللاجئين والأردنيين سكان عمان، بحب أسألكم شوية أسئلة عن عائلتكم لنقدر نفهم أكتر ظروف المعيشة. عادة الأسئلة بتاخد 45 دقيقة. للتوضيح هذه المعلومات في غاية السرية ولن يتم مشاركتها مع أي طرف ثالث بدون موافقتكم. هذه الأسئلة مش إجبارية وبتقدر إنك ما تجاوب. على كل حال بنتمنى انك تشارك لانه هاي المعلومات هي رح تقيم وضعك المعيشي. إذا ما عندك أي أسئلة، نبلش؟
  <?php } ?>
<?php } 
elseif ($type == MED_APP_REPORT_TYPE_CASE_IN_DEPTH_ASSESSMENT_REPORT) { ?>  
  You are about to report the in-depth analysis assessment for case number @[sn] titled "@[title]" . Please enter accurate information only.  
<?php } 
elseif ($type == MED_APP_REPORT_TYPE_CASE_CONFIRMATION_ASSESSMENT_REPORT) { ?>  
  You are about to submit the case plan confirmation for case number @[sn] titled "@[title]" . Please enter accurate information only.  
<?php }  
elseif ($type == MED_APP_REPORT_TYPE_CASE_VISIT_REPORT) { ?>  
  You are about to submit the home visit report for case number @[sn] titled "@[title]" . Please enter accurate information only.  
<?php } 