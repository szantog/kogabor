<?php
// $Id$

/**
 * @file
 * Administration page callbacks for the umhb_form_manager module.
 */

 /**
 * Instructions page.
 */
function cck_required_single_select_admin() {
  // Explain a bit.
  $form['explanation']['#value'] = '
<p>This page allows you to optionally define custom Disallowed First Option values for each CCK Select list. You can modify these settings for each required, single-value CCK select list if you would like the first option to be something other than "- Select One -".</p>
  ';
  // Retrieve all CCK fields.
  $content_fields = content_fields();
  // Iterate through all CCK fields.
  foreach($content_fields as $key=>$value){
    // If the field is a required, single-value select list...
    if($value['widget']['type']=='optionwidgets_select' && $value['required']==1 && !$value['multiple']){
      // Print a Disallowed First Option Value field
      $form['cck_required_single_select_'.$key.'_disallowed_first_option'] = array(
        '#type' => 'textfield',
        '#title' => '"'.$value['widget']['label'].'" '.t('Disallowed First Option Value'),
        '#field_prefix'=>'-',
        '#field_suffix'=>'-',
      );
      // We have to perform an empty() check because variable_get() uses isset() which evaluates '' as "set".
      $stored_value = variable_get('cck_required_single_select_'.$key.'_disallowed_first_option','Select One');
      if(!empty($stored_value)){
        $form['cck_required_single_select_'.$key.'_disallowed_first_option']['#default_value'] = $stored_value;
      }else{
        $form['cck_required_single_select_'.$key.'_disallowed_first_option']['#default_value'] = 'Select One';
      }
    }
  }
  return system_settings_form($form);
}