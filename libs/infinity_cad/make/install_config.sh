#!/bin/bash

###############################################

source ./struct_config.sh >/dev/null
source ./version_config.sh >/dev/null
source ./proj_config.sh >/dev/null
source ./lib_name.sh >/dev/null

###############################################

###############################################
# CREATING PATH
###############################################

# Root directory of the lib
INSTALL_ROOT_PATH=../../../dependencies

# Root of the lib file
INSTALL_LIB_ROOT_PATH=${INSTALL_ROOT_PATH}/lib
# Root of all includes
INSTALL_INC_ROOT_PATH=${INSTALL_ROOT_PATH}/include

# The directory for this lib
INSTALL_LIB_PATH=${INSTALL_LIB_ROOT_PATH}/${PROJ_CODE_NAME}
INSTALL_INC_PATH=${INSTALL_INC_ROOT_PATH}/${PROJ_CODE_NAME}
# Add version if provided
if [ -z "${VERSION}" ]; then
    NOTHING=
else
    INSTALL_LIB_PATH+=-${VERSION}
    INSTALL_INC_PATH+=-${VERSION}
fi