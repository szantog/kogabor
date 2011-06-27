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

function kgart_admin_form($form_state) {
  $default_image_dir = variable_get('kgart_image_path', 'images');

  $form['kgart_image'] = array (
    '#type' => 'fieldset',
    '#title' => t('Image paths'),
  );

  $form['kgart_image']['kgart_image_path'] = array (
    '#type' => 'textfield',
    '#title' => t('Image path'),
    '#default_value' => $default_image_dir,
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Default path for images'),
  );
  $form['kgart_image']['kgart_image_path_s'] = array (
    '#type' => 'textfield',
    '#title' => t('Image path (s)'),
    '#default_value' => variable_get('kgart_image_path_s', $default_image_dir . '/s/'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Default path for s-size images'),
  );
  $form['kgart_image']['kgart_image_path_m'] = array (
    '#type' => 'textfield',
    '#title' => t('Image path (m)'),
    '#default_value' => variable_get('kgart_image_path_m', $default_image_dir . '/m/'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Default path for for m-size images'),
  );
  $form['kgart_image']['kgart_image_path_l'] = array (
    '#type' => 'textfield',
    '#title' => t('Image path (l)'),
    '#default_value' => variable_get('kgart_image_path_l', $default_image_dir . '/l/'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Default path for for l-size images'),
  );
  return system_settings_form($form);
}