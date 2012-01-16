<?php
/**
 * @file
 * Theme functions for the YouTube field module.
 */

/**
 * Theme function for videos.
 */
function theme_youtube_video($variables) {
  $id = $variables['video_id'];
  $size = $variables['youtube_size'];

  // Get YouTube settings.
  $old = variable_get('youtube_old', FALSE);
  $suggest = variable_get('youtube_suggest', TRUE);
  $privacy = variable_get('youtube_privacy', FALSE);
  $dimensions = youtube_get_dimensions($size);

  // Protocol changes based on current page TODO.
  $protocol = (isset($_SERVER['HTTPS'])) ? 'https' : 'http';

  // Query string changes based on setings.
  $query = array();
  if ($old){
    $query['version'] = '3';
    $query['hl'] = 'en_US';
  }
  if (!$suggest) {
    $query['rel'] = '0';
  }

  // Domain changes based on settings.
  $domain = ($privacy) ? 'youtube-nocookie.com' : 'youtube.com';

  $path = $protocol . '://www.' . $domain . '/embed/' . $id;
  $src = url($path, array('query' => $query));

  if ($old) {
    $path = $protocol . '://www.' . $domain . '/v/' . $id;
    $embed_src = url($embed_path, array('query' => $query));
  }

  if ($old) {
    $output = '<object width="' . $dimensions['width'] . '" 
      height="' . $dimensions['height'] . '">
      <param name="movie" value="' . $src . '"></param>
      <param name="allowFullScreen" value="true"></param>
      <param name="allowscriptaccess" value="always"></param>
      <embed src="http://www.youtube.com/v/1SqBdS0XkV4?version=3&amp;hl=en_US&amp;rel=0" 
        type="application/x-shockwave-flash" width="' . $dimensions['width'] . '" 
        height="' . $dimensions['height'] . '" allowscriptaccess="always" allowfullscreen="true">
      </embed></object>';
  }
  else {
    $output = '<iframe width="' . $dimensions['width'] . '" 
      height="' . $dimensions['height'] . '" src="' . $src . '" 
      frameborder="0" allowfullscreen></iframe>';
  }

  return $output;
}


/**
 * Theme function for thumbnails.
 */
function theme_youtube_thumbnail($variables) {
  $id = $variables['video_id'];
  $style = $variables['image_style'];

  // Get YouTube settings - TODO is this needed?
  $size = variable_get('youtube_size', '420x315');
  $dimensions = youtube_get_dimensions($size);

  $files = variable_get('file_public_path', conf_path() . '/files');
  $youtube = variable_get('youtube_thumb_dir', 'youtube');
  $dest = $files . '/' . $youtube . '/' . $id . '.png';

  // Check to see if a thumbnail exists locally.
  if (!file_exists($dest)) {
    // Retrieve the image from YouTube.
    if (!youtube_get_remote_image($id)) {
      // Use the remote source if local copy fails.
      $src = youtube_build_remote_image_path($id);
      return theme('image', array('path' => $src));
    }
  }

  if ($style) {
    $uri = 'public://' . $youtube . '/' . $id . '.png';
    $image = theme('image_style', array('style_name' => $style, 'path' => $uri));
  }
  else {
    $path = $files . '/' . $youtube . '/' . $id . '.png';
    $image = theme('image', array('path' => $path));
  }

  return $image;
}