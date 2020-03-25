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
#include "rdkc_config_sections.h"

/**
 * @brief Converts the interger value to String.
 * @param  num is a pointer to the input integer.
 * @param  str is a pointer to the output string.
 * @return char pointer.
 */
static char* itoa(int num, char *str)
{
    if(NULL == str) {
        return NULL;
    }
    sprintf(str, "%d", num);
    return str;
}

/**
 * @brief Reads the CloudRecorderInfo from ConfigMgr.
 * @param  crc is pointer to RdkCCloudRecorderConf.
 * @return RDKC_SUCCESS on success, RDKC_FAILURE otherwise.
 */
int RdkCReadCloudRecorderSection(RdkCCloudRecorderConf *crc)
{
    char* configParam = NULL;

    /* Initialize ConfigMgr */
    /*if(config_init() != RDKC_SUCCESS) {
	printf("Failed to initialize configMgr\n");
	return RDKC_FAILURE;
    }*/

    /* Get POLLING_URL */
    configParam =  (char*) rdkc_envGet(POLLING_URL);
    if(configParam) {
	strcpy(crc -> polling_url, configParam);
    }

    /* Get POLLING_AUTH */
    configParam =  (char*) rdkc_envGet(POLLING_AUTH);
    if(configParam) {
        strcpy(crc -> polling_auth, configParam);
    }

    /* Get POLLING_INTERVAL */
    configParam =  (char*) rdkc_envGet(POLLING_INTERVAL);
    if(configParam) {
        crc -> polling_interval = atoi(configParam);
    }

    /* Get POLLING_ENABLE */
    configParam =  (char*) rdkc_envGet(POLLING_ENABLE);
    if(configParam) {
        crc -> enable =  atoi(configParam);
    }

    /* Get VIDEO_DURATION */
    configParam =  (char*) rdkc_envGet(VIDEO_DURATION);
    if(configParam) {
        crc -> video_duration =  atoi(configParam);
    }

    /* Get VIDEO_FORMAT */
    configParam =  (char*) rdkc_envGet(VIDEO_FORMAT);
    if(configParam) {
        crc -> video_format = atoi(configParam);
    }

    /* Get VIDEO_ADDRESS */
    configParam =  (char*) rdkc_envGet(VIDEO_ADDRESS);
    if(configParam) {
        strcpy(crc -> video_address, configParam);
    }

    /* Get VIDEO_AUTH */
    configParam =  (char*) rdkc_envGet(VIDEO_AUTH);
    if(configParam) {
        strcpy(crc -> video_auth, configParam);
    }

    /* Get VIDEO_USERNAME */
    configParam =  (char*) rdkc_envGet(VIDEO_USERNAME);
    if(configParam) {
        strcpy(crc -> video_username, configParam);
    }

    /* Get VIDEO_PASSWORD */
    configParam =  (char*) rdkc_envGet(VIDEO_PASSWORD);
    if(configParam) {
        strcpy(crc -> video_pass, configParam);
    }

    /* Get POLLING_TIMEOUT */
    configParam =  (char*) rdkc_envGet(POLLING_TIMEOUT);
    if(configParam) {
        crc -> timeout =  atoi(configParam);
    }

    /* Release ConfigMgr */
    // config_release();
    return RDKC_SUCCESS;
}    

/**
 * @brief Writes the CloudRecorderInfo to ConfigMgr.
 * @param  crc is pointer to RdkCCloudRecorderConf.
 * @return RDKC_SUCCESS on success, RDKC_FAILURE otherwise.
 */
int RdkCWriteCloudRecorderSection(RdkCCloudRecorderConf *crc)
{
    char str[MAX_LENGTH];

    /* Initilaize ConfigMgr */
    /*if(config_init() != RDKC_SUCCESS) {
        printf("Failed to initialize configMgr\n");
        return RDKC_FAILURE;
    }
    */
    /* Set POLLING_URL */
    if((crc -> polling_url) && (rdkc_envSet(POLLING_URL, crc -> polling_url) != RDKC_SUCCESS)) {
	printf("Error setting value for polling_url\n");
	return RDKC_FAILURE;
    }

    /* Set POLLING_AUTH */
    if((crc -> polling_auth) && (rdkc_envSet(POLLING_AUTH, crc -> polling_auth) != RDKC_SUCCESS)) {
        printf("Error setting value for polling_auth\n");
        return RDKC_FAILURE;
    }

    /* Set POLLING_INTERVAL */
    memset(&str, 0, sizeof(str));
    if((-1 != crc -> polling_interval) && (rdkc_envSet(POLLING_INTERVAL, itoa(crc -> polling_interval, str)) != RDKC_SUCCESS)) {
        printf("Error setting value for polling_interval\n");
        return RDKC_FAILURE;
    }

    /* Set POLLING_ENABLE */ 
    memset(&str, 0, sizeof(str));
    if((-1 != crc -> enable) && (rdkc_envSet(POLLING_ENABLE, itoa(crc -> enable, str)) != RDKC_SUCCESS)) {
        printf("Error setting value for enable\n");
        return RDKC_FAILURE;
    }

    /*Set VIDEO_DURATION */
    memset(&str, 0, sizeof(str));
    if((-1 != crc -> video_duration) && (rdkc_envSet(VIDEO_DURATION, itoa(crc -> video_duration, str)) != RDKC_SUCCESS)) {
        printf("Error setting value for video_duration\n");
        return RDKC_FAILURE;
    }

    /* Set VIDEO_FORMAT */
    memset(&str, 0, sizeof(str));
    if((-1 != crc -> video_format) && (rdkc_envSet(VIDEO_FORMAT, itoa(crc -> video_format, str)) != RDKC_SUCCESS)) {
        printf("Error setting value for video_format\n");
        return RDKC_FAILURE;
    }

    /* Set VIDEO_ADDRESS */
    if((crc -> video_address) && (rdkc_envSet(VIDEO_ADDRESS, crc -> video_address) != RDKC_SUCCESS)) {
        printf("Error setting value for video_address\n");
        return RDKC_FAILURE;
    }

    /* Set VIDEO_AUTH */
    if((crc -> video_auth) && (rdkc_envSet(VIDEO_AUTH, crc -> video_auth) != RDKC_SUCCESS)) {
        printf("Error setting value for video_auth\n");
        return RDKC_FAILURE;
    }

    /* Set VIDEO_USERNAME */
    if((crc -> video_username) && (rdkc_envSet(VIDEO_USERNAME, crc -> video_username) != RDKC_SUCCESS)) {
	printf("Error setting value for video_username\n");
	return RDKC_FAILURE;
    }

    /* Set VIDEO_PASSWORD */
    if((crc -> video_pass) && (rdkc_envSet(VIDEO_PASSWORD, crc -> video_pass) != RDKC_SUCCESS)) {
        printf("Error setting value for video_password\n");
        return RDKC_FAILURE;
    }
    printf("crc -> timeout:%d\n", crc -> timeout);
    /* Set POLLING_TIMEOUT */
    if((-1 != crc -> timeout) && (rdkc_envSet(POLLING_TIMEOUT, itoa(crc -> timeout, str)) != RDKC_SUCCESS)) {
        printf("Error setting value for timeout\n");
        return RDKC_FAILURE;
    }

    /* Release ConfigMgr */
    // config_release();

    return RDKC_SUCCESS;
}
