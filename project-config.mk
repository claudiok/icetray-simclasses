#
# $Id: project-config.mk 13108 2005-11-30 13:26:31Z troy $
#
#
# A template make configuration file for your project.
#

#
# directories under "private", listed in CXX_BIN_SRCDIR_NAMES,
# that contain source will have all source in each directory compiled
# and linked into a binary, one per directory, linked to the project
# library.   The mechanism is the same as that which builds the tests,
# but there is no testsuite driver program provided and the binaries
# are not run by the target all-tests-run
#
CXX_BIN_SRCDIR_NAMES :=

#
# Public directories for these projects are added to the #include path,
# and libraries are added to the link.  The toplevel make will determine
# your project's dependencies based on the contents of this variable
# and build the projects in the correct order.
#
USES_PROJECTS := dataclasses icetray

#
# same as above, but for external tools.  Paths found in config.mk.  
# You may add your own tools here:  If so you will need to add
# appropriate paths in config.mk
#
USES_TOOLS := 

#
# If you have a LinkDef.h, the build system will make a root
# dictionary.  The variable ROOT_DICTIONARY_HEADERS defines which
# header files will be visible to rootcint during the dictionary
# generation process.
#
ROOT_DICTIONARY_HEADERS :=
