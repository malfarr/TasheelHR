<div class="pie-chart easyPieChart" data-percent="<?php echo $variables['data_percent']; ?>" data-line-width="6" data-bar-color="<?php echo $variables['data_bar_color']; ?>" data-track-color="#cccccc" style="width: 80px; height: 80px; line-height: 80px;">
  <span class="strong"><?php echo $variables['data_percent']; ?>%</span>
</div>
<fieldset class="panel form-wrapper form-fieldset">
  <div class="panel-body" style="border: 0 none;">
    <ul class="checklist-list">
      <?php foreach ($variables['data'] as $row) { ?>
        <?php if ($row['completed']) { ?>
          <li class="task-done"><?php echo MED_FA_CHECK_GREEN; ?><span><?php echo $row['text']; ?></span></li>
        <?php } else { ?>
          <li class=""><?php echo MED_FA_SPINNER_RED; ?><span><?php echo $row['text']; ?></span></li>
        <?php } ?>
      <?php } ?>
    </ul>
  </div>
</fieldset>