from pond_basic import *
import requests
import datetime 
import json

# login requried
def request(argv,options):

    # options_transform(options,{'a':'all'})
    # version  = options.get('version')
    all = options.get('all') or options.get('a')

    #print( 'argv = ',argv)
    #print( 'options = ',options)
    if len(argv) == 1 and all:
        os.system("for m in `ls {0}`; do cd {0}/$m;make clean; make; make release; done".format(pond_home))
        return
    if len(argv) < 2:
        print("Name of a module should be specified.")
        return
    for module_name in argv[1:]:
        #print( '\nTry pull module ',module_name )
        module_dir = os.path.join( pond_home, module_name )
        os.chdir( module_dir )
        # repo should be created if check-module request recieved on server side
        #print("check-module res:",res)
        os.system("make clean; make; make release")

    
    return 


