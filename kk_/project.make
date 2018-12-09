
# TOOLS
#TOOL_GCC_PATH := C:/MinGW
#export CC := $(TOOL_GCC_PATH)/bin/g++.exe
export CC := g++

# PROJECT NAME
PROJECT := kk

# SOURCE FILES
SOURCES := \
    src/main.c \

# INCLUDE PATHS
INCLUDES := \
	include/ \
    $(TOOL_GCC_PATH)/include \
    $(TOOL_GCC_PATH)/lib/gcc/mingw32/4.6.2/include

# DEFINES
DEFINES := \
    HTML=1 \
    
# LIBRARIES
LIBPATH := \
	libs
	
LIBS := \
        CreditUser


CFLAGS = -c
