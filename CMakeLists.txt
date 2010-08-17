
i3_project(simclasses)

option(OLD_MMCTRACK "Compile assuming IceSim 1.9.6 names for MMC tracks" OFF)

if(OLD_MMCTRACK)
  add_definitions(-DOLD_MMCTRACK)
endif(OLD_MMCTRACK)

i3_add_library(simclasses
	private/simclasses/*.cxx private/simclasses/converter/*.cxx

	USE_TOOLS log4cplus boost root gsl python

	USE_PROJECTS phys-services dataclasses icetray tableio

	ROOTCINT public/simclasses/MMCWeight.h
)
	
add_subdirectory(private/pybindings)



