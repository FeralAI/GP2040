#!/bin/sh

echo Regenerating lib/httpd/fsdata.c
./tools/makefsdata www/build/ -f:lib/httpd/fsdata.c
echo Done

echo Replace includes
perl -i -p0e 's/#include.*?"lwip\/def.h"/#include "fsdata.h"/s' lib/httpd/fsdata.c
echo Done
