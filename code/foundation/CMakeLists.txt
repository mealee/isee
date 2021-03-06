
# https://cmake.org/cmake/help/v3.0/command/include_directories.html
# The include directories are added to the INCLUDE_DIRECTORIES directory property for the current CMakeLists file. They are also added to the INCLUDE_DIRECTORIES target property for each target in the current CMakeLists file. The target property values are the ones used by the generators.
include_directories(${PROJECT_SOURCE_DIR}/code)

# library foundation
file(GLOB SOURCE_FILES_ 
    ${PROJECT_SOURCE_DIR}/code/foundation/*.h
	${PROJECT_SOURCE_DIR}/code/foundation/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/*.c)
source_group("" FILES ${SOURCE_FILES_})

file(GLOB SOURCE_FILES_CORE 
    ${PROJECT_SOURCE_DIR}/code/foundation/core/*.h
	${PROJECT_SOURCE_DIR}/code/foundation/core/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/core/*.c)
source_group("core" FILES ${SOURCE_FILES_CORE})

file(GLOB SOURCE_FILES_MEMORY 
    ${PROJECT_SOURCE_DIR}/code/foundation/memory/*.h
 	${PROJECT_SOURCE_DIR}/code/foundation/memory/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/memory/*.c)
source_group("memory" FILES ${SOURCE_FILES_MEMORY})

file(GLOB SOURCE_FILES_PLATFORM 
    ${PROJECT_SOURCE_DIR}/code/foundation/platform/*.h 
	${PROJECT_SOURCE_DIR}/code/foundation/platform/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/platform/*.c)
source_group("platform" FILES ${SOURCE_FILES_PLATFORM})

file(GLOB SOURCE_FILES_CONTAINER 
    ${PROJECT_SOURCE_DIR}/code/foundation/container/*.h 
	${PROJECT_SOURCE_DIR}/code/foundation/container/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/container/*.c)
source_group("container" FILES ${SOURCE_FILES_CONTAINER})

file(GLOB SOURCE_FILES_UTIL 
    ${PROJECT_SOURCE_DIR}/code/foundation/util/*.h 
	${PROJECT_SOURCE_DIR}/code/foundation/util/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/util/*.c)
source_group("util" FILES ${SOURCE_FILES_UTIL})

file(GLOB SOURCE_FILES_MATH 
    ${PROJECT_SOURCE_DIR}/code/foundation/math/*.h 
	${PROJECT_SOURCE_DIR}/code/foundation/math/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/math/*.c)
source_group("math" FILES ${SOURCE_FILES_MATH})

file(GLOB SOURCE_FILES_ALGORITHM
    ${PROJECT_SOURCE_DIR}/code/foundation/algorithm/*.h 
	${PROJECT_SOURCE_DIR}/code/foundation/algorithm/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/algorithm/*.c)
source_group("algorithm" FILES ${SOURCE_FILES_ALGORITHM})

file(GLOB SOURCE_FILES_RUNTIME
    ${PROJECT_SOURCE_DIR}/code/foundation/runtime/*.h 
	${PROJECT_SOURCE_DIR}/code/foundation/runtime/*.inl 
	${PROJECT_SOURCE_DIR}/code/foundation/runtime/*.c)
source_group("runtime" FILES ${SOURCE_FILES_RUNTIME})



add_library(foundation STATIC 
    ${SOURCE_FILES_}
    ${SOURCE_FILES_CORE}
    ${SOURCE_FILES_MEMORY}
    ${SOURCE_FILES_PLATFORM}
    ${SOURCE_FILES_CONTAINER}
    ${SOURCE_FILES_UTIL}
    ${SOURCE_FILES_MATH}
    ${SOURCE_FILES_ALGORITHM}
    ${SOURCE_FILES_RUNTIME}
    )


