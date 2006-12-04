#
# $Id: Makefile 22821 2006-08-10 01:05:02Z blaufuss $
#
# This is a generic makefile, and should most probably not be changed.  
#

ifndef I3_WORK
$(error please set I3_WORK to your icecube work directory.)
endif
ifeq ($(shell test -d $(I3_WORK) || echo -n NO),NO)
  $(error I3_WORK is set ($(I3_WORK)), but the directory does not exist)
endif

ifndef I3_PORTS
$(error please set I3_PORTS to your icecube tools directory.)
endif
ifeq ($(shell test -d $(I3_PORTS) || echo -n NO),NO)
  $(error I3_PORTS is set ($(I3_PORTS)), but the directory does not exist)
endif

include $(I3_WORK)/offline-mk/config.mk

include $(PROJECT_CONFIG_FILENAME)

include $(I3_WORK)/offline-mk/defs.mk
include $(I3_WORK)/offline-mk/targets.mk




