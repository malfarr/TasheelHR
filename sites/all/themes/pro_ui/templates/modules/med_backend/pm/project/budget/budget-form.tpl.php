<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.eac_behavior = {
      attach: function (context, settings) {
        $('.total-envelope').on('input', function (e) {
          line_value_changed();
        });
        $('.line-value').on('input', function (e) {
          line_value_changed();
        });     
      }
    };

    $(document).ready(function () {
      $("#page-container").removeClass("sidebar-visible-lg");
      $("#page-container").removeClass("sidebar-partial");
      $("a#to-top").css("visibility", "hidden");
                  
      line_value_changed();
      $('.line-value').on('input', function (e) {
        line_value_changed();
      });
    });

    function line_value_changed() {
      total_balance = 0;
      $('.total-balance').parent().removeClass('has-error');

      $('.line-value').each(function (index) {
        $(this).removeClass('has-error-custom-dashed-bottom');
        line_value = $(this).val().replace(/\,/g, '');
        line_value = +line_value;

        if ($.isNumeric(line_value)) {
          total_balance = total_balance + line_value;
        }
        else {
          $(this).addClass('has-error-custom-dashed-bottom');
        }
      });
      total_balance = $().add_commas(total_balance);
      $('.total-balance').val(total_balance);
      total_budgeted = $().add_commas($('.total-budgeted').val());

      if (total_balance != total_budgeted) {
        $('.total-balance').parent().addClass('has-error');
      }
      else {
        $('.total-balance').parent().removeClass('has-error');
        $('.total-balance').parent().addClass('has-success');
      }
    }
  })(jQuery, Drupal, this, this.document);
</script>

<div class="row">
  <div class="col-sm-6">
    <?php print drupal_render($form['value']); ?>
  </div>
  <div class="col-sm-6">
    <?php print drupal_render($form['balance']); ?>
  </div>
</div>
<?php print drupal_render($form['lines']); ?>
<?php print drupal_render($form['mapping_refresh']); ?>
<?php print drupal_render($form['mapping']); ?>
<?php print drupal_render($form['revision_notes']); ?>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['buttons']); ?> </div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>