SUMMARY - YouTube Field
========================
The YouTube field module provides a simple field that allows you to add a
YouTube video to a content type, user, or any entity.

Display types include:

 * YouTube videos of various sizes.
 * YouTube thumbnails with image styles.


REQUIREMENTS
-------------
All dependencies of this module are enabled by default in Drupal 7.x.


INSTALLATION
-------------
Install this module as usual. Please see
http://drupal.org/documentation/install/modules-themes/modules-7


USEAGE
-------
To use this module create a new field of type 'YouTube video'. This field will
accept YouTube URLs of the following formats:

 * http://youtube.com/watch?v=[video_id]
 * http://youtu.be/[video_id]

It will not be a problem if users submit values with http:// or https:// and
additional parameters after the URL will be ignored.


CONFIGURATION
--------------
A YouTube field can be output as a video of at one of four standard sizes, or at
a custom size, with the ability to auto-play if necessary. The thumbnail of the
YouTube image can also be used and it can be linked to the full entity or the
video on YouTube.

To configure the field settings:

 1. click 'manage display' on the listing of entity types
 2. click the configuration gear to the right of the YouTube field


SUPPORT
--------
Please use the issue queue to report bugs or request support:
http://drupal.org/project/issues/youtube
