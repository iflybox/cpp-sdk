#common makefile header

SLNDIR = ../../
include $(SLNDIR)prj.mak/configure.mk

#this module is static library.
MODULE_EXEC		= dstester
MODULE_TARGET	= $(MODULE_EXEC)
LOCAL_INCLUDE	= -I ../../iflybox-include/iflybox-include-cpp -I ../../iflybox-share/iflybox-share-cpp/3rd-part/ACE-5.7.1
LOCAL_FLAGS		= $(LOCAL_INCLUDE) -fpermissive
#LIBS  	 		= -L $(SLNDIR)iflybox-share/iflybox-share-cpp/lib64  -lACE -lboost_program_options -lboost_thread
LIBS			:= ../../iflybox-share/iflybox-share-cpp/lib64/libACE.so.5.7.1 	\
				 ../../iflybox-share/iflybox-share-cpp/lib64/libboost_program_options.a	\
				 ../../iflybox-share/iflybox-share-cpp/lib64/libboost_thread.a

MODULE_OBJ :=	$(MODULE_OUTPUT)/iflybox_svc.o	\
				$(MODULE_OUTPUT)/dstester.o	

#common makefile foot
include $(SLNDIR)prj.mak/rules.mk

#automatically trace files.
.KEEP_STATE_FILE: $(MODULE_OUTPUT)
