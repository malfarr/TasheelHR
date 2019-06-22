<?php
$step_number = 1;
if (isset($form['step_number'])) {
  print drupal_render($form['step_number']);
  $step_number = $form['step_number']['#value'];
}
$step_percentage = ($step_number / 8) * 100;
$theme_class = 'progress-bar-danger';
if ($step_percentage == 100) {
  $theme_class = 'progress-bar-success';
}
?>
<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
      step = <?php echo $step_number; ?>;
      switch (step) {
          case 1:
            $('#training-form-block .block-title').html('<h2><strong>Training</strong> Information</h2>')
            break;
          case 2:
            $('#training-form-block .block-title').html('<h2><strong>Trainers</strong></h2>')
            break;
          case 3:
            $('#training-form-block .block-title').html('<h2><strong>Agenda</strong></h2>')
            break;
          case 4:
            $('#training-form-block .block-title').html('<h2><strong>Venue</strong></h2>')
            break;
          case 5:
            $('#training-form-block .block-title').html('<h2><strong>Participants</strong></h2>')
            break;
          case 6:
            $('#training-form-block .block-title').html('<h2><strong>Financial</strong> Proposal</h2>')
            break;
          case 7:
            $('#training-form-block .block-title').html('<h2><strong>Staff</strong></h2>')
            break;
          case 8:
            $('#training-form-block .block-title').html('<h2><strong>Training</strong> Review</h2>')
            break;
        }

      for (var instanceName in CKEDITOR.instances) {
        CKEDITOR.remove(CKEDITOR.instances[instanceName]);
      }
      CKEDITOR.replaceAll('ckeditor');
      for (var i in CKEDITOR.instances) {
        CKEDITOR.instances[i].on('change', function () {
          CKEDITOR.instances[i].updateElement();
        });
      }

      $('html, body').animate({
        scrollTop: $("#page-content").offset().top
      }, 500);

      $(".error").removeClass("error").addClass("has-error");
      $(".form-group.has-error").removeClass("error");
      
      //Trainers Step --> Step 2    
      <?php if (isset($form['trainers'])) { ?>
        trainers_table = $('.trainers-table').dataTable({
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}],
          pageLength: 5,
          lengthMenu: [[5, 10, 20, 30, -1], [5, 10, 20, 30, 'All']],
          aaSorting: [],
          buttons: [],
        }).api();
        trainers_table.on('order.dt search.dt', function () {
          trainers_table.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();

        trainers_change();
        $('.trainers-table').on('change', 'input:checkbox.trainer-checkbox', function () {
          trainers_change();
        });

        function trainers_change() {
          var selected_trainers = '';
          selected_trainers_count = 0;
          trainers_table.$('input:checkbox.trainer-checkbox').each(function () {
            if (trainers_table.$(this).is(":checked")) {
              selected_trainers += trainers_table.$(this).data('id') + ',';
              trainers_table.$(this).parents('tr').addClass('table-row-selected');
              selected_trainers_count++;
            } else {
              trainers_table.$(this).parents('tr').removeClass('table-row-selected');
            }
          });
          $('#selected-trainers').val(selected_trainers);
          $('#cbt-trainers-wrapper .trainers-table-label').html('Trainers <span class="label label-success font-size-125 padding-2-10">' + selected_trainers_count + '</span>');

        }
<?php } ?>

<?php if (isset($form['employee_trainers'])) { ?>
        employee_trainers_table = $('.employee-trainers-table').dataTable({
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}],
          pageLength: 5,
          lengthMenu: [[5, 10, 20, 30, -1], [5, 10, 20, 30, 'All']],
          aaSorting: [],
          buttons: [],
        }).api();
        employee_trainers_table.on('order.dt search.dt', function () {
          employee_trainers_table.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();

        employee_trainers_change();
        $('.employee-trainers-table').on('change', 'input:checkbox.trainer-checkbox', function () {
          employee_trainers_change();
        });
        function employee_trainers_change() {
          var selected_employee_trainers = '';
          selected_employee_trainers_count = 0;
          employee_trainers_table.$('input:checkbox.trainer-checkbox').each(function () {
            if (employee_trainers_table.$(this).is(":checked")) {
              selected_employee_trainers += employee_trainers_table.$(this).data('id') + ',';
              employee_trainers_table.$(this).parents('tr').addClass('table-row-selected');
              selected_employee_trainers_count++;
            } else {
              employee_trainers_table.$(this).parents('tr').removeClass('table-row-selected');
            }
          });
          $('#selected-employee-trainers').val(selected_employee_trainers);
          $('#cbt-employee-trainers-wrapper .employee-trainers-table-label').html('Employee Trainers <span class="label label-success font-size-125 padding-2-10">' + selected_employee_trainers_count + '</span>');
        }
<?php } ?>

      //Agenda Step --> Step 3


      //Step 5 Participants
<?php if (isset($form['participants_display']) || isset($form['participants'])) { ?>
        participants_table = $('.table-options-list-participant').dataTable({
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

        training_participants_change();
        $('.table-options-list-participant').on('change', 'input:checkbox.participant-select-checkbox', function () {
          training_participants_change();
        });
        function training_participants_change() {
          var participants = '';
          participants_table.$('input:checkbox.participant-select-checkbox').each(function () {
            if (participants_table.$(this).is(":checked")) {
              participants += participants_table.$(this).val() + ',';
              participants_table.$(this).parent().parent().addClass("participant-select-row-selected table-options-list-selected-row");
            } else {
              participants_table.$(this).parent().parent().removeClass("participant-select-row-selected  table-options-list-selected-row");
            }
          });
          $('#training-participants').val(participants);
        }
<?php } ?>

        //Staff Step --> Step 7
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
            $('.training-form .next-form-submit').prop("disabled", true);

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
                  if ($('#selected-reporter').val() === '' || $('#selected-ref-1').val() === '' || $('#selected-cash-holder').val() === '') {
                    enable_next = true;
                  }                  
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
              if ($('#selected-staff').val() === '' || $('#selected-reporter').val() === '' || $('#selected-ref-1').val() === '' || $('#selected-cash-holder').val() === '') {
                enable_next = true;
              }
              $('.training-form .next-form-submit').prop("disabled", enable_next);
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

      //Review Step --> Step 8
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div style="padding: 20px 20px 0 20px;">
  <div class="progress progress-striped active">
    <div id="progress-bar-wizard" class="progress-bar <?php echo $theme_class; ?>" role="progressbar" aria-valuenow="<?php echo $step_percentage; ?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php echo $step_percentage; ?>%;"><?php echo $step_percentage; ?> %</div>
  </div>  
</div>

<?php if (isset($form['step_message'])) { ?> 
  <div class="form-group">
    <?php print drupal_render($form['step_message']); ?> 
  </div>
<?php } ?>

<?php
if (isset($form['general_error_field'])) {
  print drupal_render($form['general_error_field']);
}
?>

<!--Step 1 Training basic info-->
<?php if (isset($form['title'])) { print drupal_render($form['title']); } ?>
<?php if (isset($form['project_outcome_output'])) { print drupal_render($form['project_outcome_output']); } ?>
<?php if (isset($form['activity_code'])) { print drupal_render($form['activity_code']); } ?>
<?php if (isset($form['goals'])) { print drupal_render($form['goals']); } ?>

<!--Step 2 Trainers-->
<?php if (isset($form['selected_trainers'])) { print drupal_render($form['selected_trainers']); } ?>
<?php if (isset($form['trainers'])) { print drupal_render($form['trainers']); } ?>
<?php if (isset($form['selected_employee_trainers'])) { print drupal_render($form['selected_employee_trainers']); } ?>
<?php if (isset($form['employee_trainers'])) { print drupal_render($form['employee_trainers']); } ?>

<!--Step 3 Duration and Agenda-->
<?php if (isset($form['agenda'])) { print drupal_render($form['agenda']); } ?>

<!--Step 4 Venue-->
<?php if (isset($form['location'])) { print drupal_render($form['location']); } ?>

<!--Step 5 Participant-->
<?php if (isset($form['participants_display']) || isset($form['participants'])) { ?>
  <div class="form-group">
    <label class="trainers-table-label" for="">Participants</label>
    <?php if (isset($form['participants_display'])) { print drupal_render($form['participants_display']); } ?>
    <?php if (isset($form['participants'])) { print drupal_render($form['participants']); } ?>
  </div>
<?php } ?>

<!--Step 6 Financial proposal-->
<?php if (isset($form['financial_proposal'])) { print drupal_render($form['financial_proposal']); } ?>
<?php if (isset($form['financial_items'])) { print drupal_render($form['financial_item']); } ?>
<?php if (isset($form['direction_map'])) { print drupal_render($form['direction_map']); } ?>

<!--Step 7 Supervisors-->
<?php if (isset($form['staff_container'])) { ?>
  <?php print drupal_render($form['staff_container']); ?>    
<?php } ?>

<!--Step 8 Review-->
<?php if (isset($form['review'])) { ?>  
  <div class="form-group">
    <?php print drupal_render($form['review']); ?>  
  </div>
<?php } ?>

<!--Back next button-->
<div class="form-group form-actions form-actions-btn-primary">
  <?php if (isset($form['buttons']['back'])) { print drupal_render($form['buttons']['back']); } ?>
  <?php if (isset($form['buttons']['next'])) { print drupal_render($form['buttons']['next']); } ?>
  <?php if (isset($form['buttons']['submit'])) { print drupal_render($form['buttons']['submit']); } ?>
  <?php if (isset($form['buttons']['save_draft'])) { print drupal_render($form['buttons']['save_draft']); } ?>
</div>

<?php print drupal_render($form['form_build_id']); ?>
<?php print drupal_render($form['form_token']); ?>
<?php print drupal_render($form['form_id']); ?>