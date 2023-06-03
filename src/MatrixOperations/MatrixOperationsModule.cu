#include "sys/time.h"
#include "pond.h"
#include "MPIModule.h"
#include "cufft.h"
#include "MatrixOperationsModule.h"

#include <fftw3-mpi.h>

using namespace std;
using namespace pond;

__DEBUG_MAIN__("input.pd");

MPIModule* pond::MatrixOperationsModule::mpiModule = NULL;

int mark_time(string info = "") {
    static timeval timemark0, timemark1;
    if (info == "") {
        gettimeofday(&timemark0, NULL);
        gettimeofday(&timemark1, NULL);
    } else {
        gettimeofday(&timemark1, NULL);
        double diff_time = (timemark1.tv_sec - timemark0.tv_sec) +
                    (timemark1.tv_usec - timemark0.tv_usec) / 1000.0 / 1000.0;
        cout << info << " : " << diff_time << endl;
        timemark0 = timemark1;
    }
    return 0;
}

MatrixOperationsModule::MatrixOperationsModule() : Module(MODULE_NAME) {
    mpiModule = (pond::MPIModule*)(EvaKernel->ModulePtr("MPI"));
}

MatrixOperationsModule::~MatrixOperationsModule() {}

int MatrixOperationsModule::PD_multiThreadsTest(Object& Argv) {
    SetDataPosition(MatrixHost);
    int N = Argv[1].Number();
    int n[3] = {N, N, N};
    // FloatComplexMatrix in_data,out_data,data,data_hat;
    ComplexMatrix in_data, out_data, data, data_hat;
    // debug data init
    mark_time();
    cout << "Init data" << endl;
    in_data.Init(3, N, N, N, MatrixHostDevice);
    out_data.Init(3, N, N, N, MatrixHostDevice);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                in_data(i, j, k) = sin(i + j + k);
            }
        }
    }
    //in_data.DumpFile("in.dat");
    cout << "Init data done" << endl;
    cout << "Test multiple threads transform " << endl;
    mark_time();
    MatrixOperationsModule::FFT3D(N, N, N, in_data, out_data);
    mark_time("Multiple threads FFT forward "+ToString(EvaSettings.threadNumberPerKernel) +" threads:");
    MatrixOperationsModule::FFT3D(N, N, N, out_data, in_data, pond::MatrixOperations_FFT_BACKWARD);
    mark_time("Multiple threads FFT backward "+ToString(EvaSettings.threadNumberPerKernel) +" threads:");
    // MatrixOperationsModule::FFT(2, n, in_data, out_data);

    //cout << "transform done result stored in out.dat" << endl;
    //out_data.DumpFile("out.dat");

    ////////////////////////////////////
    //  data = in_data;
    //  data.HostToDevice();
    //  pond::SetDataPosition(MatrixDevice);
    //  FFT3D( n[0], n[1], n[2], data, data, MatrixOperations_FFT_FORWARD );
    //  data.DeviceToHost();
    //  data.DumpFile("1.dat");
    //  ////////////////////////////////////
    //  FFT3D( n[0], n[1], n[2], data, data, MatrixOperations_FFT_BACKWARD );
    //  data.DeviceToHost();
    //  data.DumpFile("2.dat");
    ReturnNull;
}


void MatrixOperationsModule::Init() {}

const int PlanType_gpu_cpu = 0;
const int PlanType_float_gpu_cpu = 1;

const int PlanType_mpi = 2;
const int PlanType_float_mpi = 3;

static bool mpiFFTInited = false;
static void mpi_fft_init() {
    if (not mpiFFTInited) {
        mpiFFTInited = true;
        fftw_mpi_init();
    }
}

template <class type>
void MPI_FFT_MatrixInit(Matrix_T<type>& mat,
                        int rank,
                        int n[],
                        int ttype,
                        int batch) {
    mpi_fft_init();
    ptrdiff_t alloc_local, local_n0, local_0_start, nn[3];
    long dist = 1;
    for (int i = 0; i < rank; i++) {
        dist *= n[i];
        nn[i] = n[i];
    }
    if (ttype == PlanType_mpi) {
        alloc_local = fftw_mpi_local_size_many(
            rank, nn, batch, dist, MPI_COMM_WORLD, &local_n0, &local_0_start);
    } else {
        alloc_local = fftwf_mpi_local_size_many(
            rank, nn, batch, dist, MPI_COMM_WORLD, &local_n0, &local_0_start);
    }
    mat.Init(1, alloc_local, MatrixHostDevice);
    // mat.ReShape(3, n1/mpiModule->rankSize, n2, n3);
}

class planRecord {
   public:
    int rank;
    int type;  //
    int batch;
    int n[3];
    cufftHandle gpu_plan;
    fftw_plan cpu_plan_forward, cpu_plan_backward;
    fftwf_plan fcpu_plan_forward, fcpu_plan_backward;
    ComplexMatrix mat;
    FloatComplexMatrix fmat;
    planRecord(int rank, int* inn, int type, int batch = 1) {
        this->rank = rank;
        this->type = type;
        this->batch = batch;
        int stride = 1, dist = 1;
        for (int i = 0; i < rank; i++) {
            this->n[i] = inn[i];
            dist *= inn[i];
        }
        // if mpi plan
        if (type == PlanType_mpi) {  // mpi
            MPI_FFT_MatrixInit(mat, rank, n, type, batch);
            ptrdiff_t nn[3];
            for (int i = 0; i < rank; i++)
                nn[i] = n[i];
            cpu_plan_forward = fftw_mpi_plan_many_dft(
                rank, nn, batch, dist, dist, (fftw_complex*)mat.Data,
                (fftw_complex*)mat.Data, MPI_COMM_WORLD, FFTW_FORWARD,
                FFTW_ESTIMATE);
            cpu_plan_backward = fftw_mpi_plan_many_dft(
                rank, nn, batch, dist, dist, (fftw_complex*)mat.Data,
                (fftw_complex*)mat.Data, MPI_COMM_WORLD, FFTW_BACKWARD,
                FFTW_ESTIMATE);
        } else if (type == PlanType_float_mpi) {  // float mpi
            MPI_FFT_MatrixInit(fmat, rank, n, type, batch);
            ptrdiff_t nn[3];
            for (int i = 0; i < rank; i++)
                nn[i] = n[i];
            fcpu_plan_forward = fftwf_mpi_plan_many_dft(
                rank, nn, batch, dist, dist, (fftwf_complex*)fmat.Data,
                (fftwf_complex*)fmat.Data, MPI_COMM_WORLD, FFTW_FORWARD,
                FFTW_ESTIMATE);
            fcpu_plan_backward = fftwf_mpi_plan_many_dft(
                rank, nn, batch, dist, dist, (fftwf_complex*)fmat.Data,
                (fftwf_complex*)fmat.Data, MPI_COMM_WORLD, FFTW_BACKWARD,
                FFTW_ESTIMATE);
        } else if (type == PlanType_gpu_cpu) {  // gpu cpu
            mat.Init(1, batch * dist, MatrixHostDevice);
            if (pond::GetParallelMode() == ParallelModeGpu) {
                cufftPlanMany(&gpu_plan, rank, n, n, stride, dist, n, stride,
                              dist, CUFFT_Z2Z, batch);
                CUDA_LAST_ERROR();
            }
            fftw_plan_with_nthreads(EvaSettings.threadNumberPerKernel);
            cpu_plan_forward =
                fftw_plan_many_dft(rank, inn, batch, (fftw_complex*)mat.Data, n,
                                   stride, dist, (fftw_complex*)mat.Data, n,
                                   stride, dist, FFTW_FORWARD, FFTW_ESTIMATE);
            fftw_plan_with_nthreads(EvaSettings.threadNumberPerKernel);
            cpu_plan_backward =
                fftw_plan_many_dft(rank, inn, batch, (fftw_complex*)mat.Data, n,
                                   stride, dist, (fftw_complex*)mat.Data, n,
                                   stride, dist, FFTW_BACKWARD, FFTW_ESTIMATE);
        } else {  // float gpu cpu
            fmat.Init(1, batch * dist, MatrixHostDevice);
            if (pond::GetParallelMode() == ParallelModeGpu) {
                cufftPlanMany(&gpu_plan, rank, n, n, stride, dist, n, stride,
                              dist, CUFFT_C2C, batch);
                CUDA_LAST_ERROR();
            }
            fftw_plan_with_nthreads(EvaSettings.threadNumberPerKernel);
            fcpu_plan_forward = fftwf_plan_many_dft(
                rank, inn, batch, (fftwf_complex*)fmat.Data, n, stride, dist,
                (fftwf_complex*)fmat.Data, n, stride, dist, FFTW_FORWARD,
                FFTW_ESTIMATE);
            fftw_plan_with_nthreads(EvaSettings.threadNumberPerKernel);
            fcpu_plan_backward = fftwf_plan_many_dft(
                rank, inn, batch, (fftwf_complex*)fmat.Data, n, stride, dist,
                (fftwf_complex*)fmat.Data, n, stride, dist, FFTW_BACKWARD,
                FFTW_ESTIMATE);
        }
    }
    ~planRecord() {}
    bool planQ(int rank, int inn[], int type, int batch = 1) {
        if (n[0] == inn[0] and n[1] == inn[1] and n[2] == inn[2] and
            this->batch == batch and this->type == type)
            return true;
        return false;
    }
    void execute(ComplexMatrix& in_mat,
                 ComplexMatrix& out_mat,
                 MatrixOperationsType dire) {
        if (type == PlanType_gpu_cpu and
            pond::GetDataPosition() == MatrixDevice) {
            if (dire == MatrixOperations_FFT_FORWARD)
                cufftExecZ2Z(gpu_plan, (cufftDoubleComplex*)in_mat.DataDevice,
                             (cufftDoubleComplex*)out_mat.DataDevice,
                             CUFFT_FORWARD);
            else
                cufftExecZ2Z(gpu_plan, (cufftDoubleComplex*)in_mat.DataDevice,
                             (cufftDoubleComplex*)out_mat.DataDevice,
                             CUFFT_INVERSE);
            CUDA_LAST_ERROR();
        } else {
            mat = in_mat;
            if (dire == MatrixOperations_FFT_FORWARD) {
                fftw_execute(cpu_plan_forward);
            } else {
                fftw_execute(cpu_plan_backward);
            }
            out_mat = mat;
        }
    }
    void execute(FloatComplexMatrix& in_mat,
                 FloatComplexMatrix& out_mat,
                 MatrixOperationsType dire) {
        if (type == PlanType_float_gpu_cpu and
            pond::GetDataPosition() == MatrixDevice) {
            if (dire == MatrixOperations_FFT_FORWARD)
                cufftExecC2C(gpu_plan, (cufftComplex*)in_mat.DataDevice,
                             (cufftComplex*)out_mat.DataDevice, CUFFT_FORWARD);
            else
                cufftExecC2C(gpu_plan, (cufftComplex*)in_mat.DataDevice,
                             (cufftComplex*)out_mat.DataDevice, CUFFT_INVERSE);
            CUDA_LAST_ERROR();
        } else {
            fmat = in_mat;
            if (dire == MatrixOperations_FFT_FORWARD) {
                fftwf_execute(fcpu_plan_forward);
            } else {
                fftwf_execute(fcpu_plan_backward);
            }
            out_mat = fmat;
        }
    }
    void execute(Matrix& in_mat, ComplexMatrix& out_mat) {
        if (type == PlanType_gpu_cpu and
            pond::GetDataPosition() == MatrixDevice) {
            out_mat = in_mat;
            cufftExecZ2Z(gpu_plan, (cufftDoubleComplex*)out_mat.DataDevice,
                         (cufftDoubleComplex*)out_mat.DataDevice,
                         CUFFT_FORWARD);
            CUDA_LAST_ERROR();
        } else {
            mat = in_mat;
            fftw_execute(cpu_plan_forward);
            out_mat = mat;
        }
    }
    void execute(FloatMatrix& in_mat, FloatComplexMatrix& out_mat) {
        if (type == PlanType_float_gpu_cpu and
            pond::GetDataPosition() == MatrixDevice) {
            out_mat = in_mat;
            cufftExecC2C(gpu_plan, (cufftComplex*)out_mat.DataDevice,
                         (cufftComplex*)out_mat.DataDevice, CUFFT_FORWARD);
            CUDA_LAST_ERROR();
        } else {
            fmat = in_mat;
            fftwf_execute(fcpu_plan_forward);
            out_mat = fmat;
        }
    }
    void execute(ComplexMatrix& in_mat, Matrix& out_mat) {
        if (type == PlanType_gpu_cpu and
            pond::GetDataPosition() == MatrixDevice) {
            if (not mat.SameDimensionQ(in_mat))
                mat.Init(in_mat);
            cufftExecZ2Z(gpu_plan, (cufftDoubleComplex*)in_mat.DataDevice,
                         (cufftDoubleComplex*)mat.DataDevice, CUFFT_INVERSE);
            out_mat = mat;
            CUDA_LAST_ERROR();
        } else {
            mat = in_mat;
            fftw_execute(cpu_plan_backward);
            out_mat = mat;
        }
    }
    void execute(FloatComplexMatrix& in_mat, FloatMatrix& out_mat) {
        if (type == PlanType_gpu_cpu and
            pond::GetDataPosition() == MatrixDevice) {
            if (not fmat.SameDimensionQ(in_mat))
                fmat.Init(in_mat);
            cufftExecC2C(gpu_plan, (cufftComplex*)in_mat.DataDevice,
                         (cufftComplex*)fmat.DataDevice, CUFFT_INVERSE);
            out_mat = fmat;
            CUDA_LAST_ERROR();
        } else {
            fmat = in_mat;
            fftwf_execute(fcpu_plan_backward);
            out_mat = fmat;
        }
    }
};
static int planTablePtr;
static vector<planRecord*> planTable;

planRecord* getPlanReady(int rank, int n[], int type, int batch = 1) {
    if (type == PlanType_mpi or type == PlanType_float_mpi) {
        mpi_fft_init();
    }
    bool found = false;
    planRecord* plan;
    // try find in table
    if (planTable.size() > 0) {
        if (planTable[planTablePtr]->planQ(rank, n, type, batch)) {
            found = true;
        } else {
            planTablePtr = 0;
            while (true) {
                if (planTable[planTablePtr]->planQ(rank, n, type, batch)) {
                    found = true;
                    break;
                }
                planTablePtr++;
                if (planTablePtr == (int)planTable.size())
                    break;
            }
        }
    }
    // not find
    if (not found) {
        static bool fftw_threads_initialized = false;
        if (not fftw_threads_initialized &&
            pond::GetParallelMode() == ParallelModeCpu) {
            fftw_init_threads();
            fftw_threads_initialized = true;
        }
        plan = new planRecord(rank, n, type, batch);
        planTable.push_back(plan);
        return plan;
    } else {
        return planTable[planTablePtr];
    }
}

void MatrixOperationsModule::FFT(int rank,
                                 int n[],
                                 ComplexMatrix& in_mat,
                                 ComplexMatrix& out_mat,
                                 MatrixOperationsType direction) {
    if (not in_mat.SameDimensionQ(out_mat)) {
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    }
    planRecord* plan;
    int batch, product = 1;
    for (int i = 0; i < rank; i++)
        product *= n[i];
    batch = in_mat.Size() / product;
    if (batch * product != in_mat.Size()) {
        ThrowError("FFT3D",
                   "Input matrix is not consistent with transformation size.");
    }
    plan = getPlanReady(rank, n, PlanType_gpu_cpu, batch);
    plan->execute(in_mat, out_mat, direction);
    return;
}

void MatrixOperationsModule::FFT(int rank,
                                 int n[],
                                 FloatComplexMatrix& in_mat,
                                 FloatComplexMatrix& out_mat,
                                 MatrixOperationsType direction) {
    if (not in_mat.SameDimensionQ(out_mat)) {
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    }
    planRecord* plan;
    int batch, product = 1;
    for (int i = 0; i < rank; i++)
        product *= n[i];
    batch = in_mat.Size() / product;
    if (batch * product != in_mat.Size()) {
        ThrowError("FFT3D",
                   "Input matrix is not consistent with transformation size.");
    }
    plan = getPlanReady(rank, n, PlanType_float_gpu_cpu, batch);
    plan->execute(in_mat, out_mat, direction);
    return;
}

void MatrixOperationsModule::FFT3D(int n1,
                                   int n2,
                                   int n3,
                                   ComplexMatrix& in_mat,
                                   ComplexMatrix& out_mat,
                                   MatrixOperationsType direction) {
    if (not in_mat.SameDimensionQ(out_mat)) {
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    }
    planRecord* plan;
    int n[3] = {n1, n2, n3}, batch;
    batch = in_mat.Size() / (n1 * n2 * n3);
    if (batch * n1 * n2 * n3 != in_mat.Size()) {
        ThrowError("FFT3D",
                   "Input matrix is not consistent with transformation size.");
    }
    plan = getPlanReady(3, n, PlanType_gpu_cpu, batch);
    plan->execute(in_mat, out_mat, direction);
    return;
}

void MatrixOperationsModule::FFT3D(int n1,
                                   int n2,
                                   int n3,
                                   FloatComplexMatrix& in_mat,
                                   FloatComplexMatrix& out_mat,
                                   MatrixOperationsType direction) {
    if (not in_mat.SameDimensionQ(out_mat))
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    planRecord* plan;
    int n[3] = {n1, n2, n3}, batch;
    batch = in_mat.Size() / (n1 * n2 * n3);
    if (batch * n1 * n2 * n3 != in_mat.Size())
        ThrowError("FFT3D",
                   "Input matrix is not consistent with transformation size.");
    plan = getPlanReady(3, n, PlanType_float_gpu_cpu, batch);
    plan->execute(in_mat, out_mat, direction);
    return;
}

void MatrixOperationsModule::FFT3D(int n1,
                                   int n2,
                                   int n3,
                                   Matrix& in_mat,
                                   ComplexMatrix& out_mat) {
    if (not in_mat.SameDimensionQ(out_mat))
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    planRecord* plan;
    int n[3] = {n1, n2, n3}, batch;
    batch = in_mat.Size() / (n1 * n2 * n3);
    if (batch * n1 * n2 * n3 != in_mat.Size())
        ThrowError("FFT3D",
                   "Input matrix is not consistent with transformation size.");
    plan = getPlanReady(3, n, PlanType_gpu_cpu, batch);
    plan->execute(in_mat, out_mat);
    return;
}

void MatrixOperationsModule::FFT3D(int n1,
                                   int n2,
                                   int n3,
                                   FloatMatrix& in_mat,
                                   FloatComplexMatrix& out_mat) {
    if (not in_mat.SameDimensionQ(out_mat))
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    planRecord* plan;
    int n[3] = {n1, n2, n3}, batch;
    batch = in_mat.Size() / (n1 * n2 * n3);
    if (batch * n1 * n2 * n3 != in_mat.Size())
        ThrowError("FFT3D",
                   "Input matrix is not consistent with transformation size.");
    plan = getPlanReady(3, n, PlanType_float_gpu_cpu, batch);
    plan->execute(in_mat, out_mat);
    return;
}

void MatrixOperationsModule::FFT3D(int n1,
                                   int n2,
                                   int n3,
                                   ComplexMatrix& in_mat,
                                   Matrix& out_mat) {
    if (not in_mat.SameDimensionQ(out_mat))
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    planRecord* plan;
    int n[3] = {n1, n2, n3}, batch;
    batch = in_mat.Size() / (n1 * n2 * n3);
    if (batch * n1 * n2 * n3 != in_mat.Size())
        ThrowError("FFT3D",
                   "Input matrix is not consistent with transformation size.");
    plan = getPlanReady(3, n, PlanType_gpu_cpu, batch);
    plan->execute(in_mat, out_mat);
    return;
}

void MatrixOperationsModule::FFT3D(int n1,
                                   int n2,
                                   int n3,
                                   FloatComplexMatrix& in_mat,
                                   FloatMatrix& out_mat) {
    if (not in_mat.SameDimensionQ(out_mat))
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    planRecord* plan;
    int n[3] = {n1, n2, n3}, batch;
    batch = in_mat.Size() / (n1 * n2 * n3);
    if (batch * n1 * n2 * n3 != in_mat.Size())
        ThrowError("FFT3D",
                   "Input matrix is not consistent with transformation size.");
    plan = getPlanReady(3, n, PlanType_float_gpu_cpu, batch);
    plan->execute(in_mat, out_mat);
    return;
}

void MatrixOperationsModule::MPI_FFT3D(int n1,
                                       int n2,
                                       int n3,
                                       ComplexMatrix& in_mat,
                                       ComplexMatrix& out_mat,
                                       MatrixOperationsType direction) {
    if (not in_mat.SameDimensionQ(out_mat))
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    if ((mpiModule->rankSize * in_mat.Size()) != (n1 * n2 * n3))
        ThrowError("FFT3D", "Matrix size does not match transformation size.");
    if ((mpiModule->rankSize * in_mat.D1) != n1)
        ThrowError("FFT3D",
                   "Divided Matrix size of Dimension 1st does not match "
                   "transformation size.");
    mpi_fft_init();
    planRecord* plan;
    int n[3] = {n1, n2, n3};
    plan = getPlanReady(3, n, PlanType_mpi, 1);
    if (pond::GetDataPosition() == MatrixHost) {
        plan->execute(in_mat, out_mat, direction);
        out_mat.ReShape(3, n1 / mpiModule->rankSize, n2, n3);
    } else {
        cout << "acc plan apply in Device. Code not ready do nothing.\n"
             << endl;
    }
    return;
}

void MatrixOperationsModule::MPI_FFT3D(int n1,
                                       int n2,
                                       int n3,
                                       Matrix& in_mat,
                                       ComplexMatrix& out_mat) {
    if (not in_mat.SameDimensionQ(out_mat))
        ThrowError(
            "FFT3D",
            "Input matrix and output matrix should have same dimension.");
    if ((mpiModule->rankSize * in_mat.Size()) != (n1 * n2 * n3))
        ThrowError("FFT3D", "Matrix size does not match transformation size.");
    if ((mpiModule->rankSize * in_mat.D1) != n1)
        ThrowError("FFT3D",
                   "Divided Matrix size of Dimension 1st does not match "
                   "transformation size.");
    mpi_fft_init();
    planRecord* plan;
    int n[3] = {n1, n2, n3};
    plan = getPlanReady(3, n, PlanType_mpi, 1);
    plan->execute(in_mat, out_mat);
    out_mat.ReShape(3, n1 / mpiModule->rankSize, n2, n3);
    return;
}

int MatrixOperationsModule::PD_MatrixMPI_FFT3DC2C(Object& argv) {
    ReturnNull;
}
