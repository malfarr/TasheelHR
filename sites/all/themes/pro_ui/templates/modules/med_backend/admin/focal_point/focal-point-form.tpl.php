<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.focal-point-form').validate({
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
            telephone: {
              oneAtLeastRequired: ['.focal-point-telephone', '.focal-point-mobile', '.focal-point-email']
            },
            mobile: {
              oneAtLeastRequired: ['.focal-point-telephone', '.focal-point-mobile', '.focal-point-email']
            },
            email: {
              email: true,
              oneAtLeastRequired: ['.focal-point-telephone', '.focal-point-mobile', '.focal-point-email']
            },
          },
          messages: {
            telephone: {
              oneAtLeastRequired: 'Telehone, Mobile, or email is requred',
            },
            mobile: {
              oneAtLeastRequired: 'Telehone, Mobile, or email is requred',
            },
            email: {
              email: 'Invlaid email address',
              oneAtLeastRequired: 'Telehone, Mobile, or email is requred',
            },
          }
        });
        $('.focal-point-form .form-submit').click(function () {
          if ($(".focal-point-form").valid()) {
            $(".focal-point-form").submit();
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

<?php print drupal_render($form['e_name']); ?>
<?php print drupal_render($form['a_name']); ?>
<?php print drupal_render($form['photo']); ?>
<?php print drupal_render($form['title']); ?>
<?php print drupal_render($form['national_id']); ?>
<?php print drupal_render($form['telephone']); ?>
<?php print drupal_render($form['mobile']); ?>
<?php print drupal_render($form['email']); ?>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>