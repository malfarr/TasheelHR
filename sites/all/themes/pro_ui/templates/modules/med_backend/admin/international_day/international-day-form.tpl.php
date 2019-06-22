<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.international-day-form').validate({
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
            "files[logo]": {
              required: true,
              extension: "<?php echo MED_FILE_EXT_PICTURE_JQUERY; ?>"
            },
          },
          messages: {
          }
        });
        $('.international-day-form .form-submit').click(function () {
          for (var i in CKEDITOR.instances) {
            CKEDITOR.instances[i].updateElement();
            $.trim($('#' + i).val());
          }
          if ($(".international-day-form").valid()) {
            $(".international-day-form").submit();
          } else {
            return false;
          }
        });
      }
    };
    $(document).ready(function () {
      try {
        CKEDITOR.replace('description');
      } catch (e) {
      }
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php print drupal_render($form['name']); ?>
<?php print drupal_render($form['date']); ?>
<?php print drupal_render($form['logo']); ?>
<?php print drupal_render($form['description']); ?>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>
