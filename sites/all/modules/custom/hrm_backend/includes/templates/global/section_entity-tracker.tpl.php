<?php if (isset($variables['buttons']) && !empty($variables['buttons'])) { ?>
  <div class="table-options clearfix">
    <div class="btn-group-sm pull-right">
      <?php echo implode(' ', $variables['buttons']); ?>
    </div>
  </div>                    
<?php } ?>

<div class="entity-tracker-wrapper <?php echo (isset($variables['entity_class']) ? implode(' ', $variables['entity_class']) : ''); ?> clearfix widget">
  <div class="entity-tracker-title widget-extra themed-background-dark">
    <h3 class="strong themed-color-white">
      <?php echo $variables['title']; ?> <small class="strong">Tracker</small>
    </h3>
  </div>

  <section id="cd-timeline" class="cd-container">
    <?php foreach ($variables['tracker_items'] as $tracker_item) { ?>
      <div class="cd-timeline-block">
        <div class="cd-timeline-img cd-picture">
          <?php echo theme('image', array('path' => $tracker_item['photo_uri'], 'attributes' => array('class' => array('img-48', 'img-circle')))); ?>
        </div> <!-- cd-timeline-img -->
        <div class="cd-timeline-content">
          <span class="cd-name"><?php echo $tracker_item['name']; ?></span>
          <span class="cd-date"><?php echo $tracker_item['created_formatted']; ?></span>
          <h4 class="tracker-title"><strong><?php echo $tracker_item['title']; ?></strong></h4>
          <?php if (isset($tracker_item['photos']) && !empty($tracker_item['photos'])) { ?>
            <div class="gallery custom-gallery entity-tracker-photos" data-toggle="lightbox-gallery">
              <?php foreach ($tracker_item['photos'] as $photo) { ?>                              
                <a href="<?php echo $photo['url'] ?>" class="gallery-link" title="<?php echo $photo['caption'] ?>">
                  <img class="lazy" data-src="<?php print image_style_url(MED_IMAGE_STYLE_150_120, $photo['uri']) ?>" alt="<?php print $photo['name']; ?>" />                      
                </a>
              <?php } ?>
            </div>
          <?php } ?>
          <?php if (isset($tracker_item['attachments']) && !empty($tracker_item['attachments'])) { ?>
            <div class="tracker-attachments-wrapper">
              <div class="tracker-attachments-title"><strong><?php print MED_FA_PAPERCLIP; ?> Attachments:</strong></div>
              <ul class="tracker-attachments">                          
                <?php foreach ($tracker_item['attachments'] as $attachment) { ?>                              
                  <li><?php echo $attachment['download_link']; ?></li>
                <?php } ?>                          
              </ul>
            </div>
          <?php } ?>
          <?php if (isset($tracker_item['description_formatted'])) { ?>
            <div class="tracker-description-wrapper">
              <?php echo $tracker_item['description_formatted']; ?>
            </div>
          <?php } ?>     
          <?php if (isset($tracker_item['controls']) && !empty($tracker_item['controls'])) { ?>            
            <div class="btn-group-sm">
              <?php echo implode(' ', $tracker_item['controls']); ?>
            </div>            
          <?php } ?>
        </div>
      </div>
    <?php } ?>
  </section>
</div>

