from pond_basic import *
import requests
import json

# login
def request(argv,options):
    # print('args =', args )
    # print('argw =', argw )
    info      = get_pond_auth_info()
    # print('get info:', info)
    username  = info.get('username')
    token     = info.get('token')

    if not token:
        print("\nYou haven't login yet.\n")
        return

    try:
        auth_file = os.path.join(user_home,'.pond.user.auth' )
        if os.path.exists(auth_file):
            os.system("chmod 600 "+auth_file)
            os.system("rm "+auth_file)
            pass
        key_file = os.path.join(pond_home, '.git_private_key' )
        if os.path.exists(key_file):
            os.system("rm "+key_file)
            pass
        print("\nSucceed to delete login information.\n")
    except Exception as e:
        print("Failed to delete login information. Check your permission of file ~/.pond.user.auth")
        print("failed for:",e)
        pass

    return 


