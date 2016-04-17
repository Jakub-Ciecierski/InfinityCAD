#!/bin/bash

source ./make_external_config.sh >/dev/null

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

# Run externals
for (( p=0; p<${#MAKE_EXTERNAL[@]}; p++ )) do
    make ${MAKE_EXTERNAL_RULE[$p]} -C ${MAKE_EXTERNAL[$p]} \
    INSTALL_ROOT_PATH=${INSTALL_ROOT_PATH} >/dev/null
    ERROR_CODE=$?

    if [ $ERROR_CODE -ne 0 ]; then
        break;
    fi
done

exit ${ERROR_CODE}