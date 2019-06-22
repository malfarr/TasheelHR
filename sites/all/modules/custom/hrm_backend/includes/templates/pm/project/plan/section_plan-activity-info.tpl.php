<div id="plan-activity-info" class="plan-activity-info">
  <?php if (isset($variables['actions']) && !empty($variables['actions'])) { ?>
    <div class="table-options clearfix">
      <div class="btn-group-sm pull-right">
        <?php echo implode(' ', $variables['actions']); ?>
      </div>
    </div>                    
  <?php } ?>
  <div class="clearfix">
    <div class="activity-date">
      <div class="month"><?php print $variables['month']; ?></div>
      <div class="year"><?php print $variables['year']; ?></div>
    </div>
    <div class="activity-description">
      <p><strong><?php print $variables['title']; ?></strong></p>
      <?php if (isset($variables['description'])) { ?>
        <?php print $variables['description']; ?>
      <?php } ?>        
    </div>
  </div>    

  <?php if (isset($variables['type'])) { ?>
    <div class="activity-type">
      <h4 class="sub-header">Activity Type</h4>
      <?php print $variables['type']; ?>
    </div>
  <?php } ?>

  <?php if (isset($variables['location'])) { ?>
    <div class="activity-location">
      <h4 class="sub-header">Location</h4>
      <?php print $variables['location']; ?>
    </div>
  <?php } ?>

  <?php if (isset($variables['program'])) { ?>
    <div class="activity-program">
      <h4 class="sub-header">Program</h4>
      <?php print $variables['program']; ?>
    </div>
  <?php } ?>

  <?php if (isset($variables['benef'])) { ?>
    <div class="activity-benef">
      <h4 class="sub-header">Expected Beneficiaries</h4>
      <?php print $variables['benef']; ?>
    </div>
  <?php } ?>

  <?php if (isset($variables['cost'])) { ?>
    <div class="activity-cost">
      <h4 class="sub-header">Activity Cost</h4>
      <?php print $variables['cost']; ?>
    </div>
  <?php } ?>
</div>
