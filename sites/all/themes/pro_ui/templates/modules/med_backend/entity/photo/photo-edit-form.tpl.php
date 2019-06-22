<div class="form-group form-item text-center">
  <?php print drupal_render($form['photo']); ?>
</div>
<?php print drupal_render($form['caption']); ?>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['buttons']); ?>   
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>