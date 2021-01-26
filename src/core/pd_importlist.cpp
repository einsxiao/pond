#include"pd_headers.h"

using namespace std;
using namespace pond;

ImportList::ImportList(istream *istr_in,bool interactively){
  buffer = "";
  pos = 0;
  istr = istr_in;
  this->interactively = interactively;
  inputEnd = false;
}
ImportList::ImportList(string buffer){
  this->buffer = buffer; 
  pos=0;
  istr = NULL;
  interactively = false;
  inputEnd = false;
}

ImportList::~ImportList(){};

bool ImportList::End(){
  return inputEnd;
}

bool ImportList::getChar( char &ch){
  // read in a char, and move the position pointer forward
  // the next char which should be get.
  // At the very beginning, position 0 has no element and need to get from istr;
  if ( pos >= (int)buffer.length() ){
    if ( istr && istr->get(ch) ) {
      buffer.append(1,ch);
      pos++;
      return true;
    } else {
      inputEnd = true;
      ch = '\0';
      return false; 
    }
  }
  ch = buffer[pos];
  pos++;
  return true;
}

bool ImportList::getChar( char &ch, int n ){// will not change the position pointer
  if ( pos + n -1 >= signed(buffer.length()) ){
    if ( istr && istr->get(ch) ) {
      buffer.append(1,ch);
      return true;
    }else{
      ch = '\0';
      return false;
    }
  }
  ch = buffer[ pos + n - 1];
  return true;
}

bool ImportList::getRecentNumberString(string &str,char ch){
  //no dot dot may contain other meanings ( need seperate treatment )
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
  if ( ch != '`' ) rollBack(1);
  return true;
}

char ImportList::getRecentNonSpace(){// will not change the position pointer
  char ch = ' ';
  int p=pos;
  while ( p < signed(buffer.length()) ){
    ch = buffer[p];
    if (ch != ' ' and ch!= '\t' ) return ch; 
    p++;
  }
  while ( istr && istr->get(ch) ) {
    buffer.append(1,ch);
    if ( ch == ' ')
      continue; 
    return ch;
  }
  return ch;
}

bool isWordCh(char ch){
  if ( not pond::SpecialCharecterQ(ch) )
    return true;
  return false;
}
// string ImportList::getRecentOperWord(){// will not change the position pointer
//   string res;
//   char ch;
//   int p=pos;
//   //dout<<"skip space first time"<<endl;
//   while ( p< (int)buffer.length() ){ // skip spaces
//     ch = buffer[p];  p++;
//     if ( ch == ' ' or ch == '\t' or ch == '\n' or ch == '\r' )
//       continue;
//     if ( pond::SpecialCharecterQ(ch) or (ch >='0' and ch <='9') ){
//       return res;
//     }
//     res.append(1,ch); 
//     //dout<< " res = " <<res<<endl;
//     break;
//   }
//   //dout<<"append res to res ="<< res <<" in buffer "<<endl;
//   while ( p< (int)buffer.length() ){ // append oper word char in buffer
//     ch = buffer[p]; p++;
//     if ( not pond::SpecialCharecterQ(ch) ){
//       res.append(1,ch);
//       //dout<< " res = " <<res<<endl;
//     }else{
//       return res;
//     }
//   }

//   return res;
// }

bool ImportList::rollBack( int n){// rollBack the last n-th chars to buffer n>=0;
  if ( n > pos ) return false; 
  pos -= n;
  return true;
}


struct recordType{
  u_int  sym_id;
  u_char state, pL,pR, oT, oN;
};
void recordTypeInit(int p,recordType&res,Object listStack){
  res.sym_id    = listStack[p].ids();
  res.state  = listStack[p].state();
  GlobalPool.Symbols.GetRecord( res.sym_id, res.pL, res.pR, res.oT, res.oN );
};

//if state != 0 the specail operator
static const u_char RSymbol   = 1;  // if else kind of , the next three actually are not nessesary
static const u_char RLeft     = 2;  // a++
static const u_char RRight    = 3;  // ! True
static const u_char RConnect  = 4;  // a+b
static const u_char RUnit     = 5;  // a b

//the first operator is Seperator Null for operatorPositions is ok.
#define CHECK_IS_OPERATOR(sym) 
#define OPERATOR_PUSH(op,con){                  \
    Object newop(__Symbol__,op);                \
    newop.set_state( con );                     \
    listStack.PushBackRef( newop );             \
  }
#define IS_LAST_OPERATOR  ( listStack.Back().state() != 0)
#define IS_LAST_STRING    ( listStack.Back().StringQ() )
#define IS_LAST_NUMBER    ( listStack.Back().NumberQ() )

// the return value "0" or "error code and detail
int ImportList::GetList( Object &parentList ,int depth, bool detectMode ){
  //depth indicates the depth of the expression
  /////////////////////////////////////
  static Object timesOp;
  if (timesOp.NullQ()){
    timesOp.SetSymbol(SYMID_OF_Times);
    timesOp.set_state(RConnect);
  }
  static Object compoundOp;
  if ( compoundOp.NullQ() ){
    compoundOp.SetSymbol(SYMID_OF_CompoundExpression);
    compoundOp.set_state(RConnect);
  }
  // if ( depth == 0 ) parens = " ";
  Object listStack(__List__,SYMID_OF_List);
  //EvaKernel->flag = true;
  //dout<<" try get list with parent= "<<parentList<<" and listStack="<<listStack<<","<<depth<<endl;
  //Object tmp = listStack;
  //dout<<" test tmp = "<<tmp<<" listStack = "<<listStack<<endl;
  //exit(0);
  char ch;
  bool expressionEnd;
  bool listEnd = false;
  string currentWord;
  while ( 1 ){
    if ( !getChar(ch) ){ //
      //dout<<"meet final end at depth "<<depth<<endl;
      if ( depth != 0 ){
        zhErroring("语法","表达式不完整") ||
          Erroring("Syntax","Incomplete expression.");
        if ( parens.size() > 1 ){
          zhErroring("语法",parens.back()+" 不匹配, 第 "+pond::ToString(lineNumber)+" 行") ||
            Erroring("Syntax",parens.back()+" is not matched at line "+pond::ToString(lineNumber));
        }
        return -1;
      }
      expressionEnd = true;
      listEnd = true;
    }else{
      //dout<<"get ch>"<<ch<<endl;
      currentWord="";
      expressionEnd = false;
      if ( ch == ' ' or ch == '\t' ){
        continue;
      }else{
        switch ( ch )
          {
            ///////////////////////// space
          case '\r': continue;
            // only when end with a normal list can be treated as an end to the whole list
            // but now we need to treat it as compond on none top level
          case '\n':{ 
            lineNumber++;
            //dout<<"comes to line "<<lineNumber<<" with depth = "<<depth<<endl;
            if ( listStack.Size() == 0 ){
              //dout<<"listStack is empty, break"<<endl;
              break;
            }
            recordType rec;
            recordTypeInit( listStack.Size(), rec, listStack );
            //dout<<"meet return with state, oN ="<<int(rec.state)<<int(rec.oN)<<endl;
            if ( listStack.Last().state() == RConnect or
                 listStack.Last().state() == RRight  or rec.oN > 0 ){
              //dout<<"break by connect by last"<<endl;
              break;
            }
            if ( listStack.Size() >= 2 ){
              recordTypeInit( listStack.Size()-1, rec, listStack );
              if ( rec.oN > 1 ){break;}
            }
            if ( listStack.Size() >= 3 ){
              recordTypeInit( listStack.Size()-2, rec, listStack );
              if ( rec.oN > 2 ){break;}
            }
            if ( listStack.Size() >= 4 ){
              recordTypeInit( listStack.Size()-3, rec, listStack );
              if ( rec.oN > 3 ){break;}
            }

            // if not on interactive mode, consider next word
            if ( not this->interactively ){
              // try get next element
              //dout<<" try get next element by GetList"<<endl;
              int oriP = pos;
              int oriL = lineNumber;
              bool oriE = inputEnd;
              string oriParen = parens;
              Object tlist(__List__,SYMID_OF_List);
              GetList(tlist,depth,true); // detect next element
              //dout<< " tlist get ="<< tlist<<endl;
              pos = oriP;
              lineNumber=oriL;
              inputEnd=oriE;
              parens = oriParen;
              if ( tlist.Size() > 0 and tlist[1].state() != 0 and
                   (
                     ( tlist[1].state() == RLeft ) or
                     ( tlist[1].state() == RConnect ) or
                     ( tlist[1].state() == RSymbol and GlobalPool.Symbols.GetOperType( tlist[1].ids() ) == 3)
                   ) ){
                break; // just ignore this line break
              }
            }
            // string nword = getRecentOperWord();
            // if ( nword.size() > 0 ){
            //   //dout<<"recnet nword is >>"<<nword<<"<< listStack = "<<listStack << endl;
            //   Object obj(ObjectType::Symbol, nword );
            //   auto oT  = GlobalPool.Symbols.GetOperType( obj.ids() );
            //   auto pL  = GlobalPool.Symbols.GetPriorityLeft( obj.ids() );
            //   //dout<< "oT = "<< (int)oT << " pL " << int(pL) <<endl;
            //   if ( (obj.state() > 0 and pL > 0) or oT == 3 ){
            //     //dout<<"should not end"<<endl;
            //     break;
            //   }
            // }
            //// an expression finished
            //dout<<"met newline and 2 cond, depth "<<depth<<endl;
            if ( depth == 0 ){
              //dout<<"an expression end met"<<endl;
              listEnd = true;
              expressionEnd = true;
              break;
            }else{ // not in the upmost text(superList level); just ignore it
              //OPERATOR_PUSH(SYMID_OF_LineBreak,RConnect);
              // only in parenthesis, will  line break been append
              if ( parentList.ListQ( SYMID_OF_ExpressionList ) )
                OPERATOR_PUSH( SYMID_OF_LineBreak, 0 );
              break;
            }
          }
            ///////////////////////// 
            ///////////////////////// " and ' can both enclose a String
          case '`':{
            while ( getChar(ch) and
                    (ch != '`' or
                     (ch == '`' and currentWord.size()>0 and *currentWord.rbegin() == '\\') ) )
              currentWord.append(1,ch);
            if ( ch != '`' ) {
              zhErroring( "语法","字符串不完整 位置:"+pond::ToString(lineNumber)+"行") ||
                Erroring("Syntax","String uncomplete at line "+pond::ToString(lineNumber) );
              return -1;
            }
            listStack.PushBackString( currentWord.c_str() );
            break;
          }
          case '\"':{
            char ch1,ch2;
            bool e1 = getChar(ch1,1);
            bool e2 = getChar(ch2,2);
            if ( e1 and ch1=='"' and e2  and ch2 == '"' ){ // triple quote string
              //dout<<"dealing long string"<<endl;
              rollBack(-2);
              for (;;){
                bool e0 = getChar(ch); e1 = getChar(ch1,1), e2 = getChar(ch2,2);
                //met buffer end but not string end
                if ( not e0 or not e1 or not e2 ) {
                  zhErroring( "语法","字符串不完整 位置:"+pond::ToString(lineNumber)+"行") ||
                    Erroring( "Syntax","String uncomplete at line "+pond::ToString(lineNumber) );
                  return -8;
                }
                //met string end
                if ( (ch == '\"'&&currentWord.size()>0&& *currentWord.rbegin() != '\\') and
                     (ch1 == '\"' ) and
                     (ch2 == '\"' ) ){ 
                  rollBack(-2);
                  break;
                }
                currentWord.append(1,ch);
              }
              //dout<<"long string:"<<currentWord<<endl;

              listStack.PushBackString( currentWord.c_str() );
              break;
            }
            // single quote string
            while ( getChar(ch) && (ch != '\"'|| (ch == '\"'&&currentWord.size()>0&& *currentWord.rbegin() == '\\') ) )
              currentWord.append(1,ch);
            if ( ch != '\"' ) {
              zhErroring( "语法","字符串不完整 位置:"+pond::ToString(lineNumber)+"行") ||
                Erroring("Syntax","String uncomplete at line "+pond::ToString(lineNumber) );
              return -1;
            }
            listStack.PushBackString( currentWord.c_str() );
            break;
          }
          case '\'':{
            char ch1,ch2;
            bool e1 = getChar(ch1,1);
            bool e2 = getChar(ch2,2);
            if ( e1 and ch1=='\'' and e2  and ch2 == '\'' ){ // triple quote string
              rollBack(-2);
              for (;;){
                bool e0 = getChar(ch); e1 = getChar(ch1,1), e2 = getChar(ch2,2);
                //met buffer end but not string end
                if ( not e0 or not e1 or not e2 ) {
                  zhErroring("语法","字符串不完整 位置:"+pond::ToString(lineNumber)+"行" ) ||
                    Erroring("语法","String uncomplete at line "+pond::ToString(lineNumber) );
                  return -1;
                }
                //met string end
                if ( (ch == '\''&&currentWord.size()>0&& *currentWord.rbegin() != '\\') and
                     (ch1 == '\'' ) and
                     (ch2 == '\'' ) ){ 
                  rollBack(-2);
                  break;
                }
                currentWord.append(1,ch);
              }

              listStack.PushBackString( currentWord.c_str() );
              break;
            }
            //single quote string
            while ( getChar(ch) && (ch != '\''|| (ch == '\''&&currentWord.size()>0&& *currentWord.rbegin() == '\\') ) )
              currentWord.append(1,ch);
            if ( ch != '\'' ) {
              zhErroring( "语法","字符串不完整 位置:"+pond::ToString(lineNumber)+"行") ||
                Erroring( "Syntax","String uncomplete at line "+pond::ToString(lineNumber) );
              return -1;
            }
            listStack.PushBackString(currentWord.c_str() );
            break;
          }
            ///////////////////////// numbers
          case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {

            getRecentNumberString(currentWord,ch);
            listStack.PushBackNumber(  ToNumber( currentWord.c_str() ) );
            break;
          }
            ///////////////////////// .
          case '.':{
            char ch1;
            bool e1 = getChar(ch1,1);
            if ( e1 && ch1>='0' && ch1<='9' ){
              currentWord = ".";
              getChar(ch);
              getRecentNumberString(currentWord,ch);
              listStack.PushBackNumber( ToNumber(currentWord.c_str() ) ) ;
              break;
            }else{ // Member of grammar, should not treated as a operator for [] is not treated as a Operator
              OPERATOR_PUSH(SYMID_OF_Conjunct,RConnect);
              break;
            }
          }
            ///////////////////////// 
            ///////////////////////// 
          case ',':{///////////////////////// , separate atoms or list ..( comma )
            OPERATOR_PUSH(SYMID_OF_Sequence, RConnect );
            break;
          }
          case ';': {/////////////////////////;
            if ( listStack.Empty() )
              break;
            OPERATOR_PUSH( SYMID_OF_CompoundExpression,RConnect );
            if ( getRecentNonSpace()=='\n' && depth == 0 ){
              listStack.PushBackNull();
              listEnd = true;
              expressionEnd = true;
              char ch1;
              while ( getChar(ch1) && ch1!='\n' );
            }else{
              char ch1; ch1 = getRecentNonSpace();
              if ( ch1 == ']' || ch1 == ')' || ch1 == '}' || ch1 ==',' ){
                listStack.PushBackNull();
              }
            }
            break;
          }
          case '[': { ///////////////////////// [ ] indicate a composite list
            // char ch1; bool e1=getChar(ch1,1);
            // if ( e1 && ch1 == '[' ){
            parens += "[";   
            if ( listStack.Empty() or 
                 IS_LAST_OPERATOR or
                 IS_LAST_NUMBER
            ) {
              Object listList;
              listList.SetList( SYMID_OF_List );
              if ( GetList( listList , depth +1)<0 ) return -1;
              //dout<<"get list after [  "<<listList<<endl;
              if ( listList.Size() == 1 and listList[1].SimpleListQ(SYMID_OF_Sequence ) ){
                listList = listList[1];
                listList[0].SetSymbol( SYMID_OF_List );
              }
              listStack.PushBackRef( listList );
              break;
            }
            //dout<<" [ as part oper "<<endl;
            // rollBack(-1);
            OPERATOR_PUSH( SYMID_OF_PartArgsFollow,RConnect );
            Object argsList; argsList.SetList( SYMID_OF_Part );
            //dout<< " try get child list "<<endl;
            if ( GetList( argsList, depth+1 ) < 0 ) return -1;
            //dout<<"get list after part [  "<<argsList<<endl;
            if ( argsList.Size() == 1 and argsList[1].SimpleListQ( SYMID_OF_Sequence ) ){
              argsList = argsList[1];
              argsList[0].SetSymbol( SYMID_OF_Part );
            }
            listStack.PushBackRef( argsList );
            //dout<< " child list get listStack = "<<listStack<<endl;
            break;
            // as a 
          }
          case '(': {
            parens += "(";
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 && ch1 == '*' ){
              rollBack(-1);
              if ( getChar(ch) ){
                while (  ch1 !='*' || ch != ')'  ){
                  currentWord.append(1,ch);
                  ch1=ch;
                  if (!getChar(ch) ) break;
                }
                if ( End() ){
                  zhWarning("语法","注释表达式并未结束 位置: 第 "+pond::ToString(lineNumber)+" 行" ) ||
                    Warning("Syntax","expression does not end at line [ "+pond::ToString(lineNumber)+" ]" );
                }
                break;
              }else{
                zhWarning("语法","注释表达式并未结束 位置: 第 "+pond::ToString(lineNumber)+" 行" ) ||
                  Warning("Syntax","expression does not end at line [ "+pond::ToString(lineNumber)+" ]" );
                break; 
              }
            }
            // need to push a head list at the beginning of the elements of this list
            // f (x) if one want to f*(x) then the * is nessesary
            if (listStack.Size() == 0 or
                IS_LAST_OPERATOR or
                IS_LAST_STRING or
                IS_LAST_NUMBER ) { // as normal ()
              Object list; list.SetList( SYMID_OF_Parenthesis );
              if ( GetList( list , depth +1) <0 ) return -1;
              //dout<<"get normal parenthesis after part (  "<<list << endl;
              if ( list.Size() == 1 and list[1].SimpleListQ( SYMID_OF_Sequence ) ){
                list = list[1];
                list[0].SetSymbol( SYMID_OF_Parenthesis );
              }
              //dout<<"after change list = "<<list <<endl;
              listStack.PushBackRef( list );
              break;
            }else{
              Object argsList;
              if ( listStack.Back().SymbolQ() ){
                argsList.SetList( listStack.Back().ids() );
              }else{ // is a list
                argsList.CopyObject( listStack.Back() );
                argsList.Deepen();
                //argsList.SetList( SYMID_OF_List );
              }
              //listStack.PopBack();
              if ( not ( listStack.Empty() ||
                         ( listStack.Last().state() == RConnect ||
                           listStack.Last().state() == RRight ))
              ){
                OPERATOR_PUSH( SYMID_OF_ArgsFollow,RConnect );
              }
              if ( GetList( argsList, depth+1 ) < 0 ) return -1;
              if ( argsList.Size() == 1 and argsList[1].SimpleListQ( SYMID_OF_Sequence ) ){
                argsList[1][0] = argsList[0];
                argsList = argsList[1];
              }
              listStack.PushBackRef( argsList );
            }
            break;
          }
          case ']': {
            if ( parens.back() != '[' ){
              zhErroring("语法",std::string("括号")+parens.back()+" 未匹配. 先遇到一个 ] 第 "+pond::ToString(lineNumber)+" 行") ||
                Erroring("Syntax",parens.back()+" not matched. A ] met first at line "+pond::ToString(lineNumber) );
              return -1;
            }
            parens.pop_back();
            expressionEnd = true;
            listEnd = true;
            break;
          }
          case ')': {// ']' can end ( { [ [[ and so on
            if ( parens.back() != '(' ){
              zhErroring("语法",std::string("括号")+parens.back()+" 未匹配. 先遇到一个 ) 第 "+pond::ToString(lineNumber)+" 行") ||
                Erroring("Syntax",parens.back()+" not matched. A ) met first at line "+pond::ToString(lineNumber) );
              return -1;
            }
            parens.pop_back();
            expressionEnd = true;
            listEnd = true;
            break;
          }
          case '{': {
            parens += "{";
            Object listList;
            listList.SetList( SYMID_OF_ExpressionList );
            if ( GetList( listList , depth +1)<0 ) return -1;
            if ( listList.Size() == 1 && listList[1].NullQ() ) listList.Delete(1);
            if ( listList.Size() == 1 and listList[1].SimpleListQ(SYMID_OF_Sequence ) ){
              listList = listList[1];
              listList[0].SetSymbol( SYMID_OF_ExpressionList );
            }
            // check child form and determine it to be Dict or ExpressionList
            //dout << " check listlist" << listList <<endl;
            if ( listList.Size() > 0 ){
              if ( ( listList[1].PairQ( SYMID_OF_KeyValuePair ) or
                     listList[1].ListQ( SYMID_OF_UnpackDict )
                   ) and ( not listList[1][1].ListQ( SYMID_OF_case ) ) ){
                // is a DictList , except for situation of switch case swtich () { case 1:
                Object dict(ObjectType::List, SYMID_OF_Dict );
                for ( int i=1; i<= listList.Size(); i++ ){
                  if ( listList[i].NullQ() ) continue;
                  if ( not listList[i].PairQ( SYMID_OF_KeyValuePair) and
                       not listList[i].ListQ( SYMID_OF_UnpackDict )
                  ){
                    zhErroring("语法","字典结构中不都是 键值对 位置: 第 "+pond::ToString(lineNumber)+" 行" ) ||
                      Erroring("Sytax","A dict should only contain key-value pair at line [ "+pond::ToString(lineNumber)+" ]" );
                  }
                  dict.DictInsertOrUpdatePairRef( listList[i] );
                }
                listStack.PushBackRef( dict );
                //dout << " check listlist finish with " << dict <<endl;
                break;
              }
            }else if ( listList.Size() == 0 ){
              listList[0].SetSymbol( SYMID_OF_Dict );
            }
            listStack.PushBackRef( listList );
            break;
          }
          case '}': {
            if ( parens.back() != '{' ){
              zhErroring("语法",std::string("括号")+parens.back()+" 未匹配. 先遇到一个 } 第 "+pond::ToString(lineNumber)+" 行") ||
                Erroring("Syntax",parens.back()+" not matched. A } met first at line "+pond::ToString(lineNumber) );
              return -1;
            }
            parens.pop_back();
            expressionEnd = true;
            listEnd = true;
            break;
          }
            ///////////////////////////////////
            ///////////////////////////////////
          case '~': {///////////////////////// blacks _ __ ___
            if ( listStack.Size() > 0 and listStack.Last().SymbolQ() and listStack.Last().state() == 0 )
              OPERATOR_PUSH( SYMID_OF_Pattern,RConnect );
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 && ch1=='~' ){ // a BlackSequence
              char ch2; bool e2=getChar(ch2,2);
              if ( e2 && ch2=='~' ){ // a BlackNullSequence
                char ch3;
                bool e3=getChar(ch3,3);
                if ( e3 && !pond::SpecialCharecterQ(ch3) ){
                  rollBack(-2);
                  OPERATOR_PUSH( SYMID_OF_BlackNullSequence,RRight );
                  break;
                }
                rollBack(-2);
                Object plist; plist.SetList( SYMID_OF_BlackNullSequence );
                listStack.PushBackRef(plist);
                break;
              }else if (e2 && !pond::SpecialCharecterQ(ch2) ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_BlackSequence,RRight );
                break;
              }
              rollBack(-1);
              Object plist; plist.SetList( SYMID_OF_BlackSequence );
              listStack.PushBackRef(plist);
              break;
            }else if ( e1 && !pond::SpecialCharecterQ(ch1) ){
              OPERATOR_PUSH( SYMID_OF_Black,RRight );
              break;
            }
            Object plist; plist.SetList( SYMID_OF_Black );
            listStack.PushBackRef(plist);
            break;
          }
          case '#':{ //comment out
            //dout<<"met # just comment out"<<endl;
            while ( getChar(ch) && ch != '\n' );
            //dout<<"get next none comment ch>"<<ch<<endl;
            if ( ch == '\n' ){
              //dout<<"is a newline"<<endl;
              rollBack(1);
            }
            //dout<<"continue dealing"<<endl;
            break;
          }
          case '@':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '@' ){
                OPERATOR_PUSH( SYMID_OF_Apply,RConnect );
                rollBack(-1);
                break;
              }
            }
            OPERATOR_PUSH( SYMID_OF_Prefix,RConnect );
            break;
          }
            /////////////////////////////////////////////////////
          case '=':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 && ch1=='=' ){
              rollBack(-1);
              OPERATOR_PUSH( SYMID_OF_Equal,RConnect );
              break;
            }
            if ( e1 && ch1 == '>' ){
              rollBack(-1);
              OPERATOR_PUSH( SYMID_OF_ArrowFunction, RConnect );
              break;
            }
            OPERATOR_PUSH( SYMID_OF_Set,RConnect );
            break;
          }
          case ':':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if ( ch1=='=' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_SetDelayed,RConnect );
                break;
              }
              if ( ch1 == '>' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_RuleDelayed,RConnect );
                break;
              }
              if ( ch1 == ':' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_Span,RConnect );
                break;
              }
            }
            if ( parentList.ListQ( SYMID_OF_Part ) ){
              OPERATOR_PUSH( SYMID_OF_Span,RConnect );
            }else{ //if ( parentList.ListQ( SYMID_OF_Dict) ){ become the default
              OPERATOR_PUSH( SYMID_OF_KeyValuePair, RConnect );
            }
            break;
          }
            /////////////////////////////////////////////////////
          case '+':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if ( ch1 == '+' ){
                if ( listStack.Empty() ){
                  OPERATOR_PUSH( SYMID_OF_PreIncrement,RRight );
                }else {
                  OPERATOR_PUSH( SYMID_OF_Increment,RLeft );
                }
                rollBack(-1);
                break;
              }
              if ( ch1 == '=' ){
                OPERATOR_PUSH( SYMID_OF_AddTo,RConnect );
                rollBack(-1);
                break;
              }

            } 
            // if + at the first place soly, just ignore it
            if ( listStack.Empty() or IS_LAST_OPERATOR ) break; 
            OPERATOR_PUSH( SYMID_OF_Plus,RConnect );
            break;
          }
          case '-':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '-' ){
                if ( listStack.Empty() ){
                  OPERATOR_PUSH( SYMID_OF_PreDecrement,RRight );
                } else {
                  OPERATOR_PUSH( SYMID_OF_Decrement,RLeft );
                }
                rollBack(-1);
                break;
              }
              if ( ch1 == '=' ){
                OPERATOR_PUSH( SYMID_OF_SubtractFrom,RConnect );
                rollBack(-1);
                break;
              }
              if ( ch1 == '>' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_Rule,RConnect );
                break;
              }
              if ( ch1 == '.' or ( ch1 >= '0' and ch1 <= '9' ) ){
                if (listStack.Empty() or
                    ( listStack.Last().state() == RConnect or
                      listStack.Last().state() == RRight ) ) {
                  currentWord = '-';
                  rollBack(-1);
                  getRecentNumberString( currentWord,ch1);
                  listStack.PushBackNumber( ToNumber( currentWord.c_str() ) );
                }else{
                  OPERATOR_PUSH( SYMID_OF_Plus,RConnect );
                  currentWord = '-';
                  rollBack(-1);
                  getRecentNumberString( currentWord,ch1);
                  listStack.PushBackNumber( ToNumber( currentWord.c_str() ) );
                }
                break;
              }
            }
            // check if is a number | negative number
            if ( listStack.Empty() or IS_LAST_OPERATOR){ // a minus at the beginning
              listStack.PushBackNumber(-1);
              OPERATOR_PUSH( SYMID_OF_Times,RConnect );
              break; // if + at the first place soly, just ignore it
            }else{
              OPERATOR_PUSH( SYMID_OF_Plus,RConnect );
              listStack.PushBackNumber( -1 );
              OPERATOR_PUSH( SYMID_OF_Times,RConnect );
            }
            break;
          }
          case '*':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '=' ){
                OPERATOR_PUSH( SYMID_OF_TimesBy, RConnect );
                rollBack(-1);
                break;
              }
              if ( ch1 == '*' ){
                rollBack(-1);
                if ( listStack.Size() == 0 or listStack.Last().state() != 0 ) {
                  OPERATOR_PUSH( SYMID_OF_UnpackDict, RRight );
                }else{
                  OPERATOR_PUSH( SYMID_OF_Power, RConnect );
                }
                break;
              }
            }
            if ( listStack.Size() == 0 ){
              OPERATOR_PUSH( SYMID_OF_Unpack, RRight );
            }else{
              OPERATOR_PUSH( SYMID_OF_Times, RConnect );
            }
            break;
          }
          case '/':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '=' ){
                OPERATOR_PUSH( SYMID_OF_DivideBy,RConnect );
                rollBack(-1);
                break;
              }
              if (ch1 == '.' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_ReplaceAll,RConnect );
                break;
              }
              if (ch1 == '/'){
                char ch2; bool e2 =getChar(ch2,2);
                if ( e2 && ch2 == '.' ){
                  rollBack(-2);
                  OPERATOR_PUSH( SYMID_OF_ReplaceAllRepeated,RConnect );
                  break;
                }
                OPERATOR_PUSH( SYMID_OF_Postfix,RConnect );
                rollBack(-1);
                break;
              }
              if (ch1 == '@'){
                OPERATOR_PUSH( SYMID_OF_Map,RConnect );
                rollBack(-1);
                break;
              }
            }
            OPERATOR_PUSH( SYMID_OF_Times,RConnect );
            OPERATOR_PUSH( SYMID_OF_Reciprocal,RRight );
            break;
          }
          case '\\':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if ( ch1 == '\n' ){
                zhWarning("语法","请使用()等方式来改变结合方式，而不是使用续行符号. 位置: 第 "+pond::ToString(lineNumber)+" 行" )||
                  Warning("ImportList::Syntax","\\ is is not recommended to use at line [ "+pond::ToString(lineNumber)+" ]" ) ;
                rollBack(-1);// just ignore the \ and \n to join two lines.
                break;
              }
            }
            zhWarning("语法","不必要的转义字符. 位置: 第 "+pond::ToString(lineNumber)+" 行" )||
              Warning("ImportList::Syntax","\\ is not nessesary to use alone at line [ "+pond::ToString(lineNumber)+" ]" ) ;
            break;
          }
          case '%':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '=' ){
                OPERATOR_PUSH( SYMID_OF_ModBy,RConnect );
                rollBack(-1);
                break;
              }
            }
            OPERATOR_PUSH( SYMID_OF_Mod,RConnect );
            break;
          }
            /////////////////////////
          case '^':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '=' ){
                OPERATOR_PUSH( SYMID_OF_PowerWith,RConnect );
                rollBack(-1);
                break;
              }
            }
            OPERATOR_PUSH( SYMID_OF_Power,RConnect );
            break;
          }
            /////////////////////////
          case '!':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '=' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_UnEqual,RConnect );
                break;
              }
            }
            if ( listStack.Empty() || listStack.Last().state() == RLeft ) {
              OPERATOR_PUSH( SYMID_OF_Not,RRight );
            }else{
              OPERATOR_PUSH( SYMID_OF_Factorial,RLeft );
            }
            break;
          }
          case '|':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '|' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_Or,RConnect );
                break;
              }
            }
            OPERATOR_PUSH( SYMID_OF_Alternatives,RConnect );
            break;
          }
            /////////////////////////
          case '<':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '=' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_LessEqual,RConnect );
                break;
              }
              if ( ch1 == '<' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_Get,RRight );
                break;
              }
            }
            OPERATOR_PUSH( SYMID_OF_Less,RConnect );
            break;
          }
          case '>':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '=' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_GreaterEqual,RConnect );
                break;
              }
              if ( ch1 == '>' ){
                char ch2; bool e2 = getChar(ch2,2);
                if ( e2 && ch2 == '>' ){
                  rollBack(-2);
                  while (  getChar(ch) ){
                    currentWord.append(1,ch);
                    if ( currentWord.size() >= 3 &&
                         *(currentWord.rbegin()) == '<' &&
                         *(currentWord.rbegin()+1) == '<' &&
                         *(currentWord.rbegin()+2) == '<' ){
                      currentWord.erase( currentWord.end()-3,currentWord.end() );
                      break;
                    }
                  }
                  if ( listStack.Size() == 0 or not listStack.Back().SymbolQ() ){
                    listStack.PushBackString( currentWord );
                    break;
                  }
                  Object argsList;
                  argsList.SetList( listStack.Back().ids() );
                  argsList.PushBackString( currentWord );
                  if ( not ( listStack.Empty() ||
                             ( listStack.Last().state() == RConnect ||
                               listStack.Last().state() == RRight ))
                  ){
                    OPERATOR_PUSH( SYMID_OF_ArgsFollow,RConnect );
                  }
                  listStack.PushBackRef( argsList );
                  break;
                }
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_Put,RConnect );
                break;
              }
            }
            OPERATOR_PUSH( SYMID_OF_Greater,RConnect );
            break;
          }
          case '&':{
            char ch1; bool e1=getChar(ch1,1);
            if ( e1 ){
              if (ch1 == '&' ){
                rollBack(-1);
                OPERATOR_PUSH( SYMID_OF_And,RConnect );
                break;
              }
            }
            OPERATOR_PUSH( SYMID_OF_PureFunction,RLeft);
            break;
          }
          case '?':{
            OPERATOR_PUSH( SYMID_OF_Query,RConnect );
            break;
          }

          case '$':{
            bool e=getChar(ch,1);
            if ( e &&  ch>='1' && ch<='9'  ){ // pure function variable
              rollBack(-1);
              Object var; var.SetSymbol( SYMID_OF_FunctionVariable );
              //cerr<<" met a function variable:"<<var<<endl;
              while ( ch >= '0' && ch <='9' ){
                currentWord.append(1,ch);
                if (!getChar(ch)) break;
              }
              rollBack(1);
              var.set_re( ToNumber( currentWord.c_str() ) );
              //cerr<<" function variable:"<<var<<endl;
              listStack.PushBackRef(var);
              break;
            }else if ( e && ch == '_' ){ // pattern variable
              rollBack(-1);
              Object var; var.SetSymbol( SYMID_OF___Variable );
              bool e=getChar(ch,1);
              if ( e && ch>='1' && ch<='9' ){
                rollBack(-1);
                while ( ch >= '0' && ch <='9' ){
                  currentWord.append(1,ch);
                  if (!getChar(ch)) break;
                }
                rollBack(1);
                var.set_re( ToNumber(currentWord.c_str() ) );
              }else{
                var.set_re( 1 );
              }
              listStack.PushBackRef(var);
              break;
            }else if ( e && ch == '$' ){ // memory record serial code
              char ch1;
              bool e1 = getChar(ch1,2);
              if ( e1 && ch1 == '_' ){
                rollBack( -2 );
                Object code; code.SetSymbol( SYMID_OF_SerialCode );
                e=getChar(ch,1);
                if ( e && ch>='1' && ch<='9' ){
                  rollBack(-1);
                  while ( ch >= '0' && ch <='9' ){
                    currentWord.append(1,ch);
                    if (!getChar(ch)) break;
                  }
                  if ( ch == '_' ){
                    u_int first = ToNumber( currentWord.c_str() );
                    currentWord = "";
                    e=getChar(ch);
                    while ( e and ch >= '0' and ch <='9' ){
                      currentWord.append(1,ch );
                      getChar(ch);
                    }
                    rollBack(-1);
                    u_int second = ToNumber( currentWord.c_str() );
                    code.set_idx( Index(first,second) );
                  }else{
                    rollBack(1);
                    code.set_re( ToNumber(currentWord.c_str() ) );
                  }
                }else{
                  code.set_re( 1 );
                }
                listStack.PushBackRef(code);
                break;
              }
            }
            // default case same meaning as $1
            Object var; var.SetSymbol( SYMID_OF_FunctionVariable );
            var.set_re( 1 );
            //dout<<" function variable:"<<var<<endl;
            listStack.PushBackRef(var);
            break;
          }

            /////////////////////////other charactors (forming symbols)
          default:
            { 
              //dout<<"met default branch ch>"<<ch<<endl;
              if ( pond::SpecialCharecterQ(ch) and ch!='$' ){
                zhErroring("语法","字符 \'"+ToString(ch)+"\' 是系统保留字符，不能在此处使用. 位置: 第 "+pond::ToString(lineNumber)+" 行" )||
                  Erroring("Syntax","Character \'"+ToString(ch)+"\' is reserved at line [ "+pond::ToString(lineNumber)+" ]" ) ;
                return -8; 
              }
              currentWord.append(1,ch);
              char ch1;
              if (getChar(ch) ){
                while ( ( not pond::SpecialCharecterQ(ch) ) or
                        (ch == '$' and not (getChar(ch1,1) and ch1 == '_') )     ){
                  currentWord.append(1,ch);
                  if (!getChar(ch)) break;
                }
                rollBack(1);

              }
              u_int id = GlobalPool.Symbols.GetOrNew( currentWord.c_str() );
              u_char oT = GlobalPool.Symbols.GetOperType( id );
              // u_char pL = GlobalPool.Symbols.GetPriorityLeft( id );
              // u_char pR = GlobalPool.Symbols.GetPriorityRight( id );
              // u_char oN = GlobalPool.Symbols.GetOperNum( id );
              //dout<<"get word "<<currentWord<<" with "<<int(id)<<","<<int(pL)<<","<<int(pR)<<","<<int(oT)<<","<<int(oN)<<endl;

              if ( oT > 0 ){
                //dout<<"push operator symbol "<< currentWord << endl;
                OPERATOR_PUSH( id, RSymbol );
              }else{
                listStack.PushBackSymbol(id );
              }
              //listStack.PushBackSymbol( currentWord.c_str() );
              break;
            }//default
          }//end  of switch case
      }
    }//end of if(getChar)..
    //dout<<"switch done, try org"<<endl;
    if ( detectMode ){
      parentList = listStack;
      return 1; // detect mode only get one element
    }

    // RLeft :
    //////////////////////////// reduce the expression if can and need
    if ( expressionEnd || listEnd || End() ){
      //dout<<"liststack>"<<listStack<<endl;
      //dout<<"parentlist>"<<parentList<<endl;
      if ( listStack.Empty() ){
        if ( !parentList.Empty() ){
          //dout<<"get result Null"<<endl;
          parentList.PushBackNull();
        }
      }else{
        //insert proper times into stack for situation like 3+4*k is nessasary
        // but the insert is not here but when reducing

        //dout<<"try reduce listStack>"<<listStack<<endl;
        // reduce preparation
        ////////////////////////////////////////////
        // delete extra linebreak at end
        while ( listStack.Size() > 0 and listStack.Last().SymbolQ(SYMID_OF_LineBreak) ){
          listStack.PopBack();
        }
        // add a null to the start or end if needed
        if ( listStack[1].state() == RSymbol ){ // symbol oper
          u_char oT = GlobalPool.Symbols.GetOperType( listStack[1].ids() );
          if ( oT == 3 )
            listStack.InsertNull( 1 );
        }else{ // normal
          if ( listStack[1].state()>0 && listStack[1].state() != RRight )
            listStack.InsertNull( 1 );
        }
        if ( listStack.Last() .state() == RSymbol ){
          // u_char oT = GlobalPool.Symbols.GetOperType( listStack[1].ids() );
          u_char oN = GlobalPool.Symbols.GetOperNum( listStack[1].ids() );
          for (auto i=0; i< oN; i++ )
            listStack.PushBackNull();
        }else{
          if ( listStack.Last().state() > 0 && listStack.Last().state() != RLeft )
            listStack.PushBackNull();
        }
        //dout<<" listStack after treated: "<<listStack<<endl;
        ////////////////////////////////////////////// reducement
        // find the most important operator (min priority to reduce
        u_int p = listStack.Size(),ppre; //,pnex;

        recordType rp, rpre, rnex;
        while ( listStack.Size()>=2 ){ // find reduce operator according to the priority
          // either of the two must be an operator in original version.
          // but after the extending function comes, things changed. one operator may have one more
          // para on one right side
          // p = listStack.Size();
          //dout<<" start to reduce place at p = "<<p<<" stack="<<listStack<<endl;
          //dout<<"start ";
          for (int i=1; i<= listStack.Size(); i++ ){
            //dout<<"["<<i<<"]:"<<listStack[i]<<"("<<(int)listStack[i].state()<<")  ";
          }
          //dout<<endl;
          while ( listStack[p].state() == 0 and p > 0 ) p --;
          if ( p == 0 ){
            //dout<<"***** come to position at zero p ="<<p<<"listStack ="<<listStack<<endl;
            if ( listStack[1].SymbolQ( SYMID_OF_LineBreak ) ){listStack.Delete(1); p = 2; continue;}
            if ( listStack[2].SymbolQ( SYMID_OF_LineBreak ) ){
              if ( listStack.Size() > 2 ){ // to ;
                listStack[2].SetSymbol(SYMID_OF_CompoundExpression);
                listStack[2].set_state( RConnect );
                p = listStack.Size();
                continue;
              }else{
                listStack.Delete(2);
                p = 2;
                continue;
              }
            }
            if ( listStack[1].ListQ(SYMID_OF_CompoundExpression) ){
              listStack[1].InsertRef( listStack[1].Size()+1, listStack[2] );
              listStack.Delete(2);
              p = 2;
              continue;
            }
            //dout<<"***** come to position at zero p ="<<p<<"listStack ="<<listStack<<endl;
            //cout<<"Times operator ommit detected at line "<<pond::ToString(lineNumber)<<endl;
            listStack.InsertCopy(2,timesOp);
            p = 2;
            //dout<<"after insert listStack = "<<listStack<<endl;
          }
          recordTypeInit(p,rp,listStack);

          //dout<<" try find reduce place from p = "<<p<<" listStack ="<<listStack<<listStack.Size()<<endl;
          ppre = p-1;
          while ( ppre >=1 and
                  ( listStack[ppre].state() == 0 or not listStack[ppre].SymbolQ() ) )
            ppre --;
          //dout<<"get ppre>"<<ppre<<endl;
          // pnex = p+1;
          // while ( pnex <= unsigned(listStack.Size()) and
          //         ( listStack[pnex].state() == 0 or not listStack[pnex].SymbolQ() ) )
          //   pnex++;
          //dout<<"get pnex>"<<pnex<<endl;
          // do a cmplete search once p change position
          //dout<<""<<endl;
          if ( ppre > 0 ){
            //dout<<"try get priority info"<<endl;
            recordTypeInit(ppre,rpre,listStack);
            // recordTypeInit(pnex,rnex,listStack);

            //dout<<"checking Priority..."<<endl;
            if ( rp.state != RRight or ( rp.state == RSymbol and rp.oT == 3 ) ){
              // condition for p to be possible to connect to left expression
              //dout<<"try to get priority"<<endl;
              if ( (rpre.state == RConnect and rpre.pR <= rp.pL) or
                   (rpre.state == RRight and rpre.pR <= rp.pL) or // i~,x
                   (rpre.state == RSymbol and rpre.pR <= rp.pL) or
                   (rpre.state == RLeft)  ){
                p--;
                continue; // will start from the beginning while
              }
            }else{ // if left == right , right will combine first
              //dout<<"they are the same"<<endl;
              // break;
            }
          }
          //dout<<" get reduce place p = "<<p<<", ppre = "<<ppre<<" stack="<<listStack<<endl;
          {//reduce at the position
            u_int oper = listStack[p].ids();
            u_char conp = listStack[p].state();
            //dout<<"do reduce at pos "<<p<<" for "<<listStack<<" with connect code "<<(int)conp<<endl;
            //dout<<" Left: "<<RLeft<<" Right: "<<RRight<<" Connect: "<<RConnect<<endl;

            {
              // add  extra null if two operator are connected and lacks of operant
              // // left side
              // if ( rp.state == RConnect or rp.state ==RLeft or  rp.oT == 3 ){
              //   if ( listStack[p-1].SymbolQ() and
              //        ( listStack[p-1].state() == RRight or
              //          listStack[p-1].state() == RConnect or
              //          ( listStack[p-1].state() == RSymbol and
              //            GlobalPool.Symbols.GetOperNum( listStack[p-1].ids() ) >= 1 ) ) ){
              //     listStack.InsertRef( p, NullObject );
              //     continue;
              //   }
              // }
              // // right side
              // if ( rp.state == RConnect or rp.state == RLeft or rp.oN >= 1 ){
              //   if ( listStack[p+1].SymbolQ() and
              //        ( listStack[p+1].state() == RLeft or
              //          listStack[p+1].state() == RConnect or
              //          ( listStack[p+1].state() == RSymbol and
              //            GlobalPool.Symbols.GetOperType( listStack[p+1].ids() ) == 3 ) ) ){
              //     listStack.InsertRef( p+1, NullObject );
              //     continue;
              //   }
              // }
            }
                

            // special form
            if ( conp == RSymbol ){ // oper symbol like if elif
              //dout<< "deal at pos "<<p<<" stack = "<<listStack << endl;
              u_char oT = GlobalPool.Symbols.GetOperType( listStack[p].ids() );
              u_char oN = GlobalPool.Symbols.GetOperNum( listStack[p].ids() );
              //dout<<"oT,oN = "<<int(oT)<<int(oN)<<endl;
              if ( oT == 1 ){
                // listStack[p].set_state(0);
                //dout<<" do a OT == 1 reduce at pos "<< p<< " for "<<listStack<<" with "<<(int)rp.oN<<endl;
                { // these two operation are only done to symbol operators
                  // insert extra Null if meet a higher operator wall like:      return;
                  for (int i=1; i<=oN; i++ ){
                    while ( listStack[p+i].SymbolQ(SYMID_OF_LineBreak) and
                            p+i <= unsigned(listStack.Size())  )
                      listStack.Delete(p+i);
                    if ( p+i > unsigned(listStack.Size()) ) listStack.PushBackNull();
                    if ( GlobalPool.Symbols.GetPriorityLeft( listStack[p+i].ids() ) > rp.pR ){
                      for ( int j=0; j<= oN - i; j++ ){
                        Object tmp; listStack.InsertRef(p+i,tmp);
                      }
                      break;
                    }
                  }
                }
                listStack[p].Deepen();
                for (auto i=oN; i>= 1; i-- ){
                  //dout<<" do push "<<p+i << " for "<<listStack<<endl;
                  listStack[p].InsertRef(1, listStack[p+i] );
                  listStack.Delete(p+i);
                }
              }else if ( oT == 3 ){
                //dout<<" do a OT == 1 reduce at pos "<< p<< " for "<<listStack<<" with "<<(int)rp.oN<<endl;
                while ( p + oN > unsigned(listStack.Size()) ){
                  listStack.PushBackNull();
                  // zhErroring("语法",listStack[p].ToString() + " 语句结构不完整. 位置: 第 "+pond::ToString(lineNumber)+" 行" )||
                  //   Erroring("Syntax",listStack[p].ToString() + " clause is not complete at line [ "+pond::ToString(lineNumber)+" ]" ) ;
                  // return -8;
                }
                if ( listStack[p-1].SymbolQ( SYMID_OF_LineBreak ) ){
                  //dout<< "p -1 is LineBreak delete and continue"<<endl;
                  listStack.Delete(p-1);
                  continue;
                }
                // listStack[p].set_state(0);
                listStack[p].Deepen();
                listStack[p].PushBackRef( listStack[p-1] );
                for (auto i=oN; i>= 1; i-- ){
                  //dout<<" do push "<<p+i << " for "<<listStack<<endl;
                  listStack[p].InsertRef(2, listStack[p+i] );
                  listStack.Delete(p+i);
                }
                listStack.Delete(p-1);
              }
            }else if ( oper == SYMID_OF_Reciprocal ){
              Object newlist; newlist.SetList( SYMID_OF_Power ); 
              newlist.PushBackRef( listStack( p+1 ) );
              listStack.Delete( p ); 
              newlist.PushBackNumber( -1 );
              listStack[p] = newlist;
            }else if (oper == SYMID_OF_ArgsFollow ){
              //listStack[p+1][0].SetObjectRef( listStack(p-1) ); 
              //dout<<"ArgsFollow at p ="<<p<<endl;
              listStack.Delete(p);
              listStack.Delete(p-1);

            }else if (oper == SYMID_OF_PartArgsFollow ){
              // check if previous one is not a symbol or list
              //dout<<"deal args follow at "<<p<<" with listStack = "<<listStack<<endl;
              // listStack[p].set_state(0);
              bool done = false;
              if ( p > 1 ){
                recordTypeInit(p-1,rpre,listStack);
                if ( rpre.state == RRight or
                     rpre.state == RConnect or
                     rpre.oN > 0 ){
                  //dout<<"as p 1 list"<< endl;
                  listStack[p+1][0].SetSymbol( SYMID_OF_List );
                  listStack.Delete(p);
                  done = true;
                }
                if ( (not done) and  p > 2 ){
                  recordTypeInit(p-2,rpre,listStack);
                  if ( ( rpre.oN > 1 )
                       // and ( rpre.pR < rp.pL )
                  ){
                    //dout<<"as p 2 list"<< endl;
                    listStack[p+1][0].SetSymbol( SYMID_OF_List );
                    listStack.Delete(p);
                    done = true;
                  }
                }
              }
              if ( not done ){
                //dout<<"as normal part "<< endl;
                listStack[p+1].InsertRef(1, listStack[p-1] );
                listStack.Delete(p);
                listStack.Delete(p-1);
              }
              //dout<< "deal done listStack = "<< listStack <<endl;
            }else if ( oper == SYMID_OF_Postfix ){
              // listStack[p].set_state(0);
              listStack[p+1].Deepen();
              listStack[p+1].PushBackRef( listStack[p-1] );
              listStack.Delete(p);
              listStack.Delete(p-1);
            }else if ( oper == SYMID_OF_Prefix ){
              // listStack[p].set_state(0); 
              listStack[p-1].Deepen();
              listStack[p-1].PushBackRef( listStack[p+1] );
              listStack.Delete(p+1);
              listStack.Delete(p);
            }else{ // normal form
              if ( conp == RConnect ){
                //dout<<"Step into RConnect with listStack ="<<listStack<<endl;
                // listStack[p].set_state(0);
                if ( listStack[p-1].SymbolQ( SYMID_OF_LineBreak ) ){
                  //dout<< "p -1 is LineBreak delete and continue"<<endl;
                  listStack.Delete(p-1);
                  continue;
                }
                // insert extra null, when it is operator nearby
                // check if + - > < == !=  around implicit times , if exist, insert and restart reduce
                // left side
                if ( rp.pL > 100 ){  // 2a+b the priority of + is 100
                  if ( p > 2 and
                       listStack[p-2].state() == 0 and
                       not listStack[p-2].SymbolQ(SYMID_OF_LineBreak) ){
                    listStack.InsertCopy(p-1,timesOp);
                    //dout<<"add times at front to "<<listStack<<endl;
                    p = listStack.Size();
                    continue;
                  }
                }
                // right side
                if ( rp.pR > 100 ){ //a+2b
                  if ( p+2 <= unsigned(listStack.Size()) and
                       listStack[p+2].state() == 0 and
                       not listStack[p+2].SymbolQ(SYMID_OF_LineBreak) ){
                    listStack.InsertCopy(p+2,timesOp);
                    p = listStack.Size();
                    //dout<<"add times at end to "<<listStack<<endl;
                    continue;
                  }
                }
                // normal situation    a+b
                {
                  // listStack[p].set_state(0);
                  listStack[p].ToList(); 
                  if ( rp.pL == rp.pR and listStack[p-1].ListQ( listStack[p][0] ) ){
                    listStack[p].InsertRef( 1, listStack[p-1].Begin(), listStack[p-1].End() );
                  }else{
                    listStack[p].PushBackRef( listStack[p-1] );
                  }
                  if ( rp.pL == rp.pR and listStack[p+1].ListQ( listStack[p][0] ) ){
                    listStack[p].InsertRef( listStack[p].Size()+1, listStack[p+1].Begin(), listStack[p+1].End() );
                  }else{
                    listStack[p].PushBackRef( listStack[p+1] );
                  }
                  listStack.Delete(p+1);
                  listStack.Delete(p-1);
                  // do flatten for children if same head and have pL == pR
                }
                //dout<<"do normal reduce to "<<listStack<<endl;

                //dout<<" A Connect Operator processed:"<<listStack<<endl;
              }else if ( conp == RLeft ){
                listStack[p].ToList();
                // listStack[p].set_state(0);
                listStack[p].PushBackRef( listStack(p-1) );
                listStack.Delete(p-1);
              }else if ( conp == RRight ){
                listStack[p].ToList();
                // listStack[p].set_state(0);
                listStack[p].PushBackRef( listStack(p+1) );
                listStack.Delete(p+1);
                p++;
              }else{
                zhErroring("语法","操作符连接关系分析失败. 位置: 第 "+pond::ToString(lineNumber)+" 行")||
                  Erroring("Syntax","Failed to analyze operator connection relationship at line "+pond::ToString(lineNumber) ) ;
                return -8;
              }
            }

            //dout<<"end";
            // for (int i=1; i<= listStack.Size(); i++ )
            //  ddout<<""<<i<<":"<<listStack[i]<<"("<<(int)listStack[i].state()<<")  ";
            // ddout<<endl;
            //normal form reduce end
            //dout<<"end reduce with p = "<<p<<" listStack = "<< listStack <<endl;
            // p must + 1 to ensure position, imortant
            p++; while ( p > unsigned(listStack.Size()) ) p--;
            //dout<<"end one deduce " << endl;
            ///////////
          }// end of reduce at the position
        } //end of reduce,end of while size()>1

        //dout<<"reduce done>"<<listStack<<endl;
        /////////////////////////////////////////////// get final result
        if ( listStack.Size()>1 ){
          zhErroring("语法","表达式分析失败. 位置: 第 "+pond::ToString(lineNumber)+" 行" )||
            Erroring("Syntax","Expression cannot be reduced at line ["+pond::ToString(lineNumber)+"]" ) ;
          return -8;
        }
        //dout<< " get one expression listStack = "<<listStack<<endl;
        //dout<< "  parentList = "<<parentList<<endl;
        // if ( listStack.SimpleListQ( SYMID_OF_Sequence ) ){
        //   if ( parentList.SimpleListQ( SYMID_OF_Parenthesis )  or
        //        parentList.SimpleListQ( SYMID_OF_ExpressionList )  or
        //        parentList.SimpleListQ( SYMID_OF_Dict )  or
        //        parentList.SimpleListQ( SYMID_OF_List )  or
        //        )
        parentList.PushBackRef( listStack(1) );// 0 is its head, 1st is its first child list
        listStack.Delete(1);
      }
      if ( listEnd || End() ){
        //dout<< "meet final end parent = "<<parentList<< " listStack = "<<listStack<<endl;
        return 0;
      }
    }//end of if *End
  }//end of while

}
#undef OPERATOR_PUSH
#undef IS_LAST_OPERATOR

// Object ImportList::GetList(string str,int depth){
//   ImportList impo(str);
//   Object lsuper; lsuper.SetList( SYMID_OF_List );
//   //dout<<"Try the core GetList to fill"<<lsuper<<endl;
//   impo.GetList(lsuper,depth);
//   return lsuper;
// }

Object ImportList::ToExpression(string str){
  ImportList impo(str);
  Object lsuper; lsuper.SetList( SYMID_OF_ExpressionList );
  //dout<<"Try the core GetList to fill"<<lsuper<<endl;
  impo.GetList(lsuper,0);
  if ( lsuper.Size() == 1 )
    lsuper = lsuper[1];
  return lsuper;
}





