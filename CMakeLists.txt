# General parameters
cmake_minimum_required (VERSION 2.6)
project (naoigm C CXX)


####################################
# Settings
####################################
set (CMAKE_SKIP_ASSEMBLY_SOURCE_RULES ON)
set (CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/")
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_RULE_MESSAGES OFF CACHE BOOL "")



####################################
# Other parameters
####################################

find_path(EIGEN_INCLUDE_DIRS "Eigen/Core" PATHS "${SDK_DIR}/include" PATH_SUFFIXES "eigen2")
if (EIGEN_INCLUDE_DIRS)
    include_directories ("${EIGEN_INCLUDE_DIRS}")
endif (EIGEN_INCLUDE_DIRS)
include_directories ("${PROJECT_SOURCE_DIR}/include")


set (IGM_SRC_DIR "${PROJECT_SOURCE_DIR}/nao_model")
set (IGM_SRC "${IGM_SRC_DIR}/JacCoM.c"
             "${IGM_SRC_DIR}/JacCoM_a.c"
             "${IGM_SRC_DIR}/Leg2EE.c"
#             "${IGM_SRC_DIR}/from_LLeg_1.c"
#             "${IGM_SRC_DIR}/from_LLeg_2.c"
             "${IGM_SRC_DIR}/from_LLeg_3.c"
             "${IGM_SRC_DIR}/from_LLeg_4.c"
#             "${IGM_SRC_DIR}/from_RLeg_1.c"
#             "${IGM_SRC_DIR}/from_RLeg_2.c"
             "${IGM_SRC_DIR}/from_RLeg_3.c"
             "${IGM_SRC_DIR}/from_RLeg_4.c"
             "${IGM_SRC_DIR}/igm_versions.cpp"
             "${IGM_SRC_DIR}/igm.cpp")


if (LEG2JOINTS_ENABLE)
    set (L2J_SRC "${IGM_SRC_DIR}/LLeg2JointsEE.c"
                 "${IGM_SRC_DIR}/RLeg2JointsEE.c")
    add_library (leg2joints STATIC ${L2J_SRC})
endif()

add_library (naoigm STATIC ${IGM_SRC})
