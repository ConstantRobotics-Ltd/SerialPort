cmake_minimum_required(VERSION 3.13)



################################################################################
## 3RD-PARTY
## dependencies for the project
################################################################################
project(3rdparty LANGUAGES CXX)



################################################################################
## SETTINGS
## basic 3rd-party settings before use
################################################################################
# To inherit the top-level architecture when the project is used as a submodule.
SET(PARENT ${PARENT}_3RDPARTY_)
# Disable self-overwriting of parameters inside included subdirectories.
SET(${PARENT}_SUBMODULE_CACHE_OVERWRITE OFF CACHE BOOL "" FORCE)



################################################################################
## CONFIGURATION
## 3rd-party submodules configuration
################################################################################
SET(${PARENT}_SUBMODULE_TRACER ON CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_TRACER)
    SET(${PARENT}_TRACER                                 ON CACHE BOOL "" FORCE)
    SET(${PARENT}_TRACER_EXAMPLES                       OFF CACHE BOOL "" FORCE)
endif()



################################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the 3rd-party configuration
################################################################################
if (${PARENT}_SUBMODULE_TRACER)
    add_subdirectory(Tracer)
endif()

