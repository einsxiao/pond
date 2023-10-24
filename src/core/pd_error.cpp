#include"pd_headers.h"
#include <cxxabi.h>
using namespace std;
using namespace pond;

DebugOutput __DebugOutput::out;

int hex2dec(char*hex){
  int x;
  stringstream ss;
  ss << std::hex << hex ;
  ss >> x;
  return x;
}

void pond::__stacktrace_pretty_print(){
#ifdef WIN32
  //Do nothing
#else
  FILE *out =stderr;
  unsigned int max_frames = 63;
  fprintf(out, "stack trace:\n");

  // storage array for stack trace address data
  void* addrlist[max_frames+1];

  // retrieve current stack addresses
  int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

  if (addrlen == 0) {
    fprintf(out, "  <empty, possibly corrupt>\n");
    return;
  }

  // resolve addresses into strings containing "filename(function+address)",
  // this array must be free()-ed
  char** symbollist = backtrace_symbols(addrlist, addrlen);

  // allocate string which will be filled with the demangled function name
  size_t funcnamesize = 256;
  char* funcname = (char*)malloc(funcnamesize);

  // iterate over the returned symbol lines. skip the first, it is the
  // address of this function.
  for (int i = 1; i < addrlen; i++) {
    char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

    // find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x15c) [0x8048a6d]
    for (char *p = symbollist[i]; *p; ++p) {
      if (*p == '(')
        begin_name = p;
      else if (*p == '+')
        begin_offset = p;
      else if (*p == ')' && begin_offset) {
        end_offset = p;
        break;
      }
    }

    if (begin_name && begin_offset && end_offset && begin_name < begin_offset) {
      *begin_name++ = '\0';
      *begin_offset++ = '\0';
      *end_offset = '\0';

      // mangled name is now in [begin_name, begin_offset) and caller
      // offset in [begin_offset, end_offset). now apply
      // __cxa_demangle():

      int status;
      char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
      if (status == 0) {
        funcname = ret; // use possibly realloc()-ed string
        fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname, begin_offset);
        // fprintf(out, "  %s : %s+%d\n", symbollist[i], funcname, hex2dec(begin_offset) );
      } else {
        // demangling failed. Output function name as a C function with
        // no arguments.
        fprintf(out, "  %s : %s()+%s\n", symbollist[i], begin_name, begin_offset);
        // fprintf(out, "  %s : %s()+%s\n", symbollist[i], begin_name, hex2dec(begin_offset) );
      }
    } else {
      // couldn't parse the line? print the whole line.
      fprintf(out, "  %s\n", symbollist[i]);
    }
  }

  free(funcname);
  free(symbollist);
#endif
}

void pond::Error::trace()const{
  // void *array[10];                                  
  // size_t size;                                      
  // size = backtrace(array, 10);                      
  // backtrace_symbols_fd(array, size, STDERR_FILENO); 
  pond::__stacktrace_pretty_print();
}

const string pond::Error::swhat()const throw(){
  //print backtrace information
  //dout<<"pond error throwed with what calculated"<<endl;
  //dout<<"module = "<<module<<endl;
  //dout<<"function = "<<function<<endl;
  //dout<<"message = "<<message<<endl;
  string res;
  if ( module != "" ){
    res = module+"::"+function+"::Error: "+message;
  }else{
    if ( function != "" ){
      res = function+"::Error: "+message;
    }else{
      res = "Error: "+message;
    }
  }
  //dout<<"what calculated :"<< res <<endl;
  return res;
}

const char *pond::Error::what()const throw(){
  return swhat().c_str();
}

// // simple traceback version:
// #define __traceback                                     \
//   ({                                                    \
//     int nptrs;                                          \
//     void* array[10];                                    \
//     nptrs = backtrace(array, 10);                       \
//     backtrace_symbols_fd(array, nptrs, STDERR_FILENO);  \
//   })
