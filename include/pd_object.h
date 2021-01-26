#ifndef Objectect_POND_H
#define Objectect_POND_H
namespace pond{
  enum struct ObjectType:unsigned char{Symbol, Number, String, List}; //reference is v.idx not 0
  #define __Symbol__ ObjectType::Symbol
  #define __Number__ ObjectType::Number
  #define __String__ ObjectType::String
  #define __List__   ObjectType::List
  typedef int (*ObjectCompareFunction)(const Object&,const Object&);
  struct ObjectCompareFunctionLess;

  union _Object_Combine_Type{
    Index                   _idx;
    double                  _re;
  };

  class _Object{ 
  public:
    friend class Object;
    friend class __EvaObjectTable;
    friend class __EvaListTable;
    ObjectType            type;        // 1B  255               type
    u_char                state;       // 1B  255               state
    u_char                cnt;         // 2B  65535             reference count
    u_int                 ids;         // 4B  4294967295        symbol id or evarecord
    _Object_Combine_Type  val;         // 8B  4B*4B             total 16B
    _Object()=default;
    ~_Object()=default;
    _Object &operator=(const _Object&obj){
      type        = obj.type;
      state       = obj.state;
      ids         = obj.ids;
      val._idx    = obj.val._idx;
      return *this;
    }
    void           init(){
      type        = __Symbol__;
      state       = 0;
      cnt         = 0;
      ids         = 0;
      val._idx.zero();
    }
    void           clear(){
      type        = __Symbol__;
      state       = 0;
      ids         = 0;
      val._idx.zero();
    };
    ///////////////////////////////////
  };
};
#include"pd_memory_manage.h"
#define _obj_at_objid GlobalPool.Objects.objs[objid.row][objid.col ]
namespace pond{
  class Object{//need not to be the pod type, for the convenions we get
  public:
    Index objid;
  private:
    void Malloc();
    void Clear();
    void Free();
    inline void cnt_incr(){
      //dout<<objid<<" ref incr"<<std::endl;
      _obj_at_objid.cnt++;
    };
    inline void cnt_decr(){
      //dout<<objid<<" ref decr"<<std::endl;
      _obj_at_objid.cnt--;
    };
  public:
    Object &operator=(const Object&obj);
    Object  Copy()const; // copy self as a new object and return
    ///////////////////////////////////////////////////////////////////////
    inline Object()   { objid.zero(); };
    inline ~Object()  { Free();       };
    ///////////////////////////////////////////////////////////////////////
    typedef std::vector<Object>::iterator iterator;
    inline Object(const Object&obj){          objid = obj.objid; if ( nonvoidQ() ){cnt_incr();}};
    inline explicit Object(const Index id){   objid = id;        if ( nonvoidQ() ){cnt_incr();}}; 
    ///////////////////////////////////////////////////////////////////////
    Object(const ObjectType type, const u_int);
    Object(const ObjectType type, const int=0);
    Object(const ObjectType type, const double);
    Object(const ObjectType type, const char *);
    Object(const ObjectType type, const std::string&);
    inline ObjectType   type()const{         return  _obj_at_objid.type;               };
    inline void         set_type(ObjectType v){      _obj_at_objid.type=v;             };
    inline u_char       state()const{        return  _obj_at_objid.state;              };
    inline void         set_state(u_char v){         _obj_at_objid.state=v;            };
    inline u_short      cnt()const{          return  _obj_at_objid.cnt;                };
    inline void         set_ids(u_int v){            _obj_at_objid.ids = v;            };
    inline u_int        ids()const{          return  _obj_at_objid.ids;                };
    inline void         set_idx(Index val){          _obj_at_objid.val._idx = val;     };
    inline void         zero_idx(){                  _obj_at_objid.val._idx.zero();    };
    inline Index        idx()const{          return  _obj_at_objid.val._idx;           };
    inline void         set_re(double val){          _obj_at_objid.val._re = val;      };
    inline double       re()const{           return  _obj_at_objid.val._re;            };
#undef _obj_at_objid
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    inline Object&      SetVoid(){Free(); return (*this);}
    inline Object&      SetNull(){Clear(); return (*this);}
    Object&             CopyObject(const Object&,bool keepRef=false); 
    /* Object&             SetObjectRef(const Object&); */
    Object&             SetNumber(const double);
    Object&             SetNumber(const complex);
    Object&             SetString(const char*);
    Object&             SetString(const std::string&);
    ////////////////
    Object&             SetSymbol(const u_int);
    Object&             SetSymbol(const char*);
    Object&             SetBoolean(const bool); // c++ bool to Symmbol True or False
    ////////////////
    Object&             ReserveSize(const u_int);
    Object&             SetList(const u_int id=SYMID_OF_List);
    Object&             SetList(const char*);
    Object&             SetPairCopy(const Object&first,const Object&second,u_int headSymId=SYMID_OF_Rule);
    Object&             SetPairRef(const Object&first,const Object&second,u_int headSymId=SYMID_OF_Rule);
    ////////////////////////////////////////////////////////
    std::string         ToRawString()const;
    std::string         TypeString()const;
    ////////////////////////////////////////////////////////
    inline bool         voidQ()const{      return objid.zeroQ();     };
    inline bool         nonvoidQ()const{   return objid.nonzeroQ();  };
    inline bool         SameRefQ(const Object&obj)const{ return objid == obj.objid; };
    bool                NullQ()const; // object ponter is null or has a null value empty string or null list
    bool                AtomQ()const;
    bool                TypeQ(const ObjectType)const;
    bool                SymbolQ()const;
    bool                SymbolQ(const char*)const;
    bool                SymbolQ(const u_int)const;
    // the so called valuedsymbol is pointed to valuepair idx in valuetable
    bool                ValuedSymbolQ()const; 
    bool                NumberQ()const;
    bool                NumberQ(const double)const;
    bool                NumberQ(const complex)const;
    bool                RealQ()const;
    bool                RealQ(const double)const;
    bool                ComplexQ()const;
    bool                ComplexQ(const complex)const;
    bool                IntegerQ()const;
    bool                StringQ()const;
    bool                StringQ(const char*)const;
    bool                StringQ(const Index)const;
    bool                ListQ()const;
    bool                ListQ(const char*)const;
    bool                ListQ(const u_int)const;
    bool                ListQ(const Object&)const;
    bool                RuleQ()const;
    bool                PairQ()const;
    bool                PairQ(const u_int)const;
    bool                PairQ(const Object&)const;
    bool                SimpleListQ()const;
    bool                SimpleListQ(const char*)const;
    bool                SimpleListQ(const u_int)const;
    bool                HeadQ(const char *)const;
    bool                HeadQ(const Object&)const;
    ////////////////////////////////////////////////////////
    const char*         Head()const;
    ////////////////////////////////////////////////////////
    const char*         Key()const;//only String and Symbol has key
    u_int               SymbolId()const;//return 0 if no Id
    u_int               SymbolWeight()const;//only Symbol applies
    ////////////////////////////////////////////////////////
    double              &ref_double();
    std::string         &ref_string();
    std::vector<Object> &ref_list();
    ////////////////////////////////////////////////////////
    char                Boolean()const;
    double              Number()const;
    complex             Complex()const;
    const char*         String()const;//only apply to String
    explicit operator   bool()const{              return Boolean();};
    explicit operator   int()const{               return (int)Number();};
    explicit operator   float()const{             return Number();};
    explicit operator   double()const{            return Number();};
    explicit operator   complex()const{           return Complex();};
    explicit operator   floatcomplex()const{      return (floatcomplex)Complex();};
    explicit operator   std::string()const{       return(type()==ObjectType::String)?Key():ToString();};
    ////////////////////////////////////////////////////////
    static int          Dimensions(const Object&list,Object&dim,int n=-1);
    Object 	            &ElementAt(const u_int id){
      return GlobalPool.Lists.objs[ idx().row ][ idx().col][ id ];
    };
    Object 	            &_ElementAt(const u_int id)const{
      return GlobalPool.Lists.objs[ idx().row ][ idx().col][ id ];
    };
    inline Object 	    &operator[](const u_int id){      return ElementAt(id);  };
    inline Object 	    &operator()(const u_int id)const{ return _ElementAt(id); };
    int            	    ElementsSize()const;
    int                 Size()const;
    Object 	            &First();
    Object 	            &Last();
    Object              &Back();
    Object              RefObject();
    ///////////////////////////////////////////////////////////////
    iterator            Begin()const;
    iterator            End()const;
    iterator            ElementsBegin()const;
    iterator            ElementsEnd()const;
    u_int               PositionOfIterator(const iterator iter)const;
    inline bool         Empty()const{ if ( (not ListQ() )  or ( Size() == 0) ) return true; else return false; };
    /////////////////////////////////////////////////////////////////
    Object              &Swap(Object&);
    Object              &SwapRef(Object&);
    Object              &Flatten();
    Object              &FlattenSequence();
    Object              &FlattenListAndSequence();
    Object              &ForceDeepen();// make current list to be head 
    Object              &Deepen();// make current list to be head 
    Object              &DeepenAsFirst();// current list as first child head be null
    Object              &ToList();// make current list to be head 
    Object              &PushBackCopy(const Object&);
    Object              &PushBackRef(const Object&);
    Object              &PushBackNumber(const double);
    Object              &PushBackNumber(const complex);
    Object              &PushBackSymbol(const char*key);
    Object              &PushBackSymbol(u_int symbolId);
    Object              &PushBackString(const char*key);
    Object              &PushBackString(const std::string&key);
    Object              &PushBackList(const char*key);
    Object              &PushBackList(u_int symbolId);
    Object              &PushBackNull();
    /////////////
    Object              &InsertCopy(u_int pos,       const Object &obj);
    Object              &InsertCopy(iterator iter,   const Object &obj);
    Object              &InsertRef(u_int pos,        const Object &obj);
    Object              &InsertRef(iterator iter,    const Object &obj);
    Object              &InsertRef(iterator pos,iterator spos, iterator epos);
    Object              &InsertRef(u_int    pos,iterator spos, iterator epos);
    Object              &InsertNumber(u_int pos,double val);
    Object              &InsertSymbol(u_int,char*key);
    Object              &InsertSymbol(u_int,u_int keyid);
    Object              &InsertString(u_int,char*key);
    Object              &InsertNull(u_int);
    /////////////
    Object              &PopBack();
    Object              &Delete(u_int pos);
    iterator             Delete(iterator iter);
    Object              &Delete(iterator spos, iterator epos);
    Object              &DeleteElements();
    ////////////////////////////////////////////////////////////////
    std::string	        ToFullFormString()const;
    std::string         ToString(const Object&parentList)const;
    std::string         ToString()const;
    ////////////////////////////////////////////////////////////////
    Object              &LoadFromJson(const std::string);
    std::string         DumpToJson(bool isLeft=false)const;
    ////////////////////////////////////////////////////////////////
    static int 	        SimpleCompare(const Object&l1,const Object&l2) ; 
    static int 	        ExpressionCompare(const Object&l1,const Object&l2) ; 
    Object              &Sort(const ObjectCompareFunction lcf=Object::ExpressionCompare);
    Object              &SimpleSort();
    Object              &ExpressionSort();
    Object              &Reverse();
    ////////////////////////////////////////////////////////////////
    bool		            operator<(const Object&other)const;
    bool		            operator==(const Object&other)const;
    bool		            operator!=(const Object&other)const;
    ////////////////////////////////////////////////////////////////
    int                 DictGetPosition(const Object&left_value,iterator &iter_pos); // return res, update iter
    Object              DictGetPair(const Object&left_value);  // return value pair
    Object              DictGetOrNewPair(const Object&left_value); // value pair
    Object              DictValueAt(const Object&left_value);  // return right value
    bool                DictInsertPairRef(Object&pair);
    bool                DictInsertOrUpdatePairRef(Object&pair);
    Object              DictPop(const Object&left_value);
    bool                DictDelete(const Object&left_value);
    Object&             DictSort();
    //////////////////////////////////////////
    /* int                 OrderListGetPosition(Object&value,iterator&iter,ObjectCompareFunction lcf=Object::SimpleCompare); */
    /* bool                OrderListGetEqualRange(Object&value,iterator&s_iter,iterator&e_iter, ObjectCompareFunction lcf = Object::SimpleCompare); */
    /* //insert only the value */
    /* bool                OrderListInsert(Object&,ObjectCompareFunction lcf=Object::SimpleCompare); */
    /* bool                OrderListInsertNoDumplicate(Object&value,ObjectCompareFunction lcf = SimpleCompare); */
    /* bool                OrderListDeleteValue(Object&,ObjectCompareFunction lcf = SimpleCompare); */
    /* bool                OrderListDeleteAllValue(Object&,ObjectCompareFunction lcf = SimpleCompare); */
    /* void                OrderListSort(ObjectCompareFunction lcf=SimpleCompare); */
  };
  const Object NullObject( Index(0) );
  const Object Null( Index(0) );

  template<class ostype> ostype &operator<<(ostype &os,const Object&obj){
    os<<obj.ToString();
    return os;
  };

  struct ObjectCompareFunctionLess{
    ObjectCompareFunction LCF;
    inline ObjectCompareFunctionLess(ObjectCompareFunction lcf){LCF = lcf;}
    inline bool operator()(Object l1,Object l2){return LCF(l1,l2)<0;}
  };

};
#endif
