# Makefile for the ROOT test programs.
# This Makefile shows nicely how to compile and link applications
# using the ROOT libraries on all supported platforms.
#
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

RC     := root-config
ifeq ($(shell which $(RC) 2>&1 | sed -ne "s@.*/$(RC)@$(RC)@p"),$(RC))
MKARCH := $(wildcard $(shell $(RC) --etcdir)/Makefile.arch)
endif
ifneq ($(MKARCH),)
include $(MKARCH)
else
ROOTSYS = ../..
include $(ROOTSYS)/etc/Makefile.arch
endif

#------------------------------------------------------------------------------

EXTRALIBS   =
MT_MANIFEST =

pktest_rh        := pk.h pixels.h LinkDef.h 
pktest_rS        := $(wildcard *.$(SrcSuf)) pktest_rDict.$(SrcSuf)
pktest_rO        := $(pktest_rS:.$(SrcSuf)=.$(ObjSuf))
pktest_r          = pktest_r$(ExeSuf)
pktest_rSO	:= 	 pktest_r.so
pktest_rLIB      = $(shell pwd)/$(pktest_rSO)

OBJS          = $(pktest_rO)

PROGRAMS      = $(pktest_r) $(pktest_rSO) $(pktest_rDict.$(SrcSuf))

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) $(ExeSuf) .$(DllSuf)

all:            $(PROGRAMS)

pktest_rDict.$(SrcSuf): $(pktest_rh)
		@rootcint -f $@ -c $^
		@echo "$@ Generating dictionary"

$(pktest_r):     $(pktest_rO)
	$(LD) $(LDFLAGS) $^ $(GLIBS) $(EXTRALIBS) $(OutPutOpt)$@
	$(MT_MANIFEST)
	@echo "$@ done"


$(pktest_rSO):     $(pktest_rO)
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
		@echo "$@ done"



#########################################



clean:
		@rm -f $(OBJS) core

distclean:      clean
		@rm -f $(PROGRAMS)  *Dict.* *.def *.exp \
		   *.ps *.so *.lib *.dll *.pdb *.ilk \
		   *.manifest *.d *.def

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
