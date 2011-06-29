<?php
// $Id: cck_referential_integrity.admin.inc,v 1.1.2.3 2009/09/12 05:15:47 markuspetrux Exp $

/**
 * @file
 * Implementation of the administration pages of the module.
 */

/**
 * Alter the field settings form for reference fields.
 */
function _cck_referential_integrity_content_field_edit_form_alter(&$form, $form_state, $form_id) {
  if (in_array($form['#field']['type'], array('nodereference', 'userreference'))) {
    $field_name = $form['#field']['field_name'];
    $field_entities = ($form['#field']['type'] == 'nodereference' ? t('nodes') : t('users'));
    $form['field']['cck_referential_integrity_behavior'] = array(
      '#type' => 'radios',
      '#title' => t('Referential integrity behavior'),
      '#options' => cck_referential_integrity_get_behavior_options($field_entities),
      '#default_value' => variable_get('cck_referential_integrity_behavior_'. $field_name, ''),
      '#description' => t('This option defines the referential integrity behavior that will be enforced upon deletion of @entities referenced from this field.', array('@entities' => $field_entities)),
    );
    $form['#submit'][] = '_cck_referential_integrity_content_field_edit_form_submit';
  }
}

/**
 * Submit handler for nodereference field settings form.
 */
function _cck_referential_integrity_content_field_edit_form_submit($form, &$form_state) {
  $field_name = $form['#field']['field_name'];
  $form_values = $form_state['values'];
  variable_set('cck_referential_integrity_behavior_'. $field_name, $form_values['cck_referential_integrity_behavior']);
}

/**
 * Menu callback for the orphan references report page.
 */
function cck_referential_integrity_orphan_references($field_name = NULL) {
  if (!empty($field_name)) {
    drupal_set_breadcrumb(array(
      l(t('Home'), NULL),
      l(t('Content'), 'admin/content/node'),
      l(t('Orphan references'), 'admin/content/node/orphan-references'),
    ));
    return drupal_get_form('cck_referential_integrity_orphan_references_check_one_field', $field_name);
  }
  return cck_referential_integrity_orphan_references_check_all_fields();
}

/**
 * Build the orphan references report for all fields.
 */
function cck_referential_integrity_orphan_references_check_all_fields() {
  // Include common module functions.
  module_load_include('inc', 'cck_referential_integrity');

  $field_status_titles = array(
    'ok' => t('No orphan records have been found for this field.'),
    'warning' => t('Orphan records have been found for this field.'),
  );

  // Load information about all reference fields in the system.
  $reference_fields = array();
  foreach (array('nodereference', 'userreference') as $field_type) {
    foreach (cck_referential_integrity_get_reference_fields($field_type) as $field_name => $field_info) {
      $field_info['count'] = cck_referential_integrity_get_records_count($field_info);
      $field_info['orphans'] = cck_referential_integrity_get_orphans_count($field_info);
      $field_info['status'] = ($field_info['orphans'] > 0 ? 'warning' : 'ok');
      $reference_fields[$field_name] = $field_info;
    }
  }

  // Sort the fields by table and column.
  uasort($reference_fields, '_cck_referential_integrity_sort_fields');

  // Render the report.
  drupal_add_css(drupal_get_path('module', 'cck_referential_integrity') .'/css/admin.css', 'module', 'all', FALSE);
  $headers = array(
    array('data' => t('Table')),
    array('data' => t('Column')),
    array('data' => t('Type')),
    array('data' => t('Records'), 'class' => 'cck-referential-integrity-cell-numeric'),
    array('data' => t('Orphans'), 'class' => 'cck-referential-integrity-cell-numeric'),
    array('data' => t('Status'), 'class' => 'cck-referential-integrity-cell-status'),
  );

  $rows = array();
  $fields_with_orphan_records = FALSE;
  foreach ($reference_fields as $field_name => $field_info) {
    $row = array();
    $status = theme('cck_referential_integrity_status_icon', $field_info['status'], $field_status_titles[$field_info['status']]);
    if ($field_info['status'] != 'ok') {
      $fields_with_orphan_records = TRUE;
      $table_cell = l($field_info['table'], 'admin/content/node/orphan-references/'. $field_name);
    }
    else {
      $table_cell = $field_info['table'];
    }
    $row[] = array('data' => $table_cell);
    $row[] = array('data' => $field_info['column'], 'title' => t($field_info['label']) .' ('. $field_name .')');
    $row[] = array('data' => ($field_info['field_type'] == 'nodereference' ? t('Node reference') : t('User reference')));
    $row[] = array('data' => format_number($field_info['count'], 0), 'class' => 'cck-referential-integrity-cell-numeric');
    $row[] = array('data' => format_number($field_info['orphans'], 0), 'class' => 'cck-referential-integrity-cell-numeric');
    $row[] = array('data' => $status, 'class' => 'cck-referential-integrity-cell-status');
    $rows[] = array('data' => $row);
  }

  $rows_count = count($rows);
  if ($rows_count <= 0) {
    $rows[] = array(array('colspan' => count($headers), 'align' => 'center', 'data' => t('Could not find reference fields.')));
  }

  $help = t('This report displays information about the tables used for node and user reference fields.
The <em>Records</em> column is the total number of records stored for each field. The <em>Orphans</em> column is the number of records in these tables that refer to non-existing nodes or users respectively.
A warning icon is displayed on the status column when orphans are found for a particular table, and you can then click on the table name to review the corresponding orphan records.');
  $output = '<div class="help">'. $help .'</div>'."\n";
  $output .= theme('table', $headers, $rows);
  if ($fields_with_orphan_records) {
    $warning = t('Orphan records have been found for some fields. You may now click on the table name to review the corresponding orphan records.');
    $output .= '<div class="messages warning"><span class="warning">*</span> '. $warning .'</div>';
  }
  return $output;
}

/**
 * String compare function for fields.
 */
function _cck_referential_integrity_sort_fields($a, $b) {
  $a = $a['table'] .'.'. $a['column'];
  $b = $b['table'] .'.'. $b['column'];
  return ($a == $b ? 0 : ($a < $b ? -1 : 1));
}

/**
 * Build the orphan references report for just one field.
 */
function cck_referential_integrity_orphan_references_check_one_field(&$form_state, $field_name) {
  $form = array();

  // Include common module functions.
  module_load_include('inc', 'cck_referential_integrity');

  // Load information about the field.
  $field = content_fields($field_name);
  if (empty($field)) {
    drupal_set_message(t('Could not find information about the field %field.', array('%field' => $field_name)), 'warning');
    return $form;
  }
  $form['#field_info'] = $field_info = cck_referential_integrity_get_field_info($field);
  if (empty($field_info)) {
    drupal_set_message(t('Could not find referential integrity information about the field %field.', array('%field' => $field_name)), 'warning');
    return $form;
  }

  $form['keys'] = array(
    '#type' => 'checkboxes',
    '#options' => array(),
    '#records' => array(),
  );

  $sql_query = cck_referential_integrity_build_orphans_query($field_info, FALSE, TRUE);
  $sql_count = cck_referential_integrity_build_orphans_query($field_info, TRUE);

  $result = pager_query($sql_query, CCK_REFERENTIAL_INTEGRITY_ITEMS_PER_PAGE, 0, $sql_count);
  while ($record = db_fetch_object($result)) {
    $record_key = array($record->vid, $record->nid);
    if ($field_info['has_delta']) {
      $record_key[] = $record->delta;
    }
    $record_key = implode(':', $record_key);
    $form['keys']['#options'][$record_key] = '';
    $form['keys']['#records'][$record_key] = $record;
  }
  if (!empty($form['keys']['#options'])) {
    $form['operations'] = array(
      '#type' => 'fieldset',
      '#title' => t('Operations'),
    );
    $form['operations']['backups'] = array(
      '#type' => 'checkbox',
      '#title' => t('Yes, I have backups of my database.'),
      '#prefix' => '<div class="cck-referential-integrity-operations-backups">',
      '#suffix' => '</div>',
    );
    $form['operations']['set_null'] = array(
      '#type' => 'submit',
      '#value' => t('Set the value of selected fields to NULL'),
      '#prefix' => '<div class="cck-referential-integrity-operations-submit">',
      '#suffix' => '</div>',
    );
  }
  return $form;
}

/**
 * Validate handler for the orphan references report for just one field form.
 */
function cck_referential_integrity_orphan_references_check_one_field_validate($form, &$form_state) {
  if (empty($form_state['values']['backups'])) {
    form_set_error('backups', t('Please, make backups before processing orphan records.'));
  }
  $form_state['values']['keys'] = array_filter($form_state['values']['keys']);
  if (count($form_state['values']['keys']) == 0) {
    form_set_error('keys', t('No records selected.'));
  }
}

/**
 * Submit handler for the orphan references report for just one field form.
 */
function cck_referential_integrity_orphan_references_check_one_field_submit($form, &$form_state) {
  $field_info = $form['#field_info'];

  $cached_nids = array();
  $sql_conditions = array();
  $sql_args = array();
  foreach (array_keys($form_state['values']['keys']) as $record_key) {
    $record = $form['keys']['#records'][$record_key];
    if ($field_info['has_delta']) {
      $sql_conditions[] = '(vid = %d AND delta = %d)';
      $sql_args[] = $record->vid;
      $sql_args[] = $record->delta;
    }
    else {
      $sql_conditions[] = '(vid = %d)';
      $sql_args[] = $record->vid;
    }
    $cached_nids[$record->nid] = $record->nid;
  }

  // Update all selected field records at once.
  db_query('UPDATE {'. $field_info['table'] .'} SET '. $field_info['column'] .' = NULL WHERE ('. implode(' OR ', $sql_conditions) .')', $sql_args);

  // Expire cached objects related to the nodes we have just updated.
  if (!empty($cached_nids)) {
    cck_referential_integrity_clear_content_cache($cached_nids, array($field_info['field_name'] => $field_info));
  }

  drupal_set_message(t('The selected records have been processed.'));
}

/**
 * Theme the orphan references report for just one field.
 *
 * @ingroup themeable
 */
function theme_cck_referential_integrity_orphan_references_check_one_field($form) {
  if (empty($form['#field_info'])) {
    return drupal_render($form);
  }
  $field_info = $form['#field_info'];

  drupal_add_css(drupal_get_path('module', 'cck_referential_integrity') .'/css/admin.css', 'module', 'all', FALSE);
  drupal_add_js('misc/tableselect.js');
  $headers = array(
    array('data' => t('Referrer node')),
    array('data' => t('Referrer vid'), 'class' => 'cck-referential-integrity-cell-numeric'),
    array('data' => t('Referrer nid'), 'class' => 'cck-referential-integrity-cell-numeric'),
  );
  if ($field_info['has_delta']) {
    $headers[] = array('data' => t('delta'), 'class' => 'cck-referential-integrity-cell-numeric');
  }
  $headers[] = array('data' => $field_info['column'], 'class' => 'cck-referential-integrity-cell-numeric');
  $headers[] = array('data' => '', 'class' => 'select-all cck-referential-integrity-cell-action');

  $rows = array();
  foreach ($form['keys']['#records'] as $record_key => $record) {
    $referrer_node = l($record->title, 'node/'. $record->nid, array('attributes' => array('target' => '_blank', 'title' => ('View node on new window...'))));
    $referenced_value = (is_null($record->{$field_info['column']}) ? 'NULL' : $record->{$field_info['column']});
    $row = array(
      array('data' => $referrer_node),
      array('data' => $record->vid, 'class' => 'cck-referential-integrity-cell-numeric'),
      array('data' => $record->nid, 'class' => 'cck-referential-integrity-cell-numeric'),
    );
    if ($field_info['has_delta']) {
      $row[] = array('data' => $record->delta, 'class' => 'cck-referential-integrity-cell-numeric');
    }
    $row[] = array('data' => $referenced_value, 'class' => 'cck-referential-integrity-cell-numeric');
    $row[] = array('data' => drupal_render($form['keys'][$record_key]), 'class' => 'select-all cck-referential-integrity-cell-action');
    $rows[] = $row;
  }

  $rows_count = count($rows);
  if ($rows_count <= 0) {
    $rows[] = array(array('colspan' => count($headers), 'align' => 'center', 'data' => t('Could not find orphans for this field.')));
  }

  $help = t('This report displays a list of orphan records found for the table %table, column %column.
You may click on the node titles to review and/or edit each referrer node, or select the records that you want to process to resolve these orphans.
Please, make backups of your database before processing orphans should you need to restore any changes made from this panel.', array(
    '%table' => $field_info['table'],
    '%column' => $field_info['column'],
  ));
  $output = '<div class="help">'. $help .'</div>'."\n";
  $output .= theme('table', $headers, $rows);
  $output .= theme('pager', NULL, CCK_REFERENTIAL_INTEGRITY_ITEMS_PER_PAGE);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme the report status icons.
 *
 * @ingroup themeable
 */
function theme_cck_referential_integrity_status_icon($status, $title = NULL) {
  return '<div class="cck-referential-integrity-status-'. $status .'"'. (!empty($title) ? ' title="'. check_plain($title) .'"' : '') .'></div>';
}
