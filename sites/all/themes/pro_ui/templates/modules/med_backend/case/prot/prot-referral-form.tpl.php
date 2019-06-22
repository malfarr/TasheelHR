<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.prot-referral-form .form-submit').click(function () {
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

<?php if (isset($form['agency'])) { print drupal_render($form['agency']); } ?>
<?php if (isset($form['description'])) { print drupal_render($form['description']); } ?>

<div class="form-group form-actions btn-group-sm">
<?php print drupal_render($form['submit']); ?> 
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>