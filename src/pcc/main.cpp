#include<iostream>
#include"pcc.h"
using namespace std;
using namespace pond;

int main(int argc,char* argv[]){
  if ( argc == 1 ){
    cout<<"pcc: fatal error: no input files"<<endl;
    cout<<"pcc is a customed compiler for pond high performance platform."<<endl;
    cout<<"more about pcc: https://powerbyte.online/pd/ce1b95e8dbe99794"<<endl;
    return 0;
  }
  try{
    //cerr<<"argc, argv is"<<argc<<","<<argv<<endl;
    CmdLine   cl(argc,argv);
    //cerr<<"cmd constructed try construct engine"<<endl;
    Engine    eng(cl);
    //cerr<<"try processing"<<endl;
    return eng.processing();
  }catch (const cc_err &err){
    cerr<< err.info() <<endl;
    return 1;
  }catch (const Error &err){
    cerr<<"pcc:  error:  "<<err.message<<endl;
    return 1;
  }catch ( const exception &err){
    cerr<<endl<<"pcc : internal fatal error: "<<err.what()<<endl;
    return 2;
  }catch ( ... ){
    cerr<<endl<<"pcc : internal fatal unexpected unknown error. "<<endl;
    return 3;
  }
  return 0;
}
