#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/outputOptions.o \
	${OBJECTDIR}/treeManipulation.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wall
CXXFLAGS=-Wall

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=/opt/lib/libxerces-c-3.1.dylib /opt/local/lib/libboost_program_options-mt.dylib /opt/local/lib/libboost_regex-mt.dylib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/comp4_assignment7_davidlordan_v1

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/comp4_assignment7_davidlordan_v1: /opt/lib/libxerces-c-3.1.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/comp4_assignment7_davidlordan_v1: /opt/local/lib/libboost_program_options-mt.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/comp4_assignment7_davidlordan_v1: /opt/local/lib/libboost_regex-mt.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/comp4_assignment7_davidlordan_v1: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/comp4_assignment7_davidlordan_v1 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/opt/include -I/opt/boost_1_57_0 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/outputOptions.o: outputOptions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/opt/include -I/opt/boost_1_57_0 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/outputOptions.o outputOptions.cpp

${OBJECTDIR}/treeManipulation.o: treeManipulation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/opt/include -I/opt/boost_1_57_0 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/treeManipulation.o treeManipulation.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/comp4_assignment7_davidlordan_v1

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
