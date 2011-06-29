$(document).ready(function() {
  /**
   * Checks whether or not the necessary criteria is met, and shows/hides the element based on that.
   */
  function show_hide_disallowed_first_option(){
    // If the field is set to required and the Number of values is 0 (which means a single value)...
    if($("#edit-required").attr('checked')==true && $("#edit-multiple").val()==0){
      // Show the Disallowed First Option field.
      $("#edit-cck-required-single-select-disallowed-first-option-wrapper").show();
      // Otherwise, hide it.
    }else{
      $("#edit-cck-required-single-select-disallowed-first-option-wrapper").hide();
    }
  }
  // On initial page load, check the values and show or hide the element.
  show_hide_disallowed_first_option();
  
  // Any time one of the options is changed, run the check again.
  $("#edit-required").change(function(){
    show_hide_disallowed_first_option();
  });
  $("#edit-multiple").change(function(){
    show_hide_disallowed_first_option();
  });
});
