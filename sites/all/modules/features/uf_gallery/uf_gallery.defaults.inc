<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _uf_gallery_content_default_fields() {
  $fields = array();

  // Exported field: field_gallery_description
  $fields[] = array(
    'field_name' => 'field_gallery_description',
    'type_name' => 'gallery',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'trimmed',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '10',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_gallery_description][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'filefield_sources' => array(
        'imce' => 'imce',
        'reference' => 'reference',
        'remote' => 'remote',
      ),
      'label' => 'Description',
      'weight' => '-4',
      'description' => '',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_related_content
  $fields[] = array(
    'field_name' => 'field_related_content',
    'type_name' => 'gallery',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'nodereference',
    'required' => '0',
    'multiple' => '1',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'event' => 'event',
      'news' => 'news',
      'page' => 'page',
      'advertisment' => 0,
      'documents' => 0,
      'gallery' => 0,
      'picture' => 0,
      'recipe' => 0,
      'video' => 0,
      'story' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '30',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_related_content][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'filefield_sources' => array(
        'imce' => 'imce',
        'reference' => 'reference',
        'remote' => 'remote',
      ),
      'label' => 'Related Content',
      'weight' => '-1',
      'description' => '',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_gallery
  $fields[] = array(
    'field_name' => 'field_gallery',
    'type_name' => 'picture',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'nodereference',
    'required' => '1',
    'multiple' => '0',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'gallery' => 'gallery',
      'advertisment' => 0,
      'documents' => 0,
      'event' => 0,
      'news' => 0,
      'page' => 0,
      'picture' => 0,
      'recipe' => 0,
      'video' => 0,
      'story' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '30',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_gallery][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'node_link' => array(
        'teaser' => 0,
        'full' => 1,
        'title' => 'Add picture to Gallery',
        'hover_title' => '',
        'destination' => 'node',
      ),
      'fallback' => 'select',
      'filefield_sources' => array(
        'imce' => 'imce',
        'reference' => 'reference',
        'remote' => 'remote',
      ),
      'label' => 'Gallery',
      'weight' => '-3',
      'description' => '',
      'type' => 'nodereference_url',
      'module' => 'nodereference_url',
    ),
  );

  // Exported field: field_picture
  $fields[] = array(
    'field_name' => 'field_picture',
    'type_name' => 'picture',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '1',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '[title-raw]-[filefield-fid]',
      'custom_alt' => 0,
      'title' => '[title-raw]-[filefield-fid]',
      'custom_title' => 0,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'fupload_mode' => 'single',
      'fupload_previewlist_img_attributes' => '',
      'fupload_title_replacements' => '_;{;};-',
      'fupload_previewlist_field_settings' => array(
        'node_title' => 'node_title',
        'imagefield_title' => 'imagefield_title',
        'imagefield_alt' => 'imagefield_alt',
        'node_description' => 0,
        'imagefield_description' => 0,
        'cck_field_gallery' => 0,
      ),
      'fupload_previewlist_redirecturl' => 'gallery',
      'filefield_sources' => array(
        'imce' => 'imce',
        'reference' => 'reference',
        'remote' => 'remote',
      ),
      'label' => 'Picture',
      'weight' => '-4',
      'description' => '',
      'type' => 'image_fupload_imagefield_widget',
      'module' => 'image_fupload_imagefield',
    ),
  );

  // Translatables
  array(
    t('Description'),
    t('Gallery'),
    t('Picture'),
    t('Related Content'),
  );

  return $fields;
}

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _uf_gallery_context_default_contexts() {
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
        'node_form' => 0,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
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

  $translatables['gallery_node'] = array(
    t('Context on gallery nodes'),
    t('gallery'),
  );

  $export['gallery_node'] = $context;
  return $export;
}

/**
 * Helper to implementation of hook_ctools_plugin_api().
 */
function _uf_gallery_ctools_plugin_api() {
  $args = func_get_args();
  $module = array_shift($args);
  $api = array_shift($args);
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
}

/**
 * Helper to implementation of hook_imagecache_default_presets().
 */
function _uf_gallery_imagecache_default_presets() {
  $items = array(
    'gallery_small' => array(
      'presetname' => 'gallery_small',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '50',
            'height' => '50',
          ),
        ),
      ),
    ),
    'gallery_thumb' => array(
      'presetname' => 'gallery_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '120',
            'height' => '120',
          ),
        ),
      ),
    ),
    'imagefupload_thumb' => array(
      'presetname' => 'imagefupload_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '80',
            'height' => '80',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Helper to implementation of hook_user_default_permissions().
 */
function _uf_gallery_user_default_permissions() {
  $permissions = array();

  // Exported permission: create gallery content
  $permissions[] = array(
    'name' => 'create gallery content',
    'roles' => array(
      '0' => 'content manager',
    ),
  );

  // Exported permission: create picture content
  $permissions[] = array(
    'name' => 'create picture content',
    'roles' => array(
      '0' => 'content manager',
    ),
  );

  // Exported permission: delete any gallery content
  $permissions[] = array(
    'name' => 'delete any gallery content',
    'roles' => array(
      '0' => 'content manager',
    ),
  );

  // Exported permission: delete any picture content
  $permissions[] = array(
    'name' => 'delete any picture content',
    'roles' => array(
      '0' => 'content manager',
    ),
  );

  // Exported permission: delete own gallery content
  $permissions[] = array(
    'name' => 'delete own gallery content',
    'roles' => array(),
  );

  // Exported permission: delete own picture content
  $permissions[] = array(
    'name' => 'delete own picture content',
    'roles' => array(),
  );

  // Exported permission: edit any gallery content
  $permissions[] = array(
    'name' => 'edit any gallery content',
    'roles' => array(
      '0' => 'content manager',
    ),
  );

  // Exported permission: edit any picture content
  $permissions[] = array(
    'name' => 'edit any picture content',
    'roles' => array(
      '0' => 'content manager',
    ),
  );

  // Exported permission: edit own gallery content
  $permissions[] = array(
    'name' => 'edit own gallery content',
    'roles' => array(),
  );

  // Exported permission: edit own picture content
  $permissions[] = array(
    'name' => 'edit own picture content',
    'roles' => array(),
  );

  return $permissions;
}
