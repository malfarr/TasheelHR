<script>
  (function ($) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
            
      }
    };
    $(document).ready(function () {
      $('.sidebar-nav a.active').parents('.parent').addClass('active');      
    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php
  $menu_content = $variables['menu_content'];
?>

<?php echo $menu_content; ?>
