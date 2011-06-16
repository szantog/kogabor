<?php

function kogart_admin_theme_form_element($element, $value) {
  // This is also used in the installer, pre-database setup.

  if ($element['#type'] == 'textarea') {

    $t = get_t();

    $output = '<div class="form-item"';
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

    if (!empty($element['#description'])) {
      $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
    }

    $output .= " $value\n";

    $output .= "</div>\n";

    return $output;
  }

  return theme_form_element($element, $value);
}
?>
