<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.account-code-form .form-submit').removeClass('btn-success');
        $('.account-code-form').validate({
          errorClass: 'help-block animation-slideDown',
          errorElement: 'div',
          onkeyup: true,
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
            code: {
              required: true,
              digits: true,
              remote: {
                url: Drupal.settings.basePath + "med-ajax/ajax/jquery-validate",
                type: "post",
                async: false,
                data: {
                  type: function () {
                    return "account-code-check";
                  },
                  code: function () {
                    return $("#edit-code").val();
                  },
                  id: function () {
                    return $("#account-id").val();
                  },
                }
              }
            },
            name: {
              required: true,
            }
          },
          messages: {
            code: {
              remote: 'Account code must be unique',
            },
          }
        });
        $('.account-code-form .form-submit').click(function () {
          if ($(".account-code-form").valid()) {
            $(".account-code-form").submit();
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
<?php print drupal_render($form['id']); ?>
<?php print drupal_render($form['name']); ?>
<?php print drupal_render($form['code']); ?>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>