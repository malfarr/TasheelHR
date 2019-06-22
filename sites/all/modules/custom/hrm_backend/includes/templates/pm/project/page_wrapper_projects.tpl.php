<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div class="projects-list">
  <?php if (isset($variables['buttons']) && !empty($variables['buttons'])) { ?>
    <div class="table-options clearfix pull-right">
      <div class="btn-group-sm pull-right">
        <?php echo implode(' ', $variables['buttons']); ?>
      </div>
    </div>                    
  <?php } ?>

  <?php if ($variables['display_active'] && isset($variables['active_projects']) && !empty($variables['active_projects'])) { ?>
    <h3 class="sub-header themed-color-green strong">Active</h3>
    <div class="row">
      <?php foreach ($variables['active_projects'] as $project) { ?>      
        <div class="col-sm-12 col-md-6 col-lg-6">
          <a href="<?php echo $project['url']; ?>" class="widget widget-hover-effect1 animation-fadeInQuick" >
            <div class="widget-simple" style="background-color: <?php echo $project['color_code']; ?>;">
              <?php echo theme('image', array('path' => $project['logo_uri'], 'attributes' => array('class' => array('widget-image', 'img-circle', 'pull-left')))); ?>           
              <h3 class="widget-content widget-content-light">
                <?php echo $project['name'] . ' - <strong>' . $project['abbrev'] . '</strong>'; ?>
                <small><?php echo $project['donor_name']; ?></small>
              </h3>
            </div>
            <div class="widget-extra">
              <div class="row text-center animation-fadeInQuick">
                <?php foreach ($project['indicators'] as $indicator) { ?>      
                  <div class="<?php echo $indicator['class']; ?>">
                    <h3 style="color: <?php echo $project['color_code']; ?>;">
                      <b class="count-to-number" data-to="<?php echo $indicator['actual']; ?>" data-speed="5000"></b><small>/<?php echo $indicator['target']; ?></small><br>
                      <small><?php echo $indicator['label']; ?></small>
                    </h3>
                  </div>
                <?php } ?>                 
              </div>
              <div class="progress progress-striped active">
                <div id="progress-bar-wizard" class="progress-bar <?php print $project['progress_class']; ?>" role="progressbar" aria-valuenow="<?php print $project['progress']; ?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php print $project['progress']; ?>%;"></div>
              </div>
            </div>
          </a>
        </div>
      <?php } ?>      
    </div>
  <?php } ?>

  <?php if ($variables['display_approved'] && isset($variables['approved_projects']) && !empty($variables['approved_projects'])) { ?>
    <h3 class="sub-header themed-color-blue strong">Approved</h3>
    <div class="row">
      <?php foreach ($variables['approved_projects'] as $project) { ?>      
        <div class="col-sm-12 col-md-6 col-lg-6">
          <a href="<?php echo $project['url']; ?>" class="widget widget-hover-effect1 animation-fadeInQuick" >
            <div class="widget-simple" style="background-color: <?php echo $project['color_code']; ?>;">
              <?php echo theme('image', array('path' => $project['logo_uri'], 'attributes' => array('class' => array('widget-image', 'img-circle', 'pull-left')))); ?>           
              <h3 class="widget-content widget-content-light">
                <?php echo $project['name'] . ' - <strong>' . $project['abbrev'] . '</strong>'; ?>
                <small><?php echo $project['donor_name']; ?></small>
              </h3>
            </div>
            <div class="widget-extra">
              <div class="row text-center animation-fadeInQuick">
                <?php foreach ($project['indicators'] as $indicator) { ?>      
                  <div class="<?php echo $indicator['class']; ?>">
                    <h3 style="color: <?php echo $project['color_code']; ?>;">
                      <b class="count-to-number" data-to="<?php echo $indicator['actual']; ?>" data-speed="5000"></b><small>/<?php echo $indicator['target']; ?></small><br>
                      <small><?php echo $indicator['label']; ?></small>
                    </h3>
                  </div>
                <?php } ?>
              </div>            
            </div>
          </a>
        </div>
      <?php } ?>
    </div>
  <?php } ?>

  <?php if ($variables['display_pending_approval'] && isset($variables['pending_approval_projects']) && !empty($variables['pending_approval_projects'])) { ?>
    <h3 class="sub-header themed-color-purple strong">Pending Approval</h3>
    <div class="row">
      <?php foreach ($variables['pending_approval_projects'] as $project) { ?>      
        <div class="col-sm-12 col-md-6 col-lg-6">
          <a href="<?php echo $project['url']; ?>" class="widget widget-hover-effect1 animation-fadeInQuick" >
            <div class="widget-simple" style="background-color: <?php echo $project['color_code']; ?>;">
              <?php echo theme('image', array('path' => $project['logo_uri'], 'attributes' => array('class' => array('widget-image', 'img-circle', 'pull-left')))); ?>           
              <h3 class="widget-content widget-content-light">
                <?php echo $project['name'] . ' - <strong>' . $project['abbrev'] . '</strong>'; ?>
                <small><?php echo $project['donor_name'];
            ;
                ?></small>
              </h3>
            </div>
            <div class="widget-extra">
              <div class="row text-center animation-fadeInQuick">
    <?php foreach ($project['indicators'] as $indicator) { ?>      
                  <div class="<?php echo $indicator['class']; ?>">
                    <h3 style="color: <?php echo $project['color_code']; ?>;">
                      <b class="count-to-number" data-to="<?php echo $indicator['actual']; ?>" data-speed="5000"></b><small>/<?php echo $indicator['target']; ?></small><br>
                      <small><?php echo $indicator['label']; ?></small>
                    </h3>
                  </div>
    <?php } ?>
              </div>            
            </div>
          </a>
        </div>
    <?php } ?>      
    </div>
  <?php } ?>

  <?php if ($variables['display_closed'] && isset($variables['closed_projects']) && !empty($variables['closed_projects'])) { ?>
    <h3 class="sub-header themed-color-red strong">Archived</h3>
    <div class="row">
      <?php foreach ($variables['closed_projects'] as $project) { ?>      
        <div class="col-sm-12 col-md-6 col-lg-6">
          <a href="<?php echo $project['url']; ?>" class="widget widget-hover-effect1 animation-fadeInQuick" >
            <div class="widget-simple" style="background-color: <?php echo $project['color_code']; ?>;">
                <?php echo theme('image', array('path' => $project['logo_uri'], 'attributes' => array('class' => array('widget-image', 'img-circle', 'pull-left')))); ?>           
              <h3 class="widget-content widget-content-light">
              <?php echo $project['name'] . ' - <strong>' . $project['abbrev'] . '</strong>'; ?>
                <small><?php echo $project['donor_name']; ?></small>
              </h3>
            </div>
            <div class="widget-extra">
              <div class="row text-center animation-fadeInQuick">
                <?php foreach ($project['indicators'] as $indicator) { ?>      
                  <div class="<?php echo $indicator['class']; ?>">
                    <h3 style="color: <?php echo $project['color_code']; ?>;">
                      <b class="count-to-number" data-to="<?php echo $indicator['actual']; ?>" data-speed="5000"></b><small>/<?php echo $indicator['target']; ?></small><br>
                      <small><?php echo $indicator['label']; ?></small>
                    </h3>
                  </div>
                <?php } ?>
              </div>
              <div class="progress progress-striped active">
                <div id="progress-bar-wizard" class="progress-bar <?php print $project['progress_class']; ?>" role="progressbar" aria-valuenow="<?php print $project['progress']; ?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php print $project['progress']; ?>%;"></div>
              </div>
            </div>
          </a>
        </div>
      <?php } ?>
    </div>
  <?php } ?>
</div>