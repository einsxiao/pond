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

    print( 'argv = ',argv)
    print( 'options = ',options)
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
    print( 'pull module ',module_name )

    if not POND_AUTH_TOKEN:
        print("\n  You haven't login yet.\n")
        return

    module_dir = os.path.join( pond_home, module_name )


    res = module_request( 'check-module',{
        'module_name'   : module_name,
    })

    print("check-module res:",res)

    if res['status'] == 'not_exist':
        print('\n  module {0} does not exist.'.format(module_name) )
        return

    if res['status'] == 'someone':
        if not res['is_shared']:
            print('\n  module {0} is not shared by its author.'.format(module_name) )
            return

        pass
    
    if not os.path.exists( module_dir ): os.mkdir( module_dir )
    os.chdir( module_dir )
    
    return 


