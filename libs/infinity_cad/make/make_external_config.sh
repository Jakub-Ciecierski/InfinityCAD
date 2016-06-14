#!/bin/bash

######################################################################
#-------------------------- MAKE EXTERNALS --------------------------#
######################################################################

# Add paths to dir including makefile (not the makefile itself)
# Optionally Specify a rule
# E.g. MAKE_EXTERNAL = /path/to/make/ install
MAKE_EXTERNAL[0]=../../glm/make/
MAKE_EXTERNAL_RULE[0]=install

MAKE_EXTERNAL[1]=../../math/make/
MAKE_EXTERNAL_RULE[1]=install

MAKE_EXTERNAL[2]=../../ifc_gpu/make/
MAKE_EXTERNAL_RULE[2]=install

MAKE_EXTERNAL[3]=../../pso/make/
MAKE_EXTERNAL_RULE[3]=install