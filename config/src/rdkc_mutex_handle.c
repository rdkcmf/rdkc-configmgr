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
#include "rdkc_mutex_handle.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int mutexInit(pthread_mutex_t *mutex)
{
    int mret=-1;
    pthread_mutexattr_t attrs; 
    pthread_mutexattr_init(&attrs);
    pthread_mutexattr_settype(&attrs, PTHREAD_MUTEX_RECURSIVE);

    if(mutex==NULL) {
	//mret = pthread_mutex_init(mutex, NULL);
	mret = pthread_mutex_init(mutex, &attrs);
       	if(mret != 0) {
            printf("mutex init failed\n");
            return FAILURE;
   	    }
    } 
    return SUCCESS;
}

int mutexAcquire(pthread_mutex_t *mutex)
{
    int mret=-1;

    if(mutex==NULL) {

	printf("mutex init failed\n");
	return FAILURE;

    } else {

	mret=pthread_mutex_lock(mutex);
	if(mret==0) {
	    //printf("Acquiring . . . !!!\n");
	    return SUCCESS;
	}
	return FAILURE;
    }
}

int mutexRelease(pthread_mutex_t *mutex)
{
	int mret=-1;

        if(mutex==NULL) {

            printf("mutex init failed\n");
            return FAILURE;

        } else {

	    mret=pthread_mutex_unlock(mutex);
	    if(mret==0) {
	        //printf("Releasing . . . !!!\n");
		return SUCCESS;
	    }
	    return FAILURE;
        }
}
