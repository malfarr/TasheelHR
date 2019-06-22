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
global $base_url;
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
      $('#form-login').validate({
        errorClass: 'help-block animation-slideDown',
        errorElement: 'div',
        errorPlacement: function (error, e) {
          e.parents('.form-group').append(error);
        },
        highlight: function (e) {
          $(e).closest('.form-group').removeClass('has-success has-error').addClass('has-error');
          $(e).closest('.help-block').remove();
        },
        success: function (e) {
          e.closest('.form-group').removeClass('has-success has-error').addClass('has-success');
          e.closest('.help-block').remove();
        },
        ignore: "",
        rules: {
          name: {
            required: true,
            email: true,
          },
          pass: {
            required: true,
          },
        },
        messages: {
        }
      });
      $('#form-reminder').validate({
        errorClass: 'help-block animation-slideDown',
        errorElement: 'div',
        errorPlacement: function (error, e) {
          e.parents('.form-group').append(error);
        },
        highlight: function (e) {
          $(e).closest('.form-group').removeClass('has-success has-error').addClass('has-error');
          $(e).closest('.help-block').remove();
        },
        success: function (e) {
          e.closest('.form-group').removeClass('has-success has-error').addClass('has-success');
          e.closest('.help-block').remove();
        },
        ignore: "",
        rules: {
          name: {
            required: true,
            email: true,
          },
        },
        messages: {
        }
      });
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php $status = drupal_get_http_header("status"); ?>
<?php if ($status == '403 Forbidden') { ?>
  <div id="error-container">
    <div class="error-options">
      <h3><?php print MED_FA_CHEVRON_CIRCLE_LEFT_GRAY; ?> <?php echo l('Go Back', '') ?></h3>
    </div>
    <div class="row">
      <div class="col-sm-8 col-sm-offset-2 text-center">
        <h1 class="animation-hatch themed-color-red"><?php print MED_FA_TIMES_RED; ?> 403</h1>
        <h2 class="h3">Oops, we are sorry but you do not have permission to access this page..</h2>
      </div>
    </div>
  </div>
  <?php
}
elseif ($status == '404 Not Found') {
  ?>
  <div id="error-container">
    <div class="error-options">
      <h3><?php print MED_FA_CHEVRON_CIRCLE_LEFT_GRAY; ?> <?php echo l('Go Back', '') ?></h3>
    </div>
    <div class="row">
      <div class="col-sm-8 col-sm-offset-2 text-center">
        <h1 class="animation-pulse themed-color-orange"><?php print MED_FA_EXCLAMATION_CIRCLE; ?> 404</h1>
        <h2 class="h3">Oops, we are sorry but the page you are looking for was not found..</h2>
      </div>        
    </div>
  </div>
  <?php
}
else {
  ?>
  <?php
  $backgrounds = variable_get(MED_VAR_ADMIN_INTERFACE_LOGIN_BACKGROUND, array());
  $rand_background = rand(1, count($backgrounds));
  $logo = variable_get(MED_VAR_ADMIN_INTERFACE_LOGIN_LOGO, array());
  $organization_name = variable_get(MED_VAR_ADMIN_IMPACT_ORG_ENGLISH_NAME, '');
  ?>
  <img src="<?php echo $backgrounds[$rand_background]['url']; ?>" alt="<?php echo $organization_name; ?> IMPACT" class="full-bg animation-pulseSlow">    
  <div id="login-container" class="animation-fadeIn anonymous-front">
    <div id="login-container-wrapper">
      <div class="login-title text-center">
        <img src="<?php echo $logo['url']; ?>">
        <h1>IMPACT</h1>
      </div>        
      <div class="block content-block"> 
        <?php
        print $messages;
        print render($page['content']);
        ?>
      </div>
    </div> 
  </div>
<?php }
?>
