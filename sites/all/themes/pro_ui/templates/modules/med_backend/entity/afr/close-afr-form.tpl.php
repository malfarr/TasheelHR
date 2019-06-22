<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {               
      }
    };
    
    $(document).ready(function () {
 
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php print drupal_render($form['afr']); ?>

<div class="form-changed-warning messages warning col-12" style="">
  <span class="warning form-changed">*</span> if you have remaining cash you have to upload remaining receipt and add justification.
</div>
<?php if(isset($form['projects_balance'])){
  print drupal_render($form['projects_balance']);
} ?>

<?php print drupal_render($form['remaining_receipt']); ?>
<?php print drupal_render($form['remaining_justification']); ?>

<div class="form-group form-actions">
  <?php   print drupal_render($form['buttons']); ?> </div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>