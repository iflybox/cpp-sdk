#common makefile header

SLNDIR := $(CURDIR)/../../../
#IceHome=../share/Ice-3.4.2/cpp
include $(SLNDIR)prj.mak/configure.mk

MODULE_SO       = iflybox.so
MODULE_TARGET   = $(MODULE_SO)

LOCAL_INCLUDE   = -I ../../../iflybox-include/iflybox-include-cpp 
LOCAL_FLAGS     = $(LOCAL_INCLUDE) $(PIC_FLAG) -fpermissive -DDONT_CHECK_DONGLE -DPERSONAL -DHOTWORD -DSYNCH_LOG

#LIBS := -L $(SLNDIR)iflybox-share/iflybox-share-cpp/lib -L $(SLNDIR)iflybox-share/iflybox-share-cpp/lib64 -lcurl
LIBS			:= ../../../iflybox-share/iflybox-share-cpp/lib64/libcurl.so

MODULE_OBJ   = 					\
	$(MODULE_OUTPUT)/json_reader.o		\
	$(MODULE_OUTPUT)/json_value.o		\
	$(MODULE_OUTPUT)/json_writer.o		\
	$(MODULE_OUTPUT)/tinystr.o		\
	$(MODULE_OUTPUT)/tinyxmlerror.o		\
	$(MODULE_OUTPUT)/tinyxmlparser.o		\
	$(MODULE_OUTPUT)/tinyxml.o		\
	$(MODULE_OUTPUT)/httpHelper.o		\
	$(MODULE_OUTPUT)/metadata.o	\
	$(MODULE_OUTPUT)/swiftClient.o			\
	$(MODULE_OUTPUT)/iflybox.o		


	
	

#common makefile foot
include $(SLNDIR)/prj.mak/rules.mk

#automatically trace files.
.KEEP_STATE_FILE: $(MODULE_OUTPUT)
