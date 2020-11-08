#ifndef OUTPUT_POND_H
#define OUTPUT_POND_H


namespace pond{

  /* class DocOut{ */
  /* public: */
  /*   template<class type> */
  /*   DocOut&operator<<(type val){ */

  /*     /\* use to string *\/ */
  /*     /\* pond::PostMessage( pond::ToString(val)+"print" ); *\/ */

  /*     /\* use cout *\/ */
  /*     std::stringstream  ss; */
  /*     auto *old = std::cout.rdbuf( ss.rdbuf() ); */
  /*     std::cout<<val; */
  /*     std::cout.rdbuf(old); */
  /*     /\* pond::PostMessage( ss.str()+"print" ); *\/ */

  /*     return (*this); */
  /*   } */
  /*   DocOut&operator<<(Object val){ */
  /*     /\* pond::PostMessage( val.ToString() +"print" ); *\/ */
  /*     return (*this); */
  /*   } */

  /*   typedef DocOut& (*manipulator)(DocOut&); */

  /*   DocOut&operator<<(manipulator manip){ */
  /*     return manip(*this); */
  /*   } */

  /*   template<typename ... Args> void dprintf(const std::string& format, Args ... args){ */
  /*     size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0' */
  /*     if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); } */
  /*     std::unique_ptr<char[]> buf( new char[ size ] ); */
  /*     snprintf( buf.get(), size, format.c_str(), args ... ); */
  /*     /\* pond::PostMessage( std::string( buf.get(), buf.get() + size - 1 )+"print" ); *\/ */
  /*     // don't let the '\0' inside */
  /*   } */

  /* }; */

  /* class DocErr{ */
  /* public: */
  /*   template<class type> */
  /*   DocErr&operator<<(type val){ */
  /*     /\* pond::PostMessage( pond::ToString(val)+"error" ); *\/ */
  /*     return (*this); */
  /*   } */

  /*   DocErr&operator<<(Object val){ */
  /*     /\* pond::PostMessage( val.ToString() +"error" ); *\/ */
  /*     return (*this); */
  /*   } */

  /*   typedef DocErr& (*manipulator)(DocErr&); */

  /*   DocErr&operator<<(manipulator manip){ */
  /*     return manip(*this); */
  /*   } */
  /* }; */

  /* class DocIn{ */
  /* public: */
  /*   std::istringstream in_ss; */
  /* public: */
  /*   template<class type> DocIn&operator>>(type&val){ */
  /*     in_ss >> val; */
  /*     return (*this); */
  /*   } */

  /*   typedef DocIn& (*manipulator)(DocIn&); */

  /*   DocIn&operator>>(manipulator manip){ */
  /*     return manip(*this); */
  /*   } */

  /*   template<typename ... Args> void dscanf(const std::string& format, Args ... args){ */
  /*     std::string content = in_ss.str(); int p = in_ss.tellg(); int pn = p; */
  /*     while ( content[pn] != '\n' && pn < content.size() ) pn++; */
  /*     std::string line = content.substr(p,pn-p+1); */
  /*     in_ss.seekg( pn+1 ); */

  /*     sscanf( line.c_str(), format.c_str(), args ... ); */
  /*   } */

  /*   int Initialize(std::string); */

  /* }; */

/*    class DebugOutput{ */
/*   public: */

/*     template<class type> */
/*     DebugOutput&operator<<(type val){ */
/* #ifdef DEBUG */
/*       cerr<<val; */
/* #endif */
/*       return *this; */
/*     } */

/*     typedef DebugOutput& (*manipulator)(DebugOutput&); */

/*     DebugOutput&operator<<(manipulator manip){ */
/*       return manip(*this); */
/*     } */

/*     static DebugOutput&endl(DebugOutput&out){ */
/* #ifdef DEBUG */
/*       cerr<<std::endl; */
/* #endif */
/*       return out; */
/*     } */

/*     // this is the type of std::cout */
/*     typedef std::basic_ostream<char, std::char_traits<char> > CoutType; */
/*     // this is the function signature of std::endl */
/*     typedef CoutType& (*StandardEndLine)(CoutType&); */

/*     // define an operator<< to take in std::endl */
/*     DebugOutput& operator<<(StandardEndLine manip) */
/*     { */
/*       // call the function, but we cannot return it's value */
/* #ifdef DEBUG */
/*       manip(std::cerr); */
/* #endif */
/*       return *this; */
/*     } */
/*   } */

};


namespace std{
  /*   static pond::DocOut &endl(pond::DocOut&out){ */
  /*     pond::PostMessage( "\nprint" ); */
  /*     return out; */
  /*   } */
  /*   static pond::DocErr &endl(pond::DocErr&out){ */
  /*     pond::PostMessage( "\nerror" ); */
  /*     return out; */
  /*   } */

}
namespace pond{

};

#endif

