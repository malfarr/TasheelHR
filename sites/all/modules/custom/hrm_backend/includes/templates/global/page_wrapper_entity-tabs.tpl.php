<?php
$entity = (isset($variables['entity']) ? str_replace('_', '-', $variables['entity']) : 'none');
$default_tab = filter_input(INPUT_GET, 'tab', FILTER_SANITIZE_STRING);
if (!$default_tab) {
  $tabs_keys = array_keys($variables['tabs']);
  $default_tab = reset($tabs_keys);
  
  if(isset($variables['default_tab'])){
    $default_tab = $variables['default_tab'];
  }
}



$actions_btn_label = MED_FA_COG . ' Actions ' . strtr(MED_FA_ANGEL_DOWN, array('extra_class' => 'custom-dropdown-btn-icon'));

?>

<script>
  (function ($) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
      var tab = '<?php echo $default_tab; ?>';
      if (tab !== '') {
        $('ul.nav-tabs li').removeClass('active');
        $('#btn-tab-' + tab).parents('li').addClass('active');

        $('.tab-pane').removeClass('active');
        $('#tab-' + tab).addClass('active');
      }

      $('a#btn-tab-summary').click(function (event) {
        $('#map-canvas-view').gmap('refresh');
      });
      $('fieldset.financial-items-fieldset a.fieldset-title').click(function (event) {
        $('fieldset.financial-items-fieldset #map-canvas-direction').gmap('refresh');
        $('fieldset.financial-items-fieldset #map-canvas-direction').gmap('option', 'zoom', 13);
      });
     
      $('.count-to-number').data('countToOptions', {
        formatter: function (value, options) {
          return value.toFixed(options.decimals).replace(/\B(?=(?:\d{3})+(?!\d))/g, ',');
        }
      });

      $('.count-to-number').each(count);

      function count(options) {
        var $this = $(this);
        options = $.extend({}, options || {}, $this.data('countToOptions') || {});
        $this.countTo(options);
      }

      if ($('.collab-content-scroll-wrapper').length) {
        var scrollToVal = $('.collab-content-scroll-wrapper').scrollTop() + $('.collab-last-post').position().top;
        $('.collab-content-scroll-wrapper').slimScroll({scrollTo: scrollToVal + 'px'});
      }


//      jQuery('.collab-content-scroll-wrapper').animate({
//        scrollTop: jQuery(".collab-last-post").offset().top
//    }, 500);

    });
  })(jQuery, Drupal, this, this.document);
</script>

<div id="page-<?php echo $entity; ?>-details-wrapper" class="page-wrapper page-<?php echo $entity; ?>-details-wrapper">
  <div id="<?php echo $entity; ?>-details" class="block">
    <div class="block-title clearfix block-title-flex">
      <ul class="nav nav-tabs" data-toggle="tabs">
        <?php foreach ($variables['tabs'] as $tab_key => $tab) { ?>
          <li>
            <a id="btn-tab-<?php echo $tab_key; ?>" href="#tab-<?php echo $tab_key; ?>">
              <?php echo $tab['title']; ?>
              <?php print (isset($tab['title_indicator']) ? '<span class="tab-count-indicator label label-danger">' . $tab['title_indicator'] . '</span>' : ''); ?>
            </a>
          </li>        
        <?php } ?>            
      </ul>      
      <div class="block-options">
        <?php if (isset($variables['status'])) { ?>
        <div class="block-title-status white-space-nowrap">
          <span class="bold"><?php echo $variables['status']; ?></span>
        </div>
      <?php } ?>  
        <?php if (isset($variables['filter_form'])) { ?>          
          <ul class="nav navbar-nav-filter-form pull-left">
            <!-- User Dropdown -->
            <li class="dropdown">
              <a href="javascript:void(0)" class="dropdown-toggle strong" data-toggle="dropdown" aria-expanded="false">
                <?php print MED_FA_FILTER; ?> FILTERS <?php print strtr(MED_FA_ANGEL_DOWN, array('extra_class' => 'custom-dropdown-btn-icon')); ?>
              </a>        
              <span class="navbar-nav-filter-form-result-counter pull-left">
                <?php if (isset($variables['filters_count']) && $variables['filters_count']) { ?>
                  <?php echo (isset($variables['content']['result_counter']) ? number_format($variables['content']['result_counter']) . ' results' : '') ?>
                <?php } ?>
              </span>
              <ul class="dropdown-menu dropdown-custom dropdown-menu-right">
                <li class="dropdown-header text-center bold upper-case">Search Filters...</li>
                <li>
                  <?php echo drupal_render($variables['filter_form']); ?>
                </li>
              </ul>
            </li>
            <!-- END User Dropdown -->
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
      <?php if (isset($variables['buttons']) && !empty($variables['buttons'])) { ?>
        <div class="table-options clearfix">
          <div class="btn-group-sm pull-right btn-group-strong">
            <?php echo implode(' ', $variables['buttons']); ?>
          </div>
        </div>                    
      <?php } ?>
      <div class="tab-content">
        <?php foreach ($variables['tabs'] as $tab_key => $tab) { ?>
          <div id="tab-<?php echo $tab_key; ?>"
               class="tab-pane tab-<?php echo $tab_key; ?> <?php print (isset($tab['content_class']) ? implode(' ', $tab['content_class']) : ''); ?>">
                 <?php if (isset($tab['buttons']) && !empty($tab['buttons'])) { ?>
              <div class="table-options clearfix">
                <div class="btn-group-sm pull-right">
                  <?php echo implode(' ', $tab['buttons']); ?>
                </div>
              </div>                    
            <?php } ?>
            <?php print $tab['content']; ?>
          </div>
        <?php } ?>        
      </div>
    </div>
  </div>
</div>
