<?php
global $base_url;
?>

<div id="header-user-account-block" class="header-user-account-block">
  <ul class="nav navbar-nav-custom pull-right">
    <!-- User Dropdown -->
    <li class="dropdown">
      <a href="javascript:void(0)" class="dropdown-toggle trans-color" data-toggle="dropdown" aria-expanded="false">
        <?php echo $variables['photo']; ?>
      </a>
      <ul class="dropdown-menu dropdown-custom dropdown-menu-right dropdown-header-account">
        <li class="account-info-li">
          <a href="<?php echo $base_url; ?>/account">
            <div class="dropdown-header-account-photo img-widget-48">
              <?php echo $variables['photo']; ?>
            </div>
            <span class="dropdown-header-account-name"><?php echo $variables['name']; ?><br /><?php echo $variables['title']; ?></span>
          </a>
        </li>
        <li class="account-settings-li clearfix">
          <a href="<?php echo $base_url; ?>/account" 
             class="account-settings-li-my-account pull-left  trans-color">
            My Account
          </a>           
          <a href="<?php echo $base_url; ?>/user/logout" 
             class="account-settings-li-sign-out pull-right trans-color themed-color-red">
            Sign Out
          </a> 
        </li>
      </ul>
    </li>
    <!-- END User Dropdown -->
  </ul>
</div>
