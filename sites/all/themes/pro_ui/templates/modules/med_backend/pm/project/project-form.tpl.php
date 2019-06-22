<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
      $('.project-form .form-actions .form-submit').click(function () {
        for (var i in CKEDITOR.instances) {
          CKEDITOR.instances[i].updateElement();
          $.trim($('#' + i).val());
        }
        if ($('.project-form').valid()) {
          $('.project-form').submit();
        } else {
          return false;
        }
      });
      
      $('.project-form').validate({
        errorClass: 'help-block animation-slideDown',
        errorElement: 'div',
        errorPlacement: function (error, e) {
          if (error.text() === 'Please select start date') {
          }
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
          },
          "files[pdf_logo]": {
            required: true,
          },
          end_date: {
            dateGreaterThan: ["#project-start-date", "Start date"],
          },          
        },
        messages: {
          end_date: {
            dateGreaterThan: 'End date must be bigger than start date',
          },
        }
      });

      $("input.input-group").removeClass('input-group').parent('.form-group').addClass('input-group');
      $(".input-group label").addClass('input-group-addon');
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php print drupal_render($form['name']); ?>
<?php print drupal_render($form['abbrev']); ?>
<?php print drupal_render($form['donor']); ?>
<?php print drupal_render($form['logo']); ?>
<?php print drupal_render($form['pdf_logo']); ?>
<?php print drupal_render($form['start_date']); ?>
<?php print drupal_render($form['end_date']); ?>
<?php print drupal_render($form['fiscal_year']); ?>
<?php print drupal_render($form['description']); ?>
<?php print drupal_render($form['indicators']); ?>
<div class="form-group">
  <?php print drupal_render($form['color_code_display']); ?>
  <?php print drupal_render($form['color_code']); ?>
</div>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> </div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>