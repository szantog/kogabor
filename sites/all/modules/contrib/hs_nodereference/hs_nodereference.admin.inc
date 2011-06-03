<?php
// $Id: hs_nodereference.admin.inc,v 1.4 2010/04/11 08:22:31 jrao Exp $
//require_once 'Log.php';
/**
 * @file
 * Hierarchical Select for Node Reference configuration form.
 */
/**
 * Form definition; configuration form for Hierarchical Select as the widget
 * for a node reference field.
 *
 * @param $content_type_name
 *   Machine name of a content type. Provides necessary context.
 * @param $field_name
 *   Name of a field. Provides necessary context.
 */
function hs_nodereference_config_form($form_state, $content_type_name, $field_name) {
  module_load_include('inc', 'hierarchical_select', 'includes/common');
  $content_type = content_types($content_type_name);
  $field = $content_type['fields'][$field_name];
  $nodereference_path = $field['widget']['nodereference_path'];
  $labels = _hs_nodereference_get_level_labels_from_path($nodereference_path);
  list($allowed_levels, $not_editable_reasons) = _hs_nodereference_get_allowed_levels_from_path($nodereference_path);
  // Add the Hierarchical Select config form.
  $module = 'hs_nodereference';
  $params = array(
    'nodereference_path' => FALSE,
    'type_name' => $content_type_name,
    'field_name' => $field_name,  
  );
  $config_id = "nodereference-$content_type_name.$field_name";
  $defaults = array(
    'save_lineage' => 0,
    // Let admin set this, node reference validation will catch the error if
    // user didn't select from the level with the correct content type.
    'enforce_deepest' => 1,
    'entity_count'    => 0,
    'require_entity'  => 0,  
    'level_labels' => array(
        'status' => 1,
        'labels' => $labels,
    ),  
    'editability' => array(
      'status'     => 1,
      'item_types'        => $labels,
      'allowed_levels'    => $allowed_levels,
      'allow_new_levels'  => 0,    
    ),
  );
  // If this config is being created (not edited), then enable the dropbox if
  // this is a "multiple values" field. This allows for an intuitive
  // transition to a Hierarchical Select widget.
  /*
  if (!variable_get('hs_config_'. $config_id, FALSE)) {
    $defaults['dropbox']['status'] = $field['multiple'];
  }
  */
  $defaults['dropbox']['status'] = 0;
  $strings = array(
    'hierarchy'   => t('node reference path'),
    'hierarchies' => t('node reference paths'),
    'item'        => t('node'),
    'items'       => t('nodes'),
    'item_type'   => t('node reference'),
  );
  $max_hierarchy_depth = count($labels) - 1;
  $preview_is_required = $field['required'];
  $form['hierarchical_select_config'] = hierarchical_select_common_config_form($module, $params, $config_id, $defaults, $strings, $max_hierarchy_depth, $preview_is_required);

  // normalize field multiple value
  switch($field['multiple']) {
    case 0:
      $field_multiple = 1;
      break;
    case 1:
      $field_multiple = 2147483647;
      break;
    default:
      $field_multiple = $field['multiple'];
      break;
  }
  // save lineage is possible only when: 
  // 1. # of values in field >= # of levels
  if ($field_multiple < $max_hierarchy_depth) {
    $form['hierarchical_select_config']['save_lineage']['#default_value'] = 0;
    $form['hierarchical_select_config']['save_lineage']['#disabled'] = TRUE;
    $form['hierarchical_select_config']['save_lineage']['#description'] .= '<br />'
     . t("This setting is disabled because the number of values in this field 
     '!number-of-values' is less than the number of levels '!number-of-levels'.",
      array(
       '!number-of-values' => $field_multiple,
       '!number-of-levels' => $max_hierarchy_depth));
  }
  // 2. referenceable types contains all the types in all levels
  $types = _hs_nodereference_get_all_types_in_path($nodereference_path);
  foreach ($types as $type) {
    if (!in_array($type, array_filter($field['referenceable_types']))) {
      $form['hierarchical_select_config']['save_lineage']['#default_value'] = 0;
      $form['hierarchical_select_config']['save_lineage']['#disabled'] = TRUE;
      $form['hierarchical_select_config']['save_lineage']['#description'] .= '<br />'
       . t("This setting is disabled because type '!type' in the node reference
       path is not in the field's referenceable types.",
        array('!type' => $type));
      break;
    }
  }
  // check if dropbox can be enabled
  if ($form['hierarchical_select_config']['save_lineage']['#default_value'] == 0) {
    // if save lineage is not turned on, then dropbox can be enabled as long as
    // number of values > 1
    if ($field_multiple <= 1) {
      $form['hierarchical_select_config']['dropbox']['status']['#default_value'] = 0;  
      $form['hierarchical_select_config']['dropbox']['status']['#disabled'] = TRUE; 
      $form['hierarchical_select_config']['dropbox']['#description'] .= '<br />'
        . t("This setting is disabled because the field's Number of values is
        less than or equals 1.");
    }
  }
  else {
    // if save lineage is turned on, then number of values must > levels
      if ($field_multiple <= $max_hierarchy_depth) {
      $form['hierarchical_select_config']['dropbox']['status']['#default_value'] = 0;  
      $form['hierarchical_select_config']['dropbox']['status']['#disabled'] = TRUE; 
      $form['hierarchical_select_config']['dropbox']['#description'] .= '<br />'
        . t("This setting is disabled because the field's Number of values 
        '!number-of-values' is less than or equals the number of levels 
        '!number-of-levels'.",
        array(
          '!number-of-values' => $field_multiple,
          '!number-of-levels' => $max_hierarchy_depth));
    }    
  }
  // disable editability settings that doesn't make sense
  foreach ($allowed_levels as $depth => $allowed) {
    if (!$allowed) {
      $form['hierarchical_select_config']['editability']['allowed_levels'][$depth]['#default_value'] = 0;
      $form['hierarchical_select_config']['editability']['allowed_levels'][$depth]['#disabled'] = TRUE;
      $form['hierarchical_select_config']['editability']['allowed_levels'][$depth]['#description'] = $not_editable_reasons[$depth];
    }
  }      
  $form['link'] = array(
    '#value' => l('Back to the field configuration', 'admin/content/node-type/'. $content_type['type'] .'/fields/'. $field_name),
    '#prefix' => '<div class="cck-hierarchical-select-back-link">',
    '#suffix' => '</div>',
    '#weight' => -5,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#content_type_name'] = $content_type_name;
  $form['#field_name'] = $field_name;

  // Add the the submit handler for the Hierarchical Select config form.
  $parents = array('hierarchical_select_config');
  $form['#submit'][] = 'hierarchical_select_common_config_form_submit';
  $form['#hs_common_config_form_parents'] = $parents;

  return $form;
}

/**
 * Get all type names used in node reference path
 * @param $nodereference_path
 * @return array of type names
 */
function _hs_nodereference_get_all_types_in_path($nodereference_path) {
  $types = array();
  $nodereference_typefields = explode('-', $nodereference_path);
  foreach ($nodereference_typefields as $typefield) {
    list($type, $field_name) = explode('.', $typefield);
    $field = content_fields($field_name, $type);
    if (is_array($field['referenceable_types'])) {
      $types += array_filter($field['referenceable_types']);
    }
  }  
  return array_unique($types);
}

/**
 * Get allowed levels from node reference path
 * @param $nodereference_path
 * @return array(allowed levels, reason for not editable)
 */
function _hs_nodereference_get_allowed_levels_from_path($nodereference_path) {
  if (empty($nodereference_path)) {
    return array(array(), array());
  }
  $allowed_levels = array();
  $not_editable_reasons = array();
  $nodereference_typefields = explode('-', $nodereference_path);
  array_shift($nodereference_typefields);
  if (empty($nodereference_typefields)) {
    return array(array(), array());
  }  
  $nodereference_typefields = array_reverse($nodereference_typefields);
  // special handling for root
  list($root_type_name, $root_field_name) = explode('.', $nodereference_typefields[0]);
  $root_field = content_fields($root_field_name, $root_type_name);
  if (empty($root_field['referenceable_types'])) {
    $allowed_levels[] = FALSE;
    $not_editable_reasons[] = t("No type is selected in field 
      '!field-name''s 'Content types that can be referenced' setting.", 
      array('!field-name' => $root_field_name));
  }
  else {
    $referenceable_types = array_values(array_filter($root_field['referenceable_types']));
    if (count($referenceable_types) != 1) {
      $allowed_levels[] = FALSE;
      $not_editable_reasons[] = t("More than one type is selected 
        in field '!field-name''s 'Content types that can be referenced' setting.", 
        array('!field-name' => $root_field_name));
    }
    else {  
      list($allowed, $reason) = _hs_nodereference_is_type_editable($referenceable_types[0], '');
      $allowed_levels[] = $allowed;
      $not_editable_reasons[] = $reason;
    }  
  }
  // handle the rest      
  foreach ($nodereference_typefields as $typefield) {
    list($type_name, $field_name) = explode('.', $typefield);
    list($allowed, $reason) = _hs_nodereference_is_type_editable($type_name, $field_name);
    $allowed_levels[] = $allowed;
    $not_editable_reasons[] = $reason;    
  }
  return array($allowed_levels, $not_editable_reasons);  
}

/**
 * Whether a type is editable (i.e. can be created by title)
 * @param $type_name
 * @return array(TRUE or FALSE, 'Reason for not editable')
 */
function _hs_nodereference_is_type_editable($type_name, $reference_field_name) {
  $type = content_types($type_name);
  if (empty($type['fields'])) {
    return array(TRUE, '');
  }  
  foreach ($type['fields'] as $field) {
    if ($field['required'] && $field['field_name'] != $reference_field_name) {     
      return array(FALSE, t("Type '!type-name' has a required field '!field-name'.", 
        array(
          '!type-name' => $type_name,
          '!field-name' => $field['field_name'])));
    }
  }  
  return array(TRUE, '');
}