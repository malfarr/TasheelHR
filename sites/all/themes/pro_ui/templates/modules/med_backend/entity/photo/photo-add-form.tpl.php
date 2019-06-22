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
<?php if (isset($form['ref_1'])) { ?>
  <?php print drupal_render($form['ref_1']); ?>
<?php } ?>
<?php if (isset($form['type'])) { ?>
  <?php print drupal_render($form['type']); ?>
<?php } ?>
<?php print drupal_render($form['photos']); ?>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>