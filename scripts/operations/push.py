from pond_basic import *
import requests
import datetime 
import json

# login requried
def request(argv,options):

    # options_transform(options,{'v':'version'})
    # version  = options.get('version')

    # print( 'argv = ',argv)
    # print( 'options = ',options)
    if len(argv) >= 2:
        module_name = argv[1]
    else:
        cwd = os.getcwd()
        print( pond_home)
        print( cwd )
        if not cwd.startswith( pond_home ):
            print("Name of a module should be specified if outside a module working directory.")
            return
        module_name = os.path.basename( cwd[ len(pond_home): ] )
        pass
    print( '\nTry push module ',module_name )

    if not POND_AUTH_TOKEN:
        print("\nYou haven't login yet.\n")
        return

    module_dir = os.path.join( pond_home, module_name )

    if not( os.path.exists( module_dir ) and os.path.exists(os.path.join(module_dir,'.git')) ):
        print("\nModule local files are not ready. Please do pull operation first.")
        return

    # repo should be created if check-module request recieved on server side
    key_file    = os.path.join(pond_home, ".git_private_key")
    res = module_request( 'check-module',{
        'module_name'      : module_name,
        'fetch_git_key'    : ('' if os.path.exists( key_file ) else 'yes'),
        'repo_prepare'     : '1',
        'update_file'      : '1',
    })

    # print("check-module res:",res)
    if res.get('git_private_key' ):
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
        print('\nModule {0} is not shared by its author.'.format(module_name) )
        return

    os.chdir( module_dir )
    print("try git push")
    cmd = pond_git_cmd+" push"
    # print("do pull with cmd>", cmd)
    os.system( cmd )

    print("try update repo code info")
    res = module_request( 'update-repo-code-info', {
        'module_name'      : module_name,
    })
    print("res =",res)

    if res.get('status') != 'success':
        print("update repo failed for:", res.get('status') )
        pass
    
    return 


