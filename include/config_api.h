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
#ifndef CONFIG_API_H
#define CONFIG_API_H

/**
 * @brief  Retrieves the parameter value from the dev config.
 * @param  name is a pointer to the parameter name.
 * @return NULL if parameter not found, otherwise the parameter value .
 */
const char* rdkc_envGet(const char *name);

/**
 * @brief  Update the parameter value of usr config. 
 * @param  param is a pointer to the parameter name.
 * @param  value is pointer to the parameter value.
 * @return RDKC_SUCCESS on success, RDKC_FAILURE otherwise.
 */
int rdkc_envSet(const char* name ,const char* value);

/**
 * @brief  Update the parameter value of usr config. 
 * @param  param is a pointer to the parameter name.
 * @param  value is pointer to the parameter value.
 * @return RDKC_SUCCESS on success, RDKC_FAILURE otherwise.
 */
int rdkc_set_user_setting(const char* name ,const char* value);

/**
 * @brief  Retrieves the parameter value from the usr config.
 * @param  param is a pointer to the parameter name.
 * @param  value is pointer to the parameter value.
 * @return RDKC_SUCCESS on success, RDKC_FAILURE otherwise.
 */
int rdkc_get_user_setting(const char* name , char* value);

/**
 * @brief  initializes the parameter list from dev_config and usr_config. 
 * @param  void
 * @return RDKC_SUCCESS on success, RDKC_FAILURE otherwise.
 */
int config_init();

/**
 * @brief free up the parameter list
 * @param void
 * @return RDKC_SUCCESS on success, RDKC_FAILURE otherwise.
 */
int config_release();

#endif
