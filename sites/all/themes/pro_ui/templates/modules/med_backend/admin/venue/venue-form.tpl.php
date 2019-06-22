<?php print drupal_render($form['e_name']); ?>
<?php print drupal_render($form['a_name']); ?>
<?php print drupal_render($form['office']); ?>
<?php print drupal_render($form['address']); ?>
<?php print drupal_render($form['contacts']); ?>
<div class="form-group">
  <?php print drupal_render($form['location']); ?>
  <?php print drupal_render($form['coordinates']); ?>
</div>
<?php print drupal_render($form['type']); ?>
<?php print drupal_render($form['capacity']); ?>
<?php print drupal_render($form['cost']); ?>
<div class="form-group form-actions">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>

<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.venue-form .form-submit').removeClass('btn-success');
        $('.venue-form').validate({
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
  
            capacity: {
              number: true
            },
            cost: {
              number: true,
            },
          },
          messages: {            
            
          }
        });
        $('.venue-form .form-submit').click(function () {
          if ($('.venue-form').valid()) {
            $('.venue-form').submit();
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