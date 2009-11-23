import os

Env = Environment()
Env['ENV']['PATH'] = os.environ['PATH']
Env['ENV']['LIB'] = os.environ['LIB']
Env['ENV']['INCLUDE'] = os.environ['INCLUDE']

RmanTreePath = os.environ['RMANTREE']
if( RmanTreePath == '' ) :
	print "ERROR : Can't find system variable RMANTREE, you must setup it."
	exit()

BoostIncPath = os.environ['BOOST_ROOT']
if( BoostIncPath == '' ) :
	print "ERROR : Can't find system variable BOOST_ROOT, you must setup it."
	exit()

CudaIncPath = os.environ['CUDA_INC_PATH']
if( CudaIncPath == '' ) :
	print "ERROR : You have to install CUDA Toolkit."
	exit()
	
STANNIncPath = Dir('E:/SDK/Inc/stann')

RPSLibPath = Dir(RmanTreePath+'/lib')
RPSIncPath = Dir(RmanTreePath+'/include')

GSLIncPath = Dir('E:/SDK/Inc')
GSLLibPath = Dir('E:/SDK/Lib/x64')

ThrustIncPath = Dir('E:/SDK/Inc')

CC64 = '/DWIN32 /D_WIN32 /DUNICODE /D_UNICODE /DENABLE_OPENMP /DDLL /Ox /EHa /GF /GL /GR /MD /openmp'
CC64 = '/DWIN64 /D_WIN64 /DUNICODE /D_UNICODE /DENABLE_OPENMP /DDLL /Ox /EHa /GF /GL /GR /MD /openmp'

Libs32 = ['libprman','libgsl32','libgslcblas32s']
Libs64 = ['libprman','libgsl64s','libgslcblas64s']

SourceFiles = ['DPR.cpp','EPR.cpp','OPR.cpp','PPR.cpp','PR.cpp','KPR']

Env.SharedLibrary(target='PR4PR',source=SourceFiles,CPPPATH=[RPSIncPath,GSLIncPath,BoostIncPath,STANNIncPath],LIBPATH=[RPSLibPath,GSLLibPath],LIBS=Libs32,CCFLAGS=CC32)
