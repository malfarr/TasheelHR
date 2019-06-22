<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.donor-code-form .form-submit').removeClass('btn-success');
        $('.donor-code-form').validate({
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
                    return "donor-code-check";
                  },
                  code: function () {
                    return $("#edit-code").val();
                  },
                  did: function () {
                    return $("#did").val();
                  },
                }
              }
            },
          },
          messages: {
            code: {
              required: 'Please enter donor code',
              remote: 'Donor code must be unique',
            },
          }
        });
        $('.donor-code-form .form-submit').click(function () {
          if ($(".donor-code-form").valid()) {
            $(".donor-code-form").submit();
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