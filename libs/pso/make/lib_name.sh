#!/bin/bash

######################################
#
# Using all the configurations constructs
# full name of the library.
# Uses:
#   LIB_PREFIX      = e.g. standard: 'lib'
#   PROJ_CODE_NAME  = e.g. 'yolo'
#   VERSION         = optionally provided e.g. '1.0'
#   LIB_EXTENSION   = e.g. '.so.
#
#   LIB_FULL_NAME   = libyolo-1.0.so
######################################

source ./lib_config.sh
source ./proj_config.sh
source ./version_config.sh

getLibFullName(){
    echo ${LIB_FULL_NAME}
}

# Add prefix and code name
LIB_FULL_NAME=${LIB_PREFIX}${PROJ_CODE_NAME}

# Add version if provided
if [ -z "${VERSION}" ]; then
    NOTHING=
else
    LIB_FULL_NAME+=-${VERSION}
fi

# Add extension
LIB_FULL_NAME+=${LIB_EXTENSION}

getLibFullName