<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Implements hook_preprocess().
 */
function islandora_youtube_preprocess_islandora_youtube(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/authtokens');
  module_load_include('inc', 'islandora', 'includes/metadata');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $object = $variables['object'];
  $repository = $object->repository;
  // We should eventually remove the DC object and dc_array code as it only
  // exists to not break legacy implementations.

  $variables['islandora_object_label'] = $object->label;
  $variables['theme_hook_suggestions'][] = 'islandora_video__' . str_replace(':', '_', $object->id);
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
  $variables['description'] = islandora_retrieve_description_markup($object);

  $viewer_dsid = 'OBJ';

  $video_params = array(
    'pid' => $object->id,
  );
  // Video player.
  if (isset($object[$viewer_dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$viewer_dsid])) {
    $video_url = $object[$viewer_dsid]->url;

    $parts = parse_url($video_url);
    parse_str($parts['query'], $query);
    $youtube_id = $query['q'];

    # Don't really use this:
    $video_params += array(
      'mime' => '',
      'url' => $video_url,
    );
  }
  // Thumbnail.
  if (isset($object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN'])) {
    $video_params += array(
      'tn' => url("islandora/object/{$object->id}/datastream/TN/view", array('absolute' => TRUE)),
    );
  }

  #### OLD CODE
  #$viewer = islandora_get_viewer($video_params, 'islandora_video_viewers', $object);
  #### END OLD CODE

  $parts = parse_url($video_url);
  parse_str($parts['query'], $query);
  $youtube_id = $query['v'];

  $viewer = <<<EOM
<iframe width="560" height="315" src="//www.youtube.com/embed/$youtube_id" frameborder="0" allowfullscreen></iframe>
EOM;
  $variables['islandora_content'] = '';
  if ($viewer) {
    $variables['islandora_content'] = $viewer;
  }
  else {
    $variables['islandora_content'] = NULL;
  }
  return array('' => $viewer);
}
