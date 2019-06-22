<?php
global $base_url;
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.pmf-indicator-outcome-value-form').validate({
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
          },
        });
        $('.pmf-indicator-outcome-value-form .form-submit').click(function () {
          for (var i in CKEDITOR.instances) {
            CKEDITOR.instances[i].updateElement();
            $.trim($('#' + i).val());
          }
          if ($(".pmf-indicator-outcome-value-form").valid()) {
            $(".pmf-indicator-outcome-value-form").submit();
          } else {
            return false;
          }
        });
      }
    };
    $(document).ready(function () {
      try {
        CKEDITOR.replace('outcome_value');
      } catch (e) {
      }

    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php if (isset($form['outcome_value'])) {
    print drupal_render($form['outcome_value']);
  } ?>

<!--Back next button-->
<div class="form-group form-actions form-actions-btn-primary">
<?php if (isset($form['submit'])) {
  print drupal_render($form['submit']);
} ?>
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>