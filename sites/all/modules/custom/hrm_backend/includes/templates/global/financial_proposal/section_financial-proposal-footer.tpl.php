<?php
$original_budget_total = $variables['original_budget_total'];
$actual_total = $variables['actual_total'];
$diff_total = $original_budget_total - $actual_total;
?>
<div class="financial-proposal-row financial-proposal-footer 
     financial-proposal-project-row financial-proposal-project-footer">
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-no">
    Total
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-account-code">
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-project-code">   
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-activity-code">
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-output-code">
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-item">
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-original-budget">
    <?php echo number_format($original_budget_total, 2);
    ; ?>
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-description">

  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-project">

  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-type">

  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-Quantity">

  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-price">

  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-total">
<?php echo number_format($actual_total, 2); ?>
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-diff">
<?php echo number_format($diff_total, 2); ?>
  </div>
  <div class="financial-proposal-column financial-proposal-header-column financial-proposal-column-actions">    
  </div>
</div>
</div>