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
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1628725810/Comport.o \
	${OBJECTDIR}/_ext/1628725810/Firefly.o \
	${OBJECTDIR}/_ext/1628725810/NavigationCommand.o \
	${OBJECTDIR}/_ext/1628725810/Network.o \
	${OBJECTDIR}/_ext/1628725810/RouteStrategy.o \
	${OBJECTDIR}/_ext/1628725810/SensorCommand.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/home/johannes/boost_1_58_0/libs

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/firefly_final

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/firefly_final: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/firefly_final ${OBJECTFILES} ${LDLIBSOPTIONS} -lboost_system -lpthread

${OBJECTDIR}/_ext/1628725810/Comport.o: /home/johannes/Firefly_Final/Comport.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1628725810
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/johannes/boost_1_58_0/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1628725810/Comport.o /home/johannes/Firefly_Final/Comport.cpp

${OBJECTDIR}/_ext/1628725810/Firefly.o: /home/johannes/Firefly_Final/Firefly.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1628725810
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/johannes/boost_1_58_0/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1628725810/Firefly.o /home/johannes/Firefly_Final/Firefly.cpp

${OBJECTDIR}/_ext/1628725810/NavigationCommand.o: /home/johannes/Firefly_Final/NavigationCommand.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1628725810
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/johannes/boost_1_58_0/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1628725810/NavigationCommand.o /home/johannes/Firefly_Final/NavigationCommand.cpp

${OBJECTDIR}/_ext/1628725810/Network.o: /home/johannes/Firefly_Final/Network.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1628725810
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/johannes/boost_1_58_0/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1628725810/Network.o /home/johannes/Firefly_Final/Network.cpp

${OBJECTDIR}/_ext/1628725810/RouteStrategy.o: /home/johannes/Firefly_Final/RouteStrategy.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1628725810
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/johannes/boost_1_58_0/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1628725810/RouteStrategy.o /home/johannes/Firefly_Final/RouteStrategy.cpp

${OBJECTDIR}/_ext/1628725810/SensorCommand.o: /home/johannes/Firefly_Final/SensorCommand.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1628725810
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/johannes/boost_1_58_0/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1628725810/SensorCommand.o /home/johannes/Firefly_Final/SensorCommand.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/johannes/boost_1_58_0/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/firefly_final

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
