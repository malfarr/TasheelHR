<?php
$message = $variables['message'];
$type = $variables['type'];
?>

<div class="alert alert-block alert-<?php echo $type; ?> custom-alert-block">  
  <?php echo $message; ?>
</div>