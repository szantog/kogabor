<?php

/**
 * @file aef.views.inc
 *
 * Plugin definitions.
 */

/**
 * Implementation of hook_views_plugins().
 */
function aef_views_plugins() {
  return array(
    'exposed_form' => array(
      'aef_dynamic' => array(
        'title' => t('Dynamic exposed filters'),
        'help' => t('Allows user to apply the same exposed filter with different values.'),
        'handler' => 'aef_dynamic_exposed_form_plugin',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'parent' => 'basic',
        'type' => 'dynamic',
      ),
    ),
  );
}
