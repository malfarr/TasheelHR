<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
 
<?php
if (!user_is_logged_in()) {
  require_once 'page-anonymous.tpl.php';
  return;
}
?>
<?php
if (arg(0) == 'org-chart' && arg(2) == 'export') {
  require_once 'page-blank.tpl.php';
  return;
}
?>

<div id="page-wrapper" class="<?php echo implode(' ', $variables['page_wrapper_clases']); ?>">
  <?php if (isset($variables['pre_loader_content']) && !empty($variables['pre_loader_content'])): ?>
    <?php print $variables['pre_loader_content']; ?>
  <?php endif; ?>

  <?php if (!empty($page['page_top'])): ?>
    <div id="page-top">  
      <?php print render($page['page_top']); ?>
    </div>  
  <?php endif; ?>
  <div id="page-container" class="sidebar-partial sidebar-visible-lg sidebar-no-animations">
    <div id="sidebar">
      <div id="sidebar-scroll">
        <div class="sidebar-content">
          <?php if ($logo || $site_name): ?>
            <div class="sidebar-brand-wrapper">
              <a class="sidebar-brand" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
                <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="sidebar-brand-logo-image"/>
                <?php if (isset($variables['country_flag']) && !empty($variables['country_flag'])): ?>
                  <?php print $variables['country_flag']; ?>
                <?php endif; ?>
                <span><strong><?php print $site_name; ?></strong></span>
              </a>
            </div>
          <?php endif; ?>        
          <?php if (!empty($page['sidebar'])): ?>
            <?php print render($page['sidebar']); ?>
          <?php endif; ?>
          <?php if (!empty($primary_nav)): ?>
            <?php print render($primary_nav); ?>
          <?php endif; ?>
        </div>
      </div>
    </div><!-- /#sidebar -->

    <div id="main-container">
      <header class="navbar navbar-default page-header-top">
        <div class="toggle-sidebar-wrapper pull-left">
          <a href="javascript:void(0)" onclick="App.sidebar('toggle-sidebar');
              this.blur();" class="trans-color">
            <aside></aside>
          </a>
        </div>        
        <div class="page-top-title pull-left">
          <?php print render($title_prefix); ?>
          <?php if (!empty($title)): ?>
            <?php print $title; ?>
          <?php endif; ?>
          <?php print render($title_suffix); ?> 
        </div>
        <?php if (!empty($page['header_left'])): ?>
          <div id="header-left" class="pull-left">  
            <?php print render($page['header_left']); ?>
          </div>  
        <?php endif; ?>
        <?php if (!empty($page['header_right'])): ?>
          <div id="header_right" class="pull-right">  
            <?php print render($page['header_right']); ?>
          </div>  
        <?php endif; ?>
        <?php if (!empty($secondary_nav)): ?>
          <?php print render($secondary_nav); ?>
        <?php endif; ?>
      </header>
      <div id="page-content">
        <section id="content" class=" ">
          <?php if (!empty($page['content_top'])): ?>
            <div id="content-top" class="col-sm-12">  
              <?php print render($page['content_top']); ?>
            </div>  
          <?php endif; ?>

          <?php print $messages; ?>

          <?php if (!empty($tabs)): ?>
            <?php print render($tabs); ?>
          <?php endif; ?>
          <?php if (!empty($action_links)): ?>
            <ul class="action-links"><?php print render($action_links); ?></ul>
          <?php endif; ?>
          <?php print render($page['content']); ?>
        </section>                                                                                 
      </div><!-- /#page-content -->

      <footer class="clearfix">
        <?php print render($page['footer']); ?>
      </footer>
    </div><!-- /#main-container -->        
  </div><!-- /#page-container -->
</div><!-- /#page-wrapper -->
<a href="#" id="to-top" style="display: block;"><?php print MED_FA_ANGEL_DOUBLE_UP; ?></a>
<!-- /#Change content -->
<?php if (isset($changelog_content)): ?>
  <?php print $changelog_content; ?>
<?php endif; ?>
