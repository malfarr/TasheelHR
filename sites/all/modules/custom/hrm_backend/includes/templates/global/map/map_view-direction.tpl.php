<?php
global $base_url;

$origin = $variables['origin'];
$destination = $variables['destination'];
?>
<style>
  #map-canvas-direction{
    height: 400px;
  }
  #map-canvas-direction-panel{
    height: 400px;
    overflow-y: scroll;
    padding-right: 10px;
  }
  .adp, .adp table{
    width: 100%;

  }
  .adp table{
    margin: 0 0 10px 0;
  }
  .adp-step, .adp-substep{
    padding: 10px 3px;
  }
  .adp-summary {
    padding: 5px;
    font-weight: bolder;
    font-size: 16px;
  }
</style>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
    
      }
    };
    $(document).ready(function () {
          $('#map-canvas-direction').gmap({
          'zoom': 16,
          'center': '<?php echo $origin; ?>',             
        }).bind('init', function (evt, map) {
          $('#map-canvas-direction').gmap('displayDirections',
                  {
                    'origin': '<?php echo $origin; ?>',
                    'destination': '<?php echo $destination; ?>',
                    'travelMode': google.maps.DirectionsTravelMode.DRIVING,
                    'durationInTraffic': true,
                    'provideRouteAlternatives': true,
                  },
                  {
                    'panel': document.getElementById('map-canvas-direction-panel')
                  },
          function (result, status) {
            if (status !== 'OK') {
              $('#map-canvas-direction-panel').html("<h3>Direction not found</h3>");
            }
          }
          );
        });
    });
  })(jQuery, Drupal, this, this.document);
</script>
<div class="row">
  <div class="col-8 col-sm-8 col-xs-12">
    <div id="map-canvas-direction" class="map-canvas-direction"></div>
  </div>
  <div class="col-4 col-sm-4 col-xs-12">
    <div id="map-canvas-direction-panel"></div>
  </div>
</div>
