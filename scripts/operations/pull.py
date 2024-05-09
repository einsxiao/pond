from pond_basic import *
import subprocess;

# login requried
def request(argv,options):

    # options_transform(options,{'v':'version'})
    # version  = options.get('version')

    #print( 'argv = ',argv)
    #print( 'options = ',options)
    if len(argv) >= 2:
        module_name = argv[1]
    else:
        cwd = os.getcwd()
        #print( pond_home)
        #print( cwd )
        if not cwd.startswith( pond_home ):
            print("Name of a module should be specified if outside a module working directory.")
            return
        module_name = os.path.basename( cwd[ len(pond_home): ] )
        pass
    #print( '\nTry pull module ',module_name )

    if not POND_AUTH_TOKEN:
        print("\nYou haven't login yet.\n")
        return

    module_dir = os.path.join( pond_home, module_name )

    # repo should be created if check-module request recieved on server side
    key_file    = os.path.join(pond_home, ".git_private_key")
    #print("should get key", fetch_git_key)
    res = module_request( 'check-module',{
        'module_name'      : module_name,
        'fetch_git_key'    : 'yes',
        'repo_prepare'     : '1',
    })
    if res.get('git_private_key' ):
        #print("get git_private_key:", res.get('git_private_key') )
        file_content_set( key_file, res.get('git_private_key') )
        os.system("chmod 600 "+key_file )
        pass


    if res.get('git_private_key'):
        file_content_set( os.path.join(pond_home, ".git_private_key"), res.get('git_private_key') )
        os.system("chmod 600 "+key_file )
        pass

    if res.get('status') == 'not_exist':
        print('\nModule {0} does not exist.'.format(module_name) )
        return

    perm_type = res.get("perm_type")
    branch = 'master'
    if perm_type == 'lib_exec':
        branch = 'exec'
        pass

    # if res.get('status') == 'someone':
    #     if not res.get('is_shared'):
    #         print('\nModule {0} is not shared by its author.'.format(module_name) )
    #         return
    #     pass

    if not os.path.exists( pond_home ): os.mkdir( pond_home )
    os.chdir( pond_home )
    print("changed to dir:", pond_home )
    if not os.path.exists( module_dir ):
        # clone the repo
        cmd = pond_git_cmd + " clone -b {3} ssh://git@{0}:{1}/{2}.git".format(POND_SERVER, POND_GIT_PORT, module_name, branch)
        print("try clone with cmd>", cmd)
        os.system( cmd )
        print('clone done')
        pass
    else:
        os.chdir( module_dir )
        #print("chdir to ", module_dir )
        if not os.path.exists( './.git/HEAD' ):
            os.chdir( module_dir )
            os.system( "if [ -d  .temp ];then rm -rf .temp; fi" )
            cmd = pond_git_cmd + " clone -b {3} ssh://git@{0}:{1}/{2}.git .temp".format(POND_SERVER, POND_GIT_PORT, module_name, branch)
            #print("no git, try with cmd>", cmd)
            os.system( cmd )
            os.system( "cp -rf .temp/. ./; rm -rf .temp;")
            pass
        else:
            cmd = pond_git_cmd + " pull origin {0}".format(branch)
            print("try pull with cmd>", cmd)
            os.system( cmd )
            print('pull done')
            pass
        pass

    # check if empty, if empty then prepare files from pond_root
    os.chdir( module_dir )
    files = os.listdir()
    just_inited = False
    if not module_name+"Module.h" in files:
        #empty repo, prepare files by templates from pond_root
        print("repo is empty, initializing the repo.")
        os.system( os.path.join( pond_root, 'bin', 'pond-init-module-files.sh')+" "+module_name )
        just_inited = True
        pass

    if just_inited:
        os.system(pond_git_cmd+" add ." )
        os.system(pond_git_cmd+" commit -m '{0} initializing'".format(module_name) )
        #print("first push with command >",cmd)
        cmd = pond_git_cmd+ " push origin master"
        os.system( cmd )
        pass
    if os.path.exists( "Makefile" ):
        os.system("make release 2>&1" )
    
    return 


