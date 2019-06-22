<div class="employee-of-quarter-block">
  <div class="employee-name"><?php echo $variables['emp_name']; ?></div>
  <div class="gallery custom-gallery" data-toggle="lightbox-gallery">
    <a href="<?php echo $variables['image_url'] ?>" class="gallery-link" title="<?php echo $variables['image_caption']; ?>">
      <?php echo $variables['image'] ?>
    </a>
  </div>
  <div>    
    <div class="employee-description themed-color-med-orange">EMPLOYEE OF Q<?php echo $variables['quarter']; ?> <?php echo $variables['year']; ?></div>
  </div>
</div>