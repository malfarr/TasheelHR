<div id="page-staff-performance-wrapper" class="page-wrapper page-staff-performance-wrapper">
  <div id="staff-performance-form" class="block clearfix">
    <div class="block-title">
      <h2>Filter <strong> & </strong> Compare</h2>
    </div>
    <div class="block-content">  
      <div>
        <?php
        $performance_form = drupal_get_form('med_backend_user_staff_performance_form');
        echo drupal_render($performance_form);
        ?>
      </div>
    </div>
  </div>
  <div id="staff-performance-list" class="block">
    <div class="block-title">
      <h2><strong>Staff</strong> performance</h2>
      <div class="block-options">
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary" data-toggle="block-toggle-content"><?php print MED_FA_ARROWS_V; ?></a>
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary" data-toggle="block-toggle-fullscreen"><?php print MED_FA_EXPAND; ?></a>
      </div>
    </div>
    <div class="block-content">  
      <div class="table-responsive">
        <?php print theme('table', $variables['list']); ?>
      </div>
      <hr />
      <div class="table-responsive">
        <?php print $variables['chart']; ?>
      </div>
    </div>
  </div>
</div>