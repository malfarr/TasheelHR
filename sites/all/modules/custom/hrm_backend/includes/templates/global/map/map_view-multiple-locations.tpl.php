<?php
$center = $variables['center'];
$markers = $variables['markers'];
$icon_red = MED_MARKER_RED;

$id = 'map-canvas-view';
if (isset($variables['div_id']) && !empty($variables['div_id'])) {
  $id = $variables['div_id'];
}

$zoom = (isset($variables['zoom']) ? $variables['zoom'] : 16);
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('ul.nav-tabs li a').click(function (event) {
          $('#<?php echo $id; ?>').gmap('refresh');
        });
        $('a.btn-primary-full-screen').click(function (event) {
          var parentHeight = $('#<?php echo $id; ?>').parent().height();
          $('#<?php echo $id; ?>').height(parentHeight);
          $('#<?php echo $id; ?>').gmap('refresh');
        });
        $('fieldset a.fieldset-title').click(function (event) {
          $('#<?php echo $id; ?>').gmap('refresh');
        });
      }
    };
    $(document).ready(function () {
      $('#<?php echo $id; ?>').gmap({
        'zoom': <?php echo $zoom; ?>,
        'center': '<?php echo $center; ?>',
        'scrollwheel': true,
        'zoomControl': true,
        'navigationControl': false,
        'streetViewControl': false,
        'mapTypeId': google.maps.MapTypeId.HYBRID
      });
      <?php foreach ($markers as $marker) { ?>
        <?php if (isset($marker['content'])) { ?>
          $('#<?php echo $id; ?>').gmap('addMarker', {
            'position': '<?php echo $marker['coordinates']; ?>',
            'draggable': false,
            'bounds': false,
            'animation': google.maps.Animation.DROP,
            'icon': '<?php echo (isset($marker['icon']) ? $marker['icon'] : $icon_red); ?>',
            'zIndex': 99999999
          }).click(function () {
            $('#<?php echo $id; ?>').gmap('openInfoWindow', {'content': '<?php echo $marker['content']; ?>'}, this);
          });
        <?php }
        else { ?>
          $('#<?php echo $id; ?>').gmap('addMarker', {
            'position': '<?php echo $marker['coordinates']; ?>',
            'draggable': false,
            'bounds': false,
            'animation': google.maps.Animation.DROP,
            'icon': '<?php echo (isset($marker['icon']) ? $marker['icon'] : $icon_red); ?>',
          });
        <?php } ?>
      <?php } ?>
    });
  })(jQuery, Drupal, this, this.document);
</script>
<div id="<?php echo $id; ?>" class="map-canvas <?php echo (isset($variables['class']) ? implode(' ', $variables['class']) : ''); ?>"></div>

