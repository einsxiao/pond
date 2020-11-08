The network scripts for POND project.

## pond Usage for pond
Usage: pond [operation] [operants]
  Valid operations: 
    edit|-e [file_name]
          start emacs to edit file with name file_name

    run|-r [script]
  	  run script in pond kernel

    new|create [options] [module_name] 
          create module with a unique name and register into Evawiz system. 
         options:
             -t|type [private|public|secret]
                  default value: private
             -l|lan [c++|cuda|fortran|matlab|python|pond]
                  default value: c++
             -f|from [module_name]
                  created new module based on existed module [module_name]

    delete [module_name] 
          try to delete a module 
         options:
             -local
                  only remove localfiles 

    push|upload [module_name]
          upload module, push new code to server side

    get|pull|download [module_name] 
          try to download a module, pull code from server side to local 

    load [program_name]
          try to request local host to load a program pre-compiled by POND
            the program will be located in $POND_ROOT/programs/

    perm [options] [module_name]
          try to give permision to otherrs(user,group)
          options:
             -u [user]
                   give permission to a user
                   if user takes the value of 'All' or your own pond account name,
                        the permission will valid for all users.
             -g [group_name]
                   give permission to a group
             -p [perm_string]
                   specify the permission content by a string
                   default value: r
          perm_string is consisted of 'r', 's', 'd', in which
               r: permission to run the module
               s: permission to view and use source code, plus r
               d: permission to push self-modified code to the author, plus r and s

    check |[options1] |[module_name] |[options2]
          check the module status, or user status, or local account status
          options1:
             -u [user]
                   check info. of user, ignoring following arguments.
             -g [group]
                   check info of group, ignoring following arguments.
          options2:
             -v
                   show hitory version list, using together with 'check module'

    register 
          register an account in Evawiz system. 

    connect 
          setup a trusted connection to an pond account. 

    findback|changepassword
          request to reset your account password via your registered email.

    clear
          clear connect pond information.

    -h|-?|--help|help
          show this help. 
