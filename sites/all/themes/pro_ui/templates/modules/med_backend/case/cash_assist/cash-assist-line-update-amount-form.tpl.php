<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {attach: function (context, settings) {}};
    $(document).ready(function () {});
  })(jQuery, Drupal, this, this.document);
</script>

<?php
if (isset($form['amount'])) {
  print drupal_render($form['amount']);
}
?>

<div class="form-group form-actions">
<?php print drupal_render($form['submit']); ?> 
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>