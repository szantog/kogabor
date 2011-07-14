
Drupal.viewsFiltergroup = function(e) {
  var that = this;

  $.each(this.options, function() {
    var element = $(document.getElementsByName(this.value));
    var container = $(element).parents('.views-exposed-widget:first');

    if (that.value == this.value) {
      container.show();
      if (e && element[0]) {
        // Only focus when this is from a change event.
        element[0].focus();
        element[0].select();
      }
    }
    else {
      container.hide();
    }
  });
};

Drupal.behaviors.viewsFiltergroup = function(context) {
  $('select.views-filtergroup-selector:not(.views-filtergroup-processed)', context)
    .change(Drupal.viewsFiltergroup)
    .each(function() { Drupal.viewsFiltergroup.call(this); })
    .addClass('views-filtergroup-processed');
};
