<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('.pictures-edit-container').removeClass('form-group');
        $('.picture-edit-container').removeClass('form-group');

        $('.picture-action-delete').click(function () {
          var picture_container = $(this).parents('.picture-edit-container');
          $(picture_container).find('.picture-delete-field').val(1);
          $(picture_container).find('.picture-edit-inner').addClass('picture-deleted');
          $(picture_container).find('.picture-edit-inner .picture-action-delete').addClass('themed-color-red');
          $(picture_container).find('.picture-delete-redo').addClass('visible');
        });
        $('.picture-action-comm-mtrl').click(function () {
          var picture_container = $(this).parents('.picture-edit-container');
          var comm_mtrl = $(picture_container).find('.picture-comm-mtrl-field').val();

          if (comm_mtrl == 0) {
            $(picture_container).find('.picture-comm-mtrl-field').val(1);
            $(this).addClass('active themed-color-blue animation-fadeInQuick');
            
            $(picture_container).find('.picture-action-timeline').show();
          }
          else {
            $(picture_container).find('.picture-comm-mtrl-field').val(0);
            $(this).removeClass('active themed-color-blue animation-fadeInQuick');
            
            $(picture_container).find('.picture-action-timeline').hide();
            $(picture_container).find('.picture-timeline-field').val(0);
            $(picture_container).find('.picture-action-timeline').removeClass('active themed-color-yellow animation-fadeInQuick');
          }
          
          
        });
        $('.picture-action-timeline').click(function () {
          var picture_container = $(this).parents('.picture-edit-container');
          var timeline = $(picture_container).find('.picture-timeline-field').val();

          if (timeline == 0) {            
            $(picture_container).find('.picture-timeline-field').val(1);
            $(this).addClass('active themed-color-yellow animation-fadeInQuick');

          }
          else {
            $(picture_container).find('.picture-timeline-field').val(0);            
            $(this).removeClass('active themed-color-yellow animation-fadeInQuick');
          }
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

        $('.entity-pictures-edit-form').validate({
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

<?php print drupal_render($form['pictures']); ?>

<div class="form-group form-actions">
  <?php print drupal_render($form['buttons']); ?>   
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>