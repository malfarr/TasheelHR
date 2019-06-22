<script>
  (function ($) {
    Drupal.behaviors.highchart_3d_pie_behavior = {
      attach: function (context, settings) {
        var container_chartAtaFleetAvg = new Highcharts.Chart({
          credits: {
            enabled: false
          },
          colors: ['#1abc9c', '#3498db', '#9b59b6', '#34495e', '#f1c40f', '#e67e22', '#e74c3c', '#ecf0f1', '#95a5a6'],
          chart: {
            renderTo: '<?php echo $container_id; ?>',
            type: 'pie',
            options3d: {
              enabled: true,
              alpha: 45,
              beta: 0
            }
          },
          title: {
            text: '<?php echo $variables['title']; ?>'
          },
          tooltip: {
            pointFormat: '{point.y} - <b>{point.percentage:.1f}%</b>'
          },
          plotOptions: {
            pie: {
              allowPointSelect: true,
              cursor: 'pointer',
              depth: 35,
              dataLabels: {
                enabled: true,
                format: '{point.name} <b>{point.percentage:.1f}%</b>'
              }
            }
          },
          exporting: {
            enabled: true
          },
          series: [{
              type: 'pie',
              data: <?php echo json_encode($variables['series_data']); ?>
            }]
        });
      }
    };
    $(document).ready(function () {
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div id="<?php echo $variables['container_id']; ?>" class="<?php echo (isset($variables['container_class']) ? implode(' ', $variables['container_class']) : ''); ?>" style="height: 400px"></div>  