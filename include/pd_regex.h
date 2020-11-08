#ifndef __POND_REGREX_H__
#define __POND_REGREX_H__

class Regex{
  regex_t pat;
  int code = -1;
  int groupNumber;
public:
  std::vector<regmatch_t> regs;
public:
  void Init(const std::string pat_str,int n=32,int mode = REG_EXTENDED );
  Regex(const std::string pat_str,int n=32,int mode = REG_EXTENDED ); // REG_ICASE REG_NOSUB REG_NEWLINE
  ~Regex();
  typedef std::vector<std::string> Matches;
  bool Match(const char*);
  bool Match(const std::string);
  bool Match(const char*,Matches&);
  bool Match(const std::string,Matches&);
};





#endif
