<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function uf_gallery_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'gallery_node';
  $context->description = 'Context on gallery nodes';
  $context->tag = 'gallery';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'gallery' => 'gallery',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'gallery' => 'gallery',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-header_image-block_2' => array(
          'module' => 'views',
          'delta' => 'header_image-block_2',
          'region' => 'header',
          'weight' => 0,
        ),
        'views-pictures_of_gallery-block_3' => array(
          'module' => 'views',
          'delta' => 'pictures_of_gallery-block_3',
          'region' => 'footer',
          'weight' => 0,
        ),
        'views-pictures_of_gallery-block_2' => array(
          'module' => 'views',
          'delta' => 'pictures_of_gallery-block_2',
          'region' => 'footer',
          'weight' => 1,
        ),
      ),
    ),
    'menu' => 'gallery',
    'theme' => array(
      'title' => '',
      'subtitle' => '',
      'class' => 'gallery',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context on gallery nodes');
  t('gallery');

  $export['gallery_node'] = $context;
  return $export;
}
