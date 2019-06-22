<!--</pre>-->
<?php
global $base_url;
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.case-form').validate({
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
          messages: {
          }
        });
      }
    };
    $(document).ready(function () {

    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php if (isset($form['household'])) {
  print drupal_render($form['household']);
} ?>
  <?php if (isset($form['project'])) {
    print drupal_render($form['project']);
  } ?>
<?php if (isset($form['description'])) {
  print drupal_render($form['description']);
} ?>
<div class="form-group form-actions">
<?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>