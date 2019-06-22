<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.pmf-indicator-form').validate({
          errorClass: 'help-block animation-slideDown',
          errorElement: 'div',
          onkeyup: false,
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
          messages: {
          }
        });
        $('.pmf-indicator-form .form-submit').click(function () {
          if ($(".pmf-indicator-form").valid()) {
            $(".pmf-indicator-form").submit();
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
<?php print drupal_render($form['info']); ?>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>
 