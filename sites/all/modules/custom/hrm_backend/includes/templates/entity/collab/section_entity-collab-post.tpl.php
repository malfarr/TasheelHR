<?php $post = $variables['post']; ?>

<li id="collab-post-entry-<?php print $post['pid']; ?>" class="collab-post-entry <?php print implode(' ', $post['class']); ?>">
  <div class="collab-post-entry-header">
    <span class="collab-post-user-photo"><?php print $post['user_photo']; ?></span>
    <span class="collab-post-user-name"><?php print $post['user_name']; ?></span>
    <span class="collab-post-date"><?php print $post['date']; ?></span>          
  </div>
  <div class="collab-post-entry-body">
    <?php if (isset($post['links']) && !empty($post['links'])) { ?>    
      <div class="collab-post-entry-options options-menu">
        <div class="dropdown">
          <a href="#" class="dropdown-toggle options-menu-btn" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="true">            
            <i class="material-icons">more_vert</i>
          </a> 
          <ul class="dropdown-menu dropdown-menu-right"> 
            <?php foreach ($post['links'] as $link) { ?>       
              <li>
                <?php print $link; ?>
              </li> 
            <?php } ?>
          </ul> 
        </div>
      </div>
    <?php } ?>
    <div class="collab-post-entry-content">
      <?php print $post['content']; ?>
    </div>

    <?php if (isset($post['pictures']) && !empty($post['pictures'])) { ?>
      <div class="collab-post-entry-pictures">
        <div class="gallery custom-gallery" data-toggle="lightbox-gallery">
          <?php foreach ($post['pictures'] as $picture) { ?>                              
            <a href="<?php echo $picture['url'] ?>" class="gallery-link" title="<?php echo $picture['name'] ?>">
              <?php echo theme('image_style', array('style_name' => MED_IMAGE_STYLE_150_150, 'path' => $picture['uri'], 'attributes' => array('alt' => $picture['name']))); ?>                                                        
            </a>
          <?php } ?>
        </div>
      </div>            
    <?php } ?>

    <?php if (isset($post['files']) && !empty($post['files'])) { ?>
      <div class="collab-post-entry-files">
        <?php print theme('item_list', array('title' => 'Attachments', 'type' => 'ol', 'items' => $post['files'])); ?>
      </div>
    <?php } ?>
  </div>
</li>