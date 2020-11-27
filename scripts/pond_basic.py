#!/usr/bin/env python
#coding:utf-8
################################################
## author: Eins Xiao
## version: 2020.11.7
################################################
## basic functions may be called from anywhere##
################################################
import time
import hashlib
import getpass
import re
import sys
import os
import pwd
# import socket
# import crypt
import random
import string
import subprocess
import traceback
import inspect
import json
import requests
DEBUG = False

POND_SERVER        = os.getenv("POND_SERVER")
POND_URL           = os.getenv("POND_URL")
POND_PASS_HINT     = """
  If you are not signed up or forget the account info, 
  please visit https://doc.run to signin/signup.
"""


pond_root = os.getenv("POND_ROOT");
user_home = os.getenv('HOME');
pond_home = os.getenv('POND_HOME')
if ( not pond_root ):
    pond_root = os.path.join(user_home,".pond")
if ( not pond_home ):
    pond_home = os.path.join(user_home,"pond-modules")
    pass
pond_git_cmd = os.getenv("POND_GIT_CMD")

BLK_SIZE      = 1020

operation_auth_free =  {'register_user','check_user_info','connect_user','find_back', 'mod_register_user','mod_check_user_info','mod_connect_user','mod_find_back','mod_ping',}

# if DEBUG:print("Debug Mode")

class CONSTANTS:
    RUNNING_SIDE = "server"
    pass

random.seed()

def rtraceback():
    print('-'*60)
    traceback.print_exc()
    print('-'*60)

def dtraceback():
    if DEBUG:
        print('debug traceback:')
        print('-'*60)
        traceback.print_exc()
        print('-'*60)

def line_no():
    return inspect.currentframe().f_back.f_lineno;

def file_name():
    return inspect.currentframe().f_back.f_code.co_filename

def args_get(sn,args,default_value=None):
    if len(args) > sn: return args[sn]
    else: return default_value

def dprint(*params):
    if DEBUG:
        print(inspect.currentframe().f_back.f_code.co_filename,inspect.currentframe().f_back.f_lineno,":",end='',flush=True)
        for i in params:
            print(i,end='',flush=True)
        print()
logfile = pond_root+"/log"
def log(content,item='debug'):
    if not content: return
    file_content_append(logfile+"/"+item,content+"\n")

class switch(object):  
    def __init__(self, value):  
        self.value = value  
        self.fall = False  
  
    def __iter__(self):  
        """Return the match method once, then stop"""  
        yield self.match  
        raise StopIteration  
  
    def match(self, *args):  
        """Indicate whether or not to enter a case suite"""  
        if self.fall or not args:  
            return True  
        elif self.value in args: # changed for v1.5, see below  
            self.fall = True  
            return True  
        else:  
            return False 

        pass
    pass

class TerminalSizeType:
    row = 0
    col = 0
    def __init__(self,row = 0, col = 0):
        self.row  = row
        self.col = col
        pass
    pass

def shell_status_output(cmd):
    try:
        output = subprocess.check_output(cmd,shell=True)
        return (0,output)
    except subprocess.CalledProcessError as e:
        status = e.returncode
        output = e.output
        return (e.returncode, str(e) )
    except Exception as e:
        status = -1
        output = e
        return ( -1 ,str(e) )
    pass

def shell_output(cmd):
    return subprocess.check_output(cmd,shell=True)

def terminal_size():
    if sys.stdin.isatty():
        return TerminalSizeType(
            int( shell_output('stty size').split()[0] ),
            int( shell_output('stty size').split()[1] )
        )
    else:
        return TerminalSizeType(10,10);
    pass

COL_SIZE = 40
def print_progress(progress = 0, total_size = 0, volocity = 0, col_size =  COL_SIZE ):
    if col_size == 0 : col_size = (terminal_size().col)
    sys.stdout.write('\r'+' '*(col_size+20)+'\r')
    sys.stdout.write("\r    [")
    psize = int( col_size ) 
    vol = 0.0
    for i in range( psize ):
        if ( i <= psize*progress ):
            sys.stdout.write( '#' )
        else:
            sys.stdout.write(' ')
            pass
        pass
    sys.stdout.write("]    ")
    sys.stdout.write( "%.0f/%s KB   %.1f KB/s"%(total_size*progress,total_size, volocity) )
    sys.stdout.flush()
    #time.sleep(1)

def get_user_name():
    return getpass.getuser()

def get_mac_address():
    import uuid
    node = uuid.getnode()
    mac = uuid.UUID(int = node).hex[-12:]
    return mac

def is_mac_address(mac=''):
    if re.match('^[0-9a-f]{6}',mac) and len(mac) == 12:
        return True
    return False

# def calculate_file_md5(filepath): #fp is opened file
#     if not os.path.exists(filepath):
#         return None
#     local_md5 = hashlib.md5()
#     with open(filepath,'r') as fp:
#         while True:
#             blk = fp.read(32768) # 32kb per block
#             if not blk: break
#             local_md5.update( blk.encode() )
#     return local_md5.hexdigest()

# def file_md5(filepath):
#     return calculate_file_md5(filepath)

# def text_md5_new(text):
#     return hashlib.md5( text[0] ).hexdigest()

# def text_md5(text):
#     return hashlib.md5( text ).hexdigest()


def get_file_info(filepath):
    stat = os.stat(filepath)
    uid = stat.st_uid
    pw = pwd.getpwuid(uid)
    return (pw.pw_name,"%d"%uid)

def file_content_get(filepath):
    filecontent = ''
    if not os.path.exists(filepath): return "";
    with open(filepath,'r') as fp:
        while True:
            blk = fp.read(32768) # 32kb per block
            if not blk: break
            filecontent += blk
    return filecontent

def file_content_set(filepath,filecontent=''):
    with open(filepath,'w') as fp:
        fp.write(filecontent)

def file_content_append(filepath,filecontent=b''):
    if not filecontent: return None
    with open(filepath,'a') as fp:
        fp.write(filecontent)
    
def str_start_with(line,word):
    if line[0:len(word)]==word:
        return True
    return False

def str_end_with(line,word):
    if line[-len(word):]==word:
        return True
    return False

def str_after(line,word):
    wlen = len(word)
    if line[0:wlen]==word:
        return line[wlen:]
    return None 
    
def line_trim_comment(line):
    pos = line.find('#')
    if pos<0:
        return line
    else:
        return line[:pos]

# def mcrypt(passstr):
#     chars = string.ascii_letters + string.digits
#     salt = random.choice(chars)+random.choice(chars)
#     return crypt.crypt(passstr,salt)

def ctimestr():
    return time.strftime('%Y-%m-%d %H:%M:%S')

def unpackdict(dstr):
    if type(dstr) != type('a'):return {}
    dic = {}
    for pair in dstr.replace('\n',';').split(';'):
        if pair != '':
            (key,value) = pair.split(':')
            dic[key]=value
    return dic

def packdict(dic):
    if type(dic) != type({}): return ''
    dstr = ''
    for (key,value) in dic.items():
        dstr+='%s:%s;'%(key,value)
    return dstr
def dictQ(dic,key,value=None):
    if not dic: return False
    if not type(dic) == dict: return False
    if not key in dic: return False
    if value:
        return dic[key] == value;
    else:
        return True

def strstrip(string):
    return string.strip()

def get_modulename_from_path(path=''):
    if not path: path = os.getcwd()
    pond_dir = pond_home
    modules_dir = os.path.join(pond_dir,'pond-modules')
    #dprint(pond_dir,modules_dir)
    while True:
        dirname = os.path.dirname(path)
        #dprint('path = %s dirname = %s'%(path,dirname) )
        if os.path.exists(modules_dir) and os.path.samefile(dirname,modules_dir):break
        if os.path.samefile(dirname,pond_dir):break
        path = os.path.dirname(path)
        if path=='/': return None
    return os.path.basename(path)

def check_version_string(string=''):
    if not string: return False
    vers = string.split('.')
    if not len(vers) in (1,2,3): return False
    if False in list(map( lambda x: x.isdigit(), vers)): return False
    return True

def get_version_from_string(string=''):
    if not string: return None 
    vers = string.split('.')
    if not len(vers) == 3: return None
    if False in list(map( lambda x: x.isdigit(), vers)): return None 
    return list(map(lambda x: int(x) ,vers))

######
# single:options   transform: short option value to full option
######
def argv2argsoptions(args,single=[],transform={}):
    argsleft = []
    options = {}
    key = None
    for i in range(len(args)):
        arg = args[i]
        if arg[0] == '-':
            while arg and arg[0] == '-': arg = arg[1:]
            if not arg : raise Exception('Invalid argument specified "%s"'%args[i])
            key = arg
            #transform
            tokey = transform.get(key)
            if tokey: key = tokey
            if key in single: 
                options[key] = 'True'
                key = None
            continue
        if key:
            options[key] = arg
            key = None
        else:
            argsleft.append(arg)
    return (argsleft,options)

def options_transform(options,transform={}):
    for key in transform:
        value = options.get(key)
        if value:
            del options[key]
            options[ transform[key] ] = value
    return options

def get_option(options={},key='',default_value=''):
    if not options: return default_value
    value = options.get(key)
    if value: return value
    return default_value

def get_language(): #en-US, zh_CN and so on
    lang = os.getenv("POND_LANG")
    if not lang: lang = os.getenv("LANG").split('.')[0].replace('-','_')
    if not lang: lang = 'zh'
    return lang

##########################################
#uncoding and encoding class

from binascii import b2a_hex, a2b_hex
import base64

def to_bytes(s):
    if isinstance(s,str): return s.encode()
    return s

def to_str(s):
    if not isinstance(s,str): return s.decode()
    return s
    
def encoding(s): #encode a string
    if isinstance(s,str): s = s.encode()
    return base64.encodestring(s) #creq1

def decoding(s): #decode a encoded string	
    if isinstance(s,str): s = s.encode()
    missing_padding = 4 - len(s)%4
    if missing_padding:
        s+=b'='*missing_padding
    return base64.decodestring(s) #creq1


def random_key(bytelen = 32,base= None):
    if not base: base = list(string.ascii_letters)+list(string.digits)+list('!@#$%^&*()<>,./?')
    else: base = list(base)
    key = ''
    for i in range(0,bytelen):
        key = key + random.choice(base)
    return key

def random_simple_key(bytelen = 32,base= None):
    if not base: base = list(string.ascii_letters)+list(string.digits)
    else: base = list(base)
    key = ''
    for i in range(0,bytelen):
        key = key + random.choice(base)
    return key

def passcrypt(passwd):
    if isinstance(passwd,str): passwd = passwd.encode()
    return hashlib.sha1( passwd ).hexdigest()


def get_pond_auth_info():
    auth_file = os.path.join(user_home,'.pond.user.auth')
    if not os.path.exists(auth_file):
        return {'status': "NONE",}
    os.system('chmod +r '+auth_file)
    hfile = open(auth_file)
    info = hfile.readline()
    hfile.close()
    os.system('chmod 000 '+auth_file)
    try:
        return json.loads( info )
    except Exception as e:
        print("failed to parse info:",info)
        print('error:',e)
        return {'status': "NONE",}
    pass

        
__info__   = get_pond_auth_info()
POND_AUTH_TOKEN    = __info__.get('token')    or ''
POND_AUTH_USERNAME = __info__.get('username') or  ''

def module_request(operation_url,data):
    try:
        se = requests.Session()
        res = se.post(
            POND_URL+"/pond/modules/"+operation_url+"/",
            data ,
            headers={'Authorization':'Token ' + POND_AUTH_TOKEN,}
        )
        jres = json.loads( res.text )
        #print("module request get result:", jres)
        if not jres.get('status'):
            if jres.get('detail'):
                print("request failed for:",jres.get('detail') )
        return jres
    except Exception as e:
        print("request failed:", e)
        return {
            'status'  : 'failed',
        }
        pass
