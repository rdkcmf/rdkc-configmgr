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

#include "polling_config.h"

#include "PRO_file.h"
#include "conf_sec.h"
#include "sc_tool.h"
#include "RFCConfigAPI.h"
#include "RFCCommon.h"

static pthread_mutex_t polling_config_mutex;
static int polling_inited = FALSE;

static int is_json_config_enabled = RDKC_FAILURE;

int readValues(FILE *pFile, char *pToken, char *data);
int checkRepeatedValues(void *WcrfIn, void *RcrfIn, char *configFile);

/**
 * @brief check the given values are already set
 * @param name is the cvr_provision_info_t
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int checkRepeatedValues(void *WcrfIn, void *RcrfIn, char *configFile)
{
	if(!strcmp(POLLING_CONFIG_FILE, configFile))
	{
		_config_t *Wcrf = (_config_t*)WcrfIn;
		_config_t *Rcrf = (_config_t*)RcrfIn;
		if(strcmp(Wcrf->url, Rcrf->url)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->auth_token, Rcrf->auth_token)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->interval, Rcrf->interval)) return RDKC_SUCCESS;
	}
	else if(!strcmp(CLOUDRECORDER_CONFIG_FILE, configFile))
	{
		cvr_provision_info_t *Wcrf = (cvr_provision_info_t*)WcrfIn;
		cvr_provision_info_t *Rcrf = (cvr_provision_info_t*)RcrfIn;
		if(strcmp(Wcrf->enable, Rcrf->enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->url, Rcrf->url)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->auth_token, Rcrf->auth_token)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->audio_enable, Rcrf->audio_enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.format, Rcrf->cvr_segment_info.format)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.duration, Rcrf->cvr_segment_info.duration)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.cvr_lbr_info.min, Rcrf->cvr_segment_info.cvr_lbr_info.min)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.cvr_lbr_info.max, Rcrf->cvr_segment_info.cvr_lbr_info.max)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.cvr_lbr_info.med, Rcrf->cvr_segment_info.cvr_lbr_info.med)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.cvr_lbr_info.low, Rcrf->cvr_segment_info.cvr_lbr_info.low)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.cvr_stream_info.resolution, Rcrf->cvr_segment_info.cvr_stream_info.resolution)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.cvr_stream_info.fps, Rcrf->cvr_segment_info.cvr_stream_info.fps)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cvr_segment_info.cvr_stream_info.gop, Rcrf->cvr_segment_info.cvr_stream_info.gop)) return RDKC_SUCCESS;
	}
	else if(!strcmp(DETECTION_CONFIG_FILE, configFile))
	{
		detection_provision_info_t *Wcrf = (detection_provision_info_t*)WcrfIn;
		detection_provision_info_t *Rcrf = (detection_provision_info_t*)RcrfIn;
		if(strcmp(Wcrf->motion_enable, Rcrf->motion_enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->sensitivity, Rcrf->sensitivity)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->env, Rcrf->env)) return RDKC_SUCCESS;
	}
	else if(!strcmp(EVENTS_CONFIG_FILE, configFile))
	{
		events_provision_info_t *Wcrf = (events_provision_info_t*)WcrfIn;
		events_provision_info_t *Rcrf = (events_provision_info_t*)RcrfIn;
		if(strcmp(Wcrf->motion_enable, Rcrf->motion_enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->human_enable, Rcrf->human_enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->tamper_enable, Rcrf->tamper_enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->quite_interval, Rcrf->quite_interval)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->url, Rcrf->url)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->auth_token, Rcrf->auth_token)) return RDKC_SUCCESS;
	}
	else if(!strcmp(EMS_CONFIG_FILE, configFile))
	{
		ems_provision_info_t *Wcrf = (ems_provision_info_t*)WcrfIn;
		ems_provision_info_t *Rcrf = (ems_provision_info_t*)RcrfIn;
		if(strcmp(Wcrf->enable, Rcrf->enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->roomId, Rcrf->roomId)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->ers_url, Rcrf->ers_url)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->ers_port, Rcrf->ers_port)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->auth_token, Rcrf->auth_token)) return RDKC_SUCCESS;
	}
	else if(!strcmp(LIVECACHE_CONFIG_FILE, configFile))
	{
		livecache_provision_info_t *Wcrf = (livecache_provision_info_t*)WcrfIn;
		livecache_provision_info_t *Rcrf = (livecache_provision_info_t*)RcrfIn;
		if(strcmp(Wcrf->enable, Rcrf->enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->url, Rcrf->url)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->auth_token, Rcrf->auth_token)) return RDKC_SUCCESS;
	}
	else if(!strcmp(TN_CONFIG_FILE, configFile))
	{
		tn_provision_info_t *Wcrf = (tn_provision_info_t*)WcrfIn;
		tn_provision_info_t *Rcrf = (tn_provision_info_t*)RcrfIn;
		if(strcmp(Wcrf->enable, Rcrf->enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->height, Rcrf->height)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->width, Rcrf->width)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->quality, Rcrf->quality)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->url, Rcrf->url)) return RDKC_SUCCESS;
	}
	else if(!strcmp(KVS_CONFIG_FILE, configFile))
	{
		kvs_provision_info_t *Wcrf = (kvs_provision_info_t*)WcrfIn;
		kvs_provision_info_t *Rcrf = (kvs_provision_info_t*)RcrfIn;
		if(strcmp(Wcrf->enable, Rcrf->enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->role_alias, Rcrf->role_alias)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->credentialProvider, Rcrf->credentialProvider)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->stream_name, Rcrf->stream_name)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->force_CSR_req, Rcrf->force_CSR_req)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cert_CSR_enable, Rcrf->cert_CSR_enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cert_CSR_url, Rcrf->cert_CSR_url)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cert_CSR_auth, Rcrf->cert_CSR_auth)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cert_CA_enable, Rcrf->cert_CA_enable)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cert_CA_url, Rcrf->cert_CA_url)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cert_CA_auth, Rcrf->cert_CA_auth)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cert_CA_id, Rcrf->cert_CA_id)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->cert_CA_force, Rcrf->cert_CA_force)) return RDKC_SUCCESS;
		return RDKC_SUCCESS;
	}
	else if(!strcmp(LUX_CONFIG_FILE, configFile))
	{
		lux_threshold_info_t *Wcrf = (lux_threshold_info_t*)WcrfIn;
		lux_threshold_info_t *Rcrf = (lux_threshold_info_t*)RcrfIn;
		if(strcmp(Wcrf->night2day, Rcrf->night2day)) return RDKC_SUCCESS;
		if(strcmp(Wcrf->day2night, Rcrf->day2night)) return RDKC_SUCCESS;
	}
        else if(!strcmp(SECURITY_CONFIG_FILE, configFile))
        {
                usr_creds_info_t *Wcrf = (usr_creds_info_t*)WcrfIn;
                usr_creds_info_t *Rcrf = (usr_creds_info_t*)RcrfIn;
                if(strcmp(Wcrf->user_name, Rcrf->user_name)) return RDKC_SUCCESS;
                if(strcmp(Wcrf->password, Rcrf->password)) return RDKC_SUCCESS;
        }
	return RDKC_FAILURE;
}

/**
 * @brief read the file for value of a given token
 * @param name is the cvr_provision_info_t
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readValues(FILE *pFile, char *pToken, char *data)
{
	char buffer[DATA_LEN];
	char *keyValue;
	if(pFile == NULL)
		return RDKC_FAILURE;
	/* Search the token in the config txt file and copy the value */
	fseek(pFile, 0, SEEK_SET);
	while(fgets(buffer, DATA_LEN, pFile)!= NULL )
	{
	    if( 0 != strlen(buffer) ) {
		keyValue = strtok( buffer, "=" );
		if(!(strcmp(keyValue, pToken)))
		{
			keyValue = strtok(NULL, "\n" );
			if(keyValue != NULL)
			{
				strncpy(data, keyValue,strlen(keyValue));
				data[strlen(keyValue)] = '\0'; // Adding '\0' is required if strncpy is used
			}else {
				strncpy(data, "",1);
				data[1] = '\0';
			}
			break;
		}
	    }
	}
	return RDKC_SUCCESS;
}

/**
 * @brief read the Polling configuration .
 * @param name is the cvr_provision_info_t
 * @return RDKC_UPDATED on success,otherwise RDKC_FAILURE on failure.
 */
int readPollingConfig(_config_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;
	if(crf == NULL)
	{
		printf("crf is null\n");
		return RDKC_FAILURE;
	}
	readFile = fopen(POLLING_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		printf("No file named %s\n",POLLING_CONFIG_FILE);
		return RDKC_FAILURE;
	}

	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		fclose(readFile);
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	retVal = readValues(readFile, XH_ATTR_INTERVAL, crf->interval);
	retVal = readValues(readFile, XH_ATTR_URL, crf->url);
	retVal = readValues(readFile, XH_ATTR_AUTH, crf->auth_token);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		fclose(readFile);
		return RDKC_FAILURE;
	}
	fclose(readFile);
	return retVal;
}

/**
 * @brief write the Polling configuration .
 * @param name is the cvr_provision_info_t
 * @return RDKC_UPDATED if there is an UPDATE in configuration,RDKC_SUCCESS on success otherwise RDKC_FAILURE on failure.
 */
int writePollingConfig(_config_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	_config_t *Rcrf;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (_config_t*)malloc (sizeof(_config_t));
	retVal = readPollingConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)POLLING_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
			if(Rcrf)
				free(Rcrf);
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(POLLING_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		perror("Error while accquiring mutex!!\n ");
		return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	sprintf(buffer, "%s=%s\n", XH_ATTR_INTERVAL, crf->interval);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_URL, crf->url);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_AUTH, crf->auth_token);
	fputs((const char *) buffer, writeFile);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		perror("Error While Releasing Mutex!!\n ");
		return RDKC_FAILURE;
	}
	free(Rcrf);
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/**
 * @brief read the CVR could configuration .
 * @param name is the cvr_provision_info_t
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readCloudRecorderConfig(cvr_provision_info_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;
	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}
	readFile = fopen(CLOUDRECORDER_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		fclose(readFile);
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	retVal = readValues(readFile, XH_ATTR_ENABLED, crf->enable);
	retVal = readValues(readFile, XH_ATTR_URL, crf->url);
	retVal = readValues(readFile, XH_ATTR_AUTH, crf->auth_token);
	retVal = readValues(readFile, XH_ATTR_AUDIO, crf->audio_enable);
	retVal = readValues(readFile, XH_ATTR_FORMAT, crf->cvr_segment_info.format);
	retVal = readValues(readFile, XH_ATTR_DURATION, crf->cvr_segment_info.duration);
	retVal = readValues(readFile, XH_ATTR_MIN, crf->cvr_segment_info.cvr_lbr_info.min);
	retVal = readValues(readFile, XH_ATTR_MAX, crf->cvr_segment_info.cvr_lbr_info.max);
	retVal = readValues(readFile, XH_ATTR_MED, crf->cvr_segment_info.cvr_lbr_info.med);
	retVal = readValues(readFile, XH_ATTR_LOW, crf->cvr_segment_info.cvr_lbr_info.low);
	retVal = readValues(readFile, XH_ATTR_RESOLUTION, crf->cvr_segment_info.cvr_stream_info.resolution);
	retVal = readValues(readFile, XH_ATTR_FPS, crf->cvr_segment_info.cvr_stream_info.fps);
	retVal = readValues(readFile, XH_ATTR_GOP, crf->cvr_segment_info.cvr_stream_info.gop);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		fclose(readFile);
		return RDKC_FAILURE;
	}
	fclose(readFile);
	return retVal;
}

/**
 * @brief write the CVR could configuration.
 * @param name is the cvr_provision_info_t
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeCloudRecorderConfig(cvr_provision_info_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	cvr_provision_info_t *Rcrf;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (cvr_provision_info_t*)malloc (sizeof(cvr_provision_info_t));
	retVal = readCloudRecorderConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)CLOUDRECORDER_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
			if(Rcrf)
				free(Rcrf);
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(CLOUDRECORDER_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}

	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	bool abrBitrateChanged = false;

	if(strcmp(crf->cvr_segment_info.cvr_lbr_info.min, Rcrf->cvr_segment_info.cvr_lbr_info.min) != 0 )
	{
		abrBitrateChanged = true;
	}
	else if(strcmp(crf->cvr_segment_info.cvr_lbr_info.max, Rcrf->cvr_segment_info.cvr_lbr_info.max) != 0 )
	{
		abrBitrateChanged = true;
	}
	else if(strcmp(crf->cvr_segment_info.cvr_lbr_info.med, Rcrf->cvr_segment_info.cvr_lbr_info.med) != 0 )
	{
		abrBitrateChanged = true;
	}
	else if(strcmp(crf->cvr_segment_info.cvr_lbr_info.low, Rcrf->cvr_segment_info.cvr_lbr_info.low)  != 0 )
	{
		abrBitrateChanged = true;
	}

	sprintf(buffer, "%s=%s\n", XH_ATTR_ENABLED, crf->enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_URL, crf->url);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_AUTH, crf->auth_token);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_AUDIO, crf->audio_enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_FORMAT, crf->cvr_segment_info.format);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_DURATION, crf->cvr_segment_info.duration);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_MIN, crf->cvr_segment_info.cvr_lbr_info.min);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_MAX, crf->cvr_segment_info.cvr_lbr_info.max);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_MED, crf->cvr_segment_info.cvr_lbr_info.med);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_LOW, crf->cvr_segment_info.cvr_lbr_info.low);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_RESOLUTION, crf->cvr_segment_info.cvr_stream_info.resolution);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_FPS, crf->cvr_segment_info.cvr_stream_info.fps);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_GOP, crf->cvr_segment_info.cvr_stream_info.gop);
	fputs((const char *) buffer, writeFile);

#if defined (SOC_JSON_CONFIG_ENABLE)
#ifdef XCAM2
        if(systemConfigWrite(XH_TAG_NAME_CVR, (void*)crf, abrBitrateChanged)!=RDKC_SUCCESS) // Write the config params to system.conf
        {
                perror("Polling systemConfigWrite write cvr config to system.conf failed");
        }
#else
	if( is_json_config_enabled == RDKC_SUCCESS )
        {
		if(systemConfigWrite(XH_TAG_NAME_CVR, (void*)crf, abrBitrateChanged)!=RDKC_SUCCESS) // Write the config params to system.conf
		{
			perror("Polling systemConfigWrite write cvr config to system.conf failed");
		}
	}
#endif
#else
	perror("\nSOC_JSON_CONFIG is not enabled in configMgr source code\n");
#endif

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	free(Rcrf);
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/**
 * @brief read the motion event configuration.
 * @param name is the  events_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readEventConfig(events_provision_info_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}
	readFile = fopen(EVENTS_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		fclose(readFile);
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	retVal = readValues(readFile, XH_TAG_NAME_MOTION, crf->motion_enable);
	retVal = readValues(readFile, XH_TAG_NAME_HUMAN, crf->human_enable);
	retVal = readValues(readFile, XH_TAG_NAME_TAMPER, crf->tamper_enable);
	retVal = readValues(readFile, XH_ATTR_QUIET_INTERVAL, crf->quite_interval);
	retVal = readValues(readFile, XH_ATTR_URL, crf->url);
	retVal = readValues(readFile, XH_ATTR_AUTH, crf->auth_token);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		fclose(readFile);
		return RDKC_FAILURE;
	}
	fclose(readFile);
	return retVal;
}

/**
 * @brief write the motion event configuration.
 * @param name is the events_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeEventConfig(events_provision_info_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	events_provision_info_t *Rcrf;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (events_provision_info_t*)malloc (sizeof(events_provision_info_t));
	retVal = readEventConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)EVENTS_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
			if(Rcrf)
				free(Rcrf);
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(EVENTS_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	sprintf(buffer, "%s=%s\n", XH_TAG_NAME_MOTION, crf->motion_enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_TAG_NAME_HUMAN, crf->human_enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_TAG_NAME_TAMPER, crf->tamper_enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_QUIET_INTERVAL, crf->quite_interval);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_URL, crf->url);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_AUTH, crf->auth_token);
	fputs((const char *) buffer, writeFile);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	free(Rcrf);
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/**
 * @brief read the motion event configuration.
 * @param name is the  events_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readDetectionConfig(detection_provision_info_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}
	readFile = fopen(DETECTION_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		fclose(readFile);
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	retVal = readValues(readFile, XH_ATTR_ENABLED, crf->motion_enable);
	retVal = readValues(readFile, XH_ATTR_SENSITIVITY, crf->sensitivity);
	retVal = readValues(readFile, XH_ATTR_ENV, crf->env);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		fclose(readFile);
		return RDKC_FAILURE;
	}

	fclose(readFile);
	return retVal;
}

/**
 * @brief write the motion detection configuration.
 * @param name is the detection_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeDetectionConfig(detection_provision_info_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	detection_provision_info_t *Rcrf;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (detection_provision_info_t*)malloc (sizeof(detection_provision_info_t));
	retVal = readDetectionConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)DETECTION_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
			if(Rcrf)
				free(Rcrf);
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(DETECTION_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}

	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
        perror("Error while accquiring mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
	    return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	sprintf(buffer, "%s=%s\n", XH_ATTR_ENABLED, crf->motion_enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_SENSITIVITY, crf->sensitivity);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_ENV, crf->env);
	fputs((const char *) buffer, writeFile);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	free(Rcrf);
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/**
 * @brief Get the EMS configurations.
 * @param name is the ems_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readEMSConfig(ems_provision_info_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}
	readFile = fopen(EMS_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		fclose(readFile);
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	retVal = readValues(readFile, XH_ATTR_ENABLED, crf->enable);
	retVal = readValues(readFile, XH_ATTR_ROOMID, crf->roomId);
	retVal = readValues(readFile, XH_ATTR_URL, crf->ers_url);
	retVal = readValues(readFile, XH_ATTR_ERSPORT, crf->ers_port);
	retVal = readValues(readFile, XH_ATTR_AUTH, crf->auth_token);
        retVal = readValues(readFile, XH_ATTR_RESOLUTION, crf->resolution);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		fclose(readFile);
		return RDKC_FAILURE;
	}
	fclose(readFile);
	return retVal;
}

/**
 * @brief write the EMS configuration.
 * @param name is the ems_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeEMSConfig(ems_provision_info_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	ems_provision_info_t *Rcrf;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (ems_provision_info_t*)malloc (sizeof(ems_provision_info_t));
	retVal = readEMSConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)EMS_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
			if(Rcrf)
				free(Rcrf);
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(EMS_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	sprintf(buffer, "%s=%s\n", XH_ATTR_ENABLED, crf->enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_ROOMID, crf->roomId);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_URL, crf->ers_url);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_ERSPORT, crf->ers_port);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_AUTH, crf->auth_token);
	fputs((const char *) buffer, writeFile);

        sprintf(buffer, "%s=%s\n", XH_ATTR_RESOLUTION, crf->resolution);
        fputs((const char *) buffer, writeFile);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);

		return RDKC_FAILURE;
	}
	free(Rcrf);
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/**
 * @brief Get the livecache configurations.
 * @param name is the  livecahe_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readLiveCacheConfig(livecache_provision_info_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}
	readFile = fopen(LIVECACHE_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		fclose(readFile);
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	retVal = readValues(readFile, XH_ATTR_ENABLED, crf->enable);
	retVal = readValues(readFile, XH_ATTR_URL, crf->url);
	retVal = readValues(readFile, XH_ATTR_AUTH, crf->auth_token);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		fclose(readFile);
		return RDKC_FAILURE;
	}
	fclose(readFile);
	return retVal;
}

/**
 * @brief store the livecache configurations.
 * @param name is the  livecahe_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeLiveCacheConfig(livecache_provision_info_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	livecache_provision_info_t *Rcrf;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (livecache_provision_info_t*)malloc (sizeof(livecache_provision_info_t));
	retVal = readLiveCacheConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)LIVECACHE_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
			if(Rcrf)
				free(Rcrf);
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(LIVECACHE_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	sprintf(buffer, "%s=%s\n", XH_ATTR_ENABLED, crf->enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_URL, crf->url);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_AUTH, crf->auth_token);
	fputs((const char *) buffer, writeFile);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	free(Rcrf);
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/**
 * @brief Get the thumbnail configuration.
 * @param name is the tn_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readTNConfig(tn_provision_info_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}
	readFile = fopen(TN_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		fclose(readFile);
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	retVal = readValues(readFile, XH_ATTR_ENABLED, crf->enable);
	retVal = readValues(readFile, XH_ATTR_HEIGHT, crf->height);
	retVal = readValues(readFile, XH_ATTR_WIDTH, crf->width);
	retVal = readValues(readFile, XH_ATTR_QUALITY, crf->quality);
	retVal = readValues(readFile, XH_ATTR_URL, crf->url);
	retVal = readValues(readFile, XH_ATTR_AUTH, crf->auth_token);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		fclose(readFile);
		return RDKC_FAILURE;
	}
	fclose(readFile);
	return retVal;
}

/**
 * @brief store the thumbnail configuration.
 * @param name is the  tn_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeTNConfig(tn_provision_info_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	tn_provision_info_t *Rcrf;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (tn_provision_info_t*)malloc (sizeof(tn_provision_info_t));
	retVal = readTNConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)TN_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
			if(Rcrf)
				free(Rcrf);
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(TN_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	sprintf(buffer, "%s=%s\n", XH_ATTR_ENABLED, crf->enable);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_HEIGHT, crf->height);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_WIDTH, crf->width);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_QUALITY, crf->quality);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_URL, crf->url);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_AUTH, crf->auth_token);
	fputs((const char *) buffer, writeFile);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	free(Rcrf);
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/**
 * @brief Get the KVS provisioin info.
 * @param name is kvs_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readKVSConfig(kvs_provision_info_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}
	readFile = fopen(KVS_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		fclose(readFile);
		perror("Error while accquiring mutex!!\n ");
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	/* Update the config file here */
	char tmp_buff[DATA_LEN] = {'\0'};

        retVal = readValues(readFile, XH_ATTR_ENABLED, crf->enable);
        retVal = readValues(readFile, XH_ATTR_ROLE_ALIAS, crf->role_alias);
	retVal = readValues(readFile, XH_ATTR_CRED_PROVIDER, crf->credentialProvider);
        retVal = readValues(readFile, XH_ATTR_STREAM_NAME, crf->stream_name);
        retVal = readValues(readFile, XH_ATTR_FORCE_CSR_REQ, crf->force_CSR_req);
	sprintf(tmp_buff, "%s_%s", XH_TAG_NAME_CSR_CERT, XH_ATTR_ENABLED);
        retVal = readValues(readFile, tmp_buff, crf->cert_CSR_enable);
	memset(tmp_buff, 0, sizeof(tmp_buff));
	sprintf(tmp_buff, "%s_%s", XH_TAG_NAME_CSR_CERT, XH_ATTR_URL);
        retVal = readValues(readFile, tmp_buff, crf->cert_CSR_url);
        memset(tmp_buff, 0, sizeof(tmp_buff));
	sprintf(tmp_buff, "%s_%s", XH_TAG_NAME_CSR_CERT, XH_ATTR_AUTH);
        retVal = readValues(readFile, tmp_buff, crf->cert_CSR_auth);
        memset(tmp_buff, 0, sizeof(tmp_buff));
	sprintf(tmp_buff, "%s_%s", XH_TAG_NAME_CA_CERT, XH_ATTR_ENABLED);
        retVal = readValues(readFile, tmp_buff, crf->cert_CA_enable);
        memset(tmp_buff, 0, sizeof(tmp_buff));
	sprintf(tmp_buff, "%s_%s", XH_TAG_NAME_CA_CERT, XH_ATTR_URL);
        retVal = readValues(readFile, tmp_buff, crf->cert_CA_url);
        memset(tmp_buff, 0, sizeof(tmp_buff));
	sprintf(tmp_buff, "%s_%s", XH_TAG_NAME_CA_CERT, XH_ATTR_AUTH);
        retVal = readValues(readFile, tmp_buff, crf->cert_CA_auth);
        memset(tmp_buff, 0, sizeof(tmp_buff));
	sprintf(tmp_buff, "%s_%s", XH_TAG_NAME_CA_CERT, XH_ATTR_ID);
        retVal = readValues(readFile, tmp_buff, crf->cert_CA_id);
        memset(tmp_buff, 0, sizeof(tmp_buff));
	sprintf(tmp_buff, "%s_%s", XH_TAG_NAME_CA_CERT, XH_ATTR_FORCE_USE_CA);
        retVal = readValues(readFile, tmp_buff, crf->cert_CA_force);
        memset(tmp_buff, 0, sizeof(tmp_buff));
	//retVal = RDKC_SUCCESS;

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		fclose(readFile);
		perror("Error While Releasing Mutex!!\n ");
		return RDKC_FAILURE;
	}
	fclose(readFile);
	return retVal;
}

/**
 * @brief Store the KVS provisioin info.
 * @param name is the  kvs_provision_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeKVSConfig(kvs_provision_info_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	kvs_provision_info_t *Rcrf = NULL;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (kvs_provision_info_t*)malloc (sizeof(kvs_provision_info_t));
	retVal = readKVSConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)KVS_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
                        if(Rcrf) {
				free(Rcrf);
				Rcrf = NULL;
			}
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(KVS_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
                if(Rcrf) {
			free(Rcrf);
			Rcrf = NULL;
		}
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		if(writeFile) {
 			fclose(writeFile);
		}
                if(Rcrf) {
			free(Rcrf);
			Rcrf = NULL;
		}
		return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	/* Update the config file here */
	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s=%s\n", XH_ATTR_ENABLED, crf->enable);
        fputs((const char *) buffer, writeFile);
	
	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s=%s\n", XH_ATTR_ROLE_ALIAS, crf->role_alias);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s=%s\n", XH_ATTR_CRED_PROVIDER, crf->credentialProvider);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s=%s\n", XH_ATTR_STREAM_NAME, crf->stream_name);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%s=%s\n", XH_ATTR_FORCE_CSR_REQ, crf->force_CSR_req);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s_%s=%s\n", XH_TAG_NAME_CSR_CERT, XH_ATTR_ENABLED, crf->cert_CSR_enable);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s_%s=%s\n", XH_TAG_NAME_CSR_CERT, XH_ATTR_URL, crf->cert_CSR_url);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s_%s=%s\n", XH_TAG_NAME_CSR_CERT, XH_ATTR_AUTH, crf->cert_CSR_auth);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s_%s=%s\n", XH_TAG_NAME_CA_CERT, XH_ATTR_ENABLED, crf->cert_CA_enable);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s_%s=%s\n", XH_TAG_NAME_CA_CERT, XH_ATTR_URL, crf->cert_CA_url);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s_%s=%s\n", XH_TAG_NAME_CA_CERT, XH_ATTR_AUTH, crf->cert_CA_auth);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%s_%s=%s\n", XH_TAG_NAME_CA_CERT, XH_ATTR_ID, crf->cert_CA_id);
        fputs((const char *) buffer, writeFile);

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%s_%s=%s\n", XH_TAG_NAME_CA_CERT, XH_ATTR_FORCE_USE_CA, crf->cert_CA_force);
        fputs((const char *) buffer, writeFile);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   	        if(Rcrf) {
			free(Rcrf);
			Rcrf = NULL;
		}
		return RDKC_FAILURE;
	}

        if ( NULL != Rcrf ) {
		free(Rcrf);
		Rcrf = NULL;
	}
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/**
 * @brief Read the LUX threshold
 * @param name is the lux_threshold_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readLUXConfig(lux_threshold_info_t *crf)
{
	FILE *readFile;
	int retVal = RDKC_FAILURE;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}
	readFile = fopen(LUX_CONFIG_FILE, "r");
	if(readFile == NULL)
	{
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		fclose(readFile);
		return RDKC_ERR_CONF_WRITE_INPROGRESS;
	}

	retVal = readValues(readFile, XH_ATTR_NIGHT2DAY, crf->night2day);
	retVal = readValues(readFile, XH_ATTR_DAY2NIGHT, crf->day2night);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		fclose(readFile);
		return RDKC_FAILURE;
	}
	fclose(readFile);
	return retVal;
}

/**
 * @brief store the LUX threshold.
 * @param name is lux_threshold_info_t
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeLUXConfig(lux_threshold_info_t *crf)
{
	FILE *writeFile;
	char buffer[DATA_LEN];
	int retVal = RDKC_FAILURE;
	lux_threshold_info_t *Rcrf;

	if(crf == NULL)
	{
		return RDKC_FAILURE;
	}

	Rcrf = (lux_threshold_info_t*)malloc (sizeof(lux_threshold_info_t));
	retVal = readLUXConfig(Rcrf);
	if(retVal == RDKC_SUCCESS)
	{
		/* Check if data to be set is already set, if yes, return error */
		retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)LUX_CONFIG_FILE);
		if(retVal == RDKC_FAILURE)
		{
			if(Rcrf)
				free(Rcrf);
			return RDKC_ERR_DATA_ALREADY_SET;
		}
	}

	writeFile = fopen(LUX_CONFIG_FILE, "w");
	if(writeFile == NULL)
	{
		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error while accquiring mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_ERR_CONF_READ_INPROGRESS;
	}

	sprintf(buffer, "%s=%s\n", XH_ATTR_NIGHT2DAY, crf->night2day);
	fputs((const char *) buffer, writeFile);

	sprintf(buffer, "%s=%s\n", XH_ATTR_DAY2NIGHT, crf->day2night);
	fputs((const char *) buffer, writeFile);

	if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
	{
		perror("Error While Releasing Mutex!!\n ");
		if(writeFile)
 			fclose(writeFile);
   		if(Rcrf)
			free(Rcrf);
		return RDKC_FAILURE;
	}
	free(Rcrf);
	fclose(writeFile);
	return RDKC_SUCCESS;
}

/** @description: Reading the json flag via RFC.
 *  @param[in] file: file name
 *  @param[in] name: Name of the json flag
 *  @return: int
 */
int IsJSONEnabledInRFC( char *file, char *name )
{
        if ( ( NULL == file ) || ( NULL ==  name ) )
        {
                perror("Json polling : Either file or name is NULL\n");
                return RDKC_FAILURE;
        }
        if(RDKC_SUCCESS != RFCConfigInit())
        {
                perror("Json polling : RFCConfigInit Failed\n");
                return RDKC_FAILURE;
        }
        char value[MAX_SIZE] = { 0 };
	if ( RDKC_SUCCESS == IsRFCFileAvailable( file ) )
        {
                if ( RDKC_SUCCESS == GetValueFromRFCFile( file, name, value ) )
                {
                        //perror("Json polling : Able to get value from file\n");
                        if ( strcmp( value, RDKC_TRUE ) == 0 )
                        {
                                //perror("Json polling : RFC is set inside the file\n");
                                return RDKC_SUCCESS;
                        }
                        else
                        {
                                //perror("Json polling : RFC is not set inside the file\n");
                                return RDKC_FAILURE;
                        }
                }
                else
                {
                        //perror("Json polling : Unable to get value from the file\n");
                        return RDKC_FAILURE;
                }
        }
        else
        {
                perror("Json polling : RFC file is not available\n");
                return RDKC_FAILURE;
        }
}

/**
 * @brief Read the user credentials
 * @param name is the usr_creds_info_t.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int readUserCredentialInfo(usr_creds_info_t *crf)
{
        FILE *readFile;
        int retVal = RDKC_FAILURE;

        if(crf == NULL)
        {
                return RDKC_FAILURE;
        }
        readFile = fopen(SECURITY_CONFIG_FILE, "r");
        if(readFile == NULL)
        {
                return RDKC_FAILURE;
        }
        if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
        {
                perror("Error while accquiring mutex!!\n ");
                fclose(readFile);
                return RDKC_ERR_CONF_WRITE_INPROGRESS;
        }

        retVal = readValues(readFile, XH_ATTR_USER_NAME, crf->user_name);
        retVal = readValues(readFile, XH_ATTR_PASSWORD, crf->password);

        if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
        {
                perror("Error While Releasing Mutex!!\n ");
                fclose(readFile);
                return RDKC_FAILURE;
        }
        fclose(readFile);
        return retVal;
}

/**
 * @brief store the user credentials.
 * @param name is usr_creds_info_t
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */
int writeUserCredentialInfo(usr_creds_info_t *crf)
{
        FILE *fp = NULL;
        char buffer[DATA_LEN];
        int retVal = RDKC_FAILURE;
        FILE *writeFile = NULL;
        usr_creds_info_t *Rcrf;

        if(crf == NULL)
        {
                return RDKC_FAILURE;
        }
        
        Rcrf = (usr_creds_info_t*)malloc (sizeof(usr_creds_info_t));
        //open system config
        fp = fopen(SYSTEM_CONF,"r");
        if (NULL != fp){
          retVal = PRO_GetStr(SEC_USER, USER_ADMIN_NAME, Rcrf->user_name, sizeof(Rcrf->user_name),fp);
          if (retVal != 0)
            perror("Unable to get user name");

          retVal = PRO_GetStr(SEC_USER, USER_ADMIN_PASSWORD, Rcrf->password, sizeof(Rcrf->password),fp);
          if (retVal != 0)
            perror("Unable to get password");
 
          //close system config
          fclose(fp);
          fp = NULL;
        }else{
          perror("Unable to open system conf file");
        }  

        //retVal = readUserCredentialInfo(Rcrf);
        /* Check if data to be set is already set, if yes, return error */
        retVal = checkRepeatedValues((void*)crf, (void*)Rcrf, (char*)SECURITY_CONFIG_FILE);
        if(retVal == RDKC_FAILURE)
        {
                if(Rcrf)
                       free(Rcrf);
                return RDKC_ERR_DATA_ALREADY_SET;
        }
#ifdef XCAM2
        //Set Username and Password in system.conf
        retVal = PRO_SetStr(SEC_USER, USER_ADMIN_NAME, crf->user_name, SYSTEM_CONF);
        if (retVal != 0)
                perror("Unable to set User Admin Name");

        retVal = PRO_SetExt(SEC_USER, USER_ADMIN_PASSWORD, crf->password, SYSTEM_CONF);
        if (retVal != 0)
                perror("Unable to set Admin Password");

        retVal = PRO_SetInt(SEC_USER, USER_LOGIN_CHECK, 1, SYSTEM_CONF);
        if(retVal != 0)
                perror("Unable to set user_login check");

       system("usr/local/bin/auth -adm >/dev/null 2>/dev/null");
       SendSignal2AppsBySpecFile("hydra", SIGUSR1, "/tmp/hydra.lock");
       SendSignal2AppsBySpecFile("xw3_monitor", SIGUSR1, "/tmp/xw3_monitor.lock");
#endif

#if 0
        //Set Security credentials to /opt/usr_config/sys.conf
        writeFile = fopen(SECURITY_CONFIG_FILE, "w");
        if(writeFile == NULL)
        {
                if(Rcrf)
                        free(Rcrf);
                return RDKC_FAILURE;
        }
        if(pthread_mutex_lock(&polling_config_mutex)!=RDKC_SUCCESS)
        {
                perror("Error while accquiring mutex!!\n ");
                if(writeFile)
                        fclose(writeFile);
                if(Rcrf)
                        free(Rcrf);
                return RDKC_ERR_CONF_READ_INPROGRESS;
        }

        sprintf(buffer, "%s=%s\n", XH_ATTR_USER_NAME, crf->user_name);
        fputs((const char *) buffer, writeFile);

        sprintf(buffer, "%s=%s\n", XH_ATTR_PASSWORD, crf->password);
        fputs((const char *) buffer, writeFile);

        if(pthread_mutex_unlock(&polling_config_mutex)!=RDKC_SUCCESS)
        {
                perror("Error While Releasing Mutex!!\n ");
                if(writeFile)
                        fclose(writeFile);
                if(Rcrf)
                        free(Rcrf);
                return RDKC_FAILURE;
        }
        fclose(writeFile);
#endif
        if(Rcrf)
                free(Rcrf);
        return RDKC_SUCCESS;
}

int polling_config_init()
{
	int retVal = RDKC_FAILURE;
	if(polling_inited == TRUE)
	{
		return RDKC_ERR_ALREADY_INITED;
	}
	retVal = pthread_mutex_init(&polling_config_mutex, NULL);
	if(retVal != 0)
	{
		printf("%s [%d] Error Initializing Mutex!!!\n", __FILE__, __LINE__);
		return RDKC_FAILURE;
	}

	is_json_config_enabled = IsJSONEnabledInRFC( (char*)RFC_FILE, (char*)RFC_JSON_FLAG );

	polling_inited = TRUE;
	//printf("\nPolling Inited\n");
	return RDKC_SUCCESS;
}

int polling_config_exit()
{
	pthread_mutex_destroy(&polling_config_mutex);
	polling_inited = FALSE;
	//printf("\nPolling Exited\n");
	return RDKC_SUCCESS;
}

/******* END *********/
