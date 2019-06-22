<?php
$title = $variables['title'];
$hidden_id = $variables['hidden_id'];
?>
<script>
  (function ($, Drupal, window, document, undefined) {
      Drupal.behaviors.med_behavior = {
          attach: function (context, settings) {
          }
      };
      $(document).ready(function () {
        $('select[name="colorpicker-longlist"]').simplecolorpicker({theme: 'fontawesome'});            
        $('#<?php echo $hidden_id; ?>').val($('select[name="colorpicker-longlist"]').val());
        $('select[name="colorpicker-longlist"]').on('change', function() {          
          $('#<?php echo $hidden_id; ?>').val($('select[name="colorpicker-longlist"]').val());
        });
        <?php if(isset($variables['default'])) { ?>
        $('select[name="colorpicker-longlist"]').simplecolorpicker('selectColor', '<?php echo $variables['default']; ?>');
        <?php } ?>
        $('#<?php echo $hidden_id; ?>').val($('select[name="colorpicker-longlist"]').val());
      });
  })(jQuery, Drupal, this, this.document);
</script>

<div id="color-field-wrapper">
  <label><?php echo $title; ?></label>
  <div id="map-wrapper">
    <select name="colorpicker-longlist">
      <option value="#ac725e">#ac725e</option>
      <option value="#d06b64">#d06b64</option>
      <option value="#f83a22">#f83a22</option>
      <option value="#fa573c">#fa573c</option>
      <option value="#ff7537">#ff7537</option>
      <option value="#ffad46">#ffad46</option>
      <option value="#42d692">#42d692</option>
      <option value="#16a765">#16a765</option>
      <option value="#7bd148">#7bd148</option>
      <option value="#b3dc6c">#b3dc6c</option>           
      <option value="#92e1c0">#92e1c0</option>
      <option value="#9fe1e7">#9fe1e7</option>
      <option value="#9fc6e7">#9fc6e7</option>
      <option value="#4986e7">#4986e7</option>
      <option value="#9a9cff">#9a9cff</option>
      <option value="#b99aff">#b99aff</option>
      <option value="#c2c2c2">#c2c2c2</option>
      <option value="#cabdbf">#cabdbf</option>
      <option value="#cca6ac">#cca6ac</option>
      <option value="#f691b2">#f691b2</option>
      <option value="#cd74e6">#cd74e6</option>
      <option value="#a47ae2">#a47ae2</option>
      <option value="#292929">#292929</option>
      <optgroup label="----------"></optgroup>
      <option value="#1bbae1">#1bbae1</option>
      <option value="#ff0000">#ff0000</option>
      <option value="#597800">#597800</option>
      <option value="#23716d">#23716d</option>
      <option value="#dc0018">#dc0018</option>
      <option value="#144172">#144172</option>
      <option value="#1d3b85">#1d3b85</option>
      <option value="#026CB6">#026CB6</option>
      <option value="#d86422">#d86422</option>
    </select>
    <span class="help-block"></span>    
  </div>
</div>

