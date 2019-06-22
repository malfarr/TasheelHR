<?php
$entity_class = (isset($variables['entity_class']) ? implode(' ', $variables['entity_class']) : '');
$files = $variables['files'];
$temp_id = 'files-wrapper-' . med_basic_string_get_random_string() . '-' . time();

$display_seach = (isset($variables['search']) ? $variables['search'] : TRUE);
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
//      var tableOptions = $('.entity-files-wrapper').parents('.tab-pane').find('.table-options').length;      
//      if(!tableOptions){
//        $('#<?php echo $temp_id; ?>').css('margin-top', '50px');
//      }      
      
      $(function () {
        var $input = $("input[name='keyword']"),
                $context = $(".media-items");
        $input.on("input", function () {
          var term = $(this).val();
          $context.show().unmark();
          if (term) {
            $context.mark(term, {
              done: function () {
                $context.not(":has(mark)").hide();
              }
            });
          }
        });
      });
    });
  })(jQuery, Drupal, this, this.document);
</script>
<div id="<?php echo $temp_id; ?>" class="entity-files-wrapper <?php echo $entity_class; ?> clearfix">
  <?php if (isset($variables['files']) && !empty($variables['files']) && $display_seach) { ?>
    <div class="input-group col-sm-6 col-lg-4 pull-left entity-files-search">
      <input type="text" name="keyword" class="form-control input-sm" placeholder="Search...">
      <span class="input-group-addon"><?php print MED_FA_SEARCH; ?></span>
    </div>
  <?php } ?>
  <div class="media-filter-items col-sm-12 no-padding col-no-padding">
    <?php foreach ($variables['files'] as $file) { ?>
      <div class="col-sm-6 col-lg-4 media-item-wrapper" id="<?php echo (isset($file['id']) ? $file['id'] : ''); ?>">
        <div class="media-items">
          <?php if (isset($file['links']) && !empty($file['links'])) { ?>
          <div class="media-items-options text-left">            
            <?php echo implode(' ', $file['links']); ?>                 
          </div>
        <?php } ?>
        <div class="media-items-content animation-fadeInQuickInv">
          <i class="<?php echo $file['icon'] ?> fa-4x text-<?php echo $file['color_class'] ?>"></i>
        </div>
        <h4>
          <strong><?php echo $file['name']; ?></strong>.<?php echo $file['extension']; ?><br>
          <small><?php echo $file['caption']; ?></small>
        </h4>
        </div>
      </div>    
    <?php } ?>
  </div>
</div>