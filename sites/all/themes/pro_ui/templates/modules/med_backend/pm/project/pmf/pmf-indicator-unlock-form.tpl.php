<div class="form-group form-item">
  <?php print drupal_render($form['confirm_message']); ?>
</div>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['buttons']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>