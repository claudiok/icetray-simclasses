
i3_project(simclasses)

SET(LIB_${PROJECT_NAME}_SOURCEFILES private/simclasses/*.cxx)
SET(LIB_${PROJECT_NAME}_PROJECTS icetray dataclasses phys-services)
SET(LIB_${PROJECT_NAME}_TOOLS boost python)

if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/tableio)
	LIST(APPEND LIB_${PROJECT_NAME}_PROJECTS tableio)
	LIST(APPEND LIB_${PROJECT_NAME}_SOURCEFILES private/simclasses/converter/*.cxx)
	ADD_DEFINITIONS(-DUSE_TABLEIO)
	MESSAGE(STATUS "     + tableio support")
endif(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/tableio)

i3_add_library(${PROJECT_NAME} ${LIB_${PROJECT_NAME}_SOURCEFILES}
	USE_TOOLS ${LIB_${PROJECT_NAME}_TOOLS}
	USE_PROJECTS ${LIB_${PROJECT_NAME}_PROJECTS}
)

add_subdirectory(private/pybindings)

