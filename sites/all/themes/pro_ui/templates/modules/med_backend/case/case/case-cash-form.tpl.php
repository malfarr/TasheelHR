<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.case-cash-form .form-submit').click(function () {
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
<?php if (isset($form['members_count'])) { print drupal_render($form['members_count']); } ?>
<?php if (isset($form['plan'])) { print drupal_render($form['plan']); } ?>
<?php if (isset($form['start_month'])) { print drupal_render($form['start_month']); } ?>
<?php if (isset($form['start_year'])) { print drupal_render($form['start_year']); } ?>
<?php if (isset($form['amount_method'])) { print drupal_render($form['amount_method']); } ?>
<?php if (isset($form['amount_value'])) { print drupal_render($form['amount_value']); } ?>

<div class="form-group form-actions">
<?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>