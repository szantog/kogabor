// $Id: README.txt,v 1.1 2009/10/02 13:59:40 kirsh Exp $

Description
-----------
This module provides an auto increment (serial) CCK field.

Unlike Drupal built in auto increment node id, which is global and shared by
nodes of all types - serial fields are managed per node type.
For example, a serial field of an Invoice node type will generate a unique
sequential number (starting at 1, then 2, etc.) dedicated for Invoice nodes.

Allocation of serial numbers by this module is atomic, which means that
serial values are unique even when multiple nodes are created simultaneously.

For a full description of the module, visit the project page:
  http://drupal.org/project/serial
To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/serial

Requirements
------------
The CCK module (http://drupal.org/project/content) has to be installed.

Optional modules:
* Token (http://drupal.org/project/token) - to access serial values as tokens.
* Token + Path Auto (http://drupal.org/project/pathauto) -
  to use serial numbers in urls, for example:
    http://www.example.com/issue/10 (where 10 is a type scope serial number)
  instead of:
    http://www.example.com/node/64564 (where 64564 is a node id)

Installation
------------
1. Copy the serial module directory to your Drupal sites/all/modules/ directory.
2. Enable the module in administer > modules (admin/build/modules)

Usage
-----
You can add a serial field to any type at admin/content/types -> manage fields.
In the New Field form select Serial as the type of data and label and field name
as your choise, and Save. No other settings.

Note: Existing nodes will also get serial values when a serial field is added.

The serial token [field_serial-id] can be used with Path Auto.

Author
------
Ilan Kirsh