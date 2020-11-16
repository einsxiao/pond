#include"pcc.h"


string                  *NodeIndex::content  = NULL;
vector< pair<int,int> > *NodeIndex::lineInfo = NULL;
string                  *Node::content       = NULL;
vector< pair<int,int> > *Node::lineInfo      = NULL;

NodeIndex &NodeIndex::incr(u_int n){
  for ( u_int i=0;i<n; i++ ){
    if ( (*content)[pos] == '\n' ){
      if ( (int)lineInfo->size() == row ){
        lineInfo->push_back( pair<int,int>(row,column+1) );
      }
      row++;
      column = 0;
    }else{
      column ++;
    }
    pos++;
  }
  return *this;
};

NodeIndex &NodeIndex::decr(u_int n)
{
  for ( u_int i=0;i<n; i++ ){
    pos--;
    if ( (*content)[pos] == '\n' ){
      row --;
      column = (*lineInfo)[row].second;
      //lineInfo->pop_back();
    }else{
      column --;
    }
  }
  return *this;
};

NodeIndex operator-(NodeIndex&ind,int num){
  NodeIndex tmp = ind;
  for ( int i=0;i<num;i++)
    tmp.decr();
  return tmp;
};

NodeIndex operator+(NodeIndex&ind,int num){
  NodeIndex tmp = ind;
  for ( int i=0;i<num;i++)
    tmp.incr();
  return tmp;
};

void Node::output()
{
  dout<<string(2*(level+1),' ');
  if ( eles.size() == 0 ){
    if ( type== "word" ){
      dout<<getContent()<<endl;
    }else if ( type== "symbol"){
      dout<<"[@ "<<getContent()<<"]"<<endl;
    }else if ( type == "newline"){
      dout<<"["<<type <<"]"<<endl;
    }else{
      dout<<"["<<type <<": "<<getContent()<<"]"<<endl;
    }
  }else{
    dout<<"["<<type;
    dout<<":"<<endl;
    for ( u_int i=0; i<eles.size(); i++ )
      eles[i]->output();
    dout<<string(2*(level+1),' ');
    dout<<"]"<<endl;
  }
}


Syntax::Syntax()
{
}

Syntax::~Syntax()
{

}

void Syntax::init(CmdLine*cmdLine){
  if ( cmdLine->files.size() == 0 ){
    ThrowError("there is no source file to process.");
  }
  if ( cmdLine->files.size() > 1 ){
    ThrowError("no more than one file could be processed simutaliously.");
  }
  if ( not FileExistQ( cmdLine->filePath ) ){
    ThrowError("can not find source file '"+cmdLine->filePath+"'");
  }
  fileType = cmdLine->fileType;
  filePath = cmdLine->filePath;
  tmpFilePath = cmdLine->tmpFilePath;
  fileContent = GetFileContent( filePath )+"         \n          ";
  moduleName  = cmdLine->moduleName;
  conLen = fileContent.size()-17;
}

bool Syntax::rollBack(const int n ){
  idx.decr(n);
  return true;
}

bool Syntax::rollForward(const int n ){
  idx.incr(n);
  return true;
}

string Syntax::getStr(const int n ){
  if ( n <= 0 )
    return "";
  if ( idx.pos + n > conLen )
    return fileContent.substr(idx.pos, conLen - idx.pos);
  else
    return fileContent.substr(idx.pos,n);
}

char Syntax::getCharAt(int p=1){
  return fileContent[ idx.pos + p - 1 ];
}

bool Syntax::getChar(char&ch){
  if ( idx.pos >= conLen ){
    return false;
  }
  ch = fileContent[ idx.pos ];
  idx.incr();
  return true;
}

bool Syntax::getRecentNumberString(string&str,char ch){
  while ( ch >= '0' && ch <='9' ){
    str.append(1,ch);
    if ( !getChar(ch) )
      return true;
  }
  if ( ch == '.' ){
    str.append(1,ch);
    getChar(ch);
    while ( ch >= '0' && ch <='9' ){
      str.append(1,ch);
      if (!getChar(ch))
        return true;;
    }
    //dout<<"met . and get str "<<str<<" and to proceed to str processing"<<endl;
  }
  if ( ch == 'E' or ch == 'e' ){
    str.append(1,'E');
    getChar(ch);
    if ( ch == '-' or ch == '+' ){
      str.append(1,ch);
      getChar(ch);
    }
    while ( ch >= '0' && ch <='9' ){
      str.append(1,ch);
      if ( !getChar(ch) )
        return true;
    }
  }
  rollBack(1);
  return true;
}

bool Syntax::getWrod(string&type,string&content){

  return true;
}


// #define INFOEle(ele,pos) dout<<string(3*level,' ')<<"get "<<#ele<<" at "<<(pos).ToString()<<endl
// #define INFOEleStart(ele,pos) dout<<string(3*level,' ')<<#ele<<" start at "<<(pos).ToString()<<endl
// #define INFOEleEnd(ele,pos) dout<<string(3*level,' ')<<#ele<<" end at "<<(pos).ToString()<<endl

#define INFOEle(ele,pos) 
#define INFOEleStart(ele,pos) 
#define INFOEleEnd(ele,pos) 



int Syntax::processing(Node&parent,int level,bool endReturn = false,bool colonReturn = false)
{
  //dout<<"get into processing level="<< level <<endl;
  vector<Node*> &nodes = parent.eles;
  char ch;
  while ( getChar(ch) ){
    switch ( ch ){
    case ' ': case '\t': {
      continue;
    }
    case '\r': case '\n':{
      if ( getStr(1) == "\n" )
        rollForward(1);
      if ( endReturn ){ //such as #include #pragma
        //dout<<"end return met, quit current level"<<endl;
        rollBack(1);
        return 0;
      }
      if (  ( nodes.size() == 0 ) or (nodes.size() > 0  and nodes.back()->type == "newline") ){
        continue;
      }
      nodes.push_back( new Node("newline",idx-1,idx,level) );
      INFOEle(newline,idx-1);
      continue;
    }
    case '(':{ //bracket
      nodes.push_back( new Node("bracket",idx-1,level) );
      processing( *nodes.back(), level+1, false, false);
      //dout<<"( processing returned with idx "<< idx.ToString() <<endl;
      nodes.back()->Set(idx);

      //dout<<"test if module specific rewrite exists"<<endl;
      if ( moduleName != "" ){

        //dout<<"check if pd function" <<endl;
        const auto nlen =  nodes.size();

        // PD function rewrite records
        if ( nlen >=7 && nodes.back()->eles.size() == 3 &&

             nodes[ nlen-2 ]->type == "word" &&
             nodes[ nlen-2 ]->getContent().substr(0,3) == "PD_" &&

             nodes[ nlen-3 ]->type == "colon" &&
             nodes[ nlen-4 ]->type == "colon" &&

             nodes[ nlen-5 ]->EqualQ( "word", (moduleName+"Module") ) &&

             nodes[ nlen-6 ]->EqualQ( "word", "int" ) &&

             nodes[ nlen-7 ]->type == "newline" 
        ){
          //dout<<"get pd function " << nodes[ nlen-2 ]->getContent() << endl;
          pd_functions.push_back( Syntax::__node_rec( this->tree.eles.back(),
                                                      &parent,
                                                      this->tree.eles.size()-1,
                                                      parent.eles.size()-1,
                                                      nodes.back() ) );
          //dout<<"pd function stack to "<< pd_functions.size() <<endl;

        }
        //dout<<"check if pd module constructor " <<endl;
        if ( nlen >= 6 && nodes.back()->eles.size() == 0 &&

             nodes[ nlen-2 ]->EqualQ( "word", (moduleName+"Module") ) &&

             nodes[ nlen-3 ]->type == "colon" &&
             nodes[ nlen-4 ]->type == "colon" &&

             nodes[ nlen-5 ]->EqualQ( "word", (moduleName+"Module") )&&

             nodes[ nlen-6 ]->type == "newline" 
        ){
          //dout<<"get constructor for module " << moduleName << endl;
          pd_constructor = Syntax::__node_rec( this->tree.eles.back(),
                                               &parent,
                                               this->tree.eles.size()-1,
                                               parent.eles.size()-1,
                                               nodes.back() );

        }

        //dout<<"check if pd module deconstructor " <<endl;
        if ( nlen >= 7 && nodes.back()->eles.size() == 0 &&
             nodes[ nlen-2 ]->EqualQ( "word", (moduleName+"Module") ) &&

             nodes[ nlen-3 ]->type == "symbol" &&
             nodes[ nlen-3 ]->getContent() == "~" &&

             nodes[ nlen-4 ]->type == "colon" &&
             nodes[ nlen-5 ]->type == "colon" &&

             nodes[ nlen-6 ]->EqualQ( "word", (moduleName+"Module") ) &&

             nodes[ nlen-7 ]->type == "newline" 
        ){
          //dout<<"get deconstructor for module " << moduleName << endl;
          pd_deconstructor = Syntax::__node_rec( this->tree.eles.back(),
                                                 &parent,
                                                 this->tree.eles.size()-1,
                                                 parent.eles.size()-1,
                                                 nodes.back() );
        }

      }
      continue;
    }
    case ')':{
      if ( parent.type != "bracket" ){
        throwCCError(" ')' is not expected here. Please check.");
      }
      //dout<<" a ) met, quit bracket"<<endl;
      //parent.output();
      return 0;
    }
    case '{':{ //block
      nodes.push_back( new Node("block",idx-1,level) );
      processing( *nodes.back(),level+1,false,false);
      nodes.back()->Set(idx);
      continue;
    }
    case '}':{
      if ( parent.type != "block" )
        throwCCError(" '}' is not expected here. Please check.");
      //dout<<" a } met, quit block"<<endl;
      return 0;
    }
    case '<':{ //<<<
      if ( getStr(2) != "<<" ) goto __default_goto_label;
      nodes.push_back( new Node("<<<>>>",idx-1,level) );
      rollForward(2);
      processing( *nodes.back(),level+1,false,false);
      nodes.back()->Set(idx);
      continue;
    }
    case '>':{
      if ( getStr(2) != ">>" ) goto __default_goto_label;
      rollForward(2);
      //dout<<" a > met, quit <<<>>>"<<endl;
      return 0;
    }
    case '[':{ // squarebracket
      nodes.push_back( new Node("squarebracket",idx-1,level) );
      processing( *nodes.back(),level+1,false,false);
      nodes.back()->Set(idx);
      continue;
    }
    case ']':{
      if ( parent.type != "squarebracket" )
        throwCCError(" ']' is not expected here. Please check.");
      //dout<<" a ] met, quit squarebracket" <<endl;
      return 0;
    }
    case '\\':{
      if ( getCharAt(1) == '\n' ){
        continue;
      }else{
        throwCCError("An lonely '\\' was not supposed to be here. Revise and try again please.");
      }
    }
    case '#':{
      if ( inPragma ){
        nodes.push_back( new Node("sharp",idx-1,idx,level) );
        continue;
      }
      inPragma = true;
      nodes.push_back( new Node("pragma",idx-1,level) );
      processing( *nodes.back(), level+1, true ,false);
      nodes.back()->Set(idx);  //INFOEleEnd(pragma,idx);

      inPragma = false;
      if ( nodes.back()->eles.size() > 2  and
           nodes.back()->eles[0]->EqualQ("word","pragma") and
           nodes.back()->eles[1]->EqualQ("word","launch_kernel")
      ){
        pragmas.push_back( Syntax::__node_rec( this->tree.eles.back(),
                                               &parent,
                                               this->tree.eles.size()-1,
                                               parent.eles.size()-1,
                                               nodes.back() ) );
      }
      continue;
    }
    case ';':{
      if ( colonReturn ){
        //dout<<" a ; met, quit colonReturn block" <<endl;
        return 0;
      }
      nodes.push_back( new Node("semicolon",idx-1,idx,level) );
      INFOEle(semicolon,idx-1);
      continue;
    }
    case ':':{
      nodes.push_back( new Node("colon",idx-1,idx,level) );// INFOEle(semicolon,idx-1);
      continue;
    }
    case ',':{
      nodes.push_back( new Node("comma",idx-1,idx,level) );
      continue;
    }
    case '\'':{
      nodes.push_back( new Node("astring",idx-1,level ) );
      while ( getChar(ch) ){
        if ( ch == '\\' )
          rollForward(1);
        if ( ch == '\'' )
          break;
      }
      nodes.back()->Set(idx);
      continue;
    }
    case '"':{
      nodes.push_back( new Node("string",idx-1,level) );
      while ( getChar(ch) ){
        if ( ch == '\\' )
          rollForward(1);
        if ( ch == '\"' )
          break;
      }
      nodes.back()->Set(idx);
      continue;
    }
    case '/':{ //let comment out directly
      if ( getStr(1) == "/" ){
        nodes.push_back( new Node("comment",idx-1,level) );
        //int pstart = (idx-1).pos;
        while ( getChar(ch)  ){
          if ( ch == '\n' and getCharAt(-1) != '\\' )
            break;
        }
        rollBack(1);
        nodes.back()->Set(idx);

        // int pend = (idx).pos;
        // for ( int i= pstart; i<  pend; i++ )
        //   fileContent[i] = ' ';
        continue;
      }else if ( getStr(1) == "*" ){
        nodes.push_back( new Node("comment",idx-1,level) );
        //int pstart = (idx-1).pos;
        rollForward(1);
        while ( getChar(ch) ){
          if ( ch == '*' and getCharAt(1) == '/' ){
            rollForward(1);
            break;
          }
        }
        nodes.back()->Set(idx);
        // int pend = (idx).pos;
        // for ( int i= pstart; i<  pend; i++ ){
        //   if ( fileContent[i] != '\n')
        //     fileContent[i] = ' ';
        // }
        continue;
      }
      goto __default_goto_label;
    }
    case '0': case '2':  case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
      nodes.push_back( new Node("number",idx-1,level) );
      string word;
      getRecentNumberString(word,ch);
      nodes.back()->Set(idx);
      continue;
    }
    default:{
      __default_goto_label:
      if ( ('a'<=ch and ch<='z') or ('A'<=ch and ch<='Z') or ch == '_' or ch == '$' ){
        nodes.push_back( new Node("word",idx-1,level) );
        string word; word.append(1,ch);
        getChar(ch);
        while ( ('a'<=ch and ch<='z') or ('A'<=ch and ch<='Z') or ch == '_' or ch == '$' or ( '0'<=ch and ch<='9' ) ){
          word.append(1,ch);
          getChar(ch);
        }
        rollBack(1);
        nodes.back()->Set( idx );
        continue;
      }
      nodes.push_back( new Node("symbol",idx-1,idx,level) );
      continue;
    }
    }
  }
  //dout<<" just return" <<endl;
  return 0;
}

int Syntax::erase_source(NodeIndex&start_pos,NodeIndex&end_pos){
  if ( start_pos.row == end_pos.row ){ // if only one row
    for ( auto iter = old_2_new[start_pos.row].begin(); iter != old_2_new[start_pos.row].end(); iter ++ ){
      if ( start_pos.column < source[*iter].end_pos and end_pos.column > source[*iter].start_pos ){
        for ( int i= max(source[*iter].start_pos,start_pos.column) ;
              i< min(source[*iter].end_pos, end_pos.column ) ;
              i++
        ){
          source[*iter].content[ i - source[*iter].start_pos ] = ' ';
        }
      }
    }
  }else{//more than one row
    //first lines
    for ( auto iter = old_2_new[start_pos.row].begin(); iter != old_2_new[start_pos.row].end(); iter ++ ){
      if ( start_pos.column < source[*iter].end_pos ){
        for ( int p= max(source[*iter].start_pos,start_pos.column) ; p< source[*iter].end_pos ; p++ ){
          source[*iter].content[ p - source[*iter].start_pos ] = ' ';
        }
      }
    }
    //middle lines
    for ( int i = start_pos.row+1; i< end_pos.row; i++ ){
      for (auto iter = old_2_new[i].begin(); iter != old_2_new[i].end(); iter ++ ){
        for ( int p= source[*iter].start_pos ; p< source[*iter].end_pos ; p++ ){
          source[*iter].content[ p - source[*iter].start_pos ] = ' ';
        }
      }
    }
    //last line
    for ( auto iter = old_2_new[end_pos.row].begin(); iter != old_2_new[end_pos.row].end(); iter ++ ){
      if ( end_pos.column > source[*iter].start_pos ){
        for ( int p= source[*iter].start_pos; p< min(source[*iter].end_pos, end_pos.column ); p++ ){
          source[*iter].content[ p - source[*iter].start_pos ] = ' ';
        }
      }
    }
  }

  return 0;
}

//update the source constructed by line information
int Syntax::prepare_insert(NodeIndex&insert_pos,int insertedLinesN)
{
  vector<int>&rel = old_2_new[insert_pos.row];
  int source_pos = -1;
  u_int rel_pos = 0;
  //find position
  for (rel_pos = 0 ; rel_pos< rel.size(); rel_pos++ ){
    if ( source[ rel[ rel_pos] ].start_pos <= insert_pos.column and
         insert_pos.column <= source[ rel[rel_pos] ].end_pos ){
      source_pos = rel[ rel_pos ];
      break;
    }
  }
  if ( source_pos == -1 )
    throwCCError("internal error while processing launch_kernel directive '#pragma launch_kernel'.");
  //sure where and how to
  if ( insert_pos.column == source[source_pos].end_pos ){
    source_pos ++;
    //rel_pos ++;
  }else if ( insert_pos.column > source[source_pos].start_pos ){//break the line into two
    source.insert( source.begin()+source_pos +1,
                   __line_info(
                     true,
                     source[source_pos].row,
                     insert_pos.column,
                     source[source_pos].end_pos,
                     source[source_pos].content.substr(
                       insert_pos.column,
                       source[source_pos].end_pos - insert_pos.column)
                   )
    );
    source[source_pos].content = source[source_pos].content.substr(
      source[source_pos].start_pos,
      insert_pos.column - source[source_pos].start_pos
    );
    source[source_pos].end_pos = insert_pos.column;
    insertedLinesN++;
    rel.insert( rel.begin()+rel_pos+1, source_pos);
    source_pos ++;
  }else{
    // if insert directly at the begining then this lines old_2_new should also be updated 
    rel_pos --; 
  }
  source[source_pos].revised = true;
  //update old_2_new info
  // first the insert rows
  for (u_int p = rel_pos+1; p < rel.size(); p++ ){
    rel[p] += insertedLinesN;
  }
  for ( u_int p = insert_pos.row + 1; p< old_2_new.size(); p++ ){
    for (auto iter = old_2_new[p].begin(); iter != old_2_new[p].end(); iter++ ){
      (*iter) += insertedLinesN;
    }
  }
  // then following rows
  return source_pos;
}

#include"syntax_launch_kernel.h"
void Syntax::deal_launch_kernel(__node_rec&pragma,int serialN){
  launch_kernel lan_ker;
  lan_ker.pragma           = &pragma;
  lan_ker.serial_number    = serialN + 1;
  lan_ker.filePath         = filePath;
  lan_ker.tree             = &tree;
  lan_ker.content          = &fileContent;
  lan_ker.Init();
  //insert 
  //cout<<"find the source postion and see what need to do, prepare insert  "<<endl;

  //declare
  //cout<<"declare parts"<<endl;
  int source_insert_pos;
  //cout<<"find insert pos"<<endl;
  if ( fileType == "cu" )
    source_insert_pos = prepare_insert( lan_ker.declare_insert_pos, 2 );
  else
    source_insert_pos = prepare_insert( lan_ker.declare_insert_pos, 1 );

  //cout<<"do insert pos with insert parts "<< source_insert_pos<<endl;
  if ( fileType == "cu" )
    source.insert( source.begin() + source_insert_pos,
                   __line_info(true, pragma.self->start.row, 0, 0, lan_ker.declare_gpu()+";" ) );
  source.insert( source.begin() + source_insert_pos,
                 __line_info(true, pragma.self->start.row, 0, 0, lan_ker.declare_cpu()+";" ) );

  //calling
  //cout<<"calling parts"<<endl;
  if ( fileType == "cu" )
    source_insert_pos = prepare_insert( lan_ker.pragma->self->end, 3 );
  else
    source_insert_pos = prepare_insert( lan_ker.pragma->self->end, 1 );
  source[source_insert_pos-1].deleted = true; // delete the origin line
  source.insert( source.begin() + source_insert_pos,
                 __line_info(true, pragma.self->start.row, 0, 0,
                             lan_ker.call_cpu() ));
  if ( fileType == "cu" ){
    source.insert( source.begin() + source_insert_pos,
                   __line_info(true, pragma.self->start.row, 0, 0,
                               lan_ker.call_gpu() +"\n}else" ));
    source.insert( source.begin() + source_insert_pos,
                   __line_info(true, pragma.self->start.row, 0, 0,
                               "if ( pond::EvaSettings::MatrixPosition() == pond::MatrixDevice ){\n") );
  }
  //define
  if ( fileType == "cu" )
    source_insert_pos = prepare_insert( lan_ker.define_insert_pos, 3 );
  else
    source_insert_pos = prepare_insert( lan_ker.define_insert_pos, 2 );
  if ( fileType == "cu" )
    source.insert( source.begin() + source_insert_pos,
                   __line_info(true, pragma.self->start.row, 0, 0,
                               lan_ker.define_gpu() ));
  source.insert( source.begin() + source_insert_pos,
                 __line_info(true, pragma.self->start.row, 0, 0,
                             lan_ker.define_cpu('s') ));
  source.insert( source.begin() + source_insert_pos,
                 __line_info(true, pragma.self->start.row, 0, 0,
                             lan_ker.define_cpu('f') ));
  erase_source( lan_ker.content_start_pos, lan_ker.content_end_pos );
  
}

void Syntax::deal_pd_module(){
  //cout<<"deal constructor"<<endl;
  if ( pd_constructor.parent ){
    //cout<<"constructor existed"<<endl;
    // auto moduleName = pd_constructor.self->getContent();
    //cout<<endl<<endl;
    auto node = pd_constructor.parent->eles[ pd_constructor.pos - 4 ];
    // string moduleName = pd_constructor.parent;
    auto insert_pos = prepare_insert( node->start ,1);
    //cout<<"insert_pos = "<<insert_pos<<endl;
    source.insert( source.begin() + insert_pos,
                   __line_info( true, node->start.row, 0, 0, "__REGISTER_POND_MODULE__")
                   );
    source[ insert_pos+1 ].revised = true; // delete the origin line
  }

}

void Syntax::deal_pd_functions(){

  //cout<<"deal pd functions with total records:"<<pd_functions.size()<<endl;
  for (u_int i = 0; i < pd_functions.size(); i++){
    __node_rec &rec = pd_functions[i];
    auto func_name = rec.parent->eles[ rec.pos - 1 ]->getContent();
    func_name = func_name.substr(3,func_name.size()-3);
    //cout<<"deal func: "<< func_name<<endl;

    auto argv_var = rec.self->eles[2]->getContent();

    int pos = rec.pos+1;
    while ( rec.parent->eles[ pos ]->type != "block" && pos < (int)rec.parent->eles.size() ){
      pos ++;
    }
    if ( rec.parent->eles[ pos ] ->type == "block" ){

      // macro function register
      auto node = rec.parent->eles[ rec.pos - 5 ];
      auto insert_pos = prepare_insert( node->start ,1);
      // get block starting comment as functio help string
      auto block_node = rec.parent->eles[ pos ];
      string comment = "";
      for (unsigned i=0; i < block_node->eles.size(); i++){
        auto cnode = block_node->eles[i];
        if ( cnode->type == "comment" ){
          string str = cnode->getContent();
          if ( str.substr(0,2) == "//" ){
            comment += str.substr(2, str.size() -2 )+"\n";
          } else if ( str.size() > 4 )  {
            comment += str.substr(2, str.size() -4 )+"\n";
          }
        } else  break;
      }

      size_t nlp;
# define replace_pair(ostr, nstr, oN, nN)           \
      nlp = 0;                                      \
      while( true ){                                \
        nlp = comment.find(ostr,nlp);               \
        if ( nlp == string::npos ) break;           \
        comment = comment.replace( nlp, oN, nstr);  \
        nlp += nN;                                  \
      }
      replace_pair("\n","\\n",1,2);
      replace_pair("\"","\\\"",1,2);

      source.insert( source.begin() + insert_pos,
                     __line_info( true, node->start.row, 0, 0,
                                  "__REGISTER_POND_FUNCTION__(" + func_name+",\""+comment+"\")"
                                  )
                     );
      source[ insert_pos+1 ].revised = true; // delete the origin line

      if ( argv_var != "ARGV" ){
        // Argv define
        node = rec.parent->eles[ pos ];
        auto start_pos_p1 = node->start+1;// start_pos_p1.column += 1;
        insert_pos = prepare_insert( start_pos_p1, 1);
        source.insert( source.begin()+insert_pos,
                       __line_info( true, start_pos_p1.row, 0, 0 ,
                                    "#define ARGV "+argv_var ) );
        // Argv undefine
        auto end_pos_m1 = node->end-1;
        insert_pos = prepare_insert( end_pos_m1, 1);
        source.insert( source.begin()+insert_pos,
                       __line_info( true, end_pos_m1.row, 0, 0 ,
                                    "#undef ARGV" ) );
      }
    }
  }

}


void Syntax::rewrite(){
  // rewrite all rewrite rules

  // pondd launch_kernel pragma
  dout<<"try rewrite"<<endl;
  for (u_int i = 0; i < pragmas.size(); i++){
    __node_rec &para  = pragmas[i];
    deal_launch_kernel( para, i);
  }

  // pd module constructor deconstructor
  deal_pd_module();

  // pd_functions
  deal_pd_functions();

}

void Syntax::phrasing(){
  NodeIndex::content     = &fileContent;
  NodeIndex::lineInfo    = &lineInfo;
  Node::content          = &fileContent;
  Node::lineInfo         = &lineInfo;
  idx = NodeIndex(0,0,0);
  //lineInfo.push_back( pair<int,int>(0,0) );
  processing(tree,0);
  dout<<"source content processing done. parsed content is:"<<endl;
  tree.output();

  //construct the newLines and old_2_new
  int ch_count = 0;
  for (u_int i=0;i < lineInfo.size(); i++){
    old_2_new.push_back( vector<int>() );
    old_2_new.back().push_back( i );
    auto line = lineInfo[i];
    source.push_back( __line_info(false,
                                  i,
                                  0, line.second,
                                  fileContent.substr(ch_count,line.second) ) );
    ch_count += line.second;
  }
  outContent = "";
  rewrite();
}

void Syntax::output(){
  outContent = "#1 \""+filePath+"\" 1\n";
  if ( moduleName != "" ){
    outContent += "#define __MODULE_NAME__ "+moduleName+"\n#line 1\n";
  }

  for (auto line:source){
    if ( line.deleted ) continue;
    if ( line.revised ){
      //outContent += "#"+ToString( line.row +1 )+" \""+filePath+"\" 1\n";
      outContent += "#line "+ToString( line.row +1 )+"\n";
      outContent += string(line.start_pos,' ')+line.content;
    }else{
      outContent += string(line.start_pos,' ')+line.content;
    }
    if ( (*line.content.rbegin())!='\n' )
      outContent += "\n";
  }
  //cout<<"put content to tmp file:"<<tmpFilePath<<endl;
  SetFileContent(tmpFilePath,outContent);
  System((string)"touch -r " + filePath  +" "+ tmpFilePath );
  dout<<"output:"<<endl<< outContent<<endl;
}


inline void index_trans(){
  return;
}
