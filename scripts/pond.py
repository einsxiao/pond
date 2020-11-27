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

    push [module_name]
          upload module, push new code to server side

    pull [module_name] [options]
          try to download a module, pull code from server side to local 
          options:
              -v v1.v2.v3      pull given version

    login
          setup account bonded with https://doc.run 

    logout
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
  sysargv,options=argv2argsoptions(sys.argv,['version','help','run',],{'?':'help','h':'help','v':'version','r':'run',})
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

  if operation in ['say_hello','login','check','logout','pull', 'push']:
    # print("processing ",operation )
    mod = importlib.import_module("operations."+operation)
    mod.request(sysargv[1:], options )
    exit(0)
    pass

  cmd = pond_git_cmd 
  for item in sys.argv[1:]:
    cmd += " "+item
    pass
  # print("run as system cmd>",cmd)
  os.system(cmd)

except KeyboardInterrupt as e:
  print("\nprogram killed by keyboard interrupt")
  rtraceback()
except EOFError as e:
  print("\nprogram killed for EOF Error")
  rtraceback()
except Exception as e:
  print("program exist with error:",e)
  rtraceback()

