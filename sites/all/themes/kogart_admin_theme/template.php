<?php

/*
 * Using this to add $element['#type'] class to main containers of form items
 */
function kogart_admin_theme_form_element($element, $value) {
  // This is also used in the installer, pre-database setup.

  $t = get_t();

  $output = '<div class="form-item form-' . $element['#type'] . '-container"';
  if (!empty($element['#id'])) {
    $output .= ' id="' . $element['#id'] . '-wrapper"';
  }
  $output .= ">\n";
  $required = !empty($element['#required']) ? '<span class="form-required" title="' . $t('This field is required.') . '">*</span>' : '';

  if (!empty($element['#title'])) {
    $title = $element['#title'];
    if (!empty($element['#id'])) {
      $output .= ' <label for="' . $element['#id'] . '">' . $t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
    else {
      $output .= ' <label>' . $t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
  }

  if ($element['#type'] == 'textarea') {
    if (!empty($element['#description'])) {
      $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
      //prevent to print out description twice
      unset($element['#description']);
    }
  }

  $output .= " $value\n";

  if (!empty($element['#description'])) {
    $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

function kogart_admin_theme_preprocess_page(&$vars) {
  $classes_array = explode(' ', $vars['body_classes']);
  if ($vars['node']) {
    $node = $vars['node'];
  }

  if (arg(0) == 'node' && arg(1) == 'add') {
    $classes_array[] = 'node-type-' . arg(2);
  }

  if (arg(0) == 'node' && is_numeric(arg(1)) && arg(2) == 'datasheet') {
    $node = node_load(arg(1));
    $classes_array[] = 'node-type-' . $node->type;
  }

  $vars['body_classes'] = implode(' ', $classes_array);
}

/*
 * Overriden theme_ds_field function.
 * This is because of fieldset titles aren't translatable.
 * @todo: it isn't the best was, but the nd module uses t in template files too
 */
function kogart_admin_theme_ds_field($content, $field) {
  $output = '';

  if (!empty($content)) {
    if ($field['type'] == 'ds') {

      $output .= '<div class="field '. t($field['class']) .'">';
      // Above label.
      if ($field['labelformat'] == 'above') {
        $output .= '<div class="field-label">'. t($field['title']) .': </div>';
      }
      // Inline label
      if ($field['labelformat'] == 'inline') {
        $output .= '<div class="field-label-inline-first">'. t($field['title']) .': </div>';
      }
      $output .= $content;
      $output .= '</div>';
    }
    else {
      $output = $content;
    }
  }

  return $output;
}