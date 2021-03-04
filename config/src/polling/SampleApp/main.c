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

int main(int argc, char* argv[])
{
   printf("************Entering Polling Config***************\n");
   int retVal = RDKC_FAILURE;
   cvr_provision_info_t *Wcvr;
   cvr_provision_info_t *Rcvr;
   events_provision_info_t *Wevents;
   events_provision_info_t *Revents;
   ems_provision_info_t *Wems;
   ems_provision_info_t *Rems;
   livecache_provision_info_t *Wlcache;
   livecache_provision_info_t *Rlcache;
   tn_provision_info_t *Wtn;
   tn_provision_info_t *Rtn;
   kvs_provision_info_t *Wkvs;
   kvs_provision_info_t *Rkvs;
   lux_threshold_info_t *Wlux;
   lux_threshold_info_t *Rlux;

   retVal = polling_config_init();
   if(retVal == RDKC_FAILURE)
	  return RDKC_FAILURE;

/********** CLOUD RECORDER ***********/
   Wcvr = (cvr_provision_info_t*)malloc (sizeof(cvr_provision_info_t));
   strcpy(Wcvr->enable, "TRUE");
   strcpy(Wcvr->url, "http://cvrurl.com");
   strcpy(Wcvr->auth_token, "cvr_token=");
   strcpy(Wcvr->audio_enable, "AUDIO_ENABLED");
   strcpy(Wcvr->cvr_segment_info.format, "SEGMENT_FORMAT");
   strcpy(Wcvr->cvr_segment_info.duration, "SEGMENT_DURATION");
   strcpy(Wcvr->cvr_segment_info.cvr_lbr_info.min, "LBR_MIN");
   strcpy(Wcvr->cvr_segment_info.cvr_lbr_info.max, "LBR_MAX");
   strcpy(Wcvr->cvr_segment_info.cvr_lbr_info.med, "LBR_MED");
   strcpy(Wcvr->cvr_segment_info.cvr_lbr_info.low, "LBR_LOW");
   strcpy(Wcvr->cvr_segment_info.cvr_stream_info.resolution, "640x480");
   strcpy(Wcvr->cvr_segment_info.cvr_stream_info.fps, "STREAM_FPS");
   strcpy(Wcvr->cvr_segment_info.cvr_stream_info.gop, "STREAM_GOP");

   retVal = writeCloudRecorderConfig(Wcvr);
   printf("First Write retVal=%d  ", retVal);
   retVal = writeCloudRecorderConfig(Wcvr);
   printf("Second Write retVal=%d", retVal); /* retVal must be RDKC_ERR_DATA_ALREADY_SET */

   Rcvr = (cvr_provision_info_t*)malloc (sizeof(cvr_provision_info_t));
   retVal = readCloudRecorderConfig(Rcvr);
   printf("\n************CloudRecorderConfig**************\n enable=%s url=%s auth_token=%s audio_enable=%s format=%s duration=%s min=%s max=%s med=%s low=%s resolution=%s fps=%s gop=%s\n",    \
          Rcvr->enable, Rcvr->url, Rcvr->auth_token,  \
		  Rcvr->audio_enable, Rcvr->cvr_segment_info.format, Rcvr->cvr_segment_info.duration,   \
		  Rcvr->cvr_segment_info.cvr_lbr_info.min, Rcvr->cvr_segment_info.cvr_lbr_info.max,     \
		  Rcvr->cvr_segment_info.cvr_lbr_info.med, Rcvr->cvr_segment_info.cvr_lbr_info.low,     \
		  Rcvr->cvr_segment_info.cvr_stream_info.resolution, Rcvr->cvr_segment_info.cvr_stream_info.fps,   \
		  Rcvr->cvr_segment_info.cvr_stream_info.gop);
   printf("************CloudRecorderConfig**************\n\n");

/********** EVENT PROVISIION ***********/
   Wevents = (events_provision_info_t*)malloc (sizeof(events_provision_info_t));
   strcpy(Wevents->motion_enable, "MOTION_TRUE");
   strcpy(Wevents->human_enable, "HUMAN_TRUE");
   strcpy(Wevents->tamper_enable, "TAMPER_TRUE");
   strcpy(Wevents->quite_interval, "30 Secs");
   strcpy(Wevents->url, "http://eventsurl.com");
   strcpy(Wevents->auth_token, "events_token===");
   strcpy(Wevents->roi_coord, "[[0.0-0.0][0.0-0.5][0.0-1.0][1.0-1.0][1.0-0.5][1.0-0.0][0.0-0.0]]");

   retVal = writeEventConfig(Wevents);
   printf("First Write retVal=%d  ", retVal);
   retVal = writeEventConfig(Wevents);
   printf("Second Write retVal=%d", retVal); /* retVal must be RDKC_ERR_DATA_ALREADY_SET */

   Revents = (events_provision_info_t*)malloc (sizeof(events_provision_info_t));
   retVal = readEventConfig(Revents);
   printf("\n************EventProvisionConfig**************\n motion_enable=%s human_enable=%s tamper_enable=%s quite_interval=%s url=%s auth_token=%s roi_coord=%s\n",    \
          Revents->motion_enable, Revents->human_enable, Revents->tamper_enable,  \
		  Revents->quite_interval, Revents->url, Revents->auth_token, Revents->roi_coord);
   printf("************EventProvisionConfig**************\n\n");

/********** EMS PROVISIION ***********/
   Wems = (ems_provision_info_t*)malloc (sizeof(ems_provision_info_t));
   strcpy(Wems->enable, "EMS_ENABLED");
   strcpy(Wems->roomId, "EMS_ROOMID");
   strcpy(Wems->ers_url, "http://ersurl.com");
   strcpy(Wems->ers_port, "1234");
   strcpy(Wems->auth_token, "EMS_TOKEN===");

   retVal = writeEMSConfig(Wems);
   printf("First Write retVal=%d  ", retVal);
   retVal = writeEMSConfig(Wems);
   printf("Second Write retVal=%d", retVal); /* retVal must be RDKC_ERR_DATA_ALREADY_SET */

   Rems = (ems_provision_info_t*)malloc (sizeof(ems_provision_info_t));
   retVal = readEMSConfig(Rems);
   printf("\n************EmsProvisionConfig**************\n enable=%s roomId=%s ers_url=%s ers_port=%s auth_token=%s\n",    \
          Rems->enable, Rems->roomId, Rems->ers_url,  \
		  Rems->ers_port, Rems->auth_token);
   printf("************EmsProvisionConfig**************\n\n");

/********** LIVE CACHE ***********/
   Wlcache = (livecache_provision_info_t*)malloc (sizeof(livecache_provision_info_t));
   strcpy(Wlcache->enable, "LIVECACHE_ENABLED");
   strcpy(Wlcache->url, "http://liveurl.com");
   strcpy(Wlcache->auth_token, "LIVECACHE_TOKEN===");

   retVal = writeLiveCacheConfig(Wlcache);
   printf("First Write retVal=%d  ", retVal);
   retVal = writeLiveCacheConfig(Wlcache);
   printf("Second Write retVal=%d", retVal); /* retVal must be RDKC_ERR_DATA_ALREADY_SET */

   Rlcache = (livecache_provision_info_t*)malloc (sizeof(livecache_provision_info_t));
   retVal = readLiveCacheConfig(Rlcache);
   printf("\n************LiveCacheProvisionConfig**************\n enable=%s url=%s auth_token=%s\n",    \
          Rlcache->enable, Rlcache->url, Rlcache->auth_token);
   printf("************LiveCacheProvisionConfig**************\n\n");

/********** TN PROVISIION ***********/

   Wtn = (tn_provision_info_t*)malloc (sizeof(tn_provision_info_t));
   strcpy(Wtn->enable, "TN_ENABLED");
   strcpy(Wtn->height, "480");
   strcpy(Wtn->width, "640");
   strcpy(Wtn->quality, "HIGH");
   strcpy(Wtn->url, "http://tnurl.com");
   strcpy(Wtn->auth_token, "TN_TOKEN===");

   retVal = writeTNConfig(Wtn);
   printf("First Write retVal=%d  ", retVal);
   retVal = writeTNConfig(Wtn);
   printf("Second Write retVal=%d", retVal); /* retVal must be RDKC_ERR_DATA_ALREADY_SET */

   Rtn = (tn_provision_info_t*)malloc (sizeof(tn_provision_info_t));
   retVal = readTNConfig(Rtn);
   printf("\n************TNProvisionConfig**************\n enable=%s height=%s width=%s quality=%s url=%s auth_token=%s\n",    \
          Rtn->enable, Rtn->height, Rtn->width,  \
		  Rtn->quality, Rtn->url, Rtn->auth_token);
   printf("************TNProvisionConfig**************\n\n");

/********** KVS PROVISIION ***********/
   Wkvs = (kvs_provision_info_t*)malloc (sizeof(kvs_provision_info_t));

   retVal = writeKVSConfig(Wkvs);
   printf("First Write retVal=%d  ", retVal);
   retVal = writeKVSConfig(Wkvs);
   printf("Second Write retVal=%d", retVal); /* retVal will be 0, because KVS config does not have parameters towrite or read */

   Rkvs = (kvs_provision_info_t*)malloc (sizeof(kvs_provision_info_t));
   retVal = readKVSConfig(Rkvs);
   printf("\n************KVSProvisionConfig**************\n No Parameters to read\n");
   printf("************KVSProvisionConfig**************\n\n");

/********** LUX THRESHOLD ***********/
   Wlux = (lux_threshold_info_t*)malloc (sizeof(lux_threshold_info_t));
   strcpy(Wlux->night2day, "LUX_N2D");
   strcpy(Wlux->day2night, "LUX_D2N");

   retVal = writeLUXConfig(Wlux);
   printf("First Write retVal=%d  ", retVal);
   retVal = writeLUXConfig(Wlux);
   printf("Second Write retVal=%d", retVal); /* retVal must be RDKC_ERR_DATA_ALREADY_SET */

   Rlux = (lux_threshold_info_t*)malloc (sizeof(lux_threshold_info_t));
   retVal = readLUXConfig(Rlux);
   printf("\n************LuxThresholdConfig**************\n night2day=%s day2night=%s\n",    \
          Rlux->night2day, Rlux->day2night);
   printf("************LuxThresholdConfig**************\n\n");

/********* Free the mallocs **********/

   free(Wcvr);
   free(Rcvr);
   free(Wevents);
   free(Revents);
   free(Wems);
   free(Rems);
   free(Wlcache);
   free(Rlcache);
   free(Wtn);
   free(Rtn);
   free(Wkvs);
   free(Rkvs);
   free(Wlux);
   free(Rlux);

   polling_config_exit();
   printf("************End of Polling Config***************\n");
   return RDKC_SUCCESS;
}

