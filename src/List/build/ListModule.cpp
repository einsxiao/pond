#define ModuleName List
#include"pond.h"
#include"ListModule.h"
#include"algorithm"
using namespace pond;
using namespace std;

static INIT_SYMID_OF(Exists);
static INIT_SYMID_OF(exists);
static INIT_SYMID_OF(contain);
static INIT_SYMID_OF(Insert);
static INIT_SYMID_OF(insert);
static INIT_SYMID_OF(Delete);
static INIT_SYMID_OF(delete);
static INIT_SYMID_OF(Append);
static INIT_SYMID_OF(append);
static INIT_SYMID_OF(PushBack);
static INIT_SYMID_OF(push_back);
static INIT_SYMID_OF(push);
static INIT_SYMID_OF(swap);
static INIT_SYMID_OF(交换);
static INIT_SYMID_OF(Clean);
static INIT_SYMID_OF(clean);
static INIT_SYMID_OF(Length);
static INIT_SYMID_OF(length);
static INIT_SYMID_OF(size );
static INIT_SYMID_OF(reverse );
static INIT_SYMID_OF(sort );
static INIT_SYMID_OF(添加);
static INIT_SYMID_OF(插入);
static INIT_SYMID_OF(get);
static INIT_SYMID_OF(获取);
static INIT_SYMID_OF(删除);
static INIT_SYMID_OF(排序);
static INIT_SYMID_OF(清空);
static INIT_SYMID_OF(长度);
static INIT_SYMID_OF(大小);
static INIT_SYMID_OF(包含);
static INIT_SYMID_OF(翻转);
static INIT_SYMID_OF(有);
static INIT_SYMID_OF(has);
static INIT_SYMID_OF(Size);
static INIT_SYMID_OF(SetPart);
static INIT_SYMID_OF(front);
static INIT_SYMID_OF(back);
static INIT_SYMID_OF(pop_back);
static INIT_SYMID_OF(头元素);
static INIT_SYMID_OF(去头);
static INIT_SYMID_OF(尾元素);
static INIT_SYMID_OF(去尾);
static INIT_SYMID_OF(items);
static INIT_SYMID_OF(列表);


ModuleConstruction{
  ///////////////////////////////////////////////////////////////////
  {
    RegisterFunction("List",List,this);
    // AddAttribute("List",AttributeType::DelayFunction);
    // AddAttribute("List",AttributeType::DelaySetable);
    AddAttribute("List",AttributeType::Conjunctable);
  }
  {
    RegisterFunction("Dict",Dict,this);
    // AddAttribute("Dict",AttributeType::DelayFunction );
    // AddAttribute("Dict",AttributeType::DelaySetable);
    AddAttribute("Dict",AttributeType::Conjunctable  );
    AddAttribute("Dict",AttributeType::Setable       );
    AddAttribute("Dict",AttributeType::Partable      );
  }
  {
    RegisterFunction("Range",Range,this);
    RegisterFunction("range",Range,this);
    RegisterFunction("序列",Range,this);
  }
  ///////////////////////////////////////////////////////////////////
  {
    RegisterFunction("Take",Take,this); //Take[expr,{i,10}]
    // AddAttribute("Take",AttributeType::GrammarUnit);
  }
  {
    RegisterFunction("Drop",Drop,this); 
    // AddAttribute("Drop",AttributeType::GrammarUnit);
  }
  {
    RegisterFunction("Part",Part,this); 
    AddAttribute("Part",AttributeType::HoldFirst);
    RegisterFunction("SetPart",SetPart,this);
    AddAttribute("SetPart",AttributeType::HoldFirst);
    AddAttribute("Part",AttributeType::Setable );
  }
  ///////////////////////////////////////////////////////////////////
  {
    RegisterFunction("Table",Table,this);
    AddAttribute("Table",AttributeType::HoldAll);
  }
  // AddAttribute("Cases",AttributeType::HoldAll);
  ///////////////////////////////////////////////////////////////////
  RegisterFunction("Head",Head,this); 
  RegisterFunction("表头", Head,this); 
  RegisterFunction("First",First,this); 
  RegisterFunction("头元素",First,this); 
  RegisterFunction("Last",Last,this); 
  RegisterFunction("尾元素",Last,this); 
  RegisterFunction("Most",Most,this); 
  RegisterFunction("去尾",Most,this); 
  RegisterFunction("Rest",Rest,this); 
  RegisterFunction("去头",Rest,this); 
  RegisterFunction("len", (MemberFunction)(&ListModule::PD_Length),this); 
  RegisterFunction("长度", (MemberFunction)(&ListModule::PD_Length),this); 
  RegisterFunction("大小", (MemberFunction)(&ListModule::PD_Length),this); 
  RegisterFunction("bool", (MemberFunction)(&ListModule::PD_Boolean),this); 
  RegisterFunction("布尔值", (MemberFunction)(&ListModule::PD_Boolean),this); 
  ///////////////////////////////////////////////////////////////////
  RegisterFunction("Sort",Sort,this); 
  RegisterFunction("sort",Sort,this); 
  RegisterFunction("排序",Sort,this); 
  RegisterFunction("Flatten",Flatten,this); 
  RegisterFunction("Join",Join,this); 
  RegisterFunction("join",Join,this); 
  RegisterFunction("和并",Join,this); 
  RegisterFunction("Dimensions",Dimensions,this); 
  ///////////////////////////////////////////////////////////////////
  {
    RegisterFunction("Append",Append,this);
    RegisterFunction("AppendTo",AppendTo,this);
    AddAttribute("AppendTo",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("Insert",Insert,this);
    RegisterFunction("InsertTo",InsertTo,this);
    AddAttribute("InsertTo",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("Delete",Delete,this);
    RegisterFunction("DeleteFrom",DeleteFrom,this);
    AddAttribute("DeleteFrom",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("PopBack",PopBack,this);
    AddAttribute("PopBack",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("Reverse",Reverse,this);
    RegisterFunction("翻转",Reverse,this);
    RegisterFunction("Transpose",Transpose,this);
    RegisterFunction("转置",Transpose,this);
  }
  {
    RegisterFunction("NullQ",NullQ,this);
    RegisterFunction("NumberQ",NumberQ,this);
    RegisterFunction("StringQ",StringQ,this);
    RegisterFunction("SymbolQ",SymbolQ,this);
    RegisterFunction("ListQ",ListQ,this);
    RegisterFunction("曰无",zh_NullQ,this);
    RegisterFunction("曰数字",NumberQ,this);
    RegisterFunction("曰字串",StringQ,this);
    RegisterFunction("曰符号",SymbolQ,this);
    RegisterFunction("曰列表",ListQ,this);
  }
  {
    RegisterFunction("Map",Map,this);
    AddAttribute("Map",AttributeType::HoldFirst);
    RegisterFunction("Apply",Apply,this);
    AddAttribute("Apply",AttributeType::HoldFirst);
    RegisterFunction("Scan",Scan,this);
    AddAttribute("Scan",AttributeType::HoldFirst);
    RegisterFunction("Nest",Nest,this);
    AddAttribute("Nest",AttributeType::HoldFirst);
  }

}
ModuleDestruction{
}

inline bool toNormalIndex(int index,u_int len,int&normalindex){
  if ( index > (int)len or index < -(int)len )
    return false;
  if ( index < 0 )
    normalindex = len +index+1;
  return true;
}

//#define d_return(x) ({dprintf("");return x; })
bool SpanToIndexes(int len,Object &index, int &start, int &end, int &step ){
  //dout<<"try deal with span = "<< index <<endl;
  if ( not (index.Size() == 3 or index.Size() == 2 ) )
    return(false);
  //step 
  bool givenStep = false;
  if ( index.Size() == 2 ){
    step = 1;
  }else if ( index[3].NumberQ( ) ){
    step = (int)index[3];
    givenStep = true;
    if ( step == 0 )
      return(false);
  }else if ( index[3].NullQ() ){
    step = 1;
  }else
    return(false);
  //start
  if ( index[1].NullQ() ){
    if ( step > 0 )
      start = 1;
    else
      start = len;
  }else if ( index[1].NumberQ( ) ){
    start = (int)index[1];
    if ( start > len or start < - len )
      return(false);
    if ( start < 0 ) start += len +1;
  }else
    return(false);
  //end
  if ( index[2].NullQ() ){
    if ( step > 0 )
      end = len;
    else
      end = 1;
  }else if ( index[2].NumberQ( ) ){
    end = (int)index[2];
    if ( end > len or end < -len ) return(false);
    if ( end < 0 ) end += len +1;
  }else return(false);

  if ( givenStep ) return true;
  if ( start > end and step > 0 ){
    step = -step;
  }
  return true;
}
//#undef return

void localSetPart(Object&valueList,Object&partList,u_int pos,Object&rightValue);

int ListModule::List(Object &Argv){
  // DelaySet_Context(List){ // 
  //   //dout<<"list delay set context"<<endl;
  //   // List[...][...] = 
  //   // |  1,0 |  1,1 |   2
  //   Object res;
  //   localSetPart(Argv[1][0], Argv[1], 1, Argv[2] );
  //   Argv = Argv[2];
  //   ReturnNormal;
  // }
  // DelayFunction_Context(List){
  //   //dout<<"list delay function context"<<endl;
  //   Object res;
  //   GetPartList( Argv[0], Argv, 1, res, true );
  //   Argv = res;
  //   ReturnNormal;
  // }
  Conjunct_Context(List){
    //dout<<"list conjunct context"<<endl;
    Object &list = Argv[1];
    Object &oper = Argv[2];

    Conjunct_Case3( push, append, 添加 ){
      CheckArgsShouldEqual( oper, 1 );
      EvaKernel->Evaluate( oper[1] );
      list.PushBackCopy( oper[1] );
      Return(list);
    }
    Conjunct_Case2( insert, 插入 ){
      CheckArgsShouldEqual( oper, 2 );
      EvaKernel->Evaluate( oper[1] );
      EvaKernel->Evaluate( oper[2] );

      CheckArgsShouldBeNumber( oper, 1 );
      list.InsertCopy( int(oper[1]), oper[2] );
      Return(list);
    }
    Conjunct_Case2( swap, 交换 ){
      CheckArgsShouldEqual( oper, 2 );
      EvaKernel->Evaluate( oper[1] );
      EvaKernel->Evaluate( oper[2] );
      CheckArgsShouldBeNumber( oper,1);
      CheckArgsShouldBeNumber( oper,2);
      int p1 = oper[1].Number();
      int p2 = oper[2].Number();
      if ( p1 != p2 ) 
        list[p1].Swap( list[p2] );
      Return(list);
    }
    Conjunct_Case3(has, exists, 包含 ){
      CheckArgsShouldEqual(oper,1);
      for (auto iter = list.Begin(); iter != list.End(); iter ++ ){ 
        //dout<<" do test "<< (*iter) << "  ?=  "<< oper<<endl;
        if ( (*iter) == oper[1]  ){
          ReturnBoolean(true);
        }
      }
      ReturnBoolean(false);
    }
    Conjunct_Var_Case2( size, 大小 ){
      ReturnNumber( list.Size() ); 
    }
    Conjunct_Case4( length,长度,size,大小){
      CheckArgsShouldEqual( oper, 0 );
      ReturnNumber( list.Size() ); 
    }
    Conjunct_Case2( delete, 删除 ){
      CheckArgsShouldEqual( oper, 1 );
      EvaKernel->Evaluate( oper[1] );
      CheckArgsShouldBeNumber( oper, 1 );
      int ind = int( oper[1] );
      if ( ind < 1 or ind > (int)list.Size() ) {
        _Erroring( zh,"列表::删除","下标 "+ToString(ind)+" 超出有效范围.") ||
          _Erroring(_,"List::Delete","Index "+ToString(ind)+" is out of range.");
        return -1;
      }
      list.Delete(ind);
      Return(list);
    }
    Conjunct_Case2( clean,清空 ){
      CheckArgsShouldEqual( oper, 0 );
      list.DeleteElements();
      Return(list);
    }
    Conjunct_Case2( reverse,翻转 ){
      CheckArgsShouldEqual( oper, 0 );
      list.Reverse();
      Return(list);
    }
    Conjunct_Case2( sort, 排序 ){
      CheckArgsShouldEqual( oper, 0 );
      list.Sort();
      Return(list);
    }
    Conjunct_Case2( front, 头元素 ){
      CheckArgsShouldEqual( oper, 0 );
      Return(list[1]);
    }
    Conjunct_Case2( front, 去头 ){
      CheckArgsShouldEqual( oper, 0 );
      list.Delete(1);
      Return(list);
    }
    Conjunct_Case2( back, 尾元素 ){
      CheckArgsShouldEqual( oper, 0 );
      Return( list.Back() );
    }
    Conjunct_Case2( pop_back, 去尾 ){
      CheckArgsShouldEqual( oper, 0 );
      list.PopBack();
      Return(list);
    }
    //If no case matched report error 
    Object l = ( oper.ListQ() ? oper[0] : oper );
    _Erroring( zh,"列表",l.ToString() + " 不是列表方法" )||
      _Erroring(_,"List",l.ToString() + " is not a List method.");
    ReturnError;
  }

  //dealing Sequence
  u_int i = 1;
  while ( i<= Argv.Size() ){
    if ( Argv.ElementAt(i).ListQ( pond::SYMID_OF_Sequence  ) ){
      Argv.InsertRef( Argv.Begin()+i, Argv.ElementAt(i).Begin(), Argv.ElementAt(i).End() );
      Argv.Delete(i);
    }else{
      EvaKernel->Evaluate( Argv[i] );
      i++;
    }
  }
  return 1;
}

int ListModule::range_iter(Object&Argv,long&N,double&start,double&incr){
  CheckShouldBeWithin(1,3);
  if ( Argv.Size() == 1 ){
    EvaKernel->Evaluate( Argv[1] );
    if ( Argv[1].NumberQ() ){
      start  = 1;
      N      = Argv[1].Number();
      incr   = 1;
      return 1;
    }
    return 0;
  }

  if ( Argv.Size() == 2 ){
    EvaKernel->Evaluate(Argv[1]);
    EvaKernel->Evaluate(Argv[2]);
    if ( Argv[1].NumberQ() && Argv[2].NumberQ() ){
      start = Argv[1].Number();
      N     = Argv[2].Number() - start+1;
      incr  = 1;
      return 1;
    }
    return 0;
  }

  if ( Argv.Size() == 3 ){
    EvaKernel->Evaluate(Argv[1]);
    EvaKernel->Evaluate(Argv[2]);
    EvaKernel->Evaluate(Argv[3]);
    if ( Argv[1].NumberQ() && Argv[2].NumberQ() && Argv[3].NumberQ() ){
      start =Argv[1].Number() ;
      incr  =Argv[3].Number() ;
      if ( incr == 0 ){
        _Erroring( zh,"range","range的增量不能为零.")||
          _Erroring(_,"range","Increment should not be a zero.");
        return 0;
      }
      N     =(Argv[2].Number() - start)/incr + 1;
      return 1;
    }
    return 0;
  }
  return 0;
}

int ListModule::Range(Object &Argv){
  double start, incr; long N;
  if ( ListModule::range_iter(Argv,N,start,incr) == 0 ){
    _Erroring(zh,"for:in:range","range 不合法") ||
      _Erroring(_,"for:in:range","range is not valid.");
    ReturnError;
  }
  Argv.SetList();
  Argv.ReserveSize( N );
  for ( int i = 0; i< N; i++ ){
    Argv.PushBackNumber(start+i*incr);
  }
  ReturnNormal;
}

static int take(Object&expr,Object&takelist,u_int p,Object&resultList){//expr should make sure to be a nonatom
  Object & iter = takelist[p];
  resultList.SetList();
  if ( iter.NumberQ() ){
    int N =(double) iter;
    if ( N >= 0 ){
      if ( N > (int)expr.Size() ){
        { Erroring("Take","Cannot take position 1 through "+ToString(N)+"."); ReturnError; }
      }
      for ( int i = 1; i<= N; i++ ){
#define address_ele_take(p_i)                                   \
        if ( p == takelist.Size() ){                            \
          resultList.PushBackRef( expr[p_i] );                  \
        }else{                                                  \
          resultList.PushBackNull();                            \
          take( expr[p_i], takelist, p+1, resultList.Back() );  \
        }                                                       \
        /////
        address_ele_take(i);
      }
    }else{
      if ( -N> (int)expr.Size() ){
        { Erroring("Take","Cannot take position "+ToString(N)+"through -1."); ReturnError; }
      }
      for ( int i = expr.Size() + N+1; i<= (int)expr.Size(); i++ ){
        address_ele_take(i);
      }
    }
    return 1;
  }
  if ( (iter).ListQ(SYMID_OF_List) ){//AtomQ must be not true
    if ( iter.Size() > 3 || iter.Size()< 1 ){
      { Erroring("Take","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
    }
    Object &num1 = (iter)[1];
    int ind1,bak;
    if ( num1.NumberQ() ){
      bak = ind1 =(double) num1;
      if ( ind1<0 ) ind1 += expr.Size() +1 ; 
      if ( iter.Size() == 1){//only one number...
        if ( ind1 > (int)expr.Size() or ind1 < 1 ){
          { Erroring("Take","Cannot take position "+ToString(bak) ); ReturnError; }
        }
        address_ele_take( ind1 );
        return 1;
      }
    }else{
      { Erroring("Take","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
    }
    Object&num2 = (iter)(2);
    int ind2;
    if ( num2.NumberQ() ){
      bak = ind2 =(double) num2;
      if ( ind2<0 ) ind2 += expr.Size()+1;
      if ( iter.Size() == 2){
        if ( ind2 > (int)expr.Size()  or ind2 < 1 ){
          { Erroring("Take","Cannot take position "+ToString(bak)+" through "+ToString(bak)+"."); ReturnError; }
        }
        for ( int i=ind1; i<= ind2; i++ ){
          address_ele_take( i );
        }
        return 1;
      }
    }else{
      { Erroring("Take","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
    }
    Object &inter = (iter)(3);
    int ninter;
    if ( inter.NumberQ() ){
      ninter =(double)inter;
      if ( iter.Size() == 3){
        if ( ninter == 0 ){
          { Erroring("Take","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
        }
        if ( ninter> 0 ){
          for ( int i = ind1; i <= ind2; i+=ninter ){
            address_ele_take(i);
          }
        }else if ( ninter < 0 ){
          for ( int i = ind1; i>= ind2; i+=ninter ){
            address_ele_take(i);
          }
        }
        return 1;
      }
    }else{
      { Erroring("Take","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
    }
  }
  { Erroring("Take","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
}

int ListModule::Take(Object &Argv){
  CheckArgsShouldNoLessThan(Argv,2);
  CheckShouldBeList(1);
  
  Object resultList;
  take( Argv[1], Argv, 2, resultList ); 
  Argv = resultList;
  ReturnNormal;
}

static int drop(Object&expr, Object&droplist,u_int p){
  Object &iter = droplist[p];
  if ( iter.AtomQ() ){
    if ( iter.NumberQ() ){
      int N = iter.Number();
      if ( N>=0 ){
        if ( N> (int)expr.Size() ){ Erroring("Drop","Cannot drop position 1 through "+ToString(N)+" in "+expr.ToString()+"."); ReturnError; }
        expr.Delete( expr.Begin(),expr.Begin()+N );
#define address_next_level_of_drop_if_exist                             \
        if ( p < droplist.Size() ){                                     \
          for (auto iter = expr.Begin(); iter != expr.End(); iter ++ ){ \
            drop ( *iter, droplist, p+ 1 );                             \
          }                                                             \
        }
        address_next_level_of_drop_if_exist;
      }else{
        if ( -N> (int)expr.Size() ){{ Erroring("Drop","Cannot drop position "+ToString(N)+" through -1 in "+expr.ToString()+".");} ReturnError; }
        expr.Delete( expr.End() + N, expr.End() );
        address_next_level_of_drop_if_exist;
      }
      return 1;
    }else{
      { Erroring("Drop","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
    }
  }
  if ( (iter).ListQ(SYMID_OF_List) ){//AtomQ must be not true
    if ( iter.Size() > 3 || iter.Size()< 1 ){{ Erroring("Drop","Sequence specification "+iter.ToString()+" is not valid.");} ReturnError; }
    Object &num1 = (iter)(1);
    int ind1,indb1;
    if ( num1.NumberQ() ){
      indb1 = ind1 =  num1.Number() ;
      if ( ind1<0 ) ind1 += expr.Size()+1;
      if ( iter.Size() == 1){
        if ( abs(ind1) > (int)expr.Size() ){{ Erroring("Drop","Cannot drop position "+ToString(indb1)+" in "+expr.ToString()+"."); } ReturnError; }
        expr.Delete(ind1);
        address_next_level_of_drop_if_exist;
        return 1;
      }
    }else{
      { Erroring("Drop","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
    }
    Object&num2 = (iter)(2);
    int ind2,indb2;
    if ( num2.NumberQ()  ){
      indb2 = ind2 =  num2.Number();
      if ( ind2<0 ) ind2 += expr.Size()+1;
      if ( iter.Size() == 2){
        if ( abs(ind2) > (int)expr.Size() ){{ Erroring("Drop","Cannot drop position "+ToString(indb1)+" through "+ToString(indb2)+" in "+expr.ToString()+".");} ReturnError; }
        for (int i = ind1; i<= ind2; i++ )
          expr.Delete(ind1);
        address_next_level_of_drop_if_exist;
        return 1;
      }
    }else{
      { Erroring("Drop","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
    }
    Object &inter = (iter)(3);
    int ninter;
    if ( inter.IntegerQ() ){
      ninter =  inter.Number();
      if ( iter.Size() == 3){
        if ( (int)inter == 0 ){ Erroring("Drop","Sequence specification "+iter.ToString()+" is not valid."); ReturnError; }
        if ( ninter> 0 ){
          int adj = ( ind2 - ind1 ) % ninter ;
          for ( int i = ind2-adj; i >= ind1; i-=ninter ){
            expr.Delete( i );
          }
          address_next_level_of_drop_if_exist;
        }else if ( ninter < 0 ){
          for ( int i = ind1; i>= ind2; i+=ninter ){
            expr.Delete( i );
          }
          address_next_level_of_drop_if_exist;
        }
        return 1;
      }
    }
    Erroring("Drop","Sequence specification "+iter.ToString()+" is not valid.");
    ReturnError; 
  }
  Erroring("Drop","Sequence specification "+iter.ToString()+" is not valid.");
  ReturnError;
  return 0;
}

int ListModule::Drop(Object &Argv){
  CheckArgsShouldNoLessThan(Argv,2);
  CheckShouldBeList(1);
  Object expr = Argv(1);
  if ( expr.AtomQ() ){
    Erroring("Drop","Nonatomic experssion expected at position 1.");
    ReturnError;
  }
  drop( Argv[1], Argv, 2 );
  Argv = Argv[1];
  ReturnNormal;
}

// for obj = Part[ {...}, p1, p2]
// valueList is obj[1], partList is obj,  pos start from 2 (last evaluated position)
int ListModule::GetPartList(Object&valueList,Object&partList,u_int pos,Object&resultList,bool isReal){
  if ( valueList.NullQ() or partList.NullQ() ) return -1;
  if ( valueList.AtomQ() ) {
    _Erroring( zh,"索引运算","不能对原子量 "+valueList.ToString()+" 进行索引运算.")||
      _Erroring(_,"Part","Part specification for atom "+valueList.ToString()+" is not allowed.");
    ReturnError;
  }
  bool LastLevel = ( partList.Size() == pos );
  Object &index = partList[pos]; 
  if ( index.ListQ() ){
    if ( index.ListQ(SYMID_OF_List) ){
      resultList.SetList();
      for ( u_int i =1; i<= index.Size(); i++ ){
        if ( not (index)[i].NumberQ() ){
          _Erroring( zh,"索引运算","索引下标应该是数字")||
            _Erroring(_,"Part","Part specification should be a number");
          ReturnError;
        }
        int ind = index[i].Number();
        if ( not toNormalIndex(ind,valueList.Size(),ind) ){
          _Warning( zh,"索引运算",valueList.ToString()+" 索引下标 "+ToString(ind)+" 超出范围.") ||
            _Warning(_,"Part","Part specification "+ToString(ind)+" for "+valueList.ToString()+" is out of range.");
          return -1;
        }
        if ( LastLevel ){
          if ( isReal ){
            //dprintf(" do a real pushback ");
            resultList.PushBackRef( valueList[ind] ); 
          } else {
            //dprintf(" do a copy pushback ");
            resultList.PushBackCopy( valueList[ind] ); 
          }
        }else{
          resultList.PushBackNull();
          GetPartList(valueList(ind),partList,pos+1,resultList[i],isReal);
        }
      }
      return 1;
    }else if ( index.ListQ( SYMID_OF_Span ) ){
      int start, end, step;
      resultList.SetList();
      if ( not  SpanToIndexes( valueList.Size(), index, start, end, step ) ){ // just return {}
        ReturnNormal;
        // Erroring("Span","Span "+index.ToString()+" is in the wrong form.");
        // ReturnError;
      }
      //dprintf("%s -> start = %d, end = %d, step = %d",index.ToString().c_str(),start,end,step);
      for ( int i = start; (step>0?i<=end:i>=end); i+=step ){
        if (LastLevel ){
          if ( isReal ){
            //dprintf(" do a real pushback ");
            resultList.PushBackRef( valueList[ i ] );
          }else{
            //dprintf(" do a copy pushback ");
            resultList.PushBackCopy( valueList[ i ] );
          }
        }else{
          resultList.PushBackNull();
          GetPartList( valueList[ i ], partList, pos+1, resultList[i], isReal);
        }
      }
      return 1;
    }
    _Erroring( zh,"索引运算"," 索引下标格式错误.") ||
      _Erroring(_,"Part","Part specification is in wrong form.");
    ReturnError; 
  }
  if ( not partList[pos].NumberQ() )
    return -1;
  int ind = partList[pos].Number();
  if ( not toNormalIndex(ind,valueList.Size(),ind) ){

    _Warning( zh,"索引运算",valueList.ToString()+" 索引下标 "+ToString(ind)+" 超出范围.") ||
      _Warning(_,"Part","Part specification "+ToString(ind)+" for "+valueList.ToString()+" is out of range.");
    return -1;
  }
  if ( LastLevel ){
    if ( isReal ){
      //dprintf(" do a real replace");
      resultList =  valueList[ind] ;
    }else{
      //dprintf(" do a copy replace");
      resultList.CopyObject( valueList[ind] );
    }
  }else{
    GetPartList( valueList[ind], partList, pos+1, resultList, isReal);
  }
  return 1;
}

int ListModule::Part(Object&Argv){
  //dout<<"into Part with Argv ="<<Argv<<endl;
  Set_Context(Part){
    //dout<<"into Part set context with Argv ="<<Argv<<endl;
    return SetPart( Argv );
  }
  CheckShouldNoLessThan(2);
  // rewrite the Part[Part[a_,b__],c__] :> Part[a,b,c]
  while ( Argv[1].ListQ( SYMID_OF_Part ) ){ //flaten the part
    CheckArgsShouldNoLessThan(Argv[1],2);
    Argv.InsertRef(Argv.Begin()+1,Argv[1].Begin(),Argv[1].End());
    Argv.Delete(1);
  }
  // the value of part should be evaluated till last
  //cerr<<"ori valueList ="<< Argv[1].ToFullFormString() <<endl;
  EvaKernel->Evaluation::Evaluate( Argv[1], false, true ); 
  // check if Partable, and deternmine whether send to Partable one
  //dout<<"check if is partable argv="<< Argv<<endl;
  if ( Argv[1].ListQ() ){
    EvaRecord * rec = EvaKernel->GetEvaRecord( Argv[1][0] );
    if ( rec and AttributeQ( rec->attributes , AttributeType::Partable) ){
      // the Part pass through has form
      // Part( f(...), index... )           f is referenced value if is from value pair
      //dout<< "pass "<<Argv<<" to "<< Argv[1][0] <<endl;
      return EvaKernel->Call( rec, Argv );
    }
  }
  Object resultList;
  // here, part list should not be real
  if ( GetPartList(Argv[1],Argv,2,resultList,false) < 0 ) ReturnHold; 
  Argv = resultList;
  //dout<<" after part is:"<<Argv<<endl;
  // extra evaluate need to be done
  // e.g.  funcs = {x^2,y^2}   funcs[[1]]
  EvaKernel->Evaluate(Argv); 
  //dout<<" after evaluate:"<<Argv<<endl;
  ReturnNormal;
}

void localSetPart(Object&valueList,Object&partList,u_int pos,Object&rightValue){
  if ( valueList.NullQ() or partList.NullQ() ) return;
  if ( valueList.AtomQ() ) {
    _Erroring(zh,"索引运算","对原子类型的值 "+valueList.ToString()+" 进行索引运算不太合适.") ||
      _Erroring(_,"Part","Part specification for atom "+valueList.ToString()+" is not allowed.");
    return;
  }
  bool LastLevel = ( partList.Size() == pos );
  Object&index = partList[pos];
  EvaKernel->Evaluate( index );
  if ( index.ListQ() ){
    if ( index.ListQ(SYMID_OF_List) ){
      if ( rightValue.ListQ(SYMID_OF_List) ){
        if ( index.Size() != rightValue.Size() ) {
          _Erroring(zh,"索引赋值","赋值左右两边列表维度不同.")||
            _Erroring(_,"SetPart","Left Part List should have the size of the right value List.");
          return;
        }
        for ( u_int i=1; i<= index.Size(); i++ ){
          if ( not (index)[i].NumberQ() ){
            _Erroring(zh,"索引运算","所引运算必须使用数字")||
              _Erroring(_,"Part","Part specification should be a number");
            return;
          }
          int ind = (index)[i].Number();
          if ( not toNormalIndex(ind,valueList.Size(),ind) ){
            _Erroring(_,"索引运算","索引号 "+ToString(ind)+" 所指向的元素不存在") ||
              _Erroring(_,"Part","Part "+ToString(ind)+" of list does not exist.");
            return;
          }
          if ( LastLevel ){
            valueList[ind].CopyObject( rightValue[i] );
          }else{
            localSetPart( valueList[ind], partList,pos+1,rightValue[i] );
          }
        }
        return;
      }
      //only an normal Object
      for ( u_int i =1; i<= index.Size(); i++ ){
        if ( not (index)[i].NumberQ() ){
          _Erroring(zh,"索引运算","所引运算必须使用数字")||
            _Erroring(_,"Part","Part specification should be a number");
          return;
        }
        int ind = (index)[i].Number();
        if ( not toNormalIndex(ind,valueList.Size(),ind) ){
          _Erroring(_,"索引运算","索引号 "+ToString(ind)+" 所指向的元素不存在") ||
            _Erroring(_,"Part","Part "+ToString(ind)+" of list does not exist.");
          return;
        }
        if ( LastLevel ){
          valueList[ind].CopyObject( rightValue );
        }else{
          localSetPart( valueList[ind],partList,pos+1,rightValue);
        }
      }
      return;
    }else if ( index.ListQ( SYMID_OF_Span ) ){
      int start, end, step;
      if ( not  SpanToIndexes( valueList.Size(), index, start, end, step ) ) {
        _Erroring(zh, "范围索引","范围索引 "+index.ToString()+" 格式错误.");
        _Erroring(_,"Span","Span "+index.ToString()+"in not in the right form.");
        return;
      }
      for( int i = start; (step>0?i<= end:i>=end); i+=step ){
        if ( LastLevel )
          valueList[i].CopyObject( rightValue );
        else
          localSetPart( valueList[i], partList, pos+1, rightValue );
      }
      return;
    }
    _Erroring(zh, "索引赋值","索引值 "+index.ToString()+" 格式错误.") ||
    _Erroring(_,"SetPart","Part index "+index.ToString()+" is not in the right form.");
    return;
  }
  if ( not index.NumberQ() ) {
    _Erroring(zh,"索引运算","索引运算必须使用数字")||
      _Erroring(_,"Part","Part specification should be a number");
    return;
  }
  int ind = index.Number();
  if ( not toNormalIndex(ind,valueList.Size(),ind) ){
    _Erroring(zh,"索引运算","索引值 "+ToString(ind)+" 所指元素不存在.")||
      _Erroring(_,"Part","Part "+ToString(ind)+" of "+valueList.ToString()+" does not exist.");
    return;
  }
  if ( LastLevel ){
    valueList[ind].CopyObject( rightValue );
  }else{
    localSetPart( valueList[ind],partList,pos+1,rightValue);
  }
  return;
}

int ListModule::SetPart(Object&part,Object &rightValue){
  if ( not part.ListQ( SYMID_OF_Part ) ) {
    _Erroring(zh,"索引赋值","错误调用索引赋值函数.") ||
      _Erroring(_,"SetPart","Invalid calling for SetPart.");
    ReturnError;
  }
  CheckArgsShouldNoLessThan(part, 2);
  while ( part[1].ListQ( SYMID_OF_Part ) ){
    CheckArgsShouldNoLessThan( part[1],2 );
    part.InsertRef( part.Begin()+1, part[1].Begin(),part[1].End() );
    part.Delete(1);
  }
  Object value = EvaKernel->GetValuePairRef( part[1] );
  if ( value.NullQ() ) {
    _Erroring(zh,"索引赋值",part[1].ToString()+" 不是一个被赋值的符号.") ||
      _Erroring(_,"SetPart",part[1].ToString()+" is not an Symbol with a value.");
    ReturnError;
  }else if( not value[2].ListQ() ){
    _Erroring(zh,"索引赋值",part[1].ToString()+" 的值不是一个列表.") ||
      _Erroring(_,"SetPart","value of "+part[1].ToString()+" is not a list.");
    ReturnError;
  }
  // check whether setable 
  EvaKernel->Evaluate( value[2][0] );
  if ( value[2][0].SymbolQ() ){ // a vlaued symbol
    EvaRecord * rec = EvaKernel->GetEvaRecord( value[2][0] );;
    if ( rec and AttributeQ( rec->attributes , AttributeType::Setable ) ){
      Object Argv(ObjectType::List,SYMID_OF_Set );
      Argv.PushBackRef( part )     // part 
        .PushBackRef( rightValue ) // right value
        .PushBackRef( value[2] );  // dict ref
      //dout<<" pass "<<  Argv  <<" to "<<  value[2][0] <<endl;
      return EvaKernel->Call(rec, Argv );
    }
  }
  localSetPart( value[2], part, 2, rightValue );
  return 0;
}

int ListModule::SetPart( Object&Argv){
  CheckShouldEqual( 2 );
  SetPart( Argv[1], Argv[2]);
  Argv = Argv[2];
  ReturnNormal;
}

///////////////////////////////////
int ListModule::Head(Object &Argv){
  CheckShouldEqual(1);
  if ( Argv[1].ListQ() ){
    Object head = Argv[1][0];
    Argv = head;
    ReturnNormal;
  }else{
    const char * head = Argv[1].Head();
    Argv.SetSymbol( head );
    ReturnNormal;
  }
}

int ListModule::First(Object &Argv){
  CheckShouldEqual(1);
  CheckArgsShouldBeList(Argv,1);
  if ( Argv[1].Size()<1 ){
    Warning("First","Try to get first element of an empty List");
    ReturnNull;
  }
  Argv = Argv[1][1];
  ReturnNormal;
}

int ListModule::Last(Object &Argv){
  CheckShouldEqual(1);
  CheckShouldBeList(1 );
  if ( Argv[1].Size()<1 ){
    Warning("Last","Try to get last element of an emtpy List");
    ReturnNull;
  }
  Argv = Argv[1][Argv[1].Size() ];
  ReturnNormal;
}
int ListModule::Most(Object &Argv){
  CheckShouldEqual(1);
  CheckShouldBeList(1);
  if ( Argv[1].Size()>1 ){
    Argv[1].PopBack();
  }
  Argv = Argv[1];
  ReturnNormal;
}

int ListModule::Rest(Object &Argv){
  CheckShouldEqual(1);
  CheckShouldBeList(1 );
  //dout<<"Rest with argv = "<< Argv <<endl;
  if ( Argv[1].Size()>1 ){
    Argv[1].Delete(1);
  }
  Argv = Argv[1];
  ReturnNormal;
}
///////////////////////////////////

static INIT_SYMID_OF( Table );
int ListModule::Table(Object &Argv){
  CheckArgsShouldNoLessThan(Argv,2);
  //dout<<"come to table with argv = "<<Argv<<endl;
  while ( Argv.Size() >2 ){//change Table to recursively form
    Object newtable; newtable.SetList( SYMID_OF_Table );
    newtable.PushBackRef( Argv(1) );
    newtable.PushBackRef( Argv(2) );
    Argv.Delete(2);
    Argv(1)=newtable;
  }
  Object&expr = Argv[1];
  Object&iter = Argv[2];
  if ( not (iter).ListQ() ) EvaKernel->Evaluate(iter);
  if ( not (iter).ListQ() || iter.Size()<1 || iter.Size()>4 ) {
    Erroring(Argv.Key(),"Argument "+iter.ToString()+" is not an iterator.\n");
    ReturnError;
  }
  if ( iter.Size() == 1 ){
    Object&num = (iter)(1);
    EvaKernel->Evaluate(num);
    if ( num.NumberQ() ){
      Object newlist; newlist.SetList();
      int N =(double)num;
      newlist.ReserveSize(N);
      for ( int i = 1; i<=N; i++ ){
        Object newexpr; newexpr.CopyObject( expr );
        EvaKernel->Evaluate(newexpr);
        newlist.PushBackRef( newexpr );
      }
      Argv = newlist;
      ReturnNormal;
    }
    { Erroring(Argv.Key(),"Iterator "+iter.ToString()+" does not have appropriate bounds."); ReturnError; }
  }
  if ( iter.Size() == 2 ){
    Object&var = (iter)(1);
    if ( !var.SymbolQ() ) { Erroring("Do",var.ToString()+" cannot be used as an iterator."); ReturnError; }
    Object&num = (iter)(2);
    EvaKernel->Evaluate(num);
    Object&vobj = EvaKernel->StackPushCopy(var,var)[2];
    if ( num.NumberQ() ){ // form:  {i, 100}
      Object newlist; newlist.SetList();
      int N =(double)num;
      newlist.ReserveSize(N);
      for ( int i = 1; i<=N; i++ ){ 
        //dout<<"try loop "<<i<<endl;
        vobj.SetNumber( i );
        //dout<< "vobj = "<<vobj <<endl;
        //dout<< " stack = "<< EvaKernel->currentValueTable->stackTable << endl;
        //dout<< " test getvalue for i = "<< EvaKernel->GetValuePairRef( tmp ) << endl;
        Object newexpr; newexpr.CopyObject( expr );
        //dout<<" with newexpr ="<<newexpr<<endl;
        EvaKernel->Evaluate(newexpr);
        newlist.PushBackRef( newexpr );
      }
      Argv = newlist;
      //dout<<"result:"<<Argv<<endl;
      EvaKernel->StackPop();
      //dout<<"result after pop:"<<Argv<<endl;
      ReturnNormal;
    }else if ( (num).ListQ()){  // form:   {i, { 1,3,5, 9 , 20} }
      Object&vobj= EvaKernel->StackPushCopy(var,var)[2];
      Object newlist; newlist.SetList();
      for ( u_int i = 1; i<=num.Size(); i++ ){
        Object newexpr; newexpr.CopyObject( expr );
        vobj.SetNumber( i );
        EvaKernel->Evaluate(newexpr);
        newlist.PushBackCopy( newexpr );
      }
      Argv =  newlist ;
      EvaKernel->StackPop();
      ReturnNormal;
    }
    Erroring(Argv.Key(),"Iterator "+iter.ToString()+" does not have appropriate bounds."); 
    ReturnError; 
  }
  if ( iter.Size() == 3 ){ // form:  {i, 1, 100}
    Object&var = (iter)(1);
    if ( !var.SymbolQ() ) { Erroring(Argv.Key(),var.ToString()+" cannot be used as an iterator."); ReturnError; }
    Object&num1 = (iter)(2);
    Object&num2 = (iter)(3);
    EvaKernel->Evaluate(num1);
    EvaKernel->Evaluate(num2);
    if ( num1.NumberQ() && num2.NumberQ() ){
      Object&vobj = EvaKernel->StackPushCopy(var,var)[2];
      Object newlist; newlist.SetList();
      int Nbegin = num1.Number() ;
      int Nend =  num2.Number();
      for ( int i = Nbegin; i<=Nend; i++ ){
        Object newexpr; newexpr.CopyObject(expr);
        vobj.SetNumber(i);
        EvaKernel->Evaluate(newexpr);
        newlist.PushBackCopy( newexpr );
      }
      Argv = newlist;
      EvaKernel->StackPop();
      ReturnNormal;
    }
    Erroring(Argv.Key(),"Iterator "+iter.ToString()+" does not have appropriate bounds."); 
    ReturnError;
  }
  if ( iter.Size() == 4 ){ // form:  {i, 1, 100, 3}
    Object&var = (iter)(1);
    if ( !var.SymbolQ() ) { Erroring(Argv.Key(),var.ToString()+" cannot be used as an iterator."); ReturnError; }
    Object&num1 = (iter)(2);
    Object&num2 = (iter)(3);
    Object&incr = (iter)(4);
    //dout<<"current var iter is "<<var<<endl;
    EvaKernel->Evaluate(num1);
    EvaKernel->Evaluate(num2);
    EvaKernel->Evaluate(incr);
    if ( num1.NumberQ() && num2.NumberQ() && incr.NumberQ() ){
      Object pair = EvaKernel->StackPushCopy(var,var);
      //dout<<"value pair is"<<pair<<endl;
      Object newlist; newlist.SetList();
      double Nbegin = num1.Number();
      double Nend = num2.Number();
      double Nincr = incr.Number();
      if ( Nincr == 0 ) { Erroring("Table","Increment should not be a zero."); ReturnError; }
      int steps = (Nend-Nbegin)/Nincr;
      for ( int i = 0; i<= steps; i++ ){
        Object newexpr; newexpr.CopyObject(expr);
        //dout<<"loop "<<i<<" for expr: "<< newexpr<<endl;
        pair[2].SetNumber(Nbegin+i*Nincr);
        //dout<<"value pair is "<<pair<<endl;
        EvaKernel->flag = 1;
        //dout<<"kernel pointer = "<<EvaKernel<<endl;
        EvaKernel->Evaluate(newexpr);
        EvaKernel->flag = 0;
        //dout<<"value pair after evaluate "<<pair<<endl;
        newlist.PushBackCopy( newexpr );
      }
      Argv =  newlist ;
      EvaKernel->StackPop();
      ReturnNormal;

    }
    { Erroring(Argv.Key(),"Iterator "+iter.ToString()+" does not have appropriate bounds."); ReturnError; }
  }
  ReturnHold;
}

int ListModule::Sort(Object &Argv){
  CheckArgsShouldEqual(Argv,1);
  if ( not Argv[1].ListQ() ) {
    Erroring(Argv.Key(),"1st argument of "+Argv.ToString()+" should be a Argv.");
    ReturnError;
  }
  Argv[1].ExpressionSort();
  Argv = Argv[1];
  ReturnNormal;
}

int ListModule::Flatten(Object &Argv){
  CheckArgsShouldEqual(Argv,1);
  Argv = Argv[1];
  Argv.Flatten();
  ReturnNormal;
}

int ListModule::Join(Object &Argv){
  //dout<<"try to do Join with Argv = "<<Argv<<endl;
  if ( Argv.Size() < 1  ){
    Argv.SetList( SYMID_OF_List );
    ReturnNormal;
  }
  bool is_dict = true;
  for (int i=1; i<= Argv.Size(); i++){
    if ( not Argv[i].ListQ(SYMID_OF_Dict) and not Argv[i].NullQ() ){
      is_dict = false;
      break;
    }
  }
  if ( is_dict ){
    Object&ndict = Argv[1];
    for (int i = 2; i<= Argv.Size(); i++){
      if ( Argv[i].NullQ() ) continue;
      for (auto iter = Argv[i].Begin(); iter != Argv[i].End(); iter ++ ){
        ndict.DictInsertPairRef( *iter ); 
      }
    }
    Return( ndict );
  }else{
    u_int i = 1;
    while ( i<= Argv.Size() ){
      if ( Argv[i].ListQ( SYMID_OF_List ) or
           Argv[i].ListQ( SYMID_OF_Parenthesis ) or
           Argv[i].ListQ( SYMID_OF_ExpressionList ) or 
           Argv[i].ListQ( SYMID_OF_Dict )
      ){
        int s = Argv[i].Size();
        Argv.InsertRef( Argv.Begin()+i, Argv(i).Begin(), Argv(i).End() );
        Argv.Delete(i);
        i+= s;
      }else{
        i++;
      }
    }
    Argv[0].SetSymbol(SYMID_OF_List);
  }
  ReturnNormal;
}

int ListModule::Dimensions(Object&Argv){
  CheckShouldBeWithin(1,2);
  CheckShouldEqual(1);
  Object  dim; dim.SetList();
  INIT_SYMID_OF(Matrix);
  if ( Argv.Size() == 1 ){
  //   if ( Argv[1].ListQ( SYMID_OF_Matrix ) ){
  //     if ( Argv[1].Size() != 1 or not Argv[1][1].StringQ() ) { Erroring("Matrix","Matrix should have a string to specify its name."); ReturnError; }
  //     Matrix*mat = EvaKernel->GetMatrix( Argv[1][1].Key() );
  //     if ( mat == NULL ){
  //       { Erroring("Dimensions",Argv(1).ToString()+" does not exist."); ReturnError; }
  //     }
  //     int nd = mat->DimN();
  //     for (int i=1; i<=nd; i++)
  //       dim.PushBackNumber( mat->Dim(i) );
  //     Argv = dim;
  //     ReturnNormal;
  //   }else{
  //     Object::Dimensions(Argv[1],dim,-1);
  //     Argv = dim;
  //     ReturnNormal;
  //   }
  // }else{ 
    CheckShouldBeNumber(2);
    int depth = Argv[2].Number();
    Object::Dimensions(Argv[1],dim,depth);
    Argv = dim;
    ReturnNormal;
  }
  ReturnNormal;
}

int ListModule::Dict(Object&Argv){

  //dout<<"into Dict current dict ="<< Argv <<endl;
  Set_Context(Part){
    //DelaySet Calling  Dict(...) = 
    // the pass path is Part(..., ...) = ... => SetPart(...)  => Dict( ... )
    // so the form should be SetPart( partexpr,  rightvalue, valuepair[2]ref )
    // partexpr:    Part(a, 9)     a evaluated to be valuepair
    //dout<<"is dict SetPart context argv ="<<Argv<<endl;
    //dout<<"into DelaySet of Part Dict current dict ="<< Argv <<endl;
    CheckShouldEqual(3);
    CheckArgsShouldEqual(Argv[1],2);
    Object&key = Argv[1][2];
    EvaKernel->Evaluate( key );
    Object&right = Argv[2];
    Object&dict = Argv[3];
    Object pair(ObjectType::List, SYMID_OF_KeyValuePair );
    pair.PushBackRef( key );
    pair.PushBackCopy( Argv[2] );
    dict.DictInsertOrUpdatePairRef( pair );
    //dout<<"new dict ="<<dict<<endl;
    //Argv = Argv[2];
    ReturnNull;
  }
  Part_Context(Dict){
    //dout<<"into part context of dict"<<Argv<<endl;
    CheckShouldEqual(2);
    Object&dict = Argv[1];
    Object&key = Argv[2];
    Object pair = dict.DictGetPair(key);
    if ( pair.NullQ() ){
      _Warning( zh,"字典:下标运算","不存在键值为" + key.ToString() + "的记录") ||
        _Warning(_,"Dict:Part","No record with key value "+key.ToString() );
      ReturnNull;
    }
    Argv.CopyObject( pair[2] );
    //dout<<"Argv Set to "<<Argv<<endl;
    ReturnNormal;
  }
  Set_Context(Dict){
    //dout<<"into normal DelaySet Dict current dict ="<< Argv <<endl;
    ReturnNormal;
  }
  DelayFunction_Context(Dict){
    //dout<<"into DelayFunction of Dict current dict ="<< Argv <<endl;
    CheckShouldEqual(1);
    Object&dict = Argv[0];
    Object&key = Argv[1];
    Object pair = dict.DictGetOrNewPair( key );
    ReturnObject( pair[2] );
  }

  Conjunct_Context(Dict){//Conjunct calling
    Object&dict = Argv[1];
    Object&oper = Argv[2];
    Conjunct_Var_Case2(size,大小){
      ReturnNumber( dict.Size() );
    }
    Conjunct_Case4(length, size, 大小, 长度){
      CheckArgsShouldEqual(oper,0);
      ReturnNumber( dict.Size() );
    }
    Conjunct_Case3(has, exists, 包含 ){
      CheckArgsShouldEqual(oper,1);
      EvaKernel->Evaluate( oper[1] );
      Object::iterator iter;
      if ( dict.DictGetPosition( oper[1] ,iter) == 0  )
        ReturnBoolean(true);
      ReturnBoolean(false);
    }
    Conjunct_Case2(get, 获取){
      CheckArgsShouldBeWithin(oper,1,2);
      Object::iterator iter;
      EvaKernel->Evaluate( oper[1] );
      if ( oper.Size() == 1 ){
        if ( dict.DictGetPosition( oper[1] ,iter) == 0  )
          Argv = (*iter)[2].Copy();
        else
          Argv.SetNull();
        ReturnNormal;
      }else{
        if ( dict.DictGetPosition( oper[1] ,iter) == 0  ){
          Argv = (*iter)[2].Copy();
        }else{
          EvaKernel->Evaluate( oper[2] );
          Argv = oper[2];
        }
        ReturnNormal;
      }
    }
    Conjunct_Case2(delete, 删除 ){
      CheckArgsShouldEqual(oper,1);
      EvaKernel->Evaluate( oper[1] );
      dict.DictDelete( oper[1] ) ;
      ReturnNull;
    }
    Conjunct_Case2(insert, 插入 ){
      CheckArgsShouldEqual(oper,2);
      EvaKernel->Evaluate( oper[1] );
      EvaKernel->Evaluate( oper[2] );
      oper[0].SetSymbol( SYMID_OF_KeyValuePair );
      dict.DictInsertOrUpdatePairRef( oper );
      ReturnNull;
    }
    Conjunct_Case2( clean,清空 ){
      CheckArgsShouldEqual( oper, 0 );
      dict.DeleteElements();
      Argv = dict;
      ReturnNormal;
    }
    Conjunct_Case2( items,列表){
      CheckArgsShouldEqual( oper, 0 );
      Argv = dict.Copy();
      Argv[0].SetSymbol(SYMID_OF_List);
      ReturnNormal;
    }
    Object l = ( oper.ListQ() ? oper[0] : oper );
    _Erroring( zh,"列表",l.ToString() + " 不是字典方法")||
      _Erroring(_,"List",l.ToString() + " is not a Dict method.");
    ReturnNormal;
  }
  //normal calling doing nothing
  ReturnNormal;
}


int ListModule::Append(Object&Argv)
{
  CheckShouldEqual(2);
  Object &var = Argv[1];
  Object &ele = Argv[2];

  if ( not var.ListQ() ){
    { Erroring("Append","Cannot append element to an Non-List Object."); ReturnError; }
  }

  var.PushBackCopy( ele );
  Argv = Argv[1];
  ReturnNormal;
}

int ListModule::AppendTo(Object&Argv)
{
  CheckShouldEqual(2);
  Object &var = Argv[1];
  Object &ele = Argv[2];

  if ( var.NumberQ() or var.StringQ() ){
    { Erroring("Append","Cannot append element to an Object of Number or String."); ReturnError; }
  }
  // if not an Symbol with value assigned to 
  if ( var.ListQ() ){ 
    EvaKernel->Evaluate( var );
    if ( not var.ListQ() ){
      { Erroring("Append","Cannot append element to Non-List Object."); ReturnError; }
    }
    var.PushBackCopy( ele );
    Argv = Argv[1];
    ReturnNormal;
  }
  Object res = EvaKernel->GetValuePairRef(var);
  if ( res.NullQ() )
    { Erroring("Append","Cannot append element to an symbol without a value."); ReturnError; }
 
  if ( res.ids() != 0 ){
    if (  EvaKernel->AttributeQ(res.ids() ,AttributeType::Protected) ){
      { Erroring("Append",(var).ToString() +(string)" is Protected."); ReturnError; }
    }
  }
  EvaKernel->Evaluate( res[2] );

  if ( not res[2].ListQ() ){
    { Erroring("Append","Cannot append element to Non-List Object."); ReturnError; }
  }

  res[2].PushBackCopy( ele );
  Argv.CopyObject( res[2] );
  ReturnNormal;
}

int ListModule::Insert(Object&Argv)
{
  CheckShouldEqual(3);
  CheckShouldBeNumber(3);
  Object &var = Argv[1];
  Object &ele = Argv[2];
  int pos = Argv[3].Number();

  if ( not var.ListQ() ){
    { Erroring("Insert","Cannot insert element to Non-List Object."); ReturnError; }
  }

  if ( pos < 0 ){
    pos = var.Size() - pos + 1;
  }
  if ( pos < 1 or pos > ( (int) var.Size() + 1) )
    { Erroring("Insert","Insert position is out of List Range."); ReturnError; }
  
  var.InsertCopy(pos, ele);
  Argv = Argv[1];
  ReturnNormal;
}

int ListModule::InsertTo(Object&Argv)
{
  CheckShouldEqual(3);
  CheckShouldBeNumber(3);
  Object &var = Argv[1];
  Object &ele = Argv[2];
  int pos = Argv[3].Number();

  if ( var.NumberQ() or var.StringQ() ){
    { Erroring("Insert","Cannot insert element to an Object of Number or String."); ReturnError; }
  }
  // if not an Symbol with value assigned to 
  if ( var.ListQ() ){ 
    EvaKernel->Evaluate( var );
    if ( not var.ListQ() ){
      { Erroring("Insert","Cannot insert element to Non-List Object."); ReturnError; }
    }
    var.InsertCopy(pos, ele );
    Argv = Argv[1];
    ReturnNormal;
  }
  Object res = EvaKernel->GetValuePairRef(var);
  if ( res.NullQ() )
    { Erroring("Insert","Cannot insert element to an symbol without a value."); ReturnError; }
 
  if ( res.ids() != 0 ){
    if (  EvaKernel->AttributeQ(res.ids() ,AttributeType::Protected) ){
      { Erroring("Insert",(var).ToString() +(string)" is Protected."); ReturnError; }
    }
  }
  EvaKernel->Evaluate( res[2] );

  if ( not res[2].ListQ() ){
    { Erroring("Insert","Cannot insert element to Non-List Object."); ReturnError; }
  }

  res[2].InsertCopy(pos, ele );
  Argv.CopyObject( res[2] );
  ReturnNormal;
}


int ListModule::Delete(Object&Argv)
{
  CheckShouldEqual(2);
  CheckShouldBeNumber(2);
  Object &var = Argv[1];
  int pos = Argv[2].Number();
  
  if ( not var.ListQ() ){
    { Erroring("Delete","Cannot delete element to Non-List Object."); ReturnError; }
  }
  
  if ( pos < 0 ){
    pos = var.Size() - pos + 1;
  }
  if ( pos < 1 or pos > (int) var.Size()  )
    { Erroring("Delete","Delete position is out of List Range."); ReturnError; }
  
  var.Delete(pos);
  Argv = Argv[1];
  ReturnNormal;
}

int ListModule::DeleteFrom(Object&Argv)
{
  CheckShouldEqual(2);
  CheckShouldBeNumber(2);
  Object &var = Argv[1];
  int pos = Argv[2].Number();

  if ( var.NumberQ() or var.StringQ() ){
    { Erroring("Delete","Cannot delete element to an Object of Number or String."); ReturnError; }
  }
  // if not an Symbol with value assigned to 
  if ( var.ListQ() ){ 
    EvaKernel->Evaluate( var );
    if ( not var.ListQ() ){
      { Erroring("Delete","Cannot delete element to Non-List Object."); ReturnError; }
    }
    if ( pos < 1 or pos > (int) var.Size()  )
      { Erroring("Delete","Delete position is out of List Range."); ReturnError; }
    var.Delete(pos);
    Argv = Argv[1];
    ReturnNormal;
  }

  Object res = EvaKernel->GetValuePairRef(var);
  if ( res.NullQ() )
    { Erroring("DeleteFrom","Cannot delete element to an symbol without a value."); ReturnError; }
 
  if ( res.ids() != 0 ){
    if (  EvaKernel->AttributeQ(res.ids() ,AttributeType::Protected) ){
      { Erroring("DeleteFrom",(var).ToString() +(string)" is Protected."); ReturnError; }
    }
  }

  EvaKernel->Evaluate( res[2] );

  if ( not res[2].ListQ() ){
    { Erroring("DeleteFrom","Cannot delete element to Non-List Object."); ReturnError; }
  }
  
  if ( pos < 1 or pos > (int) var.Size() )
    { Erroring("Delete","Delete position is out of List Range."); ReturnError; }
  res[2].Delete( pos );
  Argv.CopyObject( res[2] );
  ReturnNormal;
}

int ListModule::PopBack(Object&Argv)
{
  CheckShouldEqual(1);
  Object &var = Argv[1];

  if ( var.NumberQ() or var.StringQ() ){
    { Erroring("PopBack","Cannot delete element to an Object of Number or String."); ReturnError; }
  }
  // if not an Symbol with value assigned to 
  if ( var.ListQ() ){ 
    EvaKernel->Evaluate( var );
    if ( not var.ListQ() ){
      { Erroring("PopBack","Cannot delete element to Non-List Object."); ReturnError; }
    }
    Object t = var.Last();
    var.PopBack();
    Argv = t;
    ReturnNormal;
  }
  Object res = EvaKernel->GetValuePairRef(var);
  if ( res.NullQ() )
    { Erroring("PopBack","Cannot delete element to an symbol without a value."); ReturnError; }
 
  if ( res.ids() != 0 ){
    if (  EvaKernel->AttributeQ(res.ids() ,AttributeType::Protected) ){
      { Erroring("PopBack",(var).ToString() +(string)" is Protected."); ReturnError; }
    }
  }
  EvaKernel->Evaluate( res[2] );

  if ( not res[2].ListQ() ){
    { Erroring("PopBack","Cannot delete element to Non-List Object."); ReturnError; }
  }
  
  Object t = res[2].Last();
  res[2].PopBack();
  Argv = t;
  ReturnNormal;
}


int ListModule::Reverse(Object&Argv){
  CheckShouldEqual(1);
  CheckShouldBeList(1);
  Argv[1].Reverse();
  Argv = Argv[1];
  ReturnNormal;
}

int ListModule::Transpose(Object&Argv){
  CheckShouldEqual(1);
  CheckShouldBeList(1);
  Argv = Argv[1];
  int row_size = Argv.Size();
  if ( row_size < 1 ) ReturnNormal;
  int col_size = Argv[1].Size();
  if ( col_size < 1 ) ReturnNormal;
  if ( row_size == 1 and col_size ==1 ) ReturnNormal;
  for ( int i=2; i<= row_size; i++ ){
    if ( Argv[i].Size() != col_size ) ReturnNormal;
  }
  if ( row_size < col_size ){ // long to to show row
    for (auto i = row_size+1; i<= col_size;i++  ) Argv.PushBackList(SYMID_OF_List);
    for (auto i=1; i<= row_size; i++){
      // part 1  of ref swap
      for (auto j=i+1; j<=row_size; j++){
        Argv[i][j].SwapRef(Argv[j][i]);
      }
      // part 2 of push back ref while pop back at the same time
      for (auto j=col_size; j>row_size; j--){
        Argv[j].PushBackRef( Argv[i][j] );
        Argv[i].PopBack();
      }
    }
  }else{ // short row to long row   row_size > col_size
    for (auto i=1; i<= col_size; i++){
      // part 1  of ref swap
      for (auto j=i+1; j<=col_size; j++){
        Argv[i][j].SwapRef(Argv[j][i]);
      }
      // part 2 of push back ref while pop back at the same time
      for (auto j=row_size; j>col_size; j--){
        Argv[i].PushBackRef( Argv[j][i] );
      }
    }
    Argv.Delete( Argv.Begin()+col_size, Argv.End() );

  }
  ReturnNormal;
}


int ListModule::NullQ(Object&Argv){
  CheckShouldEqual(1);
  if ( Argv[1].NullQ() )
    ReturnSymbol(SYMID_OF_True);
  else
    ReturnSymbol(SYMID_OF_False);
}
int ListModule::zh_NullQ(Object&Argv){
  CheckShouldEqual(1);
  if ( Argv[1].NullQ() )
    ReturnSymbol(SYMID_OF_真);
  else
    ReturnSymbol(SYMID_OF_假);
}

int ListModule::NumberQ(Object&Argv){
  CheckShouldEqual(1);
  if ( Argv[1].NumberQ() )
    ReturnSymbol(SYMID_OF_True);
  else
    ReturnSymbol(SYMID_OF_False);
}
int ListModule::zh_NumberQ(Object&Argv){
  CheckShouldEqual(1);
  if ( Argv[1].NumberQ() )
    ReturnSymbol(SYMID_OF_真);
  else
    ReturnSymbol(SYMID_OF_假);
}
int ListModule::StringQ(Object&Argv){
  CheckShouldEqual(1);
  if ( Argv[1].StringQ() )
    ReturnSymbol(SYMID_OF_True);
  else
    ReturnSymbol(SYMID_OF_False);
}
int ListModule::zh_StringQ(Object&Argv){
  CheckShouldEqual(1);
  if ( Argv[1].StringQ() )
    ReturnSymbol(SYMID_OF_真);
  else
    ReturnSymbol(SYMID_OF_假);
}


int ListModule::SymbolQ(Object&Argv){
  CheckShouldBeWithin(1,2);
  if ( not Argv[1].SymbolQ() ) ReturnSymbol( SYMID_OF_False );
  if ( Argv.Size() == 1 ){
    ReturnSymbol(SYMID_OF_True);
  }else{
    if ( Argv[1] == Argv[2] )
      ReturnSymbol( SYMID_OF_True );
    ReturnSymbol( SYMID_OF_False );
  }
  ReturnNormal;
}
int ListModule::zh_SymbolQ(Object&Argv){
  CheckShouldBeWithin(1,2);
  if ( not Argv[1].SymbolQ() ) ReturnSymbol( SYMID_OF_假 );
  if ( Argv.Size() == 1 ){
    ReturnSymbol(SYMID_OF_真);
  }else{
    if ( Argv[1] == Argv[2] )
      ReturnSymbol( SYMID_OF_真 );
    ReturnSymbol( SYMID_OF_假 );
  }
  ReturnNormal;
}

int ListModule::ListQ(Object&Argv){
  CheckShouldBeWithin(1,2);
  if ( not Argv[1].ListQ() ) ReturnSymbol( SYMID_OF_False );
  if ( Argv.Size() == 1 ){
    if ( Argv[1].ListQ(SYMID_OF_List) )
      ReturnSymbol(SYMID_OF_True);
    else
      ReturnSymbol(SYMID_OF_False);
  }else{
    CheckShouldBeSymbol(2);
    if ( Argv[1].ListQ( Argv[2] ) )
      ReturnSymbol(SYMID_OF_True);
    else
      ReturnSymbol( SYMID_OF_False );
  }
  ReturnNormal;
}
int ListModule::zh_ListQ(Object&Argv){
  CheckShouldBeWithin(1,2);
  if ( not Argv[1].ListQ() ) ReturnSymbol( SYMID_OF_假 );
  if ( Argv.Size() == 1 ){
    if ( Argv[1].ListQ(SYMID_OF_List) )
      ReturnSymbol(SYMID_OF_真);
    else
      ReturnSymbol(SYMID_OF_假);
  }else{
    CheckShouldBeSymbol(2);
    if ( Argv[1].ListQ( Argv[2] ) )
      ReturnSymbol(SYMID_OF_真);
    else
      ReturnSymbol( SYMID_OF_假 );
  }
  ReturnNormal;
}

DefinePDFunction(EmptyQ,Argv){
  CheckShouldEqual(1);
  if ( Argv[1].ListQ() and Argv[1].Empty()  ){
    Argv.SetSymbol(SYMID_OF_True);
    ReturnNormal;
  }
  Argv.SetSymbol(SYMID_OF_False);
  ReturnNormal;
}
DefinePDFunction(曰空,Argv){
  CheckShouldEqual(1);
  if ( Argv[1].ListQ() and Argv[1].Empty()  ){
    Argv.SetSymbol(SYMID_OF_真);
    ReturnNormal;
  }
  Argv.SetSymbol(SYMID_OF_假);
  ReturnNormal;
}

DefinePDFunction(Length,Argv){
  CheckShouldEqual(1);
  if ( Argv[1].ListQ() ){
    ReturnNumber( Argv[1].Size() );
  }else if ( Argv[1].StringQ() ){
    ReturnNumber( Argv[1].ref_string().size() );
  }
  _Warning(zh,"长度","尝试对非列表或字符串对象求长度.") ||
    _Warning(_,"Length","Try to apply Length on a non-List or String Object.");
  Argv.SetNumber( 0 );
  ReturnNormal;
}

int ListModule::Map(Object&Argv){
  CheckShouldEqual(2);
  CheckShouldNotBeNumber(1);
  CheckShouldNotBeString(1);
  CheckShouldBeList(2);
  if ( Argv[2].Size() < 1 ){
    Argv = Argv[2];
    ReturnNormal;
  }
  Object&no = Argv[1];
  no.ForceDeepen();

  for (int i=1; i<Argv[2].Size(); i++ ){
    Object obj; obj.CopyObject( no );
    obj.PushBackRef( Argv[2][i] );
    Argv[2][i] = obj;
  }
  no.PushBackCopy( Argv[2].Last() );
  Argv[2].Last() = no;
  Argv = Argv[2];
  EvaKernel->Evaluate( Argv );
  ReturnNormal;
}

int ListModule::Apply(Object&Argv){
  CheckShouldEqual(2);
  CheckShouldNotBeNumber(1);
  CheckShouldNotBeString(1);
  CheckShouldBeList(2);
  Argv[2][0] = Argv[1];
  Argv = Argv[2];
  EvaKernel->Evaluate( Argv );
  ReturnNormal;
}

int ListModule::Scan(Object&Argv){
  CheckShouldEqual(2);
  CheckShouldNotBeNumber(1);
  CheckShouldNotBeString(1);
  CheckShouldBeList(2);
  if ( Argv[2].Size() < 1 ){
    ReturnNull;
  }
  Object&no = Argv[1];
  no.ForceDeepen();

  for (int i=1; i<Argv[2].Size(); i++ ){
    Object obj; obj.CopyObject( no );
    obj.PushBackRef( Argv[2][i] );
    EvaKernel->Evaluate(obj);
  }
  no.PushBackCopy( Argv[2].Last() );
  Argv[2].Last() = no;
  EvaKernel->Evaluate( Argv[2].Last() );
  ReturnNull;
}

int ListModule::Nest(Object&Argv){
  CheckShouldEqual(3);
  CheckShouldNotBeNumber(1);
  CheckShouldNotBeString(1);
  CheckShouldBeNumber(3);
  // EvaKernel->PreEvaluate( Argv[1] );
  //dout<<"current nest argv is "<<Argv<<endl;
 
  int N = Argv[3].Number();
  Object&obj = Argv[2];
  for (auto i=0; i<N - 1; i++){
    obj.DeepenAsFirst();
    //dout<<" obj deppen to "<<obj<<endl;
    // obj[0].SetObjectValuedSymbolKeeped( Argv[1] );
    obj[0].CopyObject( Argv[1] );
    //dout<<" obj build to "<<obj<<endl;
    EvaKernel->Evaluate( obj );
  }
  obj.DeepenAsFirst(); obj[0] = Argv[1];
  EvaKernel->Evaluate( obj );
  Argv = Argv[2];
  ReturnNormal;
}

DefinePDFunction(Boolean,Argv){
  CheckShouldEqual(1);
  auto res = Argv[1].Boolean();
  if ( res >= 0 ) ReturnBoolean( res );
  ReturnHold;
}

DefinePDFunction(Cases,argv){Object&Argv=argv;
  /*
    ArgsN: 2,3
    Args1: List
   */
  CheckShouldBeWithin(2,3);
  CheckShouldBeList(1);
  int maxN = -1;
  if ( argv.Size() == 3 ){
    CheckShouldBeNumber(3);
    maxN = argv[3].Number();
  }
  Object matchedList(__List__);
  Object &list = argv[1];
  Object &pat = argv[2];
  //dout<<"deal with list ="<<list<<" pat ="<<pat<<endl;
  if ( pat.PairQ( SYMID_OF_Rule ) or pat.PairQ( SYMID_OF_RuleDelayed ) ){
    Pattern::UnifyRule( pat );
    Index idx = GlobalPool.Matches.New();
    vector<MatchRecord>&pairs = GlobalPool.Matches.Get( idx );

    for (auto iter=list.Begin(); iter != list.End(); iter ++ ){
      if ( maxN == 0 ) break;
      pairs.clear();
      if ( Pattern::UnifiedMatchQ(*iter,pat[1],pairs) ) { // apply change
        Object nobj = pat[2].Copy();
        Pattern::UnifiedApplyPatternPairs( nobj, pairs );
        EvaKernel->Evaluate( nobj );
        matchedList.PushBackRef( nobj );
        maxN --;
      }
    }
    GlobalPool.Matches.FreePairs( idx );
  }else{
    for (auto iter=list.Begin(); iter != list.End(); iter ++ ){
      if ( maxN == 0 ) break;
      //dout<<" test match "<<*iter <<"  ?Matched by pat "<< pat<<endl;
      if ( Pattern::MatchQ( *iter, pat ) ){
        matchedList.PushBackRef( *iter );
        maxN --;
      }
    }
  }

  Return(matchedList);
}
