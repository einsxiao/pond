
from pond_basic import *
import datetime 
import json

def request(argv, options):
    scripts_dir = os.path.join( pond_root, 'src_packages' )
    os.chdir( scripts_dir )
    scripts = os.listdir()
    res = []
    for s in scripts:
        if not s.endswith( ".sh" ): continue
        res.append( s )
        pass
    if len(argv) >= 2:
        package_name = argv[1]
        pass
    if not (package_name+".sh") in res:
        print("package ",package_name," is not avialable")
        return
    os.system("./"+package_name +".sh")

    pass
