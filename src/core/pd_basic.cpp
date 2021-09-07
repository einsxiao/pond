#include<pd_headers.h>
#include<unistd.h>
#include<sstream>
#include<math.h>
#include<sys/stat.h> // get file status
// #include<emscripten.h>
// #include<emscripten/bind.h>
// #include<emscripten/fetch.h>
//using namespace emscripten;
using namespace pond;
using namespace std;
//#include<filesystem>

//namespace fs = std::__fs::filesystem;

string pond::ToString(const int num){
  char chs[32];
  sprintf(chs,"%d",num);
  return string(chs);
}

string pond::ToString(const u_int num){
  char chs[32];
  sprintf(chs,"%d",num);
  return string(chs);
}

string pond::ToString(const u_long num){
  char chs[32];
  sprintf(chs,"%ld",num);
  return string(chs);
}

string pond::ToString(const Index num){
  char chs[64];
  sprintf(chs,"$$_%d&%d",num.row, num.col );
  return string(chs);
}

string pond::ToString(const void*num){
  char chs[10];
  sprintf(chs,"0x%lx",(u_long)num);
  return string(chs);
}

string pond::ToString(const ObjectType type){
  switch ( type ){
  case ObjectType::Number: return "Number"; break;
  case ObjectType::Symbol: return "Symbol"; break;
  case ObjectType::String: return "String"; break;
  case ObjectType::List:   return "List"; break;
  }
  return "Unknown ObjectType";
}

class OstringStreamTable:public __EvaTable< ostringstream >{
public:
  OstringStreamTable():__EvaTable( 5 ){};
};

OstringStreamTable ostringsPool;


string pond::ToString(const char *val){
  return string(val);
}
string pond::ToString(double num){
  Index idx = ostringsPool.New();
  ostringstream&ss = ostringsPool.Get(idx);
  ss.str("");
  ss.precision(EvaSettings.precision);
  ss.setf( ios::uppercase);
  if ( EvaSettings.scientificFormat ){
    ss<<std::scientific<<num;
  }else{
    ss<<num;
  }
  ostringsPool.Free(idx);
  return ss.str();
}

string pond::ToString(complex num){
  Index idx = ostringsPool.New();
  ostringstream&ss = ostringsPool.Get(idx);
  ss.str("");
  ss.precision(EvaSettings.precision);
  if ( EvaSettings.scientificFormat ){
    ss<<std::scientific<<"("<<num.re<<","<<num.im<<")";
  }else{
    ss<<"("<<num.re<<","<<num.im<<")";
  }
  ostringsPool.Free(idx);
  return ss.str();
}

string pond::ToString(float num){
  Index idx = ostringsPool.New();
  ostringstream&ss = ostringsPool.Get(idx);
  ss.str("");
  ss.precision(EvaSettings.precision);
  ss.setf( ios::uppercase);
  if ( EvaSettings.scientificFormat ){
    ss<<std::scientific<<num;
  }else{
    ss<<num;
  }
  ostringsPool.Free(idx);
  return ss.str();
}

string pond::ToString(floatcomplex num){
  Index idx = ostringsPool.New();
  ostringstream&ss = ostringsPool.Get(idx);
  ss.str("");
  ss.precision(EvaSettings.precision);
  if ( EvaSettings.scientificFormat ){
    ss<<std::scientific<<"("<<num.re<<","<<num.im<<")";
  }else{
    ss<<"("<<num.re<<","<<num.im<<")";
  }
  ostringsPool.Free(idx);
  return ss.str();
}

double pond::ToNumber(const char*str){
  double num;
  sscanf(str,"%lf",&num);
  return num;
}
  
// pond::complex pond::ToComplex(const char*str){
//   complex num;
//   sscanf(str,"(%lf,%lf)",&num.re,&num.im);
//   return num;
// }

/*
int pond::compare(ObjectType type1,ObjectType type2){
  if ( type1< type2 ) return -1;
  else if (type1> type2) return 1;
  else return 0;
}
*/

bool approximatelyEqual(double a, double b, double epsilon) {
  return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool definitelyGreaterThan(double a, double b, double epsilon) {
  return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool definitelyLessThan(double a, double b, double epsilon) {
  return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

int pond::compare(double num1,double num2){
  double cut =  ( (fabs(num1) < fabs(num2) ? fabs(num2) : fabs(num1)) * EvaSettings.epsilon);
  double diff= num1-num2;
  if ( diff > cut ) return 1;
  if ( diff < -cut ) return -1;
  return 0;
}

// int pond::compare(complex num1,complex num2){
//   int c1 = compare(num1.re,num2.re);
//   return_if_not(c1,0);
//   c1 = compare(num1.im,num2.im);
//   return c1;
// }

#define sign(num) ((num)>0?1:((num)<0?-1:0)); 
int pond::compare(const char*str1,const char*str2){
  if ( str1 == NULL ) {
    if ( str2 == NULL )
      return 0;
    else
      return -1;
  }else{
    if ( str2 == NULL )
      return 1;
    else
      return sign(strcmp(str1,str2));
  }
}

int pond::compare(string str1,string str2){
  return compare(str1.c_str(), str2.c_str() );
}

template<class type>  void local_swap(type&v1, type&v2){
  std::swap(v1,v2);
}

#define LOCAL_SWAP(type) void pond::swap(type&l1,type&l2){local_swap(l1,l2);}
//LOCAL_SWAP(ObjectType)
LOCAL_SWAP(short unsigned int)
LOCAL_SWAP(int)
LOCAL_SWAP(u_int)
LOCAL_SWAP(char*)
LOCAL_SWAP(double)
LOCAL_SWAP(float)
// LOCAL_SWAP(complex)
LOCAL_SWAP(long)
LOCAL_SWAP(u_long)

#undef LOCAL_SWAP

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// System Operations
int pond::StringSplit(string str, vector<string>&strs,string split){
  strs.clear();
  int p,p1,p2;
  for (unsigned i = 1; i < split.size(); i++){
    if (split[i] == split[0]) continue;
    p = -1;
    while ( ( p=str.find( split[i] ,p+1 ) ) >=0 ){
      str[p]=split[0];
      if (p == (int)str.size() -1 ) break;
    }
    
  }
  str += split[0];
  //while ( (p=str.find(""+split[0]+split[0])) >=0 ) str.erase(p,1);
  p2=-1;
  while ( true ){
    p1 = p2;
    if ( (p2 = str.find(split[0],p2+1) )<0 ) break;
    strs.push_back(str.substr(p1+1,p2-p1-1));
    while ( p2<(int)str.size() && str[ p2+1 ] == split[0] ) p2++;
  }
  return 0;
}
int pond::StringSplit(string str,vector<string>&strs){
  return StringSplit(str,strs," \n\t,");
}

string pond::RandomString(int len){
  if ( len < 1 ) { Erroring("RandomString","Length specified should bigger than zero."); return ""; }
  string res = "";
  for ( int i = 0 ; i< len; i++ ){
    char ch =  35+56*pond::Rand();
    res+=ch;
  }
  return res;
}

string pond::RandomNumberString(int len){
  if ( len < 1 ) { Erroring("RandomString","Length specified should bigger than zero."); return ""; }
  string res = "";
  for ( int i = 0 ; i< len; i++ ){
    char ch =  '0'+(pond::Rand()*9);
    res+=ch;
  }
  return res;
}



string pond::IthWord(string str,int n){
  int p,p1=-1,p2=-1,c=0;
  if ( str[ str.size() ] == '\n' ){
    str.erase(str.size(),1);
  }
  str=str+" ";
  while ( (p=str.find(',') ) >=0 )
    str[p]=' ';
  while ( (p=str.find("  ")) >=0 )
    str.erase(p,1);
  p1=-1;p2=-1;c=0;
  while ( true ){
    c++; p1 = p2;
    p2 = str.find(" ",p2+1);
    if ( p2<0 ){
      return "";
    }
    if ( c==n ){
      return str.substr(p1+1,p2-p1-1);
    }
    while ( p2<(int)str.size() && str[ p2+1 ] == ' ') p2++;
  }
}

string pond::RemoveFirstWord(string str,string &leftword){
  string word = IthWord(str,1);
  leftword = str.substr( word.size() );
  while ( leftword[0] == ' ' ) leftword.erase(0,1);
  return word;
}

string pond::GetEnv(string name,string default_value){
  string content = "";
  char *p;
  if ( !(p = getenv(name.c_str())) ){
    return default_value;
  }
  content = p;
  return content;
}

int pond::SetEnv(string var,string expr,bool overwrite){
  if ( setenv(var.c_str(),expr.c_str(),overwrite )  ){
    Warning("SetEnv","Failed to change environment varible by Expression: "+ expr);
    return -1;
  }
  return 0;
}

string pond::GetDirectory(){
  string pwd="";
  char *buffer;  
  if( (buffer = getcwd(NULL, 0)) == NULL) {  
    { Erroring("GetDirectory","Can not get Directory information."); return ""; }
  }  
  pwd = buffer;
  free( buffer );
  return pwd;
}

string pond::GetParentDirectory(){
  char *buffer;  
  if( (buffer = getcwd(NULL, 0)) == NULL) {  
    { Erroring("GetDirectory","Can not get Directory information."); return ""; }
  }  
  int ind = strrchr(buffer,'/') - buffer;
  if ( ind <= 0 ) return "";
  buffer[ind]='\0';
  string pwd = buffer;
  free( buffer );
  return pwd;
}

string pond::GetBaseName(string path){
  const char *base = strrchr(path.c_str(),'/');
  if ( base - path.c_str() <= 0 )
    return path;
  return (base+1);
}

string pond::GetDirName(string path){
  int ind = strrchr(path.c_str(),'/') - path.c_str();
  if ( ind <= 0 ) return "";
  return path.substr(0,ind);
}

// bool pond::SetDirectory(string dirname){
//   if ( chdir( dirname.c_str() ) != 0 )
//     return false;
//   return true;
// }

static int LocalSystem(string cmd, string&result){
  result="";
  FILE *fp;
  char line[1024];
  fp = popen(cmd.c_str(),"r");
  if ( fp == NULL ){
    Warning("System",cmd+" cannot be executed.");
    return -1;
  }
  if ( fgets(line,sizeof(line),fp) != NULL ){
    if ( line[ strlen(line)-1 ] == '\n' ){
      line[ strlen(line)-1 ] = '\0';
    }
    result=line;
  }
  int rc=pclose(fp); // the return status
  if ( rc == -1 ) return -1;
  if ( WEXITSTATUS(rc) != 0 ) return -1;
  return 0;
}

string pond::System(string cmd){
  string result;
  LocalSystem(cmd,result);
  return result;
}


int pond::System(string cmd, vector<string>&result){
  result.clear();
  FILE *fp;
  char line[1024];
  fp = popen(cmd.c_str(),"r");
  if ( fp == NULL ){
    cerr<<"  Error: "<<cmd<<" cannot be excuted"<<endl;
    return -1;
  }
  while ( fgets(line,sizeof(line),fp) != NULL ){
    if ( line[ strlen(line)-1 ] == '\n' ){
      line[ strlen(line)-1 ] = '\0';
    }
    result.push_back(line);
  }
  return pclose(fp)/256;
}

int pond::Execute(string cmd){
  int status=system(cmd.c_str());
  if ( status == -1 ){
    cerr<<"  Error: "<<cmd<<" cannot be excuted"<<endl;
    return -1;
  }else if ( WEXITSTATUS(status) != 0 ){
    return -1;
  }
  return 0;
}

bool pond::FileExistQ(string filename){
  struct stat buf;
  if ( lstat(filename.c_str(),&buf) < 0)
    return false;
  return true;
}

bool pond::CheckFileType(string filename,string type){
  struct stat buf;
  if ( lstat(filename.c_str(),&buf) < 0) return false;
  if ( type == "DIR" && S_ISDIR(buf.st_mode) ) return true;
  if ( type == "REG" && S_ISREG(buf.st_mode) ) return true;
  if ( type == "LINK" && S_ISLNK(buf.st_mode) ) return true;
  if ( type == "FIFO" && S_ISFIFO(buf.st_mode) ) return true;
  return false;
}

bool pond::CommandExist(string cmd){
  string str;
  if (LocalSystem("type "+cmd+" 2>&1",str)< 0 )
    return false;
  else
    return true;
}

string pond::GetFileContent(string filename){
  string content;
  ifstream ifs;
  ifs.open(filename.c_str() );
  if (ifs){
    string str;
    while ( getline(ifs,str) )
      content +=str+"\n";
    ifs.close();
    return content;
  }  
  return "";
}

int pond::RemoveFile(string filename){
  remove( filename.c_str() );
  return 0;
}

int pond::SetFileContent(string filename,string content){
  ofstream ofs;
  ofs.open(filename.c_str() );
  if (ofs){
    ofs<<content;
    ofs.close();
    return 0;
  }
  return -1;
}

string pond::GetFileMD5(string filename){
  string md5info = IthWord(System("md5sum "+filename),1);
  if (md5info == "md5sum:") return "FILE_NOT_EXIST";
  return md5info;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
unsigned int pond::SimpleHashCodeOfString(const char*str){
  unsigned int code = 5381;
  const char *po=str;
  if ( str == NULL ) return 0;
  for (;(*po)!='\0';po++){ code= (code*33) ^ (*po); }
  return code;
}

const char* pond::Type2String(ObjectType type){
  switch(type){
  case ObjectType::Number: return "Number";
  case ObjectType::String: return "String";
  case ObjectType::Symbol: return "Symbol";
  case ObjectType::List: return "List";
  }
  { Erroring("Type2String","switch error."); return ""; }
}


#define CT(type) case RSimpleHashCodeOfString(#type): return AttributeType::type;
AttributeType pond::String2AttributeType(const char*str){
  switch ( SimpleHashCodeOfString( str ) ){
    CT(Null);
    CT(Flat);
    CT(Listable);
    CT(OneIdentity);
    CT(HoldFirst);
    CT(HoldRest);
    CT(HoldAll);
    CT(HoldAllCompleted);
    CT(SequenceHold);
    CT(Orderless);
    // CT(GrammarUnit);
    CT(Constant);
    CT(DelayFunction);
    CT(DelayHold);
    CT(DelaySetable);

    CT(Conjunctable);
    CT(Setable);
    CT(Partable);
    // CT(Destroyable);
    CT(Protected);
    CT(Locked);
    CT(MAXENUM);
  }
  { Erroring("Attribute",(string)"Unknown Attribute type "+str+"."); return AttributeType::Null; }
}
#undef CT

#define CT(type) case AttributeType::type: return #type;
const char* pond::AttributeType2String(AttributeType type){
  switch ( type ){
    CT(Null);
    CT(Flat);
    CT(Listable);
    CT(OneIdentity);
    CT(HoldFirst);
    CT(HoldRest);
    CT(HoldAll);
    CT(HoldAllCompleted);
    CT(SequenceHold);
    CT(Orderless);
    // CT(GrammarUnit);
    CT(Constant);
    CT(DelayFunction);
    CT(DelayHold);
    CT(DelaySetable);

    CT(Conjunctable);
    CT(Setable);
    CT(Partable);
    // CT(Destroyable);
    CT(Protected);
    CT(Locked);
    CT(MAXENUM);
  }
  Erroring("Attribute","Unkown Attribute type.");
  return "";
}
#undef CT

bool pond::AttributeQ(bool*attri,AttributeType type){
  return attri[(u_char)type];
}

bool pond::AttributeQ(bool*attri,const char *str){
  return attri[ (u_char)String2AttributeType(str) ];
}

void pond::SetAttribute(bool*attri,AttributeType type,bool val){
  attri[(u_char)type] = val;
}

void pond::SetAttribute(bool*attri,const char*str,bool val){
  attri[ (u_char)String2AttributeType(str) ] = val;
}

//const string   specialCharecterSet=" \n\t\"\'\\[](){},.;+-*/^~!@#$%&|.:=_<>"; 
bool pond::SpecialCharecterQ(char ch){
  switch ( ch ){
  case (' '): case ('\n'): case ('\t'): case ('\"'): case ('\''): case ('\\'): case ('['): case (']'): case ('('): case (')'): case ('{'): case ('}'): case (','): case ('.'): case (';'): case ('+'): case ('-'): case ('*'): case ('/'): case ('^'): case ('~'): case ('!'): case ('@'): case ('#'): case ('$'): case ('%'): case ('&'): case ('|'): case (':'): case ('='): case ('<'): case ('>'): case ('?'):
    return true;
  case ('_'):  return false;
  default: return false;
  }
}

double pond::GaussRand(double mean,double variance){
  static thread_local double gau_var1,gau_var2,gau_s;
  static thread_local int gau_phase = 0;
  double x;
  if ( gau_phase == 0 ){
    do{
      double u1 = pond::Rand();
      double u2 = pond::Rand();
      gau_var1=2*u1-1;
      gau_var2=2*u2-1;
      gau_s=gau_var1*gau_var1+gau_var2*gau_var2;
    }while(gau_s>=1||gau_s==0);
    x=gau_var1*sqrt(-2*log(gau_s)/gau_s);
  }else{ 
    x=gau_var2*sqrt(-2*log(gau_s)/gau_s);
  }
  gau_phase=1-gau_phase;
  return x*variance+mean;
}

bool pond::GPUDeviceQ(){
  if ( Execute("nvidia-smi -L >/dev/null 2>&1") != 0 )
    return false;
  return true;
}

void pond::sleep_micro(int sec)
{
  usleep(1000*sec);
}

void pond::sleep_sec(int sec)
{
  usleep( sec*1000000);
}

int pond::specialCharReplace(string &str, const string ori, const string rep){
  int pos = 0;
  while ( (pos <(int)str.size())&&pos>=0 ){
    pos = str.find(ori);
    if (pos>=0){
      str.replace(pos,ori.size(),rep);
      pos += rep.size() - ori.size();
    }
  }
  return 0;
}
int pond::PondInnerStringRestoreNormal(string &str){
  specialCharReplace(str,"\\n","\n");
  specialCharReplace(str,"\\t","\t");
  specialCharReplace(str,"\\r","\r");
  specialCharReplace(str,"\\\\","\\");
  // specialCharReplace(str,"\\\"","$QUOTATION_MARK$");
  // specialCharReplace(str,"\\\'","$QUOTATION_SMARK$");
  // specialCharReplace(str,"$QUOTATION_MARK$","\"");
  // specialCharReplace(str,"$QUOTATION_SMARK$","\'");
  return 0;
}
