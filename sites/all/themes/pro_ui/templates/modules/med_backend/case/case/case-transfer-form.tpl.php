<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.case-transfer-form .form-submit').click(function () {
          for (var i in CKEDITOR.instances) {
            CKEDITOR.instances[i].updateElement();
            $.trim($('#' + i).val());
          }
        });
      }
    };
    $(document).ready(function () {
      for (var instanceName in CKEDITOR.instances)
        CKEDITOR.remove(CKEDITOR.instances[instanceName]);
      CKEDITOR.replaceAll('ckeditor');

      for (var i in CKEDITOR.instances) {
        CKEDITOR.instances[i].on('change', function () {
          CKEDITOR.instances[i].updateElement();
          $.trim($('#' + i).val());
        });
      }
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php if (isset($form['current_caseworker'])) {
  print drupal_render($form['current_caseworker']);
} ?>
<?php if (isset($form['caseworker'])) {
  print drupal_render($form['caseworker']);
} ?>
<?php if (isset($form['justification'])) {
  print drupal_render($form['justification']);
} ?>
<?php if (isset($form['user_password'])) {
  print drupal_render($form['user_password']);
} ?>

<div class="form-group form-actions">
<?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>