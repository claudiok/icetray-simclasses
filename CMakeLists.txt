
i3_project(simclasses)

i3_add_library(simclasses
	private/simclasses/*.cxx 
	private/simclasses/converter/*.cxx

	USE_TOOLS log4cplus boost python

	USE_PROJECTS phys-services dataclasses icetray tableio
)
	
add_subdirectory(private/pybindings)



