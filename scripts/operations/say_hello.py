from pond_basic import *
import requests
import json

def request(*args,**argw):
    print('args =', args )
    print('argw =', argw )

    s = requests.Session()

    res = s.post( POND_URL+"/pond/modules/hello/",{
        'info'   : 'test from pond client',
    })

    print(res)
    print(type(res.text), res.text)
    jres = json.loads( res.text )

    print( 'jres = ', type(jres), jres )


    return 



