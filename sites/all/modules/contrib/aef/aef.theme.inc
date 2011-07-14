<?php

/**
 * @file aef.theme.inc
 *
 * Variables definitions to use in aef-dynamic.tpl.php
 */

function template_preprocess_aef_dynamic(&$vars) {
  $form = &$vars['form'];
  $view = $form['#parameters'][1]['view'];

  // Create variables for filters to select
  foreach ($form['#info'] as $id => $filter) {
    $vars['select_filter'][$id] = drupal_render($form['aef_selected_filter'][$filter['value']]);
  }
  $vars['reset_button'] = drupal_render($form['reset']);
  $vars['all_groups_operator'] = drupal_render($form['all_groups_operator']);
  $vars['aef_id'] = views_css_safe("aef_{$view->name}_{$view->current_display}");
  $vars['form_id'] = views_css_safe("{$view->name}_{$view->current_display}");
  $vars['new_group_button'] = drupal_render($form['new_group']);
  $vars['refresh_button'] = drupal_render($form['refresh']);

  $exposed_form = $view->display_handler->get_plugin('exposed_form');
  $vars['selected_all_group_op'] = $exposed_form->session['all_groups_operator'];

  $types = array('where', 'group_by');
  foreach ($types as $type) {
    $vars[$type . '_groups'] = array();
    $groups = $exposed_form->get_sorted_filters($type);
    if ($type == 'where') {
      $groups_count = count($groups);

      $vars['defined_groups'] = $exposed_form->get_groups($type);
      $in_group_options = array();
      foreach ($vars['defined_groups'] as $gid => $op) {
        $in_group_options[$gid] = t('Group @group', array('@group' => $gid));
      }
    }
    if (count($groups)) {
      foreach ($groups as $id => $group) {
        if ($type == 'where') {
          $vars['put_in_group'][$id] = drupal_render($form['put_in_group'][$id]);
        }
        $vars['group_operator'][$id] = drupal_render($form['group_operator_' . $id]);
        if (!isset($vars[$type . '_groups'][$id])) {
          $vars[$type . '_groups'][$id] = array();
        }
        foreach ($group as $filter) {
          if (!isset($vars[$type . '_groups'][$filter['gr']])) {
            $vars[$type . '_groups'][$filter['gr']] = array();
          }
          $operator_form = $view->filter[$filter['fi']]->options['expose']['operator'];
          $region = $filter['gr'];
          $delta = $filter['id'];

          $form['filter_' . $delta]['group']['#options'] = $in_group_options;

          $vars[$type . '_groups'][$filter['gr']][] = array(
           'delete' => drupal_render($form['delete_' . $filter['id']]),
           'weight' => drupal_render($form['filter_' . $delta]['weight']),
           'group' => drupal_render($form['filter_' . $delta]['group']),
           'filter' => check_plain($view->filter[$filter['fi']]->options['expose']['label']),
           'operator' => !empty($form[$operator_form]['#options']) ? check_plain($form[$operator_form]['#options'][$filter['op']]) : '',
           'value' => check_plain($exposed_form->process_value($filter)),
          );
        }
      }
    }
  }

  /**
   * Now that we know how many filters are in $_SESSION, we have to
   * remove the extra forms added if any.
   */
  foreach ($form as $key => $item) {
    if (strpos($key, 'delete_') !== FALSE) {
      if (empty($item['#printed'])) {
        $id = str_replace('delete_', '', $key);
        unset($form[$key]);
        unset($form['filter_' . $id]['group']);
        unset($form['filter_' . $id]['weight']);
      }
    }
    elseif (strpos($key, 'group_operator_') !== FALSE) {
      $id = str_replace('group_operator_', '', $key);
      unset($form['put_in_group'][$id]);
      unset($form['group_operator_' . $id]);
    }
  }

  // Call to parent function the render the others
  // elements of the exposed form.
  template_preprocess_views_exposed_form($vars);
}

