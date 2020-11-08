#ifndef EM_POND_H
#define EM_POND_H

namespace pond{
  ////////////////
  // emscripten worker operations
  std::string   idb_load(const char* filename,const char*database="/pond");
  int           idb_load_to_file(const char* filename,const char* database="/pond");

  int           idb_store(const char* filename,const std::string& content,const char*database="/pond");
  int           idb_store(const char* filename, const char*content,u_int size, const char*database="/pond");
  int           idb_store_from_file(const char* filename,const char*database="/pond");

  int           idb_delete(const char* filename,const char*database="/pond");
  int           idb_existsQ(const char* filename,const char*database="/pond");

  ////////////////////////////////////////////////////
  void          PostMessage(const std::string mes);
  void          PostMessageError(const std::string mes); //set EvaKernel error code simutaniously
  void          CleanMessage(const std::string type); //in seconds
  //max wait in seconds, duration in micro seconds
  std::string   WaitForMessage(const std::string type,const double max_wait,int sleep_dur=100);
  ////////////////////////////////////////////////////
  void          sleep_ms(const double);
  ////////////////////////////////////////////////////
  
}

#endif
