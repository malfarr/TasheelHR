<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
      $('.office-form').validate({
        errorClass: 'help-block animation-slideDown',
        errorElement: 'div',
        errorPlacement: function (error, e) {
          if (error.text() === 'Please provide a coordinates') {
            $('#set-location-wrapper .help-block').remove();
            $('#set-location-wrapper').append(error);
            $('#set-location-wrapper').removeClass('has-success has-error hide-errors').addClass('has-error');
          } else if (error.text() === 'Please select the color code of this office') {
            $('#color-field-wrapper .help-block').remove();
            $('#color-field-wrapper').append(error);
            $('#color-field-wrapper').removeClass('has-success has-error hide-errors').addClass('has-error');
          } else {
            e.parents('.form-group').append(error);
          }

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
          abbrev: {
            maxlength: 3
          },
        },
        messages: {
        }
      });
      $('.office-form .form-submit').click(function () {
        for (var i in CKEDITOR.instances) {
          CKEDITOR.instances[i].updateElement();
          $.trim($('#' + i).val());
        }
      });
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php print drupal_render($form['name']); ?>
<?php print drupal_render($form['a_name']); ?>
<?php print drupal_render($form['abbrev']); ?>
<?php print drupal_render($form['city']); ?>
<?php print drupal_render($form['address']); ?>
<div class="form-group">
  <?php print drupal_render($form['location']); ?>
  <?php print drupal_render($form['coordinates']); ?>
</div>
<?php print drupal_render($form['marker']); ?>
<?php print drupal_render($form['oic']); ?>
<?php print drupal_render($form['weight']); ?>
<div class="form-group">
  <?php print drupal_render($form['color_code_display']); ?>
  <?php print drupal_render($form['color_code']); ?>
</div>
<div class="form-group form-actions">
  <?php print drupal_render($form['submit']); ?> 
</div>

<?php
  print drupal_render($form['form_build_id']);
  print drupal_render($form['form_token']);
  print drupal_render($form['form_id']);
?>