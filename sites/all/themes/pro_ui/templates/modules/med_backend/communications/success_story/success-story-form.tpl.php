<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {

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

      $('.story-partner').change(function () {
        if ($(this).is(":checked")) {
          is_leader = $(this).parents(".story-owner").find(".story-leader");
          if ($(is_leader).is(":checked")) {
            $(is_leader).trigger('click');
          }

          is_other = $(this).parents(".story-owner").find(".story-other");
          if ($(is_other).is(":checked")) {
            $(is_other).trigger('click');
          }
        }
      });

      $('.story-leader').change(function () {
        if ($(this).is(":checked")) {
          is_partner = $(this).parents(".story-owner").find(".story-partner");
          if ($(is_partner).is(":checked")) {
            $(is_partner).trigger('click');
          }

          is_other = $(this).parents(".story-owner").find(".story-other");
          if ($(is_other).is(":checked")) {
            $(is_other).trigger('click');
          }
        }
      });

      $('.story-other').change(function () {
        if ($(this).is(":checked")) {
          is_partner = $(this).parents(".story-owner").find(".story-partner");
          if ($(is_partner).is(":checked")) {
            $(is_partner).trigger('click');
          }

          is_leader = $(this).parents(".story-owner").find(".story-leader");
          if ($(is_leader).is(":checked")) {
            $(is_leader).trigger('click');
          }
        }
      });

    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php if (isset($form['title'])) {
  print drupal_render($form['title']);
} ?>
<?php if (isset($form['office'])) {
  print drupal_render($form['office']);
} ?>
<?php if (isset($form['project'])) {
  print drupal_render($form['project']);
} ?>

<?php if (isset($form['program'])) {
  print drupal_render($form['program']);
} ?>

<?php if (isset($form['body'])) {
  print drupal_render($form['body']);
} ?>
  <?php if (isset($form['recommendation'])) {
    print drupal_render($form['recommendation']);
  } ?>


<?php if (isset($form['testimonies'])) {
  print drupal_render($form['testimonies']);
} ?>

<?php if (isset($form['references'])) {
  print drupal_render($form['references']);
} ?>

<?php if (isset($form['author'])) {
  print drupal_render($form['author']);
} ?>


<!--Back next button-->
<div class="form-group form-actions form-actions-btn-primary col-sm-12">
<?php if (isset($form['submit'])) {
  print drupal_render($form['submit']);
} ?>
<?php if (isset($form['save_draft'])) {
  print drupal_render($form['save_draft']);
} ?>
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>