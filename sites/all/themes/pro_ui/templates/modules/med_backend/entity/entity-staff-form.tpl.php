<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {

      staff_table = $('.staff-table').dataTable({
        columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}],
        pageLength: 5,
        lengthMenu: [[5, 10, 20, 30, -1], [5, 10, 20, 30, 'All']],
        aaSorting: [],
        buttons: [],
      }).api();
      staff_table.on('order.dt search.dt', function () {
        staff_table.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
          cell.innerHTML = i + 1;
        });
      }).draw();
      
      staff_change();
      $('.staff-table').on('change', 'input:checkbox.staff-checkbox', function () {
        staff_change();
      });
      function staff_change() {
        var selected_staff = '';

        staff_table.$('input:checkbox.staff-checkbox').each(function () {
          if (staff_table.$(this).is(":checked")) {
            selected_staff += staff_table.$(this).data('id') + ',';
            staff_table.$(this).parents('tr').addClass('table-row-selected');
          } else {
            staff_table.$(this).parents('tr').removeClass('table-row-selected');
            if (staff_table.$(this).parents('tr').find('input:radio.staff-reporter-radio').is(":checked")) {
              staff_table.$(this).parents('tr').find('input:radio.staff-reporter-radio').prop('checked', false);
              $('#selected-reporter').val("");
            }
            if (staff_table.$(this).parents('tr').find('input:radio.staff-ref-1-radio').is(":checked")) {
              staff_table.$(this).parents('tr').find('input:radio.staff-ref-1-radio').prop('checked', false);
              $('#selected-ref-1').val("");
            }
            if (staff_table.$(this).parents('tr').find('input:radio.staff-cash-holder-radio').is(":checked")) {
              staff_table.$(this).parents('tr').find('input:radio.staff-cash-holder-radio').prop('checked', false);
              $('#selected-cash-holder').val("");
            }
          }
        });
        $('#selected-staff').val(selected_staff);
      }

      reporter_change();
      staff_table.$('input:radio.staff-reporter-radio').each(function () {
        if (staff_table.$(this).data('id') == $('#selected-reporter').val()) {
          staff_table.$(this).prop('checked', true);
        }
      });
      $('.staff-table').on('change', 'input:radio.staff-reporter-radio', function () {
        if ($(this).is(":checked")) {
          $('#selected-reporter').val(staff_table.$(this).data('id'));
        }
        reporter_change();
      });
      function reporter_change() {
        staff_table.$('input:radio.staff-reporter-radio').each(function () {
          if (staff_table.$(this).data('id') == $('#selected-reporter').val()) {
            staff_table.$(this).parents('tr').addClass('table-row-selected');
            staff_table.$(this).parents('tr').find('input:checkbox.staff-checkbox').prop('checked', true);
            staff_change();
          } else {
            staff_table.$(this).prop('checked', false);
          }
        });
      }

      cash_holder_change();
      staff_table.$('input:radio.staff-cash-holder-radio').each(function () {
        if (staff_table.$(this).data('id') == $('#selected-cash-holder').val()) {
          staff_table.$(this).prop('checked', true);
        }
      });
      $('.staff-table').on('change', 'input:radio.staff-cash-holder-radio', function () {
        if ($(this).is(":checked")) {
          $('#selected-cash-holder').val(staff_table.$(this).data('id'));
        }
        cash_holder_change();
      });
      function cash_holder_change() {
        staff_table.$('input:radio.staff-cash-holder-radio').each(function () {
          if (staff_table.$(this).data('id') == $('#selected-cash-holder').val()) {
            staff_table.$(this).parents('tr').addClass('table-row-selected');
            staff_table.$(this).parents('tr').find('input:checkbox.staff-checkbox').prop('checked', true);
            staff_change();
          } else {
            staff_table.$(this).prop('checked', false);
          }
        });
      }

      lead_trainer_change();
      staff_table.$('input:radio.staff-ref-1-radio').each(function () {
        if (staff_table.$(this).data('id') == $('#selected-ref-1').val()) {
          staff_table.$(this).prop('checked', true);
        }
      });
      $('.staff-table').on('change', 'input:radio.staff-ref-1-radio', function () {
        if ($(this).is(":checked")) {
          $('#selected-ref-1').val(staff_table.$(this).data('id'));
        }
        lead_trainer_change();
      });
      function lead_trainer_change() {
        staff_table.$('input:radio.staff-ref-1-radio').each(function () {
          if (staff_table.$(this).data('id') == $('#selected-ref-1').val()) {
            staff_table.$(this).parents('tr').addClass('table-row-selected');
            staff_table.$(this).parents('tr').find('input:checkbox.staff-checkbox').prop('checked', true);
            staff_change();
          } else {
            staff_table.$(this).prop('checked', false);
          }
        });
      }
    });
  })(jQuery, Drupal, this, this.document);
</script>

<?php print drupal_render($form['staff_container']); ?>   

<div class="form-group form-actions">
  <?php print drupal_render($form['submit']); ?> 
</div>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>