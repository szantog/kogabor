<?php
// $Id: cck_referential_integrity.inc,v 1.1.2.5 2009/09/12 05:15:47 markuspetrux Exp $

/**
 * @file
 * Common functions for the Referential Integrity for CCK module.
 */

/**
 * Get the list of referential integrity behaviors.
 *
 * @param $field_entities
 *   You can currently use either t('nodes') or t('users').
 */
function cck_referential_integrity_get_behavior_options($field_entities) {
  return array(
    CCK_REFERENTIAL_INTEGRITY_BEHAVIOR_NONE => t('None.'),
    CCK_REFERENTIAL_INTEGRITY_BEHAVIOR_SET_NULL => t('Set the value of this field to NULL when referenced @entities are deleted.', array('@entities' => $field_entities)),
    // @todo: not really implemented, yet.
    // CCK_REFERENTIAL_INTEGRITY_BEHAVIOR_RESTRICT => t('Restrict: An error will be displayed when an attempt is made to delete @entities referenced from this field.', array('@entities' => $field_entities)),
    // CCK_REFERENTIAL_INTEGRITY_BEHAVIOR_CASCADE => t('Cascade: Delete referenced nodes when the @entities referenced from this field are deleted.', array('@entities' => $field_entities)),
  );
}

/**
 * Get the RI behavior defined for the given field name.
 */
function cck_referential_integrity_get_field_behavior($field_name) {
  return variable_get('cck_referential_integrity_behavior_'. $field_name, '');
}

/**
 * Get information about reference fields of the given type.
 *
 * @param $field_type
 *   Field types supported are 'nodereference' and 'userreference'.
 */
function cck_referential_integrity_get_reference_fields($field_type) {
  $fields = array();
  foreach (content_fields() as $field_name => $field) {
    if ($field['type'] == $field_type) {
      $fields[$field_name] = cck_referential_integrity_get_field_info($field);
    }
  }
  return $fields;
}

/**
 * Get orphans related information about a single field.
 *
 * @param $field
 *   The field to retrieve information from.
 */
function cck_referential_integrity_get_field_info($field) {
  if (empty($field) || empty($field['type']) || !in_array($field['type'], array('nodereference', 'userreference'))) {
    return FALSE;
  }
  $db_info = content_database_info($field);
  if (empty($db_info)) {
    return FALSE;
  }
  return array(
    'field_name' => $field['field_name'],
    'field_type' => $field['type'],
    'has_delta' => (!empty($field['multiple']) ? TRUE : FALSE),
    'label' => $field['widget']['label'],
    'table' => $db_info['table'],
    'column' => $field['field_name'] .'_'. current(array_keys($db_info['columns'])),
    'ri_behavior' => cck_referential_integrity_get_field_behavior($field['field_name']),
  );
}

/**
 * Get the number of records for the given field.
 *
 * @param $field_info
 *   Field data structure.
 */
function cck_referential_integrity_get_records_count($field_info) {
  return (int)db_result(db_query(cck_referential_integrity_build_records_query($field_info, TRUE)));
}

/**
 * Get the number of orphans for the given field.
 *
 * @param $field_info
 *   Field data structure.
 */
function cck_referential_integrity_get_orphans_count($field_info) {
  return (int)db_result(db_query(cck_referential_integrity_build_orphans_query($field_info, TRUE)));
}

/**
 * Build the query to retrieve records for the given field.
 *
 * @param $field_info
 *   Field data structure.
 * @param $return_count
 *   TRUE to build a query to return the rows count.
 *   FALSE to build a query to return all columns.
 * @param $include_node_title
 *   This argument is optional, FALSE by default.
 *   TRUE to include the node title (requires $return_count set to FALSE).
 */
function cck_referential_integrity_build_records_query($field_info, $return_count, $include_node_title = FALSE) {
  if ($return_count) {
    $columns = 'COUNT(*)';
    $orderby = '';
  }
  else {
    $columns = 'f.vid, f.nid, f.'. $field_info['column'];
    $orderby = ' ORDER BY f.vid, f.nid';
    if ($field_info['has_delta']) {
      $columns .= ', f.delta';
      $orderby .= ', f.delta';
    }
    if ($include_node_title) {
      $columns .= ', n.title';
    }
  }
  return 'SELECT '. $columns .' FROM {'. $field_info['table'] .'} f INNER JOIN {node} n ON n.vid = f.vid'. $orderby;
}

/**
 * Build the query to retrieve orphans for the given field.
 *
 * @param $field_info
 *   Field data structure.
 * @param $return_count
 *   TRUE to build a query to return the rows count.
 *   FALSE to build a query to return all columns.
 * @param $include_node_title
 *   This argument is optional, FALSE by default.
 *   TRUE to include the node title (requires $return_count set to FALSE).
 */
function cck_referential_integrity_build_orphans_query($field_info, $return_count, $include_node_title = FALSE) {
  if ($field_info['field_type'] == 'nodereference') {
    $related_table = 'node';
    $related_key = 'nid';
  }
  else {
    $related_table = 'users';
    $related_key = 'uid';
  }
  if ($return_count) {
    $columns = 'COUNT(*)';
    $orderby = '';
  }
  else {
    $columns = 'f.vid, f.nid, f.'. $field_info['column'];
    $orderby = ' ORDER BY f.vid, f.nid';
    if ($field_info['has_delta']) {
      $columns .= ', f.delta';
      $orderby .= ', f.delta';
    }
    if ($include_node_title) {
      $columns .= ', n.title';
    }
  }
  return 'SELECT '. $columns .' FROM {'. $field_info['table'] .'} f INNER JOIN {node} n ON n.vid = f.vid LEFT JOIN {'. $related_table .'} r ON r.'. $related_key .' = f.'. $field_info['column'] .' WHERE f.'. $field_info['column'] .' IS NOT NULL AND r.'. $related_key .' IS NULL'. $orderby;
}

/**
 * Apply referential integrity rule to the given object id.
 *
 * @param $field_type
 *   Field types supported are 'nodereference' and 'userreference'.
 * @param $field_value
 *   A node id (nid) or user id (uid).
 */
function cck_referential_integrity_apply($field_type, $field_value) {
  $cached_nids = array();
  $updated_fields = array();
  // Scan all fields of the specified type.
  foreach (cck_referential_integrity_get_reference_fields($field_type) as $field_name => $field_info) {
    if ($field_info['ri_behavior'] == CCK_REFERENTIAL_INTEGRITY_BEHAVIOR_SET_NULL) {
      $sql_conditions = array();
      $sql_args = array();
      $columns = 'vid, nid'. ($field_info['has_delta'] ? ', delta' : '');
      $result = db_query('SELECT '. $columns .' FROM {'. $field_info['table'] .'} WHERE '. $field_info['column'] .' = %d', $field_value);
      while ($record = db_fetch_object($result)) {
        if ($field_info['has_delta']) {
          $sql_conditions[] = '(vid = %d AND delta = %d)';
          $sql_args[] = $record->vid;
          $sql_args[] = $record->delta;
        }
        else {
          $sql_conditions[] = '(vid = %d)';
          $sql_args[] = $record->vid;
        }
        if (!isset($cached_nids[$record->nid])) {
          $cached_nids[$record->nid] = $record->nid;
        }
      }
      if (!empty($sql_args)) {
        // Update all related field records at once.
        db_query('UPDATE {'. $field_info['table'] .'} SET '. $field_info['column'] .' = NULL WHERE ('. implode(' OR ', $sql_conditions) .')', $sql_args);

        // Keep track of which fields have been updated.
        if (!isset($updated_fields[$field_name])) {
          $updated_fields[$field_name] = $field_info;
        }
      }
    }
  }

  // Expire cached objects related to the nodes we have just updated.
  if (!empty($cached_nids)) {
    cck_referential_integrity_clear_content_cache($cached_nids, $updated_fields);
  }
}

/**
 * Expire cached objects related to the nodes we have just updated.
 *
 * @param $cached_nids
 *   An array of node IDs.
 * @param $updated_fields
 *   An array of information about fields that have been updated.
 *
 * @see cck_referential_integrity_get_field_info()
 * @see cck_referential_integrity_apply()
 * @see cck_referential_integrity_orphan_references_check_one_field_submit()
 */
function cck_referential_integrity_clear_content_cache($cached_nids, $updated_fields) {
  $cache_table = content_cache_tablename();
  foreach ($cached_nids as $nid) {
    cache_clear_all('content:'. $nid .':', $cache_table, TRUE);
  }

  // Let other modules know which nodes have been changed.
  module_invoke_all('cck_referential_integrity_update', $cached_nids, $updated_fields);
}
