#include"pd_headers.h"

using namespace std;
using namespace pond;

void Regex::Init(const string pat_str,int n,int mode)
{
  groupNumber = n;
  regs.resize( groupNumber + 1 );
  if ( ( code = regcomp( &pat, pat_str.c_str() , mode) ) != 0  ) {
    switch ( code ){
    case REG_NOMATCH: { _Erroring("Regex::pattern"," regexec() failed to match."); return; }
    case REG_BADPAT: { _Erroring("Regex::pattern"," Invalid regular expression."); return; }
    case REG_ECOLLATE: { _Erroring("Regex::pattern"," Invalid collating element referenced."); return; }
    case REG_ECTYPE: { _Erroring("Regex::pattern"," Invalid character class type referenced."); return; }
    case REG_EESCAPE: { _Erroring("Regex::pattern"," Trailing '\' in pattern."); return; }
    case REG_ESUBREG : { _Erroring("Regex::pattern"," Number in \"\\digit\" invalid or in error. "); return; }
    case REG_EBRACK : { _Erroring("Regex::pattern"," \"[]\" imbalance. "); return; }
    case REG_EPAREN : { _Erroring("Regex::pattern"," \"\\(\\)\" or \"()\" imbalance."); return; }
    case REG_EBRACE : { _Erroring("Regex::pattern"," \"\\{\\}\" imbalance. "); return; }
    case REG_BADBR : { _Erroring("Regex::pattern"," Content of \"\\{\\}\" invalid: not a number, number too large, more than two numbers, first larger than second. "); return; }
    case REG_ERANGE : { _Erroring("Regex::pattern"," Invalid endpoint in range expression. "); return; }
    case REG_ESPACE : { _Erroring("Regex::pattern"," Out of memory. "); return; }
    case REG_BADRPT : { _Erroring("Regex::pattern"," '?', '*', or '+' not preceded by valid regular expression. "); return; }
    default: { _Erroring("Regex::pattern","Internal error."); return; }
    }
  }
}

Regex::Regex(const string pat_str,int n,int mode){
  Init(pat_str,n,mode);
}

Regex::~Regex()
{
}

bool Regex::Match(const char*pat_str){
  if ( regexec( &pat, pat_str, 0, NULL, 0 ) == 0 ){
    return true;
  }else{
    return false;
  }
}

bool Regex::Match(const string pat_str){
  return Match(pat_str.c_str());
}

bool Regex::Match(const char*pat_str,Matches&mats){
  if ( regexec( &pat, pat_str, groupNumber+1, &regs[0], 0 ) == 0 ){
    mats.reserve( groupNumber + 1 );
    mats.clear();
    for ( int i=0;i<=groupNumber; i++ ){
      if ( regs[i].rm_so  >= 0 )
        mats.push_back( string(pat_str, regs[i].rm_so, regs[i].rm_eo - regs[i].rm_so ) );
      else
        break;
    }
    return true;
  }else{
    mats.clear();
    return false;
  }
}

bool Regex::Match(const string pat_str,Matches&mats){
  regs.resize( groupNumber + 1 );
  if ( regexec( &pat, pat_str.c_str(), groupNumber+1, &regs[0], 0 ) == 0 ){
    mats.resize( groupNumber + 1 );
    for ( int i=0;i<=groupNumber; i++ ){
      mats[i] = pat_str.substr( regs[i].rm_so, regs[i].rm_eo - regs[i].rm_so );
    }
    return true;
  }else{
    return false;
  }
}
