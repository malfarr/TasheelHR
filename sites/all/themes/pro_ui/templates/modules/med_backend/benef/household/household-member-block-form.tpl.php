<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {        
        $('.household-member-block-form').validate({
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
        $('.household-member-block-form .form-submit').click(function () {
          for (var i in CKEDITOR.instances) {
              CKEDITOR.instances[i].updateElement();
              $.trim($('#' + i).val());
          }
          if ($(".household-member-block-form").valid()) {
            $(".household-member-block-form").submit();
          } else {
            return false;
          }
        });
      }
    };
    $(document).ready(function () {
      for (var instanceName in CKEDITOR.instances)
        CKEDITOR.remove(CKEDITOR.instances[instanceName]);
      
      CKEDITOR.replaceAll('ckeditor');

      for (var i in CKEDITOR.instances) {
        CKEDITOR.instances[i].on('change', function () {
          CKEDITOR.instances[i].updateElement();
          $.trim($('#' + i).val());
        });
      }
    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php print drupal_render($form['name']); ?>
<?php print drupal_render($form['comments']); ?>

<div class="form-group form-actions">
  <?php print drupal_render($form['submit']); ?> 
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>