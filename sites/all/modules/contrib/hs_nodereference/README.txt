$Id: README.txt,v 1.8 2010/08/08 03:50:07 jrao Exp $

Description
-----------
This module allows the use of Hierarchical Select form element as a widget 
for CCK node reference field, both in node add/edit form and in views filter.

A Hierarchical Select in widget setting allows you to define a path in the 
hierarchy of node reference fields, once defined, this path can be used to 
render Hierarchical Select form element in node add/edit form and views filter. 
Save lineage, dropbox and create new item/level features of the Hierarchical 
Select is supported in CCK field widget, as long as the field number of values 
and referenced content types setting can support them.

Installation
------------
1) Place this module directory in your "modules" folder (this will usually be
"sites/all/modules/"). Don't install your module in Drupal core's "modules"
folder, since that will cause problems and is bad practice in general. If
"sites/all/modules" doesn't exist yet, just create it.

2) Enable the module Hierarchical Select Node Reference and Hierarchical Select 
Node Reference Views.

CCK Node Reference Field Configuration
--------------------------------------
1) When adding new node reference field, select "Hierarchical Select" as form
element. Alternatively, click Configure on an existing node reference field, and 
change the widget type to "Hierarchical Select".
 
2) In field's Global settings, make sure "Content types that can be referenced"
is selected and click "Save field settings" to save the selection. Without this
the "Node reference path" setting will be empty.

3) Once "Content types that can be referenced" is saved, go back to field configuration,
use the dropdowns under "Node reference path" to define a path you would like to use
for Hierarchical Select widget, click "Save field settings" to save the selection.

Note: In order for "Note reference path" to have options there must exist a node reference
hierarchy, i.e. the content types selected in "Content types that can be referenced" 
must have their own node reference fields referencing some other content types. Otherwise
the "Note reference path" will always be <none> 

4) Optionally, click the link under "Node reference path" to configure Hierarchical Select
settings, the settings page is also listed under admin/settings/hierarchical_select/configs

Node Reference Path Example
----------------------------------
"Node Reference Path" is the most important setting in this module, here's an
example of how "Node Reference Path" works:
1) Setup the following content types:
-- Country
-- State: 
  -- Add a node reference field Country with:
    -- Widget type: Select list
    -- Content types = Country
    -- Required 
    -- Number of values = 1
-- City:
  -- Add a node reference field State with:
    -- Widget type: Select list
    -- Content types = State
    -- Required 
    -- Number of values = 1
-- Address:
  -- Add a node reference field City with:
    -- Widget type: Hierarchical Select
    -- Content types = City
    -- Required 
    -- Number of values = 1
2) Add some test data:
-- Country: United States, Canada
-- States under United States: California, Washington, Michigan 
-- States under Canada: British Columbia, Alberta, Manitoba  
-- Cities under California: Los Angeles, San Diego, San Jose, Sacramento
-- Cities under Washington: Olympia, Seattle
-- Cities under Michigan: Lansing, Detroit
-- Cities under Alberta: Airdrie, Grande Prairie, Red Deer
-- Cities under British Columbia: Burnaby, Lumby, City of Port Moody
-- Cities under Manitoba: Birtle
3) Play with "Node Reference Path" setting:
-- Change Address content type's City field's Node Reference Path to City.State, 
  save the setting. Then attempt to create a new Address node, in the form's City
  field you can see Hierarchical Select in action, first select a state, then
  select a city.
-- Change the City field's Node Reference Path to City.State->State.Country, try
  to create a new Address node, this time the first dropdown is a list of countries,
  select a country will bring you the states, and select a state will bring you
  to the cities.
-- Note the Node Reference Path is the reverse of what actually appears on form,
  this is by design, since the field where Node Reference Path is setup is always
  going to be the last to appear in the form's Hierarchical Select

Views Filter Configuration
--------------------------------------
1) When adding new node reference field filter, an extra configuration form is
displayed, use "Selection type" to choose which widget to use for this filter. 
Choose "Dropdown" to use the original filter came from CCK, choose "Hierarchical Select".
When using "Hierarchical Select" as selection type, pick a corresponding content type
in the left side.

2) In the filter configuration, click Expose to expose the filter, and use the
"Node reference path" to select the path to use for the Hierarchical Select.

3) Optionally, use the link in extra configuration form to configure Hierarchical Select
settings, the settings page is also listed under admin/settings/hierarchical_select/configs

Node Lineage: When and How to use
---------------------------------
1) If "Save node lineage" is selected in HS setting for node reference field, this means
all the nodes in the node reference path will be saved into this node reference field. For
example, if Address has node reference field City, and node reference path is City.State, 
and user selected California->Los Angeles in the HS dropdowns, then both the state node 
California and the city node Los Angeles will be saved to the node reference field. Obviously 
this requires this node reference field to accept at least 2 values and can accept both 
city and state content types (the HS setting will disable the "Save node lineage" radio 
button if these criteria are not satisfied).
2) The "Save node lineage" in node reference field is not necessary for the HS cascade dropdown
to work with node references. The main reason "Save node lineage" is selected is for partial
matches in node reference field filter in views.
3) The HS setting for node reference field filter also has a "Save node lineage" radio button,
selecting this means all the nodes in the node reference path will be used in views query 
(instead of just the deepest node). This setting is usually used with a node reference field
whose "Save node lineage" is also turned on, this allows partial matches to work in the filter.
To continue the example in 1), if "Save node lineage" is turned on for a node reference field
filter for field City, when only California is selected in the views filter (without
city selection), the views will return any address whose state is California, including 
California->Los Angeles; if Los Angeles is selected in city filter, then only California->
Los Angeles is returned.

Rendering node reference hierarchy lineages when viewing content
----------------------------------------------------------------
This module is mainly used for input, thus it is only used on the create/edit forms of content. 
However, there is a need for the module to help display the value(s) in the node reference 
field that is using this module for input, since only this module (and the HS module) knows
how to reconstruct the lineage based on field value(s).  

Thus the module now provides 3 theme functions, similar to those provided in HS module:
-- theme_hs_nodereference_selection_as_lineages: This function will take a node and a node 
  reference field name, and render the field value as lineages, based on the field 
  configuration.
-- theme_hs_nodereference_lineages: This function is used to render multiple lineages,
  in case the field has multiple node references in it (i.e. when dropbox is enabled on the 
  input side). Theme developer can override this function to customize the display of multiple
  lineages.  
-- theme_hs_nodereference_lineage: This function is used to render a particular lineage, 
  theme developer can override this function to customize the display of individual lineage.

Sample usage:
<?php print theme('hs_nodereference_selection_as_lineages', $node, 'field_addresses'); ?>

This will automatically render all lineages for the value(s) in $node's field_addresses field,
assuming this field is a node reference field using this module as widget.

CCK Formatter
-------------
This module now provides a CCK formatter called "Hierarchical Select lineages" which
can be used in CCK and Views, the formatter will use theme function 
theme_hs_nodereference_selection_as_lineages to render the node reference field in node
display or views.