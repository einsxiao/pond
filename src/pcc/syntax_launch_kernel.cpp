#include"pcc.h"
#include "syntax_launch_kernel.h"

int launch_kernel::Init(){
  //cout<<"try init lank"<<endl;
  //check self
  if ( pragma->self->eles.size() != 4 ){
    warnErrorAtPos( pragma->self->start,
                    "kernel profile and kernel function arguments and input variable list is expected. No other things allowed.");
    if ( pragma->self->eles.size() > 4 )
      throwCCErrorAtPos( pragma->self->eles[4]->start,"extra expression find here.");
    if ( pragma->self->eles.size() < 4 )
      throwCCErrorAtPos( pragma->self->end-1,"argument list is missed.");
  }
  //cout<<"length check done"<<endl;
  // check args list format
  //cout<<"check args list"<<endl;
  if ( pragma->self->eles[2]->type != "<<<>>>" )
    throwCCErrorAtPos( pragma->self->eles[3]->start, "a kernel dimension profile enclosed by '<<<' and  '>>>' is expected here.");
  if ( pragma->self->eles[3]->type != "bracket" )
    throwCCErrorAtPos( pragma->self->eles[3]->start, "arguments list and input variables of form '(type1 arg1:var1, ...)' is expected here.");
  //check context
  // get declaration position and defination position
  if ( pragma->parent->type != "block" ){
    throwCCErrorAtPos( pragma->self->start,"pragma launch_kernel is not properly used within current context.");
  }

  int p_pos = pragma->pos; //NodeIndex insert_pos;
  if ( tree->eles[p_pos]->type != "block" )
    throwCCErrorAtPos( pragma->self->start,"pragma launch_kernel is not properly used within current context.");
  define_insert_pos = tree->eles[p_pos]->end;
  
  for(;;){
    p_pos --;
    if ( p_pos < 0 ) break;
    if ( tree->eles[p_pos]->type == "semicolon" or
         tree->eles[p_pos]->type == "block"     or
         tree->eles[p_pos]->type == "pragma" ) 
      break;
  }
  declare_insert_pos = tree->eles[p_pos+1]->start;
  //get content position
  if ( pragma->level_pos+1 >= pragma->parent->eles.size() or
       pragma->parent->eles[pragma->level_pos+1]->type != "block"){
    throwCCErrorAtPos( pragma->self->end,"pragma launch_kernel should followed by a block enclosed by '{' and '}'.");
  }
  content_start_pos = pragma->parent->eles[pragma->level_pos+1]->start ;
  content_end_pos   = pragma->parent->eles[pragma->level_pos+1]->end ;

  //iter iterator profile
  //cout<<" deal iter profile"<<endl;
  this->iterNode  = pragma->self->eles[2];
  iters_start_pos = this->iterNode->start.column;
  if ( this->iterNode->eles.size() == 0 )
    throwCCErrorAtPos( this->iterNode->start, "loop or thread profile is not allowed to be empty.");
  u_int pos = 0;
  while ( pos <  this->iterNode->eles.size() ){ // i: Ni
    //i
    if ( this->iterNode->eles[pos]->type != "word" )
      throwCCErrorAtPos( this->iterNode->eles[pos]->start, "a loop or thread index variable is expected here." );

    iters.push_back( launch_kernel::__iter_rec() );

    iters.back().istart = this->iterNode->eles[pos]->start.pos ;
    iters.back().iend   = this->iterNode->eles[pos]->end.pos ;

    //:
    pos++;
    if ( pos >= this->iterNode->eles.size() )
      throwCCErrorAtPos( this->iterNode->eles[pos-1]->end+1, "a ':' is expected here.");
    if ( this->iterNode->eles[pos]->type != "colon" )
      throwCCErrorAtPos( this->iterNode->eles[pos]->start, "a ':' is expected here.");

    //:Ni[,]
    pos++;
    if ( pos >= this->iterNode->eles.size() )
      throwCCErrorAtPos( this->iterNode->eles[pos-1]->end+1, "loop or thread dimension is expected here.");
    iters.back().Nstart = this->iterNode->eles[pos]->start.pos;
    while ( pos < this->iterNode->eles.size() and this->iterNode->eles[ pos ]->type != "comma" )
      pos ++;
    iters.back().Nend   = this->iterNode->eles[pos-1]->end.pos;
    pos++;
  }
    
  //args list and input list
  //cout<<"deal input Node parts"<<endl;
  this->inputNode = pragma->self->eles[3];
  args_start_pos = this->inputNode->start.column;
  if ( this->inputNode->eles.size() == 0 )
    throwCCErrorAtPos( this->inputNode->start, "arguments and input variables list  is not allowed to be empty.");
  pos = 0;
  while ( pos < this->inputNode->eles.size() ){// type arg: var,...
    //type arg :
    args.push_back( launch_kernel::__arg_rec() );
    args.back().typestart = this->inputNode->eles[pos]->start.pos;
    int t_pos = pos;
    while ( pos < this->inputNode->eles.size() and this->inputNode->eles[pos]->type != "colon" )
      pos ++;
    if ( pos >= this->inputNode->eles.size() or this->inputNode->eles[pos]->type != "colon" )
      throwCCErrorAtPos( this->inputNode->eles[t_pos]->start, "arguments and variable list of form 'type arg: var' is expected here.");
    if ( pos - t_pos < 2 ) // int(0) a(1):(2) ina(3),
      throwCCErrorAtPos( this->inputNode->eles[t_pos]->start, "type or arg of form 'type arg: var' is expected here.");
    if ( this->inputNode->eles[pos-1]->type != "word" )
      throwCCErrorAtPos( this->inputNode->eles[pos-1]->start, "arg of form 'type arg: var' is expected to be a valid c++ variable name.");
    args.back().typeend  = this->inputNode->eles[pos-2]->end.pos;
    args.back().argstart = this->inputNode->eles[pos-1]->start.pos;
    args.back().argend   = this->inputNode->eles[pos-1]->end.pos;

    pos++; // to pass through the ':'

    //var
    if ( pos == this->inputNode->eles.size() )
      throwCCErrorAtPos( this->inputNode->eles[pos-1]->end, "an input variable is expected here.");
    args.back().varstart = this->inputNode->eles[pos]->start.pos;
    while ( pos < this->inputNode->eles.size() and this->inputNode->eles[pos]->type != "comma" )
      pos++;
    args.back().varend   = this->inputNode->eles[pos-1]->end.pos;
    pos++;
  }
  // block node
  // this->blockNode = pragma->parent->eles[ pragma->
  //cout<<"launch kernel init done"<<endl;
  return 0;
}

string launch_kernel::iter_i(int i){
  return content->substr( iters[i].istart, iters[i].iend -  iters[i].istart );
}
string launch_kernel::iter_N(int i){
  return content->substr( iters[i].Nstart, iters[i].Nend -  iters[i].Nstart );
}
string launch_kernel::iter_Ni(int i){
  return "_N"+iter_i(i);
}
string launch_kernel::arg_type(int i){
  return content->substr( args[i].typestart, args[i].typeend -  args[i].typestart );
}
string launch_kernel::arg_arg(int i){
  return content->substr( args[i].argstart, args[i].argend -  args[i].argstart );
}
string launch_kernel::arg_var(int i){
  return content->substr( args[i].varstart, args[i].varend -  args[i].varstart );
}


string  launch_kernel::declare_cpu(){
  string out = "static            void";
  out += string(this->iterNode->start.column - 13,' ');
  out += " cpu_kernel_"+ToString(1000+serial_number);
  out += "(";
  //cout<<"launch kernel cpu header:"<<out<<endl;
  out += string(args[0].typestart -this->inputNode->start.pos-1, ' ');
  out += arg_type(0); 
  out += string( args[0].argstart - args[0].typeend ,' ');
  out += arg_arg(0);
  for ( u_int i = 1; i< args.size(); i++ ){
    out += string( args[i].typestart - args[i-1].argend -1 ,' ')+",";
    out += arg_type(i); 
    out += string( args[i].argstart - args[i].typeend ,' ');
    out += arg_arg(i); 
  }
  out += string ( this->inputNode->end.pos - args.back().argend -1, ' ');
  for ( u_int i=0; i< iters.size(); i++ ){
    out += ",const int _N"+iter_i(i);
  }
  out += ")";
  return out;
}

string  launch_kernel::declare_gpu(){
  string out = "static __global__ void";
  //cout<<"try calculate insert content for gpu"<<endl;
  //cout<<this->iterNode->start.column<<endl;
  out += string(this->iterNode->start.column - 13,' ');
  out += "cuda_kernel_"+ToString(1000+serial_number);
  out += "(";
  //cout<<"launch kernel gpu header:"<<out<<endl;
  out += string(args[0].typestart -this->inputNode->start.pos-1, ' ');
  out += arg_type(0); 
  out += string( args[0].argstart - args[0].typeend ,' ');
  out += arg_arg(0); 
  //cout<<"out 1 ="<<out<<endl;
  for ( u_int i = 1; i< args.size(); i++ ){
    out += string( args[i].typestart - args[i-1].argend -1 ,' ')+",";
    out += arg_type(i); 
    out += string( args[i].argstart - args[i].typeend ,' ');
    out += arg_arg(i); 
  }
  out += string ( this->inputNode->end.pos - args.back().argend -1, ' ');
  // the Ni,Nj,Nk
  for ( u_int i=0; i< iters.size(); i++ ){
    out += ",const int _N"+iter_i(i); 
  }
  string N_strs[9];
  N_strs[ iters.size() ] = "";
  for ( int i = iters.size()-1; i>=0; i-- ){
    N_strs[i] = iter_i(i)+N_strs[i+1];
    if ( i < (int)iters.size() - 1 ) {
      out += ",const int _N"+N_strs[i];
    }
  }
  out += ")";
  //cout<<"gpu output is:" << out << endl;
  return out;
}

string  launch_kernel::call_cpu(){
  string out;
  out += "cpu_kernel_"+ToString(1000+serial_number);
  out += string(this->inputNode->start.column - pragma->self->start.column - 15,' ');
  //cout<<"launch kernel header:"<< out<<endl;
  out += "(";
  out += string(args[0].varstart -this->inputNode->start.pos-1, ' ');
  out += arg_var(0); 
  for ( u_int i = 1; i< args.size(); i++ ){
    out += string( args[i].varstart - args[i-1].varend -1 ,' ')+",";
    out += arg_var(i); 
  }
  out += string ( this->inputNode->end.pos - args.back().varend -1, ' ');
  for ( u_int i = 0; i< iters.size(); i++ ){
    out += ","+ iter_N(i); 
  }
  out += ");";
  return out;
}

string  launch_kernel::call_gpu(){
  string out;
  out += "cuda_kernel_"+ToString(1000+serial_number);
  out += string(this->iterNode->start.column - pragma->self->start.column - 18,' ');
  out += "<<<_cuDim(";
  out += string(iters[0].Nstart - this->iterNode->start.pos -3 , ' ');

  out +=  iter_N(0); 
  for ( u_int i = 1; i < iters.size(); i++ ){
    out += string( iters[i].Nstart - iters[i-1].Nend -3, ' ')+"*";
    out += iter_N(i); 
  }
  out += ")>>>";
  //out+="<<< _cuDim(9*10) >>>";
  out += "(";
  out += string(args[0].varstart -this->inputNode->start.pos-1, ' ');
  out += arg_var(0); 
  for ( u_int i = 1; i< args.size(); i++ ){
    out += string( args[i].varstart - args[i-1].varend -1 ,' ')+",";
    out += arg_var(i); 
  }
  out += string ( this->inputNode->end.pos - args.back().varend -1, ' ');
  for ( u_int i = 0; i< iters.size(); i++ ){
    out += ","+ iter_N(i); 
  }

  string N_strs[9];
  N_strs[ iters.size() ] = "";
  for ( int i = iters.size()-2; i>=0; i-- ){
    out += "," + iter_N( iters.size() -1 );
    for ( int j= iters.size() -2; j >= i; j-- )
      out += "*"+iter_N( j );
  }

  out += "); CUDA_LAST_ERROR(); ";
  return out;
}

string launch_kernel::define_cpu(char part)
{ //part: 's'(second part) 'f'(first part)
  int size = iters.size();
  string out;
  if ( part == 'f' ){
    out = declare_cpu() + " {\n#pragma omp parallel for num_threads( EvaSettings::threadNumberPerKernel )";
    return out;
  }
  out = string(declare_cpu().size(),' ') + "  ";
  for ( int i=0;i<size; i++ ){
    out += "for (int " + iter_i(i) + " = 0; "+ iter_i(i) + " < "+ iter_Ni(i) + "; "+ iter_i(i) + "++ )";
  }
  out += "\n"+(*content).substr( pragma->self->end.pos, content_start_pos.pos - pragma->self->end.pos );
  out += (*content).substr( content_start_pos.pos, content_end_pos.pos - content_start_pos.pos )+"}";
  return out;
}

string launch_kernel::define_gpu(){
  //int size = iters.size();
  string out = declare_gpu() + " {";
  string N_strs[9];
  N_strs[ iters.size() ] = "";
  for ( int i = iters.size()-1; i>=0; i-- ){
    N_strs[i] = iter_i(i)+N_strs[i+1];
  }
  out += "int _tid = blockIdx.x*blockDim.x + threadIdx.x; ";
  string temp_var = iter_i( iters.size()-1 );
  if ( iters.size() > 2 ){
    out += "int "+temp_var+";";
  }
  out += "while ( _tid < _N"+N_strs[0]+" ){";
  string iterI;
  if ( iters.size() == 1 ){
    out += "const int "+iter_i(0) +" = _tid;";
  }else if ( iters.size() == 2 ){
    out += "const int "+iter_i(0) + " = _tid/" + iter_Ni(1) + ";";
    out += "const int "+iter_i(1) + " = _tid - " + iter_i(0) + "*" + iter_Ni(1) + ";";
  }else if ( iters.size() >= 3 ){
    out += "const int "+iter_i(0) + " = ("+temp_var+" = _tid)/_N"+N_strs[1] + ";"; 
    for ( int i= 1; i < (int)iters.size() -1 ; i++ ){
      out += "const int "+iter_i(i) + " = ("+temp_var+" -= "+ iter_i(i-1) +"*_N"+ N_strs[i]  +")/_N"+N_strs[i+1] + ";"; 
    }
    out += iter_i( iters.size() -1 )+" = ("+temp_var+"-"+ iter_i(iters.size()-2)+"*_N"+N_strs[iters.size()-1] +");"; 
  }
  //append content
  out += "\n"+(*content).substr( pragma->self->end.pos, content_start_pos.pos - pragma->self->end.pos );
  out += (*content).substr( content_start_pos.pos, content_end_pos.pos - content_start_pos.pos );
  out += " _tid += blockDim.x* gridDim.x;}}";
  return out;
}
