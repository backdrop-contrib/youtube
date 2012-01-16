<?php

/**
 * @ file
 * YouTube Field helper functions.
 */

/**
 * Get images by building correctly formed URL - kinda hackey.
 * NOT USED in youtube field module.
 */
function youtube_build_remote_image_path($id = NULL) {
  if (!$id) {
    return;
  }
  // Default image thumbnail.
  $default = 'http://img.youtube.com/vi/' . $id . '/default.jpg';

  // Full size image.
  $full_size = 'http://img.youtube.com/vi/' . $id . '/0.jpg';
  // High Quality version of the default thumbnail.
  $hq_default = 'http://img.youtube.com/vi/' . $id . '/hqdefault.jpg';
  // High Resolution version of the default thumbnail.
  $max_res_default = 'http://img.youtube.com/vi/' . $id . '/maxresdefault.jpg'; // May not work.
  // First thumbnail.
  $thumb1 = 'http://img.youtube.com/vi/' . $id . '/1.jpg';
  // Second thumbnail.
  $thumb2 = 'http://img.youtube.com/vi/' . $id . '/2.jpg';
  // Third thumbnail.
  $thumb3 = 'http://img.youtube.com/vi/' . $id . '/3.jpg';
  
  return url($default);
}
