<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {       
        $('.entity-document-edit-form').validate({
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
                "files[document]": {
                    required: true
                },
            },
            messages: {
            }
        });
        $('.entity-document-edit-form .form-submit').click(function () {
          if ($(".entity-document-edit-form").valid()) {
            $(".entity-document-edit-form").submit();
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
<div class="form-group form-item text-center">
  <?php print drupal_render($form['video_output']); ?>
</div>

<?php print drupal_render($form['title']); ?>
<?php print drupal_render($form['document']); ?>
<?php print drupal_render($form['description']); ?>
<div class="form-group form-actions form-actions-btn-primary">
  <?php   print drupal_render($form['submit']); ?> </div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>