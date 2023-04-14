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
cp -rf $POND_ROOT/Template/Makefile             ./
cp -rf $POND_ROOT/Template/TemplateModule.h     ./${module_name}Module.h
echo -n "Enable cuda in module? ([y]/n): "
read cuda_enable
if [[ $cuda_enable == 'y' ]] || [[ $cuda_enable == 'Y' ]] ; then
    if [ -f $POND_ROOT/Template/TemplateModule.cpp ]; then
        cp -rf $POND_ROOT/Template/TemplateModule.cpp   ./${module_name}Module.cu
    fi
    if [ -f $POND_ROOT/Template/TemplateModule.cu ]; then
        cp -rf $POND_ROOT/Template/TemplateModule.cu   ./${module_name}Module.cu
    fi
else
    if [ -f $POND_ROOT/Template/TemplateModule.cpp ]; then
        cp -rf $POND_ROOT/Template/TemplateModule.cpp   ./${module_name}Module.cpp
    fi
    if [ -f $POND_ROOT/Template/TemplateModule.cu ]; then
        cp -rf $POND_ROOT/Template/TemplateModule.cu   ./${module_name}Module.cpp
    fi
fi
cp -rf $POND_ROOT/Template/TemplateModule.pd    ./${module_name}Module.pd
cp -rf $POND_ROOT/Template/input.pd             ./
cp -rf $POND_ROOT/Template/.gitignore           ./
cp -rf $POND_ROOT/Template/.vscode              ./

change_from_to ./ Template ${module_name} 2>&1 >/dev/null
change_from_to ./ MOD_DATE "`date`" 2>&1>/dev/null
chmod +r ~/.pond.user.auth
change_from_to ./ MOD_AUTHOR "`json-item ~/.pond.user.auth nickname`" 2>&1>/dev/null
chmod -r ~/.pond.user.auth
change_from_to ./ MOD_VERSION "0.1.1" 2>&1>/dev/null

exit 0

