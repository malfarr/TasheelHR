<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {attach: function (context, settings) {}};
    $(document).ready(function () {});
  })(jQuery, Drupal, this, this.document);
</script>

<?php if (isset($form['e_name'])) { print drupal_render($form['e_name']); } ?>
<?php if (isset($form['a_name'])) { print drupal_render($form['a_name']); } ?>
<?php if (isset($form['id_num'])) { print drupal_render($form['id_num']); } ?>
<?php if (isset($form['pay_method'])) { print drupal_render($form['pay_method']); } ?>
<?php if (isset($form['pay_account'])) { print drupal_render($form['pay_account']); } ?>
<div class="form-item form-group">
  <?php if (isset($form['reflect_changes'])) { print drupal_render($form['reflect_changes']); } ?>
</div>

<div class="form-group form-actions">
<?php print drupal_render($form['submit']); ?> 
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>