<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('input.household-approved').change(function () {
          if ($(this).is(":checked")) {
            is_rejected = $(this).parents("tr").find("input.household-rejected");
            if ($(is_rejected).is(":checked")) {
              $(is_rejected).trigger('click');
            }

            $(this).parents("tr").addClass('success');
            $(this).parents("tr").removeClass('danger');
          }
          else{
            $(this).parents("tr").removeClass('success');
          }
        });

        $('input.household-rejected').change(function () {
          if ($(this).is(":checked")) {
            is_approved = $(this).parents("tr").find("input.household-approved");
            if ($(is_approved).is(":checked")) {
              $(is_approved).trigger('click');
            }
            $(this).parents("tr").removeClass('success');
            $(this).parents("tr").addClass('danger');
          }
          else{
            $(this).parents("tr").removeClass('danger');
          }
        });

        $('input.household-approved').each(function () {
          if ($(this).is(":checked")) {
            $(this).parents("tr").removeClass('danger');
            $(this).parents("tr").addClass('success');                        
          } else {
            $(this).parents("tr").removeClass('success');
          }
        });
        
         $('input.household-rejected').each(function () {
          if ($(this).is(":checked")) {
            $(this).parents("tr").removeClass('success');
            $(this).parents("tr").addClass('danger');                        
          } else {
            $(this).parents("tr").removeClass('danger');
          }
        });
        
        $('.household-bulk-manage-submitted-form .form-submit').click(function () {
          for (var i in CKEDITOR.instances) {
            CKEDITOR.instances[i].updateElement();
            $.trim($('#' + i).val());
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

<?php if (isset($form['empty_households'])) {
  print drupal_render($form['empty_households']);
} ?>
<?php if (isset($form['households'])) {
  print drupal_render($form['households']);
} ?>
  <?php if (isset($form['approve_justification'])) {
    print drupal_render($form['approve_justification']);
  } ?>
<?php if (isset($form['reject_justification'])) {
  print drupal_render($form['reject_justification']);
} ?>
<?php if (isset($form['user_password'])) {
  print drupal_render($form['user_password']);
} ?>
<?php if (isset($form['submit'])) { ?>
  <div class="form-group form-actions">
  <?php print drupal_render($form['submit']); ?> 
  </div>
<?php } ?>


<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>