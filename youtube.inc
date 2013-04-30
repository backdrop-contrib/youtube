<?php

/**
 * @ file
 * YouTube field helper functions.
 */

/**
 * Extracts the video_id from the submitted field value.
 *
 * @param $input
 *   The input submitted to the field.
 *
 * @return
 *   Returns the video_id if available, or FALSE if not.
 */
function youtube_get_video_id($input) {
  // The video URL in the format http://youtube.com/watch?v=1SqBdS0XkV4.
  if (strstr($input, 'youtube.com/watch?') && preg_match('/v=[^&]*(?=&|$)/', $input, $matches)) {
    $video_id = ltrim($matches[0], 'v=');
  }

  // The video URL in the format http://youtu.be/1SqBdS0XkV4.
  elseif (strstr($input, 'youtu.be/')) {
    $anchor = 'be/';
    $position = strpos($input, $anchor);
    $video_id = trim(substr($input, $position + strlen($anchor)));
    // Remove additional query parameters.
    if ($param = strpos($video_id, '?')) {
      $video_id = substr($video_id, 0, $param);
    }
  }

  if (!empty($video_id)) {
    return $video_id;
  }

  return FALSE;
}

/**
 * Returns a list of standard YouTube video sizes.
 */
function youtube_size_options() {
  return array(
    '420x315' => '450px by 315px',
    '480x360' => '480px by 360px',
    '640x480' => '640px by 480px',
    '960x720' => '960px by 720px',
    'custom' => 'custom',
  );
}

/**
 * Splits height and width when given size, as from youtube_size_options.
 */
function youtube_get_dimensions($size = NULL, $width = NULL, $height = NULL) {
  $dimensions = array();
  if ($size == 'custom') {
    $dimensions['width'] = (int) $width;
    $dimensions['height'] = (int) $height;
  }
  else {
    // Locate the 'x'.
    $strpos = strpos($size, 'x');
    // Width is the first dimension.
    $dimensions['width'] = substr($size, 0, $strpos);
    // Height is the second dimension.
    $dimensions['height'] = substr($size, $strpos + 1, strlen($size));
  }

  return $dimensions;
}

/**
 * Retreve youtube thumbnail image via YouTube API.
 *
 * TODO add error messaging if something goes wrong, and return FALSE.
 */
function youtube_get_remote_image($id = NULL) {
  $path = 'http://gdata.youtube.com/feeds/api/videos/' . $id;
  $query = array(
    'v' => '2',
    'alt' => 'jsonc'
  );
  $url = url($path, array('query' => $query));
  $result = drupal_http_request($url);
  $data = json_decode($result->data);
  // Get the high quality default thumbnail.
  $src = $data->data->thumbnail->hqDefault;

  // Make the actual request to download the file.
  $image_result = drupal_http_request($src);

  // Assure the youtube thumbnail directory exists.
  $files = variable_get('file_public_path', conf_path() . '/files');
  $youtube_dir = variable_get('youtube_thumb_dir', 'youtube');
  $youtube_path = $files . '/' . $youtube_dir;
  if (!file_prepare_directory($youtube_path, FILE_CREATE_DIRECTORY) && !mkdir($youtube_path, 0775, TRUE)) {
    watchdog('youtube', 'Failed to create YouTube thumbnail directory: %dir', array('%dir' => $youtube_path), WATCHDOG_ERROR);
  }

  // Save the file.
  $dest = $files . '/' . $youtube_dir . '/' . $id . '.png';
  file_put_contents($dest, $image_result->data);

  return TRUE;
}

/**
 * Get images by building correctly formed URL - kinda hackey.
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

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * Adds a target option for YouTube fields to Feeds mapping options.
 *
 * @param &$targets
 *   Array containing the targets to be offered to the user. Add to this array
 *   to expose additional options. Remove from this array to suppress options.
 *   Remove with caution.
 * @param $entity_type
 *   The entity type of the target, for instance a 'node' entity.
 * @param $bundle_name
 *   The bundle name for which to alter targets.
 */
function youtube_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);

    if (in_array($info['type'], array('youtube'))) {
      $targets[$name] = array(
        'name' => check_plain($instance['label']),
        'callback' => 'youtube_set_target',
        'description' => t('The @label field of the node.', array('@label' => $instance['label'])),
      );
    }
  }
}

/**
 * Callback to set the Feeds target for a YouTube field.
 *
 * @param $source
 *   Field mapper source settings.
 * @param $entity
 *   An entity object, for instance a node object.
 * @param $target
 *   A string identifying the target on the node.
 * @param $value
 *   The value to populate the target with.
 */
function youtube_set_target($source, $entity, $target, $value, $mapping) {
  $video_id = youtube_get_video_id($value);
  if ($video_id) {
    $entity->{$target}[LANGUAGE_NONE][0] = array(
      'input' => $value,
      'video_id' => $video_id,
    );
  }
}
