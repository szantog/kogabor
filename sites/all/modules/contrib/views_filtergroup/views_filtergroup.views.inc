<?php

/**
 * @file
 * Implements the views hooks that announce handlers.
 */

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function views_filtergroup_views_handlers() {
  return array(
    'handlers' => array(
      'views_filtergroup_handler_filter_group' => array(
        'parent' => 'views_handler_filter',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data_alter().
 */
function views_filtergroup_views_data_alter(&$cache) {
  $cache['node']['filtergroup'] = array(
    'group' => t('[Grouping]'),
    'title' => t('Filter Group'),
    'help' => t('A Filter Group'),
    'filter' => array(
      'handler' => 'views_filtergroup_handler_filter_group',
    ),
  );

}