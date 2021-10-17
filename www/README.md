# GP2040 Web Configurator

Simple web application for gamepad configuration.

## Dev Notes

* Use JPG and PNG files for images, SVG file support requires modification that are unnecessary for this tool.
* The lwIP lib in Wizio Pico PlatformIO platform doesn't include the `makefsdata` source. A precompiled version for Windows and Unix are included in the `tools` folder.
* Use the `regen-fsdata.sh` shell script to update `lib/httpd/fsdata.c` file with the latest web app build output.
  * `makefsdata` doesn't set the correct `#include` lines for our use. This script will fix this issue.
