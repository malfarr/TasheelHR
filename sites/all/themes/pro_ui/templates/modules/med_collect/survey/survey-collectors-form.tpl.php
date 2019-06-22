<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
      staff_table = $('.table-options-list-staff').dataTable({
        columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}],
        bPaginate: false,
        aaSorting: [],
        buttons: [],
         scrollX: true,
        scrollY: "400px",
      });

      staff_change();
      $('.table-options-list-staff').on('change', 'input:checkbox.staff-select-checkbox', function () {
        staff_change();
      });
      function staff_change() {
        var staff = '';

        staff_table.$('input:checkbox.staff-select-checkbox').each(function () {
          if (staff_table.$(this).is(":checked")) {
            staff += staff_table.$(this).val() + ',';
            staff_table.$(this).parent().parent().addClass("success");
          } else {
            staff_table.$(this).parent().parent().removeClass("success");
          }
        });
        $('#staff-uids').val(staff);

      }
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php print drupal_render($form['staff_display']); ?>
<?php print drupal_render($form['staff']); ?>
<div class="form-group form-actions form-actions-btn-primary">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>