/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {


// To understand behaviors, see https://drupal.org/node/756722#behaviors
  Drupal.behaviors.med_behavior_general = {
    attach: function (context, settings) {
      App.init();
      App.datatables();

      var myLazyLoad = new LazyLoad({
        elements_selector: ".lazy"
      });

      page_content_width = $('#page-content').width();

      $(".pmf-list tbody tr > td > p").css("maxWidth", (page_content_width - 100) + "px");

      $("ul.nav-tabs li a.active").parent().addClass("active");

      $('fieldset.financial-items-fieldset a.fieldset-title').click(function (event) {
        $('fieldset.financial-items-fieldset #map-canvas-direction').gmap('refresh');
        $('fieldset.financial-items-fieldset #map-canvas-direction').gmap('option', 'zoom', 13);
      });

      $('a#btn-tab-summary').click(function (event) {
        $('#map-canvas-view').gmap('refresh');
      });

      $("input.input-group").removeClass('input-group').parent('.form-group').addClass('input-group');

      $(".input-group label").addClass('input-group-addon');

      $(".form-type-plupload").find(".form-group").removeClass("form-group");

      $(".plupload-submit-form").prop('disabled', true);

      $('.expandable-textarea').parent('.form-textarea-wrapper').addClass('expandable-textarea-wrapper');

      $('.custom-datepicker').datepicker({
        weekStart: 7,
        todayHighlight: true,
        firstDay: 0,
      }).on('changeDate', function (e) {
        $(this).datepicker('hide');
      });

      $('.custom-future-datepicker').datepicker({
        weekStart: 7,
        todayHighlight: true,
        startDate: "+0d",
        firstDay: 0,
      }).on('changeDate', function (e) {
        $(this).datepicker('hide');
      });

      $('.input-daterange.input-daterange-all-dates').datepicker({
        weekStart: 7,
        todayHighlight: true,
        firstDay: 0,
      }).on('changeDate', function (e) {
        $(this).datepicker('hide');
      });

      $('.input-daterange').datepicker({
        weekStart: 7,
        startDate: "+0d",
        todayHighlight: true,
        firstDay: 0,
      }).on('changeDate', function (e) {
        $(this).datepicker('hide');
      });


      $('.custom-timepicker').timepicker({
        minuteStep: 15,
        showSeconds: false,
        showMeridian: true,
        defaultTime: false,
        disableMousewheel: true,
        modalBackdrop: true,
        explicitMode: true
      });

      $('.duration-timepicker').timepicker({
        minuteStep: 5,
        showSeconds: false,
        showMeridian: false,
        defaultTime: false,
        disableMousewheel: true,
        modalBackdrop: true,
        explicitMode: true
      });

      /*
       * Datatables
       */
      $('.datatable-custom').once().dataTable({
        aaSorting: [],
        columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
        bSort: false,
        bFilter: false,
        bInfo: false,
        bPaginate: false,
        buttons: [],
      });

      $('.datatable-minimal').once().dataTable({
        aaSorting: [],
        columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
        bSort: false,
        bFilter: false,
        bInfo: false,
        bPaginate: false,
        sDom: 't',
        buttons: [],
      });

      $('.datatable-all').once().dataTable({
        aaSorting: [],
        columnDefs: [
          {orderable: false, targets: 'nosort'},
          {searchable: false, targets: 'nosearch'},
          {targets: 'colspan-enabled', defaultContent: ''}],
        pageLength: 10,
        lengthMenu: [[10, 20, 30, -1], [10, 20, 30, 'All']],
        buttons: ['copy', 'csv', 'excel', 'print'],
      });

      $('.datatable-all-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          pageLength: 10,
          lengthMenu: [[10, 20, 30, -1], [10, 20, 30, 'All']],
          buttons: ['copy', 'csv', 'excel', 'print'],
        }).api();

        indexed_list.on('order.dt search.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });

      $('.datatable-custom-export-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          bSort: false,
          bFilter: false,
          bInfo: false,
          bPaginate: false,
          buttons: ['copy', 'csv', 'excel', 'print'],
        }).api();

        indexed_list.on('draw.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });

      $('.datatable-custom-export').once().dataTable({
        aaSorting: [],
        columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
        bSort: false,
        bFilter: false,
        bInfo: false,
        bPaginate: false,
        buttons: ['copy', 'csv', 'excel', 'print'],
      });

      $('.datatable-custom-export-sort-filter-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          bPaginate: false,
          buttons: ['copy', 'csv', 'excel', 'print'],
        }).api();

        indexed_list.on('order.dt search.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });


      $('.datatable-custom-export-sort-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          bPaginate: false,
          bFilter: false,
          buttons: ['copy', 'csv', 'excel', 'print'],
        }).api();

        indexed_list.on('order.dt search.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });

      $('.datatable-custom-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          bSort: false,
          bFilter: false,
          bInfo: false,
          bPaginate: false,
          buttons: [],
        }).api();

        indexed_list.on('draw.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });
      
      $('.datatable-custom-sort-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          bFilter: false,
          bInfo: true,
          bPaginate: false,
          buttons: [],
        }).api();

        indexed_list.on('order.dt search.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });
      
      $('.datatable-custom-sort-filter-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          bPaginate: false,
          buttons: [],
        }).api();

        indexed_list.on('order.dt search.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });

      $('.datatable-custom-sort-filter-pager-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          pageLength: 10,
          lengthMenu: [[10, 20, 30, -1], [10, 20, 30, 'All']],
          buttons: [],
        }).api();

        indexed_list.on('order.dt search.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });

      $('.datatable-custom-export-sort-filter').once().dataTable({
        aaSorting: [],
        columnDefs: [
          {orderable: false, targets: 'nosort'},
          {searchable: false, targets: 'nosearch'},
          {targets: 'colspan-enabled', defaultContent: ''}],
        bPaginate: false,
        buttons: ['copy', 'csv', 'excel', 'print'],
      });

      $('.datatable-custom-export-sort').once().dataTable({
        aaSorting: [],
        columnDefs: [
          {orderable: false, targets: 'nosort'},
          {searchable: false, targets: 'nosearch'},
          {targets: 'colspan-enabled', defaultContent: ''}],
        bFilter: false,
        bPaginate: false,
        buttons: ['copy', 'csv', 'excel', 'print'],
      });

      $('.datatable-custom-sort-filter').once().dataTable({
        aaSorting: [],
        columnDefs: [
          {orderable: false, targets: 'nosort'},
          {searchable: false, targets: 'nosearch'},
          {targets: 'colspan-enabled', defaultContent: ''}],
        bPaginate: false,
        buttons: [],
      });

      $('.datatable-custom-export-sort-filter').once().dataTable({
        aaSorting: [],
        columnDefs: [
          {orderable: false, targets: 'nosort'},
          {searchable: false, targets: 'nosearch'},
          {targets: 'colspan-enabled', defaultContent: ''}],
        bPaginate: false,
        buttons: ['copy', 'csv', 'excel', 'print'],
      });

      $('.datatable-custom-sort').once().dataTable({
        aaSorting: [],
        columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
        bFilter: false,
        bInfo: false,
        bPaginate: false,
        sDom: 't',
        buttons: [],
      });

      $('.dataTables_filter input').once().attr('placeholder', 'Search');

      $('.datatable-custom-export-fixed').once().dataTable({
        aaSorting: [],
        columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
        bSort: false,
        bFilter: false,
        bInfo: false,
        bPaginate: false,
        buttons: ['copy', 'csv', 'excel', 'print'],
        scrollX: true,
        scrollY: "400px",
        fixedColumns: {
          leftColumns: 1
        }
      });

      $('.datatable-custom-index').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          bSort: false,
          bFilter: false,
          bInfo: false,
          bPaginate: false,
          buttons: [],
        }).api();

        indexed_list.on('draw.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });

      $('.datatable-custom-index-export-search-sort-scroll-500').once().each(function () {
        var indexed_list = $(this).dataTable({
          aaSorting: [],
          columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
          bFilter: true,
          bInfo: false,
          bPaginate: false,
          buttons: ['copy', 'excel', 'pdf', 'print'],
          scrollX: true,
          scrollY: "500px",
        }).api();

        indexed_list.on('draw.dt', function () {
          indexed_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
            cell.innerHTML = i + 1;
          });
        }).draw();
      });

      /*
       * Input Mask
       */
      $('.masked-duration-minutes').mask('99:99');
      $('.masked-date').mask("99/99/9999", {placeholder: 'DD/MM/YYYY'});


      $('span.text-collapsed').click(function () {
        if ($(this).hasClass('closed')) {
          $(this).switchClass("closed", "opened", 500, "swing");
          $(this).find('.fas').switchClass("fa-plus", "fa-minus");
        } else {
          $(this).switchClass("opened", "closed", 500, "swing");
          $(this).find('.fas').switchClass("fa-minus", "fa-plus");
        }
      });

      $('.btn-2-clicks').attr('disabled', 'disabled');
      $('.btn-2-clicks-wrapper').click(function () {
        $('.btn-2-clicks').attr('disabled', 'disabled');

        var removeButton = $(this).find('.btn-2-clicks').removeAttr('disabled');
        $(removeButton).removeAttr('disabled');

        setTimeout(function () {
          $(removeButton).attr('disabled', 'disabled');
        }, 5000);
      });

      $(".btn-checkbox-confirm-remove").prop('disabled', true);
      $('.checkbox-confirm-activate-remove').change(function () {
        var checkbox_switch = $(this);
        var removeButton = $(this).parent().parent().find(".btn-checkbox-confirm-remove");
        if ($(this).is(":checked")) {
          $(removeButton).prop('disabled', false);


          setTimeout(function () {
            $(removeButton).prop('disabled', true);
            $(checkbox_switch).attr("checked", false);
          }, 5000);
        } else {
          $(removeButton).prop('disabled', true);
        }
      });

      $('[data-toggle="lightbox-image"]').magnificPopup({
        type: 'image',
        image: {titleSrc: 'title'},
        retina: {ratio: 1, }
      });

      $('[data-toggle="lightbox-gallery"]').each(function () {
        $(this).magnificPopup({
          delegate: 'a.gallery-link',
          type: 'image',
          gallery: {
            enabled: true,
            navigateByImgClick: true,
            arrowMarkup: '<button type="button" class="mfp-arrow mfp-arrow-%dir%" title="%title%"></button>',
            tPrev: 'Previous',
            tNext: 'Next',
            tCounter: '<span class="mfp-counter">%curr% of %total%</span>'
          },
          image: {titleSrc: 'title'},
          retina: {ratio: 1, }

        });
      });

      $(".form-item-chosen-dynamic .chosen-container input[type=text]").on('keyup', function (e) {
        if ($(this).parents('.chosen-container').find('li.no-results').length > 0 &&
                $(this).parents('.chosen-container').find('li.no-results span.chosen-new-term').length == 0) {
          $(this).parents('.chosen-container').find('li.no-results span').addClass('chosen-new-term');
          $(this).parents('.chosen-container').find('li.no-results').append('<span class="chosen-add-new">Add New</span>');
          $('.chosen-add-new').on('click', function (e) {
            var term_text = $(this).parents('.chosen-container').find('li.no-results span.chosen-new-term').text();
            var opt = document.createElement("OPTION");
            opt.value = term_text;
            opt.text = term_text;
            opt.selected = true;
            $(this).parents('.form-item-chosen-dynamic').find('.select-chosen-dynamic').append(opt).trigger("chosen:updated");
          });
        }
      });

      $('.header-help-btn').click(function () {
        introJs().start();
      });

      /*
       * User account pages
       */
      $(".user-account-form .form-item-pass input.password-field").on('keyup', function (e) {
        var passwordStrengthClass = $('.password-strength-text').text().toLowerCase() + '-password';
        $('.password-strength-text').addClass('weak-password');

      });

      /*
       * Forms validation
       */
      $('.form-jquery-validate').validate({
        errorClass: 'help-block animation-slideDown',
        errorElement: 'div',
        errorPlacement: function (error, e) {
          if (error.text() === 'Please provide a coordinates') {
            $('#set-location-wrapper .help-block').remove();
            $('#set-location-wrapper').append(error);
            $('#set-location-wrapper').removeClass('has-success has-error hide-errors').addClass('has-error');
          } else if (error.text() === 'Please select the color code of this office') {
            $('#color-field-wrapper .help-block').remove();
            $('#color-field-wrapper').append(error);
            $('#color-field-wrapper').removeClass('has-success has-error hide-errors').addClass('has-error');
          } else {
            e.parents('.form-group').append(error);
          }

        },
        highlight: function (e) {
          $(e).closest('.form-group').removeClass('has-success has-error').addClass('has-error');
          $(e).closest('.help-block').remove();
        },
        success: function (e) {
          e.closest('.form-group').removeClass('has-success has-error').addClass('has-success');
          e.closest('.help-block').remove();
        },
        ignore: "",
        rules: {
        },
        messages: {
        }
      });
      $('.form-jquery-validate .form-submit').click(function () {
        for (var i in CKEDITOR.instances) {
          CKEDITOR.instances[i].updateElement();
          $.trim($('#' + i).val());
        }
      });

      /*
       * Changelog
       */
      $('.changelog-view-btn').click(function (event) {
        event.preventDefault();

        $('body').addClass('changelog-open');
        $('.changelog-wrapper').fadeIn('slow');
      });
      $('.changelog-close-btn').click(function (event) {
        event.preventDefault();

        $('body').removeClass('changelog-open');
        $('.changelog-wrapper').fadeOut('slow');
      });

      /*
       * Confirm message on leave
       */
//      $(document).on("submit", "form", function (event) {
//        $(window).off('beforeunload');
//      });
//      $('.form-submit').on('click', function () {
//        $(window).off('beforeunload');
//      });
//      
//      $(window).on('beforeunload', function () {
//        if ($(".confirm-leave").length) {
//          var message = "Changes you made may not be saved.", e = e || window.event;
//          if (e) {
//            e.returnValue = message;
//          }
//          return message;
//        }
//      });

      /*
       * Convert messages to notify
       */
//      $('.alert').notifyMessage();
      $('.notify-message').once().each(function () {
        message = $(this).data('notify-message');
        type = $(this).data('notify-type');
        $(this).notifyMessage(message, type);
        $(this).remove();
      });
    }
  };

  Drupal.behaviors.med_behavior_general.pluploadCompleted = function () {
    $(".plupload-submit-form").prop('disabled', false);
  };

  Drupal.behaviors.med_behavior_general.pluploadOptionalCompleted = function () {
    $(".form-submit").prop('disabled', false);
  };

  $(document).ready(function () {
    $(".container-inline-date").addClass("form-group").find(".form-group").removeClass("form-group");
    $(".form-type-plupload").find(".form-group").removeClass("form-group");
    $('form .form-actions-btn-primary .form-submit').removeClass('btn-success');
    $(".confirm-delete").confirm();

    $('.fsv-details-wrapper .observation-questions-result-list .question-option-cell span.option-hint').click(function () {
      if ($(this).hasClass('closed')) {
        $(this).switchClass("closed", "opened", 500, "swing");
        $(this).find('.fa').switchClass("fa-plus", "fa-minus");
      } else {
        $(this).switchClass("opened", "closed", 500, "swing");
        $(this).find('.fa').switchClass("fa-minus", "fa-plus");
      }
    });

    $('.form-item-sn > *')
            .focus(function () {
              $('.form-item-sn').addClass('focused');
            })
            .blur(function () {
              $('.form-item-sn').removeClass('focused');
            });

    $('.count-to-number').data('countToOptions', {
      formatter: function (value, options) {
        return value.toFixed(options.decimals).replace(/\B(?=(?:\d{3})+(?!\d))/g, ',');
      }
    });
    $('.count-to-number').each(count);
    function count(options) {
      var $this = $(this);
      options = $.extend({}, options || {}, $this.data('countToOptions') || {});
      $this.countTo(options);
    }

    /*
     * Custom dropdown
     */
    $('.custom-dropdown .custom-dropdown-btn').on('click', function () {
      $('.custom-dropdown').not($(this).parents('.custom-dropdown')).removeClass('open');
      $('.custom-dropdown').not($(this).parents('.custom-dropdown')).find('.custom-dropdown-btn-icon').removeClass('fa-angle-up').addClass('fa-angle-down');

      $(this).parents('.custom-dropdown').toggleClass('open');

      if ($(this).parents('.custom-dropdown').hasClass('open')) {
        $(this).find('.custom-dropdown-btn-icon').removeClass('fa-angle-down').addClass('fa-angle-up');
      } else {
        $(this).find('.custom-dropdown-btn-icon').removeClass('fa-angle-up').addClass('fa-angle-down');
      }
    });

  });

  $(document).keyup(function (e) {
    if (e.keyCode == 27) {
      if ($('body').hasClass('changelog-open')) {
        $('body').removeClass('changelog-open');
        $('.changelog-wrapper').fadeOut('slow');
      }

    }
  });

  $.fn.remove_element = function (element_id) {
    var effects = Array('explode', 'bounce', 'fade', 'blind', 'clip', 'drop', 'fold', 'transfer', 'size', 'shake');
    $("#" + element_id)
            .addClass('width-100')
            .html('<div class="file-remove-waiting themed-background-warning text-light"><div class="file-remove-loading"></div>file is being removed</div>')
            .delay(5000)
            .queue(function (n) {
              $(this).html('<div class="file-remove-waiting themed-background-success text-light">file has been removed</div>');
              n();
            })
            .delay(5000)
            .fadeOut('slow', function () {
              $("#" + element_id).remove();
            });
  }

  $.fn.ajax_edit_focal_point = function () {
    $('.edit-focal-point-form .form-submit').removeClass('btn-success');
    $('.edit-focal-point-form').validate({
      errorClass: 'help-block animation-slideDown',
      errorElement: 'div',
      errorPlacement: function (error, e) {
        e.parents('.form-group').append(error);
      },
      highlight: function (e) {
        $(e).closest('.form-group').removeClass('has-success has-error').addClass('has-error');
        $(e).closest('.help-block').remove();
      },
      success: function (e) {
        e.closest('.form-group').removeClass('has-success has-error').addClass('has-success');
        e.closest('.help-block').remove();

      },
      ignore: "",
      rules: {
        focal_point_telephone: {
          oneAtLeastRequired: ['.focal-point-telephone', '.focal-point-mobile', '.focal-point-email']
        },
        focal_point_mobile: {
          oneAtLeastRequired: ['.focal-point-telephone', '.focal-point-mobile', '.focal-point-email']
        },
        focal_point_email: {
          oneAtLeastRequired: ['.focal-point-telephone', '.focal-point-mobile', '.focal-point-email']
        },
      },
      messages: {
        focal_point_telephone: {
          oneAtLeastRequired: 'One of Telehone, Mobile, or email is requred',
        },
        focal_point_mobile: {
          oneAtLeastRequired: 'One of Telehone, Mobile, or email is requred',
        },
        focal_point_email: {
          oneAtLeastRequired: 'One of Telehone, Mobile, or email is requred',
        },
      }
    });
  }

  $.fn.ajax_add_leader = function () {
  }

  $.fn.ajax_interactive_blocks = function () {
    // Toggle block's content
    $('[data-toggle="block-toggle-content"]').on('click', function () {
      var blockContent = $(this).closest('.block').find('.block-content');

      if ($(this).hasClass('active')) {
        blockContent.slideDown();
      } else {
        blockContent.slideUp();
      }

      $(this).toggleClass('active');
    });

    // Toggle block fullscreen
    $('[data-toggle="block-toggle-fullscreen"]').on('click', function () {
      var block = $(this).closest('.block');

      if ($(this).hasClass('active')) {
        block.removeClass('block-fullscreen');
      } else {
        block.addClass('block-fullscreen');
      }

      $(this).toggleClass('active');
    });

    // Hide block
    $('[data-toggle="block-hide"]').on('click', function () {
      $(this).closest('.block').fadeOut();
    });
  }

  $.fn.updateCalendarEvents = function (calendar_events) {
    var events = [];
    for (var i = 0; i < calendar_events.length; i++) {
      var event = new Object();
      event.title = calendar_events[i].title;
      event.start = new Date(calendar_events[i].start * 1000);
      event.end = new Date(calendar_events[i].end * 1000);
      event.allDay = false;
      if (calendar_events[i].allDay === 'true') {
        new_event_end = parseInt(calendar_events[i].end) + (60 * 60 * 24);
        event.end = new Date(new_event_end * 1000);
        event.allDay = true;
      }
      event.url = calendar_events[i].url;
      event.className = calendar_events[i].class;

      if (calendar_events[i].color != 'undefined') {
        event.color = calendar_events[i].color;
      }
      if ('tooltip_title' in calendar_events[i]) {
        event.tooltip_title = calendar_events[i].tooltip_title;
      }
      if ('tooltip_content' in calendar_events[i]) {
        event.tooltip_content = calendar_events[i].tooltip_content;
      }
      events.push(event);
    }

    $('#calendar').fullCalendar('removeEvents');
    $('#calendar').fullCalendar('addEventSource', events);
    $('#calendar').fullCalendar('rerenderEvents');
  };

  $.fn.collabScrollLast = function () {
    var scrollToVal = $('.collab-content-scroll-wrapper').scrollTop() + $('.collab-posts li.collab-post-entry:last').position().top;
    $('.collab-content-scroll-wrapper').slimScroll({scrollTo: scrollToVal + 'px'});
  };

  $.fn.updateApprovalQueueCalendarEvents = function (calendar_events) {
    var active_class = '.approval-queue-calendar-nav, .approval-queue-calendar-tab';
    if ($('.approval-queue-list-nav').hasClass('active')) {
      active_class = '.approval-queue-list-nav, .approval-queue-list-tab';
    }

    $('.approval-queue-calendar-nav, .approval-queue-list-nav, .approval-queue-calendar-tab, .approval-queue-list-tab').removeClass('active');
    $('.approval-queue-calendar-nav').addClass('active');
    $('.approval-queue-calendar-tab').addClass('active');

    var events = [];
    for (var i = 0; i < calendar_events.length; i++) {
      var event = new Object();
      event.title = calendar_events[i].title;
      event.start = new Date(calendar_events[i].start * 1000);
      event.end = new Date(calendar_events[i].end * 1000);
      event.allDay = false;
      if (calendar_events[i].allDay === 'true') {
        new_event_end = parseInt(calendar_events[i].end) + (60 * 60 * 24);
        event.end = new Date(new_event_end * 1000);
        event.allDay = true;
      }
      event.url = calendar_events[i].url;
      event.className = calendar_events[i].class;

      if (calendar_events[i].color != 'undefined') {
        event.color = calendar_events[i].color;
      }
      events.push(event);
    }

    $('#calendar').fullCalendar('removeEvents');
    $('#calendar').fullCalendar('addEventSource', events);
    $('#calendar').fullCalendar('rerenderEvents');

    $('.approval-queue-calendar-nav, .approval-queue-list-nav, .approval-queue-calendar-tab, .approval-queue-list-tab').removeClass('active');
    $(active_class).addClass('active');
  };

  $.fn.notifyMessage = function (message, type, title, icon) {
    title = title || '';
    icon = icon || '';
    $.notify({
      // options
      icon: icon,
      title: title,
      message: message
    }, {
      // settings
      element: 'body',
      type: type,
      showProgressbar: true,
      offset: 30,
      spacing: 10,
      delay: 7000,
      timer: 50,
      mouse_over: 'pause',
      animate: {
        enter: 'animated bounceInRight',
        exit: 'animated bounceOutRight'
      },
      icon_type: 'class',
      template: '<div data-notify="container" class="custom-alert alert alert-{0}" role="alert">' +
              '<div class="progress" data-notify="progressbar">' +
              '<div class="progress-bar progress-bar-{0}" role="progressbar" aria-valuenow="0" aria-valuemin="0" aria-valuemax="100" style="width: 0%;"></div>' +
              '</div>' +
              '<div class="custom-alert-content">' +
              '<button type="button" aria-hidden="true" class="close" data-notify="dismiss">×</button>' +
              '<span data-notify="icon"></span> ' +
              '<span data-notify="title">{1}</span> ' +
              '<span class="message" data-notify="message">{2}</span>' +
              '<a href="{3}" target="{4}" data-notify="url"></a>' +
              '</div>' +
              '</div>'
    });

  };

  $.fn.add_commas = function (Num) { //function to add commas to textboxes
    Num += '';
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    Num = Num.replace(',', '');
    x = Num.split('.');
    x1 = x[0];
    x2 = x.length > 1 ? '.' + x[1] : '.' + '00';
    if (x2.length === 2) {
      x2 = x2 + '0';
    }

    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1))
      x1 = x1.replace(rgx, '$1' + ',' + '$2');
    return x1 + x2;
  }

  jQuery.validator.addMethod("dateGreaterThan", function (value, element, params) {
    if ($(params[0]).val() != '') {
      if (!/Invalid|NaN/.test(new Date(value))) {
        return new Date(value) > new Date($(params[0]).val());
      }
      return isNaN(value) && isNaN($(params[0]).val()) || (Number(value) > Number($(params[0]).val()));
    }
    ;
    return true;
  }, 'Must be greater than {1}.');

  jQuery.validator.addMethod("greaterThan", function (value, element, param) {
    var $min = $(param);
    if (this.settings.onfocusout) {
      $min.off(".validate-greaterThan").on("blur.validate-greaterThan", function () {
        $(element).valid();
      });
    }
    return parseInt(value) > parseInt($min.val());

  }, "Max must be greater than min");

  jQuery.validator.addMethod("oneAtLeastRequired", function (value, element, params) {
    flag = false;
    for (i = 0; i < params.length; i++) {
      if ($(params[i]).val() !== '') {
        flag = true;
      }
    }
    return flag;
  }, 'One element must be required');

  jQuery.validator.addMethod("checkboxOneAtLeastRequired", function (value, element, params) {
    flag = false;
    for (i = 0; i < params.length; i++) {
      if ($(params[i]).is(':checked')) {
        flag = true;
      }
    }
    return flag;
  }, 'One element must be required');

  jQuery.validator.addMethod('filesize', function (value, element, param) {
    max_size = param * 1024 * 1024;
    return this.optional(element) || (element.files[0].size <= max_size)
  }, 'File size must be less than {0} MB');


})(jQuery, Drupal, this, this.document);