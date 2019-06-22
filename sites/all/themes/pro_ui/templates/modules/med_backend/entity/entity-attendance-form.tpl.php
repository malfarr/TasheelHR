<?php
global $base_url;
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.form-wrapper').removeClass('form-group');
        $('.entity-attendance-form').validate({
          errorClass: 'help-block animation-slideDown',
          errorElement: 'div',
          errorPlacement: function (error, e) {
            e.parents('.form-group').append(error);
          },
          highlight: function (e) {
            $(e).closest('.form-group').removeClass('has-success has-error').addClass('has-error');
            $(e).closest('.help-block').remove();
          },
          success: function (e) {
            e.closest('.form-group').removeClass('has-success has-error').addClass('has-success');
            e.closest('.help-block').remove();
          },
          ignore: "",
          rules: {
            "files[attendance]": {
              required: true,
            },
          },
          messages: {
          }
        });
        $('.entity-attendance-form .form-submit').click(function () {
          if ($('.entity-attendance-form').valid()) {
            $('.entity-attendance-form').submit();
          } else {
            return false;
          }
        });
      }
    };
    $(document).ready(function () {

    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php if (isset($form['header'])) { print drupal_render($form['header']); } ?>
<?php if (isset($form['participants'])) { print drupal_render($form['participants']); } ?>
<?php if (isset($form['attendance'])) { print drupal_render($form['attendance']); } ?>

<!--Back next button-->
<div class="form-group form-actions form-actions-btn-primary">
<?php if (isset($form['submit'])) { print drupal_render($form['submit']); } ?>
</div>

<?php
  print drupal_render($form['form_build_id']);
  print drupal_render($form['form_token']);
  print drupal_render($form['form_id']);
?>