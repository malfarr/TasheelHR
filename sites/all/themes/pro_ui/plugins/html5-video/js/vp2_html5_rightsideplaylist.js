/*
 * HTML5 Video Player with RightSide Playlist v4.5.1
 *
 * Copyright 2012-2015, LambertGroup
 * 
 */

(function ($) {
  $.fn.vp2_html5_rightSidePlaylist_Video = function (options) {
    var videoID;
    //fullscreen vars
    var bodyOrigMargin;
    var bodyOrigOverflow;
    //video
    var videoIsFullScreen = false;
    var videoOrigWidth;
    var videoOrigHeight;
    var videoOrigPosition;
    var videoOrigLeft;
    var videoOrigTop;
    //border
    var videoBorderOrigPosition;
    var videoBorderOrigZIndex;
    //video container
    var videoContainerOrigWidth;
    var videoContainerOrigHeight;
    var videoContainerOrigPosition;
    var videoContainerOrigLeft;
    var videoContainerOrigTop;
    //controllers
    var videoControllersOrigPosition;
    var videoControllersOrigBottom;
    //info
    var infoBoxAdjust = 40;
    var infoBoxOrigPosition;

    var videoIsShowHideRunning = false;

    //timer
    var curTime;
    var totalTime;
    var totalTimeInterval;

    // the skins		
    var skins = {
      skin: 'universalBlack',
      initialVolume: 1,
      showInfo: true,
      autoPlayFirstMovie: false,
      autoPlay: true,
      loop: true,
      autoHideControllers: true,
      seekBarAdjust: 255,
      borderWidth: 15,
      borderColor: '#e9e9e9',
      playlistWidth: 300,
      playlistCharactersLimit: 100,
      numberOfThumbsPerScreen: 4,
      isSliderInitialized: false,
      isProgressInitialized: false,
      responsive: true,
      responsiveRelativeToBrowser: false,
      width: 0, //hidden, used for responsive
      height: 0, //hidden, used for responsive
      width100Proc: false,
      height100Proc: false,
      setOrigWidthHeight: true,
      origWidth: 0,
      origHeight: 0,
      origEntirePlayerWidth: 0,
      origPlayerWidthProc: 0,
      origPlaylistWidthProc: 0,
      origThumbW: 0,
      origThumbH: 0,
      origThumbImgW: 75,
      origThumbImgH: 75,
      origthumbLeftPadding: 0,
      origthumbRightPadding: 0,
      origthumbTopPadding: 0,
      origthumbBottomPadding: 0,
      origthumbTitleFont: 0,
      origthumbRegFont: 0,
      origthumbTitleLineHeight: 0,
      origthumbRegLineHeight: 0,
      origthumbsHolder_MarginTop: 0,
      windowOrientationScreenSize0: 0,
      windowOrientationScreenSize90: 0,
      windowOrientationScreenSize_90: 0,
      windowCurOrientation: 0
    };
    var options = $.extend(skins, options);

    function getInternetExplorerVersion() {
      // -1 - not IE
      // 7,8,9 etc
      var rv = -1; // Return value assumes failure.
      if (navigator.appName == 'Microsoft Internet Explorer')
      {
        var ua = navigator.userAgent;
        var re = new RegExp("MSIE ([0-9]{1,}[\.0-9]{0,})");
        if (re.exec(ua) != null)
          rv = parseFloat(RegExp.$1);
      }
      else if (navigator.appName == 'Netscape')
      {
        var ua = navigator.userAgent;
        var re = new RegExp("Trident/.*rv:([0-9]{1,}[\.0-9]{0,})");
        if (re.exec(ua) != null)
          rv = parseFloat(RegExp.$1);
      }
      return parseInt(rv, 10);
    }
    var ver_ie = getInternetExplorerVersion();

    return this.each(function () {
      var vp2_html5_rightSidePlaylist_Video = $(this);
      //alert (vp2_html5_rightSidePlaylist_Video.css("z-index"));
      videoID = vp2_html5_rightSidePlaylist_Video.attr('id');
      //alert (videoID);

      var current_obj = {
        windowWidth: 0,
        thumbMarginTop: 0,
        newPlaylistWidth: 0
      };
      current_obj.newPlaylistWidth = options.playlistWidth;

      //the controllers
      var vp2_html5_rightSidePlaylist_controls = $('<div class="VideoControls"><a class="VideoRewind" title="Rewind"></a><a class="VideoPlay" title="Play/Pause"></a><div class="VideoBuffer"></div><div class="VideoSeek"></div><a class="VideoShowHidePlaylist" title="Show/Hide Playlist"></a><a class="VideoInfoBut" title="Info"></a><div class="VideoTimer">00:00</div><div class="VolumeAll"><div class="VolumeSlider"></div><a class="VolumeButton" title="Mute/Unmute"></a></div><a class="VideoFullScreen" title="FullScreen"></a></div> <div class="VideoInfoBox"></div>    <div class="thumbsHolderWrapper"><div class="thumbsHolderVisibleWrapper"><div class="thumbsHolder"></div></div></div>  <div class="slider-vertical"></div>    </div>');

      //the elements
      var vp2_html5_rightSidePlaylist_container = vp2_html5_rightSidePlaylist_Video.parent('.vp2_html5_rightSidePlaylist');
      var vp2_html5_rightSidePlaylist_border = vp2_html5_rightSidePlaylist_container.parent('.vp2_html5_rightSidePlaylistBorder');

      vp2_html5_rightSidePlaylist_container.addClass(options.skin);
      vp2_html5_rightSidePlaylist_container.append(vp2_html5_rightSidePlaylist_controls);

      var vp2_html5_rightSidePlaylist_controls = $('.VideoControls', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_info_box = $('.VideoInfoBox', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_rewind_btn = $('.VideoRewind', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_play_btn = $('.VideoPlay', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_Video_buffer = $('.VideoBuffer', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_Video_seek = $('.VideoSeek', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_info_btn = $('.VideoInfoBut', vp2_html5_rightSidePlaylist_container);
      if (!options.showInfo)
        vp2_html5_rightSidePlaylist_info_btn.addClass("hideElement");
      var vp2_html5_rightSidePlaylist_showHidePlaylist_btn = $('.VideoShowHidePlaylist', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_Video_timer = $('.VideoTimer', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_volumeAll = $('.VolumeAll', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_volume = $('.VolumeSlider', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_volume_btn = $('.VolumeButton', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_fullscreen_btn = $('.VideoFullScreen', vp2_html5_rightSidePlaylist_container);

      //set controllers size
      vp2_html5_rightSidePlaylist_controls.width(vp2_html5_rightSidePlaylist_Video.width());

      //set border size
      vp2_html5_rightSidePlaylist_border.width(vp2_html5_rightSidePlaylist_Video.width() + 3 * options.borderWidth + current_obj.newPlaylistWidth);
      vp2_html5_rightSidePlaylist_border.height(vp2_html5_rightSidePlaylist_Video.height() + 2 * options.borderWidth);
      vp2_html5_rightSidePlaylist_border.css("background", options.borderColor);

      vp2_html5_rightSidePlaylist_container.css({
        'width': vp2_html5_rightSidePlaylist_Video.width() + 'px',
        'height': vp2_html5_rightSidePlaylist_Video.height() + 'px',
        'top': options.borderWidth + 'px',
        'left': options.borderWidth + 'px'
      });
      /*vp2_html5_rightSidePlaylist_container.css('top',options.borderWidth+'px');
       vp2_html5_rightSidePlaylist_container.css('left',options.borderWidth+'px');*/

      //set seekbar width
      vp2_html5_rightSidePlaylist_Video_seek.css('width', vp2_html5_rightSidePlaylist_Video[0].offsetWidth - options.seekBarAdjust + 'px');
      vp2_html5_rightSidePlaylist_Video_buffer.css('width', vp2_html5_rightSidePlaylist_Video_seek.css('width'));

      //set info box
      vp2_html5_rightSidePlaylist_info_box.css('width', vp2_html5_rightSidePlaylist_Video[0].offsetWidth - infoBoxAdjust + 'px');


      vp2_html5_rightSidePlaylist_controls.hide(); // the controls are still hidden


      /*original values start*/
      options.origWidth = document.getElementById(videoID).offsetWidth;
      options.origHeight = document.getElementById(videoID).offsetHeight;
      options.width = options.origWidth;
      options.height = options.origHeight;
      options.origEntirePlayerWidth = options.origWidth + 3 * options.borderWidth + current_obj.newPlaylistWidth;
      options.origPlayerWidthProc = parseInt(options.origWidth * 100 / (options.origEntirePlayerWidth - 3 * options.borderWidth), 10);
      options.origPlaylistWidthProc = 100 - options.origPlayerWidthProc - 5;

      videoOrigPosition = vp2_html5_rightSidePlaylist_Video.css('position');
      videoOrigLeft = vp2_html5_rightSidePlaylist_Video.css('left');
      videoOrigTop = vp2_html5_rightSidePlaylist_Video.css('top');

      videoContainerOrigWidth = vp2_html5_rightSidePlaylist_container[0].offsetWidth;
      videoContainerOrigHeight = vp2_html5_rightSidePlaylist_container[0].offsetHeight;
      videoContainerOrigPosition = vp2_html5_rightSidePlaylist_container.css('position');
      videoContainerOrigLeft = vp2_html5_rightSidePlaylist_container.css('left');
      videoContainerOrigTop = vp2_html5_rightSidePlaylist_container.css('top');

      infoBoxOrigPosition = vp2_html5_rightSidePlaylist_info_box.css('position');

      videoControllersOrigPosition = vp2_html5_rightSidePlaylist_controls.css('position');

      videoBorderOrigPosition = vp2_html5_rightSidePlaylist_border.css('position');
      videoBorderOrigZIndex = vp2_html5_rightSidePlaylist_border.css('z-index');
      videoContainerOrigZIndex = vp2_html5_rightSidePlaylist_container.css('z-index');
      videoControllersOrigBottom = vp2_html5_rightSidePlaylist_controls.css('bottom');

      infoBoxOrigPosition = vp2_html5_rightSidePlaylist_info_box.css('position');
      if (options.responsive && (options.width100Proc || options.height100Proc)) {
        options.setOrigWidthHeight = false;
      }

      //body
      /*bodyOrigMargin=$("body").css("margin");
       if (ver_ie!=-1) {
       bodyOrigOverflow=$("html").css("overflow");
       } else {
       bodyOrigOverflow=$("body").css("overflow");
       }*/
      bodyOrigOverflow = $("html").css("overflow");
      //alert (bodyOrigOverflow);				

      vp2_html5_rightSidePlaylist_controls.css('width', '100%');

      var supports_h264_baseline_video = function () {
        var v = document.getElementById(videoID);
        return v.canPlayType('video/mp4; codecs="avc1.42E01E, mp4a.40.2"');
      }

      var detectBrowserAndVideo = function () {
        //activate current
        $(thumbsHolder_Thumbs[current_img_no]).addClass('thumbsHolder_ThumbON');
        //auto scroll carousel if needed
        carouselScroll();

        var currentVideo = playlist_arr[current_img_no]['sources_webm'];
        var val = navigator.userAgent.toLowerCase();


        if (val.indexOf("chrome") != -1 || val.indexOf("msie") != -1 || val.indexOf("safari") != -1 || val.indexOf("android") != -1)
          currentVideo = playlist_arr[current_img_no]['sources_mp4'];

        //if (val.match(/(iPad)|(iPhone)|(iPod)|(webOS)/i))
        if (val.indexOf("ipad") != -1 || val.indexOf("iphone") != -1 || val.indexOf("ipod") != -1 || val.indexOf("webos") != -1)
          currentVideo = playlist_arr[current_img_no]['sources_mp4'];

        if (val.indexOf("android") != -1)
          currentVideo = playlist_arr[current_img_no]['sources_mp4'];


        if (ver_ie != -1) {
          currentVideo = playlist_arr[current_img_no]['sources_mp4'];
        }

        if (val.indexOf("opera") != -1) {
          currentVideo = playlist_arr[current_img_no]['sources_webm'];
          if (supports_h264_baseline_video() != '') {
            currentVideo = playlist_arr[current_img_no]['sources_mp4'];
          }
        }

        if (val.indexOf("opr/") != -1) {
          currentVideo = playlist_arr[current_img_no]['sources_webm'];
          if (supports_h264_baseline_video() != '') {
            currentVideo = playlist_arr[current_img_no]['sources_mp4'];
          }
        }


        if (val.indexOf("firefox") != -1 || val.indexOf("mozzila") != -1) {
          currentVideo = playlist_arr[current_img_no]['sources_webm'];
          if (supports_h264_baseline_video() != '') {
            currentVideo = playlist_arr[current_img_no]['sources_mp4'];
          }
        }

        //var val = this.dataBrowser;
        //alert (currentVideo+ '  --  ' +val);
        return currentVideo;
      };

      //generate playlist
      var isCarouselScrolling = false;
      var currentCarouselTop = 0;
      var vp2_html5_rightSidePlaylist_thumbsHolderWrapper = $('.thumbsHolderWrapper', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_thumbsHolderVisibleWrapper = $('.thumbsHolderVisibleWrapper', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_thumbsHolder = $('.thumbsHolder', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_sliderVertical = $('.slider-vertical', vp2_html5_rightSidePlaylist_container);
      var vp2_html5_rightSidePlaylist_paddingDiv;
      var vp2_html5_rightSidePlaylist_titleDiv;
      var vp2_html5_rightSidePlaylist_regDiv;

      vp2_html5_rightSidePlaylist_thumbsHolderVisibleWrapper.width(current_obj.newPlaylistWidth);
      vp2_html5_rightSidePlaylist_thumbsHolderVisibleWrapper.height(vp2_html5_rightSidePlaylist_container.height());
      //vp2_html5_rightSidePlaylist_thumbsHolderVisibleWrapper.css('top',0);

      vp2_html5_rightSidePlaylist_thumbsHolderWrapper.width(current_obj.newPlaylistWidth);
      vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height(vp2_html5_rightSidePlaylist_container.height());
      vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css("top", 0);
      vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css("left", +vp2_html5_rightSidePlaylist_Video[0].offsetWidth + options.borderWidth + 'px');

      vp2_html5_rightSidePlaylist_thumbsHolder.width(current_obj.newPlaylistWidth);
      vp2_html5_rightSidePlaylist_thumbsHolder.css('top', options.borderWidth + 'px');

      //var thumbMarginTop=0;
      var current_img_no = 0;
      var total_images = 0;
      var playlist_arr = new Array();
      var thumbsHolder_Thumb;

      var playlistElements = $('.xplaylist', vp2_html5_rightSidePlaylist_container).children();
      playlistElements.each(function () { // ul-s
        currentElement = $(this);
        total_images++;
        playlist_arr[total_images - 1] = new Array();
        playlist_arr[total_images - 1]['title'] = '';
        playlist_arr[total_images - 1]['desc'] = '';
        playlist_arr[total_images - 1]['thumb'] = '';
        playlist_arr[total_images - 1]['preview'] = '';
        playlist_arr[total_images - 1]['xsources_mp4'] = '';
        playlist_arr[total_images - 1]['xsources_ogv'] = '';
        playlist_arr[total_images - 1]['xsources_webm'] = '';
        playlist_arr[total_images - 1]['xsources_mp4v'] = '';

        //alert (currentElement.find('.xdesc').html())
        if (currentElement.find('.xtitle').html() != null) {
          playlist_arr[total_images - 1]['title'] = currentElement.find('.xtitle').html();
        }

        if (currentElement.find('.xdesc').html() != null) {
          playlist_arr[total_images - 1]['desc'] = currentElement.find('.xdesc').html();
        }

        if (currentElement.find('.xfull_desc').html() != null) {
          playlist_arr[total_images - 1]['full_desc'] = currentElement.find('.xfull_desc').html();
        }

        if (currentElement.find('.xthumb').html() != null) {
          playlist_arr[total_images - 1]['thumb'] = currentElement.find('.xthumb').html();
        }

        if (currentElement.find('.xpreview').html() != null) {
          playlist_arr[total_images - 1]['preview'] = currentElement.find('.xpreview').html();
        }


        if (currentElement.find('.xsources_mp4').html() != null) {
          playlist_arr[total_images - 1]['sources_mp4'] = currentElement.find('.xsources_mp4').html();
        }

        if (currentElement.find('.xsources_ogv').html() != null) {
          playlist_arr[total_images - 1]['sources_ogv'] = currentElement.find('.xsources_ogv').html();
        }

        if (currentElement.find('.xsources_webm').html() != null) {
          playlist_arr[total_images - 1]['sources_webm'] = currentElement.find('.xsources_webm').html();
        }

        if (currentElement.find('.xsources_mp4v').html() != null) {
          playlist_arr[total_images - 1]['sources_mp4v'] = currentElement.find('.xsources_mp4v').html();
        }

        desc_aux = String(playlist_arr[total_images - 1]['desc']);
        //substr(0,options.playlistCharactersLimit);
        if (desc_aux.length > options.playlistCharactersLimit)
          desc_aux = desc_aux.substr(0, desc_aux.indexOf(' ', options.playlistCharactersLimit)) + '...';

        thumbsHolder_Thumb = $('<div class="thumbsHolder_ThumbOFF" rel="' + (total_images - 1) + '"><div class="padding"><img src="' + playlist_arr[total_images - 1]['thumb'] + '"><span class="title">' + playlist_arr[total_images - 1]['title'] + '</span><br><span class="reg">' + desc_aux + '</span></div></div>');
        vp2_html5_rightSidePlaylist_thumbsHolder.append(thumbsHolder_Thumb);


        if (options.origThumbW == 0) {

          if (options.numberOfThumbsPerScreen == 0) {
            options.numberOfThumbsPerScreen = Math.floor((options.origHeight) / thumbsHolder_Thumb.height());
          }
          options.origThumbW = thumbsHolder_Thumb.width();
          options.origThumbH = thumbsHolder_Thumb.height();

          vp2_html5_rightSidePlaylist_paddingDiv = $('.thumbsHolder_ThumbOFF .padding', vp2_html5_rightSidePlaylist_container);
          options.origthumbLeftPadding = vp2_html5_rightSidePlaylist_paddingDiv.css('padding-left').substr(0, vp2_html5_rightSidePlaylist_paddingDiv.css('padding-left').lastIndexOf('px'));
          options.origthumbRightPadding = vp2_html5_rightSidePlaylist_paddingDiv.css('padding-right').substr(0, vp2_html5_rightSidePlaylist_paddingDiv.css('padding-left').lastIndexOf('px'));
          options.origthumbTopPadding = vp2_html5_rightSidePlaylist_paddingDiv.css('padding-top').substr(0, vp2_html5_rightSidePlaylist_paddingDiv.css('padding-left').lastIndexOf('px'));
          options.origthumbBottomPadding = vp2_html5_rightSidePlaylist_paddingDiv.css('padding-bottom').substr(0, vp2_html5_rightSidePlaylist_paddingDiv.css('padding-left').lastIndexOf('px'));
          current_obj.thumbMarginTop = Math.floor((vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - thumbsHolder_Thumb.height() * options.numberOfThumbsPerScreen) / (options.numberOfThumbsPerScreen - 1));

          vp2_html5_rightSidePlaylist_titleDiv = $('.thumbsHolder_ThumbOFF .title', vp2_html5_rightSidePlaylist_container);
          vp2_html5_rightSidePlaylist_regDiv = $('.thumbsHolder_ThumbOFF .reg', vp2_html5_rightSidePlaylist_container);

          if (vp2_html5_rightSidePlaylist_titleDiv.css('font-size').lastIndexOf('px') != -1) {
            options.origthumbTitleFont = vp2_html5_rightSidePlaylist_titleDiv.css('font-size').substr(0, vp2_html5_rightSidePlaylist_titleDiv.css('font-size').lastIndexOf('px'));
          } else if (vp2_html5_rightSidePlaylist_titleDiv.css('font-size').lastIndexOf('em') != -1) {
            options.origthumbTitleFont = vp2_html5_rightSidePlaylist_titleDiv.css('font-size').substr(0, vp2_html5_rightSidePlaylist_titleDiv.css('font-size').lastIndexOf('em'));
          }
          if (vp2_html5_rightSidePlaylist_titleDiv.css('line-height').lastIndexOf('px') != -1) {
            options.origthumbTitleLineHeight = vp2_html5_rightSidePlaylist_titleDiv.css('line-height').substr(0, vp2_html5_rightSidePlaylist_titleDiv.css('line-height').lastIndexOf('px'));
          } else if (vp2_html5_rightSidePlaylist_titleDiv.css('line-height').lastIndexOf('em') != -1) {
            options.origthumbTitleLineHeight = vp2_html5_rightSidePlaylist_titleDiv.css('line-height').substr(0, vp2_html5_rightSidePlaylist_titleDiv.css('line-height').lastIndexOf('em'));
          }


          if (vp2_html5_rightSidePlaylist_regDiv.css('font-size').lastIndexOf('px') != -1) {
            options.origthumbRegFont = vp2_html5_rightSidePlaylist_regDiv.css('font-size').substr(0, vp2_html5_rightSidePlaylist_regDiv.css('font-size').lastIndexOf('px'));
          } else if (vp2_html5_rightSidePlaylist_regDiv.css('font-size').lastIndexOf('em') != -1) {
            options.origthumbRegFont = vp2_html5_rightSidePlaylist_regDiv.css('font-size').substr(0, vp2_html5_rightSidePlaylist_regDiv.css('font-size').lastIndexOf('em'));
          }
          if (vp2_html5_rightSidePlaylist_regDiv.css('line-height').lastIndexOf('px') != -1) {
            options.origthumbRegLineHeight = vp2_html5_rightSidePlaylist_regDiv.css('line-height').substr(0, vp2_html5_rightSidePlaylist_regDiv.css('line-height').lastIndexOf('px'));
          } else if (vp2_html5_rightSidePlaylist_regDiv.css('line-height').lastIndexOf('em') != -1) {
            options.origthumbRegLineHeight = vp2_html5_rightSidePlaylist_regDiv.css('line-height').substr(0, vp2_html5_rightSidePlaylist_regDiv.css('line-height').lastIndexOf('em'));
          }


        }


        //activate first
        if (total_images === 1)
          thumbsHolder_Thumb.addClass('thumbsHolder_ThumbON');

        current_obj.thumbMarginTop = Math.floor((vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - thumbsHolder_Thumb.height() * options.numberOfThumbsPerScreen) / (options.numberOfThumbsPerScreen - 1));
        //alert(vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height());
        vp2_html5_rightSidePlaylist_thumbsHolder.css('height', vp2_html5_rightSidePlaylist_thumbsHolder.height() + current_obj.thumbMarginTop + thumbsHolder_Thumb.height() + 'px');
        if (total_images <= 1) {
          thumbsHolder_Thumb.css('margin-top', Math.floor((vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - 2 * options.borderWidth - (current_obj.thumbMarginTop + thumbsHolder_Thumb.height()) * (options.numberOfThumbsPerScreen - 1) - thumbsHolder_Thumb.height()) / 2) + 'px');
        } else {
          thumbsHolder_Thumb.css('margin-top', current_obj.thumbMarginTop + 'px');
        }


        thumbsHolder_MarginTop = parseInt((vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - parseInt(thumbsHolder_Thumb.css('height').substring(0, thumbsHolder_Thumb.css('height').length - 2))) / 2);


      });

      vp2_html5_rightSidePlaylist_paddingDiv = $('.thumbsHolder_ThumbOFF .padding', vp2_html5_rightSidePlaylist_container);
      vp2_html5_rightSidePlaylist_titleDiv = $('.thumbsHolder_ThumbOFF .title', vp2_html5_rightSidePlaylist_container);
      vp2_html5_rightSidePlaylist_regDiv = $('.thumbsHolder_ThumbOFF .reg', vp2_html5_rightSidePlaylist_container);

      //the scroller
      if (total_images > options.numberOfThumbsPerScreen) {
        vp2_html5_rightSidePlaylist_sliderVertical.slider({
          orientation: "vertical",
          range: "min",
          min: 1,
          max: 100,
          step: 1,
          value: 100,
          slide: function (event, ui) {
            //alert( ui.value );
            carouselScroll(ui.value);
          }
        });
        vp2_html5_rightSidePlaylist_sliderVertical.css('display', 'inline');
        vp2_html5_rightSidePlaylist_sliderVertical.css('position', 'absolute');
        vp2_html5_rightSidePlaylist_sliderVertical.height(vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - 25); // 25 is the height of  .slider-vertical.ui-slider .ui-slider-handle 
        vp2_html5_rightSidePlaylist_sliderVertical.css('left', Math.floor(vp2_html5_rightSidePlaylist_Video[0].offsetWidth + options.borderWidth + current_obj.newPlaylistWidth + (options.borderWidth - vp2_html5_rightSidePlaylist_sliderVertical.width()) / 2) + 'px');
        vp2_html5_rightSidePlaylist_sliderVertical.css('top', 0 + 'px');
        //vp2_html5_rightSidePlaylist_sliderVertical.css('left',Math.floor(vp2_html5_rightSidePlaylist_Video[0].offsetWidth)+2+'px');
      }

      var carouselScroll = function (direction) {
        //var MAX_TOP=(thumbsHolder_Thumb.height()+current_obj.thumbMarginTop)*(total_images-1);
        var MAX_TOP = (thumbsHolder_Thumb.height() + current_obj.thumbMarginTop) * (total_images - options.numberOfThumbsPerScreen);
        var new_top = 0;
        //alert (vp2_html5_rightSidePlaylist_sliderVertical.slider( "option", "animate" ));
        vp2_html5_rightSidePlaylist_thumbsHolder.stop(true, true);
        if (direction && !isCarouselScrolling) {

          isCarouselScrolling = true;
          //vp2_html5_rightSidePlaylist_thumbsHolder.css('opacity','0.5');
          if (direction <= 1)
            direction = 0;
          new_top = parseInt(options.borderWidth + MAX_TOP * (direction - 100) / 100);
          if (new_top > options.borderWidth) {
            new_top = options.borderWidth;
          }


          vp2_html5_rightSidePlaylist_thumbsHolder.animate({
            //opacity: 1,
            top: new_top + 'px'
          }, 1100, 'easeOutQuad', function () {
            // Animation complete.
            isCarouselScrolling = false;
          });
        } else if (!isCarouselScrolling && total_images > options.numberOfThumbsPerScreen) {
          isCarouselScrolling = true;
          vp2_html5_rightSidePlaylist_thumbsHolder.css('opacity', '0.5');
          //var new_top=parseInt(options.borderWidth-(thumbsHolder_Thumb.height()+current_obj.thumbMarginTop)*current_img_no);
          var new_top = parseInt(options.borderWidth - (thumbsHolder_Thumb.height() + current_obj.thumbMarginTop) * current_img_no, 10);
          if (Math.abs(new_top) > MAX_TOP) {
            new_top = options.borderWidth - MAX_TOP;
          }

          vp2_html5_rightSidePlaylist_sliderVertical.slider("value", 100 + parseInt(new_top * 100 / MAX_TOP));
          vp2_html5_rightSidePlaylist_thumbsHolder.animate({
            opacity: 1,
            top: new_top + 'px'
          }, 500, 'easeOutCubic', function () {
            // Animation complete.
            isCarouselScrolling = false;
          });
        }
      };

      // mouse wheel
      vp2_html5_rightSidePlaylist_thumbsHolderVisibleWrapper.mousewheel(function (event, delta, deltaX, deltaY) {
        event.preventDefault();
        var currentScrollVal = vp2_html5_rightSidePlaylist_sliderVertical.slider("value");
        //alert (currentScrollVal+' -- '+delta);
        if ((parseInt(currentScrollVal) > 1 && parseInt(delta) == -1) || (parseInt(currentScrollVal) < 100 && parseInt(delta) == 1)) {
          currentScrollVal = currentScrollVal + delta;
          vp2_html5_rightSidePlaylist_sliderVertical.slider("value", currentScrollVal);
          carouselScroll(currentScrollVal);
          //alert (currentScrollVal);
        }

      });


      vp2_html5_rightSidePlaylist_thumbsHolderWrapper.swipe({
        swipeStatus: function (event, phase, direction, distance, duration, fingerCount)
        {
          //$('#logulmeu').html("phase: "+phase+"<br>direction: "+direction+"<br>distance: "+distance);
          if (direction == 'up' || direction == 'down') {
            if (distance != 0) {
              currentScrollVal = vp2_html5_rightSidePlaylist_sliderVertical.slider("value");
              if (direction == "up") {
                currentScrollVal = currentScrollVal - 1.5;
              } else {
                currentScrollVal = currentScrollVal + 1.5;
              }
              vp2_html5_rightSidePlaylist_sliderVertical.slider("value", currentScrollVal);
              //carouselScroll(currentScrollVal,current_obj,options,vp2_html5_rightSidePlaylist_thumbsHolder)
              carouselScroll(currentScrollVal);
            }
          }

          //Here we can check the:
          //phase : 'start', 'move', 'end', 'cancel'
          //direction : 'left', 'right', 'up', 'down'
          //distance : Distance finger is from initial touch point in px
          //duration : Length of swipe in MS 
          //fingerCount : the number of fingers used
        },
        threshold: 100,
        maxTimeThreshold: 500,
        fingers: 'all'
      });

      //tumbs nav
      var thumbsHolder_Thumbs = $('.thumbsHolder_ThumbOFF', vp2_html5_rightSidePlaylist_container);
      thumbsHolder_Thumbs.click(function () {
        var currentBut = $(this);
        var i = currentBut.attr('rel');
        if (current_img_no != i) {
          //alert (current_img_no+'  --  '+i)
          //deactivate previous 
          $(thumbsHolder_Thumbs[current_img_no]).removeClass('thumbsHolder_ThumbON');

          current_img_no = i;
          changeSrcAndPoster(options.autoPlay);
        }
      });

      thumbsHolder_Thumbs.mouseenter(function () {
        var currentBut = $(this);
        var i = currentBut.attr('rel');

        currentBut.addClass('thumbsHolder_ThumbON');
      });

      thumbsHolder_Thumbs.mouseleave(function () {
        var currentBut = $(this);
        var i = currentBut.attr('rel');

        if (current_img_no != i)
          currentBut.removeClass('thumbsHolder_ThumbON');
      });
      thumbsHolder_Thumbs.dblclick(function () {
        var currentBut = $(this);
        var i = currentBut.attr('rel');

        currentBut.addClass('thumbsHolder_ThumbON');
      });

      var changeSrcAndPoster = function (auto_play) {
        //seekbar init
        if (options.isSliderInitialized) {
          vp2_html5_rightSidePlaylist_Video_seek.slider("destroy");
          options.isSliderInitialized = false;
        }

        if (options.isProgressInitialized) {
          vp2_html5_rightSidePlaylist_Video_buffer.progressbar("destroy");
          vp2_html5_rightSidePlaylist_Video.unbind('progress');
          options.isProgressInitialized = false;
        }

        totalTimeInterval = 'Infinity';

        document.getElementById(videoID).poster = playlist_arr[current_img_no]['preview'];
        //info
        vp2_html5_rightSidePlaylist_info_box.html('<p class="movieTitle">' + playlist_arr[current_img_no]['title'] + '</p><p class="movieDesc">' + playlist_arr[current_img_no]['full_desc'] + '</p>');

        document.getElementById(videoID).src = detectBrowserAndVideo();
        document.getElementById(videoID).load();

        var val = navigator.userAgent.toLowerCase();
        if (val.indexOf("ipad") != -1 || val.indexOf("iphone") != -1 || val.indexOf("ipod") != -1 || val.indexOf("webos") != -1 || val.indexOf("android") != -1) {
          if (auto_play) {
            document.getElementById(videoID).play();
            vp2_html5_rightSidePlaylist_play_btn.addClass('VideoPause');
          } else {
            vp2_html5_rightSidePlaylist_play_btn.removeClass('VideoPause');
          }
          generate_seekBar();
        } else {
          clearInterval(totalTimeInterval);
          totalTimeInterval = setInterval(function () {
            //alert (document.getElementById(videoID).readyState);
            //alert (document.getElementById(videoID).duration);
            if (document.getElementById(videoID).readyState > 0 && !isNaN(document.getElementById(videoID).duration) && document.getElementById(videoID).duration != 'Infinity') {
              totalTime = document.getElementById(videoID).duration;
              //alert ("totalTime1: "+totalTime);					
              generate_seekBar();
              if (auto_play) {
                document.getElementById(videoID).play();
                vp2_html5_rightSidePlaylist_play_btn.addClass('VideoPause');
              } else {
                vp2_html5_rightSidePlaylist_play_btn.removeClass('VideoPause');
              }

              clearInterval(totalTimeInterval);
            }
          }, 700);
        }


      };

      //initialize first video
      changeSrcAndPoster(options.autoPlayFirstMovie);

      /* rewind */
      vp2_html5_rightSidePlaylist_rewind_btn.click(function () {
        document.getElementById(videoID).currentTime = 0;
        document.getElementById(videoID).play();
      });
      /* play/pause*/
      var vp2_html5_rightSidePlaylist_PlayPause = function () {
        if (document.getElementById(videoID).paused == false) {
          document.getElementById(videoID).pause();
        } else {
          document.getElementById(videoID).play();
        }
      };

      vp2_html5_rightSidePlaylist_play_btn.click(vp2_html5_rightSidePlaylist_PlayPause);
      vp2_html5_rightSidePlaylist_Video.click(vp2_html5_rightSidePlaylist_PlayPause);

      vp2_html5_rightSidePlaylist_Video.bind('play', function () {
        vp2_html5_rightSidePlaylist_play_btn.addClass('VideoPause');
      });

      vp2_html5_rightSidePlaylist_Video.bind('pause', function () {
        vp2_html5_rightSidePlaylist_play_btn.removeClass('VideoPause');
      });

      var val = navigator.userAgent.toLowerCase();
      if (val.indexOf("ipad") != -1 || val.indexOf("iphone") != -1 || val.indexOf("ipod") != -1 || val.indexOf("webos") != -1) {
        //nothing
      } else {
        //show controllers on mouser over / hide controllers on mouse out
        vp2_html5_rightSidePlaylist_container.mouseover(function () {
          if (options.autoHideControllers) {
            vp2_html5_rightSidePlaylist_controls.show();
          }
        });
        vp2_html5_rightSidePlaylist_container.mouseout(function () {
          if (options.autoHideControllers) {
            if (vp2_html5_rightSidePlaylist_volumeAll.css('height').substring(0, vp2_html5_rightSidePlaylist_volumeAll.css('height').length - 2) < 120) {
              vp2_html5_rightSidePlaylist_controls.hide();
            }
          }
        });
      }
      //play/pause using spacebar
      vp2_html5_rightSidePlaylist_container.keydown(function (evt) {
        if (evt.keyCode == 32)
          vp2_html5_rightSidePlaylist_PlayPause();
      });

      //fullscreen
      var fullScreenOn = function () {
        videoIsFullScreen = true;
        //change button
        vp2_html5_rightSidePlaylist_fullscreen_btn.removeClass('VideoFullScreen');
        vp2_html5_rightSidePlaylist_fullscreen_btn.addClass('VideoFullScreenIn');
        //preserve original/nonFullScreen values
        //body
        //$("body").css("overflow", "hidden");
        //$("html").css("overflow", "hidden");
        $("body").css("margin", 0);
        $(".vp2_html5_rightSidePlaylist").css('display', 'none');
        vp2_html5_rightSidePlaylist_container.css('display', 'block');


        var new_height;
        var new_videoControllersBottom;
        if (vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css('display') != 'none') {
          new_height = window.innerHeight - vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height();
          new_videoControllersBottom = window.innerHeight - new_height + 'px';
        } else {
          new_height = window.innerHeight;
          new_videoControllersBottom = videoControllersOrigBottom;
        }

        //border
        vp2_html5_rightSidePlaylist_border.css('position', 'fixed');
        vp2_html5_rightSidePlaylist_border.css('top', 0);
        vp2_html5_rightSidePlaylist_border.css('left', 0);
        vp2_html5_rightSidePlaylist_border.css('width', window.innerWidth + "px");
        vp2_html5_rightSidePlaylist_border.css('height', window.innerHeight + "px");
        vp2_html5_rightSidePlaylist_border.css('background', "#000000");
        vp2_html5_rightSidePlaylist_border.css('z-index', '10000');

        //container
        vp2_html5_rightSidePlaylist_container.css('position', 'absolute');
        vp2_html5_rightSidePlaylist_container.css('top', 0);
        vp2_html5_rightSidePlaylist_container.css('left', 0);

        //video
        vp2_html5_rightSidePlaylist_Video.css('position', 'fixed');
        if (vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css('display') != 'none') {
          vp2_html5_rightSidePlaylist_Video.css('width', window.innerWidth - 2 * options.borderWidth - options.playlistWidth + "px");
          vp2_html5_rightSidePlaylist_info_box.css('width', window.innerWidth - 2 * options.borderWidth - options.playlistWidth - infoBoxAdjust + 'px');
        } else {
          vp2_html5_rightSidePlaylist_Video.css('width', window.innerWidth + "px");
          vp2_html5_rightSidePlaylist_info_box.css('width', window.innerWidth - infoBoxAdjust + 'px');
        }
        vp2_html5_rightSidePlaylist_Video.css('height', window.innerHeight + "px");
        vp2_html5_rightSidePlaylist_Video.css('top', 0);
        vp2_html5_rightSidePlaylist_Video.css('left', 0);


        //controller
        vp2_html5_rightSidePlaylist_controls.css('position', 'fixed');
        vp2_html5_rightSidePlaylist_controls.css('bottom', videoControllersOrigBottom);
        vp2_html5_rightSidePlaylist_controls.width(vp2_html5_rightSidePlaylist_Video.width());
        vp2_html5_rightSidePlaylist_Video_seek.css('width', vp2_html5_rightSidePlaylist_Video.width() - options.seekBarAdjust + 'px');
        vp2_html5_rightSidePlaylist_Video_buffer.css('width', vp2_html5_rightSidePlaylist_Video_seek.css('width'));
        //info box

        vp2_html5_rightSidePlaylist_info_box.css('position', 'fixed');

        //playlist
        //alert (vp2_html5_rightSidePlaylist_Video[0].offsetWidth+ '   -   '+ vp2_html5_rightSidePlaylist_Video.width())
        vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css("left", +vp2_html5_rightSidePlaylist_Video[0].offsetWidth + options.borderWidth + 'px');
        vp2_html5_rightSidePlaylist_thumbsHolderWrapper.width(options.playlistWidth);
        vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height(window.innerHeight);
        vp2_html5_rightSidePlaylist_thumbsHolderVisibleWrapper.height(window.innerHeight);
        vp2_html5_rightSidePlaylist_sliderVertical.height(vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - 25);
        var aux_val = (options.borderWidth - vp2_html5_rightSidePlaylist_sliderVertical.width()) / 2;
        if (aux_val < 0)
          aux_val = options.borderWidth - vp2_html5_rightSidePlaylist_sliderVertical.width();
        vp2_html5_rightSidePlaylist_sliderVertical.css('left', Math.floor(vp2_html5_rightSidePlaylist_Video[0].offsetWidth + options.borderWidth + options.playlistWidth + aux_val) + 'px');

        vp2_html5_rightSidePlaylist_thumbsHolder.children().each(function () {
          theThumb = $(this);
          theThumb.width(options.playlistWidth);
        });


      };
      var fullScreenOff = function () {
        videoIsFullScreen = false;
        //change button
        vp2_html5_rightSidePlaylist_fullscreen_btn.removeClass('VideoFullScreenIn');
        vp2_html5_rightSidePlaylist_fullscreen_btn.addClass('VideoFullScreen');


        //body
        $("body").css("margin", bodyOrigMargin);
        //$("body").css("overflow", bodyOrigOverflow);
        //$("html").css("overflow", bodyOrigOverflow);
        // alert (bodyOrigOverflow);
        $(".vp2_html5_rightSidePlaylist").css('display', 'block');



        //border
        vp2_html5_rightSidePlaylist_border.css('position', videoBorderOrigPosition);
        vp2_html5_rightSidePlaylist_border.css('width', options.origWidth + 3 * options.borderWidth + current_obj.newPlaylistWidth + "px");
        vp2_html5_rightSidePlaylist_border.css('height', options.origHeight + 2 * options.borderWidth + "px");
        vp2_html5_rightSidePlaylist_border.css('background', options.borderColor);
        vp2_html5_rightSidePlaylist_border.css('z-index', videoBorderOrigZIndex);







        //video
        var new_width;
        vp2_html5_rightSidePlaylist_Video.css('position', videoOrigPosition);
        if (vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css('display') != 'none') {
          new_width = options.origWidth;
        } else {
          new_width = options.origWidth + options.borderWidth + options.playlistWidth;
        }
        vp2_html5_rightSidePlaylist_Video.css('width', new_width + "px");
        vp2_html5_rightSidePlaylist_Video.css('height', options.origHeight + "px");
        vp2_html5_rightSidePlaylist_Video.css('top', videoOrigTop);
        vp2_html5_rightSidePlaylist_Video.css('left', videoOrigLeft);


        //container
        vp2_html5_rightSidePlaylist_container.css('position', videoContainerOrigPosition);
        vp2_html5_rightSidePlaylist_container.css('width', new_width + "px");
        vp2_html5_rightSidePlaylist_container.css('height', options.origHeight + "px");
        vp2_html5_rightSidePlaylist_container.css('top', videoContainerOrigTop);
        vp2_html5_rightSidePlaylist_container.css('left', videoContainerOrigLeft);
        vp2_html5_rightSidePlaylist_container.css('z-index', videoContainerOrigZIndex);

        vp2_html5_rightSidePlaylist_container.css('top', videoContainerOrigTop);
        vp2_html5_rightSidePlaylist_container.css('left', videoContainerOrigLeft);


        //playlist
        vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css("left", +options.origWidth + options.borderWidth + 'px');
        vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height(options.origHeight);
        vp2_html5_rightSidePlaylist_thumbsHolderVisibleWrapper.height(options.origHeight);
        vp2_html5_rightSidePlaylist_sliderVertical.height(options.origHeight - 25);
        vp2_html5_rightSidePlaylist_sliderVertical.css('left', Math.floor(options.origWidth + options.borderWidth + current_obj.newPlaylistWidth + (options.borderWidth - vp2_html5_rightSidePlaylist_sliderVertical.width()) / 2) + 'px');



        //controllers
        vp2_html5_rightSidePlaylist_controls.css('position', videoControllersOrigPosition);
        vp2_html5_rightSidePlaylist_controls.css('bottom', videoControllersOrigBottom);
        vp2_html5_rightSidePlaylist_controls.width(vp2_html5_rightSidePlaylist_Video.width());
        vp2_html5_rightSidePlaylist_Video_seek.css('width', vp2_html5_rightSidePlaylist_Video.width() - options.seekBarAdjust + 'px');
        vp2_html5_rightSidePlaylist_Video_buffer.css('width', vp2_html5_rightSidePlaylist_Video_seek.css('width'));

        //info box
        vp2_html5_rightSidePlaylist_info_box.css('width', new_width - infoBoxAdjust + 'px');
        vp2_html5_rightSidePlaylist_info_box.css('position', infoBoxOrigPosition);


        if (options.responsive) {
          doResize();
        }
      };

      var handleFullScreen = function () {
        if (!videoIsFullScreen) {
          fullScreenOn();
        } else {
          fullScreenOff();
        }
      };

      /*document.onkeydown = function(evt) {
       evt = evt || window.event;
       if (evt.keyCode == 27 && videoIsFullScreen) {
       //alert("Escape");
       fullScreenOff();
       }
       };	*/

      vp2_html5_rightSidePlaylist_container.dblclick(function () {
        if (!videoIsFullScreen) {
          vp2_html5_rightSidePlaylist_fullscreen_btn.click();
        }
      });

      vp2_html5_rightSidePlaylist_fullscreen_btn.click(function () {
        if (screenfull.enabled) {
          vp2_html5_rightSidePlaylist_container.css('display', 'none');
          //screenfull.toggle(document.getElementById( vp2_html5_rightSidePlaylist_container.attr("id") ));
          screenfull.toggle(vp2_html5_rightSidePlaylist_border[0]);
          //screenfull.toggle(document.body);
          screenfull.onchange = function (e) {

            //rearange thumbs area if it was resized	
            if (options.responsive && options.width != options.origWidth && !videoIsFullScreen) {
              options.width = options.origWidth;
              options.height = options.origHeight;

              vp2_html5_rightSidePlaylist_Video.css('width', options.width + "px");
              vp2_html5_rightSidePlaylist_Video.css('height', options.height + "px");
              //container
              vp2_html5_rightSidePlaylist_container.css('width', options.width + "px");
              vp2_html5_rightSidePlaylist_container.css('height', options.height + "px");
              //controller
              vp2_html5_rightSidePlaylist_controls.css('bottom', videoControllersOrigBottom);
              vp2_html5_rightSidePlaylist_Video_seek.css('width', options.width - options.seekBarAdjust + 'px');
              vp2_html5_rightSidePlaylist_Video_buffer.css('width', vp2_html5_rightSidePlaylist_Video_seek.css('width'));
              //info box
              vp2_html5_rightSidePlaylist_info_box.css('width', options.width - infoBoxAdjust + 'px');
              rearangethumbs();
            }



            //alert (screenfull.isFullscreen);
            setTimeout(function () {
              handleFullScreen()
            }, 50);
            //handleFullScreen();
          };
        } else {
          //var ver_iex=getInternetExplorerVersion();
          if ((ver_ie != -1 || val.indexOf("ipad") != -1 || val.indexOf("iphone") != -1 || val.indexOf("ipod") != -1 || val.indexOf("webos") != -1) && options.responsive && options.responsiveRelativeToBrowser && options.width100Proc && options.height100Proc) {
            //nothing
          } else {
            handleFullScreen();
          }
        }

      });

      // timer mouse over
      var is_overTimer = false;
      vp2_html5_rightSidePlaylist_Video_timer.mouseover(function () {
        //alert (videoID+' -- '+document.getElementById(videoID).currentTime+' -- '+document.getElementById(videoID).duration);
        is_overTimer = true;
        curTime = document.getElementById(videoID).currentTime;
        totalTime = document.getElementById(videoID).duration;
        vp2_html5_rightSidePlaylist_Video_timer.text('-' + vp2_html5_rightSidePlaylist_FormatTime(totalTime - curTime));
      });
      vp2_html5_rightSidePlaylist_Video_timer.mouseout(function () {
        is_overTimer = false;
        curTime = document.getElementById(videoID).currentTime;
        totalTime = document.getElementById(videoID).duration;
        vp2_html5_rightSidePlaylist_Video_timer.text(vp2_html5_rightSidePlaylist_FormatTime(curTime));
      });

      var is_seeking = false;
      function generate_seekBar() {
        //alert ("gen");
        if (document.getElementById(videoID).readyState) {
          totalTime = document.getElementById(videoID).duration;
          vp2_html5_rightSidePlaylist_Video_seek.slider({
            value: 0,
            step: 0.01,
            orientation: "horizontal",
            range: "min",
            max: totalTime,
            animate: true,
            slide: function () {
              is_seeking = true;
            },
            stop: function (e, ui) {
              is_seeking = false;
              document.getElementById(videoID).currentTime = ui.value;
            },
            create: function (e, ui) {
              options.isSliderInitialized = true;
            }
          });


          var bufferedTime = 0;
          vp2_html5_rightSidePlaylist_Video_buffer.progressbar({
            value: bufferedTime,
            create: function (e, ui) {
              options.isProgressInitialized = true;
            }
          });

          vp2_html5_rightSidePlaylist_Video.bind('progress', function () {
            //alert(document.getElementById(videoID).buffered.end(document.getElementById(videoID).buffered.length-1));
            bufferedTime = document.getElementById(videoID).buffered.end(document.getElementById(videoID).buffered.length - 1);
            //alert (bufferedTime);
            if (bufferedTime > 0) {
              vp2_html5_rightSidePlaylist_Video_buffer.progressbar({value: bufferedTime * vp2_html5_rightSidePlaylist_Video_buffer.css('width').substring(0, vp2_html5_rightSidePlaylist_Video_buffer.css('width').length - 2) / totalTime});
            }
          });

          vp2_html5_rightSidePlaylist_controls.show();
        } else {
          setTimeout(generate_seekBar, 200);
        }
      }
      ;

      generate_seekBar();

      var vp2_html5_rightSidePlaylist_FormatTime = function (seconds) {
        var m = Math.floor(seconds / 60) < 10 ? "0" + Math.floor(seconds / 60) : Math.floor(seconds / 60);
        var s = Math.floor(seconds - (m * 60)) < 10 ? "0" + Math.floor(seconds - (m * 60)) : Math.floor(seconds - (m * 60));
        return m + ":" + s;
      };

      var seekUpdate = function () {
        if (options.isSliderInitialized) {
          curTime = document.getElementById(videoID).currentTime;
          totalTime = document.getElementById(videoID).duration;
          if (!is_seeking)
            vp2_html5_rightSidePlaylist_Video_seek.slider('value', curTime);
          if (!is_overTimer)
            vp2_html5_rightSidePlaylist_Video_timer.text(vp2_html5_rightSidePlaylist_FormatTime(curTime));
          else
            vp2_html5_rightSidePlaylist_Video_timer.text('-' + vp2_html5_rightSidePlaylist_FormatTime(totalTime - curTime));
        }
      };

      vp2_html5_rightSidePlaylist_Video.bind('timeupdate', seekUpdate);

      //info
      //var vp2_html5_rightSidePlaylist_info_btn = $('.VideoInfoBut', vp2_html5_rightSidePlaylist_container);
      vp2_html5_rightSidePlaylist_info_btn.click(function () {
        if (options.skin == "giant") {
          if (vp2_html5_rightSidePlaylist_info_box.css("display") == "none")
            vp2_html5_rightSidePlaylist_info_box.fadeIn();
          else
            vp2_html5_rightSidePlaylist_info_box.fadeOut();
        } else {
          vp2_html5_rightSidePlaylist_info_box.slideToggle(300);
        }
      });

      //show/hide playlist
      vp2_html5_rightSidePlaylist_showHidePlaylist_btn.click(function () {
        if (!videoIsShowHideRunning) {
          videoIsShowHideRunning = true;
          if (vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css('display') != 'none') { //hide
            var new_width;
            //new_width=vp2_html5_rightSidePlaylist_Video[0].offsetWidth+options.borderWidth+current_obj.newPlaylistWidth;
            new_width = vp2_html5_rightSidePlaylist_Video.width() + options.borderWidth + current_obj.newPlaylistWidth;
            if (videoIsFullScreen)
              new_width = vp2_html5_rightSidePlaylist_Video.width() + 2 * options.borderWidth + options.playlistWidth;

            vp2_html5_rightSidePlaylist_showHidePlaylist_btn.addClass('VideoShowHidePlaylist_onlyShow');


            if (videoIsFullScreen) {
              vp2_html5_rightSidePlaylist_Video_seek.width(vp2_html5_rightSidePlaylist_Video_seek.width() + 2 * options.borderWidth + options.playlistWidth);
              vp2_html5_rightSidePlaylist_Video_buffer.width(vp2_html5_rightSidePlaylist_Video_buffer.width() + 2 * options.borderWidth + options.playlistWidth);

              vp2_html5_rightSidePlaylist_info_box.width(vp2_html5_rightSidePlaylist_info_box.width() + 2 * options.borderWidth + options.playlistWidth);
            } else {
              vp2_html5_rightSidePlaylist_Video_seek.width(vp2_html5_rightSidePlaylist_Video_seek.width() + options.borderWidth + current_obj.newPlaylistWidth);
              vp2_html5_rightSidePlaylist_Video_buffer.width(vp2_html5_rightSidePlaylist_Video_buffer.width() + options.borderWidth + current_obj.newPlaylistWidth);

              vp2_html5_rightSidePlaylist_info_box.width(vp2_html5_rightSidePlaylist_info_box.width() + options.borderWidth + current_obj.newPlaylistWidth);

              //vp2_html5_rightSidePlaylist_container.css( 'width', videoContainerOrigWidth+options.borderWidth+current_obj.newPlaylistWidth + "px" );
              //vp2_html5_rightSidePlaylist_container.css( 'height', videoContainerOrigHeight + "px" );

            }
            vp2_html5_rightSidePlaylist_sliderVertical.css('display', 'none');
            vp2_html5_rightSidePlaylist_thumbsHolderWrapper.animate({
              opacity: 0
            }, 300, function () {
              vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css('display', 'none');
            }
            );


            vp2_html5_rightSidePlaylist_controls.css('opacity', 0);
            vp2_html5_rightSidePlaylist_controls.animate({
              width: new_width,
              opacity: 1
            }, 300, function () {
              // Animation complete.
            }
            );


            vp2_html5_rightSidePlaylist_container.css('width', new_width + "px");
            vp2_html5_rightSidePlaylist_Video.animate({
              width: new_width
            }, 300, function () {
              // Animation complete.
              videoIsShowHideRunning = false;
            }
            );
          } else { //show
            new_width = vp2_html5_rightSidePlaylist_Video[0].offsetWidth - options.borderWidth - current_obj.newPlaylistWidth;
            if (videoIsFullScreen)
              new_width = vp2_html5_rightSidePlaylist_Video[0].offsetWidth - options.borderWidth - options.playlistWidth - options.borderWidth;

            vp2_html5_rightSidePlaylist_showHidePlaylist_btn.removeClass('VideoShowHidePlaylist_onlyShow');

            if (videoIsFullScreen) {
              vp2_html5_rightSidePlaylist_Video_seek.width(vp2_html5_rightSidePlaylist_Video_seek.width() - 2 * options.borderWidth - options.playlistWidth);
              vp2_html5_rightSidePlaylist_Video_buffer.width(vp2_html5_rightSidePlaylist_Video_buffer.width() - 2 * options.borderWidth - options.playlistWidth);

              vp2_html5_rightSidePlaylist_info_box.width(vp2_html5_rightSidePlaylist_info_box.width() - 2 * options.borderWidth - options.playlistWidth);
              vp2_html5_rightSidePlaylist_sliderVertical.css('left', Math.floor(new_width + options.borderWidth + options.playlistWidth + (options.borderWidth - vp2_html5_rightSidePlaylist_sliderVertical.width()) / 2) + 'px');
            } else {
              vp2_html5_rightSidePlaylist_Video_seek.width(vp2_html5_rightSidePlaylist_Video_seek.width() - options.borderWidth - current_obj.newPlaylistWidth);
              vp2_html5_rightSidePlaylist_Video_buffer.width(vp2_html5_rightSidePlaylist_Video_buffer.width() - options.borderWidth - current_obj.newPlaylistWidth);

              vp2_html5_rightSidePlaylist_info_box.width(vp2_html5_rightSidePlaylist_info_box.width() - options.borderWidth - current_obj.newPlaylistWidth);
              vp2_html5_rightSidePlaylist_sliderVertical.css('left', Math.floor(new_width + options.borderWidth + current_obj.newPlaylistWidth + (options.borderWidth - vp2_html5_rightSidePlaylist_sliderVertical.width()) / 2) + 'px');

              //vp2_html5_rightSidePlaylist_container.css( 'width', videoContainerOrigWidth + "px" );
              //vp2_html5_rightSidePlaylist_container.css( 'height', videoContainerOrigHeight + "px" );						
            }

            vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css('display', 'block');
            vp2_html5_rightSidePlaylist_sliderVertical.css('display', 'block');
            vp2_html5_rightSidePlaylist_thumbsHolderWrapper.animate({
              opacity: 1
            }, 600, function () {
              // Animation complete.
              videoIsShowHideRunning = false;
              carouselScroll();
            }
            );


            vp2_html5_rightSidePlaylist_controls.css('opacity', 0);
            vp2_html5_rightSidePlaylist_controls.animate({
              width: new_width,
              opacity: 1
            }, 300, function () {
              // Animation complete.
            }
            );

            vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css("left", +new_width + options.borderWidth + 'px');
            vp2_html5_rightSidePlaylist_container.css('width', new_width + "px");
            vp2_html5_rightSidePlaylist_Video.animate({
              width: new_width
            }, 300, function () {
              // Animation complete.
            }
            );
          }
        }
      });

      //movie ended
      vp2_html5_rightSidePlaylist_Video[0].addEventListener('ended', endMovieHandler, false);
      function endMovieHandler(e) {
        if (!e) {
          e = window.event;
        }
        // What you want to do after the event
        //alert ("ended");
        if (options.loop && val.indexOf("android") == -1) {
          //alert (current_img_no);
          //deactivate previous
          $(thumbsHolder_Thumbs[current_img_no]).removeClass('thumbsHolder_ThumbON');

          if (current_img_no == total_images - 1)
            current_img_no = 0;
          else
            current_img_no++;

          changeSrcAndPoster(options.autoPlay);
        }
      }

      var vp2_html5_rightSidePlaylist_VolumeValue = 1;
      vp2_html5_rightSidePlaylist_volume.slider({
        value: options.initialVolume,
        orientation: "vertical",
        range: "min",
        max: 1,
        step: 0.05,
        animate: true,
        slide: function (e, ui) {
          document.getElementById(videoID).muted = false;
          vp2_html5_rightSidePlaylist_VolumeValue = ui.value;
          document.getElementById(videoID).volume = ui.value;
        }
      });
      document.getElementById(videoID).volume = options.initialVolume;

      var muteVolume = function () {
        if (document.getElementById(videoID).muted == true) {
          document.getElementById(videoID).muted = false;
          vp2_html5_rightSidePlaylist_volume.slider('value', vp2_html5_rightSidePlaylist_VolumeValue);

          vp2_html5_rightSidePlaylist_volume_btn.removeClass('VolumeButtonMute');
          vp2_html5_rightSidePlaylist_volume_btn.addClass('VolumeButton');
        } else {
          document.getElementById(videoID).muted = true;
          vp2_html5_rightSidePlaylist_volume.slider('value', '0');

          vp2_html5_rightSidePlaylist_volume_btn.removeClass('VolumeButton');
          vp2_html5_rightSidePlaylist_volume_btn.addClass('VolumeButtonMute');
        }
        ;
      };

      vp2_html5_rightSidePlaylist_volume_btn.click(muteVolume);

      vp2_html5_rightSidePlaylist_Video.removeAttr('controls');

      var rearangethumbs = function () {
        //thumbs
        //if (options.origWidth!=options.width) {
        //alert (vp2_html5_rightSidePlaylist_regDiv.html());
        vp2_html5_rightSidePlaylist_thumbsHolderWrapper.width(current_obj.newPlaylistWidth);

        vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css("left", +vp2_html5_rightSidePlaylist_Video[0].offsetWidth + options.borderWidth + 'px');
        vp2_html5_rightSidePlaylist_sliderVertical.css('left', Math.floor(vp2_html5_rightSidePlaylist_Video[0].offsetWidth + options.borderWidth + current_obj.newPlaylistWidth + (options.borderWidth - vp2_html5_rightSidePlaylist_sliderVertical.width()) / 2) + 'px');


        thumbsHolder_Thumbs.css({
          'height': parseInt(options.origThumbH / (options.origWidth / options.width), 10) + 'px'
        });
        //parseInt(options.origThumbH/(options.origWidth/options.width),10)

        vp2_html5_rightSidePlaylist_paddingDiv.css({
          'padding-left': parseInt(options.origthumbLeftPadding / (options.origWidth / options.width), 10) + 'px',
          'padding-right': parseInt(options.origthumbRightPadding / (options.origWidth / options.width), 10) + 'px',
          'padding-top': parseInt(options.origthumbTopPadding / (options.origWidth / options.width), 10) + 'px',
          'padding-bottom': parseInt(options.origthumbBottomPadding / (options.origWidth / options.width), 10) + 'px'
        });

        var font_units = 'px';
        if (vp2_html5_rightSidePlaylist_titleDiv.css('font-size').lastIndexOf('em') != -1)
          font_units = 'em';
        var height_units = 'px';
        if (vp2_html5_rightSidePlaylist_titleDiv.css('line-height').lastIndexOf('em') != -1)
          height_units = 'em';
        vp2_html5_rightSidePlaylist_titleDiv.css({
          'font-size': parseInt(options.origthumbTitleFont / (options.origWidth / options.width), 10) + font_units,
          'line-height': parseInt(options.origthumbTitleLineHeight / (options.origWidth / options.width), 10) + height_units
        });
        //alert (options.origthumbRegLineHeight+'  --  '+parseInt(options.origthumbRegLineHeight/(options.origWidth/options.width),10));
        font_units = 'px';
        if (vp2_html5_rightSidePlaylist_regDiv.css('font-size').lastIndexOf('em') != -1)
          font_units = 'em';
        height_units = 'px';
        if (vp2_html5_rightSidePlaylist_regDiv.css('line-height').lastIndexOf('em') != -1)
          height_units = 'em';
        vp2_html5_rightSidePlaylist_regDiv.css({
          'font-size': parseInt(options.origthumbRegFont / (options.origWidth / options.width), 10) + font_units,
          'line-height': parseInt(options.origthumbRegLineHeight / (options.origWidth / options.width), 10) + height_units
        });

        var imageInside = $('.thumbsHolder_ThumbOFF', vp2_html5_rightSidePlaylist_container).find('img:first');

        imageInside.css({
          "width": parseInt(options.origThumbImgW / (options.origWidth / options.width), 10) + "px",
          "height": parseInt(options.origThumbImgH / (options.origWidth / options.width), 10) + "px"
        });



        vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height(options.height);
        vp2_html5_rightSidePlaylist_sliderVertical.height(vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - 25); // 25 is the height of  .slider-vertical.ui-slider .ui-slider-handle						
        //vp2_html5_rightSidePlaylist_thumbsHolder.css('height',vp2_html5_rightSidePlaylist_thumbsHolder.height()+current_obj.current_obj.thumbMarginTop+thumbsHolder_Thumb.height()+'px');
        current_obj.thumbMarginTop = Math.floor((vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - thumbsHolder_Thumb.height() * options.numberOfThumbsPerScreen) / (options.numberOfThumbsPerScreen - 1));
        var thumb_i = 0;
        var theThumb;
        vp2_html5_rightSidePlaylist_thumbsHolder.children().each(function () {
          thumb_i++;
          theThumb = $(this);
          theThumb.width(current_obj.newPlaylistWidth);
          if (thumb_i <= 1) {
            //theThumb.css('margin-top',Math.floor( ( vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height()-2*options.borderWidth/(options.origWidth/options.width)-(current_obj.thumbMarginTop+theThumb.height())*(options.numberOfThumbsPerScreen-1) - theThumb.height() )/2 )+'px');
            theThumb.css('margin-top', Math.floor((vp2_html5_rightSidePlaylist_thumbsHolderWrapper.height() - 2 * options.borderWidth - (current_obj.thumbMarginTop + theThumb.height()) * (options.numberOfThumbsPerScreen - 1) - theThumb.height()) / 2) + 'px');
          } else {
            theThumb.css('margin-top', current_obj.thumbMarginTop + 'px');
          }


        });

        carouselScroll();
        //}


      }

      var doResize = function () {
        if (!videoIsFullScreen) {
          responsiveWidth = vp2_html5_rightSidePlaylist_border.parent().width();
          responsiveHeight = vp2_html5_rightSidePlaylist_border.parent().height();

          if (options.responsiveRelativeToBrowser) {
            responsiveWidth = $(window).width();
            responsiveHeight = $(window).height();
          }
          /*if (options.width100Proc) {
           options.width=responsiveWidth-1;
           }
           
           if (options.height100Proc) {
           options.height=responsiveHeight;
           }*/
          //////alert (options.origWidth+' !=  '+responsiveWidth+' !=  '+options.setOrigWidthHeight);
          //////alert (vp2_html5_rightSidePlaylist_border.width()+'  ---  '+vp2_html5_rightSidePlaylist_container.width());

          //if (options.origWidth!=responsiveWidth || options.width100Proc) {
          if (vp2_html5_rightSidePlaylist_border.width() != responsiveWidth || options.width100Proc) {
            if (options.origEntirePlayerWidth > responsiveWidth || options.width100Proc) {
              options.width = parseInt((responsiveWidth - 3 * options.borderWidth) * options.origPlayerWidthProc / 100, 10);
              //current_obj.newPlaylistWidth=parseInt(responsiveWidth*options.origPlaylistWidthProc/100,10);
              current_obj.newPlaylistWidth = responsiveWidth - 3 * options.borderWidth - options.width;
            } else if (!options.width100Proc) {
              options.width = options.origWidth;
              current_obj.newPlaylistWidth = options.playlistWidth;
            }
            if (!options.height100Proc)
              options.height = options.width * options.origHeight / options.origWidth;

            options.height = parseInt(options.height, 10);
            var new_width = options.width;
            if (vp2_html5_rightSidePlaylist_thumbsHolderWrapper.css('display') == 'none') {
              new_width = options.width + options.borderWidth + current_obj.newPlaylistWidth;
              //new_width=responsiveWidth;
            }

            vp2_html5_rightSidePlaylist_Video.css('width', new_width + "px");
            vp2_html5_rightSidePlaylist_Video.css('height', options.height + "px");
            //container
            vp2_html5_rightSidePlaylist_container.css('width', new_width + "px");
            vp2_html5_rightSidePlaylist_container.css('height', options.height + "px");
            //controller
            vp2_html5_rightSidePlaylist_controls.width(new_width);
            vp2_html5_rightSidePlaylist_controls.css('bottom', videoControllersOrigBottom);
            vp2_html5_rightSidePlaylist_Video_seek.css('width', new_width - options.seekBarAdjust + 'px');
            vp2_html5_rightSidePlaylist_Video_buffer.css('width', vp2_html5_rightSidePlaylist_Video_seek.css('width'));
            //info box
            vp2_html5_rightSidePlaylist_info_box.css('width', new_width - infoBoxAdjust + 'px');


            if (!options.setOrigWidthHeight) {
              options.origWidth = options.width;
              options.origHeight = options.height;
              videoContainerOrigWidth = vp2_html5_rightSidePlaylist_container[0].offsetWidth;
              videoContainerOrigHeight = vp2_html5_rightSidePlaylist_container[0].offsetHeight;
              //options.setOrigWidthHeight=true;
            }


            //border
            vp2_html5_rightSidePlaylist_border.width(options.width + 3 * options.borderWidth + current_obj.newPlaylistWidth);
            vp2_html5_rightSidePlaylist_border.height(vp2_html5_rightSidePlaylist_Video.height() + 2 * options.borderWidth);

            rearangethumbs();

          }
        } else {
          if (val.indexOf("ipad") != -1 || val.indexOf("iphone") != -1 || val.indexOf("ipod") != -1 || val.indexOf("webos") != -1) {
            vp2_html5_rightSidePlaylist_fullscreen_btn.click();
          }
        }

      };

      var TO = false;
      $(window).resize(function () {
        doResizeNow = true;

        if (ver_ie != -1 && ver_ie == 9 && current_obj.windowWidth == 0)
          doResizeNow = false;


        if (current_obj.windowWidth == $(window).width()) {
          doResizeNow = false;
          if (options.windowCurOrientation != window.orientation && navigator.userAgent.indexOf('Android') != -1) {
            options.windowCurOrientation = window.orientation;
            doResizeNow = true;
          }
        } else {
          /*if (current_obj.windowWidth===0 && (val.indexOf("ipad") != -1 || val.indexOf("iphone") != -1 || val.indexOf("ipod") != -1 || val.indexOf("webos") != -1))
           doResizeNow=false;*/
          current_obj.windowWidth = $(window).width();
        }

        if (options.responsive && doResizeNow) {
          if (TO !== false)
            clearTimeout(TO);


          TO = setTimeout(function () {
            doResize()
          }, 300); //300 is time in miliseconds
        }
      });

      if (options.responsive) {
        doResize();
      }

      $(".confirm-delete").confirm();
    });
  };

  //
  // plugin skins
  //
  $.fn.vp2_html5_rightSidePlaylist_Video.skins = {
  };
})(jQuery);