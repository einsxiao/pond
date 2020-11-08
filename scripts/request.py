from pond_basic import *
import importlib

def request(operation='say_hello',*args,**argw):
    mod = importlib.import_module("requests."+operation)
    return mod.request(self,*args)
