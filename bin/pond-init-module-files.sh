#!/bin/bash
if ! [ $# == 1 ]; then
  echo "Usage:
	./pond-init-module-files.sh [module_name] 
"
  exit 0;
fi
module_name=$1
echo "Initialize files for module $module_name"
if [ "x$POND_HOME" == "x" ]; then
    echo "env variable POND_HOME is not ready"
    exit 1
fi
if ! [ -d $POND_HOME/$module_name ]; then
    echo "POND env is not ready!"
    exit 1
fi

cd $POND_HOME/$module_name
cp -rf $POND_ROOT/Template/Makefile            ./
cp -rf $POND_ROOT/Template/TemplateModule.h    ./${module_name}Module.h
cp -rf $POND_ROOT/Template/TemplateModule.cpp  ./${module_name}Module.cpp
cp -rf $POND_ROOT/Template/TemplateModule.pd   ./${module_name}Module.pd
cp -rf $POND_ROOT/Template/input.pd            ./
cp -rf $POND_ROOT/Template/.gitignore          ./

change_from_to ./ Template ${module_name} 2>&1 >/dev/null
change_from_to ./ DATE "`date`" 2>&1>/dev/null
chmod +r ~/.pond.user.auth
change_from_to ./ AUTHOR "`cat ~/.pond.user.auth |json nickname`" 2>&1>/dev/null
chmod -r ~/.pond.user.auth
change_from_to ./ VERSION "0.1.0" 2>&1>/dev/null

exit 0

