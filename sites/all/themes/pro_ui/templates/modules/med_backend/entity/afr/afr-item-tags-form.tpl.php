<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {                
        $('.afr-item-tags-form').validate({
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
          },
          messages: {
          }
        });
        $('.afr-item-tags-form .form-submit').click(function () {
          if ($(".afr-item-tags-form").valid()) {
            $(".afr-item-tags-form").submit();
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

<?php print drupal_render($form['item_output']); ?>
<br />
<?php if(isset($form['tags'])){ print drupal_render($form['tags']); } ?>
<?php if(isset($form['submit'])){ ?>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php } ?>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>