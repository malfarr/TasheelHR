<!--</pre>-->
<?php
global $base_url;
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        
        $('.user-emp-of-quarter-form').validate({
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
            employee: {
              required: true,
            },
            "files[image]": {
              required: true,
            },
            text: {
              required: true,
            },
          },
          messages: {
          }
        });

        $('.user-emp-of-quarter-form .form-actions .form-submit').click(function(){
          if($(".user-emp-of-quarter-form").valid()) {
            $(".user-emp-of-quarter-form").submit();            
          }else{
            return false;
          }
      });
      }
    };
    $(document).ready(function(){});
  })(jQuery, Drupal, this, this.document);
</script>
<?php print drupal_render($form['employee']); ?>
<?php print drupal_render($form['image']); ?>
<?php print drupal_render($form['text']); ?>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>