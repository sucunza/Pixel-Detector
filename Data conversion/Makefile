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

pktest_0h        := pk.h pixel.h LinkDef.h 
pktest_0S        := $(wildcard *.$(SrcSuf)) pktest_0Dict.$(SrcSuf)
pktest_0O        := $(pktest_0S:.$(SrcSuf)=.$(ObjSuf))
pktest_0          = pktest_0$(ExeSuf)
pktest_0SO	:= 	 pktest_0.so
pktest_0LIB      = $(shell pwd)/$(pktest_0SO)

OBJS          = $(pktest_0O)

PROGRAMS      = $(pktest_0) $(pktest_0SO) $(pktest_0Dict.$(SrcSuf))

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) $(ExeSuf) .$(DllSuf)

all:            $(PROGRAMS)

pktest_0Dict.$(SrcSuf): $(pktest_0h)
	@rootcint -f $@ -c $^
	@echo "$@ Generating dictionary"

$(pktest_0):     $(pktest_0O)
	$(LD) $(LDFLAGS) $^ $(GLIBS) $(EXTRALIBS) $(OutPutOpt)$@
	$(MT_MANIFEST)
	@echo "$@ done"


$(pktest_0SO):     $(pktest_0O)
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
	@echo "$@ done"	



clean:
	@rm -f $(OBJS) core

distclean:      clean
	@rm -f $(PROGRAMS)  *Dict.* *.def *.exp \
		*.root *.ps *.so *.lib *.dll *.pdb *.ilk \
		*.manifest *.d *.def *.pcm

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
