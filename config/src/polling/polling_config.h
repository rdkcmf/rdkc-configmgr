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

#ifndef _POLLING_CONFIG_H
#define _POLLING_CONFIG_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <config_api.h>
#include <pthread.h>
#include <stdbool.h>

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
#define POLLING_CONFIG_FILE "/opt/usr_config/polling.conf"
#define CLOUDRECORDER_CONFIG_FILE "/opt/usr_config/cvr.conf"
#define EVENTS_CONFIG_FILE "/opt/usr_config/event.conf"
#define DETECTION_CONFIG_FILE "/opt/usr_config/detection.conf"
#define EMS_CONFIG_FILE "/opt/usr_config/live_video.conf"
#define LIVECACHE_CONFIG_FILE "/opt/usr_config/livecache.conf"
#define TN_CONFIG_FILE "/opt/usr_config/tn_upload.conf"
#define CVRSTATS_CONFIG_FILE "/opt/usr_config/cvrstats.conf"
#define KVS_CONFIG_FILE "/opt/usr_config/kvs.conf"
#define LUX_CONFIG_FILE "/opt/usr_config/lux.conf"
#define SECURITY_CONFIG_FILE "/opt/usr_config/sys.conf"
#define SENSOR_CONFIG_FILE "/opt/usr_config/sensor.conf"
#define DING_CONFIG_FILE "/opt/usr_config/ding.conf"
#define CHIME_CONFIG_FILE "/opt/usr_config/chime.conf"
#define DOI_CONFIG_FILE "/opt/usr_config/doi.conf"
#define DOI_BITMAP_FNAME "/opt/usr_config/doi_bitmap"
#define DOI_BITMAPWRITE_FNAME "/opt/usr_config/doi_bitmap.new"
#define MAX_ERROR_COUNT				5
#define PARAM_LEN				128
#define URL_LEN					1024
#define MAC_STRING_LEN				12
#define WAIT_RANDOM_TIME_MAX			60
#define ENVETN_INTERVAL_DEF			30
#define DATA_LEN				4096
#define ATTRIBUTE_ZERO_COUNT			0
#define ATTRIBUTE_VALUE_MAX_LEN			512
#define ERR_SLEEP_TIME				1
#define DEFAULT_MIN_INTERVAL			1
#define DEFAULT_MAX_INTERVAL 			20
#define DEFAULT_RETRY_FACTOR 			2
#define CONFIG_STRING_MAX			(256)
#define AUTH_TOKEN_MAX				(1024)
#define CONFIG_ATTR_MAX				(32)
#define RDKC_FAILURE				-1
#define RDKC_SUCCESS				0
#define RDKC_UPDATED				1
#define RDKC_ERR_CONF_READ_INPROGRESS		2
#define RDKC_ERR_CONF_WRITE_INPROGRESS		3
#define RDKC_ERR_ALREADY_INITED			4
#define RDKC_ERR_DATA_ALREADY_SET		5
#define TRUE					1
#define FALSE					0


#define XH_TAG_NAME_CONFIG                 "config"
#define XH_TAG_NAME_SYSTEM                 "system"
#define XH_TAG_NAME_LUX                    "lux"
#define XH_TAG_NAME_KVS                    "kvs"
#define XH_TAG_NAME_THRESHOLD              "threshold"
#define XH_TAG_NAME_CVR                    "cvr"
#define XH_TAG_NAME_EVENTS                 "events"
#define XH_TAG_NAME_MOTION                 "motion"
#define XH_TAG_NAME_PROVISION_MOTION       "provisionmotion"
#define XH_TAG_NAME_HUMAN                  "human"
#define XH_TAG_NAME_TAMPER                 "tamper"
#define XH_TAG_NAME_DETECTION              "detection"
#define XH_TAG_NAME_SEGMENTS               "segments"
#define XH_TAG_NAME_LBR                    "LBR"
#define XH_ATTR_ROLE_ALIAS		   "roleAlias"
#define XH_ATTR_CRED_PROVIDER		   "credentialProvider"
#define XH_ATTR_STREAM_NAME		   "streamName"
#define XH_ATTR_FORCE_CSR_REQ		   "forceCSRReq"
#define XH_TAG_NAME_CSR_CERT		   "csrCert"
#define XH_TAG_NAME_CA_CERT		   "caCert"
#define XH_ATTR_ID			   "ID"
#define XH_ATTR_FORCE_USE_CA		   "forceUseCA"
#define XH_ATTR_INTERVAL                   "interval"
#define XH_FAILURE_PRECENT                   "failure-percent"
#define XH_ATTR_URL                        "url"
#define XH_ATTR_ROOMID                     "roomid"
#define XH_ATTR_ERSPORT                    "ersport"
#define XH_ATTR_ENABLED                    "enabled"
#define XH_ATTR_AUTH                       "auth"
#define XH_ATTR_DURATION                   "duration"
#define XH_ATTR_FORMAT                     "format"
#define XH_ATTR_QUIET_INTERVAL             "quietInterval"
#define XH_ATTR_MIN                        "min"
#define XH_ATTR_LOW                        "low"
#define XH_ATTR_MED                        "med"
#define XH_ATTR_MAX                        "max"
#define XH_TAG_NAME_H264                   "H264"
#define XH_ATTR_RESOLUTION                 "resolution"
#define XH_ATTR_FPS                        "fps"
#define XH_ATTR_GOP                        "gop"
#define XH_ATTR_HDR                        "hdr"
#define XH_ATTR_ENV                        "env"
#define XH_ATTR_SENSITIVITY                "sensitivity"
#define XH_TAG_NAME_DISABLED               "disabled"
#define XH_TAG_NAME_MODEL                  "model"
#define XH_TAG_NAME_SERIAL                 "serial"
#define XH_TAG_NAME_MAC                    "mac"
#define XH_TAG_NAME_FIRMWARE               "firmware"
#define XH_TAG_NAME_WIRED                  "wired"
#define XH_TAG_NAME_SSID                   "ssid"
#define XH_TAG_NAME_RADIO                  "radio"
#define XH_TAG_NAME_STREAM                 "stream"
#define XH_TAG_NAME_CPU                    "cpu"
#define XH_TAG_NAME_MEMORY                 "memory"
#define XH_TAG_NAME_SNR                    "snr"
#define XH_TAG_NAME_RSSI                   "rssi"
#define XH_TAG_NAME_INDOOR                 "indoor"
#define XH_TAG_NAME_OUTDOOR                "outdoor"
#define XH_ATTR_DAY2NIGHT                  "day2night"
#define XH_ATTR_NIGHT2DAY                  "night2day"
#define XH_ATTR_USER_NAME                  "username"
#define XH_ATTR_PASSWORD                   "password"
#define XH_ATTR_VERSION                    "version"
#define XH_ATTR_SOURCE                     "source"
#define XH_ATTR_FORCE                      "force"
#define XH_ATTR_TIMEOUT                    "timeout"
#define XH_ATTR_HEIGHT                     "height"
#define XH_ATTR_WIDTH                      "width"
#define XH_ATTR_QUALITY                    "quality"
#define XH_ATTR_CHIME_TYPE                 "chime_type"
#define XH_ATTR_CHIME_DURATION             "duration"
#define XH_ATTR_DOI_CONFIG_UPDATED	   "updated"
#define XH_ATTR_DOI_TIMESTAMP              "timestamp"
#define XH_ATTR_THRESHOLD	   	   "threshold"
#define _SUPPORT_AAC_
#ifdef  _SUPPORT_AAC_
#define XH_ATTR_AUDIO                      "audio"
#endif
#define XH_ATTR_ROICOORD                   "roi"
#define XH_ATTR_TZ                         "timeZone"

typedef struct {
	char enable[CONFIG_ATTR_MAX];
	char roomId[CONFIG_ATTR_MAX];
	char ers_url[CONFIG_STRING_MAX];
    char ers_port[CONFIG_STRING_MAX];
	char auth_token[AUTH_TOKEN_MAX];
        char resolution[AUTH_TOKEN_MAX];
} ems_provision_info_t;

typedef struct {
    char enable[CONFIG_ATTR_MAX];
    char url[CONFIG_STRING_MAX];
    char auth_token[AUTH_TOKEN_MAX];
} livecache_provision_info_t;

typedef struct {
    char enable[CONFIG_ATTR_MAX];
    char height[CONFIG_ATTR_MAX];
    char width[CONFIG_ATTR_MAX];
    char quality[CONFIG_ATTR_MAX];
    char url[CONFIG_STRING_MAX];
    char auth_token[AUTH_TOKEN_MAX];
} tn_provision_info_t;

typedef struct {
    char interval[CONFIG_ATTR_MAX];
    char enabled[CONFIG_ATTR_MAX];
    char failurePercent[CONFIG_ATTR_MAX];
    char url[CONFIG_STRING_MAX];
    char auth_token[AUTH_TOKEN_MAX];
} cvrStats_provision_info_t;

typedef struct {
    char enable[CONFIG_ATTR_MAX];
    char role_alias[AUTH_TOKEN_MAX];
    char credentialProvider[AUTH_TOKEN_MAX];
    char stream_name[CONFIG_ATTR_MAX];
    char force_CSR_req[CONFIG_ATTR_MAX];
    char cert_CSR_enable[CONFIG_ATTR_MAX];
    char cert_CSR_url[AUTH_TOKEN_MAX];
    char cert_CSR_auth[AUTH_TOKEN_MAX];
    char cert_CA_enable[CONFIG_ATTR_MAX];
    char cert_CA_url[AUTH_TOKEN_MAX];
    char cert_CA_auth[AUTH_TOKEN_MAX];
    char cert_CA_id[AUTH_TOKEN_MAX];
    char cert_CA_force[CONFIG_ATTR_MAX];
    char partner_id[CONFIG_ATTR_MAX];
} kvs_provision_info_t;

typedef struct {
    char min[CONFIG_ATTR_MAX];
    char max[CONFIG_ATTR_MAX];
    char med[CONFIG_ATTR_MAX];
    char low[CONFIG_ATTR_MAX];
} cvr_lbr_info_t;

typedef struct {
    char resolution[AUTH_TOKEN_MAX];
    char fps[CONFIG_ATTR_MAX];
    char gop[CONFIG_ATTR_MAX];
} cvr_stream_info_t;

typedef struct {
	char format[CONFIG_ATTR_MAX];
	char duration[CONFIG_ATTR_MAX];
	cvr_lbr_info_t cvr_lbr_info;
	cvr_stream_info_t cvr_stream_info;
} cvr_segment_info_t;

typedef struct {
    char enable[CONFIG_ATTR_MAX];
    char url[CONFIG_STRING_MAX];
    char auth_token[AUTH_TOKEN_MAX];
    char audio_enable[CONFIG_ATTR_MAX];
    cvr_segment_info_t cvr_segment_info;
} cvr_provision_info_t;

typedef struct {
    char motion_enable[CONFIG_ATTR_MAX];
    char sensitivity[CONFIG_ATTR_MAX];
    char env[CONFIG_ATTR_MAX];
} detection_provision_info_t;

typedef struct {
    char night2day[CONFIG_ATTR_MAX];
    char day2night[CONFIG_ATTR_MAX];
    char hdr[CONFIG_ATTR_MAX];
    char sensitivity[CONFIG_ATTR_MAX];
    char env[CONFIG_ATTR_MAX];
} sensor_config_info_t;
//ding": {"enabled":true,"url": "" , "auth": "", "quiteTime":"" }
typedef struct {
    char enable[CONFIG_ATTR_MAX];
    char url[CONFIG_STRING_MAX];
    char auth_token[AUTH_TOKEN_MAX];
    char quite_interval[CONFIG_ATTR_MAX];
} ding_config_info_t;


//"chime": {"enabled":true/false,"chime_type": "analog/digital" }
typedef struct {
    char enable[CONFIG_ATTR_MAX];
    char chime_type[CONFIG_STRING_MAX];
    char duration[CONFIG_ATTR_MAX];
} chime_config_info_t;

typedef struct {
    char enable[CONFIG_ATTR_MAX];
    char updated[CONFIG_STRING_MAX];
    char url[CONFIG_STRING_MAX];
    char auth[AUTH_TOKEN_MAX];
    char threshold[CONFIG_ATTR_MAX];
    char timestamp[CONFIG_ATTR_MAX];
}doi_config_info_t;

typedef struct {
    char motion_enable[CONFIG_ATTR_MAX];
    char human_enable[CONFIG_ATTR_MAX];
    char tamper_enable[CONFIG_ATTR_MAX];
    char quite_interval[CONFIG_ATTR_MAX];
    char url[CONFIG_STRING_MAX];
    char auth_token[AUTH_TOKEN_MAX];
    char roi_coord[CONFIG_STRING_MAX];
} events_provision_info_t;

typedef struct {
    char night2day[CONFIG_ATTR_MAX];
    char day2night[CONFIG_ATTR_MAX];
} lux_threshold_info_t;

typedef struct {
    char user_name[CONFIG_STRING_MAX];
    char password[CONFIG_STRING_MAX];
} usr_creds_info_t;

typedef struct {
    char url[CONFIG_STRING_MAX];
    char auth_token[AUTH_TOKEN_MAX];
    char interval[CONFIG_ATTR_MAX];
    char timeZone[CONFIG_ATTR_MAX];
} _config_t;


/**
 * @brief read the Polling configuration .
 * @param name is the cvr_provision_info_t
 * @return RDKC_UPDATED on success,otherwise RDKC_FAILURE on failure.
 */
int readPollingConfig(_config_t *crf);

/**
 * @brief write the Polling configuration .
 * @param name is the cvr_provision_info_t
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writePollingConfig(_config_t *crf);

/**
 * @brief read the CVR could configuration .
 * @param name is the cvr_provision_info_t
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readCloudRecorderConfig(cvr_provision_info_t *crf);

/**
 * @brief write the CVR could configuration.
 * @param name is the cvr_provision_info_t 
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeCloudRecorderConfig(cvr_provision_info_t *crf);

/**
 * @brief read the motion event configuration.
 * @param name is the  detection_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readDetectionConfig(detection_provision_info_t *crf);

/**
 * @brief write the motion event configuration.
 * @param name is the detection_provision_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeDetectionConfig(detection_provision_info_t *crf);

/**
 * @brief read the motion event configuration.
 * @param name is the  events_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readEventConfig(events_provision_info_t *crf);

/**
 * @brief write the motion event configuration.
 * @param name is the events_provision_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeEventConfig(events_provision_info_t *crf);

/**
 * @brief read the ding configuration.
 * @param name is the  ding_config_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
*/
int readDingConfig(ding_config_info_t *cnf);

/**
 * @brief write the ding configuration.
 * @param name is the ding_config_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */

int writeDingConfig(ding_config_info_t *cnf);

/**
 * @brief read the chime configuration.
 * @param name is the  chime_config_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
*/
int readChimeConfig(chime_config_info_t *cnf);

/**
 * @brief write the chime configuration.
 * @param name is the chime_config_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */

int writeChimeConfig(chime_config_info_t *cnf);

/**
 * @brief read the sensor configuration.
 * @param name is the  sensor_config_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
*/
int readSensorConfig(sensor_config_info_t *cnf);

/**
 * @brief write the sensor  configuration.
 * @param name is the sensor_config_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */

int writeSensorConfig(sensor_config_info_t *cnf);
/**
 * @brief Get the EMS configurations.
 * @param name is the ems_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readEMSConfig(ems_provision_info_t *crf);

/**
 * @brief write the EMS configuration.
 * @param name is the ems_provision_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeEMSConfig(ems_provision_info_t *crf);

/**
 * @brief Get the livecache configurations.
 * @param name is the  livecahe_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readLiveCacheConfig(livecache_provision_info_t *crf);

/**
 * @brief store the livecache configurations.
 * @param name is the  livecahe_provision_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeLiveCacheConfig(livecache_provision_info_t *crf);

/**
 * @brief Get the thumbnail configuration.
 * @param name is the tn_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readTNConfig(tn_provision_info_t *crf);

/**
 * @brief store the thumbnail configuration.
 * @param name is the  tn_provision_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeTNConfig(tn_provision_info_t *crf);

/**
 * @brief Get the KVS provisioin info.
 * @param name is kvs_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readKVSConfig(kvs_provision_info_t *crf);

/**
 * @brief Store the KVS provisioin info.
 * @param name is the  kvs_provision_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeKVSConfig(kvs_provision_info_t *crf);

/**
 * @brief Get the cvrStats provision info.
 * @param name is cvrStats_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readCVRStatsConfig(cvrStats_provision_info_t *crf);

/**
 * @brief Store the cvrStats provision info.
 * @param name is the cvrStats_provision_info_t.
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeCVRStatsConfig(cvrStats_provision_info_t *crf);

/**
 * @brief Read the LUX threshold 
 * @param name is the lux_threshold_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readLUXConfig(lux_threshold_info_t *crf);

/**
 * @brief store the LUX threshold.
 * @param name is lux_threshold_info_t
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeLUXConfig(lux_threshold_info_t *crf);

/**
 * @brief store the user credentials.
 * @param name is usr_creds_info_t
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeUserCredentialInfo(usr_creds_info_t *crf);

/**
 * @brief Read the user credentials 
 * @param name is the usr_creds_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readUserCredentialInfo(usr_creds_info_t *crf);

/**
 * @brief store the doi config.
 * @param name is doi_config_info_t
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writeDOIConfig(doi_config_info_t *crf);
/**
 * @brief Get the DOI info.
 * @param name is doi_config_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readDOIConfig(doi_config_info_t *crf);

int polling_config_init();
int polling_config_exit();
#ifdef __cplusplus
}
#endif
#endif




