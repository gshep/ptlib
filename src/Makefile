# Makefile
#
# Make file for ptlib library
#
# Portable Windows Library
#
# Copyright (c) 1993-1998 Equivalence Pty. Ltd.
#
# The contents of this file are subject to the Mozilla Public License
# Version 1.0 (the "License"); you may not use this file except in
# compliance with the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
# the License for the specific language governing rights and limitations
# under the License.
#
# The Original Code is Portable Windows Library.
#
# The Initial Developer of the Original Code is Equivalence Pty. Ltd.
#
# Portions are Copyright (C) 1993 Free Software Foundation, Inc.
# All Rights Reserved.
# 
# Contributor(s): Matthias Schneider____________________.

ifndef PTLIBDIR
  $(error PTLIBDIR must be defined)
endif

include $(PTLIBDIR)/make/ptbuildopts.mak
include $(PTLIBDIR)/make/unix.mak

OBJDIR	= $(PTLIB_OBJDIR)
TARGET  = $(PTLIB_LIBDIR)/$(PTLIB_FILE)
VERSION_FILE = $(PTLIBDIR)/version.h
REVISION_FILE = $(PTLIBDIR)/revision.h

##########################################

COMPONENT_SRC_DIR	= ptclib
COMMON_SRC_DIR		= ptlib/common
PLUGIN_DIR		= ../plugins
ifeq ($(target_os),mingw)
PLATFORM_SRC_DIR        = ptlib/msos
else
PLATFORM_SRC_DIR        = ptlib/unix
endif
VPATH_CXX		:= $(PLATFORM_SRC_DIR) $(COMMON_SRC_DIR) $(COMPONENT_SRC_DIR) 

ifeq ($(target_os),Darwin)
SED_EXTREGEX_FLAG		= -E
else
SED_EXTREGEX_FLAG		= -r
endif

# try and keep the order of modules in reverse order of usage so any
# global statics will be constructed/destructed in the correct order

ifeq ($(HAS_SASL2),1)
SOURCES	+= $(COMPONENT_SRC_DIR)/psasl.cxx 
endif

ifeq ($(HAS_LDAP),1)
SOURCES	+= \
	$(COMPONENT_SRC_DIR)/pldap.cxx \
        $(COMPONENT_SRC_DIR)/pils.cxx
endif

ifeq ($(HAS_SSL),1)
SOURCES += $(COMPONENT_SRC_DIR)/pssl.cxx 
endif

ifeq ($(HAS_SDL),1)
SOURCES += $(COMPONENT_SRC_DIR)/vsdl.cxx

$(OBJDIR)/vsdl.o: $(COMPONENT_SRC_DIR)/vsdl.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@

endif

ifeq ($(HAS_ODBC),1)
SOURCES += $(COMPONENT_SRC_DIR)/podbc.cxx
endif


ifeq ($(HAS_VIDEO),1)

SOURCES	 += $(COMMON_SRC_DIR)/vfakeio.cxx \
            $(COMMON_SRC_DIR)/videoio.cxx \
	    $(COMMON_SRC_DIR)/vconvert.cxx \
	    $(COMMON_SRC_DIR)/pvidchan.cxx \
	    $(COMMON_SRC_DIR)/tinyjpeg.c \
	    $(COMMON_SRC_DIR)/jidctflt.c

$(OBJDIR)/vfakeio.o: $(COMMON_SRC_DIR)/vfakeio.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@

ifeq ($(HAS_SHM_VIDEO),1)
SOURCES	+= $(PLATFORM_SRC_DIR)/shmvideo.cxx
endif

ifeq ($(HAS_VFW_CAPTURE),1)
SOURCES		+= $(PLATFORM_SRC_DIR)/vfw.cxx
endif

ifeq ($(HAS_DIRECTSHOW),1)
SOURCES	+= $(PLATFORM_SRC_DIR)/directshow.cxx
$(OBJDIR)/directshow.o: $(PLATFORM_SRC_DIR)/directshow.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@
endif

ifeq ($(target_os),Darwin)
OBJS	+= $(PLATFORM_SRC_DIR)/macvidcap.o
ENDLDLIBS += -framework QTKit -framework CoreVideo -framework Foundation -lobjc
$(OBJDIR)/macvidcap.o: $(PLATFORM_SRC_DIR)/macvidcap.mm
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@
endif # Darwin

endif # HAS_VIDEO

## SOUND DRIVERS
## Note this is mostly handled by the plugin system

ifeq ($(HAS_AUDIO),1)

SOURCES += $(COMMON_SRC_DIR)/sound.cxx 

ifeq ($(target_os),mingw)
SOURCES += $(PLATFORM_SRC_DIR)/sound_win32.cxx
$(OBJDIR)/sound_win32.o: $(PLATFORM_SRC_DIR)/sound_win32.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@
endif

# Although not an external plugin, the MacOSX
# CoreAudio sound driver still is in plugin form
# and requires the PPluginManager
ifeq ($(target_os),Darwin)
SOURCES	+= $(PLATFORM_SRC_DIR)/maccoreaudio.cxx
endif # Darwin

endif  # HAS_AUDIO

## Various modules

SOURCES	+= $(COMPONENT_SRC_DIR)/pxml.cxx 

ifeq ($(HAS_EXPAT),1)

ifeq ($(HAS_XMLRPC),1)
SOURCES	+= \
	$(COMPONENT_SRC_DIR)/pxmlrpc.cxx \
	$(COMPONENT_SRC_DIR)/pxmlrpcs.cxx 
endif

ifeq ($(HAS_SOAP),1)
SOURCES	+= $(COMPONENT_SRC_DIR)/psoap.cxx 
endif

ifeq ($(HAS_VXML),1)
SOURCES	+= $(COMPONENT_SRC_DIR)/vxml.cxx 
endif

ifeq ($(HAS_SASL2),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/xmpp.cxx \
	$(COMPONENT_SRC_DIR)/xmpp_c2s.cxx \
	$(COMPONENT_SRC_DIR)/xmpp_muc.cxx \
	$(COMPONENT_SRC_DIR)/xmpp_roster.cxx
endif
endif # HAS_EXPAT

ifeq ($(HAS_LUA),1)
SOURCES	+= $(COMPONENT_SRC_DIR)/lua.cxx
endif

ifeq ($(HAS_V8),1)
SOURCES	+= $(COMPONENT_SRC_DIR)/jscript.cxx
endif

ifeq ($(HAS_DNS_RESOLVER),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/pdns.cxx \
	$(COMPONENT_SRC_DIR)/enum.cxx 
endif

ifeq ($(HAS_TTS),1)
SOURCES += $(COMPONENT_SRC_DIR)/ptts.cxx 
endif

ifeq ($(HAS_ASN),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/asner.cxx \
	$(COMPONENT_SRC_DIR)/pasn.cxx 
endif

ifeq ($(HAS_SNMP),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/snmpclnt.cxx \
	$(COMPONENT_SRC_DIR)/snmpserv.cxx \
	$(COMPONENT_SRC_DIR)/psnmp.cxx \
	$(COMPONENT_SRC_DIR)/snmp.cxx \
	$(COMPONENT_SRC_DIR)/rfc1155.cxx 
endif

ifeq ($(HAS_FTP),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/ftpclnt.cxx \
	$(COMPONENT_SRC_DIR)/ftpsrvr.cxx \
	$(COMPONENT_SRC_DIR)/ftp.cxx 
endif

ifeq ($(HAS_TELNET),1)
SOURCES += $(COMPONENT_SRC_DIR)/telnet.cxx
endif

ifeq ($(HAS_CLI),1)
SOURCES += $(COMPONENT_SRC_DIR)/cli.cxx
endif

ifeq ($(HAS_NAT),1)
SOURCES += $(COMPONENT_SRC_DIR)/pnat.cxx
endif

ifeq ($(HAS_STUN),1)
SOURCES += $(COMPONENT_SRC_DIR)/pstun.cxx

$(OBJDIR)/pstun.o: $(COMPONENT_SRC_DIR)/pstun.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@

endif

ifeq ($(HAS_STUNSRVR),1)
SOURCES += $(COMPONENT_SRC_DIR)/pstunsrvr.cxx

$(OBJDIR)/pstunsrvr.o: $(COMPONENT_SRC_DIR)/pstunsrvr.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@
endif


ifeq ($(HAS_SOCKS),1)
SOURCES += $(COMPONENT_SRC_DIR)/socks.cxx 
endif

ifeq ($(HAS_PIPECHAN),1)
ifeq ($(target_os),mingw)
SOURCES += $(PLATFORM_SRC_DIR)/pipe.cxx 
SOURCES += $(COMMON_SRC_DIR)/pipechan.cxx 
else
SOURCES += $(PLATFORM_SRC_DIR)/pipechan.cxx 
endif
endif

ifeq ($(HAS_REMCONN),1)
SOURCES += $(PLATFORM_SRC_DIR)/remconn.cxx 
endif

ifeq ($(HAS_WAVFILE),1)
SOURCES += $(COMPONENT_SRC_DIR)/pwavfile.cxx \
           $(COMPONENT_SRC_DIR)/pwavfiledev.cxx

$(OBJDIR)/pwavfiledev.o: $(COMPONENT_SRC_DIR)/pwavfiledev.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@
endif

ifeq ($(HAS_DTMF),1)
SOURCES += $(COMPONENT_SRC_DIR)/dtmf.cxx 
endif

ifeq ($(HAS_VCARD),1)
SOURCES += $(COMPONENT_SRC_DIR)/vcard.cxx 
endif

ifeq ($(HAS_SERIAL),1)

ifeq ($(target_os),mingw)
SOURCES += $(PLATFORM_SRC_DIR)/winserial.cxx \
     	   $(COMMON_SRC_DIR)/serial.cxx

else
SOURCES += $(PLATFORM_SRC_DIR)/serchan.cxx
endif

SOURCES += $(COMPONENT_SRC_DIR)/modem.cxx 
endif

ifeq ($(HAS_POP3SMTP),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/inetmail.cxx 
endif

ifeq ($(HAS_URL),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/url.cxx 
endif

ifeq ($(HAS_HTTP),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/http.cxx \
	$(COMPONENT_SRC_DIR)/httpclnt.cxx \
	$(COMPONENT_SRC_DIR)/html.cxx \
	$(COMPONENT_SRC_DIR)/httpsrvr.cxx

ifeq ($(HAS_SSDP),1)
SOURCES += $(COMPONENT_SRC_DIR)/ssdp.cxx
endif
endif

ifeq ($(HAS_HTTPFORMS),1)
SOURCES += \
	$(COMPONENT_SRC_DIR)/httpform.cxx
endif #HAS_HTTPFORMS

ifeq ($(HAS_HTTPSVC),1)
SOURCES += \
	$(PLATFORM_SRC_DIR)/svcproc.cxx \
	$(COMPONENT_SRC_DIR)/httpsvc.cxx

ifeq ($(HAS_SSL),1)
SOURCES += $(COMPONENT_SRC_DIR)/shttpsvc.cxx
endif #HAS_SSL
endif #HAS_HTTPSVC

ifeq ($(HAS_CONFIG_FILE),1)
ifeq ($(target_os),mingw)
SOURCES += $(PLATFORM_SRC_DIR)/wincfg.cxx 
else
SOURCES += $(PLATFORM_SRC_DIR)/config.cxx 
endif
endif

ifeq ($(HAS_VIDFILE),1)
SOURCES += $(COMPONENT_SRC_DIR)/pvidfile.cxx \
	   $(COMPONENT_SRC_DIR)/pvfiledev.cxx 

$(OBJDIR)/pvfiledev.o: $(COMPONENT_SRC_DIR)/pvfiledev.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@

endif

ifeq ($(HAS_FFVDEV),1)
SOURCES += $(COMPONENT_SRC_DIR)/pffvdev.cxx

$(OBJDIR)/pffvdev.o: $(COMPONENT_SRC_DIR)/pffvdev.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) -DP_FORCE_STATIC_PLUGIN $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@

endif

ifeq ($(target_os),mingw)
SOURCES	+= \
        $(PLATFORM_SRC_DIR)/ptlib.cxx \
        $(PLATFORM_SRC_DIR)/icmp.cxx \
        $(PLATFORM_SRC_DIR)/winsock.cxx \
        $(PLATFORM_SRC_DIR)/win32.cxx \
        $(PLATFORM_SRC_DIR)/dllmain.cxx \
        $(COMMON_SRC_DIR)/pchannel.cxx \
        $(COMMON_SRC_DIR)/pethsock.cxx \
        $(COMMON_SRC_DIR)/pconfig.cxx
else
SOURCES	+= \
	$(PLATFORM_SRC_DIR)/uicmp.cxx \
	$(PLATFORM_SRC_DIR)/socket.cxx \
	$(PLATFORM_SRC_DIR)/udll.cxx \
	$(PLATFORM_SRC_DIR)/channel.cxx \
	$(PLATFORM_SRC_DIR)/osutil.cxx \
	$(PLATFORM_SRC_DIR)/tlib.cxx \
	$(PLATFORM_SRC_DIR)/switch.cxx
endif

GETDATE_SOURCE = $(COMMON_SRC_DIR)/getdate.c

SOURCES	+= \
	$(COMPONENT_SRC_DIR)/spooldir.cxx \
	$(COMPONENT_SRC_DIR)/threadpool.cxx \
	$(COMPONENT_SRC_DIR)/ipacl.cxx \
	$(COMPONENT_SRC_DIR)/qchannel.cxx \
	$(COMPONENT_SRC_DIR)/delaychan.cxx \
	$(COMPONENT_SRC_DIR)/memfile.cxx \
	$(COMPONENT_SRC_DIR)/cypher.cxx \
	$(COMPONENT_SRC_DIR)/random.cxx \
	$(COMPONENT_SRC_DIR)/inetprot.cxx \
	$(COMPONENT_SRC_DIR)/notifier_ext.cxx \
	$(COMPONENT_SRC_DIR)/guid.cxx \
	$(COMPONENT_SRC_DIR)/vartype.cxx \
	$(COMPONENT_SRC_DIR)/script.cxx \
	$(GETDATE_SOURCE) \
	$(PLATFORM_SRC_DIR)/assert.cxx \
	$(COMMON_SRC_DIR)/pluginmgr.cxx \
	$(COMMON_SRC_DIR)/sockets.cxx \
	$(COMMON_SRC_DIR)/psockbun.cxx \
	$(COMMON_SRC_DIR)/ptime.cxx \
	$(COMMON_SRC_DIR)/syslog.cxx \
	$(COMMON_SRC_DIR)/osutils.cxx \
	$(COMMON_SRC_DIR)/safecoll.cxx \
	$(COMMON_SRC_DIR)/collect.cxx \
	$(COMMON_SRC_DIR)/contain.cxx \
	$(COMMON_SRC_DIR)/object.cxx   # must be last module

ifneq ($(HAS_REGEX),1)
OBJS	= $(OBJDIR)/regcomp.o $(OBJDIR)/regexec.o $(OBJDIR)/regerror.o $(OBJDIR)/regfree.o
endif

CLEAN_FILES = $(GETDATE_SOURCE)


$(COMMON_SRC_DIR)/osutils.cxx: $(REVISION_FILE)

$(REVISION_FILE) : $(REVISION_FILE).in
ifeq ($(shell which svn 2> /dev/null),)
	sed -e "s/.WCREV./`sed -n -e 's/.*Revision: \([0-9]*\).*/\1/p' $<`/" $< > $@
else
ifeq ($(wildcard $(PTLIBDIR)/.svn),)
	sed -e "s/.WCREV./`sed -n -e 's/.*Revision: \([0-9]*\).*/\1/p' $<`/" $< > $@
else
	$(Q)sed "s/SVN_REVISION.*/SVN_REVISION `LC_ALL=C svn info | sed -n 's/Revision: //p'`/" $< > $@.tmp
	$(Q)if diff -q $@ $@.tmp >/dev/null 2>&1; then \
	  rm $@.tmp; \
	else \
	  mv -f $@.tmp $@; \
	  echo "Revision file updated to `sed $(SED_EXTREGEX_FLAG) -n 's/.*SVN_REVISION(.*)/\1/p' $@`" ; \
	fi

.PHONY: $(REVISION_FILE)
endif
endif

#######################################################

include ../make/common.mak

#######################################################

alllibs:
	$(MAKE) both
	$(MAKE) bothshared


$(OBJDIR)/regcomp.o: $(COMMON_SRC_DIR)/regex/regcomp.c
	$(Q_CC)$(CC) $(DEBUG_FLAG) $(OPTCCFLAGS) -c -DPOSIX_MISTAKE -I$(COMMON_SRC_DIR)/regex $(CFLAGS) -o $@ -c $<

$(OBJDIR)/regexec.o: $(COMMON_SRC_DIR)/regex/regexec.c
	$(Q_CC)$(CC) $(DEBUG_FLAG) $(OPTCCFLAGS) -c -DPOSIX_MISTAKE -I$(COMMON_SRC_DIR)/regex $(CFLAGS) -o $@ -c $<

$(OBJDIR)/regerror.o: $(COMMON_SRC_DIR)/regex/regerror.c
	$(Q_CC)$(CC) $(DEBUG_FLAG) $(OPTCCFLAGS) -c -DPOSIX_MISTAKE -I$(COMMON_SRC_DIR)/regex $(CFLAGS) -o $@ -c $<

$(OBJDIR)/regfree.o: $(COMMON_SRC_DIR)/regex/regfree.c
	$(Q_CC)$(CC) $(DEBUG_FLAG) $(OPTCCFLAGS) -c -DPOSIX_MISTAKE -I$(COMMON_SRC_DIR)/regex $(CFLAGS) -o $@ -c $<


ifdef REQUIRES_SEPARATE_SWITCH 
$(OBJDIR)/switch.o: $(PLATFORM_SRC_DIR)/switch.cxx
	@if [ ! -d $(OBJDIR) ] ; then mkdir -p $(OBJDIR) ; fi
	$(Q_CC)$(CXX) $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@
endif


#######################################################

$(OBJDIR)/getdate.o: $(GETDATE_SOURCE)
	$(Q_CC)$(CC) -Wno-write-strings $(PTLIB_CFLAGS) $(CFLAGS) -c $< -o $@

$(DEPDIR)/getdate.dep: $(GETDATE_SOURCE)
	$(Q_CC)$(CC) $(PTLIB_CFLAGS) -M $< >> $@

GETDATE_TAB_C := $(COMMON_SRC_DIR)/getdate.tab.c

$(GETDATE_SOURCE): $(GETDATE_TAB_C)
	cp $< $@

ifdef BISON
$(GETDATE_TAB_C): $(COMMON_SRC_DIR)/getdate.y
	$(BISON) -o $@ $<
endif


#######################################################

ifeq ($(P_SHAREDLIB),1)
  STATIC_LIB_FILE = $(PTLIB_LIBDIR)/$(subst .$(LIB_SUFFIX),_s.$(STATICLIBEXT),$(PTLIB_FILE))

  $(PTLIB_LIBDIR)/$(PTLIB_FILE): $(PTLIB_LIBDIR)/$(PTLIB_SONAME)
	@cd $(dir $@) ; rm -f $@ ; ln -sf $(notdir $<) $(notdir $@)

  $(PTLIB_LIBDIR)/$(PTLIB_SONAME): $(STATIC_LIB_FILE)
	@if [ ! -d $(dir $@) ] ; then mkdir $(dir $@) ; fi
	$(Q_LD)$(LD) $(LDSOFLAGS) -o $@ $(EXTLIBS) $(OBJS) $(SYSLIBS) $(ENDLDLIBS)

  CLEAN_FILES += $(PTLIB_LIBDIR)/$(PTLIB_SONAME) $(PTLIB_LIBDIR)/$(PTLIB_FILE)

else # P_SHAREDLIB

  STATIC_LIB_FILE = $(PTLIB_LIBDIR)/$(PTLIB_FILE)

endif # P_SHAREDLIB

$(STATIC_LIB_FILE): $(OBJS)
	@if [ ! -d $(dir $@) ] ; then mkdir $(dir $@) ; fi
	$(Q_AR)$(ARCHIVE) $(STATIC_LIB_FILE) $(OBJS)
ifeq ($(P_USE_RANLIB),1)
	$(RANLIB) $(STATIC_LIB_FILE)
endif

CLEAN_FILES += $(STATIC_LIB_FILE)


#######################################################

# End of Makefile
