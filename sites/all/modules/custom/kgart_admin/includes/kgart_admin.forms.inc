<?php

/**
 * @file
 *   Custom administration pages and functions for Kovács Gábor gyűjtemény site.
 *   Project:Young Element
 *
 * @developers:
 *    Gabor Szanto <hello@szantogabor.com>
 *    http://szantogabor.com
 *
 */

function kgart_admin_form() {
  $form['kgart_image_path'] = array (
    '#type' => 'textfield',
    '#title' => t('Image path'),
    '#default_value' => variable_get('kgart_image_path', NULL),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Default path for images'),
  );
  return system_settings_form($form);
}