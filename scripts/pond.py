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
#import user_request
#import request
import importlib

pond_version='0.2.3'
pond_server=os.getenv("POND_SERVER",'server.doc.run')
pond_server_port=os.getenv("POND_SERVER_PORT",'5555')
pond_git_port=os.getenv("POND_GIT_PORT",'22')
pond_proxy=os.getenv("POND_PROXY")

#print( pond_server, pond_server_port)
#user = user_request.UserRequest(pond_server,int(pond_server_port) ,pond_proxy)
lang = os.getenv("POND_LANG")

def help_info():
  if lang == 'zh':
    print("""用法: pd [操作] [操作参数]
  可用操作: 
    run|-r [脚本]
          在pond核中解释执行[脚本]

    save [模块名]
          上传模块内容, 将更改推送到服务端的代码仓库中

    pull [模块名] [选项]
          下载模块内容, 从服务端的代码仓库中拉取更改 

    remake [模块名] 
          重新编译模块

    remake --all
          重新编译所有模块

    login
          使用 https://doc.run 的帐号密码登录系统

    logout
          清除登录信息

    check|list
          检验当前登录信息
          如果已经登录, 列举已经创建的模块

    -----------------------------------------------
    list_packages 
          列举可安装软件包名(python, mpi 等等)
    install [软件包名]
          安装软件包
    -----------------------------------------------

    version|-v
          显示当前的pond版本

    -h|-?|--help|help
          显示当前帮助信息
  """)
    pass
  else:
    print("""Usage: pd [operation] [operants]
  operations allowed: 
    run|-r [script]
          run script in pond kernel

    save [module_name]
          upload module, push new code to server side

    pull [module_name] [options]
          try to download a module, pull code from server side to local 

    remake [module_name] 
          recompile module

    remake --all
          recompile all modules

    login
          setup account bonded with https://doc.run 

    logout
          clear pond login info

    check|list
          check current login information 
          if login, list modules created

    -----------------------------------------------
    list_packages
          list packages can be installed(python, mpi 等等)
    install [软件包名]
          install package
    -----------------------------------------------

    version|-v
          show current pond version.

    -h|-?|--help|help
          show this help.
  """)
    pass

try:
  #according content of sys.argv decide what to do

  #no argv run something
  script_path = os.path.split(os.path.realpath(__file__))[0]
  sysargv,options=argv2argsoptions(sys.argv,['version','help','run','all'],{'?':'help','h':'help','v':'version','r':'run',})
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
    print(pond_version)
    exit(0)

  #unkown option or operator
  if operation[0] == '-':
    print("Error: unknown option \'"+operation+"\'")
    exit(1)
    pass

  if operation == 'clear': operation = 'logout'
  if operation == 'list': operation = 'check'

  if operation in [
      'say_hello','login','check','logout','pull', 'push',
      'save', 'list_packages', 'install', 'remake',
  ]:
    #print("processing ",operation )
    mod = importlib.import_module("operations."+operation)
    mod.request(sysargv[1:], options )
    exit(0)
    pass

  cmd = pond_git_cmd 
  for item in sys.argv[1:]:
    cmd += " "+item
    pass
  #print("run as system cmd>",cmd)
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

