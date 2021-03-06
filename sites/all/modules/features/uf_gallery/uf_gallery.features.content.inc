<?php

/**
 * Implementation of hook_content_default_fields().
 */
function uf_gallery_content_default_fields() {
  $fields = array();

  // Exported field: field_gallery_description
  $fields['gallery-field_gallery_description'] = array(
    'field_name' => 'field_gallery_description',
    'type_name' => 'gallery',
    'display_settings' => array(
      'label' => array(
        'format' => 'hidden',
      ),
      'format' => 'default',
      'css-class' => array(),
      'region' => 'disabled',
      'ds_weight' => '-89',
      'teaser' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'default',
        'css-class' => '',
        'region' => 'disabled',
        'type' => 'text',
        'parent_id' => '',
        'field_id' => 'field_gallery_description',
        'label_value' => '',
        'exclude' => 1,
        'weight' => '-89',
      ),
      'sticky' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'default',
        'css-class' => '',
        'region' => 'middle',
        'type' => 'text',
        'parent_id' => '',
        'field_id' => 'field_gallery_description',
        'label_value' => '',
        'exclude' => 0,
        'weight' => '-98',
      ),
      'weight' => '-4',
      'parent' => '',
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
    'semantic_cck_field_settings' => '',
    'widget' => array(
      'rows' => '10',
      'size' => 60,
      'semantic_cck_widget_settings' => NULL,
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

  // Exported field: field_gallery
  $fields['picture-field_gallery'] = array(
    'field_name' => 'field_gallery',
    'type_name' => 'picture',
    'display_settings' => array(
      'label' => array(
        'format' => 'hidden',
      ),
      'format' => 'default',
      'css-class' => array(),
      'region' => 'disabled',
      'ds_weight' => '-90',
      'teaser' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'default',
        'css-class' => '',
        'region' => 'disabled',
        'type' => 'nodereference',
        'parent_id' => '',
        'field_id' => 'field_gallery',
        'label_value' => '',
        'exclude' => 1,
        'weight' => '-90',
      ),
      'small_pictures' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'default',
        'css-class' => '',
        'region' => 'disabled',
        'type' => 'nodereference',
        'parent_id' => '',
        'field_id' => 'field_gallery',
        'label_value' => '',
        'exclude' => 1,
        'weight' => '-98',
      ),
      'weight' => '-4',
      'parent' => '',
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'sticky' => array(
        'label' => array(
          'format' => 'inline',
        ),
        'format' => 'default',
        'css-class' => '',
        'region' => 'header',
        'type' => 'nodereference',
        'parent_id' => '',
        'field_id' => 'field_gallery',
        'label_value' => 'Posted on gallery',
        'exclude' => 0,
        'weight' => '-99',
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
    'semantic_cck_field_settings' => '',
    'widget' => array(
      'node_link' => array(
        'teaser' => 0,
        'full' => 1,
        'title' => 'Add picture to Gallery',
        'hover_title' => '',
        'destination' => 'node',
      ),
      'fallback' => 'select',
      'edit_fallback' => NULL,
      'semantic_cck_widget_settings' => NULL,
      'autocomplete_match' => 'contains',
      'size' => '30',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_gallery][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'filefield_sources' => array(
        'imce' => 'imce',
        'reference' => 'reference',
        'remote' => 'remote',
      ),
      'label' => 'Posted on gallery',
      'weight' => '-4',
      'description' => '',
      'type' => 'nodereference_url',
      'module' => 'nodereference_url',
    ),
  );

  // Exported field: field_picture
  $fields['picture-field_picture'] = array(
    'field_name' => 'field_picture',
    'type_name' => 'picture',
    'display_settings' => array(
      'label' => array(
        'format' => 'hidden',
      ),
      'format' => 'default',
      'css-class' => array(),
      'region' => 'disabled',
      'ds_weight' => '-99',
      'teaser' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'news__colorbox',
        'css-class' => '',
        'region' => 'middle',
        'type' => 'filefield',
        'parent_id' => '',
        'field_id' => 'field_picture',
        'label_value' => '',
        'exclude' => 0,
        'weight' => '-99',
      ),
      'small_pictures' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'content_half__colorbox',
        'css-class' => '',
        'region' => 'middle',
        'type' => 'filefield',
        'parent_id' => '',
        'field_id' => 'field_picture',
        'label_value' => '',
        'exclude' => 0,
        'weight' => '-100',
      ),
      'weight' => '-3',
      'parent' => '',
      'full' => array(
        'format' => 'news__colorbox',
        'exclude' => 0,
      ),
      'sticky' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'news__colorbox',
        'css-class' => '',
        'region' => 'middle',
        'type' => 'filefield',
        'parent_id' => '',
        'field_id' => 'field_picture',
        'label_value' => '',
        'exclude' => 0,
        'weight' => '-98',
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
    'semantic_cck_field_settings' => array(
      'enabled' => 0,
      'field_element' => 'div',
      'field_class' => '',
      'label_element_above' => 'h3',
      'label_class_above' => '',
      'label_suffix_above' => '',
      'label_element_inline' => 'span',
      'label_class_inline' => '',
      'label_suffix_inline' => ':',
      'multiple_items_element' => 'div',
      'multiple_items_class' => '',
      'multiple_item_element' => 'div',
      'multiple_item_class' => '',
      'use_singlevalue_settings' => 0,
      'single_item_element' => 'div',
      'single_item_class' => '',
      'last_every_nth' => '0',
      'first_class' => 'first',
      'last_class' => 'last',
      'striping_classes' => 'odd even',
    ),
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '2M',
      'max_filesize_per_node' => '',
      'max_resolution' => '1400x1400',
      'min_resolution' => '0',
      'alt' => '[title-raw]',
      'custom_alt' => 0,
      'title' => '[title-raw]',
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
        'cck_field_picture_description' => 'cck_field_picture_description',
        'node_description' => 0,
        'imagefield_description' => 0,
        'cck_field_gallery' => 0,
      ),
      'fupload_previewlist_redirecturl' => 'gallery',
      'semantic_cck_widget_settings' => array(
        'use_widget_settings' => 0,
      ),
      'filefield_sources' => array(
        'imce' => 'imce',
        'reference' => 'reference',
        'remote' => 'remote',
      ),
      'label' => 'Picture',
      'weight' => '-3',
      'description' => '',
      'type' => 'image_fupload_imagefield_widget',
      'module' => 'image_fupload_imagefield',
    ),
  );

  // Exported field: field_picture_description
  $fields['picture-field_picture_description'] = array(
    'field_name' => 'field_picture_description',
    'type_name' => 'picture',
    'display_settings' => array(
      'label' => array(
        'format' => 'hidden',
      ),
      'format' => 'default',
      'css-class' => array(),
      'region' => 'disabled',
      'ds_weight' => '-92',
      'teaser' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'default',
        'css-class' => '',
        'region' => 'disabled',
        'type' => 'text',
        'parent_id' => '',
        'field_id' => 'field_picture_description',
        'label_value' => '',
        'exclude' => 1,
        'weight' => '-92',
      ),
      'small_pictures' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'default',
        'css-class' => '',
        'region' => 'disabled',
        'type' => 'text',
        'parent_id' => '',
        'field_id' => 'field_picture_description',
        'label_value' => '',
        'exclude' => 1,
        'weight' => '-90',
      ),
      'weight' => '-2',
      'parent' => '',
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'sticky' => array(
        'label' => array(
          'format' => 'hidden',
        ),
        'format' => 'default',
        'css-class' => '',
        'region' => 'disabled',
        'type' => 'text',
        'parent_id' => '',
        'field_id' => 'field_picture_description',
        'label_value' => '',
        'exclude' => 1,
        'weight' => '-90',
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
    'semantic_cck_field_settings' => array(
      'enabled' => 0,
      'field_element' => 'div',
      'field_class' => '',
      'label_element_above' => 'h3',
      'label_class_above' => '',
      'label_suffix_above' => '',
      'label_element_inline' => 'span',
      'label_class_inline' => '',
      'label_suffix_inline' => ':',
      'multiple_items_element' => 'div',
      'multiple_items_class' => '',
      'multiple_item_element' => 'div',
      'multiple_item_class' => '',
      'use_singlevalue_settings' => 0,
      'single_item_element' => 'div',
      'single_item_class' => '',
      'last_every_nth' => '0',
      'first_class' => 'first',
      'last_class' => 'last',
      'striping_classes' => 'odd even',
    ),
    'widget' => array(
      'rows' => '5',
      'size' => 60,
      'semantic_cck_widget_settings' => array(
        'use_widget_settings' => 0,
      ),
      'default_value' => array(
        '0' => array(
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_picture_description][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Description',
      'weight' => '-2',
      'description' => '',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Description');
  t('Picture');
  t('Posted on gallery');

  return $fields;
}
