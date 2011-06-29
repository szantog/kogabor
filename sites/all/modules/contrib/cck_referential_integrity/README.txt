;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Referential Integrity for CCK
;; $Id: README.txt,v 1.1.2.4 2009/07/18 09:55:30 markuspetrux Exp $
;;
;; Original author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OVERVIEW
========

This module provides a method to enforce referential integrity rules for CCK
user and node reference fields. In other words, it allows you to decide what
to do with reference values when a referenced entity (node or user) is deleted.

The problem:

CCK node and user reference fields may contain values that point to nodes or
users that have been removed. CCK does nothing when a node or user is removed
with the values of fields that reference them. So after some time, your node
and user reference fields may contain records that point to nowhere. Orphans.


- Extension for reference fields:

  Once the module has been installed, the user and node reference field
  settings form will provide a new option "Referential integrity behavior".

  Available options are:

  - None (default).
  - Set NULL (set the value of this field to NULL when referenced users/nodes
    are deleted).

- Monitoring orphan records:

  You can also review all your reference fields to monitor if they have orphan
  records from Administer -> Content management -> Content -> Orphan references.

  This report displays the total number of records in the tables used for each
  reference field, and also the number of orphan records found, if any.

  For the tables where orphan records have been found, you can then list these
  records to review them manually, or to set their values to null. You should
  make backups of your database before processing orphans should you need to
  revert any changes made from this panel.


REQUIREMENTS
============

- CCK (node and user reference modules).
  http://drupal.org/project/cck

- Format Number API (used to format numbers in the administration reports).
  http://drupal.org/project/format_number


INSTALLATION
============

- Be sure to install all dependent modules.

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Go to Administer -> Site building -> Modules to install module.

- Review the settings of your user and node reference fields. You will
  find a new option labelled "Referential integrity behavior". It is
  disabled by default.


CREDITS
=======

- Original versions of the icons can be found free from here:

  http://www.famfamfam.com/
