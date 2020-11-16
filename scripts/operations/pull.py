from pond_basic import *
import requests
import datetime 
import dateutil
import json

# login
def request(argv,options):

    options_transform(options,{'b':'branch','v':'version'})
    branck   = options.get('branch')
    version  = options.get('version')

    # print( 'argv = ',argv)
    # print( 'options = ',options)
    if len(argv) >= 2:
        module_name = argv[1]
    else:
        cwd = os.getcwd()
        print( pond_home)
        print( cwd )
        if not cwd.startswith( pond_home ):
            print("  Name of a module should be specified if outside a module working directory.")
            return
        module_name = os.path.basename( cwd[ len(pond_home): ] )
        pass
    print( '\n  Try pull module ',module_name )

    if not POND_AUTH_TOKEN:
        print("\n  You haven't login yet.\n")
        return

    module_dir = os.path.join( pond_home, module_name )

    # repo should be created if check-module request recieved on server side
    key_file    = os.path.join(pond_home, ".git_private_key")
    res = module_request( 'check-module',{
        'module_name'      : module_name,
        'fetch_git_key'    : not os.path.exists( key_file ),
        'repo_prepare'     : '1',
    })

    # print("check-module res:",res)
    if res.get('git_private_key' ):
        file_content_set( key_file, res.get('git_private_key') )
        os.system("chmod 600 "+key_file)
        pass


    if res.get('git_private_key'):
        file_content_set( os.path.join(pond_home, ".git_private_key"), res.get('git_private_key') )
        pass

    if res['status'] == 'not_exist':
        print('\n  Module {0} does not exist.'.format(module_name) )
        return

    if res['status'] == 'someone':
        if not res['is_shared']:
            print('\n  Module {0} is not shared by its author.'.format(module_name) )
            return
        pass

    if not os.path.exists( pond_home ): os.mkdir( pond_home )
    os.chdir( pond_home )
    if not os.path.exists( module_dir ):
        # clone the repo
        print("  try clone")
        cmd = "GIT_SSH_COMMAND='ssh -i {0}/.git_private_key' git clone git@{1}:{2}.git".format(
            pond_home,POND_SERVER,module_name
        )
        print("run with cmd", cmd)
        os.system( cmd )
        pass
    else:
        print("  try pull")
        os.chdir( module_dir )
        cmd = "GIT_SSH_COMMAND='ssh -i {0}/.git_private_key' git pull".format(
            pond_home,POND_SERVER
        )
        print("run with cmd", cmd)
        os.system( cmd )
        pass

    # check if empty, if empty then prepare files from pond_root
    os.chdir( module_dir )
    files = os.listdir()
    if len( files ) == 1 and files[0] == '.git':
        #empty repo, prepare files by templates from pond_root
        print("repo is empty, initializing the repo.")
        os.system( os.path.join( pond_root, 'bin', 'pond-init-module-files.sh')+" "+module_name )
        os.system("git add .")
        os.system("git commit -m '{0} initializing'")
        cmd = "GIT_SSH_COMMAND='ssh -i {0}/.git_private_key' git push origin master".format(
            pond_home,POND_SERVER,module_name
        )
        os.system( cmd )
        pass
    
    return 


