<?php
$videos = $variables['videos'];
$preload = (isset($variables['preload']) ? $variables['preload'] : 'metadata');
?>


<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_backend = {
      attach: function (context, settings) {
        $('ul.nav-tabs li a').click(function (event) {
          $('.grid').masonry({
            itemSelector: '.grid-item', // use a separate class for itemSelector, other than .col-
            columnWidth: '.grid-sizer',
            percentPosition: true
          });
        });

      }
    };
    $(document).ready(function () {
      $("body").imagesLoaded(function () {
        $('.grid').masonry({
          itemSelector: '.grid-item', // use a separate class for itemSelector, other than .col-
          columnWidth: '.grid-sizer',
          percentPosition: true
        });
      });      
    });
  })(jQuery, Drupal, this, this.document);
</script>


<div class="videos-gallery-wrapper row grid">
  <?php foreach ($videos as $video) { ?>
    <div class="grid-item grid-sizer col-sm-6" style="margin-bottom: 20px;">
      <div class="video-wrapper clearfix">
        <div class="video-outer">
          <div class="video-inner">
            <video width="100%" controls preload="<?php echo $preload; ?>"
                   <?php echo (isset($video['thumbnail']) ? 'poster="' . $video['thumbnail'] . '"' : ''); ?>
                   >
              <source src="<?php echo $video['url']; ?>" type="video/mp4">
              Your browser does not smedort HTML5 video.
            </video>
          </div>        
        </div>
        <div class="video-title push"><?php echo $video['title']; ?></div>
        <div class="video-description border-bottom push-bit"><?php echo $video['description']; ?></div>
        <div class="clearfix">
          <div class="col-sm-6 remove-padding">
            <div class="video-info content-float clearfix">          
              <div class="author-photo pull-left margin-0-5"><?php echo $video['author_photo']; ?></div>
              <div class="author-name bold"><?php echo $video['author_name']; ?></div>
              <div class="author-title"><?php echo $video['author_title']; ?></div>
            </div>
          </div>
          <div class="col-sm-6 content-float clearfix remove-padding text-right">
            <div class="video-published remove-margin"><?php echo $video['published']; ?></div>
            <div class="video-controls"><div class="btn-group btn-group-sm"><?php echo (isset($video['controls']) ? implode('', $video['controls']) : ''); ?></div></div>
          </div>
        </div>

      </div>
    </div>
  <?php } ?>
</div>