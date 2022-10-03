
#	Makefile for MPW (Macintosh Programmers Workshop by Apple)
#
#   File:       libmikmod.make
#   Target:     libmikmod
#
#	Build with: make -f libmikmod.make libmikmod 
#
#	NOTE:		Experimental! Only tried with Metrowerks compilers so far
#
#	Written by Anders F Bj�rklund <afb@algonet.se>

MAKEFILE        = libmikmod.make
libs		 	= :libs:
MikModDir	 	=  "::"

LibName68k		= "libmikmod68K.dll"
StubName68k		= "libmikmod68K.lib"
LibNamePPC		= "libmikmodPPC.dll"
StubNamePPC		= "libmikmodPPC.lib"

LibNameFat		= "libmikmod"
StubNameFat		= "libmikmodStub"
Header          = "mikmod.h"

#------------------------------------------------------------------------------
# Choose your tools and libraries:

# Metrowerks CodeWarrior:
CC68K         = MWC68K
CCPPC         = MWCPPC
LINK68K       = MWLink68k
LINKPPC       = MWLinkPPC

IncLibraries68k = �
			    "{SharedLibraries}InterfaceLib" �
				"{MW68KLibraries}MathLibCFM68K (4i_8d).Lib" �
				"{MW68KLibraries}MSL AppRuntimeCFM68K.Lib" �
				"{MW68KLibraries}MSL MWRuntimeLibCFM68K" �
				"{MW68KLibraries}MSL C.CFM68K Fa(4i_8d).Lib" �
				"{MW68KLibraries}MSL SIOUX.CFM68K.Lib"
		
IncLibrariesPPC = �
			    "{SharedLibraries}InterfaceLib" �
				"{SharedLibraries}MathLib" �
				"{SharedLibraries}SoundLib" �
				"{MWPPCLibraries}MSL RuntimePPC.Lib" �
				"{MWPPCLibraries}MSL C.PPC.Lib" �
				"{MWPPCLibraries}MSL SIOUX.PPC.Lib"

# provided by Apple : (these compilers/linkers are so far untested)
#CC68K         = SC
#CCPPC         = MrC
#LINK68K       = Lib
#LINKPPC       = PPCLink

#IncLibraries68k = �
#				"{SharedLibraries}InterfaceLib" �
#				"{SharedLibraries}StdCLib" �
#				"{SharedLibraries}MathLib" �
#				"{SharedLibraries}SoundLib" �
#				"{PPCLibraries}StdCRuntime.o" �
#				"{PPCLibraries}PPCCRuntime.o" �
#				"{PPCLibraries}PPCToolLibs.o"

				
#IncLibrariesPPC = �
#				"{SharedLibraries}InterfaceLib"
#				"{SharedLibraries}StdCLib" �
#				"{CFM68KLibraries}NuMathLib.o" �
#				"{CFM68KLibraries}NuMacRuntime.o" �
#				"{CFM68KLibraries}NuToolLibs.o" �


#------------------------------------------------------------------------------

Prefix       = -prefix "cwmikmodheaders.h"
Export       = "libmikmod.exp"

Headers      = -i "{MikModDir}include:" �
			   -i "{MikModDir}playercode:"

Sources      =	":" �
				"{MikModDir}playercode:" �
				"{MikModDir}loaders:" �
				"{MikModDir}drivers:" �
				"{MikModDir}mmio:"

obj          = :obj:
objppc	     = :objppc:

Warnings        =   -w noimplicitconv,nopossible,nounusedvar,nounusedarg
Options         =   -EOL -mpw_pointers 

SymOptions		=	-sym off				# turn this on to debug with SADE/R2Db
Debug68kOptions	=	-opt on					# turn this off to build debug 68K code
DebugPPCOptions	=	-opt speed				# turn this off to build debug PowerPC code

LibOptions      =   -t 'shlb' -c 'cfmg' -xm s
LibOptions68k	=	{LibOptions} -model CFMflatdf
LibOptionsPPC	=	{LibOptions} 

IncludesFolders	=	{Prefix} -nosyspath {Headers} 
CCOptions       =   {IncludesFolders} {Options} {Warnings} {SymOptions}
CC68KOptions    =	{CCOptions} {Debug68kOptions} -model cfmflatdf
CCPPCOptions	=	{CCOptions} {DebugPPCOptions}

#------------------------------------------------------------------------------
# These are modified default build rules.  
#------------------------------------------------------------------------------

{obj}			�	{Sources}

.c.o			�	.c
	Echo "# compiling "{Default}.c" using {CC68K}"
	{CC68K} {CC68KOptions} {DepDir}{Default}.c -o {TargDir}{Default}.c.o

{objppc}		�	{Sources}

.c.x			�	.c
	Echo "# compiling "{Default}.c" using {CCPPC}"
	{CCPPC} {CCPPCOptions} {DepDir}{Default}.c -o {TargDir}{Default}.c.x

#------------------------------------------------------------------------------
# These are the objects that we want to include in the library.
#------------------------------------------------------------------------------

LibObjects68k		=	�
					{obj}strdup.c.o �
					{obj}mmalloc.c.o �
					{obj}mmerror.c.o �
					{obj}mmio.c.o �
					{obj}mdreg.c.o �
					{obj}mdriver.c.o �
					{obj}mlreg.c.o �
					{obj}mlutil.c.o �
					{obj}mloader.c.o �
					{obj}mplayer.c.o �
					{obj}munitrk.c.o �
					{obj}mwav.c.o �
					{obj}npertab.c.o �
					{obj}sloader.c.o �
					{obj}virtch_common.c.o �
					{obj}virtch.c.o �
					{obj}virtch2.c.o �
					{obj}load_669.c.o �
					{obj}load_amf.c.o �
					{obj}load_dsm.c.o �
					{obj}load_far.c.o �
					{obj}load_gdm.c.o �
					{obj}load_imf.c.o �
					{obj}load_it.c.o �
					{obj}load_m15.c.o �
					{obj}load_med.c.o �
					{obj}load_mod.c.o �
					{obj}load_mtm.c.o �
					{obj}load_s3m.c.o �
					{obj}load_stm.c.o �
					{obj}load_stx.c.o �
					{obj}load_ult.c.o �
					{obj}load_uni.c.o �
					{obj}load_xm.c.o �
					{obj}drv_nos.c.o �
					{obj}drv_stdout.c.o �
					{obj}drv_raw.c.o �
					{obj}drv_wav.c.o �
					{obj}drv_mac.c.o
					

LibObjectsPPC	=	�
					{objppc}strdup.c.x �
					{objppc}mmalloc.c.x �
					{objppc}mmerror.c.x �
					{objppc}mmio.c.x �
					{objppc}mdreg.c.x �
					{objppc}mdriver.c.x �
					{objppc}mlreg.c.x �
					{objppc}mlutil.c.x �
					{objppc}mloader.c.x �
					{objppc}mplayer.c.x �
					{objppc}munitrk.c.x �
					{objppc}mwav.c.x �
					{objppc}npertab.c.x �
					{objppc}sloader.c.x �
					{objppc}virtch_common.c.x �
					{objppc}virtch.c.x �
					{objppc}virtch2.c.x �
					{objppc}load_669.c.x �
					{objppc}load_amf.c.x �
					{objppc}load_dsm.c.x �
					{objppc}load_far.c.x �
					{objppc}load_gdm.c.x �
					{objppc}load_imf.c.x �
					{objppc}load_it.c.x �
					{objppc}load_m15.c.x �
					{objppc}load_med.c.x �
					{objppc}load_mod.c.x �
					{objppc}load_mtm.c.x �
					{objppc}load_s3m.c.x �
					{objppc}load_stm.c.x �
					{objppc}load_stx.c.x �
					{objppc}load_ult.c.x �
					{objppc}load_uni.c.x �
					{objppc}load_xm.c.x �
					{objppc}drv_nos.c.x �
					{objppc}drv_stdout.c.x �
					{objppc}drv_raw.c.x �
					{objppc}drv_wav.c.x �
					{objppc}drv_mac.c.x


#------------------------------------------------------------------------------
# These are the targets.
#------------------------------------------------------------------------------

ALL				    � {LibNameFat} {StubNameFat} {Header}

{Header}		    �
	Duplicate -y 'mikmod_build.h' {Header}

{LibName68k}		�� {LibObjects68k}
	Echo "# Building 680X0 shared library"
	{LINK68K} {LibObjects68k} {IncLibraries68k} {SymOptions} {LibOptions68k} -f {Export} -o {Targ}

{StubName68k}		� {Export}
	MakeStub -arch m68k {Export} -o {StubName68k}
	
{LibNamePPC}		�� {LibObjectsPPC}
	Echo "# Building PowerPC shared library"
	{LINKPPC} {LibObjectsPPC} {IncLibrariesPPC} {SymOptions} {LibOptionsPPC} -f {Export} -o {Targ}

{StubNamePPC}		� {Export}
	MakeStub -arch pwpc {Export} -o {StubNamePPC}

{LibNameFat}		� {MAKEFILE} {LibName68k} {LibNamePPC}
	Echo "# Building Fat shared library (merging)"
	Duplicate -y {LibName68k} {LibNameFat}
	MergeFragment {LibNamePPC} {LibNameFat}
	Delete -i {LibName68k}
	Delete -i {LibNamePPC}
	Rez  'libmikmodversion.r' -append -o {LibNameFat}
    SetFile -a C {LibNameFat}

{StubNameFat}		� {Export} {StubName68k} {StubNamePPC}
	MakeStub -arch fat {Export} -o {StubNameFat}
	Delete -i {StubName68k}
	Delete -i {StubNamePPC}