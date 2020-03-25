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

include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak

CONFIG_MNGR_ROOT_DIR  := ./../..
BUILD_ROOT_DIR  := $(CONFIG_MNGR_ROOT_DIR)/..


# ConfigManager Includes
INCPATH += $(CONFIG_MNGR_ROOT_DIR)/include
INCPATH += $(CONFIG_MNGR_ROOT_DIR)/config/include
INCPATH += $(CONFIG_MNGR_ROOT_DIR)/config/configMgrSection

CFLAGS += $(addprefix -I, $(INCPATH))
CFLAGS += -fPIC -Wall
LDFLAGS += -shared -lpthread

ifeq ($(XCAM_MODEL), SCHC2)
CFLAGS  += -DXCAM2
endif

RM = rm -f
TARGET_LIB = libconfigmanager.so
SRCS = rdkc_mutex_handle.c $(CONFIG_MNGR_ROOT_DIR)/config/configMgrSection/rdkc_config_sections.c rdkc_config_params.c polling/polling_config.c

OBJS = $(SRCS:.c=.o)
ifeq ($(SOC_JSON_CONFIG_ENABLE),true)
CFLAGS += -DSOC_JSON_CONFIG_ENABLE=true
LDFLAGS += -L $(CONFIG_MNGR_ROOT_DIR)/config/src/polling/soc -lsystemconfig
endif
LDFLAGS += $(LIBFLAGS)

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJS) $(OBJS_SOC)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@


clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)

.PHONY: all clean
