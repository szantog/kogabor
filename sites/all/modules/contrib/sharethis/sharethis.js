
/**
 * The Drupal behaviors for the ShareThis web service.
 */
Drupal.behaviors.sharethis = function(context) {
  // Process each element.
  if (Drupal.settings.sharethis) {
    jQuery.each(Drupal.settings.sharethis, function(id, options) {
      var element = document.getElementById(id);
      if (!element) {
        return;
      }
      var shared_object = SHARETHIS.addEntry({
        title: options['title'],
        url: options['url']
      });
      if (options['chicklet'] || false) {
        shared_object.attachChicklet(options['chicklet'], element);
      }
      else {
        shared_object.attachButton(element, {button: false});
      }
      $('#' + id).click(function() { return false; });
      delete(Drupal.settings.sharethis[id]);
    });
  }
};
