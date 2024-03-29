from pond_basic import *
import requests
import json

pond_home = os.getenv('POND_HOME')

# login
def request(argv,options):
    #print('args =', args )
    #print('argw =', argw )

    s = requests.Session()
    # s.headers.update({
    #     'Authorization','Token '+POND_AUTHR_TOKEN,
    # })

    #username
    sys.stdout.write("DocRun(https://powerbyte.online) username > ")
    sys.stdout.flush()
    username = input()
    username.strip()
    sys.stdout.flush()

    #password
    sys.stdout.write("password > ")
    sys.stdout.flush()
    password = getpass.getpass("")
    password.strip()
    sys.stdout.flush()

    res = s.post( POND_URL+"/pond/modules/login/",{
        'username'    : username,
        'password'    : password,
    },{
        'Authorization' : 'Token '+POND_AUTH_TOKEN,
    })

    jres = json.loads( res.text )

    #print( 'jres = ', type(jres), jres )

    if jres.get('status') != 'success':
        print("\n  User name or password is invalid.")
        print(POND_PASS_HINT)
    else:
        try:
            if not os.path.exists(pond_home):
                os.system("mkdir "+pond_home)
                pass
            auth_file = os.path.join(user_home,'.pond.user.auth')
            if os.path.exists(auth_file):
                os.system("chmod 600 "+auth_file)
                pass
            else:
                os.system("touch "+auth_file)
                pass
            file = open(auth_file,'w')
            file.write( res.text )
            file.close()
            os.system("chmod 600 "+auth_file)
            key_file = os.path.join(pond_home, '.git_private_key' )
            if os.path.exists(key_file):
                os.system("rm "+key_file)
                pass
            print("  You have been login successfully with DocRun user:",jres.get('username'))
        except Exception as e:
            print("  Failed to write login information. Check your permission of file ~/.pond.user.auth")
            print("  failed for:",e)
            pass
        pass

    return 


