<?php
$data = $variables['data'];
?>

<html <?php echo (isset($variables['dir']) ? 'dir="rtl"' : ''); ?> >
  <body>
    <?php foreach ($data as $row) { ?>
      <?php
      switch ($row['type']) {
        case "header":
          ?>
          <div class="header-row <?php echo (isset($row['class']) ? implode(' ', $row['class']) : ''); ?>">
          <?php echo $row['title']; ?>     
          </div>
          <?php break; ?>      
          <?php case "subheader": ?>
          <div class="subheader-row <?php echo (isset($row['class']) ? implode(' ', $row['class']) : ''); ?>">
          <?php echo $row['title']; ?>     
          </div>
          <?php break; ?> 
          <?php case "label": ?>
          <div class="subheader-row <?php echo (isset($row['class']) ? implode(' ', $row['class']) : ''); ?>">
          <?php echo $row['value']; ?>     
          </div>
          <?php break; ?> 
          <?php case "row": ?>
          <div class="row <?php echo (isset($row['class']) ? implode(' ', $row['class']) : ''); ?>">
              <?php if (isset($row['label'])) { ?>
              <span class="row-label <?php echo (isset($row['label_class']) ? implode(' ', $row['label_class']) : ''); ?>">
              <?php echo $row['label']; ?>
              </span>
            <?php } ?>            
              <?php if (isset($row['value'])) { ?>
              <span class="row-value <?php echo (isset($row['value_class']) ? implode(' ', $row['value_class']) : ''); ?>">
              <?php echo $row['value']; ?>
              </span>
          <?php } ?>                        
          </div>
          <?php break; ?>
          <?php case "raw": ?>
          <div class="row <?php echo (isset($row['class']) ? implode(' ', $row['class']) : ''); ?>">
          <?php echo $row['value']; ?>                    
          </div>
          <?php break; ?>
          <?php case "table": ?>
          <div class="row table-row <?php echo (isset($row['class']) ? implode(' ', $row['class']) : ''); ?>">
            <?php if (isset($row['label'])) { ?>
              <span class="row-label <?php echo (isset($row['label_class']) ? implode(' ', $row['label_class']) : ''); ?>">
              <?php echo $row['label']; ?>
              </span>
            <?php } ?>    
              <?php if (isset($row['value'])) { ?>
              <div class="row-value <?php echo implode(' ', $row['value_class']) ?>">
              <?php echo theme('table', $row['value']); ?>
              </div>
          <?php } ?>                                      
          </div>
          <?php break; ?>
          <?php case "list": ?>
          <div class="row list-row <?php echo (isset($row['class']) ? implode(' ', $row['class']) : ''); ?>">
              <?php if (isset($row['value'])) { ?>
              <div class="row-value <?php echo implode(' ', $row['value_class']) ?>">
              <?php echo theme('item_list', array('items' => $row['value'])); ?>
              </div>
          <?php } ?>                        
          </div>
          <?php break; ?>
        <?php case "hr": ?>
          <hr />
          <?php break; ?>
      <?php case "break": ?>
        <pagebreak />
        <?php break; ?>
        
        <?php case "container_start": ?>
        <div class="<?php echo (isset($row['class']) ? implode(' ', $row['class']) : ''); ?>">
        <?php break; ?>
          <?php case "container_end": ?>
        </div>
        <?php break; ?>
    <?php } ?>
<?php } ?>
</body>
</html>