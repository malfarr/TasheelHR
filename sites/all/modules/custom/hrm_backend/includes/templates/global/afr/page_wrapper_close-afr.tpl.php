<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php 
  $title = $variables['title'];
?>
<div id="page-close-afr-wrapper" class="page-wrapper page-close-afr-wrapper">
  <div id="close-afr-block" class="block">
      <div class="block-title">
        <h2><?php echo $title ?> <strong>Activity Financial Report</strong></h2>
      </div>
      <div class="block-content">
        <?php
          echo $variables['content'];
        ?>   
        </div>
    </div>
</div>