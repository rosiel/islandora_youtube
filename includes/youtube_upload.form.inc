<?php

/**
 * @file
 * Handles the uploading of the video file.
 */

/**
 * Defines a file upload form for uploading the islandora video.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_youtube_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('ogg mp4 mov qt m4v avi mkv');
  return array(
    'youtube_link' => array(
      '#title' => t('Youtube Link'),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#description' => t('Select Youtube Video to ingest.<br/>'),
    ),
  );
}

/**
 * Submit handler, adds uploaded file to the video object.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_youtube_upload_form_submit(array $form, array &$form_state) {
  $object = $form_state['islandora']['objects'][0];
  if (empty($object['OBJ'])) {
    $ds = $object->constructDatastream('OBJ', 'R');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['OBJ'];
  }
  $path = $form_state['values']['youtube_link'];
  $ds->url = $path;
  $ds->label = 'Youtube Video';
  $ds->mimetype = 'text/plain';
}
