  <script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {         
      }
    };
    $(document).ready(function(){
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php if(isset($form['buttons'])){?>
<div class="text-right">
<?php print drupal_render($form['buttons']); ?> 
</div>
<?php } ?>
<?php if(isset($form['survey_not_completed_messsage'])){?>
<?php print drupal_render($form['survey_not_completed_messsage']); ?> 
<?php } ?>
<?php print drupal_render($form['survey_details']); ?>  

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>