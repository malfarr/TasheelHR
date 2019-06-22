<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php print drupal_render($form['labels']); ?>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> </div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>