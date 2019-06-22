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

<?php print drupal_render($form['e_name']); ?>
<?php print drupal_render($form['a_name']); ?>
<?php print drupal_render($form['medair_id']); ?>
<?php print drupal_render($form['gender']); ?>
<?php print drupal_render($form['photo']); ?>
<?php print drupal_render($form['title']); ?>
<?php print drupal_render($form['work_phone']); ?>
<?php print drupal_render($form['personal_phone']); ?>
<?php print drupal_render($form['email']); ?>
<?php print drupal_render($form['role']); ?>
<?php print drupal_render($form['user_fields']); ?>
<div class="form-group form-item">
<?php print drupal_render($form['role_primary_account']); ?>
</div>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>