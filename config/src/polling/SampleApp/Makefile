##########################################################################
# If not stated otherwise in this file or this component's LICENSE
# file the following copyright and licenses apply:
#
# Copyright 2019 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################

# Add dependent libraries
USE_OPENCV = yes
USE_RFCCONFIG = yes

include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak

#Includes
INCPATH += $(RDK_SOURCE_PATH)/include
INCPATH += $(RDK_PROJECT_ROOT_PATH)/configMgr/include
INCPATH += $(RDK_PROJECT_ROOT_PATH)/configMgr/config/src/polling

CFLAGS += $(addprefix -I, $(INCPATH))
CFLAGS += -fPIC -Wall


LDFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/configMgr/config/src -lconfigmanager
LDFLAGS += $(LIBFLAGS)

LDFLAGS += -lpthread
LDFLAGS += -Wl,-unresolved-symbols=ignore-in-shared-libs

RM = rm -f
TARGET = polling_config

SRCS = main.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ -lrt
%.o:%.c
	$(CC) -c $< $(CFLAGS)  -o $@

clean:
	-${RM} ${TARGET} ${OBJS}

install:
	$(INSTALL)  -D  $(TARGET) $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/local/bin/$(TARGET)

uninstall:
	$(RM) -f $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/local/bin/$(TARGET)

.PHONY: all clean


