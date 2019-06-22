<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.project-code-form .form-submit').removeClass('btn-success');
        $('.project-code-form').validate({
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
            code: {
              required: true,
              remote: {
                url: Drupal.settings.basePath + "med-ajax/ajax/jquery-validate",
                type: "post",
                async: false,
                data: {
                  type: function () {
                    return "project-code-check";
                  },
                  code: function () {
                    return $("#edit-code").val();
                  },
                  pid: function () {
                    return $("#pid").val();
                  },
                }
              }
            },
          },
          messages: {
            code: {
              required: 'Please enter project code',
              digits: 'Digits only',
              remote: 'Project code must be unique',
            },
          }
        });
        $('.project-code-form .form-submit').click(function () {
          if ($(".project-code-form").valid()) {
            $(".project-code-form").submit();
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

<?php print drupal_render($form['code']); ?>
<?php print drupal_render($form['pid']); ?>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>