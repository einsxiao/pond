#!/usr/bin/env python3
from pond_basic import *
if ( len(sys.argv)<2 ):
    exit(0);

pre = " -Wl,"
if sys.argv[1] == 'pcc' or sys.argv[1] == 'nvcc':
    pre = " -Xlinker "
    pass
inc= "";

def add_include(path,absolute=False):
    if ( not os.path.exists( path ) ): return
    global inc
    inc += " -I%s "%(path,)
    pass

def add_lib(path,absolute=False):
    if ( not os.path.exists( path ) ): return
    global inc
    inc += " -L%s "%(path,);
    if absolute:
        inc += " %s-rpath=%s "%(pre,path)
    else:
        inc += " %s-rpath=$ORIGIN/%s "%(pre,path)
        pass
    pass

def add_path(path,absolute=False):
    if ( not os.path.exists( path ) ): return
    global inc
    inc += " -I%s "%(path,)
    inc += " -L%s "%(path,);
    if absolute:
        inc += " %s-rpath=%s "%(pre,path)
    else:
        inc += " %s-rpath=$ORIGIN/%s "%(pre,path)
        pass
    pass
        
pond_home = os.getenv("POND_HOME") or ''
if not pond_home: pond_home = os.getenv("HOME")+"/pond"
pond_root = os.getenv("POND_ROOT") or ''
if not pond_root: pond_root = os.getenv("HOME")+"/.pond"
#current directory
add_lib('./lib');
add_include('./include');
add_lib('./');
add_include('./');
add_lib( os.path.join(pond_root,'lib') )
add_include( os.path.join(pond_root,'include') )
#add_lib( os.path.join(pond_root,'packages','lib') )
#add_include( os.path.join(pond_root,'packages','include') )

#POND_INCLUDE_PATH
pond_include_path = os.getenv("POND_INCLUDE") or ''
for item in pond_include_path.split(':'):
    add_include(item, True);
    pass

#POND_LIBRARY_PATH
pond_library_path = os.getenv("POND_LIB") or ''
for item in pond_library_path.split(':'):
    add_lib(item,True);
    #print item
    pass

current_pulled = False
N = len(sys.argv)
i = 2
while i < N:
    is_find = False;
    mod = sys.argv[i]
    path = os.path.join( pond_home,mod);
    if os.path.exists(path):
        is_find = True
        add_lib('../%s/lib'%mod);
        add_include('../%s/include'%mod);
        add_path('../'+mod);
        pass
    path = os.path.join(pond_root,"modules/"+mod);
    if os.path.exists(path):
        is_find = True
        add_lib('../%s/lib'%mod);
        add_lib(path+'/lib',True);
        add_lib(path+'/lib64',True);
        add_include(path+'/include',True);
        add_path(path,True);
        pass
    path = os.path.join(pond_root,"base/"+mod);
    if os.path.exists(path):
        is_find = True
        add_lib('../%s/lib'%mod);
        add_lib(path+'/lib',True);
        add_lib(path+'/lib64',True);
        add_include(path+'/include',True);
        add_path(path,True);
        pass
    path = os.path.join(pond_root,"programs/"+mod);
    if os.path.exists(path):
        is_find = True
        add_lib('../%s/lib'%mod);
        add_lib(path+'/lib',True);
        add_lib(path+'/lib64',True);
        add_include(path+'/include',True);
        add_path(path,True);
        pass

    if not is_find:
        if not current_pulled:
            os.system('pd pull '+mod)
            current_pulled = True
            continue
        else:
            sys.stderr.write(""" Error: Dependency failed.
            Cannot find Depending Module '%s'.
            Please execute 'pond get %s' ahead of current operation.
            """%(mod,mod) );
            exit(1);
        pass
    i += 1
    current_pulled = False
    pass

sys.stdout.write(inc)
exit(0);

