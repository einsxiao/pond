#ifndef Gisp_Pattern_H
#define Gisp_Pattern_H

namespace pond{
  const int MatchLess = -2;
  const int MatchLeftByRight= -1;
  const int MatchEqual = 0;
  const int MatchRightByLeft = 1;
  const int MatchGreater = 2;
  

  class Pattern{
  public:
    Pattern();
    ~Pattern();
    // replace pattern.second to get the replaced pattern.second
    // pair is from matchqq function
    /////////////////////////////////////////////////
    ////////////////////////////////////////
    //used between unified patterns
    //static int        UnifiedMatchCompare(Object &pat1,Object &pat2); 
    static int        PatternCompare(const Object&pat1, const Object&pat2); 
    static int        UnifyRuleLeft(Object&left);
    static int        UnifyRule(Object&left,Object&right);
    static int        UnifyRule(Object&rule);
    static int        UnifiedMatchQ(const Object&expr,const Object&pattern);
    static int        UnifiedMatchQ(const Object&expr,const Object&pattern,std::vector<MatchRecord>&pairs);
    static void       UnifiedApplyPatternPairs(Object&Argv,std::vector<MatchRecord>&pairs);
    static int        UnifiedRawReplaceAll(Object&Argv,Object&rule,bool repeated);
    static int        UnifiedReplaceAll(Object&expr, Object&rule,bool repeated);
    ////////////////////////////////////////////////////////////////////
    static int        Compare(const Object&pat1, const Object&pat2,bool matched = false);
    static bool       MatchQ(const Object&Argv, const Object&pattern); //well done
    // static int        ReplaceAll(Object&Argv,Object&rule,bool repeated = true);
    ////////////////////////////////////////
    static bool       RuleQ(const Object&Argv);
    static bool       PatternListQ(const Object&list);
  };
};
#endif
