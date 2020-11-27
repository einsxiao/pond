from pond_basic import *
import requests
import datetime 
import json

# login requried
def request(argv,options):

    # options_transform(options,{'v':'version'})
    # version  = options.get('version')

    #print( 'argv = ',argv)
    #print( 'options = ',options)
    if len(argv) >= 2:
        module_name = argv[1]
    else:
        cwd = os.getcwd()
        #print( pond_home)
        #print( cwd )
        if not cwd.startswith( pond_home ):
            print("Name of a module should be specified if outside a module working directory.")
            return
        module_name = os.path.basename( cwd[ len(pond_home): ] )
        pass
    #print( '\nTry pull module ',module_name )

    if not POND_AUTH_TOKEN:
        print("\nYou haven't login yet.\n")
        return

    module_dir = os.path.join( pond_home, module_name )

    # repo should be created if check-module request recieved on server side
    key_file    = os.path.join(pond_home, ".git_private_key")
    res = module_request( 'check-module',{
        'module_name'      : module_name,
        'fetch_git_key'    : ('' if os.path.exists( key_file ) else 'yes'),
        'repo_prepare'     : '1',
    })

    #print("check-module res:",res)
    if res.get('git_private_key' ):
        #print("get git_private_key:", res.get('git_private_key') )
        file_content_set( key_file, res.get('git_private_key') )
        os.system("chmod 600 "+key_file)
        pass


    if res.get('git_private_key'):
        file_content_set( os.path.join(pond_home, ".git_private_key"), res.get('git_private_key') )
        pass

    if res.get('status') == 'not_exist':
        print('\nModule {0} does not exist.'.format(module_name) )
        return

    if res.get('status') == 'someone':
        if not res.get('is_shared'):
            print('\nModule {0} is not shared by its author.'.format(module_name) )
            return
        pass

    if not os.path.exists( pond_home ): os.mkdir( pond_home )
    os.chdir( pond_home )
    if not os.path.exists( module_dir ):
        # clone the repo
        #print("try clone")
        cmd = pond_git_cmd + " clone git@{0}:{1}.git".format(POND_SERVER, module_name)
        #print("do clone with cmd>", cmd)
        os.system( cmd )
        pass
    else:
        #print("try pull")
        os.chdir( module_dir )
        cmd = pond_git_cmd+" pull"
        #print("do pull with cmd>", cmd)
        os.system( cmd )
        pass

    # check if empty, if empty then prepare files from pond_root
    os.chdir( module_dir )
    files = os.listdir()
    if len( files ) == 1 and files[0] == '.git':
        #empty repo, prepare files by templates from pond_root
        print("repo is empty, initializing the repo.")
        os.system( os.path.join( pond_root, 'bin', 'pond-init-module-files.sh')+" "+module_name )
        os.system(pond_git_cmd+" add .")
        os.system(pond_git_cmd+" commit -m '{0} initializing'".format(module_name) )
        cmd = pond_git_cmd+ " push origin master"
        os.system( cmd )
        pass
    
    return 


