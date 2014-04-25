
i3_project(simclasses)

SET(LIB_${PROJECT_NAME}_SOURCEFILES 
  private/simclasses/CorsikaLongStep.cxx
  private/simclasses/I3MMCTrack.cxx
  private/simclasses/I3MCPMTResponse.cxx
  private/simclasses/I3MCNKGInfo.cxx
  private/simclasses/I3CorsikaShowerInfo.cxx
  private/simclasses/I3MCPulse.cxx
  private/simclasses/I3MCPE.cxx
  private/simclasses/I3MCPEConverters.cxx
  )


SET(LIB_${PROJECT_NAME}_PROJECTS icetray dataclasses)
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

i3_test_executable(test 
  private/test/*.cxx
  USE_PROJECTS icetray simclasses
  USE_TOOLS boost python
  )

i3_add_pybindings(simclasses
  private/pybindings/CorsikaLongStep.cxx
  private/pybindings/I3CorsikaShowerInfo.cxx
  private/pybindings/I3MCPMTResponse.cxx
  private/pybindings/I3MMCTrack.cxx
  private/pybindings/I3MCPulse.cxx
  private/pybindings/I3MCPE.cxx
  private/pybindings/module.cxx
  USE_TOOLS boost python
  USE_PROJECTS simclasses icetray tableio
  )
