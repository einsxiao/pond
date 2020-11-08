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

    info      = get_pond_auth_info()
    username  = info.get('username')
    token     = info.get('token')
    if not token:
        print("\n  You haven't login yet.\n")
        return
    se = requests.Session()
    res = se.post( POND_API_URL+"module-pull/",{
        'username'    : username,
        'token'       : token,
    },{'Authorization':'Token ' + POND_AUTH_TOKEN,})

    jres = json.loads( res.text )
    print('jres = ', jres)


    return 


