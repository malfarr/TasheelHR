<?php
$video_file = $variables['video_file'];

$url = file_create_url($video_file->uri);
?>

<video style="width:600px;max-width:100%;" controls>
  <source src="<?php echo $url; ?>" type="video/mp4">
  Your browser does not support HTML5 video.
</video>