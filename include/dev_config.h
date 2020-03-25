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
#ifndef __DEV_CONFIG_H_
#define __DEV_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#include "config_api.h"
}
#endif

//VIDEO-ANALYTICS
#define VA_ENGINE			    "VA_algorithm"
#define FRAME_TYPE			    "Frame_Type"
#define OBJ_SENSITIVITY			    "od_sensitivity"
#define MD_MODE                             "md_mode"

//WEBRTC
#define DEFAULT_MIV_BITRATE		    "default_min_bitrate"
#define DEFAULT_MAX_BITRATE		    "default_max_bitrate"
#define DEFAULT_MAX_FRAMERATE		    "default_max_framerate"

//TEST_HARNESS COMMON
#define TEST_HARNESS_ON_FILE_ENABLED	    "Test_Harness_on_File_Enabled"

//TEST-HARNESS PHASE II
#define RESET_ALG_ON_FIRST_FRAME	    "Reset_Alg_on_First_Frame"
#define NW_INTERFACE			    "nw_interface"			
#define HOSTNAME			    "hostname"
#define PORT				    "port"

//OVERLAY
#define STREAM_ID			    "stream_id"
#define AREA_ID				    "area_id"
#define OVERLAY_X_ORDINATE_720P		    "overlay_x_ord_720p"
#define OVERLAY_Y_ORDINATE_720P		    "overlay_y_ord_720p"
#define OVERLAY_X_ORDINATE_1080P	    "overlay_x_ord_1080p"
#define OVERLAY_Y_ORDINATE_1080P	    "overlay_y_ord_1080p"
#define OVERLAY_TRANSPARENCY		    "overlay_transparency"
#define OVERLAY_IMAGE_PATH		    "overlay_image_path"

//XPC_Retry
#define XPC_RETRY_MAX_INTERVAL		    "xpc_max_retry_interval"
#define XPC_RETRY_MIN_INTERVAL		    "xpc_min_retry_interval"
#define XPC_RETRY_FACTOR		    "xpc_exp_retry_factor"

//CVR_Upload
#define CVR_UPLOAD_RETRY_MAX_INTERVAL	    "cvr_upload_max_retry_interval"
#define CVR_UPLOAD_RETRY_MIN_INTERVAL	    "cvr_upload_min_retry_interval"
#define CVR_UPLOAD_RETRY_FACTOR		    "cvr_upload_exp_retry_factor"


//Server URL for ntpdate
#define SERVER1_URL			    "ntp_server1"
#define SERVER2_URL			    "ntp_server2"
#define SERVER3_URL			    "ntp_server3"
#define NTP_UPDATED_TIME		    "ntp_updated_time"
#define NTP_BEST_SERVER_INDEX		    "ntp_best_server_index"
#define NTP_SYNC_WAIT_TIME		    "ntp_sync_wait_time"

//CloudRecorderInfo
#define POLLING_URL			    "polling_url"
#define POLLING_AUTH	                    "polling_auth"
#define POLLING_INTERVAL		    "polling_interval"
#define POLLING_ENABLE			    "enable"
#define VIDEO_DURATION			    "video_duration"
#define VIDEO_FORMAT			    "video_format"
#define VIDEO_ADDRESS			    "video_address"
#define VIDEO_AUTH			    "video_auth"
#define VIDEO_USERNAME			    "video_username"
#define VIDEO_PASSWORD			    "video_password"
#define POLLING_TIMEOUT			    "timeout"

//CVR-UPLOAD
#define DNS_CACHE_TIMEOUT		    "dns_cache_timeout"

//THUMBNAIL_UPLOAD
#define THUMBNAIL_UPLOAD_ENABLE		    "thumbnail_upload_enable"
#define THUMBNAIL_UPLOAD_URL                "thumbnail_upload_url"
#define THUMBNAIL_UPLOAD_AUTH_TOKEN         "thumbnail_upload_auth_token"
#define THUMBNAIL_UPLOAD_PASSIVE_INTERVAL   "thumbnail_upload_passive_interval"
#define THUMBNAIL_UPLOAD_ACTIVE_INTERVAL    "thumbnail_upload_active_interval"
#define THUMBNAIL_ACTIVE_UPLOAD_DURATION    "thumbnail_active_upload_duration"
#define THUMBNAIL_BUF_ID		    "thumbnail_buf_id"

//RKDC AUDIO ANALYTIC
#define AA_THRESHOLD_SMOKE_ALARM            "aa_threshold_smoke_alarm"
#define AA_THRESHOLD_BABY_CRY               "aa_threshold_baby_cry"
#define AA_THRESHOLD_DOG_BARK               "aa_threshold_dog_bark"
#define AA_THRESHOLD_GLASS_BREAK            "aa_threshold_glass_break"

//DAY-NIGHT MODE SENSITIVITY
#define IV_DAY_SENSITIVITY		    "IV_Day_Mode_Sensitivity"
#define IV_NIGHT_SENSITIVITY		    "IV_Night_Mode_Sensitivity"
#define XCV_DAY_SENSITIVITY		    "XCV_Day_Mode_Sensitivity"
#define XCV_NIGHT_SENSITIVITY		    "XCV_Night_Mode_Sensitivity"

//IVA-DAEMON 4TH BUFFER RESOLUTION
#define XCV_FRAME_RESOLUTION		    "XCV_frame_resolution"
#define IV_FRAME_RESOLUTION		    "IV_frame_resolution"

//LIVE_STREAMING_INFO
#define MAX_LIVE_VIDEO_STREAM_SUPPORTED     "Max_Live_Video_Stream_Supported"
#define ENCODED_STREAM_0_ENABLED            "Encoded_Stream_0_Enabled"
#define ENCODED_STREAM_1_ENABLED            "Encoded_Stream_1_Enabled"
#define ENCODED_STREAM_2_ENABLED            "Encoded_Stream_2_Enabled"
#define ENCODED_STREAM_3_ENABLED            "Encoded_Stream_3_Enabled"
#define LIVE_VIDEO_STREAM_1                 "Live_Video_Stream_1"

//LIVE_STREAMING_AUDIO
#define ENABLE_AUDIO_LIVE_STREAM            "enable_audio_on_LS"

//CVR_AUDIO
#define CVR_AUDIO_STATUS		    "cvr_audio_status"

//Partner_ID
#define PARTNER_ID		    "partnerid"

//Account_ID
#define ACCOUNT_ID                   "service_number"

//Account hash
#define ACCOUNT_HASH		     "accounthash"

//smart thumbnail
#define SMART_THUMBNAIL_UPLOAD_URL      "smart_thumbnail_upload_URL"
#define SMART_THUMBNAIL_AUTH_CODE       "smart_thumbnail_auth_code"

#endif
//to suppress the motion event for below duration(in seconds)
#define EVENT_QUIET_TIME	    "event_quiet_time"
