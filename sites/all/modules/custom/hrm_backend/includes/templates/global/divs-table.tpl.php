<?php
$attributes = $variables['list']['attributes'];
$header = array();
if(isset($variables['list']['header'])){
  $header = $variables['list']['header'];
}
$rows = $variables['list']['rows'];
?>
<div id="<?php echo $attributes['id']; ?>" class="divs-table <?php echo implode(" ", $attributes['class']) ?>" >
  <div class="divs-table-row divs-table-header <?php echo (isset($row['class']) ? implode(" ", $row['class']) : ''); ?>">
    <?php if(isset($header['data'])){ ?>
      <?php foreach ($header['data'] as $row) { ?>    
      <div class="divs-table-cell divs-table-header-cell <?php echo (isset($row['class']) ? implode(" ", $row['class']) : ''); ?>">
        <?php echo $row['data']; ?>
      </div>
    <?php } ?>
    <?php } ?>
  </div>
  <?php $counter = 0; ?>
  <?php foreach ($rows as $row) { ?>
    <?php $class_zebra = 'divs-table-row-' . ($counter % 2 == 0 ? 'even' : 'odd') ?>
    <div class="divs-table-row <?php echo $class_zebra; ?> <?php echo 'divs-table-row-' . $counter; ?> <?php echo (isset($row['class']) ? implode(" ", $row['class']) : ''); ?>">
      <?php foreach ($row['data'] as $value) { ?>
        <div class="divs-table-cell divs-table-row-cell <?php echo (isset($value['class']) ? implode(" ", $value['class']) : ''); ?>">
          <?php echo $value['data']; ?>
        </div>
      <?php } ?>
    </div>
    <?php $counter++; ?>
  <?php } ?>
</div>