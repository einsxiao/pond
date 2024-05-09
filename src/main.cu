/*
  Author: Eins, Xiao
  Content: Pond interface main function
  CopyRight: CDE
*/
#include<pond.h>
#include"unistd.h"

using namespace std;
using namespace pond;

int parseArgv(int argc, char* argv[], vector<pair<string,string> > &options, vector<string>&args){
  for (int i=0; i< argc; i++){
    string cont = argv[i];
    if ( cont.size()>=2 && cont[0] == '-' ){ // is an option
      if ( cont[1] == '-' ) cont.erase(0,2);
      else cont.erase(0,1);
      int p = cont.find("=");
      if ( p ){
        options.push_back( pair<string,string>(
                             cont.substr(0,p),
                             cont.substr(p+1,cont.size()-p-1)
                           )
        );
      } else{
        options.push_back( pair<string,string>(cont,"" ) );
      }
    } else { // is a normal arg
      args.push_back( cont );
    }
  }

  return 0;
}

int main(int argc,char* argv[])
{ 
  string lang        = GetEnv("POND_LANG","zh");
  int    res         = 0;
  bool   noprint     = false;
  bool   pmark       = false;
  bool   pimport     = false;
  try{
    vector<pair<string,string> > options;
    vector<string> args;
    parseArgv( argc, argv, options, args );
    ////////////////////////////////////////////
    string rcfile = GetEnv("POND_RC");
    if ( rcfile.size() == 0 ) rcfile = GetEnv("HOME")+"/.pondrc";
    for(size_t i=0; i<options.size(); i++){
      if ( options[i].first == "version" || options[i].first == "v" ){
        cout<<"Pond Science © LiuBoCloud. All rights reserved."<<endl;
        cout<<"Version "<< POND_VERSION <<endl;
        return 0;
      }
      if ( options[i].first == "help" || options[i].first == "h" ){
        cout<<"Pond Science © LiuBoCloud. All rights reserved."<<endl;
        if ( lang == "zh"){
          cout<<"用法:  pond [选项] ..."<<endl;
          cout<<"    pond [选项] 脚本文件 ..."<<endl;
          cout<<"可选项: "<<endl;
          cout<<"    --exec='CODE'        : 解释并执行CODE"<<endl;
          cout<<"    --version    / -v    : 显示当前版本"<<endl;
          cout<<"    --noprint            : 不打印求值得到结果的行(脚本模式)"<<endl;
          cout<<"    --rcfile=rcfile      : 指定启动脚本"<<endl;
          cout<<"    --pmark              : 在交互模式中，打印模式标记"<<endl;
          cout<<"    --pimport            : 打印导入模块的详细信息"<<endl;
          cout<<"    --help       / -h    : 显示帮助信息"<<endl;
          cout<<""<<endl;
        } else {
          cout<<"Usage:  pond [option] ..."<<endl;
          cout<<"    pond [option] script-file ..."<<endl;
          cout<<"options: "<<endl;
          cout<<"    --exec='code'        : prase and evaluate CODE"<<endl;
          cout<<"    --version    / -v    : show current version"<<endl;
          cout<<"    --noprint            : do not print lines with result(script mode)"<<endl;
          cout<<"    --rcfile=rcfile      : set rc file for initializing"<<endl;
          cout<<"    --pmark              : print mode mark information in interactive mode"<<endl;
          cout<<"    --pimport            : print import information"<<endl;
          cout<<"    --help       / -h    : show help info"<<endl;
          cout<<""<<endl;
        }
        return 0;
      }
      if ( options[i].first == "rcfile" ){
        rcfile = options[i].second;
        continue;
      }
      if ( options[i].first == "noprint" ){
        noprint = true;
        continue;
      }
      if ( options[i].first == "pmark" ){
        pmark = true;
        continue;
      }
      if ( options[i].first == "pimport" ){
        pimport = true;
        continue;
      }

    }
    if ( args.size() == 1 ){
      if ( isatty( 0 ) ){
        cout<<"Pond Science © LiuBoCloud. All rights reserved."<<endl;
        Kernel kernel(&cin, true, argc, argv, rcfile, pmark, pimport);
        res = kernel.Phrasing();
      }else{
        Kernel kernel(&cin, false, argc, argv, rcfile,pmark, pimport);
        kernel.noprint = noprint ;
        res = kernel.Phrasing();
      }
    }else if ( args.size() >= 2 ){
      ////////////////////////////////////////////
      if ( args.size() > 1 ){
        string filename = args[1];
        ifstream fcin(filename.c_str());
        if ( fcin ){
          Kernel kernel(&fcin, false, argc, argv, rcfile, pmark, pimport);
          kernel.noprint = noprint;
          res = kernel.Phrasing();
        }else{
          if ( lang == "zh" ){
            _Warning("pond","脚本文件'"+filename+"'未找到");
          } else {
            _Warning("pond","Script file "+filename+" is not found.");
          }
          return 1;
        }
        fcin.close();
      }
    }else{
      if ( lang == "zh" ){
        _Warning("pond","要求至少1个参数");
      } else {
        _Warning("pond","1 argument is required.");
      }
      return 1;
    }
  }catch ( const Error&err ){
    dout<<"got pond::Error in main"<<endl;
    cerr<<err.swhat()<<endl;
  }catch ( const exception &err){
    dout<<"got exception in main"<<endl;
    if ( lang == "zh" ){
      cerr<<"\nSourcecode::Error: "<<err.what()<<endl;
    } else {
      cerr<<"\n源代码::错误: "<<err.what()<<endl;
    }
  }catch ( ... ){
    if ( lang == "zh" ){
      cerr<<"\nSystem::Error: Unexpected error occured."<<endl;
    } else {
      cerr<<"\n系统::错误: 出现未知错误."<<endl;
    }
  }
 
  return res;
}

/*/
tip 1:
	If construction function called from any place, the space of many variables will be re set. Thus the previous values set will be lost.

//*/
/*/ test code
  //////// for Dimensions of EvaObj
  EvaObj list,dim;
  ImportList::ToExpression("{1,2,3}",list);
  ImportList::ToExpression("2",list);
  ImportList::ToExpression("{{1,2},{2,3},{3,4}}",list);
  ImportList::ToExpression("{{1,2},{2,3},{3}}",list);
  cout<<EvaObj::Dimensions(list,dim)<<endl;
  cout<<list<<":"<<dim<<endl;

  TVL
  TVL(
  VL(ls)

  //cout<<"pointer null >>>"<<NULL<<"<<<"<<endl;
  EvaObj dict; dict.ToList();
  for ( int i = 0 ;i< 100;i++){
    EvaObj item;
    item.PushBackString( ToString(i)  );
    item.PushBackString( ToString(100-i)  );
    dict.DictInsert(item[1],item[2]);
  }
  dict.DictSort();
  cout<<dict<<endl;
  cout<<*dict.DictValueAt( StringList("1") )<<endl;
  dict.DictInsertOrUpdate( StringList("1"), NumberList(99) );
  cout<<*dict.DictValueAt( StringList("1") )<<endl;
  cout<<*dict.DictValueAt( StringList("77") )<<endl;
  auto res = dict.DictGetRealPtr( StringList("33") );
  cout<<(*res.first)<<"->"<<(*res.second)<<endl;
  
  complex a(3,4),b(4,5);
  double d1,d2,id1,id2;
  d1 = (a/b).re;
  d2 = 32/41.;
  id1= (a*(1/b)).im;
  id2 = 1.0/41;
  cout<<d1<<endl;
  cout<<d2<<endl;
  cout<<(d1 == d2)<<endl;
  cout<<(id1 == id2)<<endl;
  cout<<std::numeric_limits<double>::epsilon()<<endl;

  
  return 0;
//*/
 
