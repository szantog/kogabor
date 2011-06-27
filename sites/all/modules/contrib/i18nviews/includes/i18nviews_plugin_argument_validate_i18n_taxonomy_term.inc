<?php
/**
 * @file
 * Contains the 'Taxonomy term (i18n)' argument validator plugin.
 */

/**
 * Validate whether an argument is a localized term.
 */
class i18nviews_plugin_argument_validate_i18n_taxonomy_term extends views_plugin_argument_validate_taxonomy_term {

  function option_definition() {
    $options = parent::option_definition();
    $options['type']['default'] = 'i18n_name';

    return $options;
  }
  function convert_options(&$options) {
    if (!isset($options['vids']) && !empty($this->argument->options['validate_argument_vocabulary_i18n'])) {
      $options['vids'] = $this->argument->options['validate_argument_vocabulary_i18n'];
      $options['type'] = $this->argument->options['validate_argument_type_i18n'];
      $options['transform'] = $this->argument->options['validate_argument_transform_i18n'];
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $vocabularies = taxonomy_get_vocabularies();
    $options = array();
    // Get the localized vocabularies.
    $localisedvocabs = variable_get('i18ntaxonomy_vocabulary', array());
    // Only add the localized vocabularies as options.
    foreach ($localisedvocabs as $localisedvocab => $localisedtype) {
      if ($localisedtype =='1') {
        $options[$localisedvocab] = check_plain($vocabularies[$localisedvocab]->name);
      }
    }
    $form['vids']['#description'] = t('Limit this validator to vocabularies that have been localized');
    $form['vids']['#options'] = $options;
    
    $form['type']['#options'] = array(
      'i18n_name' => t('Localised Term name or synonym'),
      'i18n_convert' => t('Localised Term name/synonym converted to Term ID'),
    );
    $form['type']['#description'] = t('Select the form of this argument; if using term name, it is generally more efficient to convert it to a term ID and use Taxonomy: Term ID rather than Taxonomy: Term Name" as an argument.');
  }

  function validate_argument($argument) {
    global $language;
    $langcode =$language->language;
    $vids = $this->options['vids'];
    $type = $this->options['type'];
    $transform = $this->options['transform'];
    switch ($type) {
      case 'i18n_name':
      case 'i18n_convert':
        // Check to see if the term is in fact localised
        $localised =db_fetch_object(db_query("SELECT source FROM {locales_source} ls INNER JOIN {locales_target} lt ON ( ls.lid = lt.lid AND  lt.translation = '%s' AND lt.language= '%s' AND ls.textgroup='taxonomy' )",  $argument, $langcode));
        if (!empty($localised)) {
          // If it is localised I set the $argument to the orginal and tell the view that the argument has been localized and to use the source
          $argument = $localised ->source;
          $this->argument->argument = $localised ->source;
        }
        $and = '';
        if (!empty($vids)) {
          $and = " AND td.vid IN(" . implode(', ', $vids) . ')';
        }
        if ($transform) {
          $result = db_fetch_object(db_query("SELECT td.* FROM {term_data} td LEFT JOIN {term_synonym} ts ON ts.tid = td.tid WHERE (replace(td.name, ' ', '-') = '%s' OR replace(ts.name, ' ', '-') = '%s')$and", $argument, $argument));
        }
        else {
          $result = db_fetch_object(db_query("SELECT td.* FROM {term_data} td LEFT JOIN {term_synonym} ts ON ts.tid = td.tid WHERE (td.name = '%s' OR ts.name = '%s')$and", $argument, $argument));
        }
        if (!$result) {
          return FALSE;
        }
        if ($type == 'i18n_convert') {
          $this->argument->argument = $result->tid;
        }
        // ToDo: check vocabulary translation mode (if localization needed)
        $this->argument->validated_title = check_plain(tt('taxonomy:term:'. $result->tid .':name', $result->name));
        return TRUE;
    }
  }
}
