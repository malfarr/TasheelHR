<script>
  (function ($) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
      $(".assessment-summary-chart-graph").removeClass("active");
      setTimeout(function () {
        $(".assessment-summary-chart-graph").addClass("active");
      }, 150);


      $("#btn-tab-assessment").click(function () {
        $(".assessment-summary-chart-graph").removeClass("active");
        setTimeout(function () {
          $(".assessment-summary-chart-graph").addClass("active");
        }, 150);
      });
      $(".trigger").click(function () {
        $(".assessment-summary-chart-graph").toggleClass("active");
      });

      $(".active-item .assessment-summary-chart-item-icon").mouseover(function () {
        $(".table-responsive").css("overflow", "visible");
        var indicator = $(this).data('indicator');
        $('.vaf-row-' + indicator).addClass('vaf-row-selected');
      });

      $(".active-item .assessment-summary-chart-item-icon").mouseleave(function () {
        $(".table-responsive").css("overflow", "auto");
        $('.vaf-row').removeClass('vaf-row-selected');
      });
    });
  })(jQuery, Drupal, this, this.document);
</script>



<div class="assessment-summary clearfix">
  <div class="assessment-summary-chart-wrapper col-lg-4  col-md-5 col-sm-12">

    <div class="assessment-summary-chart clearfix">
      <div class="assessment-summary-chart-graph">
        <div class="assessment-summary-chart-item assessment-summary-chart-center rotater-wrapper trigger"><?php print MED_FA_USER; ?></div>
        <?php foreach ($variables['vaf'] as $indicator_key => $indicator) { ?>
          <div class="assessment-summary-chart-item assessment-summary-chart-<?php print $indicator_key; ?> assessment-summary-chart-<?php print $indicator['score']; ?> <?php print $indicator['item_class']; ?> rotater">
            <div class="rotater-wrapper rotater-inner">              
              <div class="assessment-summary-chart-item-icon <?php print $indicator['border_class'] ?>" data-indicator="<?php print $indicator_key; ?>">              
                <span class="<?php print $indicator['color_class'] ?>"><?php print ($indicator['score'] ? $indicator['score'] : 'N/A') ?></span>
              </div>
            </div>        
          </div>  
        <?php } ?>
      </div>
    </div>
  </div>
  <div class="assessment-summary-table-wrapper col-lg-8  col-md-7 col-sm-12">
    <?php if (isset($variables['components_list'])) { ?>
      <?php print theme('table', $variables['components_list']); ?>
    <?php } ?>
  </div>
</div>