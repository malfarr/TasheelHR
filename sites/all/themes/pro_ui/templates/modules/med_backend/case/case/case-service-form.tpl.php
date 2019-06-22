<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.case-service-form .form-submit').click(function () {
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
<?php if (isset($form['family'])) { print drupal_render($form['family']); } ?>
<?php if (isset($form['member'])) { print drupal_render($form['member']); } ?>
<?php if (isset($form['type'])) { print drupal_render($form['type']); } ?>
<?php if (isset($form['type_other'])) { print drupal_render($form['type_other']); } ?>
<?php if (isset($form['urgent'])) { print drupal_render($form['urgent']); } ?>
<?php if (isset($form['description'])) { print drupal_render($form['description']); } ?>
<?php if (isset($form['covered'])) { print drupal_render($form['covered']); } ?>
<?php if (isset($form['action'])) { print drupal_render($form['action']); } ?>
<?php if (isset($form['deadline'])) { print drupal_render($form['deadline']); } ?>
<?php if (isset($form['accomp'])) { print drupal_render($form['accomp']); } ?>
<?php if (isset($form['referral'])) { print drupal_render($form['referral']); } ?>
<?php if (isset($form['referral_agency'])) { print drupal_render($form['referral_agency']); } ?>
<?php if (isset($form['documents'])) { print drupal_render($form['documents']); } ?>

<div class="form-group form-actions">
<?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>