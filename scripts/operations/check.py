from pond_basic import *
import requests
import datetime 
import dateutil
import json

def pdate(dstr):
    return dstr[:16].replace('T',' ')
# login
def request(argv,options):
    # print('args =', args )
    # print('argw =', argw )
    info      = get_pond_auth_info()
    # print('get info:', info)
    username  = info.get('username')
    token     = info.get('token')

    if not token:
        print("\n  You haven't login yet.\n")
        return

    if len(argv) > 1:
        module_name = argv[1]
        # try check module information
        res = module_request('check-module',{
            'module_name'   :  module_name,
        })
        if res['status'] == 'not_exist':
            print("\n  Module {0} does not created yet.\n".format(module_name) )
            return
        if res['status'] == 'owner':
            print("\n  Module {0} is created by yourself({1}).".format(module_name, res['owner']) )
            print("    version     : {0}".format(res['version']) )
            print("    update_time : {0}".format(res['update_time']) )
            if res['homepage']:
                print("\n    Your sharepage url is https://doc.run/p/{0}\n".format(res['homepage'],) )
            return
        if res['status'] == 'someone':
            print("\n  Module {0} is owned by user with nickname {1}.\n".format(module_name, res['owner']) )
            print("    version     : {0}".format(res['version']) )
            print("    update_time : {0}".format(res['update_time']) )
            if res['homepage']:
                print("\n    You can visit the author's sharepage by url https://doc.run/p/{0}\n".format(res['homepage'],) )
                pass
            
            return

        return

    jres = module_request('check-login',{
        'username'    : username,
        'token'       : token,
    })


    if jres['status'] != 'success':
        print("\n  Your login information is expired. please use command \`pd login\` to login.")
        print(POND_PASS_HINT)
        return
        pass
    print("\n  You've signed in as user '"+username+"'\n")

    #print modules information
    if len( jres['modules'] ) > 0:
        print("  module list you've created:")
        print("  update time         \t version \t module name\t")
        print("  -------------------------------------------------------------")
        for mod in jres['modules']:
            print( "  {0}\t {1}   \t {2}".format( pdate(mod['update_time']), mod['version'],mod['name'],  ) )
            pass
        else: pass
        pass
        print("  -------------------------------------------------------------")
    else:
        print("  No modules created yet.")
        pass

    return 


