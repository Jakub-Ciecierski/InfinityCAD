#--------------------------------------------
# SETTINGS
#--------------------------------------------

PROJECT_NAME=.

INSTALL_ROOT_PATH=$(shell pwd)/${PROJECT_NAME}
INSTALL_LIB_ROOT_PATH := ${INSTALL_ROOT_PATH}/dependencies

#--------------------------------------------
# RULE NAMES
#--------------------------------------------

# Cleans builds of apps and libs
CLEAN=clean
# Cleans builds of apps
CLEAN_APPS=clean_apps
# Cleans builds of libs
CLEAN_LIBS=clean_libs

# Builds apps and libs
MAKE_ALL=make_all
# Makes apps
MAKE_APPS=make_apps
# Makes libs
MAKE_LIBS=make_libs

# Install both libs and apps
INSTALL=install
# Installs apps
INSTALL_APPS=install_apps
# Installs libs
INSTALL_LIBS=install_libs

# Cleans dependencies
CLEAN_DEPENDENCIES=clean_deps

#--------------------------------------------
# FOLDERS
#--------------------------------------------

APPS=apps
LIBS=libs
DEPENDENCIES=dependencies

#--------------------------------------------
# APPS/LIBS
#--------------------------------------------

# Find all makefiles in apps
APPS_MODULES = \
$(addsuffix '/make' , $(shell find ${APPS} -mindepth 1 -maxdepth 1 \
! -name '*_qt' -type d 2>/dev/null))

# Find all makefiles in libs
LIBS_MODULES = \
$(addsuffix '/make' , $(shell find ${LIBS} -mindepth 1 -maxdepth 1 \
! -name 'googletest' -type d 2>/dev/null))

#--------------------------------------------
# EXTERNAL RULE
#--------------------------------------------

# Set the rule name to run for all makefiles

RULE_NAME := all

ifeq ($(MAKECMDGOALS),${CLEAN})
	RULE_NAME := clean
endif

ifeq ($(MAKECMDGOALS),${CLEAN_APPS})
	RULE_NAME := clean
endif

ifeq ($(MAKECMDGOALS),${CLEAN_LIBS})
	RULE_NAME := clean
endif

ifeq ($(MAKECMDGOALS),${INSTALL})
	RULE_NAME := install
endif

ifeq ($(MAKECMDGOALS),${INSTALL_APPS})
	RULE_NAME := install
endif

ifeq ($(MAKECMDGOALS),${INSTALL_LIBS})
	RULE_NAME := install
endif

#--------------------------------------------
# DEPENDENCIES
#--------------------------------------------

DEPENDENCIES_INCLUDES =\
$(shell find ${DEPENDENCIES}/include/  \
-mindepth 1 -maxdepth 1)

DEPENDENCIES_LIBS =\
$(shell find ${DEPENDENCIES}/lib/ \
-mindepth 1 -maxdepth 1)

#--------------------------------------------
# RULES
#--------------------------------------------

${MAKE_ALL}: ${MAKE_LIBS} ${MAKE_APPS}

${MAKE_APPS}: ${APPS_MODULES}

${MAKE_LIBS}: ${LIBS_MODULES}


${CLEAN}: ${CLEAN_LIBS} ${CLEAN_APPS}

${CLEAN_APPS}: ${APPS_MODULES}

${CLEAN_LIBS}: ${LIBS_MODULES}

${INSTALL}: ${INSTALL_LIBS} #${INSTALL_APPS}


${INSTALL_APPS}: ${APPS_MODULES}

${INSTALL_LIBS}: ${LIBS_MODULES}

${CLEAN_DEPENDENCIES}:
	@echo "Removing:"
	@$(foreach var, ${DEPENDENCIES_INCLUDES}, echo ${var};)
	@$(foreach var, ${DEPENDENCIES_LIBS}, echo ${var};)
	@$(foreach var, ${DEPENDENCIES_INCLUDES}, rm -rf ${var})
	@$(foreach var, ${DEPENDENCIES_LIBS}, rm -rf ${var})
	@rm -rf ${INSTALL_LIB_ROOT_PATH}

#--------------------------------------------

$(APPS_MODULES):
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@echo 'Compiling External: $@'
	@echo ${INFO_SEPARATOR}
	@$(MAKE) ${RULE_NAME} -C $@ INSTALL_ROOT_PATH=${INSTALL_ROOT_PATH}
	@echo ${INFO_SEPARATOR}
	@echo 'Compiling External $@ Finished'
	@echo

$(LIBS_MODULES):
	@echo ${INFO_SEPARATOR}
	@echo ${INFO_SEPARATOR}
	@echo 'Compiling External: $@'
	@echo ${INFO_SEPARATOR}
	@$(MAKE) ${RULE_NAME} -C $@ INSTALL_ROOT_PATH=${INSTALL_LIB_ROOT_PATH}
	@echo ${INFO_SEPARATOR}
	@echo 'Compiling External $@ Finished'
	@echo



#@cp -rf resources ${INSTALL_ROOT_PATH}
#@cp INSTALLED_README.txt ${INSTALL_ROOT_PATH}/README.txt