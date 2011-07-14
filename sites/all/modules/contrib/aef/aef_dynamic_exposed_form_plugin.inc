<?php

class aef_dynamic_exposed_form_plugin extends views_plugin_exposed_form_basic {
  var $session;
  var $last_filter_id;
  var $start_group_id;

  /**
   * Overriden methods from Exposed Form Basic.
   */

  function init(&$view, &$display, $options) {
    parent::init($view, $display, $options);

    $this->session = &$_SESSION['aef'][$this->view->name][$this->view->current_display];

    $this->last_filter_id = NULL;
    if (empty($this->session)) {
      $this->session = array(
        'filters' => array(),
        'groups' => array(),
        'all_groups_operator' => 'AND',
      );
    }

    // For each exposed filter we delete the exposed data.
    // This emulate an empty exposed filter, so it is not executed
    if (is_array($this->view->filter)) {
      foreach ($this->view->filter as $id => $filter) {
        if ($filter->is_exposed()) {
          $this->view->exposed_data[$id] = array();
        }
      }
    }
  }

  function summary_title() {
    return t('Dynamic exposed filters');
  }

  /**
   * Override exposed_form_alter function to add
   * new controls to the exposed form.
   */
  function exposed_form_alter(&$form, &$form_state) {
    parent::exposed_form_alter($form, $form_state);

    $form['#theme'] = 'aef_dynamic';
    $form['#tree'] = TRUE;
    // Radio box to select the filter to apply to the view
    $form['aef_selected_filter'] = array(
      '#type' => 'radios',
    );

    // Go through each handler to get its label.
    foreach ($this->view->display_handler->handlers as $type => $value) {
      foreach ($this->view->$type as $id => $handler) {
        if ($handler->can_expose() && $handler->is_exposed()) {
          if ($info = $handler->exposed_info()) {
            if ($type == 'filter') {
              $filters[$info['value']] = check_plain($info['label']);
            }
          }
        }
      }
    }
    $form['aef_selected_filter']['#options'] = $filters;

    // Build select box to define group operators
    $groups = $this->get_groups();

    /* Views build the exposed form in this way:
     *
     * First, call to views_exposed_form()
     * Next, call to views_exposed_form_submit()
     * Finally, render the form using theme.inc
     *
     * When user click on apply, views call again to views_exposed_form
     * before call views_exposed_form_submit, for this reason, we cannot
     * get the values storaged in the $_SESSION variable
     * to build new form elements. The solution is create an extra form
     * even it is not necessary and remove it in the render step if it
     * is not needed.
     */

    //add the extra group
    $groups[] = array(
      'op' => 'AND',
    );

    $delete_img_path = '/'. drupal_get_path('module', 'aef') .'/images/delete.png';
    $in_group = array();
    foreach ($groups as $id => $group) {
      $form['group_operator_' . $id] = array(
        '#type' => 'select',
        '#options' => array(
          'AND' => t('And'),
          'OR' => t('Or'),
        ),
        '#default_value' => $group['op'],
        '#attributes' => array(
          'class' => 'warning-on-change',
        ),
      );
      $in_group[$id] = '';
      $move_group[$id] = t('Group @group', array('@group' => $id));
    }

    // Create the same group operator but for Group By filters
    $form['group_operator_group_by'] = array(
        '#type' => 'select',
        '#options' => array(
          'AND' => t('And'),
          'OR' => t('Or'),
        ),
        '#default_value' => $group['op'],
        '#attributes' => array(
          'class' => 'warning-on-change',
        ),
      );

    // Build radio buttons to define where put the new filter
    if (count($in_group)) {
      $form['put_in_group'] = array(
        '#type' => 'radios',
        '#options' => $in_group,
        '#default_value' => 0,
      );
    }

    // For each filter in session, we have to provide a delete button
    // and a select box to move the filter to other group
    $filters = $this->session['filters'];

    // For the same reason, add an extra filter, this filter represent
    // the possible new filter applied to the view
    $filters[] = array(
      'id' => $this->get_next_filter_id(),
      'gr' => 0, // TODO: See how we can get the last group selected
      '#weight' => 50,
    );

    $form['filter_in_group'] = array();
    foreach ($filters as $id => $filter) {
      // We use $filter['id'] instead of $id since $id is lost when
      // filters are sorted by weight
      $form['delete_' . $filter['id']] = array(
        '#type' => 'submit',
        '#value' => t('Delete @filter', array('@filter' => $filter['id'])),
        '#attributes' => array(
          'src' =>  $delete_img_path,
          'title' => t('Delete this filter'),
          'class' => 'icon-button',
        ),
        //'#theme' => 'image_button',
      );

      $form['filter_' . $filter['id']]['group'] = array(
        '#type' => 'select',
        '#options' => $move_group,
        '#default_value' => $filter['gr'],
        '#attributes' => array(
          'class' => 'aef-region-select aef-region-' . $filter['gr'],
        ),
      );

      $form['filter_' . $filter['id']]['weight'] = array(
        '#type' => 'weight',
        '#default_value' => $filter['#weight'],
        '#attributes' => array(
          'class' => 'aef-weight aef-weight-' . $filter['gr'],
        ),
      );
    }

    $form['all_groups_operator'] = array(
      '#type' => 'select',
      '#options' => array (
        'AND' => t('And'),
        'OR' => t('Or'),
      ),
      '#default_value' => $this->session['all_groups_operator'],
      '#attributes' => array(
        'class' => 'warning-on-change',
      ),
    );

    $form['new_group'] = array(
      '#type' => 'submit',
      '#value' => t('New group'),
    );

    $form['refresh'] = array(
      '#type' => 'submit',
      '#value' => t('Refresh'),
    );
  }

  function exposed_form_submit(&$form, &$form_state, &$exclude) {
    if ($form_state['clicked_button']['#name'] == 'op' && $form_state['clicked_button']['#value'] == t('Reset')) {
      $form_state = array();
      $form_state['values'] = array();
      $this->reset_form($form, $form_state);
      return;
    }
    if ($form_state['clicked_button']['#name'] == 'op' && $form_state['clicked_button']['#value'] == t('New group')) {
      $this->add_group();
      return;
    }
    if (!empty($form_state['clicked_button']['#id']) && strpos($form_state['clicked_button']['#id'], 'edit-delete-') !== FALSE) {
      $filter_id = intval(str_replace('edit-delete-', '', $form_state['clicked_button']['#id']));
      $this->remove_filter($filter_id, $form, $form_state);
      return;
    }

    $form_state['view']->exposed_raw_input['selected_filter'] = $form_state['values']['aef_selected_filter'];

    // Storage group operators
    $groups_operators = array();
    foreach ($form_state['values'] as $id => $value) {
      $expode = explode('_', $id);
      if (count($explode) == 2) {
        list($str, $gr) = $explode;
        if ($str = 'group' && is_numeric($gr)) {
          $groups_operators[$gr] = $value;
        }
      }
    }
    $form_state['view']->exposed_raw_input['group_operators'] = $groups_operators;

    // Storage all group operators
    $form_state['view']->exposed_raw_input['all_groups_operator'] = $form_state['values']['all_groups_operator'];


    // Storage exposed data, this will be done twice since
    // exposed_form_submit() is called before this
    // in views_exposed_form_submit but this is not a big deal
    foreach ($form_state['values'] as $key => $value) {
      if (!in_array($key, $exclude)) {
        $this->view->exposed_raw_input[$key] = $value;
      }
    }

    if ($form_state['clicked_button']['#name'] == 'op' && $form_state['clicked_button']['#value'] == t('Refresh')) {
     $this->refresh();
      return;
    }

    // Check submit options are not being called form pager
    if (!empty($GLOBALS['_SERVER']['HTTP_REFERER']) && empty($_GET['page'])) {
      $use_ajax = $this->view->display_handler->get_option('use_ajax');
      if (!$use_ajax || ($use_ajax && empty($_GET['js']))) {
        $clicked_button = $form_state['clicked_button']['#name'];
        if (empty($clicked_button) && $form_state['clicked_button']['#type'] == 'submit') {
          //$this->add_filter_to_session();
          $this->add_selected_filter();
        }
      }
    }
  }

  function query() {
    // apply exposed filters
    $this->apply_exposed_filters_from_session();

    // update the all group operator value
    $this->view->query->group_operator = $this->session['all_groups_operator'];
  }

  /**
   * Override default reset_form function.
   */
  function reset_form(&$form, &$form_state) {
    unset($_SESSION['aef'][$this->view->name][$this->view->current_display]);
    parent::reset_form($form, $form_state);
  }

  function render_exposed_form() {
    // apply non exposed filters and arguments
    $this->relocate_handlers();
    return parent::render_exposed_form();
  }

  /**
   * Helper methods.
   */

  /**
   * Filters
   */

  function get_next_filter_id() {
    if (count($this->session['filters']) > 0) {
      foreach ($this->session['filters'] as $filter) {
        if ($last < $filter['id']) {
          $last = $filter['id'];
        }
      }
      return $last + 1;
    }
    return 0;
  }

  /**
   * When apply is clicked, the selected filter should be added to the
   * query. However, aef have to save this value in the correct group
   * and only save this filter if a filter was selected from the
   * radio button filter selector.
   */
  function add_selected_filter() {
    $selected_filter = $this->view->filter[$this->view->exposed_raw_input['selected_filter']];
    $identifier = $selected_filter->options['expose']['identifier'];
    $operator = $this->view->exposed_raw_input[$selected_filter->options['expose']['operator']];
    $value = $this->view->exposed_raw_input[$identifier];
    $group = $this->view->exposed_raw_input['put_in_group'];
    $weight = $this->view->exposed_raw_input['weight_' . $this->get_next_filter_id()];
    if (!$identifier) {
      return;
    }
    $this->add_filter($identifier, $operator, $value, $weight, $group);
  }

  /**
   * Add a new filter to the filter set
   *
   * @param $identifier
   *  The identifier to indentify the filter to add.
   *
   * @param $operator
   *  If unlock operator is set, $operator contains the operator to use
   *  with this filter.
   *
   * @param $value
   *  An array with the submited values for this exposed filter.
   *
   * @group $group
   *  Optional, the group to put this filter. If none is set filter will
   *  be saved into group 0.
   */
  function add_filter($identifier, $operator, $value, $weight, $group = 0) {
    if (isset($this->view->filter[$identifier])) {
      $handler = $this->view->filter[$identifier];
    }

    if (!$handler) {
      return;
    }

    $id = $this->get_next_filter_id();

    // In order to simplify UI group by filters will be storaged in an
    // unique group
    if ($handler->options['group_type'] != 'group') {
      $group = 'group_by'; // use an non numeric id to isolate from where groups
    }

    $filter = array(
      'fi' => $identifier,      // Filter identifier
      'op' => $operator,        // Filter operator
      'va' => $value,           // Exposed filter values
      'gr' => $group,           // Group where put this filter
      'id' => $id,              // Unique id for this filter
      '#weight' => 50,          // Weight to sort filters in a group
    );

    $this->session['filters'][] = $filter;
    if (!$this->group_exists($group)) {
      $this->add_group($group, 'AND');
    }
  }

  function get_sorted_filters($type = 'where') {
    $groups = array();
    foreach ($this->session['groups'] as $id => $group_info) {
      if (($type == 'where' && $id !== 'group_by') || ($type != 'where' && $id === 'group_by')) {
        $groups[$id] = array();
      }
    }
    $return = array();
    if (count($this->session['filters'])) {
      foreach ($this->session['filters'] as $filter) {
        // variable $gr, only to make this code more readable
        $gr = $filter['gr'];
         // Retrive only filters for the specified $type
        if (($type == 'where' && $gr !== 'group_by') || ($type != 'where' && $gr === 'group_by')) {
          if (!isset($groups[$gr])) {
            $groups[$gr] = array();
          }
          $groups[$gr][] = $filter;
        }
      }
      foreach ($groups as $id => $group) {
        $return[$id] = $group;
        uasort($return[$id], 'element_sort');
      }
    }
    return $return;
  }

  function set_filter_weight($id, $weight) {
    foreach ($this->session['filters'] as $key => $filter) {
      if ($filter['id'] == $id) {
        $this->session['filters'][$key]['#weight'] = $weight;
        break;
      }
    }
  }

  function set_filter_group($id, $group) {
    foreach ($this->session['filters'] as $key => $filter) {
      if ($filter['id'] == $id) {
        $this->session['filters'][$key]['gr'] = $group;
        break;
      }
    }
  }

  function remove_filter($id, $form = NULL, $form_state = NULL) {
    foreach ($this->session['filters'] as $key => $filter) {
      if ($id == $filter['id']) {
        unset($this->session['filters'][$key]);
        break;
      }
    }
    // If a user delete all filters, reset the form.
    // This is more intuitive than leave a lot of empty groups in the UI
    if (count($this->session['filters']) == 0 && $form && $form_state) {
      $this->reset_form($form, $form_state);
    }
  }

  /**
   * Groups
   */

  function group_exists($id) {
    return isset($this->session['groups'][$id]);
  }

  /**
   * Add a new group to the filter set, groups
   * are used to isolate filters.
   */
  function add_group($id = NULL, $operator = 'AND') {
    if (!$id) {
      $groups = $this->session['groups'];
      if (isset($groups['group_by'])) {
        unset($groups['group_by']);
      }
      $id = count($groups);
    }
    $this->session['groups'][$id] = array(
      'op' => $operator,    // Group operator
    );
  }

  function set_group_operator($id, $operator) {
    if (!empty($this->session['groups'][$id])) {
      $this->session['groups'][$id]['op'] = $operator;
    }
    else {
      $this->add_group($id, $operator);
    }
  }

  function get_group_operator($id) {
    if (!empty($this->session['groups'][$id])) {
      return $this->session['groups'][$id]['op'];
    }
  }

  /**
   * Remove a group and all its filters.
   */
  function remove_group($id) {
    if (isset($this->session['groups'][$id])) {
      unset($this->session['groups'][$id]);

      // When a group is removed, some filters may be updated
      // Filters in erased group must be deleted
      // and filters in group with id greather than erased must be
      // moved one group
      foreach ($this->session['filters'] as $key => $filter) {
        if ($filter['gr'] == $id) {
          unset($this->session['filters'][$key]);
        }
        elseif ($filter['gr'] > $id) {
          $this->session['filters'][$key]['gr'] -= 1;
        }
      }

      // update also the groups_operator array
      $tmp_groups_operator = array();
      foreach ($this->session['groups'] as $gid => $group) {
        if ($gid != $id) {
          $tmp_groups_operator[] = $group;
        }
      }
      $this->$this->session['groups'] = $tmp_groups_operator;
    }
  }

  /**
   * Returns avaible groups for a given type
   */
  function get_groups($type = 'where') {
    if (isset($this->session['groups'])) {
      $return = $this->session['groups'];
      if ($type == 'where') {
        unset($return['group_by']);
        return $return;
      }
      else {
        return array('group_by' => $this->session['groups']['group_by']);
      }
    }
  }

  function refresh() {
    // Update filter weights and groups
    if (is_array($this->session['filters'])) {
      foreach ($this->session['filters'] as $filter) {
        $this->set_filter_weight($filter['id'], $this->view->exposed_raw_input['filter_' . $filter['id']]['weight']);
        $this->set_filter_group($filter['id'], $this->view->exposed_raw_input['filter_' . $filter['id']]['group']);
      }
    }

    // Update groups operators
    foreach ($this->session['groups'] as $id => $group_info) {
      $this->set_group_operator($id, $this->view->exposed_input['group_operator_' . $id]);
    }

    // Update all group operator
    $this->session['all_groups_operator'] = $this->view->exposed_input['all_groups_operator'];
  }

  /**
   * In order to group WHERE clauses into sql, handlers must be reorganized
   * into isolated groups:
   *  HAVING filters going into Having Group
   *  Arguments going into Group 0
   *  Non exposed filters into Grup 1
   *  Exposed filters into groups greater or equals than 2
   */
  function relocate_handlers()  {
    // Create the two groups for non exposed handlers and arguments
    $group_for_argument = 0;
    $group_for_non_exposed_filters = 0;
    foreach ($this->view->argument as $id => $handler) {
      if ($handler->options['group_type'] == 'group') {
        $this->view->argument[$id]->options['group'] = 0;
        $group_for_argument = 1;
      }
    }

    foreach ($this->view->filter as $id => $handler) {
      if ($handler->options['group_type'] == 'group') {
        if (empty($handler->options['exposed'])) {
          $this->view->filter[$id]->options['group'] = $group_for_argument;
          $group_for_non_exposed_filters = 1;
        }
      }
    }

    $this->start_group_id =  $group_for_argument + $group_for_non_exposed_filters;
  }

  /**
   * Every time query is built, views process exposed filters one by one
   * Since this module allows apply a same exposed filter multiple times
   * we permorm the same actions than views::build() but taking the data
   * from SESSION instead of $view->exposed_data .
   */
  function apply_exposed_filters_from_session() {
    $types = array('where', 'group_by');
    foreach ($types as $type) {
      $groups = $this->get_sorted_filters($type);
      foreach ($groups as $id => $group) {
        foreach ($group as $filter) {
          $handler = $this->view->filter[$filter['fi']];
          if ($handler) {
            $this->view->exposed_data = array();
            $this->view->exposed_data[$filter['fi']] = $filter['va'];
            $this->view->exposed_data[$handler->options['expose']['operator']] = $filter['op'];
            // Give this handler access to the exposed filter input.
            if (!empty($this->view->exposed_data)) {
              $rc = $handler->accept_exposed_input($this->view->exposed_data);
              $handler->store_exposed_input($this->view->exposed_data, $rc);
              if (!$rc) {
                continue;
              }
            }
            if ($type == 'where') {
              $handler->options['group'] = $this->start_group_id + $id;
            }
            $handler->set_relationship();
            $handler->query($this->view->display_handler->use_group_by());
          }
        }
        if ($type == 'where' && isset($this->view->query->where[$this->start_group_id + $id])) {
          // Update group operator
          $this->view->query->set_where_group($this->session['groups'][$id]['op'], $this->start_group_id + $id);
        }
      }
    }
  }

  /**
   * TODO: Make more research about this ugly function.
   *
   * This function try to return a more human readable value when a filter
   * is applied.
   *
   * Since views never displayed before this kind of values outside exposed
   * forms elements, we have to try to approximate the value with this function.
   *
   * For now only support numeric, and strings values with multiple and single
   * values.
   */
  function process_value($filter) {
    $handler = $this->view->filter[$filter['fi']];

    $raw_value = $filter['va'];

    if (is_array($raw_value)) {
      if (isset($handler->value_options)) {
        $return = array();
        foreach ($handler->value_options as $key => $label) {
          if (isset($raw_value[$key])) {
            $return[] = $label;
          }
        }
        return implode(', ', $return);
      }
      else {
        foreach ($raw_value as $key => $raw) {
          if (trim($raw) == '') {
            unset($raw_value[$key]);
          }
        }
        return implode(' ' . t('and') . ' ', $raw_value);
      }
    }
    else {
      if (isset($handler->value_options) && !empty($handler->value_options[$raw_value])) {
        return $handler->value_options[$raw_value];
      }
    }
    return $raw_value;
  }

}
