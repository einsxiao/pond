#define ModuleName Matrix
#include"pond.h"
#include"MatrixModule.h"
using namespace std;
using namespace pond;

INIT_SYMID_OF(Matrix);
INIT_SYMID_OF(Init);
INIT_SYMID_OF(HostToDevice);INIT_SYMID_OF(DeviceToHost);
INIT_SYMID_OF(ToMatlabArray);
INIT_SYMID_OF(Total); INIT_SYMID_OF(TotalDevice);
INIT_SYMID_OF(ToList);
INIT_SYMID_OF(ReadFile);
INIT_SYMID_OF(BinaryReadFile);
INIT_SYMID_OF(ReadVTKFile);
INIT_SYMID_OF(DumpFile);
INIT_SYMID_OF(BinaryDumpFile);
INIT_SYMID_OF(DumpVTKFile);
INIT_SYMID_OF(Clear);
INIT_SYMID_OF(Size);

__DEBUG_MAIN__("input.pd"); 

MatrixModule::MatrixModule():Module(MODULE_NAME){
  {
    // RegisterFunction("Matrix",(MemberFunction)(&MatrixModule::Matrix_Eva));
    AddAttribute("Matrix",AttributeType::Conjunctable);
    AddAttribute("Matrix",AttributeType::Setable);
  }
  {
    // RegisterFunction("MatrixExist",(MemberFunction)(&MatrixModule::MatrixExist_Eva));
  }
  {
    RegisterFunction("MatrixPosition",(MemberFunction)(&MatrixModule::MatrixPosition), this);
    AddAttribute("MatrixPosition","Protected");
  }
  {
    RegisterFunction("MatrixSetPosition",(MemberFunction)(&MatrixModule::MatrixSetPosition), this);
    AddAttribute("MatrixSetPosition","Protected");
  }
}

MatrixModule::~MatrixModule(){
}

Matrix *MatrixModule::GetMatrix(string name){
  return GlobalPool.Matrices.GetMatrix(name);
}

ComplexMatrix *MatrixModule::GetComplexMatrix(string name){
  return GlobalPool.ComplexMatrices.GetMatrix(name);
}

Matrix *MatrixModule::GetOrNewMatrix(string name){
  return GlobalPool.Matrices.GetOrNewMatrix(name);
}

ComplexMatrix *MatrixModule::GetOrNewComplexMatrix(string name){
  return GlobalPool.ComplexMatrices.GetOrNewMatrix(name);
}

int MatrixModule::RemoveMatrix(string name){
  return GlobalPool.Matrices.RemoveMatrix(name);
}

int MatrixModule::RemoveComplexMatrix(string name){
  return GlobalPool.ComplexMatrices.RemoveMatrix(name);
}


//////////////////////

Matrix&MatrixModule::operator()(string id){
  return *EvaKernel->GetMatrix(id);
}

Matrix&MatrixModule::operator[](string id){
  return *EvaKernel->GetOrNewMatrix(id);
}

int MatrixModule::PD_Matrix(Object &ARGV){
  Conjunct_Context(Matrix){
    Object&matobj = ARGV[1];
    Object&operobj = ARGV[2];
    CheckArgsShouldEqual(matobj,1);
    CheckArgsShouldBeString(matobj,1);
    string matname = matobj[1].Key();

    Conjunct_Case( Set ){
      CheckArgsShouldEqual(operobj,1);
      EvaKernel->Evaluate( operobj[1] );
      if ( operobj[1].NumberQ() ){
        Matrix* matptr = GetMatrix( matname );
        if ( matptr == NULL )
          ThrowError("Matrix","Assign a number to an empty Matrix is not allowed.");
        *matptr = operobj[1].Number();
        // ARGV.SetTo(1);
        ARGV = ARGV[1];
        ReturnNormal;
      }
      CheckArgsShouldBeList(operobj,1);
      Matrix* matptr = GetOrNewMatrix( matname );
      MatrixSet( *matptr, operobj[1] );
      // ARGV.SetTo(1);
      ARGV = ARGV[1];
      ReturnNormal;
    }

    Conjunct_Case( ReadFile ){
      CheckArgsShouldEqual(operobj,1);
      EvaKernel->Evaluate( operobj[1] );
      CheckArgsShouldBeString(operobj,1);
      string filename = operobj[1].Key();
      if ( not FileExistQ( filename ) )
        ThrowError(ARGV.Key(),"data file "+filename+" deos not exist.");
      Matrix* matptr = GetOrNewMatrix( matname );
      if ( matptr->ReadFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" deos not exist.");
      }
      // ARGV.SetTo(1);
      ARGV = ARGV[1];
      ReturnNormal;
    }

    Conjunct_Case( BinaryReadFile ){
      CheckArgsShouldEqual(operobj,1);
      EvaKernel->Evaluate( operobj[1] );
      CheckArgsShouldBeString(operobj,1);
      string filename = operobj[1].Key();
      if ( not FileExistQ( filename ) )
        ThrowError(ARGV.Key(),"data file "+filename+" deos not exist.");
      Matrix* matptr = GetOrNewMatrix( matname );
      if ( matptr->BinaryReadFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" deos not exist.");
      }
      // ARGV.SetTo(1);
      ARGV = ARGV[1];
      ReturnNormal;
    }

    Conjunct_Case( ReadVTKFile ){
      CheckArgsShouldEqual(operobj,1);
      EvaKernel->Evaluate( operobj[1] );
      CheckArgsShouldBeString(operobj,1);
      string filename = operobj[1].Key();
      if ( not FileExistQ( filename ) )
        ThrowError(ARGV.Key(),"data file "+filename+" deos not exist.");
      Matrix* matptr = GetOrNewMatrix( matname );
      if ( matptr->ReadVTKFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" deos not exist.");
      }
      // ARGV.SetTo(1);
      ARGV = ARGV[1];
      ReturnNormal;
    }

    Matrix* matptr = GetMatrix( matname );

    Conjunct_Case( ToList ){
      if ( matptr == NULL ) ThrowError("Matrix","ToList",matobj.ToString()+" does not exist yet." );
      CheckArgsShouldEqual(operobj,0);
      Object res;
      MatrixGet( *matptr, res );
      ARGV = res;
      ReturnNormal;
    }

    Conjunct_Case( Size ){
      if ( matptr == NULL ) ThrowError("Matrix","Size",matobj.ToString()+" does not exist yet." );
      CheckArgsShouldEqual(operobj,0);
      ARGV.SetList();
      ARGV.PushBackNumber( matptr->Size() );
      if ( matptr->ND > 0 ){
        Object dim; dim.SetList();
        for (int i=1; i <= matptr->ND; i++ )
          dim.PushBackNumber( matptr->Dim(i) );
        ARGV.PushBackRef( dim );
      }
      ReturnNormal;
    }

    Conjunct_Case( DeviceToHost ){
      if ( matptr == NULL ) ThrowError("Matrix","DeviceToHost",matobj.ToString()+" does not exist yet." );
      CheckArgsShouldEqual(operobj,0);
      if ( matptr->DataDevice == NULL )
        ThrowError("Matrix","DeviceToHost",matobj.ToString() + " has no data on Device side; operation cannot be completed.");
      matptr->DeviceToHost();
      ReturnNormal;
    }

    Conjunct_Case( HostToDevice ){
      if ( matptr == NULL ) ThrowError("Matrix","HostToDevice",matobj.ToString()+" does not exist yet." );
      CheckArgsShouldEqual(operobj,0);
      if ( matptr->Data == NULL )
        ThrowError("Matrix","HostToDevice",matobj.ToString() +" has no data on Host side; operation cannot be completed.");
      matptr->HostToDevice();
      ReturnNormal;
    }

    Conjunct_Case( DumpFile ){
      if ( matptr == NULL ) ThrowError("Matrix","DumpFile",matobj.ToString()+" does not exist yet." );
      CheckArgsShouldEqual(operobj,1);
      EvaKernel->Evaluate( operobj[1] );
      CheckArgsShouldBeString(operobj,1);
      string filename = operobj[1].Key();
      if ( matptr->DumpFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" can not be written.");
      }
      ReturnNull;
    }

    Conjunct_Case( BinaryDumpFile ){
      if ( matptr == NULL ) ThrowError("Matrix","DumpFile",matobj.ToString()+" does not exist yet." );
      CheckArgsShouldEqual(operobj,1);
      EvaKernel->Evaluate( operobj[1] );
      CheckArgsShouldBeString(operobj,1);
      string filename = operobj[1].Key();
      if ( matptr->BinaryDumpFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" can not be written.");
      }
      ReturnNull;
    }

    Conjunct_Case( DumpVTKFile ){
      if ( matptr == NULL ) ThrowError("Matrix","DumpFile",matobj.ToString()+" does not exist yet." );
      CheckArgsShouldEqual(operobj,1);
      EvaKernel->Evaluate( operobj[1] );
      CheckArgsShouldBeString(operobj,1);
      string filename = operobj[1].Key();
      if ( matptr->DumpVTKFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" can not be written.");
      }
      ReturnNull;
    }

    Conjunct_Case( Clear ){
      if ( matptr == NULL ) ThrowError("Matrix","Clear",matobj.ToString()+" does not exist yet." );
      RemoveMatrix( matname );
    }

    ReturnNormal;
  }

  Set_Context(Matrix){
    CheckArgsShouldEqual(ARGV[1],1);
    CheckArgsShouldBeString(ARGV[1],1);
    EvaKernel->Evaluate( ARGV[2] );
    MatrixSet( ARGV[1][1].Key(), ARGV[2] );
    // ARGV.SetTo(2);
    ARGV = ARGV[2];
    ReturnNormal;
  }
  
  CheckShouldNoLessThan(1);
  if ( ARGV[1].StringQ() ){//A Single string, only to figure out the name of the matrix, without care about the existence
    if ( ARGV.Size() > 1 )
      ThrowError("Matrix","When specify a Matrix with its name, one argument is required.");
    ReturnNormal;
  }

  if ( ARGV[1].NumberQ() ){// form of Matrix[3,{5,9,4},"HostDevice"]
    int dimN=(double)ARGV[1];
    if ( dimN < 1)
      ThrowError("Matrix","The number of Matrix Dimensions should be a positive integer.");
    if ( ARGV.Size() < 2 ){
      ThrowError("Matrix","When Init Matrix with Dimensions, two or more arguments are required.");
    }
    int dim[dimN+2]; dim[0]=dimN;
    if ( (int)ARGV[2].Size() != dimN )
      ThrowError("Matrix","The dimension specification is not consistent with previous argument.");


    for ( int i=1; i<=dimN; i++ ){
      if ( ! ARGV[2][i].NumberQ() )ThrowError("Matrix","Matrix Dimension specification should be numbers.");
      dim[i] = (double)ARGV[2][i];
      if ( dim[i] < 1) ThrowError("Matrix","Matrix Dimension should be a positive integer.");
    }
    int matPos= MatrixHost;
    if ( ARGV.Size() > 2 ){
      CheckShouldBeString(3);
      if ( ARGV[3].StringQ("Host") ) matPos = MatrixHost;
      else if ( ARGV[3].StringQ("Device") ) matPos = MatrixDevice;
      else if ( ARGV[3].StringQ("HostDevice") ) matPos = MatrixHostDevice;
      else {
        ThrowError("Matrix","When Init Matrix, position specification should be one of Host, Device, or HostDevice.");
      }
    }
    for ( ;;){
      string name = pond::RandomString( Len_Of_New_Matrix_Name );
      Matrix *mat = GetMatrix(name);
      if ( mat == NULL ){
        mat = GetOrNewMatrix(name);
        mat->Init(dim,matPos);
        ARGV.DeleteElements();
        ARGV.PushBackString( name );
        break;
      }
    }
    ReturnNormal;
  }
  if ( ARGV[1].ListQ(SYMID_OF_List)  ){
    if ( ARGV.Size() > 1 )
      ThrowError("Matrix","When Init Matrix with a List, only one argument is required.");
    for ( ;;){
      string name = pond::RandomString( Len_Of_New_Matrix_Name );
      Matrix *mat = GetMatrix(name);
      if ( mat == NULL ){
        mat = GetOrNewMatrix(name);
        MatrixSet( *mat, ARGV[1] );
        ARGV.DeleteElements();
        ARGV.PushBackString( name );
        break;
      }
    }

      
  }
  ReturnHold;
}

int MatrixModule::MatrixGet(string name,Object &ARGV){//mat of Object form
  Matrix * mat = GetMatrix(name);
  if ( mat == NULL ){
    ARGV.SetNull();
    return 0;
  }else{
    MatrixModule::MatrixGet(*mat,ARGV);
    return 1;
  }
}


int MatrixModule::MatrixSet(string name,Object&ARGV){
  Matrix *mat = GetOrNewMatrix(name);
  return MatrixSet(*mat,ARGV); 
} 

int MatrixModule::MatrixPosition(Object &ARGV){
  CheckArgsShouldEqual(ARGV,0)
  if ( EvaSettings::MatrixPosition() == MatrixDevice )
    ARGV.SetString("Device");
  else if ( EvaSettings::MatrixPosition() == MatrixHostDevice )
    ARGV.SetString("HostDevice");
  else ARGV.SetString("Host");
  ReturnNormal;
}

int MatrixModule::MatrixSetPosition(Object &ARGV){
  CheckArgsShouldEqual(ARGV,1);
  CheckShouldBeString(1);
  if ( ARGV[1].StringQ( "Host" ) )
    EvaSettings::SetMatrixPosition(MatrixHost);
  else if ( ARGV[1].StringQ("Device") )
    EvaSettings::SetMatrixPosition(MatrixDevice);
  else if ( ARGV[1].StringQ("HostDevice") )
    EvaSettings::SetMatrixPosition( MatrixHostDevice);
  else{
    ThrowError(ARGV.Key(),"position specification should be one of {\"Host\",\"Device\",\"HostDevice\"}.");
  }
  ReturnNull;
}

int LocalMatrixQ(Object &ARGV,Object &dim){
  if ( not ARGV.ListQ(SYMID_OF_List) ){
    if ( !ARGV.NumberQ() ) return -1;
    return 1;
  }
  if ( dim.NullQ() )
    dim.SetList();
  dim.PushBackNumber( ARGV.Size() );
  Object dim1;
  if ( LocalMatrixQ( ARGV[1], dim1 ) < 0 ){
    dim.SetNull();
    return -1;
  }
  for ( u_int i=2; i<=ARGV.Size(); i++ ){
    Object dim2;
    LocalMatrixQ(ARGV[i],dim2);
    if ( dim1 != dim2 ){
      dim.SetNull();
      return -1;
    }
  }
  if ( not dim1.NullQ() ){
    dim.InsertRef( dim.End(), dim1.Begin(), dim1.End() );
  }
  return 1;
}

bool MatrixModule::MatrixQ(Object & ARGV,Object&dim){
  dim.SetList();
  if ( LocalMatrixQ( ARGV,dim)>0 )
    return true;
  return false;
}


bool MatrixModule::MatrixQ(Object&ARGV){
  Object dim;
  return MatrixQ( ARGV, dim );
}

bool MatrixModule::MatrixExist(string name){
  if ( GetMatrix(name ) != NULL ) return true;
  return false;
}

int MatrixModule::PD_MatrixExist(Object &ARGV){
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  if ( MatrixExist( ARGV[1].Key() ) ){
    ARGV.SetNumber( true );
    ReturnNormal;
  }
  ARGV.SetNumber(false);
  ReturnNormal;
}

#define LOCAL_ARRAY_2_LIST(type,to_type)                                \
  static int LocalArray2List(Object &ARGV,type*arr,int *dim,int pdim, int parr){ \
    if ( pdim>dim[0] ){                                                 \
      ARGV.SetNumber( (to_type) arr[parr] );                            \
      return parr+1;                                                    \
    }                                                                   \
    ARGV.SetList();                                                \
    for (int i=0; i<dim[pdim]; i++){                                    \
      ARGV.PushBackNull();                                              \
      parr=LocalArray2List( ARGV[i+1],arr,dim,pdim+1,parr);             \
    }                                                                   \
    return parr;                                                        \
  }

LOCAL_ARRAY_2_LIST(int,int)
LOCAL_ARRAY_2_LIST(long long,double)
LOCAL_ARRAY_2_LIST(float,double)
LOCAL_ARRAY_2_LIST(double,double)
LOCAL_ARRAY_2_LIST(complex,complex)
#undef LOCAL_ARRAY_2_LIST
#define ARRARY_2_LIST(type) \
int MatrixModule::Array2List(Object &ARGV,type*arr,int *dim){ \
  LocalArray2List( ARGV,arr,dim,1,0); \
  return 0; \
}
ARRARY_2_LIST(int)
ARRARY_2_LIST(long long)
ARRARY_2_LIST(float)
ARRARY_2_LIST(double)
ARRARY_2_LIST(complex)
#undef ARRARY_2_LIST

int MatrixModule::MatrixGet(Matrix&matrix,Object &ARGV){
  ARGV.SetList();
  if ( not matrix.Size() == 0 ){
    int *dim = matrix.NewDimArray();
    Array2List(ARGV,matrix.Data,dim);
    delete []dim;
  }else{
    return -1;
  }
  return 1;
}

int MatrixModule::MatrixGet(ComplexMatrix&matrix,Object &ARGV){
  if ( matrix.Size() != 0 ){
    int *dim = matrix.NewDimArray();
    Array2List(ARGV,matrix.Data,dim);
    delete []dim;
  }else{
    return -1;
  }
  return 1;
}

static int LocalList2Array(Object &ARGV,double*arr, int parr){
  if ( ARGV.AtomQ() ){
    if ( ARGV.NumberQ() ){
      arr[parr] = double(ARGV);
      return parr+1;
    }else{
      Warning("List2Array","Non-number encontered in operation of ARGV to array.");
      return -1;
    }
  }
  for (u_int i=0; i<ARGV.Size(); i++){
    parr=LocalList2Array( ARGV[i+1],arr,parr);
    if ( parr<0 ) return -1;
  }
  return parr;
}


static int LocalList2Array(Object &ARGV,complex*arr, int parr){
  if ( ARGV.AtomQ() ){
    if ( ARGV.NumberQ() ){
      arr[parr] = ARGV.Number();
      return parr+1;
    }else{
      Warning("List2Array","Non-number encontered in operation of ARGV to array.");
      return -1;
    }
  }
  for (u_int i=0; i<ARGV.Size(); i++){
    parr=LocalList2Array( ARGV[i+1],arr,parr);
    if ( parr<0 ) return -1;
  }
  return parr;
}

int MatrixModule::List2Array(Object &ARGV,double*arr){
  LocalList2Array(ARGV,arr,0);
  return 0;
}
int MatrixModule::List2Array(Object &ARGV,complex*arr){
  LocalList2Array(ARGV,arr,0);
  return 0;
}

template<class type1>
void localMatrixSet(Matrix_T<type1>&mat,Object&ARGV,u_long&ind){
  if ( ARGV.ListQ() ){
    for(u_int i=1;i<=ARGV.Size();i++){
      localMatrixSet(mat,ARGV[i],ind);
    }
    return;
  }else if ( ARGV.NumberQ() ){
    mat.Data[ind] = (type1)ARGV;
    ind++;
    return;
  }
  ThrowError("MatrixSet","Object is not all numbers.");
}

#define _MATRIX_SET(type_basic)                                         \
  int MatrixModule::MatrixSet(Matrix_T<type_basic>&matrix,Object&ARGV){ \
    if ( ARGV.NumberQ() ){                                              \
      if ( matrix.Size() == 0  )                                        \
        ThrowError("MatrixSet","Assign a number to an empty Matrix is not allowd."); \
      matrix = ARGV.Number();                                           \
    }                                                                   \
    Object dim; dim.SetList();                                     \
    if ( not MatrixQ( ARGV, dim ) )                                     \
      ThrowError("MatrixSet","Matrix assignment from list should be of Matrix form."); \
    int n =dim.Size();                                                  \
    int *arr= new int[n+2];                                             \
    arr[0]=n;                                                           \
    for (int i=1; i<=n; i++){                                           \
      arr[i] =  (int)dim[i] ;                                           \
    }                                                                   \
    matrix.Init( arr ,MatrixHost);                                      \
    u_long ind=0;                                                       \
    localMatrixSet( matrix, ARGV, ind);                                 \
    delete []arr;                                                       \
    return 1;                                                           \
  }; 
_MATRIX_SET(complex);
_MATRIX_SET(double);
_MATRIX_SET(float);
_MATRIX_SET(floatcomplex);
_MATRIX_SET(int);
#undef _MATRIX_SET


/*
Matrix &MatrixModule::GetTempMatrix(Matrix mat)
{
  int first = 0, final = tempMatrix.size() -1, pos = 0,result = 0;
  auto sIter = tempMatrix.begin();
  vector<Matrix*>::iterator iter;
  while ( first <= final ){
    pos = first + ( final - first )/2;
    result = mat.DimensionCompare( tempMatrix[pos].first );
    if ( result == 0 ){
      sIter = tempMatrix.begin() + pos;
      break;
    }else if ( result < 0 ){
      final = pos -1;
    }else{
      first = pos +1;
    }
  }
  if ( result != 0 ){
    if ( pos >= (int)tempMatrix.size() ){
      tempMatrix.push_back(  pair<int*,vector<Matrix*> >() );
      tempMatrix.rbegin()->first = mat.NewDimArray();
      sIter = tempMatrix.end() - 1;
    }else{
      if ( result > 0 ) pos++;
      tempMatrix.insert( tempMatrix.begin()+pos, pair<int*,vector<Matrix*> >() );
      sIter =  tempMatrix.begin()+pos;
    }
  }
  iter = sIter->second.begin();
  for(;;) {
    if ( iter == sIter->second.end() ){
      Matrix *tM = new Matrix;
      (*tM).Init( sIter->first ,MatrixHostDevice );
      sIter->second.push_back( tM );
      return *tM;
    }
    if ( (**iter).state == 2 ){
      (**iter).state = 3;
      return (**iter);
    }else
      iter++;
  }
}
*/

