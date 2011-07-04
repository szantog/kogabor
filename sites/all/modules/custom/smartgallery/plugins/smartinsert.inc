<?php

/**
 * Implementation of hook_INCLUDE_plugin().
 */
function smartgallery_smartinsert_plugin() {
  $plugins['smartinsert'] = array(
    'title' => t('Smartgallery plugin'),
    'vendor url' => '',
    'icon path' => '',
    'icon file' => '',
    'icon title' => t('Insert image'),
    // An array of settings for this button. Required, but API is still in flux.
    'settings' => array(
    ),
  );
  return $plugins;
}