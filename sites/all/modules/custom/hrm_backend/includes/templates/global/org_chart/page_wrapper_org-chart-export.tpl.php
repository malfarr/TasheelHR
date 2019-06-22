<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
      var error_message = '<?php echo $variables['error']; ?>';
      var datascource = <?php echo json_encode($variables['datasource']); ?>;
      if (datascource.length !== 0 && error_message === '') {
        $('#chart-container').orgchart({
          'data': datascource,
          'nodeContent': 'content',
          'exportButton': true,
          'exportFilename': '<?php echo $variables['export_file_name']; ?>',
          'pan': true,
          'zoom': true
        });
      }
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div id="page-org-chart" class="page-wrapper page-org-chart">
  <div id="org-chart-block" class="block">    
      <div class="block-title clearfix">
        <h2><?php echo $variables['title']; ?></h2>
      </div>
      <div class="block-content push">           
        <div id="chart-container" class="med-org-chart">
          <?php if ($variables['error'] != '') { ?>
            <div class="alert alert-block alert-danger margin-20">
              <h4 class="element-invisible">Error message</h4>
              <?php echo $variables['error']; ?>
            </div>    
          <?php } ?>
        </div>          
      </div>   
    </div>   
</div>
