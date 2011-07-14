
Drupal.behaviors.aefVisualEffects = function(context) {
  $(".aef_post_hide").hide();

  $(".icon-button").each(function() {
    $(this).parent().append("<img class = 'img-icon-button' src = '" + $(this).attr("src") + "'/>");
    $(this).hide();
  });

  $(".img-icon-button").click(function() {
    $(this).after('<span class="views-throbbing">&nbsp</span>');
    $("form#views-exposed-form-" + Drupal.settings.aef.form_id).get(0).clk = $(this).prev().get(0);
    $("form#views-exposed-form-" + Drupal.settings.aef.form_id).submit();
  });
}

Drupal.behaviors.aefFilterChange = function(context) {

  $(".warning-on-change").change(function () {
    $("#aef_need_refresh").hide().removeClass("aef_hidden").fadeIn("slow");
  });

  $("#edit-aef-filter-group").change(function () {
    if ($(this).val() != 'new') {
      $("#edit-aef-group-operator").parent().hide();
    }
    else {
      $("#edit-aef-group-operator").parent().fadeIn("slow");
    }
  });
}

Drupal.behaviors.aefFilterDrag = function(context) {
  var table_id = Drupal.settings.aef.table_id;
  var table = $('table#' + table_id);
  if (!table) {
    return;
  }
  var tableDrag = Drupal.tableDrag[table_id];

  // Add a handler for when a row is swapped, update empty regions.
  tableDrag.row.prototype.onSwap = function(swappedRow) {
    checkEmptyRegions(table, this);
  };

  Drupal.theme.tableDragChangedWarning = function () {
    $("#aef_need_refresh").hide().removeClass("hidden").fadeIn("slow");
    return '<div></div>';
  };

  // Add a handler so when a row is dropped, update fields dropped into new group.
  tableDrag.onDrop = function() {
    dragObject = this;
    // If this occurs row is in an empty group or its is the first of the group
    if ($(dragObject.rowObject.element).prev('tr').is('.region-message')) {
      // Get the previous group, this contains the group id
      var regionRow = $(dragObject.rowObject.element).prev('tr').get(0);
      var groupId = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');

      // Then, update the select group value
      var selectGroupField = $('select.aef-region-select', dragObject.rowObject.element);
      selectGroupField.val(groupId);
    }
  }

  var checkEmptyRegions = function(table, rowObject) {
    $('tr.region-message', table).each(function() {
      // If the dragged row is in this region, but above the message row, swap it down one space.
      if ($(this).prev('tr').get(0) == rowObject.element) {
        // Prevent a recursion problem when using the keyboard to move rows up.
        if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
          rowObject.swap('after', this);
        }
      }
      // This region has become empty
      if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').size() == 0) {
        $(this).removeClass('region-populated').addClass('region-empty');
      }
      // This region has become populated.
      else if ($(this).is('.region-empty')) {
        $(this).removeClass('region-empty').addClass('region-populated');
      }
    });
  };
}

