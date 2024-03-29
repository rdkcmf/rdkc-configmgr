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
        if (!(inFp = v_secure_popen("r","pgrep %s", processname))) {
		printf("\n Unable to locate process %s\n",processname);
		return -1;
	} else {
		fscanf(inFp,"%d",&pid);
		v_secure_pclose(inFp);
		if (pid > 0) {
			kill(pid, signalname);
		} else {
			printf("\n sendsignaltoprocess : Unable to locate process %s\n",processname);
			return -1;
		}
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
	cvrConfig = (cvr_provision_info_t*) config;

	v_secure_system("/lib/rdk/sys_multisecupdate.sh '%s'#'%s'#'%s' '%s'#'%s'#'%s' '%s'#'%s'#'%s' '%s'#'%s'#'%s' '%s'#'%s'#'%s' ",
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_ENABLE, (char*)cvrConfig->enable,
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_VIDEO_DURATION, (char*)cvrConfig->cvr_segment_info.duration,
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_VIDEO_FORMAT, (char*)cvrConfig->cvr_segment_info.format,
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_VIDEO_ADDRESS, (char*)cvrConfig->url,
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_VIDEO_AUTH, (char*)cvrConfig->auth_token);

	/*char cmd_buf[1024] = {0};
	snprintf(cmd_buf, sizeof(cmd_buf), "/lib/rdk/sys_multisecupdate.sh '%s'#'%s'#'%s' '%s'#'%s'#'%s' '%s'#'%s'#'%s' '%s'#'%s'#'%s' '%s'#'%s'#'%s' ",
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_ENABLE, (char*)cvrConfig->enable,
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_VIDEO_DURATION, (char*)cvrConfig->cvr_segment_info.duration,
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_VIDEO_FORMAT, (char*)cvrConfig->cvr_segment_info.format,
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_VIDEO_ADDRESS, (char*)cvrConfig->url,
					(char*)SEC_CLOUD_RECORDER,(char*)CLOUD_RECORDER_VIDEO_AUTH, (char*)cvrConfig->auth_token);
	system(cmd_buf);*/

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

	//send signal to cvr_daemon
	sendsignaltoprocess("cvr_daemon_kvs2",SIGUSR1);

	return retVal;
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
