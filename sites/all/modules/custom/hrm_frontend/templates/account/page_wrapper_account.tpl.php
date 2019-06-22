<?php
$content = $variables['content'];
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.hr_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
    });

  })(jQuery, Drupal, this, this.document);
</script>


<div class="block user-account-block">
  <div class="block-title">
    <ul class="nav nav-tabs">
      <?php if($variables['display_account']){ ?>
      <li class="<?php echo $variables['account_class']; ?> text-center" style="width: 33%;">
        <?php echo l(MED_FA_USER . 'Account', 'account', array('html' => TRUE)); ?>      
      </li>
      <?php } ?>
      <?php if($variables['display_settings']){ ?>
      <li class="<?php echo $variables['settings_class']; ?> text-center" style="width: 33%;">
        <?php echo l(MED_FA_COGS . 'Account Settings', 'account/settings', array('html' => TRUE)); ?>            
      </li>
      <?php } ?>
      <?php if($variables['display_notifications']){ ?>
      <li class="<?php echo $variables['notifications_class']; ?> text-center" style="width: 33%;">
        <?php echo l(MED_FA_BULLHORN . 'Email notifications', 'account/notifications', array('html' => TRUE)); ?>            
      </li>
      <?php } ?>
    </ul>      
  </div>
  <div class="block-content clearfix <?php echo implode(' ', $variables['block_content_class']); ?>"> 
    <?php echo $content; ?>
  </div>
</div>