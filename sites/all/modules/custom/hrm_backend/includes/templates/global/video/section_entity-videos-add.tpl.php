<div id="page-add-video-wrapper" class="page-add-video-wrapper page-wrapper page-video-wrapper">
  <div class="block">
    <div class="block-title">
      <h2><strong>Add </strong> video</h2>
    </div>
    <div class="block-content">
      <?php
      $add_video_form = drupal_get_form('med_backend_entity_video_add_video_form', $variables['entity'], $variables['entity_id']);
      print drupal_render($add_video_form);
      ?>   
    </div>
  </div>
</div>