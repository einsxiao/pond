#ifndef Gisp_ValueTable_H
#define Gisp_ValueTable_H

namespace pond{
  class ValueTable{
  public:
    friend class Evaluation;
  private:
#ifdef DEBUG
  public:
#endif
    void                        VeryInit();
    // ids of valued Symbol is equal to  pairTable[i].ids
    // pairTable[i].ids() == RecordTable addr
    Object                      stackTable;   //normal certain value table
    Object                      pairTable;    //normal certain value table
    Object                      patternTable; //rule list of patterns
    ////////////////////////////////////////////
  public:
    typedef std::vector<Object>::iterator iterator;
    ValueTable();
    ValueTable(const ValueTable &upValueTable);
    ValueTable(Object&pairT );
    ValueTable(Object&pairT, Object&patternT );
    ~ValueTable();
    ValueTable &operator=(const ValueTable&); 
    /////////////////////////////////////////////////
  private:
    bool     UpdateOrInsertPattern(const Object  &left,
                                   const Object  &right,
                                   bool     onlyUpdate=false,
                                   bool     tryDel=false );
    //Insert or update a value pair
    Object   UpdateOrInsertPairValue(const Object&left,
                                     const Object&right,
                                     const bool onlyUpdate=false,
                                     const bool isRef=false
                                     );

  public:
    // Ref is checked no pattern
    Object   RefInsertOrUpdatePairValue(const Object&left, const Object&right);
    //Insert or update a value pair
    Object   UpdateOrInsert(const Object&left,
                            const Object&right,
                            const bool   onlyUpdate=false,
                            const bool   isCheckedPattern=false
                            );
    /////////////////////////////////////////////////
    /////////////////////////////////////////////////
    //get the pair Object
    Object   GetOrNewValuePairRef(const  Object&left,
                                  const bool   onlyGet=false,
                                  const bool   isCheckedNoPattern=false
                                  );
    /////////////////////////////////////////////////
    // 0: nothing get 1: get self-equal value 2:get new value 
  private:
    int      GetValueFromPattern(const Object&left,  Object&right);

  public:
    int      GetValueRef(        const Object&left,  Object&right); 

    int      GetValue(           const Object&left,  Object&right,
                                 const bool onlyCheckPatternTable=false); 
    /////////////////////////////////////////////////
    // Push and Pop must pair each other
    Object   StackPushCopy(const Object&left,const Object&right);
    void     StackPushRef(const Object&pair);
    int      StackPop();
    /////////////////////////////////////////////////
    int      Clear(Object&list,Evaluation*Eva); // Clear the value

    int      ClearAll(Evaluation*Eva); // Clear all values
    /////////////////////////////////////////////////
  };


};
#endif
