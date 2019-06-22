<div id="page-edit-entity-pictures-wrapper" class="page-wrapper page-entity-pictures-wrapper">
  <div id="edit-entity-pictures-block" class="block">
    <div class="block-title">
      <h2><?php echo $variables['title']; ?></h2>
    </div>
    <div class="block-content">
      <?php
      $edit_entity_pictures_form = drupal_get_form('med_backend_entity_picture_edit_entity_pictures_form', $variables['entity'], $variables['entity_id'], $variables['pictures'], $variables['account'], $variables['delete'], $variables['delete_app_pictures']);
      print drupal_render($edit_entity_pictures_form);
      ?>   
    </div>
  </div>
</div>