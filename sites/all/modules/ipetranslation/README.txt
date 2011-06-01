/* $Id: README.txt,v 1.1 2009/07/12 10:43:32 khorpyakov Exp $ */

Translate when create: module implements functionality needed for multilanguage sites 
with user-created content that has to be translated in every language when it's created.
Based on I18N and Translation modules it extends forms workflow settings in the way 
of i18nsync module with complement meaning: add fields for In-place translation and 
then split them into node translations.

Current release supports title and body in-place translation.

Additional bonus feature: batch translate any i18n string using Translation Framework 
and Google translation service. Enable Batch translator sub-module to use this feature.

INSTALLATION
------------
Install and enable the In-place translation module as you would any Drupal module.

USAGE
-----
When enabled, go to the content types administration page. On edit content type page
expand Workflow settings section then locate Expand translation section. Mark fields
which you want to be in-place translated.

Then, when creating or editing content there will be multiply fields (bodies, titles 
etc.) on the form, for each language enabled. Saving node triggers translations 
creation with use of appropriate fields from the originaly submitted form.

CONFIGURATION OPTIONS
---------------------
There is no configuration options yet.

CREDITS
-------
Developed and maintained by Mikhail Khorpyakov <khorpyakov@kitairu.net>
Sponsored by Kitairu.net <http://www.kitairu.net/>