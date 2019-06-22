<?php
global $base_url;

$marker = $variables['marker'];
$center = (isset($variables['center']) ? $variables['center'] : $marker);

$icon = MED_MARKER_BLUE;
if (isset($variables['icon'])) {
  $icon = $variables['icon'];
}
$content = '';
if (isset($variables['content'])) {
  $content = $variables['content'];
}

$id = 'map-canvas-view-' . time() . '-' . med_basic_string_get_random_string();
if(isset($variables['div_id']) && !empty($variables['div_id'])){
  $id = $variables['div_id'];
}
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        $('ul.nav-tabs li a').click(function (event) {
          $('#<?php echo $id; ?>').gmap('refresh');
        });
        $('a.btn-primary-full-screen').click(function (event) {
          $('#<?php echo $id; ?>').gmap('refresh');
        });
        $('fieldset a.fieldset-title').click(function (event) {              
          $('#<?php echo $id; ?>').gmap('refresh');
        });
      }
    };
    $(document).ready(function () {
      center = '<?php echo $center; ?>';
      marker = '<?php echo $marker; ?>';
      icon = '<?php echo $icon; ?>';
      content = '<?php echo $content; ?>';
      $('#<?php echo $id; ?>').gmap({
        'zoom': 16,
        'center': center,
        'scrollwheel': true,
        'zoomControl': true,
        'navigationControl': false,
        'streetViewControl': true,
        'mapTypeId': google.maps.MapTypeId.HYBRID
      });
      $('#<?php echo $id; ?>').gmap('addMarker', {
        'position': marker,
        'draggable': false,
        'bounds': false,
        'animation': google.maps.Animation.DROP,
        'icon': icon
      }).click(function () {
        if (content !== '') {
          $('#<?php echo $id; ?>').gmap('openInfoWindow', {'content': content}, this);
        }
      });
    });
  })(jQuery, Drupal, this, this.document);
</script>
<div id="<?php echo $id; ?>" class="map-canvas map-canvas-400 <?php echo (isset($variables['class']) ? implode(' ', $variables['class']) : ''); ?>"></div>

