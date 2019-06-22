<div class="col-sm-12 padding-0 <?php print $variables['class']; ?>">
  <div class="card-block">
    <div class="card card-new">
      <div class="card-hdr">
        <div class="card-count"><span class=""><?php print $variables['count']; ?></span></div>        
      </div>
      <div class="card-bdy row">
        <div class="col-sm-3 card-info">
          <div class="card-photo">
            <img class="img-thumbnail" src="<?php print $variables['user']['photo_url']; ?>">
          </div>
          <div class="card-info-details">
            <div class="card-user-name"><?php print $variables['user']['name']; ?></div>
            <div class="card-user-title"><?php print $variables['user']['title']; ?></div>          
          </div>
          <div class="card-info-datetime">
            <div class="card-date"><?php print MED_FA_CALENDAR_PURPLE; ?> <?php print $variables['created_date']; ?></div>
            <div class="card-time"><?php print MED_FA_CLOCK_PURPLE; ?> <?php print $variables['created_time']; ?></div>
            <div class="card-duration"><?php print MED_FA_HOURGLASS; ?> <?php print $variables['duration']; ?></div>
          </div>
        </div>  
        <div class="col-sm-9 card-content">
          <div class="card-content-description"><?php print $variables['description']; ?></div>          
        </div>        
      </div>
    </div> 
  </div> 
</div>