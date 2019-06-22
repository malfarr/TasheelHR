<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.pictures-edit-container').removeClass('form-group');
        $('.picture-edit-container').removeClass('form-group');
        $('.attachments-edit-container').removeClass('form-group');
        $('.attachments-edit-container').removeClass('form-group');
        $('.attachments-edit-container .attachment-edit-container').removeClass('form-group');
        $('.attachments-edit-container .attachment-edit-container').removeClass('form-group');
               
        $('.picture-action-delete').click(function () {
          var picture_container = $(this).parents('.picture-edit-container');
          $(picture_container).find('.picture-delete-field').val(1);
          $(picture_container).find('.picture-edit-inner').addClass('picture-deleted');
          $(picture_container).find('.picture-edit-inner .picture-action-delete').addClass('themed-color-red');
          $(picture_container).find('.picture-delete-redo').addClass('visible');
        });
        $('.picture-action-rotate').click(function () {
          var picture_container = $(this).parents('.picture-edit-container');

          var rotate_value = $(picture_container).find('.picture-rotate-field').val();
          rotate_value = parseInt(rotate_value) + 90;
          if (rotate_value > 270) {
            rotate_value = 0;
          }
          $(picture_container).find('.picture-rotate-field').val(rotate_value);
          $(picture_container).find('.picture-wrapper img').removeClass('img-rotate-0 img-rotate-90 img-rotate-180 img-rotate-270 img-rotate-360').addClass('img-rotate-' + rotate_value);
        });
        $('.picture-action-delete-redo').click(function () {
          var picture_container = $(this).parents('.picture-edit-container');

          $(picture_container).find('.picture-delete-field').val(0);
          $(picture_container).find('.picture-edit-inner').removeClass('picture-deleted');
          $(picture_container).find('.picture-edit-inner .picture-action-delete').removeClass('themed-color-red');
          $(picture_container).find('.picture-delete-redo').removeClass('visible');
        });

        $('.attachment-action-delete').click(function () {
          var attachment_container = $(this).parents('.attachment-edit-wrapper');
          $(attachment_container).find('.attachment-delete-field').val(1);
          $(attachment_container).find('.attachment-edit-inner').remove();
          $(attachment_container).removeClass('col-sm-4');
        });
        
        $('.tracker-add-form').validate({
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
            description: {
              required: true
            }
          },
          messages: {
          }
        });
        $('.tracker-add-form .form-submit').click(function () {
          for (var i in CKEDITOR.instances) {
            CKEDITOR.instances[i].updateElement();
            $.trim($('#' + i).val());
          }
          if ($(".tracker-add-form").valid()) {
            $(".tracker-add-form").submit();
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
<?php print drupal_render($form['title']); ?>
<?php print drupal_render($form['description']); ?>
<?php print drupal_render($form['photos']); ?>
<?php print drupal_render($form['documents']); ?>
<?php if(isset($form['pictures_edit'])) { print drupal_render($form['pictures_edit']); } ?>
<?php if(isset($form['attachments_edit'])) { print drupal_render($form['attachments_edit']); } ?>

<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?>   
  <?php print drupal_render($form['cancel']); ?>     
</div>
<?php
  print drupal_render($form['form_build_id']);
  print drupal_render($form['form_token']);
  print drupal_render($form['form_id']);
?>