<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
      $(".page-success-stories-wrapper").imagesLoaded(function () {
        $('.grid').masonry({
          itemSelector: '.grid-item', // use a separate class for itemSelector, other than .col-
          columnWidth: '.grid-sizer',
          percentPosition: true
        });
      });
    });
  })
          (jQuery, Drupal, this, this.document);
</script>
<?php
$data = $variables['data'];

global $base_url;
?>
<div id="page-success-stories-wrapper" class="page-success-stories-wrapper page-wrapper row">
  <div class="clearfix">
    <ul class="custom-dropdown custom-dropdown-filters pull-right margin-right-15 margin-bottom-10">          
            <li class="">
              <a href="javascript:void(0)" class="custom-dropdown-btn btn btn-primary btn-sm">
                <?php print MED_FA_FILTER; ?> FILTERS <?php print strtr(MED_FA_ANGEL_DOWN, array('extra_class' => 'custom-dropdown-btn-icon')); ?>
                <?php if (isset($variables['filters_count']) && $variables['filters_count']) { ?>
                  <span class="label label-danger label-indicator animation-floating"><?php echo $variables['filters_count']; ?></span>
                <?php } ?>
              </a>                         
              <ul class="custom-dropdown-content custom-dropdown-right">
                <li class="custom-dropdown-header text-center bold upper-case">Search Filters...</li>
                <li class="custom-dropdown-inner">
                  <?php echo drupal_render($variables['filter_form']); ?>
                </li>
              </ul>
            </li>          
          </ul>
  </div>
  <?php if (empty($data)) { ?>
    <div id="error-container">
      <div class="row">
        <div class="col-sm-8 col-sm-offset-2 text-center">
          <h1><?php print strtr(MED_FA_FILE_BLUE, array('extra_class' => 'animation-bounce')); ?> NO SUCCESS STORIES</h1>
          <h2 class="h3">We are sorry there is no published story at this time..</h2>
        </div>
      </div>
    </div>
  <?php } else { ?>
    <div class="container-fluid padding-0">         
      <div class="grid">
  <?php foreach ($data as $row) { ?>
          <div class="grid-item grid-sizer col-md-6 animation-fadeInQuick">
            <!-- Advanced Animated Gallery Widget -->
            <div class="widget">
              <div class="widget-advanced widget-advanced-alt">
                <!-- Widget Header -->
                <div class="widget-header text-left">
                  <!-- For best results use an image with at least 150 pixels in height (with the width relative to how big your widget will be!) - Here I'm using a 1200x150 pixels image -->
                  <img src="<?php echo $row['caver_photo']; ?>" alt="<?php echo $row['title']; ?>" class="widget-background animation-pulseSlow widget-background-responsive-width-100">
                  <h3 class="widget-content widget-content-image widget-content-light clearfix">
                    <?php if (!empty($row['photos_gallery'])) { ?>                
                      <a href="javascript:void(0)" class="widget-icon pull-right">
                        <?php echo MED_FA_IMAGE; ?>
                      </a>
                    <?php } ?>
                    <a href="<?php echo $base_url . '/' . $row['url']; ?>" class="themed-color-default"><strong><?php echo $row['title']; ?></strong></a><br>
                    <small>
                      <?php print MED_FA_CALENDAR; ?><?php echo date(MED_DATE_LONG, $row['publish_date']); ?>                       
                      <?php echo ($row['testimonies_count'] > 0 ? MED_FA_QUOTE_LEFT . $row['testimonies_count'] : ''); ?>
                      <?php print MED_FA_MAP_MARKER; ?><strong><?php echo $row['office']; ?></strong>
                      <?php print MED_FA_PROJECT_DIAGRAM; ?><strong><?php echo $row['project']; ?></strong>
                    </small>
                  </h3>
                </div>
                <!-- END Widget Header -->

                <!-- Widget Main -->
                <div class="widget-main">
                  <?php if ($row['story_teaser'] != '') { ?>    
                    <?php echo $row['story_teaser']; ?>
                  <?php } ?>
                    <?php if ($row['testimony_teaser'] != '') { ?>
                    <blockquote style="font-size: 100%;">                  
                    <?php echo $row['testimony_teaser']; ?>                  
                    </blockquote>                
                  <?php } ?>

    <?php if (!empty($row['photos_gallery'])) { ?>
                    <div class="gallery gallery-widget clearfix" data-toggle="lightbox-gallery" style="margin-left: -5px; margin-right: -5px;">
      <?php foreach ($row['photos_gallery'] as $photo) { ?>
                          <div class="col-xs-6 col-sm-3 gallery-image-col">
                            <a href="<?php echo $photo['url']; ?>" class="gallery-link" title="<?php echo $photo['caption']; ?>">
                              <img src="<?php echo $photo['thumbnail_url']; ?>" alt="<?php echo $photo['caption']; ?>">
                            </a>
                          </div>
      <?php } ?>
                    </div>
    <?php } ?>
                </div>
                <!-- END Widget Main -->
              </div>
            </div>
            <!-- END Advanced Animated Gallery Widget -->
          </div>
    <?php } ?>
      </div></div>
<?php } ?>
</div>