<?php
$events = $variables['events'];
$first_day = $variables['first_day'];
?>

<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior = {
      attach: function (context, settings) {
      }
    };
    $(document).ready(function () {
      $('ul.nav-tabs li a').once().click(function (event) {
        $('#calendar').fullCalendar('render');
      });

      calendar_events = <?php echo json_encode($events); ?>;
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
        if('tooltip_content' in calendar_events[i]){
          event.tooltip_content = calendar_events[i].tooltip_content;
        }

        events.push(event);
      }      

      $('#calendar').fullCalendar({
        header: {
          left: 'prev,next',
          center: 'title',
          right: 'month,agendaWeek,agendaDay'
        },
        firstDay: <?php echo $first_day; ?>,
        editable: false,
        droppable: false,
        events: events,
        eventRender: function (event, $element) {
          $element.find('.fc-title').html(event.title);
          
          $element.popover({
            html: true,
            title: event.tooltip_title,
            content: event.tooltip_content,
            trigger: 'hover',
            placement: 'top',
            container: 'body'
          });
        },
        eventClick: function (event) {
          if (event.url) {
            window.open(event.url);
            return false;
          }
        },
        eventAfterAllRender: function (event) {
        }
      });
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div id="calendar"></div>

