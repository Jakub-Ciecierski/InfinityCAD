#!/bin/bash

###############################################################
# TODO: Add Unit Tests
# TODO: PROJECT_ROOT=. does not work when in 'make' directory
###############################################################

source ./release_config.sh >/dev/null

###############################################

PROJECT_ROOT=..

###############################################
# INSTALLATION
###############################################

# Includes
cp -rf ${PROJECT_ROOT}/${STRUCT_INC} ${RELEASE_PATH}/${STRUCT_INC}

# Lib
mkdir -p ${RELEASE_PATH}/${STRUCT_OBJ}
cp -rf ${PROJECT_ROOT}/${STRUCT_OBJ}/${LIB_FULL_NAME} ${RELEASE_PATH}/${STRUCT_OBJ}

# Sources
cp -rf ${PROJECT_ROOT}/${STRUCT_SRC} ${RELEASE_PATH}/${STRUCT_SRC}
# Libraries
cp -rf ${PROJECT_ROOT}/${STRUCT_LIB} ${RELEASE_PATH}/${STRUCT_LIB}

# Make
cp -rf ${PROJECT_ROOT}/${STRUCT_MAKE} ${RELEASE_PATH}/${STRUCT_MAKE}

# Documentation
cp -rf ${PROJECT_ROOT}/${STRUCT_DOC} ${RELEASE_PATH}/${STRUCT_DOC}

cp ${PROJECT_ROOT}/README.txt ${RELEASE_PATH}/
cp ${PROJECT_ROOT}/INSTALL.txt ${RELEASE_PATH}/