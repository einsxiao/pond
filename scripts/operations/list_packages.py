from pond_basic import *
import datetime 
import json

def request(argv, options):
    scripts_dir = os.path.join( pond_root, 'src_packages' )
    os.chdir( scripts_dir )
    scripts = os.listdir()
    for s in scripts:
        if not s.endswith( ".sh" ): continue
        print( s[:-3] )
    pass
