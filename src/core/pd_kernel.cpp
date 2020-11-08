/////////////////////////////
#include"pd_headers.h"
using namespace std;
using namespace pond;

Kernel::Kernel(istream *istr_in,bool interactively,int argc,char*argv[]){
  //dout<<"try init kernel"<<endl;
  superList.SetList();
  inputList.SetList();
  //dout<<"init import list"<<endl;
  importList = new ImportList(istr_in,interactively);
  //dout<<"init eva kernel"<<endl;
#ifdef DEBUG
  evaluation = new Evaluation(true);
#else
  evaluation = new Evaluation();
#endif
  //dout<<"nealy done"<<endl;
  // evaluation->superList = this->superList;
  // evaluation->inputList = this->inputList;
  this->interactively = interactively;
  // evaluation->InteractivelyMode = interactively;
  evaluation->argc = argc;
  evaluation->argv = argv;
  //dout<<"kernel initialized"<<endl;
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
      if ( importList->GetList( superList , 0) < 0 ){
        if ( interactively ) cout<<"\n";
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
            Warning("Kernel","Function 'SimpleSimplify' from 'System' is not loaded properly.");
        }
        ssrec->staticFunction( superList.Last() );
        //dout<<"   evaluate finished, printing: "<< superList.Last() << endl;
        static const INIT_SYMID_OF( FullForm );
        if ( superList.Last().ListQ( SYMID_OF_FullForm ) ){
          if ( superList.Last().Size()!=1 ){
            ThrowError("FullForm",(string)"FullForm called with "+ToString(superList.Last().Size())+" arguments; 1 argument is expected.");
          }
          if ( interactively )
            cout<<"Out["<<printQuan<<"]//FullForm= ";
          string strout =superList.Last()[1].ToFullFormString();
          //dout<<"strout to print is:"<<strout<<endl;
          if ( strout != "" ) cout<<strout<<"\n";
        }else{
          string strout =superList.Last().ToString();
          //dout<<"strout to print is: "<<strout<<endl;
          if ( superList.Last().NullQ() ) strout = "";
          if ( interactively && strout!="")
            cout<<"Out["<<printQuan<<"]= ";
          if ( strout != "" ) cout<<strout<<"\n";
        }
      }
      if ( interactively ) cout<<"\n";
      if ( evaluation->statusCode == KernelStatusQuit ) break;
      if ( evaluation->statusCode == KernelStatusErrorThrow ) break;
      //dout<<"kernel evaluation one run done"<<endl;
    }catch ( const ExceptionQuit&err ){
      return err.code;
    }catch ( const Error&err){
      //dout<<"get error in kernel.cpp:"<<err.swhat()<<endl;
      cerr<<err.swhat()<<endl;
      evaluation->EvaluationDepth = 0;
      evaluation->ClearValueTablesTo(1);
    }catch ( const exception &err){
      cerr<<"Sourcecode::Error: "<<err.what()<<endl;
      evaluation->EvaluationDepth = 0;
      evaluation->ClearValueTablesTo(1);
    }catch ( ... ){
      cerr<<"System::Error: Unexpected error occured."<<endl;
      evaluation->EvaluationDepth = 0;
      evaluation->ClearValueTablesTo(1);
    }
  }
  return 0;
}



