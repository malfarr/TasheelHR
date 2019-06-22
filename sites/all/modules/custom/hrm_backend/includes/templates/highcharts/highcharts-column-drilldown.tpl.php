<script>
  (function ($) {
    Drupal.behaviors.highchart_column_drilldown_behavior = {
      attach: function (context, settings) {

// Create the chart
        var container_chartAtaFleetAvg = new Highcharts.Chart({
          credits: {
            enabled: false
          },
          colors: ['#e74c3c', '#e67e22', '#f1c40f', '#34495e', '#9b59b6', '#3498db', '#1abc9c', '#95a5a6', '#ecf0f1'],
          chart: {
            renderTo: '<?php echo $container_id; ?>',
            type: 'column'
          },
          title: {
            text: '<?php echo $variables['title']; ?>'
          },
          subtitle: {
            text: 'Click the column to drilldown details'
          },
          xAxis: {
            type: 'category',
            title: {
              text: '<?php echo $variables['x_title']; ?>'
            }
          },
          yAxis: {
            title: {
              text: '<?php echo $variables['y_title']; ?>'
            }
          },
          legend: {
            enabled: false
          },
          plotOptions: {
            series: {
              borderWidth: 0,
              dataLabels: {
                enabled: true,
                format: '{point.y:.1f}%'
              }
            }
          },
          tooltip: {
            headerFormat: '<span style="font-size:11px">{series.name}</span><br>',
            pointFormat: '<span style="color:{point.color}">{point.name}</span>: <b>{point.y:.2f}%</b> of total<br/>'
          },
          series: [{
              name: '<?php echo $variables['series_name']; ?>',
              colorByPoint: true,
              data: <?php echo json_encode($variables['series_data']); ?>
            }],
          drilldown: {
            series: <?php echo json_encode($variables['series_drilldown_data']); ?>
          }
        });
      }
    };
    $(document).ready(function () {
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div id="<?php echo $variables['container_id']; ?>" class="<?php echo (isset($variables['container_class']) ? implode(' ', $variables['container_class']) : ''); ?>" style="height: 400px"></div>  