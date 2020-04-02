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

#include "system_config_update.h"
#include "secure_wrapper.h"

/**
 * @brief  send signal to process
 * @return success/failure
 */
int sendsignaltoprocess(const char* processname,int signalname)
{
	pid_t pid = -1;
	FILE *inFp = NULL;
	char command[32];
	//sprintf(command, "pidof %s", processname);
	//if (!(inFp = popen(command, "r"))) {
          if (!(inFp = v_secure_popen("r","pidof %s", processname))) {
		printf("\n Unable to locate process %s\n",processname);
		return -1;
	}

	fscanf(inFp,"%d",&pid);
	pclose(inFp);
	if (pid > 0) {
		kill(pid, signalname);
	} else {
		printf("\n Unable to locate process %s\n",processname);
		return -1;
	}
	return 0;
}

/** @description Write the system config params to system.conf.
 *  @param tag : Module Name.
 *  @param config : Module Struct.
 *  @return integer (RDKC_SUCCESS if success, RDKC_FAILURE if failure).
 */
int systemConfigWrite(char* tag, void* config, bool abrBitrateChanged)
{
	int retVal = RDKC_SUCCESS;
	char set_param_val[MAXSIZE] = {0};

        if((tag == NULL) || (config == NULL))
        {
                perror("systemConfigWrite Invalid Params \n");
                return RDKC_FAILURE;
        }

	if(!strcmp(tag, XH_TAG_NAME_CVR))
        {
		cvrConfig = (cvr_provision_info_t*) config;

		if((PRO_SetInt((char*)SEC_CLOUD_RECORDER, (char*)CLOUD_RECORDER_ENABLE, atoi(cvrConfig->enable), (char*)CONF_FILE)) != 0)
		{
			perror("\nFailed to PRO_SetInt CVR Enable in system.conf \n");
			retVal = RDKC_FAILURE;
		}

		if((PRO_SetInt((char*)SEC_CLOUD_RECORDER, (char*)CLOUD_RECORDER_VIDEO_DURATION, atoi(cvrConfig->cvr_segment_info.duration), (char*)CONF_FILE)) != 0)
		{
			perror("\nFailed to PRO_SetInt CVR video duration in system.conf \n");
			retVal = RDKC_FAILURE;
		}

		if((PRO_SetInt((char*)SEC_CLOUD_RECORDER, (char*)CLOUD_RECORDER_VIDEO_FORMAT, atoi(cvrConfig->cvr_segment_info.format), (char*)CONF_FILE)) != 0)
		{
			perror("\nFailed to PRO_SetInt CVR video format in system.conf \n");
			retVal = RDKC_FAILURE;
		}

		if((PRO_SetStr((char*)SEC_CLOUD_RECORDER, (char*)CLOUD_RECORDER_VIDEO_ADDRESS, (char*)cvrConfig->url, (char*)CONF_FILE)) != 0)
		{
			perror("\nFailed to PRO_SetStr CVR polling url in system.conf \n");
			retVal = RDKC_FAILURE;
		}

		if((PRO_SetStr((char*)SEC_CLOUD_RECORDER, (char*)CLOUD_RECORDER_VIDEO_AUTH, (char*)cvrConfig->auth_token, (char*)CONF_FILE)) != 0)
		{
			perror("\nFailed to PRO_SetStr CVR polling auth token in system.conf \n");
			retVal = RDKC_FAILURE;
		}

		if(abrBitrateChanged)
		{
			/* Store ABR configuration in AbrTargetBitrate structure  */
			AbrTargetBitrate cvr_lbr_temp;
			memset(&cvr_lbr_temp, 0, sizeof(cvr_lbr_temp));

			if (atoi(cvrConfig->cvr_segment_info.cvr_lbr_info.min) > 0)
			{
				cvr_lbr_temp.no_mot_bitrate = atoi(cvrConfig->cvr_segment_info.cvr_lbr_info.min);
			}

			if (atoi(cvrConfig->cvr_segment_info.cvr_lbr_info.max) > 0)
			{
			    cvr_lbr_temp.high_mot_bitrate = atoi(cvrConfig->cvr_segment_info.cvr_lbr_info.max);
			}

			if (atoi(cvrConfig->cvr_segment_info.cvr_lbr_info.low) > 0)
			{
				cvr_lbr_temp.low_mot_bitrate = atoi(cvrConfig->cvr_segment_info.cvr_lbr_info.low);
			}

			if (atoi(cvrConfig->cvr_segment_info.cvr_lbr_info.med) > 0)
			{
				cvr_lbr_temp.mid_mot_bitrate = atoi(cvrConfig->cvr_segment_info.cvr_lbr_info.med);
			}

			/* Store received ABR configuration to system conf file. */
			if (abrSystemConfigWrite(&cvr_lbr_temp) != RDKC_SUCCESS) {
				perror("\nABR config write failed \n");
			}
		}
	}
	
	//send signal to cvr_daemon
	sendsignaltoprocess("cvr_daemon_kvs",SIGUSR1);

	return retVal;
}

/** @description Read the system config params from system.conf.
 *  @param tag : Module Name.
 *  @param config : Module Struct.
 *  @return integer (RDKC_SUCCESS if success, RDKC_FAILURE if failure).
 */
int systemConfigRead(char* tag, void* config)
{
	if((tag == NULL) || (config == NULL))
	{
		perror("systemConfigRead Invalid Params \n");
		return RDKC_FAILURE;
	}

	FILE *fd = NULL;
    	fd = fopen(CONF_FILE,"rt");
	int polling_enable = 0;

	if((PRO_GetInt((char*)SEC_CLOUD_RECORDER, (char*)CLOUD_RECORDER_ENABLE, &polling_enable, fd)) != 0)
	{
		perror("\nFailed to PRO_GetInt CVR recorder enable from system.conf \n");
		return RDKC_FAILURE;
	}
	/* ToDo : Copy system config params from get_param_val */

	return RDKC_SUCCESS;
}

#define  DEF_CVR_CHANNEL                3

/** @description Write the ABR configurations to system.conf.
 *  @param tag : Module Name.
 *  @param config : Module Struct.
 *  @return integer (RDKC_SUCCESS if success, RDKC_FAILURE if failure).
 */
int abrSystemConfigWrite(AbrTargetBitrate *abr_bitrate)
{
	All_Conf *pconf;
	char str[ABR_KEY_AREA_LEN];
	char MyXPCMsg[CGI_ERR_MSG_SIZE_MAX];
	int cfg_chg_flag = 0;

	memset(str, 0, sizeof(str));
	memset(MyXPCMsg, 0, sizeof(MyXPCMsg));

	printf("%s(%d): Inside abrSystemConfigWrite: min = %d, max = %d, low = %d, med = %d\n",
		__FUNCTION__, __LINE__, abr_bitrate->no_mot_bitrate, abr_bitrate->high_mot_bitrate, abr_bitrate->low_mot_bitrate, abr_bitrate->mid_mot_bitrate);

	/* Read all config settings */
	pconf = (All_Conf*)malloc(sizeof(All_Conf));
	if(NULL == pconf) {
	       printf("%s(%d): Memory allocation error for All Conf!!\n", __FUNCTION__, __LINE__);
	       return RDKC_FAILURE;
	}
	memset(pconf, 0, sizeof(All_Conf));

	if (ReadAllConf(pconf, MyXPCMsg) != 0)
	{
		printf("%s(%d): Error reading all conf settings.\n", __FUNCTION__, __LINE__);
		if(pconf) {
			free(pconf);
		}
		return RDKC_FAILURE;
	}

	ABR_SetConf(abr_bitrate, str, DEF_CVR_CHANNEL, pconf->img_conf.h264[DEF_CVR_CHANNEL].quality_level-1);

	if (set_abr_conf_by_index(str, 1, MyXPCMsg, pconf, (LIST *)NULL, DEF_CVR_CHANNEL, pconf->img_conf.h264[DEF_CVR_CHANNEL].resolution) != 0)
	{
		printf("%s(%d): Failed to set ABR bitrate!\n", __FUNCTION__, __LINE__);
		return RDKC_FAILURE;
	}

	printf("%s(%d): Finished ABR_SetConf \n",__FUNCTION__, __LINE__);

	/* save all config settings to memory anf flash */
	if (SaveAllConf(pconf, MyXPCMsg, 1, &cfg_chg_flag))
	{
		printf("%s(%d): saving to conf file failed \n",__FUNCTION__, __LINE__);
	}

	/* apply new settings to work */
	if (cfg_chg_flag)
	{
		printf("%s(%d): Configuration changed make changes live \n",__FUNCTION__, __LINE__);
		AllFuncReact(pconf, MyXPCMsg);
	}

	if(pconf) {
		free(pconf);
	}

	return RDKC_SUCCESS;
}
