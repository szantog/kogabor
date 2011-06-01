<?php
// $Id: theme.inc,v 1.6 2010/05/19 13:38:21 jrao Exp $

/**
 * @file
 * All theme functions for the Hierarchical Select Node Reference module.
 */

//require_once 'Log.php';
/**
 * @ingroup themeable
 * @{
 */

/**
 * Theme function for 'hs_lineages' nodereference field formatter.
 */
function theme_hs_nodereference_formatter_hs_lineages($element) {
  // $element['#node'] is a real node when this is used in node display
  if (isset($element['#node']->type) && isset($element['#node']->{$element['#field_name']})) {
    return theme('hs_nodereference_selection_as_lineages', $element['#node'], 
      $element['#field_name'], $element['#formatter'] == 'hs_lineages_default');
  }
  // otherwise $element['#node'] is a fake node created by views
  $node = new stdClass();
  $node->nid = $element['#node']->nid;
  $node->type = $element['#node']->node_type;
  $node->vid = $element['#node']->node_vid;
  $node->title = $element['#node']->node_title;
  // we need to fill in the field values for use in theme_hs_nodereference_selection_as_lineages
  $field_values = array();
  foreach (element_children($element) as $delta) {
    $field_values[$delta] = array('nid'=>$element[$delta]['#item']['nid']);
  }
  $node->{$element['#field_name']} = $field_values;
  return theme('hs_nodereference_selection_as_lineages', $node, $element['#field_name'],
    $element['#formatter'] == 'hs_lineages_default');
}

/**
 * Themeing function to render a selection (of items) according to a given
 * node reference field as one or more lineages.
 *
 * @param $node
 *   The node whose reference field will be rendered.
 * @param $field_name
 *   Node reference field name
 */
function theme_hs_nodereference_selection_as_lineages($node, $field_name, $render_link = TRUE) {
  require_once(drupal_get_path('module', 'hierarchical_select') . '/includes/common.inc');
  // get the config for the field
  $content_type = $node->type;
  $field = content_fields($field_name, $content_type);
  $config_id = "nodereference-$content_type.$field_name"; 
  $config = hierarchical_select_common_config_get($config_id);
  $config['module'] = 'hs_nodereference';
  $config['params'] = array(
      'nodereference_path' => $field['widget']['nodereference_path'],
      'type_name' => $content_type,
      'field_name' => $field_name,  
    );    
  // create selection array from field value
  $selection = array();
  foreach ($node->$field_name as $field_value) {
    $selection[] = $field_value['nid'];
  }

  // The following comes from hierarchical_select module's 
  // theme_hierarchical_select_selection_as_lineages
  
  // Generate a dropbox out of the selection. This will automatically
  // calculate all lineages for us.
  $dropbox = _hierarchical_select_dropbox_generate($config, $selection);

  return theme('hs_nodereference_lineages', $node, $field, $dropbox->lineages, $render_link);
}

/**
 * Themeing function to render multiple lineages for a node reference field.
 *
 * @param $node
 *   The node whose reference field will be rendered.
 * @param $field
 *   Node reference field
 * @param $lineages
 *   Array of lineage id => lineage array
 */
function theme_hs_nodereference_lineages($node, $field, $lineages, $render_link = TRUE) {
  // Actual formatting.
  $output = '';
  foreach ($lineages as $id => $lineage) {
    if ($id > 0) {
      $output .= '<br />';
    }
    $output .= theme('hs_nodereference_lineage', $id, $lineage, $render_link);
  }

  // Add the CSS.
  drupal_add_css(drupal_get_path('module', 'hierarchical_select') .'/hierarchical_select.css');

  return $output;
}

/**
 * Themeing function to render single lineage for a node reference field.
 * 
 * @param $id
 *   The lineage id from dropbox.
 * @param $lineage
 *   Lineage array of node ids.
 */
function theme_hs_nodereference_lineage($id, $lineage, $render_link = TRUE) {
    $items = array();
    foreach ($lineage as $level => $item) {
      if ($render_link) {
        $items[] = l($item['label'], 'node/'. $item['value']);
      }
      else {
        $items[] = check_plain($item['label']);
      }
    }
    return implode('<span class="hierarchical-select-item-separator">›</span>', $items);  
}
/**
 * @} End of "ingroup themeable".
 */


//----------------------------------------------------------------------------
// Private functions.
