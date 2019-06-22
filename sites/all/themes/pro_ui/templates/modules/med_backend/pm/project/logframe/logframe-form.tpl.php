<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {   
        $(".remove-btn").prop('disabled', true);
      
        $('.activate-remove-btn').change(function() {
        if($(this).is(":checked")) {
          $(this).parent().parent().find(".remove-btn").prop('disabled', false);
        }
        else{
          $(this).parent().parent().find(".remove-btn").prop('disabled', true);
        }        
    });
      }
    };
    $(document).ready(function () {
      
    
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div class="form-group form-item">
  <?php print drupal_render($form['items']); ?>
</div>
<?php print drupal_render($form['revision_notes']); ?>
<div class="form-group form-actions">
  <?php print drupal_render($form['buttons']); ?> 
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>