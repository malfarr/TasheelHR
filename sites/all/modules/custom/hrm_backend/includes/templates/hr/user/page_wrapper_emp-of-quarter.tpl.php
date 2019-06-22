<?php
drupal_add_library('system', 'drupal.collapse');
?>
<div id="page-emp-of-quarter-wrapper" class="page-wrapper page-emp-of-quarter-wrapper">
  <div id="emp-of-quarter-list" class="block">
    <div class="block-title">
      <h2><strong>Employee</strong> of Quarter</h2>
      <div class="block-options">
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary" data-toggle="block-toggle-content"><?php print MED_FA_ARROWS_V; ?></a>
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary" data-toggle="block-toggle-fullscreen"><?php print MED_FA_EXPAND; ?></a>          
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

      <?php if (isset($variables['data']) && !empty($variables['data'])) { ?>
        <div id="employee-of-quarter">
          <?php foreach ($variables['data'] as $key => $year) { ?>          
            <fieldset class="panel form-wrapper collapsible collapsed">
              <legend class="panel-heading">
                <span class="panel-title fieldset-legend">
                  <?php print MED_FA_CARET_UP; ?> <?php print MED_FA_CARET_DOWN; ?> <?php echo $key ?>
                </span>
              </legend>
              <div class="panel-body fieldset-wrapper">
                <ul class="med-hierarchy-list emp-of-quarter-list">
                  <?php foreach ($year as $quarter) { ?>                          
                    <li class="med-hierarchy-list-item emp-of-quarter-list-item">
                      <div class="quarter-title">Q<?php echo $quarter['querter']; ?></div>
                      <div class="emp-of-quarter-list-item-centent-wrapper">                                         
                        <div class="emp-of-quarter-list-item-image-wrapper">
                          <img src="<?php echo $quarter['image_url']; ?> " alt="<?php echo $quarter['emp_name']; ?>" class="img-circle">
                        </div>
                        <div><?php echo l($quarter['emp_name'], 'med-users/details/' . $quarter['emp_uid']) ?></span> <span><?php echo $quarter['emp_title']; ?></div>
                        <div><?php echo check_markup($quarter['text'], 'filtered_html'); ?></div>
                        <div>Selected by <?php echo $quarter['selected_by_name'] ?></div>           
                      </div>
                    </li> 
                  <?php } ?>
                </ul>
              </div>
            </fieldset>          
          <?php } ?>
        </div>
      <?php } ?>
    </div>
  </div>
</div>