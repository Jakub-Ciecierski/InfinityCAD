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
RELEASE_ROOT_PATH=..

# The directory for this lib
RELEASE_PATH=${RELEASE_ROOT_PATH}/${PROJ_CODE_NAME}
# Add version if provided
if [ -z "${VERSION}" ]; then
    NOTHING=
else
    RELEASE_PATH+=-${VERSION}
fi

mkdir -p ${RELEASE_ROOT_PATH}
mkdir -p ${RELEASE_PATH}

echo Releasing to ${RELEASE_PATH}