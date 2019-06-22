  <script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {         
      }
    };
    $(document).ready(function () {

    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php print drupal_render($form['e_text']); ?>
<?php print drupal_render($form['a_text']); ?>
<?php print drupal_render($form['abbrev']); ?>
<?php print drupal_render($form['type']); ?>
<?php print drupal_render($form['options']); ?>
<?php print drupal_render($form['data_limit']); ?>
<div class="form-item form-group">
  <?php print drupal_render($form['other']); ?>
</div>
<div class="form-item form-group">
  <?php print drupal_render($form['required']); ?>
</div>
<div class="form-item form-group">
  <?php print drupal_render($form['summary']); ?>
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