<?php

/**
 * @file
 * Implements the the actual filter.
 */

/**
 * Filter by group
 */
class views_filtergroup_handler_filter_group extends views_handler_filter {
  // Don't display empty space where the operator would be.
  var $no_operator = TRUE;

  function members() {
    return isset($this->options['group_members']) ? $this->options['group_members'] : array();
  }

  function operator_form(&$form, &$form_state) {
    $options = array();
    foreach ($this->view->get_items('filter') as $id => $filter) {
      if (!empty($filter['exposed']) && $filter['expose']['optional'] && $id != $this->options['id']) {
        $options[$id] = $filter['expose']['label'];
      }
    }

    $form['group_members'] = array(
      '#type' => 'select',
      '#title' => t('Group Members'),
      '#multiple' => TRUE,
      '#description' => t('Determines the members of this group. Only one filter will be active at a time. All group members have to be optional.'),
      '#options' => $options,
      '#default_value' => $this->members(),
    );
  }


  function value_form(&$form, &$form_state) {
    $options = array();
    $filters = $this->view->get_items('filter');
    foreach ($this->members() as $id) {
      $options[$filters[$id]['expose']['identifier']] = $filters[$id]['expose']['label'];
    }

    $form['value'] = array(
      '#title' => t('Default selection'),
      '#type' => 'select',
      '#description' => t('Controls the default selection of the filter dropdown.'),
      '#options' => $options,
      '#default_value' => $this->options['value'],
    );
  }

  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);

    if (!empty($this->options['expose']['identifier'])) {
      $id = $this->options['expose']['identifier'];
      unset($form[$id]['#description']);
      $form[$id]['#process'] = array('views_filtergroup_exposed_grouping');
    }
  }

  function exposed_submit(&$form, &$form_state) {
    if (!empty($this->options['expose']['identifier'])) {
      $id = $this->options['expose']['identifier'];

      $filters = $this->view->get_items('filter');

      if (!empty($this->view->exposed_input[$id])) {
        // Remove the values of the non-selected field so that it goes into
        // the optional state.
        foreach ($this->members() as $filter_name) {
          $name = $filters[$filter_name]['expose']['identifier'];
          if ($name != $this->view->exposed_input[$id]) {
            $form_state['values'][$name] = '';
          }
        }
      }
    }
  }

  function query() {
    // No querying.
  }
}

function views_filtergroup_exposed_grouping($element) {
  drupal_add_js(drupal_get_path('module', 'views_filtergroup') .'/views_filtergroup.js');

  $element['#attributes'] = array('class' => 'views-filtergroup-selector');
  return $element;
}