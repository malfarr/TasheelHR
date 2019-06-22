<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
        
      }
    };
    $(document).ready(function () {
      for (var instanceName in CKEDITOR.instances)
        CKEDITOR.remove(CKEDITOR.instances[instanceName]);
      CKEDITOR.replaceAll('ckeditor');
      
      
      $('html, body').animate({
        scrollTop: $("#page-content").offset().top
      }, 500);

      $(".error").removeClass("error").addClass("has-error");
      $(".form-group.has-error").removeClass("error");

      //Basic information --> Step 1
      $(".form-item-is-international-day").addClass("form-group");
      $(".form-item-no-official-letter").addClass("form-group");

      //Venue Step --> Step 2            
      $(".container-inline-date").addClass("form-group").find(".form-group").removeClass("form-group");

      //Leaders Step --> Step 3
      <?php if (isset($form['participants_display']) || isset($form['participants'])) { ?>
        participants_table = $('.table-options-list-participants').dataTable({
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}],
          pageLength: 10,
          lengthMenu: [[10, 20, 30, -1], [10, 20, 30, 'All']],
          aaSorting: [],
          buttons: [],
        }).api();
        participants_table.on('order.dt search.dt', function () {
          participants_table.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();

        $('.add-activity-form .next-form-submit').prop("disabled", true);
        activity_participants_change();
        participants_table.$('input:checkbox.participant-select-checkbox').change(function () {
          activity_participants_change();
        });        
        function activity_participants_change() {
          var participants = '';

          var enable_next = false;

          participants_table.$('input:checkbox.participant-select-checkbox').each(function () {
            if (participants_table.$(this).is(":checked")) {
              participants += participants_table.$(this).val() + ',';
              participants_table.$(this).parent().parent().addClass("participant-select-row-selected table-options-list-selected-row");
              
              enable_next = true;              
            } else {
              participants_table.$(this).parent().parent().removeClass("participant-select-row-selected  table-options-list-selected-row");              
            }
          });
          if (participants === '') {
            enable_next = true;
          }
          if (participants === '' && $(".activity-participants-optional").val() === "1") {
            enable_next = false;
          }

          $('.add-activity-form .next-form-submit').prop("disabled", enable_next);
          $('#activity-participants').val(participants);

    }
<?php } ?>

      //Participant Step --> Step 4
      $('#participant-children-wrapper input[type="radio"][value=""]').parent().addClass('radio-empty-option');
      $('#participant-children-wrapper .children-participant-clear').click(function () {
        $('#participant-children-wrapper input[type="text"]').val('');
        $('#participant-children-wrapper input[type="radio"]').prop("checked", false);
        $('#participant-children-wrapper input[type="radio"]').removeAttr("checked");
        $('#participant-children-wrapper input[type="radio"][value=""]').attr('checked', 'checked');
      });

      $('#participant-youth-wrapper input[type="radio"][value=""]').parent().addClass('radio-empty-option');
      $('#participant-youth-wrapper .youth-participant-clear').click(function () {
        $('#participant-youth-wrapper input[type="text"]').val('');
        $('#participant-youth-wrapper input[type="radio"]').prop("checked", false);
        $('#participant-youth-wrapper input[type="radio"]').removeAttr("checked");
        $('#participant-youth-wrapper input[type="radio"][value=""]').attr('checked', 'checked');
      });

      $('#participant-community-members-wrapper input[type="radio"][value=""]').parent().addClass('radio-empty-option');
      $('#participant-community-members-wrapper .community-members-participant-clear').click(function () {
        $('#participant-community-members-wrapper input[type="text"]').val('');
        $('#participant-community-members-wrapper input[type="radio"]').prop("checked", false);
        $('#participant-community-members-wrapper input[type="radio"]').removeAttr("checked");
        $('#participant-community-members-wrapper input[type="radio"][value=""]').attr('checked', 'checked');
      });

      //Partner Staff
<?php if (isset($form['partner_staff']) && isset($form['partner_staff_table'])) { ?>
        partner_staff_table = $('.partner-staff-table').dataTable({
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}],
          pageLength: 5,
          lengthMenu: [[5, 10, 20, 30, -1], [5, 10, 20, 30, 'All']],
          aaSorting: [],
          buttons: [],
        }).api();
        partner_staff_table.on('order.dt search.dt', function () {
          partner_staff_table.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();

        partner_staff_change();
        $('.partner-staff-table').on('change', 'input:checkbox.partner-staff-checkbox', function () {
          partner_staff_change();
        });

        function partner_staff_change() {
          var selected_partner_staff = '';
          selected_partner_staff_count = 0;
          partner_staff_table.$('input:checkbox.partner-staff-checkbox').each(function () {
            if (partner_staff_table.$(this).is(":checked")) {
              selected_partner_staff += partner_staff_table.$(this).data('id') + ',';
              partner_staff_table.$(this).parents('tr').addClass('table-row-selected');
              selected_partner_staff_count++;
            } else {
              partner_staff_table.$(this).parents('tr').removeClass('table-row-selected');
            }
          });
          $('#selected-partner-staff').val(selected_partner_staff);
          $('#partner-staff-wrapper .partner-staff-table-label').html('Partner Staff <span class="label label-success font-size-125 padding-2-10">' + selected_partner_staff_count + '</span>');

        }
<?php } ?>

      //Associates
<?php if (isset($form['associates']) && isset($form['associates_table'])) { ?>
        associates_table = $('.associates-table').dataTable({
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}],
          pageLength: 5,
          lengthMenu: [[5, 10, 20, 30, -1], [5, 10, 20, 30, 'All']],
          aaSorting: [],
          buttons: [],
        }).api();
        associates_table.on('order.dt search.dt', function () {
          associates_table.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();

        associates_change();
        $('.associates-table').on('change', 'input:checkbox.associate-checkbox', function () {
          associates_change();
        });

        function associates_change() {
          var selected_associates = '';
          selected_associates_count = 0;
          associates_table.$('input:checkbox.associate-checkbox').each(function () {
            if (associates_table.$(this).is(":checked")) {
              selected_associates += associates_table.$(this).data('id') + ',';
              associates_table.$(this).parents('tr').addClass('table-row-selected');
              selected_associates_count++;
            } else {
              associates_table.$(this).parents('tr').removeClass('table-row-selected');
            }
          });
          $('#selected-associates').val(selected_associates);
          $('#associates-wrapper .associates-table-label').html('Associates <span class="label label-success font-size-125 padding-2-10">' + selected_associates_count + '</span>');

        }
<?php } ?>
      //Step 6 --> Staff
<?php if (isset($form['staff_container'])) { ?>
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
        $('.activity-form .next-form-submit').prop("disabled", true);

        staff_change();
        $('.staff-table').on('change', 'input:checkbox.staff-checkbox', function () {
          staff_change();
        });
        function staff_change() {
          var selected_staff = '';

          var enable_next = false;
          staff_table.$('input:checkbox.staff-checkbox').each(function () {
            if (staff_table.$(this).is(":checked")) {
              selected_staff += staff_table.$(this).data('id') + ',';
              staff_table.$(this).parents('tr').addClass('table-row-selected');
              if ($('#selected-reporter').val() === '' || $('#selected-cash-holder').val() === '') {
                enable_next = true;
              }
            } else {
              staff_table.$(this).parents('tr').removeClass('table-row-selected');
              if (staff_table.$(this).parents('tr').find('input:radio.staff-reporter-radio').is(":checked")) {
                staff_table.$(this).parents('tr').find('input:radio.staff-reporter-radio').prop('checked', false);
                $('#selected-reporter').val("");
              }
              if (staff_table.$(this).parents('tr').find('input:radio.staff-cash-holder-radio').is(":checked")) {
                staff_table.$(this).parents('tr').find('input:radio.staff-cash-holder-radio').prop('checked', false);
                $('#selected-cash-holder').val("");
              }
            }
          });
          $('#selected-staff').val(selected_staff);
          if ($('#selected-staff').val() === '' || $('#selected-ref-1').val() === '' || $('#selected-cash-holder').val() === '') {
            enable_next = true;
          }
          $('.activity-form .next-form-submit').prop("disabled", enable_next);
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
<?php } ?>
    });
  })(jQuery, Drupal, this, this.document);
</script>
<?php
if (isset($form['step_number'])) {
  print drupal_render($form['step_number']);
  $step_number = $form['step_number']['#value'];
  $step_percentage = ($step_number / 8) * 100;
  $theme_class = 'progress-bar-danger';
  if ($step_percentage == 100) {
    $theme_class = 'progress-bar-success';
  }
}
?>
<div style="padding: 20px 20px 0 20px;">
  <div class="progress progress-striped active">
    <div id="progress-bar-wizard" class="progress-bar <?php echo $theme_class; ?>" role="progressbar" aria-valuenow="<?php echo $step_percentage; ?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php echo $step_percentage; ?>%;"><?php echo $step_percentage; ?> %</div>
  </div>  
</div>
<?php if (isset($form['step_message'])) { ?> 
  <?php print drupal_render($form['step_message']); ?> 
<?php } ?>
<?php if (isset($form['general_error_field'])) {
  print drupal_render($form['general_error_field']);
} ?>
<!--Step 1 Basic info-->
<?php if (isset($form['title'])) {
  print drupal_render($form['title']);
} ?>
<?php if (isset($form['type'])) {
  print drupal_render($form['type']);
} ?>
<?php if (isset($form['activity_sub_type'])) {
  print drupal_render($form['activity_sub_type']);
} ?>
<?php if (isset($form['project'])) {
  print drupal_render($form['project']);
} ?>
  <?php if (isset($form['project_outcome_output'])) {
    print drupal_render($form['project_outcome_output']);
  } ?>
<?php if (isset($form['office'])) {
  print drupal_render($form['office']);
} ?>
  <?php if (isset($form['cla'])) {
    print drupal_render($form['cla']);
  } ?>
<?php if (isset($form['program'])) {
  print drupal_render($form['program']);
} ?>
<?php if (isset($form['activity_code'])) {
  print drupal_render($form['activity_code']);
} ?>
<?php if (isset($form['goals'])) {
  print drupal_render($form['goals']);
} ?>
<?php if (isset($form['official_letter'])) {
  print drupal_render($form['official_letter']);
} ?>
<?php if (isset($form['no_official_letter'])) {
  print drupal_render($form['no_official_letter']);
} ?>
  <?php if (isset($form['official_letter_justification'])) {
    print drupal_render($form['official_letter_justification']);
  } ?>

<!--Step 2 Venue-->
  <?php if (isset($form['venue'])) {
    print drupal_render($form['venue']);
  } ?>
  <?php if (isset($form['start_date']) || isset($form['start_time']) || isset($form['end_date']) || isset($form['end_time'])) { ?>
  <div class="form-group form-item">
    <?php if (isset($form['start_date'])) {
      print drupal_render($form['start_date']);
    } ?>
  <?php if (isset($form['start_time'])) {
    print drupal_render($form['start_time']);
  } ?>
  </div>
  <div class="form-group form-item">
    <?php if (isset($form['end_date'])) {
      print drupal_render($form['end_date']);
    } ?>
  <?php if (isset($form['end_time'])) {
    print drupal_render($form['end_time']);
  } ?>
  </div>
<?php } ?>
<?php if (isset($form['is_international_day'])) {
  print drupal_render($form['is_international_day']);
} ?>
<?php if (isset($form['international_day'])) {
  print drupal_render($form['international_day']);
} ?>
<?php if (isset($form['agenda'])) {
  print drupal_render($form['agenda']);
} ?>

<!--Step 3 Participants-->
<?php if (isset($form['participants_display']) || isset($form['participants']) || isset($form['participants_optioanal'])) { ?>
  <div class="form-group">
  <?php if (isset($form['participants_display'])) {
    print drupal_render($form['participants_display']);
  } ?>
  <?php if (isset($form['participants'])) {
    print drupal_render($form['participants']);
  } ?>
  <?php if (isset($form['participants_optioanal'])) {
    print drupal_render($form['participants_optioanal']);
  } ?>
  </div>
<?php } ?>

<!--Step 4 Beneficiaries-->
<?php if (isset($form['beneficiaries'])) {
  print drupal_render($form['beneficiaries']);
} ?>
<?php if (isset($form['influencers'])) {
  print drupal_render($form['influencers']);
} ?>
<?php if (isset($form['youth'])) {
  print drupal_render($form['youth']);
} ?>

<!--Step 5 Financial proposal-->
<?php if (isset($form['financial_proposal'])) {
  print drupal_render($form['financial_proposal']);
} ?>
<?php if (isset($form['financial_items'])) {
  print drupal_render($form['financial_item']);
} ?>
<?php
if (isset($form['direction_map'])) {
  print drupal_render($form['direction_map']);
}
?>

<!--Step 6 Supervisors-->
<?php if (isset($form['staff_container'])) { ?>
  <?php print drupal_render($form['staff_container']); ?>    
<?php } ?>

<!--Step 7 Review-->
<?php if (isset($form['review'])) { ?> 
  <div class="form-group">
  <?php print drupal_render($form['review']); ?> 
  </div>
<?php } ?>

<!--Back next button-->
<div class="form-group form-actions form-actions-btn-primary">
<?php if (isset($form['buttons']['back'])) {
  print drupal_render($form['buttons']['back']);
} ?>
<?php if (isset($form['buttons']['next'])) {
  print drupal_render($form['buttons']['next']);
} ?>
<?php if (isset($form['buttons']['submit'])) {
  print drupal_render($form['buttons']['submit']);
} ?>
<?php if (isset($form['buttons']['save_draft'])) {
  print drupal_render($form['buttons']['save_draft']);
} ?>
</div>

<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_token']);
print drupal_render($form['form_id']);
?>