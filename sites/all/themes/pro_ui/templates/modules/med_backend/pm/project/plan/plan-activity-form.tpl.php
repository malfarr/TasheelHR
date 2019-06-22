<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.plan-activity-form .form-submit').click(function () {
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
<div class="padding-0-20"><?php print drupal_render($form['item_text']); ?></div>
<?php print drupal_render($form['title']); ?>
<?php print drupal_render($form['description']); ?>

<div class="form-item form-group plan-activity-form-group">
  <?php print drupal_render($form['type']); ?>
  <?php print drupal_render($form['type_other']); ?>
</div>

<div class="form-item form-group plan-activity-form-group">
  <?php print drupal_render($form['location']); ?>
  <?php print drupal_render($form['location_other']); ?>
</div>

<div class="form-item form-group plan-activity-form-group">
  <?php print drupal_render($form['program']); ?>
  <?php print drupal_render($form['program_other']); ?>
</div>

<div class="form-item form-group plan-activity-form-group">
  <?php print drupal_render($form['thematic']); ?>
  <?php print drupal_render($form['thematic_other']); ?>
</div>
<?php print drupal_render($form['benef']); ?>
<div class="form-item form-group plan-activity-form-group">
  <?php print drupal_render($form['cost']); ?>
  <?php print drupal_render($form['cost_other']); ?>
</div>


<!--Back next button-->
<div class="form-group form-actions">
  <?php if (isset($form['submit'])) {
    print drupal_render($form['submit']);
  } ?>
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>