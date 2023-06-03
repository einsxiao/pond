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
        print("Name of a module should be specified.")
        return
    #print( '\nTry pull module ',module_name )

    module_dir = os.path.join( pond_home, module_name )

    os.chdir( module_dir )
    # repo should be created if check-module request recieved on server side

    #print("check-module res:",res)
    os.system("make clean; make; make release")

    
    return 


