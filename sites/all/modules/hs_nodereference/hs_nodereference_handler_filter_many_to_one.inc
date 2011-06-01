<?php
// $Id: hs_nodereference_handler_filter_many_to_one.inc,v 1.1 2009/11/05 05:18:21 jrao Exp $
//require_once 'Log.php';
/**
 * @file
 * This file defines a new filter handler for using Hierarchical Select in
 * exposed node reference filters.
 */

class hs_nodereference_handler_filter_many_to_one extends content_handler_filter_many_to_one {
  function has_extra_options() { return TRUE; }
  
  //based on views_handler_filter_term_node_tid
  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'select');
    $options['nodereference_path'] = array('default' => '');
    $options['content_type'] = array('default' => '');
    
    return $options;
  }
  //based on views_handler_filter_term_node_tid and hs_nodereference_views_handler_filter_term_node_tid.inc
  function extra_options_form(&$form, &$form_state) { 
    $filter_id  = $this->options['id'];
    $display_id = _hs_nodereference_views_get_display_id_for_filter($this->view, $filter_id);
    $filter = _hs_nodereference_views_get_filter($this->view, $display_id, $filter_id);
    $field = _hs_nodereference_views_get_field_for_filter($filter);    
    $types = content_types();
  
    foreach ($types as $type => $info) {
      if (isset($info['fields'][$field['field_name']])) {
        $options[$type] = t($info['name']);
      }
    }    

    $form['content_type'] = array(
      '#prefix' => '<div class="views-left-40">',
      '#suffix' => '</div>',
      '#type' => 'radios',
      '#title' => t('Content Type'),
      '#options' => $options,
      '#description' => t('Select which content type to use when using Hierarchical Select.'),
      '#default_value' => $this->options['content_type'],
      '#dependency' => array('radio:options[type]' => array('hierarchical_select')),
    );  

    $form['markup_start'] = array(
      '#value' => '<div class="views-left-40">',
    );

    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Selection type'),
      '#options' => array('select' => t('Dropdown'), 'hierarchical_select' => t('Hierarchical Select')),
      '#default_value' => $this->options['type'],
    );

    
    $filter_id  = $form_state['id'];
    $display_id = _hs_nodereference_views_get_display_id_for_filter($this->view, $filter_id);
    $form['configure_hs'] = array(
      '#prefix'     => '<div class="form-item"><div id="configure-hierarchical-select-link" style="display: inline-block">',
      '#value'      => l(t('Configure Hierarchical Select'), _hs_nodereference_views_config_path($this->view->name, $display_id, $filter_id)),
      '#suffix'     => '</div></div>',
      // Doesn't work because #process is not called for #type = markup form items.
      '#process'    => array('views_process_dependency'),
      '#dependency' => array('radio:options[type]' => array('hierarchical_select')),
      // Set #input = TRUE so that #process will be called.
      '#input'      => TRUE,
      // Set #id manually because that is not generated for #type = markup.
      // This is the same id attribute as in #prefix.
      '#id'         => 'configure-hierarchical-select-link',
    );

    $form['markup_end'] = array(
      '#value' => '</div>',
    );
  }
  
  
  function init(&$view, $options) {
    parent::init($view, $options);

    // The following code should logically be wrapped in a
    //   $this->select_type_is_hierarchical_select()
    // check. However, if we'd do this, you wouldn't be able to dynamically
    // switch between Hierarchical Select and another Selection Type.
    // This incurs a possibly unnecessary load (i.e. loading unneeded JS), but
    // is worth it because of the ease of use and because it's likely that
    // Hierarchical Select will be used, otherwise the user shouldn't install
    // this module in the first place.
    // If you can live with a reload of the View edit form, you can wrap the
    // code below in such a check.

    // Add JS and CSS required for Hierarchical Select to work.
    _hierarchical_select_setup_js();

    // Ensure that Drupal.HierarchicalSelect.prepareGETSubmit() gets called.
    module_load_include('inc', 'hierarchical_select', 'includes/common');
    hierarchical_select_common_add_views_js();
  }

  function value_form(&$form, &$form_state) {
    $view_name  = $this->view->name;
    $filter_id  = $this->options['id'];
    $display_id = _hs_nodereference_views_get_display_id_for_filter($this->view, $filter_id);
    $filter = _hs_nodereference_views_get_filter($this->view, $display_id, $filter_id);
    $field = _hs_nodereference_views_get_field_for_filter($filter);

    $config_id = "nodereference-views-$view_name-$display_id-$filter_id";

    // When not exposed: settings form.
    if (empty($form_state['exposed'])) {
      // When the 'Selection type' is 'Hierarchical Select', user our own
      // value_form, otherwise use the parent's class form.
      if (!$this->select_type_is_hierarchical_select()) {
        parent::value_form($form, $form_state);
      }
      else {
        // we still use normal dropdown for value, use hs for nodereference_path
        parent::value_form($form, $form_state);
        $form['nodereference_path'] = array(
          '#type' => 'hierarchical_select',
          '#title' => t('Node reference path'),
          '#size' => 1,
          '#required' => 0,
          '#config' => array(
            'module' => 'hs_nodereference',
            'params' => array(
              'nodereference_path' => FALSE,
              'type_name' => $this->options['content_type'],
              'field_name' => $field['field_name'],
            ),
            'save_lineage'    => 0,
            'enforce_deepest' => 0,
            'entity_count'    => 0,
            'require_entity'  => 0,
            'resizable'       => 0,
            'level_labels' => array(
              'status' => 0,
            ),
            'dropbox' => array(
              'status'   => 0,
            ),
            'editability' => array(
              'status'   => 0,
            ),
            // These settings cannot be configured through the UI: they can only be
            // overridden through code.
            'animation_delay'    => 400,
            'special_items'      => array(),
            'render_flat_select' => 0,
            'path'               => "hs_nodereference_views_json/$view_name/$display_id",
          ),      
          '#default_value' => isset($this->options['nodereference_path']) ? $this->options['nodereference_path'] : array(),
        );        
      }
    }
    // When exposed: filter form.
    else {
      // When the 'Selection type' is 'Hierarchical Select', user our own
      // value_form, otherwise use the parent's class form.
      if (!$this->select_type_is_hierarchical_select()) {
        parent::value_form($form, $form_state);
      }
      else {
        $optional   = $this->options['expose']['optional'];
        $default_value = array();
        if (isset($this->value) && !empty($this->value)) {
          if (is_array($this->value)) {
            // initial value comes from parent's value_form, which is something
            // like [1] => xxx, this would mess up HS' logic, so we need to normalize it
            foreach ($this->value as $key => $value) {
              $default_value[] = $value; 
            }
          }
          else {
            $default_value = $this->value;
          }
        }
        // Basic settings for the form item.
        $form['value']['#type']          = 'hierarchical_select';
        $form['value']['#default_value'] = $default_value;
        $form['value']['#required']      = !((bool) $optional);

        // Apply the Hierarchical Select configuration to the form item.
        $defaults_override = array(
         'module' => 'hs_nodereference_views',
         'params' => array(
           'optional'    => (bool) $optional,
           'filter_id'   => $filter_id,
           'nodereference_path'         => $this->options['nodereference_path'],
          ),
         // When the 'Any' option is selected, nothing else should be and it
         // should replace the '<none>' option.
         'special_items' => array(
          HS_NODEREFERENCE_VIEWS_ANY_OPTION => array('none', 'exclusive'),
          ),
         // This is a GET form, so also render the flat select.
         'render_flat_select' => 1,
         // Use our custom callback.
         'path' => "hs_nodereference_views_json/$view_name/$display_id",
        );
        hierarchical_select_common_config_apply($form['value'], $config_id, $defaults_override);
      }
    }
  }
  
  function expose_form_right(&$form, &$form_state) {
    // The form with the "Optional", "Force single" and "Remember" checkboxes.
    parent::expose_form_right($form, $form_state);

    // When the selection type is Hierarchical Select, remove the "Force
    // single" checkbox.
    if ($this->select_type_is_hierarchical_select()) {
      unset($form['expose']['single']);
    }
  }

  /**
   * Validate the exposed filter form
   */
  function exposed_validate(&$form, &$form_state) {
    // No validation necessary when the filter is not exposed or when no
    // identifier is set.
    if (empty($this->options['exposed']) || empty($this->options['expose']['identifier'])) {
      return;
    }

    if (!$this->select_type_is_hierarchical_select()) {
      parent::exposed_validate($form, $form_state);
    }
    else {
      $identifier = $this->options['expose']['identifier'];
      $input = $form_state['values'][$identifier];

      // If the HS_NODEREFERENCE_VIEWS_ANY_OPTION option is selected, then the
      // results should not be filtered at all.
      if (!is_array($input)) { // When filtering by single node.
        if ($input != HS_NODEREFERENCE_VIEWS_ANY_OPTION) {
          // views handler expects an array as input value
          $this->validated_exposed_input = array($input);
        }
      }
      else { // When filtering by a multiple nodes.
        if (!in_array(HS_NODEREFERENCE_VIEWS_ANY_OPTION, $input)) {
          $this->validated_exposed_input = $input;
        }
      }
    }
  }

  /**
   * Take input from exposed filters and assign to this handler, if necessary.
   */
  function accept_exposed_input($input) {
    if (!$this->select_type_is_hierarchical_select()) {
      return parent::accept_exposed_input($input);
    }
    else {
      // copied from views_handler_filter_term_node_tid
      if (empty($this->options['exposed'])) {
        return TRUE;
      }
  
      // If it's optional and there's no value don't bother filtering.
      if ($this->options['expose']['optional'] && empty($this->validated_exposed_input)) {
        return FALSE;
      }
  
      $rc = parent::accept_exposed_input($input);
      if ($rc) {
        // If we have previously validated input, override.
        if (isset($this->validated_exposed_input)) {
          $this->value = $this->validated_exposed_input;
        }
      }
      return $rc;      
    }
  }

  function admin_summary() {
    $this->value = (array) $this->value;
    return parent::admin_summary();
  }

  /**
   * Check whether the "Selection type" (in the configuration of the exposed
   * filter) is Hierarchical Select.
   *
   * This function is used in almost every overridden method to determine
   * whether our custom logic should be used or the parent class's, i.e. the
   * parent method in the content_handler_filter_many_to_one class.
   */
  function select_type_is_hierarchical_select() {
    return $this->options['type'] == 'hierarchical_select';
  }
}
