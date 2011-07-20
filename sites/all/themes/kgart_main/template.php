<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to kgart_main_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: kgart_main_breadcrumb()
 *
 *   where kgart_main is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and template suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440
 *   and http://drupal.org/node/190815#template-suggestions
 */

/* Set message, if less module doesn't exist. */
if (!module_exists('less')){
  drupal_set_message(t('The module <a href="http://drupal.org/project/less">less</a> doesn\'t exist. Download, and enable it"'), 'warning');
}

/**
 * Implementation of HOOK_theme().
 */
function kgart_main_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function kgart_main_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */

function kgart_main_preprocess_page(&$vars, $hook) {
  if ($vars['node']) {
    $node = $vars['node'];
  }
  // The toplinks will printed before the content, it contains backlinks, print link, etc..
  $toplinks = array();
  //If a node type need to hide the original h1 title, put this in this array
  $hidetitle = array(
    'art',
  );
  if (in_array($node->type, $hidetitle)){
    $vars['hidetitle'] = TRUE;
  }

  if (!$vars['is_front']) {
    $toplinks[] = '<a class="form-button form-btn form-button-cancel" rel="nofollow" href="javascript: history.go(-1)" title="' . t('Back') .'" alt="' . t('Back') .'">' . t('Back') . '</a>';
  }

  //This section create the addthis links
  if (module_exists('addthis') && $node->type == 'art') {
    // Work also for not-node pages
    $addthis_widget_type = variable_get('addthis_widget_type', 'addthis_button');
    $vars['addthiswidget'] = theme($addthis_widget_type);
  }
  if ($node->type == 'art') {
    $toplinks[] = l(t('Print'), "node/$node->nid/print");
    $toplinks[] = l(t('Start slideshow'), "node/$node->nid/slide");
  }

  if (!empty($toplinks)) {
    $vars['toplinks'] = theme('item_list', $toplinks);
  }
  //dsm(get_defined_vars());
  // To remove a class from $classes_array, use array_diff().
  //$vars['classes_array'] = array_diff($vars['classes_array'], array('class-to-remove'));
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */

function kgart_main_preprocess_node(&$vars, $hook) {
  $node = $vars['node'];
  $vars['my_print_link'] = l(t('Print'), "node/$node->nid/print");
  // Optionally, run node-type-specific preprocess functions, like
  // kgart_main_preprocess_node_page() or kgart_main_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $vars['node']->type;
  if (function_exists($function)) {
    $function($vars, $hook);
  }
}


function  kgart_main_preprocess_node_art(&$vars) {
  $node = $vars['node'];
  //$vars['service_links_rendered'] = 'service_links tmp disabled';
  //dsm(get_defined_vars());
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function kgart_main_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function kgart_main_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Views Slideshow: "previous" control.
 * Overidden theme function, because of we need some txet change
 *
 * @ingroup themeable
 */
function kgart_main_views_slideshow_singleframe_control_previous($vss_id, $view, $options) {
  if ($vss_id == 'front_page_slideshow-block_1') {
  return l(t('Previous picture'), '#', array(
    'attributes' => array(
      'class' => 'views_slideshow_singleframe_previous views_slideshow_previous',
      'id' => "views_slideshow_singleframe_prev_" . $vss_id,
    ),
    'fragment' => ' ',
    'external' => TRUE,
  ));
  }
  return l(t('Previous'), '#', array(
    'attributes' => array(
      'class' => 'views_slideshow_singleframe_previous views_slideshow_previous',
      'id' => "views_slideshow_singleframe_prev_" . $vss_id,
    ),
    'fragment' => ' ',
    'external' => TRUE,
  ));
}

/**
 * Views Slideshow: "next" control.
 * Overidden theme function, because of we need some txet change
 *
 * @ingroup themeable
 */
function kgart_main_views_slideshow_singleframe_control_next($vss_id, $view, $options) {
  if ($vss_id == 'front_page_slideshow-block_1') {
    return l(t('Next picture'), '#', array(
      'attributes' => array(
        'class' => 'views_slideshow_singleframe_next views_slideshow_next',
        'id' => "views_slideshow_singleframe_next_" . $vss_id,
      ),
      'fragment' => ' ',
      'external' => TRUE,
    ));
  }

  return l(t('Next'), '#', array(
    'attributes' => array(
      'class' => 'views_slideshow_singleframe_next views_slideshow_next',
      'id' => "views_slideshow_singleframe_next_" . $vss_id,
    ),
    'fragment' => ' ',
    'external' => TRUE,
  ));
}

/*
 * Override comment box title
 */
function kgart_main_preprocess_box(&$vars) {
  if ($vars['id'] == 2) {
    $vars['title'] = t('Send oponion');
  }
}