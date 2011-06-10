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

function kgart_admin_page() {
  module_load_include('inc', 'kgart_admin', 'includes/kgart_admin.forms');
  $page = drupal_get_form('kgart_admin_form');
  dsm(get_defined_vars());
  return $page;
}