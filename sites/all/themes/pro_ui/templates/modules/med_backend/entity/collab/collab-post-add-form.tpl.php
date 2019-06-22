
<div class="form-items-tab collab-post-add-tab mini-tabs">
  <ul class="nav nav-tabs mini-nav-tabs" data-toggle="tabs">          
    <li class=""><a id="btn-collab-post-form-tab-description" href="#collab-post-form-tab-description"><?php print MED_FA_ALIGN_LEFT; ?> Description</a></li>                        
    <li class=""><a id="btn-collab-post-form-tab-attachments" href="#collab-post-form-tab-attachments"><?php print MED_FA_PAPERCLIP; ?> Attachments <small>jpg, jpeg, gif, png, txt, doc, xls, docx, xlsx, pdf, ppt, pps</small></a></li>                    
  </ul>
  <div class="tab-content">
    <div class="tab-pane" id="collab-post-form-tab-description">
      <?php print drupal_render($form['post_add_content']); ?>
    </div>
    <div class="tab-pane" id="collab-post-form-tab-attachments">
      <?php print drupal_render($form['attachments']); ?>
    </div>
  </div>           
</div>  
<div class="form-actions">
  <?php print drupal_render($form['submit']); ?>  
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>

<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
      $('#collab-post-form-tab-description').addClass('active');
      $('#btn-collab-post-form-tab-description').parent().addClass('active');

      $('.collab-post-add-form').validate({
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

      $('.collab-post-add-form .form-submit').on('click', function () {
        if ($(".collab-post-add-form").valid()) {
          $('.collab-post-add-form').submit();
          $('.collab-post-add-form .form-submit').prop('disabled', true);
        } else {
          $('#collab-post-form-tab-description').addClass('active');
          $('#btn-collab-post-form-tab-description').parent().addClass('active');

          $('#collab-post-form-tab-attachments').removeClass('active');
          $('#btn-collab-post-form-tab-attachments').parent().removeClass('active');
          return false;
        }
      });
    });
  })(jQuery, Drupal, this, this.document);
</script>