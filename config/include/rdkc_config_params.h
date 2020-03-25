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
#ifndef RDKC_CONFIG_PARAMS_H
#define RDKC_CONFIG_PARAMS_H

#include "stdlib.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#define DELIM_FILENAME '.'
#define DELIM_PARAM    '='

/* DEV_CONFIG_PATH will be used if DEFAULT_DEV_CONFIG_PATH is not available */

#define DEFAULT_DEV_CONFIG_PATH "/opt/dev_config.txt" 	/* default path for devloper config file */
#define DEV_CONFIG_PATH "/etc/dev_config.txt"    	/* path for developer config file */

#define USER_CONFIG_PARAMETER "user_config_path"    	/* parameter name for getting the location of user config files */

#define RDKC_FAILURE -1
#define RDKC_SUCCESS  0

#define MAX_BUFFER_SIZE 1024 // The maximum length of a line in config files should not exceed 1024

/* store the name-value pair for parameters 
 * in developer config file */
typedef struct EnvVarNode
{
    char * name;
    char * value;
    struct EnvVarNode *next;
} EnvVarNode;

#endif
