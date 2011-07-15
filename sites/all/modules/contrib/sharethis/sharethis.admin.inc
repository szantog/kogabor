<?php

/**
 * @file
 * Provides the administration settings for ShareThis.
 */

/**
 * Menu callback; Settings administration.
 */
function sharethis_admin_settings() {
  // Clear the cache.
  service_links_get_links(array('sharethis' => 'sharethis'), TRUE);
  $form['sharethis_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Code'),
    '#default_value' => variable_get('sharethis_code', ''),
    '#description' => t('The <a href="@code">Get the Button code</a> provided by ShareThis. If this is not given, the default one will be used.', array('@code' => 'http://sharethis.com/publishers/getbutton/')),
  );
  $form['sharethis_icon'] = array(
    '#type' => 'radios',
    '#title' => t('Icon'),
    '#default_value' => variable_get('sharethis_icon', 'sharethis.png'),
    '#description' => t('The icon to associate with the ShareThis link.'),
    '#options' => array(
      'sharethis.png' => t('!image Static', array('!image' => theme('image', drupal_get_path('module', 'sharethis') .'/images/sharethis.png'))),
      'sharethis-rotating.gif' => t('!image Rotating', array('!image' => theme('image', drupal_get_path('module', 'sharethis') .'/images/sharethis-rotating.gif'))),
    ),
  );
  return system_settings_form($form);
}
