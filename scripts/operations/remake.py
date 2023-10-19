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

    if options.get('all',None):
        print("###### try remake all modules ##########")
        for module_name in os.listdir( pond_home ):
            try:
                module_dir = os.path.join( pond_home, module_name )
                os.chdir( module_dir )
                print("########################################")
                print("######         try make",module_name)
                print("########################################")
                os.system("make clean; make; make release")
            except Exception as e:
                pass
            pass
        # should make all modules located in module directory
        return
    #normal make one module
    if len(argv) < 2:
        print("Name of a module should be specified.")
        return

    for module_name in argv[1:]:
        module_dir = os.path.join( pond_home, module_name )
        os.chdir( module_dir )
        os.system("make clean; make; make release")
    
    return 

