<script>
  (function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.med_behavior_timeline = {
      attach: function (context, settings) {
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

      }
    };
    $(document).ready(function () {


      tids = '<?php echo $variables['tids']; ?>';
      offset = <?php echo $variables['offset']; ?>;
      range = <?php echo $variables['range']; ?>;

      isPreviousEventComplete = true;
      isDataAvailable = true;
      loadMore = <?php echo $variables['load_more']; ?>;

      var timeline_data = <?php echo json_encode($variables['items']); ?>;
      var timeline = new Timeline($('#timeline'), timeline_data);
      timeline.setOptions({
        animation: true,
        lightbox: true,
        showMonth: true,
        separator: false,
        columnMode: 'dual',
        order: 'desc',
        responsive_width: 800
      });
      timeline.display();

      if (loadMore) {
        $(window).scroll(function () {
          if ($(document).height() - 100 <= $(window).scrollTop() + $(window).height()) {
            if (isPreviousEventComplete && isDataAvailable) {
              isPreviousEventComplete = false;

              offset = offset + range;
              $.ajax({
                url: Drupal.settings.basePath + "dashboard/timeline/load-more",
                type: "post",

                data: {'tids': tids, 'offset': offset, 'range': range},
                dataType: "json",
                success: function (data) {
                  timeline.appendData(data);

                  isPreviousEventComplete = true;
                  if (data == '') {
                    isDataAvailable = false;
                    $('.timeline_loadmore').css("display", "none");
                  }

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
                }
              });
            }
          }
        });
      } else {
        $('.timeline_loadmore').css("display", "none");
      }

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
      var myLazyLoad = new LazyLoad({
        elements_selector: ".timeline-lazy"
      });
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div id="timeline" style="width:100%;margin:auto;"></div>
<div id="loadmore" class="timeline_loadmore loading">
  <span class="loadmore-loading"></span>
</div>
