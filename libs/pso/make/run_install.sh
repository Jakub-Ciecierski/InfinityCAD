#!/bin/bash

###############################################

source ./struct_config.sh >/dev/null
source ./version_config.sh >/dev/null
source ./proj_config.sh >/dev/null
source ./lib_name.sh >/dev/null

###############################################

for i in "$@"
do
case $i in
    -i=*|--install-root=*)
    INSTALL_ROOT_PATH="${i#*=}"
    ;;
    *)
            # unknown option
    ;;
esac
done

###############################################

###############################################
# CREATING PATH
###############################################

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

###############################################

PROJECT_ROOT=..

###############################################
# INSTALLING
###############################################

mkdir -p ${INSTALL_ROOT_PATH}

mkdir -p ${INSTALL_LIB_ROOT_PATH}
mkdir -p ${INSTALL_INC_ROOT_PATH}

mkdir -p ${INSTALL_LIB_PATH}
mkdir -p ${INSTALL_INC_PATH}

echo Installing lib to directory ${INSTALL_LIB_PATH}
echo Installing includes to directory ${INSTALL_INC_PATH}

cp -rfu ${PROJECT_ROOT}/${STRUCT_INC}/* ${INSTALL_INC_PATH}
cp ${PROJECT_ROOT}/${STRUCT_OBJ}/${LIB_FULL_NAME} ${INSTALL_LIB_PATH}