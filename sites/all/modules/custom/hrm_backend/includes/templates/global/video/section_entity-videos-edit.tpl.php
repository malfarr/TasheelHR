<div id="page-edit-video-wrapper" class="page-edit-video-wrapper page-wrapper page-video-wrapper">
  <div class="block">
    <div class="block-title">
      <h2><strong>Edit </strong> video</h2>
    </div>
    <div class="block-content">
      <?php
      $edit_video_form = drupal_get_form('med_backend_entity_video_edit_video_form', $variables['video']);
      print drupal_render($edit_video_form);
      ?>   
    </div>
  </div>
</div>