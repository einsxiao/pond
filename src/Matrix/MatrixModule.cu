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
    AddAttribute("Matrix",AttributeType::Partable);
  }
  {
    // RegisterFunction("MatrixExist",(MemberFunction)(&MatrixModule::MatrixExist_Eva));
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

int get_part_obj_index( Object&partobj, Matrix&mat){
  EvaKernel->EvaluateRest( partobj );
  for (int i=2; i<= partobj.Size(); i++){
    if ( !partobj[i].NumberQ() ){
      ThrowError("Matrix::Part", "Part indices should be nubmers");
    }
  }
  int ind =  int( partobj[2] );
  //cout<<"int ind = "<< ind<<endl;
  for (int i=1; i< mat.ND; i++){
    ind = int( partobj[i+2] ) + ind * mat.Dim(i+1);
    //cout<<"deal i= "<< i << ", " << partobj[i+2] <<", " << mat.Dim(i+1)<<" :"<< ind <<endl;
  }
  //cout<<" endl ind = "<< ind <<endl;
  return ind;
}
int MatrixModule::PD_Matrix(Object &ARGV){
  //cout<<"into matrix:"<<ARGV.ToFullFormString() <<endl;
  Part_Context(Matrix){
    //cout<<"into matrix part :"<< ARGV.ToFullFormString() << endl;
    Object&matobj = ARGV[1];
    CheckArgsShouldEqual(matobj,1);
    CheckArgsShouldBeString(matobj,1);
    string matname = matobj[1].Key();
    Matrix* matptr = GetMatrix( matname );
    if ( matptr == NULL ){
      ThrowError("Matrix", "Part operation on a matrix which is not initilized");
    }
    Object &partobj = ARGV;
    if ( partobj.Size() -1 != (*matptr).ND ){
      ThrowError("Matrix", "Part indices should be same size with the matrix");
    }
    int ind = get_part_obj_index( partobj, *matptr);
    double v = (*matptr)[ ind ];
    ReturnNumber( v );
  }
  Set_Part_Context{
    // form : Set( c[0,0], 0.3, Matrix("c")     which c = Matrix('c') ahead
    // from : Set( Matrix('c')[0,0], 0.3, Matrix('c') )
    //cout<<" met set context for matrix "<< endl;
    Object&matobj = ARGV[3];
    CheckArgsShouldEqual(matobj,1);
    CheckArgsShouldBeString(matobj,1);
    string matname = matobj[1].Key();
    Matrix* matptr = GetMatrix( matname );
    if ( matptr == NULL ){
      ThrowError("Matrix", "Part operation on a matrix which is not initilized");
    }
    Object &partobj = ARGV[1];
    if ( (partobj.Size() -1) != (*matptr).ND ){
      ThrowError("Matrix", "Part indices should be same size with the matrix");
    }
    //cout<< " partobj = "<< partobj <<endl;
    int ind = get_part_obj_index( partobj, *matptr);
    //cout<<" try set index "<<ind<<" to number:"<< ARGV[2] <<endl;
    double v = double( ARGV[2] );
    (*matptr)[ ind ] = v;
    ReturnNumber( v );
  }
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
      Object2Matrix( operobj[1], *matptr );
      // ARGV.SetTo(1);
      ARGV = ARGV[1];
      ReturnNormal;
    }
    Conjunct_Case( Init ){
      EvaKernel->Evaluate( operobj );
      for (int i=1; i<= operobj.Size(); i++ ){
        CheckArgsShouldBeNumber( operobj, i);
      }
      if ( operobj[1].Number() + 1 > operobj.Size()  ){
        ThrowError("Matrix","Called Init without form Init(dimN,dim1,dim2,..dimN)");
      }
      Matrix* matptr = GetOrNewMatrix( matname );
      int *dim = new int[ int(operobj[1].Number()) ];
      dim[0] = 1;
      for (int i=0; i<= operobj[1].Number(); i++ ){
        dim[i] = operobj[i+1].Number();
      }
      (*matptr).Init(dim);
      delete []dim;

      ReturnObject( ARGV[1] );
    }
    Conjunct_Case( ReadFile ){
      CheckArgsShouldEqual(operobj,1);
      EvaKernel->Evaluate( operobj[1] );
      CheckArgsShouldBeString(operobj,1);
      string filename = operobj[1].Key();
      if ( not FileExistQ( filename ) )
        ThrowError(ARGV.Key(),"data file "+filename+" does not exist.");
      Matrix* matptr = GetOrNewMatrix( matname );
      if ( matptr->ReadFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" does not exist.");
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
        ThrowError(ARGV.Key(),"data file "+filename+" does not exist.");
      Matrix* matptr = GetOrNewMatrix( matname );
      if ( matptr->BinaryReadFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" does not exist.");
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
        ThrowError(ARGV.Key(),"data file "+filename+" does not exist.");
      Matrix* matptr = GetOrNewMatrix( matname );
      if ( matptr->ReadVTKFile(filename)<0 ){
        ThrowError(ARGV.Key(),"data file "+filename+" does not exist.");
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
      //MatrixGet( *matptr, res );
      Matrix2Object( *matptr, res );
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
    //cout<< "get set context for Matrix "<<endl;
    CheckArgsShouldEqual(ARGV[1],1);
    CheckArgsShouldBeString(ARGV[1],1);
    EvaKernel->Evaluate( ARGV[2] );
    Matrix2Object( ARGV[1][1].Key(), ARGV[2] );
    ARGV = ARGV[2];
    ReturnNormal;
  }
  
  CheckShouldNoLessThan(1);
  if ( ARGV[1].StringQ() ){
    // A Single string, only to figure out the name of the matrix, without care about the existence
    // form of Matrix('a')
    if ( ARGV.Size() > 1 )
      ThrowError("Matrix","When specify a Matrix with its name, one argument is required.");
    ReturnNormal;
  }
  ReturnNormal;

}


int MatrixModule::PD_GetMatrixPosition(Object &ARGV){
  CheckArgsShouldEqual(ARGV,0);
  if ( pond::GetDataPosition() == DataDevice )
    ARGV.SetString("Device");
  else if ( pond::GetDataPosition() == DataHostDevice )
    ARGV.SetString("HostDevice");
  else ARGV.SetString("Host");
  ReturnNormal;
}

int MatrixModule::PD_SetMatrixPosition(Object &ARGV){
  CheckArgsShouldEqual(ARGV,1);
  CheckShouldBeString(1);
  if ( ARGV[1].StringQ( "Host" ) )
    pond::SetDataPosition(DataHost);
  else if ( ARGV[1].StringQ("Device") )
    pond::SetDataPosition(DataDevice);
  else if ( ARGV[1].StringQ("HostDevice") )
    pond::SetDataPosition( MatrixHostDevice);
  else{
    ThrowError(ARGV.Key(),"position specification should be one of {\"Host\",\"Device\",\"HostDevice\"}.");
  }
  ReturnNull;
}

int LocalMatrixQ(Object &ARGV,Object &dim){
  if ( not ARGV.ListQ() ){
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

///////////////////////////////////////////////////////////////
// array2list && matrix2object parts
template<class type, class to_type>
static int LocalArray2List(Object &ARGV,type*arr,int *dim,int pdim, int parr){ 
  if ( pdim>dim[0] ){                                                 
    ARGV.SetNumber( (to_type) arr[parr] );                            
    return parr+1;                                                    
  }                                                                   
  ARGV.SetList();                                                     
  for (int i=0; i<dim[pdim]; i++){                                    
    ARGV.PushBackNull();                                              
    parr=LocalArray2List<type,to_type>( ARGV[i+1],arr,dim,pdim+1,parr);             
  }                                                                   
  return parr;                                                        
}

#define ARRAY_2_LIST(type)                                      \
  int MatrixModule::Array2List(Object &ARGV,type*arr,int *dim){ \
    LocalArray2List<type,type>( ARGV,arr,dim,1,0);              \
    return 0;                                                   \
  }
ARRAY_2_LIST(int)
ARRAY_2_LIST(long long)
ARRAY_2_LIST(float)
ARRAY_2_LIST(double)
ARRAY_2_LIST(complex)
#undef ARRAY_2_LIST

#define MATRIX_2_OBJECT(b_type)                                         \
  int MatrixModule::Matrix2Object(Matrix_T<b_type> &matrix, Object &ARGV){ \
    ARGV.SetList();                                                     \
    if ( matrix.Size() != 0 ){                                      \
      int *dim = matrix.NewDimArray();                                  \
      Array2List(ARGV, matrix.Data, dim);                               \
      delete []dim;                                                     \
    }else{                                                              \
      return -1;                                                        \
    }                                                                   \
    return 1;                                                           \
  }
MATRIX_2_OBJECT(int)
MATRIX_2_OBJECT(float)
MATRIX_2_OBJECT(double)
MATRIX_2_OBJECT(complex)
#undef MATRIX_2_OBJECT

int MatrixModule::Matrix2Object(string name,Object &ARGV){//mat of Object form
  Matrix * mat = GetMatrix(name);
  if ( mat == NULL ){
    ARGV.SetNull();
    return 0;
  }else{
    MatrixModule::Matrix2Object(*mat,ARGV);
    return 1;
  }
}

template<class type1>
void localObject2Matrix(Matrix_T<type1>&mat,Object&ARGV,u_long&ind){
  if ( ARGV.ListQ() ){
    for(u_int i=1;i<=ARGV.Size();i++){
      localObject2Matrix(mat,ARGV[i],ind);
    }
    return;
  }else if ( ARGV.NumberQ() ){
    mat.Data[ind] = (type1)ARGV;
    ind++;
    return;
  }
  ThrowError("Object2Matrix","Object is not all numbers.");
}

#define OBJECT_2_MATRIX(type_basic)                                     \
  int MatrixModule::Object2Matrix(Object&ARGV,Matrix_T<type_basic>&matrix){ \
    if ( ARGV.NumberQ() ){                                              \
      if ( matrix.Size() == 0  )                                        \
        ThrowError("Object2Matrix","Assign a number to an empty Matrix is not allowd."); \
      matrix = ARGV.Number();                                           \
    }                                                                   \
    Object dim; dim.SetList();                                          \
	  if ( !MatrixQ( ARGV, dim ) )                                     \
      ThrowError("Object2Matrix","Matrix assignment from list should be of Matrix form."); \
    int n =dim.Size();                                                  \
    int *arr= new int[n+2];                                             \
    arr[0]=n;                                                           \
    for (int i=1; i<=n; i++){                                           \
      arr[i] =  (int)dim[i] ;                                           \
    }                                                                   \
    matrix.Init( arr ,MatrixHost);                                      \
    u_long ind=0;                                                       \
    localObject2Matrix( matrix, ARGV, ind);                             \
    delete []arr;                                                       \
    return 1;                                                           \
  }; 
OBJECT_2_MATRIX(complex);
OBJECT_2_MATRIX(double);
// OBJECT_2_MATRIX(float);
// OBJECT_2_MATRIX(floatcomplex);
// OBJECT_2_MATRIX(int);
#undef OBJECT_2_MATRIX

int MatrixModule::Object2Matrix(Object&ARGV,string name){
  Matrix *mat = GetOrNewMatrix(name);
  return Object2Matrix(ARGV, *mat); 
} 


int MatrixModule::PD_test(Object&argv){
  // test function for Matrix Module
  //cout<<argv<<endl;
  Matrix mat(1,5,MatrixHost);
  mat.ReadFile("m.txt");
  cout<< mat <<endl;
  return 0;
}
