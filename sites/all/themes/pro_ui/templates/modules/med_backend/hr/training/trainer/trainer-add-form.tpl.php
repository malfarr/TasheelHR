  <script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.add-trainer-form .form-submit').removeClass('btn-success');
        
        $('.add-trainer-form').validate({
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
            email: {
              email: true,
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

<?php print drupal_render($form['name']); ?>
<?php print drupal_render($form['photo']); ?>
<?php print drupal_render($form['gender']); ?>
<?php print drupal_render($form['title']); ?>
<?php print drupal_render($form['organization']); ?>
<?php print drupal_render($form['phone']); ?>
<?php print drupal_render($form['email']); ?>
<?php print drupal_render($form['bio']); ?>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>