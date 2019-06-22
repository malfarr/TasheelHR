<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {       
        $('.entity-video-add-form').validate({
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
                "files[video]": {
                    required: true,
                    extension: "mp4",
                    filesize: 50
                },
            },
            messages: {
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
<?php print drupal_render($form['video']); ?>
<?php print drupal_render($form['thumbnail']); ?>
<?php print drupal_render($form['description']); ?>
<div class="form-group form-item">
<?php print drupal_render($form['comm_mtrl']); ?>
</div>
<div class="form-group form-actions form-actions-btn-primary">
  <?php   print drupal_render($form['submit']); ?> </div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>