<?php
/**
 * @file
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or
 *   'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see bootstrap_preprocess_html()
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 *
 * @ingroup themeable
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>
  <head profile="<?php print $grddl_profile; ?>">
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
    <?php print $styles; ?>
    <!-- HTML5 element support for IE6-8 -->
    <!--[if lt IE 9]>
      <script src="//html5shiv.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
    <?php print $scripts; ?>
    <script src="https://maps.googleapis.com/maps/api/js?key=<?php print variable_get(MED_VAR_ADMIN_API_GOOGLE_MAP_API_KEY, ''); ?>&v=3.exp"></script> 
  </head>
  <body class="<?php print $classes; ?>" <?php print $attributes; ?> >
    <script>
      (function ($, Drupal, window, document, undefined) {
        var page, pageContent, header, footer;

        $(window).load(function () {
          $(".progress-loader").attr("aria-hidden", "true");
        });

        $(document).ajaxStart(function () {
          $(".progress-loader").attr("aria-hidden", "false");
        });
        $(document).ajaxStop(function () {
          $(".progress-loader").attr("aria-hidden", "true");
        });
      })(jQuery, Drupal, this, this.document);

    </script>
    <style>
      .progress-loader{
        position: fixed;
        width: 100%;
        z-index: 1900;
        pointer-events: none;
        top: 0;
        transition: transform 150ms cubic-bezier(0.4,0.0,1,1);
      }
      .progress-loader[aria-hidden="true"] {
        transition: transform 150ms cubic-bezier(0.4,0.0,1,1),visibility 0ms linear 150ms;
        visibility: hidden;
        transform: translateY(-100%);
      }

      .progress-loader-bar{
        background: #c6dafc;
        width: 100%;
        height: 5px;
        position: relative;
        overflow: hidden;    
      }
      .progress-loader-bar .progress-loader-bar-a{
        background: #4285f4;
        position: absolute;
        left: 0;
        right: 0;
        top: 0;
        bottom: 0;
        transform-origin: left;
        animation: sliderAAnimation 4s infinite linear;
      }
      .progress-loader-bar .progress-loader-bar-b{
        background: #4285f4;
        position: absolute;
        left: 0;
        right: 0;
        top: 0;
        bottom: 0;
        transform-origin: left;
        animation: sliderBAnimation 4s infinite linear;

      }

      @keyframes sliderAAnimation {
        0% {
          transform: translateX(-20%) scaleX(.2)
        }
        75% {
          transform: translateX(100%) scaleX(.90)
        }
        to {
          transform: translateX(100%) scaleX(.90)
        }
      }

      @keyframes sliderBAnimation {
        0% {
          transform: translateX(0) scaleX(0)
        }
        65% {
          transform: translateX(-90%) scaleX(.90)
        }
        95% {
          transform: translateX(120%) scaleX(.2)
        }
        to {
          transform: translateX(100%) scaleX(.2)
        }
      }
    </style>
    <div class="progress-loader">
      <div class="progress-loader-bar">
        <div class="progress-loader-bar-a"></div>
        <div class="progress-loader-bar-b"></div>
      </div>
    </div>
    <div id="skip-link">
      <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
    </div>
    <?php print $page_top; ?>
    <?php print $page; ?>
    <?php print $page_bottom; ?>
  </body>
</html>
