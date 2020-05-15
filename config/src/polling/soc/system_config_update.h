/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2019 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#ifndef  _SYSTEM_CONFIG_UPDATE_H_
#define  _SYSTEM_CONFIG_UPDATE_H_

#include <stdbool.h>

#include "polling_config.h"
#include "conf_sec.h"
#include "PRO_file.h"
#include "IMG_conf.h"
#include "main.h"

#include "sysUtils.h"
#include "list-lib.h"
#include "cgi_image.h"

#define CONF_FILE        "/mnt/ramdisk/env/system.conf"
#define MAXSIZE          256

cvr_provision_info_t *cvrConfig;

int systemConfigWrite(char* tag, void* config, bool abrBitrateChanged);
int abrSystemConfigWrite(AbrTargetBitrate *abr_bitrate);

#endif

