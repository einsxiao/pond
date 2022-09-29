/////////////////////////////
#include"pd_headers.h"
using namespace std;
using namespace pond;

#define __MODULE_NAME__ 

Kernel::Kernel(istream *istr_in,bool interactively,int argc,char*argv[],
               string rcfile,bool pmark, bool pimport){
  //dout<<"try init kernel"<<endl;
  superList.SetList();
  inputList.SetList();
  //dout<<"init import list"<<endl;
  importList = new ImportList(istr_in,interactively);
  //dout<<"init eva kernel"<<endl;
  
  this->pmark = pmark;
#ifdef DEBUG
  evaluation = new Evaluation(true, pmark, pimport);
#else
  evaluation = new Evaluation(false,pmark, pimport);
#endif
  //dout<<"nealy done"<<endl;
  // evaluation->superList = this->superList;
  // evaluation->inputList = this->inputList;
  this->interactively = interactively;
  // evaluation->InteractivelyMode = interactively;
  evaluation->argc = argc;
  evaluation->argv = argv;
  //dout<<"kernel initialized"<<endl;

  // evaluate the rcfile content
  if ( FileExistQ( rcfile ) ){
    //dout<<"try init rcfile"<<endl;
    Object tmp;
    evaluation->EvaluateFile( rcfile, tmp );
  } else {
    //dout<<"rcfile file not found"<<rcfile<<endl;
  }
}

Kernel::~Kernel(){
  delete importList;
  delete evaluation;
}

int Kernel::Phrasing( ){
  //dout<<"start phrasing"<<endl;
  u_int printQuan=0;
  bool printed = false;
  while ( !importList->End() ){ // one expression per loop
    //dout<<"\n\ntry read more expression and evaluate"<<endl;
    try{
      if ( interactively && not printed ){
        cout<<"In["<<printQuan+1<<"]:="; // print the head
        printed = true;
      }
      const int g_res = importList->GetList( superList , 0);
      //dout<< "read list with result "<< g_res <<" " <<superList.Size() << endl;
      if (  g_res < 0 ){
        if ( interactively ) cout<<"\n";
        printed = false;
        continue; //get Argv
      }
      if ( superList.Size() > printQuan ){
        printQuan++; printed = false;
        inputList.PushBackRef(superList.Last());
        //dout<<endl<<" try evaluate >>>  "<< superList.Last()<<"  <<<"<<endl;
        evaluation->Evaluate(superList.Last());
        //dout<<" evaluate done:"<< superList.Last() <<endl;
        //dout<<" try simplify result"<<endl;

        static EvaRecord *ssrec= evaluation->GetEvaRecord( "SimpleSimplify" );
        //dout<<"simplesimplfy with function:"<<ssrec<<endl;
        if ( ssrec == NULL ){
          zhWarning("Kernel","函数'SimpleSimplify'未能从模块'System'加载成功.")||
            _Warning("Kernel","Function 'SimpleSimplify' from 'System' is not loaded properly.");
        }
        ssrec->staticFunction( superList.Last() );
        //dout<<"   evaluate finished, printing: "<< superList.Last() << endl;
        static const INIT_SYMID_OF( FullForm );
        if ( this->noprint ) continue;
        if ( superList.Last().ListQ( SYMID_OF_FullForm ) ){
          if ( superList.Last().Size()!=1 ){
            ThrowError("FullForm",(string)"FullForm called with "+ToString(superList.Last().Size())+" arguments; 1 argument is expected.");
          }
          if ( interactively ){
            cout<<"Out["<<printQuan<<"]//FullForm= ";
          }
          string strout =superList.Last()[1].ToFullFormString();
          if ( strout == "" ) continue;
          cout<<strout<<"\n";
        }else{
          string strout =superList.Last().ToString();
          if ( !this->pmark && superList.Last().NullQ() ) continue;
          if ( strout == "" ) continue;
          if ( interactively ){
            cout<<"Out["<<printQuan<<"]= ";
          }
          cout<<strout<<"\n";
        }
      }
      if ( interactively ) cout<<"\n";
      if ( evaluation->statusCode == KernelStatusQuit ) break;
      if ( evaluation->statusCode == KernelStatusErrorThrow ) break;
      //dout<<"kernel evaluation one run done"<<endl;
    }catch ( const ExceptionQuit&err ){
      return err.code;
    }catch ( const Error&err){
      //dout<<"get pond::Error in kernel.cpp:"<<err.swhat()<<endl;
      cerr<<err.swhat()<<endl;
      evaluation->EvaluationDepth = 0;
      evaluation->ClearValueTablesTo(1);
    }catch ( const exception &err){
      //dout<<"get exception in kernel.cpp:"<<err.what()<<endl;
      zhErroring("源代码",err.what() )||
        _Erroring("Sourcecode",err.what() );
      evaluation->EvaluationDepth = 0;
      evaluation->ClearValueTablesTo(1);
    }catch ( ... ){
      zhErroring("系统","出现未预料到的错误." )||
        _Erroring("System","Unexpected error occured." );
      evaluation->EvaluationDepth = 0;
      evaluation->ClearValueTablesTo(1);
    }
  }
  if ( evaluation->statusObject.NumberQ() ){
    return evaluation->statusObject.Number();
  }
  return 0;
}



