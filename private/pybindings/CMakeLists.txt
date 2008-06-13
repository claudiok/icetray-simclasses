if(BUILD_PYBINDINGS)
  i3_add_pybindings(simclasses
    I3MCPMTResponse.cxx
    I3MMCTrack.cxx
    module.cxx
    USE_TOOLS boost python log4cplus
    USE_PROJECTS simclasses icetray
    )
endif(BUILD_PYBINDINGS)

