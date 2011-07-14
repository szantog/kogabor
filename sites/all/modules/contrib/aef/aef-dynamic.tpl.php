<?php

/**
 * @file aef-dynamic.tpl.php
 * Default theme implementation to list filters applied in a view with Advanced Exposed Filters
 *
 * @see template_preprocess_aef_dynamic()
 */
?>

<?php
  drupal_add_css(drupal_get_path('module', 'aef') . '/aef.css');
  drupal_add_js(array('aef' => array('table_id' => array($aef_id))), 'setting');
  drupal_add_js(array('aef' => array('form_id' => array($form_id))), 'setting');
?>

<fieldset class = "collapsible aef_new_filter">
  <legend><a href ="#"><?php print t('Apply new filter'); ?></a></legend>

  <table class = "aef_exposed">
    <tbody>
    <?php foreach ($widgets as $id => $widget): ?>
    <tr>
      <td><?php print $select_filter[$id]; ?></td>
      <td><?php print $widget->operator; ?></td>
      <td><?php print $widget->widget; ?></td>
    </tr>
    </tbody>
    <?php endforeach; ?>
  </table>
  <div class = "buttons">
    <?php print $button ?>
  </div>
</fieldset>

<?php if (count ($where_groups) || count ($group_by_groups)): ?>
<?php
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'aef') .'/aef.js');
  foreach (array_keys($where_groups) as $id) {
    drupal_add_tabledrag($aef_id, 'match', 'sibling', 'aef-region-select', 'aef-region-'. $id, NULL, FALSE);
    drupal_add_tabledrag($aef_id, 'order', 'sibling', 'aef-weight', 'aef-weight-'. $id);
  }
?>

<fieldset class ="collapsible aef_applied_filters <?php if($ajax_on) print 'aef_ajax_on';?>">
  <legend><a href ="#"><?php print t('Applied filters'); ?></a></legend>
  <div class="description">
    <span>
      <?php print $advanced_help . t('Groups are grouping using <strong>@all_filter_group</strong> you can change this here: ', array('@all_filter_group' => $selected_all_group_op)) . '</span>' . $all_groups_operator; ?>
  </div>
  <?php if (count ($where_groups)): ?>
  <table id ="<?php print $aef_id;?>">
    <thead>
      <tr class = "aef">
        <th><?php print t('Fitler'); ?></th>
        <th><?php print t('Operator'); ?></th>
        <th><?php print t('Value'); ?></th>
        <th class = "aef_post_hide"><?php print t('In group'); ?></th>
        <th><?php print t('Position'); ?></th>
        <th class = "aef_right"><?php print t('Operations'); ?></th>
      </tr>
    </thead>

    <tbody>
      <?php foreach ($where_groups as $group_id => $group) : ?>

          <tr class = "aef_group group_<?php print $group_id; ?>">
            <td class = "region">
              <?php print t('Group: %group', array('%group' => $group_id)); ?>
            </td>
            <td class = "region" colspan = "2"><span>
              <?php print t('Group operator: ') . '</span>'. $group_operator[$group_id]; ?>
            </td>
            <td class = "region aef_right"  colspan = "3">
              <?php print $put_in_group[$group_id] . '<span>' . t('Put filters here: ');?> </span>
            </td>
          </tr>

          <tr class="region-message region-<?php print $group_id; ?>-message <?php print count($group) == 0 ? 'region-empty' : 'region-populated'; ?>">
            <td colspan="5"><em><?php print t('This group is empty'); ?></em></td>
          </tr>

          <?php if (count($group)): ?>
            <?php $row = 0; ?>
            <?php foreach ($group as $delta => $filter) : ?>
                <tr class = "draggable <?php print ($row++ % 2)?'odd':'even'; ?>">
                  <td><?php print $filter['filter']; ?></td>
                  <td><?php print $filter['operator']; ?></td>
                  <td><?php print $filter['value']; ?></td>
                  <td class = "aef_post_hide"><?php print $filter['group']; ?></td>
                  <td><?php print $filter['weight']; ?></td>
                  <td class = "aef_right"><?php print $filter['delete']; ?></td>
                </tr>
            <?php endforeach; ?>
          <?php endif; ?>
      <?php endforeach; ?>
    </tbody>
  </table>
  <?php endif; ?>
  <?php if (count($group_by_groups)) : ?>
    <table>
    <thead>
      <tr class = "aef">
        <th><?php print t('Fitler'); ?></th>
        <th><?php print t('Operator'); ?></th>
        <th><?php print t('Value'); ?></th>
        <th class = "aef_right"><?php print t('Operations'); ?></th>
      </tr>
    </thead>
    <tbody>
    <?php foreach ($group_by_groups as $group_id => $group) : ?>
      <tr class = "aef_group group_<?php print $group_id; ?>">
        <td class = "region">
          <?php print t('Group: Group BY'); ?>
        </td>
        <td class = "region" colspan = "5"><span>
          <?php print t('Group operator: ') . '</span>'. $group_operator[$group_id]; ?>
        </td>
      </tr>

      <tr class="region-message region-<?php print $group_id; ?>-message <?php print count($group) == 0 ? 'region-empty' : 'region-populated'; ?>">
        <td colspan="5"><em><?php print t('This group is empty'); ?></em></td>
      </tr>

      <?php if (count($group)): ?>
        <?php $row = 0; ?>
        <?php foreach ($group as $delta => $filter) : ?>
            <tr class = " <?php print ($row++ % 2)?'odd':'even'; ?>">
              <td><?php print $filter['filter']; ?></td>
              <td><?php print $filter['operator']; ?></td>
              <td><?php print $filter['value']; ?></td>
              <td class = "aef_right"><?php print $filter['delete']; ?></td>
            </tr>
        <?php endforeach; ?>
      <?php endif; ?>
    <?php endforeach; ?>
    </tbody>
    </table>
  <?php endif; ?>
  <div id = "aef_need_refresh" class="warning aef_hidden"><span class="warning tabledrag-changed">*</span><?php print t('Changes made in this table will not be saved until you click on refresh.') ?></div>
  <div class = "aef_buttons">
    <?php
      print $reset_button;
      print $refresh_button;
      print $new_group_button;
    ?>
  </div>
</fieldset>
<?php else: ?>
<?php
  // at least add the table drag for row 0
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'aef') .'/aef.js');
  drupal_add_tabledrag($aef_id, 'match', 'sibling', 'aef-region-select', 'aef-region-0', NULL, FALSE);
  drupal_add_tabledrag($aef_id, 'order', 'sibling', 'aef-weight', 'aef-weight-0');
?>
<table id ="<?php print $aef_id;?>"></table>
<?php endif; ?>
