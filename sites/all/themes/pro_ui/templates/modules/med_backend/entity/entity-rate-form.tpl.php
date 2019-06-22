<?php
global $base_url;
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.entity-rate-form').validate({
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
        $('.entity-rate-form .form-submit').click(function () {
          for (var i in CKEDITOR.instances) {
            CKEDITOR.instances[i].updateElement();
            $.trim($('#' + i).val());
          }
          if ($(".entity-rate-form").valid()) {
            $(".entity-conclusion-form").submit();
          } else {
            return false;
          }
        });
      }
    };
    $(document).ready(function () {
      try {
        CKEDITOR.replace('feedback');
      } catch (e) {
      }

    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php if (isset($form['score'])) {
    print drupal_render($form['score']);
  } ?>
<?php if (isset($form['feedback'])) {
    print drupal_render($form['feedback']);
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