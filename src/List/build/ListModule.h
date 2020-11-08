#ifndef POND_H_List
#define POND_H_List
#ifndef ListModule_Eins_H
#define ListModule_Eins_H

namespace pond{
  DeclareModule(List){Constructors(List);
  public:
    /////////////////////////////////
    static int List(Object &);
    //Declare_ValueTablePair_Id_Of(List);
    static int range_iter(Object&,long &N,double &start,double&incr);
    static int Range(Object &);
    /////////////////////////////////
    static int Take(Object &);
    static int Drop(Object &);
    /////////////////////////////////
    static int Head(Object &);
    static int Size(Object &);
    static int First(Object &);
    static int Last(Object &);
    static int Most(Object &);
    static int Rest(Object &);
    /////////////////////////////////////////////
    /////////////////////////////////////////////
    static int NullQ(Object &);
    static int zh_NullQ(Object &);
    static int NumberQ(Object &);
    static int zh_NumberQ(Object &);
    static int StringQ(Object &);
    static int zh_StringQ(Object &);
    static int SymbolQ(Object &);
    static int zh_SymbolQ(Object &);
    static int ListQ(Object &);
    static int zh_ListQ(Object &);
    static int TrueQ(Object &);
    static int zh_TrueQ(Object &);
    int PD_RealQ(Object&);
    int PD_zh_RealQ(Object&);
    int PD_IntegerQ(Object&);
    int PD_zh_IntegerQ(Object&);
    int PD_ComplexQ(Object&);
    int PD_zh_ComplexQ(Object&);
    int PD_EmptyQ(Object&);
    int PD_曰空(Object&);
    int PD_HeadQ(Object&);
    int PD_Length(Object&);
    int PD_Boolean(Object&);
    /////////////////////////////////////////////
    static int Map(Object &);
    static int Apply(Object &);
    static int Scan(Object &);
    static int Nest(Object &);
    static int NestList(Object &);
    static int NestWhile(Object &);
    /////////////////////////////////////////////
    static int Reverse(Object &);
    static int Transpose(Object &);
    /////////////////////////////////////////////
    static int GetPartList( Object   &valueList,
                            Object   &partList,
                            u_int     pos,
                            Object   &resultList,
                            bool      isReal=false
                            );
    static int Part(Object &);
    static int SetPart(Object&left,Object&right);
    static int SetPart(Object&);
    static int Append(Object &);
    static int AppendTo(Object &);
    static int Insert(Object &);
    static int InsertTo(Object &);
    static int Delete(Object &);
    static int DeleteFrom(Object &);
    static int PopBack(Object &);
    /////////////////////////////////////////////
    static int Table(Object &);
    static int Band(Object &);
    /////////////////////////////////////////////
    static int Sort(Object &);
    static int Flatten(Object &);
    static int Join(Object &);
    static int Dimensions(Object &);
    /////////////////////////////////////////////
    //////////////////////////////////////////
    static int Dict(Object&);
    int PD_Cases(Object&);
  };
};


#endif

#endif