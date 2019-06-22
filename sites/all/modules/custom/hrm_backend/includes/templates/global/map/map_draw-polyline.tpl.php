<?php
$center = $variables['center'];
$canvas_id = (isset($variables['canvas_id']) ? $variables['canvas_id'] : 'map-canvas-view');

$zoom = (isset($variables['zoom']) ? $variables['zoom'] : 6);
$markers = $variables['markers'];
$points = $variables['points'];
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {     
      }
    };
    $(document).ready(function () {
      var map = new google.maps.Map(document.getElementById('<?php echo $canvas_id; ?>'), {
          zoom: <?php echo $zoom; ?>,
          center: {lat: <?php echo $center['lat']; ?>, lng: <?php echo $center['lng']; ?>},
          mapTypeId: google.maps.MapTypeId.HYBRID
        });



        <?php foreach ($markers as $marker) { ?>
          var content = '<?php echo $marker['content']; ?>';
          var infowindow = new google.maps.InfoWindow();         
          var marker = new google.maps.Marker({
            position: {lat: <?php echo $marker['lat']; ?>, lng: <?php echo $marker['lng']; ?>},
            map: map,
            icon: '<?php echo $marker['icon']; ?>'
          });
          
         google.maps.event.addListener(marker,'click', (function(marker,content,infowindow){ 
        return function() {
           infowindow.setContent(content);
           infowindow.open(map,marker);
        };
    })(marker,content,infowindow));      
      <?php } ?>
        
        var polylinePoints = [];
        <?php foreach ($points as $point) { ?>
          var point = {lat:<?php echo $point['lat']; ?>, lng:<?php echo $point['lng']; ?>};
          polylinePoints.push(point);
        <?php } ?>
        
         var pointsPath = new google.maps.Polyline({
          path: polylinePoints,
          geodesic: true,
          strokeColor: '#FF0000',
          strokeOpacity: 1.0,
          strokeWeight: 2
        });
        pointsPath.setMap(map);
    });
  })(jQuery, Drupal, this, this.document);
</script>
<div id="<?php echo $canvas_id; ?>" class="map-canvas" style="min-height: 400px"></div>

