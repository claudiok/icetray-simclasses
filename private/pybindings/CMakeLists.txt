if(BUILD_PYBINDINGS)
  i3_add_pybindings(simclasses
    CorsikaLongStep.cxx
    I3CorsikaShowerInfo.cxx
    I3MCPMTResponse.cxx
    I3MMCTrack.cxx
    I3MCPulse.cxx
    I3MCPE.cxx
    module.cxx
    USE_TOOLS boost python
    USE_PROJECTS simclasses icetray tableio
    )
endif(BUILD_PYBINDINGS)

