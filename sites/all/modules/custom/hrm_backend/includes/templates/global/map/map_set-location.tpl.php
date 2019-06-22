<?php
global $base_url;

$hidden_id = $variables['hidden_id'];
$title = $variables['title'];
$center = $variables['center'];

if (isset($variables['marker']) && !empty($variables['marker'])) {
  $center = $variables['marker'];
}

$icon_blue = MED_MARKER_BLUE;
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
      $('#map-canvas').gmap({
        'zoom': 16,
        'center': '<?php echo $center; ?>',
        'scrollwheel': true,
        'zoomControl': true,
        'navigationControl': false,
        'streetViewControl': false,
        'mapTypeId': google.maps.MapTypeId.HYBRID,        
      }).bind('init', function (event, map) {
        $(map).click(function (event) {
          $('#map-canvas').gmap('clear', 'markers');
          $('#map-canvas').gmap('addMarker', {
            'position': event.latLng,
            'draggable': false,
            'bounds': false,
            'animation': google.maps.Animation.DROP,
            'icon': '<?php echo $icon_blue; ?>'
          });

          $("#<?php echo $hidden_id; ?>").val(event.latLng);
          var coordinates = event.latLng.toString();
          coordinates = coordinates.replace("(", "");
          coordinates = coordinates.replace(")", "");
          coordinates = coordinates.replace(" ", "");
          $("#edit-map-coordinates").val(coordinates);
        });
      });
      <?php if (isset($variables['marker']) && !empty($variables['marker'])) { ?>
        var marker_coordinates = '<?php echo $variables['marker']; ?>';
        $('#map-canvas').gmap('addMarker', {
          'position': marker_coordinates,
          'draggable': false,
          'bounds': false,
          'animation': google.maps.Animation.DROP,
          'icon': '<?php echo $icon_blue; ?>'
        });
        $("#edit-map-coordinates").val(marker_coordinates);
      <?php } ?>

      if ($('#edit-map-coordinates').val() === '' && $('#<?php echo $hidden_id; ?>').val() !== '') {
        var coordinates = $('#<?php echo $hidden_id; ?>').val();
        coordinates = coordinates.replace("(", "");
        coordinates = coordinates.replace(")", "");
        coordinates = coordinates.replace(" ", "");
        $("#edit-map-coordinates").val(coordinates);

        $('#map-canvas').gmap('addMarker', {
          'position': coordinates,
          'draggable': false,
          'bounds': false,
          'animation': google.maps.Animation.DROP,
          'icon': '<?php echo $icon_blue; ?>'
        });
      }

      $(".map-coordinates-button").click(function (event) {
        var coordinates_array = $("#edit-map-coordinates").val().split(',');
        if (coordinates_array.length != 2) {
          $(".form-item-map-coordinates .help-block").remove();
          $(".form-item-map-coordinates").addClass("has-error").append('<div for="edit-map-coordinates" class="help-block animation-slideDown">Invalid coordinates.</div>');
        } else if (!inrange(-90, coordinates_array[0], 90) || !inrange(-180, coordinates_array[1], 180)) {
          $(".form-item-map-coordinates .help-block").remove();
          $(".form-item-map-coordinates").addClass("has-error").append('<div for="edit-map-coordinates" class="help-block animation-slideDown">Invalid coordinates.</div>');
        } else {
          var location = coordinates_array[0] + ", " + coordinates_array[1];
          $('#map-canvas').gmap('clear', 'markers');
          $('#map-canvas').gmap('addMarker', {
            'position': location,
            'draggable': false,
            'bounds': true,
            'animation': google.maps.Animation.DROP,
            'icon': '<?php echo $icon_blue; ?>',
          });


          $("#<?php echo $hidden_id; ?>").val("(" + location + ")");
        }
      });

      function inrange(min, number, max) {
        if (!isNaN(number) && (number >= min) && (number <= max)) {
          return true;
        } else {
          return false;
        }
      }
    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php if (isset($variables['form_item']) && $variables['form_item']) { ?>
  <div class="form-item form-group">
  <?php } ?>
  <div id="set-location-wrapper">
    <label for="edit-map-coordinates" class="block-label"><?php echo $title; ?> <span class="form-required" title="This field is required."></span></label>
    <div class="form-type-textfield form-item-map-coordinates padding-0 clearfix">
      <div id="map-wrapper"> 
        <div class="map-coordinates-wrapper">
          <input class="map-coordinates-text form-control form-text required pull-left" type="text" id="edit-map-coordinates" name="map_coordinates" placeholder="Coordinates...">
          <span class="map-coordinates-button btn btn-primary" id="edit-map-coordinates-button"><?php echo MED_FA_LOCATION_ARROW; ?></span>
        </div>
      </div>
      <div id="map-canvas"></div>
      <span class="description-block">Click location on the map to add its marker.</span>   
    </div>
  </div>
  <?php if (isset($variables['form_item']) && $variables['form_item']) { ?>
  </div>
<?php } ?>