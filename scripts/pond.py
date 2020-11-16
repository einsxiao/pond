#!/usr/bin/env python3
#coding:utf-8
################################################
## author: Eins Xiao
## version: 2020.10.27
################################################
import sys
import os
pond_root = os.getenv("POND_ROOT")
sys.path.append(pond_root+"/pymods")
from pond_basic import *
import user_request
#import request
import importlib

pond_server=os.getenv("POND_SERVER");
pond_server_port=os.getenv("POND_SERVER_PORT");
pond_proxy=os.getenv("POND_PROXY");
if not pond_server: pond_server = 'server.doc.run'
if not pond_server_port: pond_server_port = '5555'

#dprint( pond_server, pond_server_port)
user = user_request.UserRequest(pond_server,int(pond_server_port) ,pond_proxy)

def help_info():
  print("""Usage: pond [operation] [operants]
  operations allowed: 
    run|-r [script]
          run script in pond kernel

    push|upload [module_name]
          upload module, push new code to server side

    pull [module_name] [options]
          try to download a module, pull code from server side to local 
          options:
              -v v1.v2.v3      pull given version

    login
          setup account bonded with https://doc.run 

    logout|clear
          clear pond login info

    check 
          check current login information 
          if login, list modules created

    version|-v
          show current pond version.

    -h|-?|--help|help
          show this help.
  """)

try:
  #according content of sys.argv decide what to do

  #no argv run something
  script_path = os.path.split(os.path.realpath(__file__))[0]
  sysargv,options=argv2argsoptions(sys.argv,['version','help','run','nowindow','local'],{'?':'help','h':'help','v':'version','r':'run','u':'user','g':'group','p':'perm','t':'type','b':'branch','nw':'nowindow'})
  #print('sysargv =',sysargv)
  #print('options =',options)
  #no argument
  if (len(sys.argv) == 1):
    os.system(script_path+'/../bin/pond')
    exit(0)
    pass

  if len(sysargv)>1:
    operation = sysargv[1]
  else:
    operation = 'NOT_DEFINED'
    pass

  # no_window_opt_str = '  '
  # if options.get('nowindow'):
  #   no_window_opt_str = ' -nw ';

  #help
  if operation == 'help' or options.get('help'):
    help_info()
    exit(0)
    pass


  #checkmod check module state
  if operation == 'ping':
    # user.mod_request('ping')
    exit(0)
    pass

  # if operation == 'check':
  #   if len(sysargv)<3:
  #     if ( get_option(options,'user') ):
  #       #user.request_list_module(options);
  #       user.mod_request("list_module",options)
  #       exit(0)
  #       pass
  #     print("check pond settings")
  #     #if user.request_check_user_info():
  #     if user.mod_request("check_user_info"):
  #       user.mod_request("list_module",options)
  #       #user.request_list_module();
  #       exit(0)
  #       pass
  #     exit(1)
  #     pass
  #   module_name = sysargv[2]
  #   print("check module '%s'... "%module_name)
  #   if get_option(options,'version'):
  #     #user.request_check_module(module_name,True);
  #     user.mod_request('check_module',module_name,True)
  #   else:
  #     #user.request_check_module(module_name);
  #     user.mod_request('check_module',module_name )
  #     pass
  #   exit(0)

  #new module
  # if operation == 'new' or operation == 'create':
  #   if len(sysargv)<3:
  #     print('Arguement error: a module name is required.')
  #     exit(1)
  #     pass
  #   module_name = sysargv[2]
  #   options_transform(options,{'t':'type','n':'name','l':'lang','f':'from'})
  #   print('try to register module %s'%module_name)
  #   #user.request_new_module(module_name,options)
  #   user.mod_request('new_module',module_name,options)
  #   exit(0)
    
  #delete module
  # if operation == 'delete':
  #   if len(sysargv) < 3:
  #     print('Argument error. A module name is required.')
  #     exit(1)
  #     pass
  #   module_name = sysargv[2]
  #   #user.request_delete_module(module_name)
  #   user.mod_request('delete_module',module_name)
  #   exit(0)
    
  #upload module
  # if operation == 'upload' or operation == 'push':
  #   if len(sysargv) < 3:
  #     module_name = ''
  #   else:
  #     module_name = sysargv[2]
  #     pass
  #   ####
  #   options_transform(options,{'b':'branch','t':'type','v':'version'})
  #   #user.request_upload_module(module_name,options)
  #   user.mod_request("upload_module",module_name,options)
  #   exit(0)

  #download module
  # if operation == 'download' or operation == 'pull' or operation == 'get':
  #   sysargv,options=argv2argsoptions(sys.argv,[],{'v':'version','b':'branch'})
  #   if len(sysargv) < 3:
  #     module_name = get_modulename_from_path()
  #     if not module_name:
  #       print('Argument Error:A module name is required. Otherwize, operation need to be done at the root of the module.')
  #   else:
  #     module_name = sysargv[2]
  #     pass
  #   #########
  #   dprint('module_name = %s'%module_name)
  #   options_transform(options,{'b':'branch','v':'version'})
  #   #user.request_download_module_to_user(module_name,options)
  #   user.mod_request("download_module",module_name,options)
  #   exit(0)

  # #load program
  # if operation == 'load' :
  #   if len(sysargv) < 3:
  #     print('Argument Error:A program name is required.')
  #     pass
  #   program_name = sysargv[2]
  #   #user.request_load_program(program_name,'normal');
  #   user.mod_request("load_program",program_name,'normal');
  #   exit(0);

  # #perm_module
  # if operation == "perm":
  #   if len(sysargv) < 3:
  #     module_name = get_modulename_from_path()
  #     if not module_name:
  #       print('Argument Error:A module name is required. Otherwize, operation need to be done at the root of the module.')
  #   else:
  #     module_name = sysargv[2]
  #     ############
  #   dprint('module_name = %s'%module_name)
  #   options_transform(options,{'g':'group','u':'user','p':'perm'})
  #   #user.request_perm_module(module_name,options)
  #   user.mod_request("perm_module",module_name,options)
  #   exit(0)

  # #say_hello test
  # if operation == 'say_hello':
  #   print('say hello test')
  #   user.contact_host('say_hello','noncrypt')
  #   user.request_say_hello()
  #   exit(0)

  # if operation == 'register':
  #   print('checking your account infomation')
  #   #local connection info
  #   #check_result = user.request_check_user_info_only()
  #   check_result = user.mod_request("check_user_info_only")
  #   if check_result == 'connected to server':
  #     print('You are already connected to server. No need to register a new pond account.')
  #     exit(0)
  #     pass
  #   print("checking finished. Permitted to register.")
  #   if check_result == "connection info outdated": # delete connection info
  #     pond_dir= os.getenv('POND_HOME')
  #     auth_file = pond_dir + '/.user.auth'
  #     os.system('rm -f '+auth_file)
  #     ###
  #     #user.request_register_user()
  #   user.mod_request('register_user')
  #   exit(0)

  # if operation == 'connect':
  #   #user.request_connect_user()
  #   user.mod_request("connect_user")
  #   exit(0)

  # if operation == 'disconnect':
  #   #user.request_connect_user()
  #   user.mod_request("disconnect_user")
  #   exit(0)

  if operation == 'run': 
    if len(sysargv)<3:
      print('Error: a script is required.')
      exit(1)
      pass
    os.system(script_path+'/../bin/pond <'+sysargv[2])
    exit(0)
    pass

  if options.get('run'):
    if len(sysargv)<2:
      print('Error: a script is required.')
      exit(1)
      pass
    os.system(script_path+'/../bin/pond <'+sysargv[1])
    exit(0)
    pass

  # if operation == 'clear':
  #   pond_dir = os.getenv('POND_HOME')
  #   if not os.path.exists(pond_dir): os.system("mkdir "+pond_dir)
  #   auth_file = pond_dir+'/.user.auth'
  #   dprint("auth_file = ",auth_file);
  #   if os.path.exists(auth_file):
  #     os.system('chmod 600 '+auth_file)
  #     os.system('rm '+auth_file)
  #     print("clear local auth information.")
  #     pass
  #   exit(0);
  #   pass

  #show local version
  if operation == 'version' or get_option(options,'version'):
    print("""
    Pond High Performance Platform.
    Version: 11.05.2020.
    All rights reserved by OVO.LTD.
    """)
    exit(0)

  #unkown option or operator
  if operation[0] == '-':
    print("Error: unknown option \'"+operation+"\'")
    exit(1)
    pass

  if operation == 'clear': operation = 'logout'

  if operation in ['say_hello','login','check','logout','pull']:
    # print("processing ",operation )
    mod = importlib.import_module("operations."+operation)
    mod.request(sysargv[1:], options )
    exit(0)
    pass

  cmd = "GIT_SSH_COMMAND='ssh -i {0}/.git_private_key' git".format(pond_home)
  for item in sys.argv[1:]:
    cmd += " "+item
    pass
  #print("run as system cmd>",cmd)
  os.system(cmd)
  #treated as edit, operation as script file
  #os.system(script_path+'/../bin/pond <'+sysargv[1])
  # files = ''
  # for i in range(1,len(sysargv)):
  #   files+=""" \""""+sysargv[i]+"""\""""
  #   pass
  # os.system(pond_root+"/bin/pond "+files)
except KeyboardInterrupt as e:
  print("\nprogram killed by keyboard interrupt")
  dtraceback()
except EOFError as e:
  print("\nprogram killed for EOF Error")
  dtraceback()
except Exception as e:
  print("program exist with error:",e)
  dtraceback()

