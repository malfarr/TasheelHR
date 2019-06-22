<div id="pmf-view" class="pmf-view">
  <?php if (isset($variables['buttons']) && !empty($variables['buttons'])) { ?>
    <div class="table-options clearfix">
      <div class="btn-group-sm pull-right">
        <?php echo implode(' ', $variables['buttons']); ?>
      </div>
    </div>                    
  <?php } ?>
  <?php if (isset($variables['info']) && !empty($variables['info'])) { ?>
    <?php echo $variables['info']; ?>                  
  <?php } ?>
  <?php if (isset($variables['pmf_list']) && !empty($variables['pmf_list'])) { ?>
    <div class="table-responsive">
      <?php echo theme('table', $variables['pmf_list']); ?>                  
    </div>
  <?php } ?>
</div>