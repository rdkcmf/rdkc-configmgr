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
USE_RFCCONFIG = yes
USE_SYSUTILS = yes
USE_LIBSYSWRAPPER =yes

include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak

CONFIG_MNGR_ROOT_DIR  := ./../../../..
BUILD_ROOT_DIR  := $(CONFIG_MNGR_ROOT_DIR)/..

# ConfigManager Includes
INCPATH += $(CONFIG_MNGR_ROOT_DIR)/include
INCPATH += $(CONFIG_MNGR_ROOT_DIR)/config/include
INCPATH += $(CONFIG_MNGR_ROOT_DIR)/config/configMgrSection
INCPATH += $(CONFIG_MNGR_ROOT_DIR)/config/src/polling

CFLAGS += $(addprefix -I, $(INCPATH))
CFLAGS += -fPIC -Wall

LDFLAGS += -shared -lpthread
LDFLAGS += $(LIBFLAGS)

TARGET_LIB = libsystemconfig.so
SRCS = system_config_update.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJS) $(OBJS_SOC)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

clean:
	-${RM} ${OBJS} $(SRCS:.c=.d)


.PHONY: all clean
