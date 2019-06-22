<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.service-mapping-form .form-submit').click(function () {
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

<?php if (isset($form['office'])) { print drupal_render($form['office']); } ?>
<?php if (isset($form['district'])) { print drupal_render($form['district']); } ?>
<?php if (isset($form['e_name'])) { print drupal_render($form['e_name']); } ?>
<?php if (isset($form['a_name'])) { print drupal_render($form['a_name']); } ?>
<?php if (isset($form['type'])) { print drupal_render($form['type']); } ?>
<?php if (isset($form['sub_type'])) { print drupal_render($form['sub_type']); } ?>
<?php if (isset($form['nationality'])) { print drupal_render($form['nationality']); } ?>
<?php if (isset($form['nationalities'])) { print drupal_render($form['nationalities']); } ?>
<?php if (isset($form['description'])) { print drupal_render($form['description']); } ?>
<?php if (isset($form['service'])) { print drupal_render($form['service']); } ?>
<?php if (isset($form['contact'])) { print drupal_render($form['contact']); } ?>
<?php if (isset($form['address'])) { print drupal_render($form['address']); } ?>
<?php if (isset($form['comments'])) { print drupal_render($form['comments']); } ?>
<?php if (isset($form['notes'])) { print drupal_render($form['notes']); } ?>

<div class="form-group form-actions">
  <?php print drupal_render($form['submit']); ?> 
</div>

<?php
  print drupal_render($form['form_build_id']);
  print drupal_render($form['form_token']);
  print drupal_render($form['form_id']);
?>