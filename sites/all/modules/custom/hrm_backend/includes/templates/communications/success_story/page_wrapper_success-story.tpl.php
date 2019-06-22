<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
//       $(window).trigger('resize');
//       alert("hgghh");
    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php
$data = $variables['data'];
?>

<div class="page-success-story-wrapper">
  <!--<div class="widget">-->
  <div class="widget-advanced widget-advanced-alt">
    <div class="widget-header text-left">
      <img src="<?php echo $data['caver_photo']; ?>" alt="<?php echo $data['title']; ?>" class="widget-background animation-pulseSlow widget-background-responsive-width-100">
      <h3 class="widget-content widget-content-image widget-content-light clearfix themed-color-white">
        <strong><?php echo $data['title']; ?></strong><br />
        <small class=" themed-color-white">
          <?php echo MED_FA_CALENDAR; ?> <?php echo date(MED_DATE_LONG, $data['publish_date']); ?>
          <?php echo ($data['testimonies_count'] > 0 ? MED_FA_QUOTE_LEFT . $data['testimonies_count'] : ''); ?>
        </small>
      </h3>
    </div>
  </div>
  <!--</div>-->
  <div class="block block-alt-noborder clearfix">
    <div class="col-md-10 col-md-offset-1 col-lg-10 col-lg-offset-1">
      <article>
        <h3 class="sub-header text-center"><strong><?php echo $data['title']; ?></strong></h3>
        <?php echo check_markup($data['body'], 'full_html'); ?>
        <br />
      </article>
      <?php if (!empty($data['testimonies'])) { ?>
        <!-- Testimonies Block -->
        <!-- Testimonies Content -->
        <?php foreach ($data['testimonies'] as $testimony) { ?>
          <blockquote>
            <?php echo $testimony['testimony']; ?>
            <footer><?php echo $testimony['name']; ?></footer>
          </blockquote>
          <br />
        <?php } ?>
        <!-- END Testimonies Content -->
        <!-- END Testimonies Block -->
      <?php } ?>

      <?php if (!empty($data['photos_gallery'])) { ?>
        <!-- Pictures Block -->      
        <!-- Pictures Content -->
        <div class="row push-top-bottom">
          <div class="gallery gallery-widget clearfix" data-toggle="lightbox-gallery">            
              <?php foreach ($data['photos_gallery'] as $photo) { ?>
                <div class="col-xs-6 col-sm-3 gallery-image-col">
                  <a href="<?php echo $photo['url']; ?>" class="gallery-link" title="<?php echo $photo['caption']; ?>">
                    <img src="<?php echo $photo['thumbnail_url']; ?>" alt="<?php echo $photo['caption']; ?>">
                  </a>
                </div>
              <?php } ?>            
          </div>
        </div>
        <!-- END Pictures Content -->
        <!-- END Pictures Block -->
      <?php } ?> 

      <?php if (!empty($data['videos_gallery'])) { ?>
        <div class="row">
          <?php foreach ($data['videos_gallery'] as $video) { ?>
            <div class="col-sm-12">
              <p class="push-bit">
                <strong><?php echo $video['title']; ?></strong>
              </p>
              <video controls  style="text-align: center; border: 5px solid #faa61a;width: 100%;">
                <source src="<?php echo $video['url']; ?>" type="video/mp4">      
                Your browser does not smedort HTML5 video.
              </video> 
              <p class="push-bit"><?php echo $video['description']; ?></p>
            </div>
          <?php } ?>
        </div>
      <?php } ?>

      <!-- Author and More Row -->
      <div class="row">
        <div class="col-md-6">
          <!-- Author Block -->
          <!-- Author Content -->
          <h3 class="sub-header">About the Author</h3>
          <div class="block-section content-float clearfix">
            <?php echo theme('image_style', array('style_name' => MED_IMAGE_STYLE_150_150, 'path' => $data['author_info']['photo'], 'attributes' => array('class' => array('img-circle', 'img-64', 'pull-left')))); ?>
            <div class="push-bit themed-color-default">
              <strong><?php echo $data['author_info']['name']; ?></strong><br />
            </div>
            <div>
              <?php echo $data['author_info']['title']; ?>
            </div>            
          </div>
          <!-- END Author Content -->
          <!-- END Author Block -->
        </div>
      </div>
      <!-- END Author and More Row -->
    </div>
  </div>
</div>
