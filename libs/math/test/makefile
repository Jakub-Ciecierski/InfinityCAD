######################################################################
#--------------------------------1-----------------------------------#
#------------------------ VARIABLES SECTION -------------------------#
#--------------------------------------------------------------------#
######################################################################

######################################################################
#------------------ DYNAMIC SUBSECTION - CAN MODIFY -----------------#
######################################################################

PROJECT_ROOT = ..

EXTERNAL_LIBS_INCLUDE_PATH=${INSTALL_ROOT_PATH}/include
EXTERNAL_LIBS_LIB_PATH=${INSTALL_ROOT_PATH}/lib

#---------------- UNIT TESTS ------------------#

# Path to root of googletest project
GTEST_ROOT = ${PROJECT_ROOT}/../../libs/googletest

#--------------- APPLICATION NAME -------------#

APP_NAME = utest

#--------------- FILE EXTENSIONS --------------#

# c++ extension
CPP_EXT = .cpp
# cuda extension
CUDA_EXT = .cu

#-------------- PROJECT STRUCTURE -------------#

# Your header files
INC = include
# Your local libraries to be linked with the application
LIB = lib
# Build dumper. All your object files (*.o) will be located here
OBJ = build
# Source files and only source files, subdirectories are supported.
SRC = src
# Executables of your Application
BIN = bin

#------------- EXTERNAL MAKEFILES -------------#

# Paths to the Makefiles to be ran before this Makefile.
# Default target will be executed.
# Can be used to compile your static libraries before linking.
# Surrounded the path with '...' and separate different paths
# with a space.
MAKE_EXTERNAL = '$(GTEST_ROOT)/make/'

#---------------- LIBRARIES -------------------#

# Paths and names of libraries to link with
LIBS += $(GTEST_ROOT)/make/gtest_main.a -lpthread

#----------------- INCLUDES -------------------#

# Paths to all Includes
INCLUDES += -I $(GTEST_ROOT)/include

#------------------- FLAGS --------------------#

# Use '-static' for static libraries
# Use '-shared' for shared libraries
L_FLAGS +=
C_FLAGS += -g -std=c++11
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

# Variable representing comma ','
COMMA = ,
# The flags needed for ld path
LD_SHARED_PATH_FLAG = -Wl$(COMMA)-rpath

# Adds each module in lib folder to search for libs
EXTERNAL_LIB_MODULES =\
 $(shell find ${EXTERNAL_LIBS_LIB_PATH} \
 -type d 2>/dev/null)

# Add link flag to all LIB_MODULES
EXTERNAL_LIB_LINK_MODULES = $(addprefix -L , $(EXTERNAL_LIB_MODULES))

# The paths to all shared libraries
EXTERNAL_LIB_LINK_RPATH = $(addprefix ${LD_SHARED_PATH_FLAG} , $(EXTERNAL_LIB_MODULES))

# Include modules of this application
INC_MODULES = $(addprefix -I, $(shell find ${INC} -type d 2>/dev/null))

# Include modules of tested application
TST_INC_MODULES = $(addprefix -I, $(shell find ../${INC} -type d 2>/dev/null))

LIBS += -L ${LIB} ${EXTERNAL_LIB_LINK_MODULES} ${EXTERNAL_LIB_LINK_RPATH}
# It is not a mistake to include source files in unit tests!
INCLUDES += -I ${INC} -I ../${INC} \
			${EXTERNAL_LIBS_INCLUDE_MODULES}

CC = g++
NVCC = nvcc 

L_FLAGS += ${LIBS}
C_FLAGS += ${INCLUDES} -MMD
NVCC_FLAGS +=

# All source files
# [2>/dev/null] is added to supresss the 'file not found'
# message when inititing project structure for the first time
# (src file does not exist at the beginning)
CPP_SOURCES += $(shell find ${SRC} -name *${CPP_EXT} 2>/dev/null)
CUDA_SOURCES = $(shell find ${SRC} -name *${CUDA_EXT} 2>/dev/null)

# List of all subdirectories in source directory 
MODULES += $(shell find ${SRC} -type d 2>/dev/null)

# All objects to be compiled
OBJECTS += $(addprefix ${OBJ}/,$(notdir $(CPP_SOURCES:${CPP_EXT}=.o)))
OBJECTS += $(addprefix ${OBJ}/,$(notdir $(CUDA_SOURCES:${CUDA_EXT}=.o)))

# Add modules to vpath
VPATH = ${MODULES}

#----------------- TESTED OBJECTS -------------------#

# Find all objects from the main application being tested.
TST_SRCS += $(shell find ../${SRC} -name *${CPP_EXT} 2>/dev/null)
TST_OBJS_TMP += $(addprefix ../${OBJ}/,$(notdir $(TST_SRCS:${CPP_EXT}=.o)))

# Remove main.o. We don't want 2 main functions. One is included in gtests.
TST_OBJS = $(filter-out %main.o, ${TST_OBJS_TMP})

######################################################################
#---------------------------------2----------------------------------#
#------------------------------ RULES -------------------------------#
#--------------------------------------------------------------------#
######################################################################

######################################################################
#------------------ STATIC SUBSECTION - DO NOT MODIFY ---------------#
######################################################################

#------------------ DEFAULT -------------------#

# Release rule
all: link run

# Links, does not run
link: $(MAKE_EXTERNAL) info_prefix init ${APP_NAME} info_suffix

# Runs the tests
run:
	@echo ${INFO_SEPARATOR}
	@echo 'Running Unit Tests'
	@echo ${INFO_SEPARATOR}
	@$(BIN)/$(APP_NAME)
	@echo ${INFO_SEPARATOR}

#------------------ LINKER --------------------#

${APP_NAME}: ${OBJECTS}
	@$(CC) -o ${BIN}/${APP_NAME} ${TST_OBJS} $^ $(L_FLAGS)

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

#------------------ EXTERNALS -----------------#

# Make external makefiles
$(MAKE_EXTERNAL):
	@echo
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@echo 'Compiling External: $@'
	@echo ${INFO_SEPARATOR}
	@$(MAKE) -C $@
	@echo ${INFO_SEPARATOR}
	@echo 'Compiling External $@ Finished'
	@echo

#---------------- UTILITY RULES ---------------#

# Creates all the directories of the project
init:
	@mkdir -p ${OBJ} ${BIN} ${SRC} ${LIB} ${INC} ${TST}

clean: 
	@$(RM) *.o *~ ${OBJ}/*.o ${OBJ}/*.d ${OBJ}/*~ ${BIN}/*


.PHONY: $(SUBDIRS) clean

######################################################################
#------------------ DYNAMIC SUBSECTION - CAN MODIFY -----------------#
######################################################################

#----------------- PRINT RULES ----------------#

info_prefix:
	@echo
	@echo
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@echo 
	@echo ${INFO_FILLER}"MAKING:" ${APP_NAME}
	@echo 
	@echo

info_suffix:
	@echo
	@echo ${INFO_SEPARATOR}
	@echo
	@echo ${INFO_FILLER}"LINKING SUCCESSFULL"
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
# Must be inserted at the end of the makefile (TODO verify)
-include $(OBJECTS:.o=.d)
