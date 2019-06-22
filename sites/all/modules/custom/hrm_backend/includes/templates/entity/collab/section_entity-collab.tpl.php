<div class="clearfix">
  <div class="collab-wrapper <?php echo (isset($variables['entity_class']) ? implode(' ', $variables['entity_class']) : ''); ?> 
       col-lg-8 col-lg-offset-2 col-md-10 col-md-offset-1 col-sm-12 ">
    <div class="collab-content">
      <?php if (isset($variables['posts']) && !empty($variables['posts'])) { ?>
        <div class="collab-content-scroll-wrapper slimscroll" data-height="350px" >
          <ul class="collab-posts">
            <?php foreach ($variables['posts'] as $post) { ?>
              <?php print theme('section_entity_collab_post', array('post' => $post)); ?>
            <?php } ?>
            <li class="callab-new-post-place"></li>
          </ul>  
        </div>
        <?php
      }
      else {
        ?>
        <div class="collab-empty-message">
          <p>There are no Posts yet, be the first that post here</p>
        </div>
      <?php } ?>
    </div>
    <?php if (isset($variables['add_post_form'])) { ?>
      <div class="collab-post-loggedin-account clearfix">
        <?php print $variables['account_photo']; ?>
        <h6 class="">Make Post</h6>   
      </div>
      <div class="collab-post-form">
        <?php print drupal_render($variables['add_post_form']); ?>
      </div>
    <?php } ?>
  </div>
</div>

