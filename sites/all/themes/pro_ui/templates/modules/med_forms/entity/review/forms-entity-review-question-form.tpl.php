<?php print drupal_render($form['qkey']); ?>
<?php print drupal_render($form['e_text']); ?>
<?php print drupal_render($form['a_text']); ?>
<?php print drupal_render($form['e_abbrev']); ?>
<?php print drupal_render($form['a_abbrev']); ?>
<?php print drupal_render($form['e_prefix']); ?>
<?php print drupal_render($form['a_prefix']); ?>
<?php print drupal_render($form['e_header']); ?>
<?php print drupal_render($form['a_header']); ?>
<?php print drupal_render($form['entity']); ?>
<div class="form-item form-group">
  <?php print drupal_render($form['required']); ?>
</div>
<div class="form-item form-group">
  <?php print drupal_render($form['status']); ?>
</div>
<?php print drupal_render($form['weight']); ?>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>