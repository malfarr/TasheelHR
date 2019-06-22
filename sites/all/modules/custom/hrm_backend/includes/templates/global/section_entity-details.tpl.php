<?php
  $entity_class = (isset($variables['entity_class']) ? implode(' ', $variables['entity_class']) : '');
  $data = $variables['data']; 
  drupal_add_library('system', 'drupal.collapse');
  
  $label_suffix = ':';
  if(isset($variables['entity_class']) && in_array('entity-details-wrapper-alt', $variables['entity_class'])){
    $label_suffix = '';
  }
  
?>

<div class="entity-details-wrapper <?php echo $entity_class; ?> clearfix">
  <?php foreach ($data as $row){ ?>
    <?php switch ($row['type']){
      case "header": ?>
        <fieldset class="panel form-wrapper collapsible <?php echo (isset($row['collapsed']) ? $row['collapsed'] : ''); ?> <?php echo (isset($row['row_class']) ? implode(' ', $row['row_class']) : ''); ?>">
          <legend class="panel-heading">
            <span class="panel-title fieldset-legend">
              <?php print strtr(MED_FA_CARET_UP, array('extra_class' => 'fieldset-arrow-indicator')); ?> <?php print strtr(MED_FA_CARET_DOWN, array('extra_class' => 'fieldset-arrow-indicator')); ?> <?php echo $row['title']; ?>    
            </span>
          </legend>
          <div class="panel-body fieldset-wrapper">        
      <?php break; ?>      
     <?php case "footer": ?>
            <?php echo (isset($row['prefix']) ? $row['prefix'] : ''); ?>
          </div>          
        </fieldset>
        <?php echo (isset($row['suffix']) ? $row['suffix'] : ''); ?>
      <?php break; ?>
         <?php  case "subheader": ?>
          <div class="review-row-wrapper review-row-subheader-header-wrapper <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : ''); ?>">
          <div class="review-row <?php echo (isset($row['row_class']) ? implode(" ", $row['row_class']) : '');?>">                        
              <?php if(isset($row['value'])){ ?>
                <span class="review-row-value display-table-cell <?php  echo (isset($row['value_class']) ? implode(" ", $row['value_class']) : ''); ?>">
                  <?php echo $row['value']; ?>
                </span>
              <?php } ?>                        
            </div>
          </div>
      <?php break; ?>   
      <?php case "label": ?>
        <div class="review-row-wrapper review-row-label-header-wrapper <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : '');?>">
          <div class="review-row <?php echo (isset($row['row_class']) ? implode(" ", $row['row_class']) : '');?>">                        
              <?php if(isset($row['value'])){ ?>
                <span class="review-row-value review-row-label-value display-table-cell <?php  echo (isset($row['value_class']) ? implode(" ", $row['value_class']) : ''); ?>">
                  <?php echo $row['value']; ?>
                </span>
              <?php } ?>                        
            </div>
          </div>
      <?php break; ?>
      <?php case "row": ?>
        <div class="review-row-wrapper <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : '');?>">
          <div class="review-row <?php echo (isset($row['row_class']) ? implode(' ', $row['row_class']) : ''); ?>">
              <?php if(isset($row['label'])){ ?>
                <span class="review-row-label display-table-cell <?php echo (isset($row['label_class']) ? implode(' ', $row['label_class']) : ''); ?>">
                  <?php echo $row['label']; ?><?php echo $label_suffix; ?>
                </span>
              <?php } ?>            
              <?php if(isset($row['value'])){ ?>
                <span class="review-row-value display-table-cell <?php echo (isset($row['value_class']) ? implode(" ", $row['value_class']) : ''); ?>">
                  <?php echo $row['value']; ?>
                </span>
              <?php } ?>                        
            </div>
          </div>
      <?php break; ?>
      <?php case "table": ?>
        <div class="review-row-wrapper <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : '');?>">
          <div class="review-row <?php echo (isset($row['row_class']) ? implode(' ', $row['row_class']) : ''); ?>">       
              <?php if(isset($row['value'])){ ?>
                <div class="review-row-value <?php echo (isset($row['value_class']) ? implode(" ", $row['value_class']) : ''); ?>">
                  <?php if(isset($row['responsive']) && $row['responsive']){ ?>
                    <div class="table-responsive">
                  <?php } ?>
                    <?php echo theme('table', $row['value']); ?>
                  <?php if(isset($row['responsive']) && $row['responsive']){ ?>
                    </div>
                  <?php } ?>
                </div>
              <?php } ?>                        
            </div>
          </div>
      <?php break; ?>
      <?php case "div_table": ?>
        <div class="review-row-wrapper <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : '');?>">
          <div class="review-row <?php echo (isset($row['row_class']) ? implode(' ', $row['row_class']) : ''); ?>">       
              <?php if(isset($row['value'])){ ?>
                <div class="review-row-value <?php echo (isset($row['value_class']) ? implode(" ", $row['value_class']) : ''); ?>">
                  <?php echo theme('divs_table', array('list' => $row['value'])); ?>
                </div>
              <?php } ?>                        
            </div>
          </div>
      <?php break; ?>
      <?php case "list": ?>
        <div class="review-row-wrapper <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : '');?>">
          <div class="review-row <?php echo (isset($row['row_class']) ? implode(' ', $row['row_class']) : ''); ?>">
              <?php if(isset($row['label'])){ ?>
                <span class="review-row-label display-table-cell <?php echo (isset($row['label_class']) ? implode(' ', $row['label_class']) : ''); ?>">
                  <?php echo $row['label']; ?>
                </span>
              <?php } ?>            
              <?php if(isset($row['value'])){ ?>
                <div class="review-row-value display-table-cell <?php echo (isset($row['value_class']) ? implode(" ", $row['value_class']) : ''); ?>">
                  <?php echo theme('item_list', array('items' => $row['value'])); ?>
                </div>
              <?php } ?>                        
            </div>
          </div>
      <?php break; ?>
      <?php case "raw": ?>
        <div class="review-row-wrapper <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : ''); ?>">
          <div class="review-row <?php echo (isset($row['row_class']) ? implode(' ', $row['row_class']) : ''); ?>">
            <?php echo $row['value']; ?>                    
          </div>
        </div>
      <?php break; ?>
      <?php case "controls": ?>
          <div class="table-options clearfix <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : '');?>">
            <div class="<?php echo implode(' ', $row['controls_class'])?>">
              <?php echo implode(' ', $row['value']); ?>
            </div>
          </div>           
      <?php break; ?>
      <?php case "horizintal_line": ?>
      <div class="col-sm-12"><hr /></div>
      <?php break; ?>
  <?php case "progress": ?>
  <div class="review-row-wrapper <?php echo (isset($row['wrapper_class']) ? implode(' ', $row['wrapper_class']) : '');?>">
  <div class="progress progress-striped active">
                <div id="progress-bar-wizard" class="progress-bar <?php echo (isset($row['value_class']) ? implode(' ', $row['value_class']) : '');?>" role="progressbar" aria-valuenow="<?php print $row['value']; ?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php print $row['value']; ?>%;"><?php print $row['value']; ?>%</div>
              </div>
    </div>
      <?php break; ?>
    <?php } ?>
  <?php } ?>
</div>
