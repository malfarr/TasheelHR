<?php
$entity = (isset($variables['entity']) ? str_replace('_', '-', $variables['entity']) : 'none');

$actions_btn_label = MED_FA_COG . ' Actions ' . strtr(MED_FA_ANGEL_DOWN, array('extra_class' => 'custom-dropdown-btn-icon'));
$filters_btn_label = MED_FA_FILTER . ' FILTERS ' . strtr(MED_FA_ANGEL_DOWN, array('extra_class' => 'custom-dropdown-btn-icon'));

?>

<script>
  (function ($) {
    Drupal.behaviors.med_behavior = {attach: function (context, settings) {}};
    $(document).ready(function () {});
  })(jQuery, Drupal, this, this.document);
</script>

<div id="page-<?php echo $entity; ?>-details-wrapper" class="page-wrapper page-<?php echo $entity; ?>-details-wrapper">
  <div id="<?php echo $entity; ?>-details" class="block">
    <div class="block-title clearfix block-title-flex">
      <ul class="nav nav-tabs">
        <?php foreach ($variables['tabs'] as $tab) { ?>
          <li><?php print $tab['title']; ?></li>        
        <?php } ?>            
      </ul>       
      <div class="block-options">
        <?php if (isset($variables['status'])) { ?>
        <h3 class="white-space-nowrap">
          <span class="bold"><?php echo $variables['status']; ?></span>
        </h3>
      <?php } ?> 
        <span class="navbar-nav-filter-form-result-counter pull-left">
          <?php if (isset($variables['result_count']) && is_numeric($variables['result_count'])) { ?>
            <?php echo number_format($variables['result_count']) . ' results'; ?>
          <?php } ?>
        </span>        
        <?php if (isset($variables['filter_form'])) { ?>
          <ul class="custom-dropdown custom-dropdown-filters pull-left">          
            <li class="">
              <a href="javascript:void(0)" class="custom-dropdown-btn btn btn-primary btn-sm">
                <?php print (isset($variables['filters_size']) && $variables['filters_size'] == 'small' ? MED_FA_FILTER : $filters_btn_label); ?>
                <?php if (isset($variables['filters_count']) && $variables['filters_count']) { ?>
                  <span class="label label-danger label-indicator animation-floating"><?php echo $variables['filters_count']; ?></span>
                <?php } ?>
              </a>                         
              <ul class="custom-dropdown-content custom-dropdown-right">
                <li class="custom-dropdown-header text-center bold upper-case">Search Filters...</li>
                <li class="custom-dropdown-inner">
                  <?php echo drupal_render($variables['filter_form']); ?>
                </li>
              </ul>
            </li>          
          </ul>
        <?php } ?>  
        <?php if (isset($variables['actions']) && !empty($variables['actions'])) { ?>
          <ul class="custom-dropdown custom-dropdown-actions pull-left">          
            <li>
              <a href="javascript:void(0)" class="custom-dropdown-btn btn btn-success btn-sm">
                <?php print (isset($variables['actions_size']) && $variables['actions_size'] == 'small' ? MED_FA_COG : $actions_btn_label); ?>
                <?php if (isset($variables['actions']['actions_count']) && $variables['actions']['actions_count']) { ?>
                  <span class="label label-danger label-indicator animation-floating"><?php echo MED_FA_BELL; ?></span>
                  <?php unset($variables['actions']['actions_count']); ?>
                <?php } ?>                                  
              </a>                         
              <ul class="custom-dropdown-content custom-dropdown-right">
                <li class="custom-dropdown-header text-center bold upper-case">Actions...</li>
                <?php foreach ($variables['actions'] as $action) { ?>
                  <li class="custom-dropdown-action-item"><?php echo $action; ?></li>
                <?php } ?>
              </ul>
            </li>          
          </ul>
        <?php } ?>
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary" data-toggle="block-toggle-content"><?php print MED_FA_ARROWS_V; ?></a>
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary btn-primary-full-screen" data-toggle="block-toggle-fullscreen"><?php print MED_FA_EXPAND; ?></a>        
      </div>
    </div>
    <div class="block-content">      
      <div class="tab-content">
        <?php if (isset($variables['buttons']) && !empty($variables['buttons'])) { ?>
          <div class="table-options clearfix">
            <div class="btn-group-sm pull-right btn-group-strong">
              <?php echo implode(' ', $variables['buttons']); ?>
            </div>
          </div>                    
        <?php } ?>
        <?php print $variables['content']; ?>
      </div>
    </div>
  </div>
</div>
