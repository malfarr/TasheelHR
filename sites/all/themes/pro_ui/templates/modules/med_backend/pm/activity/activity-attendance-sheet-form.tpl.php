<?php
global $base_url;
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.form-wrapper').removeClass('form-group');
        $('.activity-attendance-sheet-form').validate({
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
            "files[attendance_sheet]": {
              required: true,
            },
          },
          messages: {
          }
        });
        $('.activity-attendance-sheet-form .form-submit').click(function () {
          if ($('.activity-attendance-sheet-form').valid()) {
            $('.activity-attendance-sheet-form').submit();
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

<?php if (isset($form['attendance_sheet'])) { print drupal_render($form['attendance_sheet']); } ?>

<!--Back next button-->
<div class="form-group form-actions form-actions-btn-primary">
<?php if (isset($form['submit'])) { print drupal_render($form['submit']); } ?>
</div>

<?php
  print drupal_render($form['form_build_id']);
  print drupal_render($form['form_token']);
  print drupal_render($form['form_id']);
?>