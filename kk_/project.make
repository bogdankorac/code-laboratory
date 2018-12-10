
# TOOLS
#TOOL_GCC_PATH := C:/MinGW
#export CC := $(TOOL_GCC_PATH)/bin/g++.exe
export CC := g++

# PROJECT NAME
PROJECT := kk_mingw32

# SOURCE FILES
ifeq ($(PROJECT),kk_mingw32)
SOURCES := \
    src/main.c
else
SOURCES := \
    src/main.c
endif

# INCLUDE PATHS
INCLUDES := \
	include/ \
    $(TOOL_GCC_PATH)/include \
    $(TOOL_GCC_PATH)/lib/gcc/mingw32/4.6.2/include

# DEFINES
ifeq ($(PROJECT),kk_mingw32)
DEFINES := \
    MINGW32=1 \
    HTML=1
else
DEFINES := \
    HTML=1
endif
    
# LIBRARIES
ifeq ($(PROJECT),kk_mingw32)
LIBPATH := \
      libs \
	$(TOOL_GCC_PATH)/lib \
	$(TOOL_GCC_PATH)/lib/gcc/mingw32/4.6.2/include
else
LIBPATH := \
      libs
endif

# LIBS
ifeq ($(PROJECT),kk_mingw32)
LIBS := \
     CreditUserMinGW \
	ws2_32
else
LIBS := \
     CreditUser
endif


CFLAGS = -c
