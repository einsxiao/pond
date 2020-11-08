#ifndef EvaStatic_POND_H
#define EvaStatic_POND_H

namespace pond{

  const int __EvaMaxWaitTime = 5000;
  class __EvaSymbolTable{//will not free
    struct positionType{
      u_int pos;
      char* key;
      positionType(char*_k,u_int _p){key = _k; pos = _p;};
    };
    struct recordType{
      // priority left right, operator type, operatant number
      u_char pL,pR, oType, oNum;
      // weight is the alphabetic order among all keys, to accelerate symbol comparision
      u_int  weight;           
      char*  key;
      //  + -  /@  ->  are dealed by importlist not in mech, but the priority are the same

      // otype: 0-> normal symbols                    default  this

      //        1-> right extending functions         if( x ) EXT_EXPR       n = 2 
      //                                              for (x) ...            n = 2
      //                                              while (x) ...          n = 2
      //                                              switch ( v ) EXT_EXPR  n = 2
      //                                              import 'xxx'   
      //                                              import 'xxx','yyy'     n  = 1
      //                                              case v1,v2 : aa        n = 1
      //                                              function(...){ }       n = 2
      //                                              class name {}          n = 2
      //                                              new expr               n = 1
      //                                              new expr               n = 1
      //                                              break                  n = 0
      //                                              continue               n = 0

      //        3-> left connect right extending      exprl else  expr2 expr..   n = 1
      //                                              expr1 elif (x)  expr2  expr...   n = 2
      //                                              import x from y        n = 1

      // so  for ( ) {} is case 1
      //     while ( ) {} is case 1
      //     
      recordType(char*_k,u_int _w,u_char _l,u_char _r,u_char t=0,u_char n=0){
        key = _k; weight = _w; pL = _l; pR = _r; oType = t, oNum = n;
      };
    }; //key -> pos_key
    //bool locked;
    /* std::mutex mt; */
    std::vector< positionType > positions;// 
    std::vector< recordType > records; //position to symbol weight and content 
    u_int ptr;//total amount of symbols in SymbolTable
  public:
    __EvaSymbolTable();
    ~__EvaSymbolTable();
    u_int GetOrNew(const char*,u_char priLeft=0,u_char priRight=0,u_char type=0,u_char num=0);
    inline u_int   GetId(const char*key){      return GetOrNew(key); };
    inline char*   GetKey(u_int id){           return records[id].key; };
    inline u_int   GetWeight(u_int id){        return records[id].weight; };
    inline u_char  GetPriorityLeft(u_int id){  return records[id].pL; };
    inline u_char  GetPriorityRight(u_int id){ return records[id].pR; };
    inline u_char  GetOperType(u_int id){      return records[id].oType; };
    inline u_char  GetOperNum(u_int id){       return records[id].oNum; };
    void           GetRecord(u_int id,u_char&pL,u_char&pR,u_char&oT,u_char&oN){
      pL = records[id].pL;
      pR = records[id].pR;
      oT = records[id].oType;
      oN = records[id].oNum;
    }
    inline u_int   TableSize(){                return records.size(); };
  };

  ////////////////////////////////////////////
#ifndef __MEMORY_POOL_ROW_SIZE
#  define __MEMORY_POOL_ROW_SIZE 65530
#endif
  template<class type>
    class __EvaTable{
  public:
    std::string tableName;
    /* std::mutex mt; */
    u_int RowSize;
    Index idx;
    std::vector< Index > freeObjs; //v_second row_idx
    std::vector< type* > objs; //first one is refed counter
    __EvaTable(int inSize = __MEMORY_POOL_ROW_SIZE){
      RowSize = __MEMORY_POOL_ROW_SIZE;
      if ( inSize >= 5 )
        RowSize = inSize; 
      else
        RowSize = 5; 
      type* row = new type[ RowSize +1 ];
      objs.push_back(row);
      idx.zero();
    };
    virtual  ~__EvaTable(){
      for ( auto iter = objs.begin(); iter!=objs.end(); iter++){
        delete [](*iter);
      }
      objs.clear();
    };
    type &Get(const Index v){
      /* if ( idx < v ){ */
      /*   Erroring("Pool","Try get elements out of range."); */
      /*   return objs[0][0]; */
      /* } */
      return objs[ v.i ][ v.j ];
    }
    Index New(){
      //waitAndLock();
      //if ( mt.try_lock_for( std::chrono::milliseconds( __EvaMaxWaitTime ) ) ){
      // guard the lock
      /* std::lock_guard<std::mutex> lock(mt); */
      if ( freeObjs.size() > 0 ){
        Index res = freeObjs.back() ;
        freeObjs.pop_back();
        return res;
      }else{
        if ( idx.j == RowSize ){
          type* row = new type[ RowSize+1 ];
          objs.push_back(row);
          idx.i ++; 
          idx.j = 0;
          return idx;
        }else{
          idx.j ++;
          return idx;
        }
      };

    };
    inline void Free(Index id){
      /* std::lock_guard<std::mutex> lock(mt); */
      if ( id.nonzeroQ() ){
        freeObjs.push_back(id);
      }
    };
  };

  ///////////////////////////////////////////////////////////////////////////////
  class __EvaStringTable:public __EvaTable<std::string>{
  public:
    __EvaStringTable(){
      tableName = "Strings";
    };
    Index NewString(const char*);
    Index NewString(Index);
  };
  class __EvaObjectTable:public __EvaTable<_Object>{
  public:
    __EvaObjectTable(){
      tableName = "Objects";
    };
  };
  /////////////////////
  class __EvaListTable:public __EvaTable< std::vector<Object> >{
  public:
    __EvaListTable(){
      tableName = "Lists";
    };
    Index NewList(const Index);//new from other List by Value
    void  FreeList(Index);
    void  SetList(Index left, Index right);
  private:
    void Free(Index);
  };

  class MatchRecord{
  public:
    u_int n;
    u_int p;// when n >1 p is the position or first element
    Index idx;//idx of Object(n==1) or List(n>1)
  public:
    MatchRecord(){
      n       = 0;
      p       = 0;
      idx.zero() ;
    }
    MatchRecord(Index i){
      n = 1;
      p=0;
      idx = i;
    }
    MatchRecord(Index i, int n,int p){
      this->n = n;
      this->p = p;
      this->idx = i;
    }
  };

  class __EvaMatchTable:public __EvaTable< std::vector<MatchRecord> >{
  public:
    __EvaMatchTable(){
      tableName = "PatternMatchPairs";
    };
    void  FreePairs(Index id);
  private:
    void Free();
  };

  template<class type>
  class __EvaMatrixTable
  {
    std::map<std::string,Matrix_T<type>*> matrixTable; 
  public:
    __EvaMatrixTable()
    {
    };
    ~__EvaMatrixTable()
    {
      typename std::map<std::string,Matrix_T<type>* >::iterator iter;
      for ( iter = matrixTable.begin(); iter != matrixTable.end(); iter++ ){
        delete iter->second;
        iter->second = NULL;
      }
    };
    Matrix_T<type>   *GetMatrix(std::string name)
    {
      typename std::map<std::string,Matrix_T<type>* >::iterator iter;
      iter = matrixTable.find(name);
      if ( iter != matrixTable.end() ){
        return iter->second;
      }
      return NULL;
    };
    Matrix_T<type>   *GetOrNewMatrix(std::string name)
    {
      Matrix_T<type> **matPP = &matrixTable[name];
      if ( *matPP == NULL )
        *matPP = new Matrix_T<type>();
      return *matPP;
    };
    int RemoveMatrix(std::string name)
    {
      typename std::map<std::string,Matrix_T<type>* >::iterator iter;
      iter = matrixTable.find(name);
      if ( iter != matrixTable.end() ){
        delete iter->second;
        matrixTable.erase(iter);
        return 1;
      }
      return 0;
    };
    Matrix_T<type>   &operator()(std::string id)
    {
      return *GetMatrix(id);
    };
    Matrix_T<type>   &operator[](std::string id)
    {
      return *GetOrNewMatrix(id);
    };
  };

  class __EvaOptimizeRecord:public __EvaTable< std::pair<double,std::string> >{};

  class EvaMemoryPool{
  public:
    __EvaSymbolTable    Symbols;
    __EvaStringTable    Strings;
    __EvaListTable      Lists; 
    __EvaObjectTable    Objects;
    __EvaMatchTable     Matches;
    __EvaOptimizeRecord OpRec;
    __EvaMatrixTable<double>       Matrices;
    __EvaMatrixTable<complex>      ComplexMatrices;
    __EvaMatrixTable<float>        FloatMatrices;
    __EvaMatrixTable<floatcomplex> FloatComplexMatrices;
  public:
    EvaMemoryPool();
    ~EvaMemoryPool();
  public:
    Evaluation         *Eva;
    int                 flag;
  };

  /* extern EvaMemoryPool  *__GlobalPoolPtr; */

  template<int id> class __GlobalPool{
  public:
    static EvaMemoryPool pool;
  };
  template<int id> EvaMemoryPool __GlobalPool<id>::pool;
#define GlobalPool (__GlobalPool<0>::pool)
#define EvaKernel  (__GlobalPool<0>::pool.Eva)

  inline u_int SymbolIdOf(char*key){
    return GlobalPool.Symbols.GetOrNew( key );
  };

    
#ifdef DEBUG
  inline int __Init_Clock_(Index &__clock_ID,std::string file, std::string function){
    return 0;
  }
#  define clockStart static Index __clock_ID;                           \
  if ( __clock_ID == Index0 ){                                          \
    __clock_ID = GlobalPool.OpRec.New();                                \
    GlobalPool.OpRec.Get( __clock_ID ).first = 0;                       \
    GlobalPool.OpRec.Get( __clock_ID ).second = (std::string)__FILE__+":\t"+__FUNCTION__; \
  }                                                                     \
  clock_t __start_T = clock()
#  define clockEnd GlobalPool.OpRec.Get( __clock_ID ).first+=(1.0*clock() - __start_T)/CLOCKS_PER_SEC;
#else
#  define clockStart 
#  define clockEnd 
#endif

#define INIT_SYMID_OF(sym)                                  \
  u_int SYMID_OF_##sym = GlobalPool.Symbols.GetOrNew(#sym)
#define INIT_OPER_SYMID_OF(sym, pL, pR, oT, oN)                         \
  u_int SYMID_OF_##sym = GlobalPool.Symbols.GetOrNew(#sym,pL,pR,oT,oN)

#define INIT_OPER(sym, pL, pR, oT, oN)          \
  GlobalPool.Symbols.GetOrNew(sym,pL,pR,oT,oN)

#define _DS(sym,id) const u_int SYMID_OF_##sym = id
  _DS(Null,                  0);
  _DS(True,                  1);
  _DS(False,                 2);
  _DS(Slot,                  3);
  _DS(ArgsFollow,            4);
  _DS(PartArgsFollow,        5);
  _DS(ParenthesisFollow,     6);
  _DS(Prefix,                7);
  _DS(PrefixIn,              8);
  _DS(Black,                 9);
  _DS(BlackSequence,         10);
  _DS(BlackNullSequence,     11);
  _DS(Pattern,               12);
  _DS(Get,                   13);
  _DS(Increment,             14);
  _DS(PreIncrement,          15);
  _DS(Decrement,             16);
  _DS(PreDecrement,          17);
  _DS(Factorial,             18);
  _DS(Power,                 19);
  _DS(Reciprocal,            20);
  _DS(Times,                 21);
  _DS(Divide,                22);
  _DS(Mod,                   23);
  _DS(Dot,                   24);
  _DS(Plus,                  25);
  _DS(Subtract,              26);
  _DS(Greater,               27);
  _DS(Less,                  28);
  _DS(GreaterEqual,          29);
  _DS(LessEqual,             30);
  _DS(Equal,                 31);
  _DS(UnEqual,               32);
  _DS(Not,                   33);
  _DS(And,                   34);
  _DS(Or,                    35);
  _DS(Alternatives,          36);
  _DS(PureFunction,          37);
  _DS(Rule,                  38);
  _DS(RuleDelayed,           39);
  _DS(ReplaceAll,            40);
  _DS(ReplaceAllRepeated,    41);
  _DS(Postfix,               42);
  _DS(Set,                   43);
  _DS(SetDelayed,            44);
  _DS(AddTo,                 45);
  _DS(TimesBy,               46);
  _DS(SubtractFrom,          47);
  _DS(DivideBy,              48);
  _DS(PowerWith,             49);
  _DS(ModBy,                 50);
  _DS(Put,                   51);
  _DS(CompoundExpression,    52);
  _DS(Seperator,             53);
  _DS(Span,                  54);
  _DS(Help,                  55);
  _DS(Conjunct,              56);
  _DS(Apply,                 57);
  _DS(Map,                   58);
  _DS(__Variable,            59);
  _DS(SerialCode,            60);
  _DS(FunctionVariable,      61);
  _DS(KeyValuePair,          62);
  _DS(if,                    63);
  _DS(else,                  64);
  _DS(elif,                  65);
  _DS(while,                 66);
  _DS(for,                   67);
  _DS(do,                    68);
  _DS(switch,                69);
  _DS(case,                  70);
  _DS(import,                71);
  _DS(from,                  72);
  _DS(and,                   73);
  _DS(or,                    74);
  _DS(not,                   75);
  _DS(in,                    76);
  _DS(var,                   77);
  _DS(global,                78);
  _DS(continue,              79);
  _DS(break,                 80);
  _DS(return,                81);
  _DS(function,              82);
  _DS(class,                 83);
  _DS(new,                   84);
  _DS(del,                   85);
  _DS(extends,               86);
  _DS(Part,                  87);
  _DS(Parenthesis,           88);
  _DS(Complex,               89);
  _DS($START$,               90);
  _DS($END$,                 91);
  _DS(HoldPattern,           92);
  _DS(Hold,                  93);
  _DS(HoldOnce,              94);
  _DS(Evaluate,              95);
  _DS(Exit,                  96);
  _DS(List,                  97);
  _DS(Dict,                  98);
  _DS(Sequence,              99);
  _DS(Undefined,             100);             
  _DS(Unknown,               101);             
  _DS(default,               102);             
  _DS(ExpressionList,        103);             
  _DS(Unpack,                104);             
  _DS(UnpackDict,            105);             
  _DS(Query,                 106);
  _DS(LineBreak,             107);       // a special one
  _DS(ArrowFunction,         108);
#undef _DS



};


#endif
