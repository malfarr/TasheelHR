<div class="page-wrapper page-template">
  <div class="block">
    <div class="block-title">                
      <?php if (isset($variables['title'])) { ?>
        <h2><?php print $variables['title']; ?></h2>
      <?php } ?>          
      <div class="block-options">          
        <?php if (isset($variables['result_counter']) && (!empty($variables['result_counter']) || $variables['result_counter'] >= 0)) { ?>
        <span class="result-counter pull-left"><strong><span class="result-counter-value"><?php echo (isset($variables['result_counter']) ? $variables['result_counter'] : ''); ?></span></strong> results</span>
        <?php } ?>          
        <?php if (isset($variables['filter_form'])) { ?>
          <ul class="custom-dropdown custom-dropdown-filters pull-left">          
            <li class="">
              <a href="javascript:void(0)" class="custom-dropdown-btn btn btn-primary btn-sm">
                <?php print MED_FA_FILTER; ?> FILTERS <?php print strtr(MED_FA_ANGEL_DOWN, array('extra_class' => 'custom-dropdown-btn-icon')); ?>
                <span class="label label-danger label-indicator animation-floating <?php echo (isset($variables['filters_count']) && $variables['filters_count'] ? '' : 'hidden'); ?>"><?php echo (isset($variables['filters_count']) && $variables['filters_count'] ? $variables['filters_count'] : ''); ?></span>
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
                <?php print MED_FA_COG; ?> Actions <?php print strtr(MED_FA_ANGEL_DOWN, array('extra_class' => 'custom-dropdown-btn-icon')); ?>
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
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary margin-left-20" data-toggle="block-toggle-content"><?php print MED_FA_ARROWS_V; ?></a>
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary btn-primary-full-screen" data-toggle="block-toggle-fullscreen"><?php print MED_FA_EXPAND; ?></a>          
      </div>
    </div>
    <div class="block-content">
      <?php if (isset($variables['buttons']) && !empty($variables['buttons'])) { ?>
        <div class="table-options clearfix">
          <div class="btn-group-sm pull-right">
            <?php echo implode(' ', $variables['buttons']); ?>
          </div>
        </div>                    
      <?php } ?>
      <div class="block-content-wrapper">
        <?php echo $variables['content']; ?>
      </div>

    </div>
  </div>
</div>