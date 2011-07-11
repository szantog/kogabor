/**
 * Wysiwyg plugin button implementation for Awesome plugin.
 */
Drupal.wysiwyg.plugins.smartinsert = {
  /**
   * Return whether the passed node belongs to this plugin.
   *
   * @param node
   *   The currently focused DOM element in the editor content.
   */
  isNode: function(node) {
    return ($(node).is('img.smartinsert'));
  },

  /**
   * Execute the button.
   *
   * @param data
   *   An object containing data about the current selection:
   *   - format: 'html' when the passed data is HTML content, 'text' when the
   *     passed data is plain-text content.
   *   - node: When 'format' is 'html', the focused DOM element in the editor.
   *   - content: The textual representation of the focused/selected editor
   *     content.
   * @param settings
   *   The plugin settings, as provided in the plugin's PHP include file.
   * @param instanceId
   *   The ID of the current editor instance.
   */
  invoke: function(data, settings, instanceId) {
    var self = this;

    // onSubmit callback for modal frame dialogs.
    function onSubmitCallback(args, statusMessages) {
       Drupal.wysiwyg.instances[instanceId].insert("[smartgallery:"+args.gid+" fid:"+args.fid+"]");
    }

    // Build modal frame options.
    var modalOptions = {
      url: '/smartimage/list',
      width: $(window).width() - 30,
      height: $(window).height() - 30,
      autoFit: true,
      onSubmit: onSubmitCallback
    };

    // Open the modal frame.
    Drupal.modalFrame.open(modalOptions);
  },

  /**
   * Prepare all plain-text contents of this plugin with HTML representations.
   *
   * Optional; only required for "inline macro tag-processing" plugins.
   *
   * @param content
   *   The plain-text contents of a textarea.
   * @param settings
   *   The plugin settings, as provided in the plugin's PHP include file.
   * @param instanceId
   *   The ID of the current editor instance.
   */
  attach: function(content, settings, instanceId) {
    content = content.replace(/<!--break-->/g, this._getPlaceholder(settings));
    return content;
  },

  /**
   * Process all HTML placeholders of this plugin with plain-text contents.
   *
   * Optional; only required for "inline macro tag-processing" plugins.
   *
   * @param content
   *   The HTML content string of the editor.
   * @param settings
   *   The plugin settings, as provided in the plugin's PHP include file.
   * @param instanceId
   *   The ID of the current editor instance.
   */
  detach: function(content, settings, instanceId) {
    var $content = $('<div>' + content + '</div>');
    $.each($('img.smartinsert', $content), function (i, elem) {
      //...
    });
    return $content.html();
  },

  /**
   * Helper function to return a HTML placeholder.
   *
   * The 'drupal-content' CSS class is required for HTML elements in the editor
   * content that shall not trigger any editor's native buttons (such as the
   * image button for this example placeholder markup).
   */
  _getPlaceholder: function (settings) {
    return '<img src="' + settings.path + '/images/spacer.gif" alt="&lt;--break-&gt;" title="&lt;--break--&gt;" class="wysiwyg-break drupal-content" />';
  }
};
