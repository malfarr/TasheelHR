<?php
$entity_class = (isset($variables['entity_class']) ? implode(' ', $variables['entity_class']) : '');
$photos = $variables['photos'];
?>

<div class="entity-photos-wrapper <?php echo $entity_class; ?> clearfix">
  <div class="gallery custom-gallery" data-toggle="lightbox-gallery">
    <div class="row original-row">
      <?php foreach ($photos as $photo) { ?>
        <div class="col-lg-3 col-md-3 col-sm-4 col-xs-6 gallery-image">
          <img class="lazy" data-src="<?php print image_style_url(MED_IMAGE_STYLE_300_200, $photo['uri'])?>" alt="<?php print $photo['user_name'] . ' - ' . $photo['caption']; ?>" />          
          <div class="gallery-image-options text-center">            
            <div class="btn-group btn-group-sm">
              <a href="<?php echo $photo['url'] ?>" class="gallery-link btn btn-sm btn-primary" title="<?php echo $photo['user_name'] . ' - ' . $photo['caption'] ?>">View</a>
              <?php if (isset($photo['links'])) { ?>
                <?php foreach ($photo['links'] as $link) { ?>
                  <?php if (isset($link['link'])) { ?>
                    <?php echo $link['link']; ?>
                  <?php }
                  else { ?>
                    <a 
                      href="<?php echo $link['url'] ?>" 
                      class="btn btn-xs <?php echo (isset($link['class']) ? implode(' ', $link['class']) : ''); ?>" 
                      title="<?php echo $link['title'] ?>" 
                      <?php echo (isset($link['download']) ? $link['download'] : ''); ?>
                        <?php echo (isset($link['target']) ? 'target="' . $link['target'] . '"' : ''); ?> >
                    <?php echo $link['text'] ?>
                    </a> 
                  <?php } ?>
                <?php } ?>              
              <?php } ?>              
            </div>            
          </div>
          <?php if (isset($photo['tags'])) { ?>
            <?php echo $photo['tags'] ?>
        <?php } ?>
        </div>
      <?php } ?>
    </div>
  </div>
</div>