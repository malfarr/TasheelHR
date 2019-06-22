<?php
$item = $variables['item'];
$total = $item['quantity'] * $item['price'];
$counter = $variables['counter'];
$tags = '';
if(isset($item['tags']) && !empty($item['tags'])){
  $tags = '<br /><span class="strong label label-primary">' . MED_FA_TAGS . ' ' . count($item['tags']) . '</span>';
}

$row_class = 'afr-row-odd';
if ($counter % 2 == 0) {
  $row_class = 'afr-row-even';
}

?>
<div class="afr-row afr-row-item <?php echo $row_class; ?>">
  <div class="afr-column afr-header-column afr-column-no text-center">
    <?php echo $counter . $tags; ?>
  </div>
  <div class="afr-column afr-header-column afr-column-expense-date">
    <?php echo date(MED_DATE_SIMPLE_MONTH_3_YEAR_2, $item['expense_date']); ?>
  </div>
  <div class="afr-column afr-header-column afr-column-account-code text-center">
    <?php echo $item['item_code']; ?>
  </div>
  <div class="afr-column afr-header-column afr-column-project-code text-center">
    <?php echo $item['project_code']; ?>
  </div>
  <div class="afr-column afr-header-column afr-column-activity-code text-center">
    <?php echo $item['activity_code']; ?>
  </div> 
   <div class="afr-column afr-header-column afr-column-output-code text-center">
    <?php echo ($item['output_code'] ? $item['output_code'] : ''); ?>
  </div> 
  <div class="afr-column afr-header-column afr-column-item">
    <?php echo $item['item_name']; ?>
  </div>
  <div class="afr-column afr-header-column afr-column-description">
    <?php echo check_markup($item['description'], 'filtered_html'); ?>
  </div>
  <div class="afr-column afr-header-column afr-column-project-name">
    <?php echo $item['project_name']; ?>
  </div>
  <div class="afr-column afr-header-column afr-column-quantity">
    <?php echo $item['quantity']; ?>
  </div>
  <div class="afr-column afr-header-column afr-column-price">
    <?php echo number_format($item['price'], 2); ?>
  </div>
  <div class="afr-column afr-header-column afr-column-total text-right">
    <?php echo number_format($total, 2); ?>
  </div>
  <div class="afr-column afr-header-column afr-column-cash-balance text-right">
    <?php echo number_format($item['cash_balance'], 2); ?>
  </div>
  <div class="afr-column afr-header-column afr-column-attachments">
    <?php echo implode('<br />', $item['attachments_links']); ?>
  </div>
  <div class="afr-column afr-header-column afr-column-actions text-center"> 

