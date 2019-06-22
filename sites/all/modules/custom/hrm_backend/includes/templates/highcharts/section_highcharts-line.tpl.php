<?php
$container_id = $variables['container_id'];
$container_classes = implode(' ', $variables['container_classes']);

$chart_data = $variables['chart_data'];
?>

<script>
  (function ($) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('#<?php echo $container_id; ?>').highcharts({
          credits: {
            enabled: false
          },
          title: {
            text: '<?php echo (isset($chart_data['title']) ? $chart_data['title'] : ''); ?>',
            x: -20 //center
          },
          subtitle: {
            text: '<?php echo (isset($chart_data['subtitle']) ? $chart_data['subtitle'] : ''); ?>',
            x: -20
          },
          xAxis: {
            categories: <?php echo json_encode($chart_data['x_categories']); ?>,
          },
          yAxis: {           
            <?php echo (isset($chart_data['y_min']) ? "min:" . $chart_data['y_min'] . "," : ''); ?>
            <?php echo (isset($chart_data['y_max']) ? "max:" . $chart_data['y_max'] . "," : ''); ?>
            <?php echo (isset($chart_data['y_tickInterval']) ? "tickInterval:" . $chart_data['y_tickInterval'] . "," : ''); ?>
            title: {
              text: '<?php echo (isset($chart_data['y_title']) ? $chart_data['y_title'] : ''); ?>'
            },            
            plotLines: [{
                value: 0,
                width: 1,
                color: '#808080'
              }]
          },
          
          tooltip: {            
            formatter: function() {
               return this.point.toolTipContent;
            }
          },
          legend: {
            layout: 'vertical',
            align: 'right',
            verticalAlign: 'middle',
            borderWidth: 0
          },
          exporting: {
            enabled: true
        },
          series: <?php echo json_encode($chart_data['series_data']); ?>,
          
        });
      }
    };
    $(document).ready(function () {
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div class="col-sm-12">
  <div id="<?php echo $container_id; ?>" class="<?php echo $container_classes; ?>" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
</div>