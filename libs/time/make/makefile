######################################################################
#--------------------------------------------------------------------#
#---------------------------- COMMENTS ------------------------------#
#--------------------------------------------------------------------#
######################################################################

#---------------------------------------------------------------------
#
# Structure.
#
# This Makefile is split into:
# 1) Variables Section
# 	1.1) Dynamic subsection
# 	1.2) Static subsection - DO NOT MODIFY
# 2) Rules Section
# 	2.1) Static subsection - DO NOT MODIFY
# 	2.2) Dynamic subsection
# 3) Dependency Graph - DO NOT MODIFY
#
#
# As a user of this makefile You should not be modifing
# any of the static subsections.
#
#---------------------------------------------------------------------
#
# Compilers supported:
#
# 1) g++
# 2) nvcc
#
#---------------------------------------------------------------------
#
# Supports a particular project structure:
# Refer to Project Structure section to define the following folders:
#
# 1) INC: Header files of this application
# 2) LIB: Your, local Libraries to be linked with application
# 3) BLD: Build where your compiled object files are stored
# 4) SRC: Source files of this application
# 5) BIN: Binary files, executables of the application
#
#---------------------------------------------------------------------
#
# Tips:
#
# Use '\' to break new line
# Take special care of whitespaces, it might cause errors.
# $@ is the value of target
# $^ is the list of all prerequisites of the rule
# $< is just the first prerequisite
#
#---------------------------------------------------------------------
#
# TODO:
# (Lack of these features might cause bugs)
#
# Realese / Debug
# Support for gcc for .c files
# Currently only one extension per XX_EXT variable applies
# Support for unit tests
# Variable printouts
# Add usage
#
#---------------------------------------------------------------------
#
# Author:
#
# github.com/Jakub-Ciecierski
# jakub.ciecier@gmail.com
#
#---------------------------------------------------------------------
#
# For further information refer to:
# http://www.gnu.org/software/make/manual/make.html
#
#---------------------------------------------------------------------
#
# Changes log:
# 01.07.2015 - Created
# 26.10.2015 - Added Unit Test rule
# 08.11.2015 - OpenGL and CUDA libraries commented out
# 12.11.2015 - Added each module in include separately.
#	For example.
#	File:
#		./include/math/clustering/kmeans.h
#	can be included as:
#		#include "kmeans.h"
#	or:
#		#include "math/clustering/kmeans.h"
#
# 15.12.2015 - Added install rule
# 11.01.2016 - Made new clean functionality for libraries
# 11.01.2016 - Automatic header include and library
#				paths for all libraries in lib/
# 11.01.2016 - Added Config section.
#---------------------------------------------------------------------

######################################################################
#--------------------------------0-----------------------------------#
#---------------------- INCLUDE CONFIG FILES ------------------------#
#--------------------------------------------------------------------#
######################################################################

PROJECT_ROOT = ..
PATH_TO_CONFIGS = ${PROJECT_ROOT}/make

# Default value
INSTALL_ROOT_PATH := $(shell pwd)/${PROJECT_ROOT}/../../dependencies

# Path to directory containing
# common external libs in custom directory
EXTERNAL_LIBS_INCLUDE_PATH=${INSTALL_ROOT_PATH}/include

# Include make configurations
include ${PATH_TO_CONFIGS}/proj_config.sh
include ${PATH_TO_CONFIGS}/lib_config.sh
include ${PATH_TO_CONFIGS}/struct_config.sh
include ${PATH_TO_CONFIGS}/compiler_config.sh
include ${PATH_TO_CONFIGS}/make_external_config.sh

# Get runners scripts handles
RUN_INSTALL_SCRIPT:=${PATH_TO_CONFIGS}/run_install.sh -i=${INSTALL_ROOT_PATH}
RUN_RELEASE_SCRIPT:=${PATH_TO_CONFIGS}/run_release.sh
LIB_NAME_SCRIPT:=${PATH_TO_CONFIGS}/lib_name.sh

RUN_MAKE_EXTERNAL_SCRIPT:=${PATH_TO_CONFIGS}/run_make_external.sh -i=${INSTALL_ROOT_PATH}

# Get a full name of the lib
LIB_FULL_NAME=$(shell $(LIB_NAME_SCRIPT))

######################################################################
#--------------------------------0-----------------------------------#
#--------------------------- RULE NAMES -----------------------------#
#--------------------------------------------------------------------#
######################################################################

INSTALL_RULE_NAME=install
RELEASE_RULE_NAME=release
EXTERNALS_RULE_NAME=externals

######################################################################
#--------------------------------0-----------------------------------#
#-------------------------- CONFIG SECTION --------------------------#
#--------------------------------------------------------------------#
######################################################################

PROJ_NAME := $(PROJ_NAME)
PROJ_CODE_NAME := $(PROJ_CODE_NAME)

######################################################################
#--------------------------------1-----------------------------------#
#------------------------ VARIABLES SECTION -------------------------#
#--------------------------------------------------------------------#
######################################################################

######################################################################
#------------------ DYNAMIC SUBSECTION - CAN MODIFY -----------------#
######################################################################

#--------------- FILE EXTENSIONS --------------#

# c++ extension
CPP_EXT := ${COMPILER_CC_EXT}
# cuda extension
CUDA_EXT := ${COMPILER_NVCC_EXT}

#-------------- PROJECT STRUCTURE -------------#

INC = ${PROJECT_ROOT}/${STRUCT_INC}
SRC = ${PROJECT_ROOT}/${STRUCT_SRC}
LIB = ${PROJECT_ROOT}/${STRUCT_LIB}
OBJ = ${PROJECT_ROOT}/${STRUCT_OBJ}
TST = ${PROJECT_ROOT}/${STRUCT_TST}
DOC = ${PROJECT_ROOT}/${STRUCT_DOC}

#------------- EXTERNAL MAKEFILES -------------#

# Paths to the Makefiles to be ran before this Makefile.
# Default target will be executed.
# Can be used to compile your static libraries before linking.
# Surrounded the path with '...' and separate different paths
# with a space.
#MAKE_EXTERNAL := ${MAKE_EXTERNAL}

#---------------- LIBRARIES -------------------#

# Paths and names of libraries to link with
LIBS += -lpthread

#----------------- INCLUDES -------------------#

# Paths to all Includes
INCLUDES +=

#------------------- FLAGS --------------------#

# Use '-static' for static libraries
# Use '-shared' for shared libraries
LD_FLAGS +=
C_FLAGS += -std=c++11 -Wall
NVCC_FLAGS += -arch=sm_20

#----------------- PRINT OUTS -----------------#

# The separator will be printed between compiling objects
INFO_SEPARATOR = "|-------------------------------------------------------------------|"
INFO_FILLER = "  <<  "


######################################################################
#------------------ STATIC SUBSECTION - DO NOT MODIFY ---------------#
######################################################################

# Finds all libraries in libs/include path
EXTERNAL_LIBS_INCLUDE_MODULES=$(addprefix -I ,\
$(shell find ${EXTERNAL_LIBS_INCLUDE_PATH} -maxdepth 1 -type d 2>/dev/null))

#----------------- COMPILERS -----------------#

CC = ${COMPILER_CC}
NVCC = ${COMPILER_NVCC}

#----------------- PROJECT MODULES -----------------#

# List of all subdirectories in source directory
SRC_MODULES = $(shell find ${SRC} -type d 2>/dev/null)
# Adds each modules in include separately.
INC_MODULES =  $(addprefix -I , $(shell find ${INC} -type d 2>/dev/null))

#----------------- LIBRARIES MODULES -----------------#

# Variable representing comma ','
COMMA = ,
# The flags needed for ld path
LD_SHARED_PATH_FLAG = -Wl$(COMMA)-rpath

# Adds each module in lib folder to search for libs
LIB_MODULES = $(shell find ${LIB} -type d 2>/dev/null)

# Add link flag to all LIB_MODULES
LIB_LINK_MODULES = $(addprefix -L , $(LIB_MODULES))

# The paths to all shared libraries
LIB_LINK_RPATH = $(addprefix ${LD_SHARED_PATH_FLAG} , $(LIB_MODULES))

# All Lib modules to include
LIB_INC_MODULES = $(addprefix -I , $(shell find ${LIB} -type d 2>/dev/null))

LIBS += ${LIB_LINK_MODULES} ${LIB_LINK_RPATH}
INCLUDES += ${LIB_INC_MODULES} -I ${INC} ${EXTERNAL_LIBS_INCLUDE_MODULES}

#----------------- COMPILER FLAGS -----------------#

LD_FLAGS += -${LIB_TYPE} ${LIBS}
C_FLAGS += -fPIC -g -MMD ${INCLUDES}
NVCC_FLAGS +=

#----------------- SOURCE/OBJECT FILES -----------------#

# All source files
# [2>/dev/null] is added to supresss the 'file not found'
# message when initiating project structure for the first time
# (src file does not exist at the beginning)
CPP_SOURCES = $(shell find ${SRC} -name *${CPP_EXT} 2>/dev/null)
CUDA_SOURCES = $(shell find ${SRC} -name *${CUDA_EXT} 2>/dev/null)

# All objects to be compiled
OBJECTS += $(addprefix ${OBJ}/,$(notdir $(CPP_SOURCES:${CPP_EXT}=.o)))
OBJECTS += $(addprefix ${OBJ}/,$(notdir $(CUDA_SOURCES:${CUDA_EXT}=.o)))

# Add modules to vpath
VPATH = ${SRC_MODULES}

######################################################################
#---------------------------------2----------------------------------#
#------------------------------ RULES -------------------------------#
#--------------------------------------------------------------------#
######################################################################

######################################################################
#------------------ STATIC SUBSECTION - DO NOT MODIFY ---------------#
######################################################################

################################################
#----------------- MAIN RULES -----------------#
################################################

# All
all: compile_lib run_test

compile_lib: $(EXTERNALS_RULE_NAME) compile_objects \
			info_prefix ${PROJ_CODE_NAME} info_suffix

compile_objects: init_project ${OBJECTS}

${INSTALL_RULE_NAME}: compile_lib
	@$(RUN_INSTALL_SCRIPT)

${RELEASE_RULE_NAME}: compile_lib
	@$(RUN_RELEASE_SCRIPT)

compile_test:
	@$(MAKE) link -C $(TST)

# Unit Test rule, assumes that $(TST) has a makefile inside
run_test:
	@$(MAKE) INSTALL_ROOT_PATH=${INSTALL_ROOT_PATH} -C $(TST)

################################################
#--------------- LINKER / COMPILER ------------#
################################################

#------------------ LINKER --------------------#

# Used in debug
${PROJ_CODE_NAME}: ${OBJECTS}
	@$(CC) -o ${OBJ}/${LIB_FULL_NAME} $^ $(LD_FLAGS)

#----------------- COMPILERS ------------------#

# cpp compiler
$(OBJ)/%.o: %${CPP_EXT}
	@echo ${INFO_SEPARATOR}
	@echo 'COMPILING: ' $<
	@echo
	@$(CC) $(C_FLAGS) -c -o $@ $<

# cuda compiler
${OBJ}/%.o: %${CUDA_EXT}
	@echo ${INFO_SEPARATOR}
	@echo 'COMPILING: ' $<
	@echo
	@$(NVCC) $(NVCC_FLAGS) -c -o $@ $<

################################################
#----------------- EXTERNALS ------------------#
################################################

${EXTERNALS_RULE_NAME}:
	@echo
	@echo "Starting Externals"
	@echo
	@$(RUN_MAKE_EXTERNAL_SCRIPT)
	@echo
	@echo "Finishing Externals"
	@echo

######################################################################
#------------------ DYNAMIC SUBSECTION - CAN MODIFY -----------------#
######################################################################

################################################
#-------------- INITIALIZATION ----------------#
################################################

init_all: init_project

# Creates all the directories of the project
init_project:
	@mkdir -p ${OBJ} ${SRC} ${LIB} ${INC} ${TST} ${DOC}

################################################
#------------------- CLEAN --------------------#
################################################

clean: clean_obj
	@$(MAKE) -C $(TST) clean

clean_obj:
	$(eval OBJECTS=$(shell find ${OBJ} -name *.o))
	$(eval LIBS=$(shell find ${OBJ} -name *.so))
	$(eval DEPS=$(shell find ${OBJ} -name *.d))
	$(eval TMPS=$(shell find ${OBJ} -name *.~))
	@echo ${INFO_SEPARATOR}
	@echo
	@echo ${INFO_FILLER} "Cleaning: ${OBJ}/"
	@echo
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@echo
	@echo "Object files:"
	@echo
	@$(foreach var, ${OBJECTS}, echo ${var};)
	@echo
	@echo ${INFO_SEPARATOR}
	@echo
	@echo "Dependency files:"
	@echo
	@$(foreach var, ${DEPS}, echo ${var};)
	@echo
	@echo
	@echo ${INFO_SEPARATOR}
	@echo
	@echo "Shared Library files:"
	@echo
	@$(foreach var, ${LIBS}, echo ${var};)
	@echo
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@rm -rf ${OBJ}

# Cleans all compiled objects within the lib directory
clean_lib:
	$(eval OBJS=$(shell find ${LIB} -name *.o))
	$(eval DEPS=$(shell find ${LIB} -name *.d))
	$(eval ST_LIBS=$(shell find ${LIB} -name *.a))
	$(eval SH_LIBS=$(shell find ${LIB} -name *.so))
	@echo ${INFO_SEPARATOR}
	@echo
	@echo ${INFO_FILLER} "Cleaning: ${LIB}/"
	@echo
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@echo
	@echo "Object files:"
	@echo
	@$(foreach var, ${OBJS}, echo ${var};)
	@echo
	@echo ${INFO_SEPARATOR}
	@echo
	@echo "Dependency files:"
	@echo
	@$(foreach var, ${DEPS}, echo ${var};)
	@echo
	@echo ${INFO_SEPARATOR}
	@echo
	@echo "Static Libraries:"
	@echo
	@$(foreach var, ${ST_LIBS}, echo ${var};)
	@echo
	@echo ${INFO_SEPARATOR}
	@echo
	@echo "Shared Libraries:"
	@echo
	@$(foreach var, ${SH_LIBS}, echo ${var};)
	@echo
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@$(foreach var, ${OBJS}, rm -f ${var})
	@$(foreach var, ${DEPS}, rm -f ${var})
	@$(foreach var, ${ST_LIBS}, rm -f ${var})
	@$(foreach var, ${SH_LIBS}, rm -f ${var})

clean_all: clean clean_lib

.PHONY: $(SUBDIRS) clean

################################################
#----------------- PRINT RULES-----------------#
################################################

info_prefix:
	@echo
	@echo
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@echo
	@echo ${INFO_FILLER}"MAKING:" ${PROJ_NAME}
	@echo
	@echo

info_suffix:
	@echo
	@echo ${INFO_SEPARATOR}
	@echo
	@echo ${INFO_FILLER}"COMPILATION SUCCESSFULL"
	@echo
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@echo

######################################################################
#---------------------------------3----------------------------------#
#---------------- DEPENDENCY GRAPH - DO NOT MODIFY ------------------#
#--------------------------------------------------------------------#
######################################################################

#--------------- DEPENDENCY GRAPH -------------#

# Automatic dependency graph generation
# Must be inserted at the end of the makefile
-include $(OBJECTS:.o=.d)
