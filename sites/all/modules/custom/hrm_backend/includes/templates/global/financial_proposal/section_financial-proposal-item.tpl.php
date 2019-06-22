<?php 
  $item = $variables['item'];
  $counter = $variables['counter'];  
  $subitems = $item['subitems'];          
?>
<?php $rows_counter = 0; ?>
<?php $original_budget = $item['original_budget']; ?>
<?php foreach ($subitems as $subitem){?>
  <?php 
    $total = $subitem['quantity'] * $subitem['price'];
    $diff = $original_budget - $total;    

    $row_class = 'financial-proposal-row-odd';
    if($counter % 2 == 0){
    $row_class = 'financial-proposal-row-even';
  }
  ?>
  <?php if($rows_counter != (count($subitems) - 1)){ ?>    
    <div class="financial-proposal-row financial-proposal-row-item
                 financial-proposal-project-row financial-proposal-project-row-item <?php echo $row_class;?>">
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-no">
        <?php echo ($rows_counter == 0 ? $counter : '');?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-account-code">
        <?php echo $item['item_code'];?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-project-code">
        <?php echo $subitem['project_code'];?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-activity-code">
        <?php echo $item['activity_code'];?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-output-code">
        <?php echo ($subitem['output_code'] ? $subitem['output_code'] : '');?>        
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-item">
        <?php echo ($rows_counter == 0 ? $item['item_name'] : '');?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-original-budget">
        <?php if ($original_budget == $item['original_budget']){?>
          <?php echo number_format($original_budget, 2); ?>
        <?php } else{?>
          <span class="red-italic-text"><?php echo number_format($original_budget, 2);?></span>
        <?php }?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-description">
        <?php echo check_markup($subitem['description'], 'filtered_html'); ?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-project">
        <?php echo $subitem['project_name']?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-type">
        <?php echo (($subitem['stock_item']) ? 'Stock' : 'Advance');?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-Quantity">
        <?php echo $subitem['quantity'];?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-price">
        <?php echo number_format($subitem['price'], 2); ?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-total">
        <?php echo number_format($total, 2);?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-diff">
        <?php echo number_format($diff, 2);?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-actions"> 
      </div>
    </div>
  <?php }else{ ?>
  <div class="financial-proposal-row financial-proposal-row-item 
       financial-proposal-project-row financial-proposal-project-row-item <?php echo $row_class;?>">
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-no">
        <?php echo ($rows_counter == 0 ? $counter : '');?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-account-code">
        <?php echo $item['item_code'];?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-project-code">
        <?php  echo $subitem['project_code']; ?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-activity-code">
        <?php echo $item['activity_code'];?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-output-code">
        <?php echo ($subitem['output_code'] ? $subitem['output_code'] : '');?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-item">
        <?php echo ($rows_counter == 0 ? $item['item_name'] : '');?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-original-budget">
        <?php if ($original_budget == $item['original_budget']){?>
          <?php echo number_format($original_budget, 2);?>
        <?php } else{?>
          <span class="red-italic-text"><?php echo number_format($original_budget, 2);?></span>
        <?php }?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-description">
        <?php echo check_markup($subitem['description'], 'filtered_html');?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-project">
        <?php echo $subitem['project_name'];?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-type">
        <?php echo (($subitem['stock_item']) ? 'Stock' : 'Advance');?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-Quantity">
        <?php echo $subitem['quantity'];?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-price">
        <?php  echo number_format($subitem['price'], 2); ?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-total">
        <?php echo number_format($total, 2);?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-diff">
        <?php echo number_format($diff, 2);?>
      </div>
      <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-actions"> 

  <?php } ?>
  <?php $original_budget = $original_budget - $total; ?>
  <?php $rows_counter++; ?>
<?php } ?>