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
#ifndef RDKC_CONFIG_SECTIONS_H
#define RDKC_CONFIG_SECTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dev_config.h"

#define  MAX_LENGTH			10

#define  POLLING_URL_LEN                1024
#define  POLLING_AUTH_LEN               128
#define  POLLING_USERNAME_LEN           128
#define  POLLING_PASS_LEN               128

#define  RDKC_SUCCESS 			0
#define  RDKC_FAILURE			-1

/*typedef enum RdkCStatus {
	RDKC_SUCCESS = 0,
	RDKC_FAILURE = -1
} RdkCStatus;*/

typedef struct RdkCCloudRecorderConf {
        char polling_url[POLLING_URL_LEN+1];
        char polling_auth[POLLING_AUTH_LEN+1];
        int polling_interval;
        int enable;
        int video_duration;
        int video_format;
        char video_address[POLLING_URL_LEN+1];
        char video_auth[POLLING_AUTH_LEN+1];
        char video_username[POLLING_USERNAME_LEN+1];
        char video_pass[POLLING_PASS_LEN+1];
        int timeout;
} RdkCCloudRecorderConf;

int RdkCReadCloudRecorderSection(RdkCCloudRecorderConf *crf);
int RdkCWriteCloudRecorderSection(RdkCCloudRecorderConf *crf);

#endif
