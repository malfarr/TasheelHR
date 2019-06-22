<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
    });
  })(jQuery, Drupal, this, this.document);
</script>
<div id="page-form-wrapper" class="page-wrapper page-form-wrapper <?php echo (isset($variables['wrapper_class']) ? implode(' ', $variables['wrapper_class']) : ''); ?>">
  <div id="page-form-block" class="block <?php echo (isset($variables['block_class']) ? implode(' ', $header['block_class']) : ''); ?>">
      <div class="block-title">
        <h2><?php echo $variables['title']; ?></h2>
      </div>
      <div class="block-content">
        <?php
          print drupal_render($variables['form']);
        ?>   
        </div>
    </div>
</div>