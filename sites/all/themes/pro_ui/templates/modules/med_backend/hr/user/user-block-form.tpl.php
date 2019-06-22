<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {        
        $('.user-block-form').validate({
          errorClass: 'help-block animation-slideDown',
          errorElement: 'div',
          onkeyup: false,
          errorPlacement: function (error, e) {
            e.parents('.form-group').append(error);
          },
          highlight: function (e) {
            $(e).blockst('.form-group').removeClass('has-success has-error').addClass('has-error');
            $(e).blockst('.help-block').remove();
          },
          success: function (e) {
            e.blockst('.form-group').removeClass('has-success has-error').addClass('has-success');
            e.blockst('.help-block').remove();
          },
          ignore: "",
          rules: {            
          },         
        });
        $('.user-block-form .form-submit').click(function () {
          for (var i in CKEDITOR.instances) {
              CKEDITOR.instances[i].updateElement();
              $.trim($('#' + i).val());
          }
          if ($('.user-block-form').valid()) {
            $('.user-block-form').submit();
          } else {
            return false;
          }
        });
      }
    };
    $(document).ready(function () {
      try { 
        CKEDITOR.replace('comments');         
      } catch(e){}
    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php if(isset($form['comments'])){ print drupal_render($form['comments']); } ?>

<div class="form-group form-actions">
  <?php print drupal_render($form['buttons']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>