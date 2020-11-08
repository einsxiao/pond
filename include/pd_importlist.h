#ifndef Gisp_Reading_H
#define Gisp_Reading_H

namespace pond{

  //class Evaluation;

  class ImportList{
  public:
    ImportList(std::istream *istr_in,bool interactively);
    ImportList(std::string buffer);
    ~ImportList();
    std::istream	 *istr;
    std::string 	 buffer;
    bool	         interactively;
    int            lineNumber=1;
    int 	         pos;
    std::string    parens=" ";
    bool inputEnd;
    //////////////////////////////////////////////
    int 	         GetList(Object&Argv,int depth=0,bool detectMode=false);//begin with 0
    //////////////////////////////////////////////
    static Object  ToExpression(std::string);
    //////////////////////////////////////////////
    //////////////////////////////////////////////
    bool	         End(); 
    bool	         getChar(char &ch); 
    bool 	         getChar(char &ch,int n);// won't move buffer pointer
    char           getRecentNonSpace();
    bool           getRecentNumberString(std::string&,char ch=' ');
    /* std::string    getRecentOperWord(); */
    //////////////////////////////////////////////
    bool	         rollBack(int n);
    //////////////////////////////////////////////
    //static bool isSpecialCharArray[255+1];
  };
};

#endif
