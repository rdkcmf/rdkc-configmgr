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
#include "rdkc_config_params.h"
#include "config_api.h"

static EnvVarNode *g_envCache = NULL;
static const char* g_usrConfigLoc;
static pthread_mutex_t g_rdkc_config_file_mutex; // = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/**
 * @brief This function is used to remove the white spaces at the begining and the end.
 * @param  instr is a pointer to the input string.
 * @param  outstr is a pointer to the output string.
 * @return None.
 */

static int trim(char *instr, char* outstr)
{
    if(!instr || !outstr)
	return RDKC_FAILURE;

    char *ptr = instr;
    char *endptr = instr + strlen(instr)-1;
    int length;

    length=-1;

    /* Advance pointer to first non-whitespace char */
    while (isspace(*ptr))
        ptr++;

    if (ptr > endptr) {
        /*
         * avoid breaking things when there are
         * no non-space characters in instr
         */
        outstr[0] = '\0';
        return RDKC_SUCCESS;
    }

    /* Move end pointer toward the front to first non-whitespace char */
    while (isspace(*endptr))
        endptr--;

    length = endptr + 1 - ptr;
    strncpy(outstr,ptr,length);
    outstr[length] = '\0';

    return RDKC_SUCCESS;
}

/**
 * @brief updates the parameter on basis of value in user configuration file.
 * @Param param is the parameter name.
 * @Param value is the parameter value.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

static int update_param_list(const char* param,const char* value) 
{
    EnvVarNode *node=g_envCache;

    /*search for node to update the value*/
    while(node!=NULL) {
        if(strcmp(node->name,param)==0) {
            if (node->value) {
                free(node->value);
                node->value=NULL;
            }
            node->value=strdup(value);
            return RDKC_SUCCESS;
        }
	node=node->next;
    }
	
    if(param!=NULL)
        printf("update_param_list : parameter \"%s\" in user config not available in parameter list\n",param);
    return RDKC_FAILURE;
}
/**
 * @brief Get the updated value to the configuration files.
 * @param name is the  parameter name.
 * @param value is the parameter value.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

static int get_usr_config(const char* name,char* value)
{
    char fullpath[MAX_BUFFER_SIZE] = {'\0'};
    FILE* fpUsr = NULL;

    /*generating the fullpath of user config file*/
    strcpy(fullpath,g_usrConfigLoc);
    strcat(fullpath,name);
    strcat(fullpath,".txt");

    if((fpUsr=fopen(fullpath,"r"))==NULL) {
        //printf("ERROR!  Could not open configuration file!\n");
        //printf("Tried %s\n", fullpath);
        return RDKC_FAILURE;
    }

    fgets(value,MAX_BUFFER_SIZE+1,fpUsr);
    value[strlen(value)-1] = '\0';

    if(fclose(fpUsr)){
	printf("get_usr_config : Error while closing files\n");
	return RDKC_FAILURE;
    }

    return RDKC_SUCCESS;
}


/**
 * @brief Writes the updated value to the configuration files.
 * @param name is the  parameter name.
 * @param value is the parameter value.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

static int set_usr_config(const char* name,const char* value)
{
    char fullpath[MAX_BUFFER_SIZE] = {'\0'};
    FILE* fpUsr = NULL;
    int path_len = 0;
    char newpath[MAX_BUFFER_SIZE] = {'\0'};
    /*generating the fullpath of user config file*/
    strcpy(fullpath,g_usrConfigLoc);
    strcat(fullpath,name);
    strcat(fullpath,".txt.new");
    path_len = strlen(fullpath) - 4;
    strncpy(newpath,fullpath,path_len);
    newpath[path_len] = '\0';
    if((fpUsr=fopen(fullpath,"w"))==NULL) {
        //printf("ERROR!  Could not open configuration file!\n");
        //printf("Tried %s\n", fullpath);
        return RDKC_FAILURE;
    }

    /*write the value to the user config file*/
    fprintf(fpUsr,"%s\n",value);

    if(fclose(fpUsr)){
	printf("set_usr_config : Error while closing files\n");
	return RDKC_FAILURE;
    }
    rename(fullpath,newpath);
    return RDKC_SUCCESS;
}

/**
 * @brief traverse the list of environment variable.
 *
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

static int traverse_param_list()
{
    EnvVarNode *node = g_envCache;

    if(mutexAcquire(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("Error while accquiring mutex!!\n ");
        return RDKC_FAILURE;
    }

    while (node != NULL) {
	printf("traverse_param_list : parameter name::%s\n",node->name);
	printf("traverse_param_list : parameter value::%s\n",node->value);
	node=node->next;
    }

    if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("traverse_param_list : Error While Releasing Mutex!!\n ");
        return RDKC_FAILURE;
    }
}

/**
 * @brief delete the list of environment variable if an error has occured while creating the list.
 *
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

int config_release()
{
    EnvVarNode *current = NULL;
    EnvVarNode *next = NULL;
    current = g_envCache;

//    printf("Deleting existing parameter list.\n");

    /*deleting the existing node*/
    while(current) {
        next=current->next;
        if(NULL != current->name) {
                free(current->name);
                current->name = NULL;
        }
        if(NULL != current->value) {
                free(current->value);
                current->value = NULL;
        }
        if(NULL != current) {
                free(current);
                current=NULL;
        }

        current=next;
    }

    if(g_envCache)
        g_envCache = NULL;

    return RDKC_SUCCESS;
}

/**
 * @brief  This function is used to parse the developer configuration file and generate the link list.
 * @param  path Path of the file.
 * @return returns RDKC_SUCCESS on success.
 */

static int rdkc_env_add_dev_config(const char *path)
{
    FILE* fpDev = NULL;
    char lineBuffer[MAX_BUFFER_SIZE];

    //printf("%s path \n",path);
    /* Open the default config file */
    if ((fpDev = fopen( path,"r")) == NULL) {
        //printf("ERROR!  Could not open configuration file!\n");
        //printf("Tried %s\n", path);
        return RDKC_FAILURE;
    }

    //printf("Conf file %s open success!!\n", path);

    /*accquire the mutex lock*/
    if(mutexAcquire(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_env_add_dev_config : Error while acquiring mutex!!\n");
        return RDKC_FAILURE;
    }

    while (fgets(lineBuffer,MAX_BUFFER_SIZE,fpDev) != NULL) {
        char name[MAX_BUFFER_SIZE] = {'\0'};
        char value[MAX_BUFFER_SIZE] = {'\0'};
        char trimname[MAX_BUFFER_SIZE] = {'\0'};
        char trimvalue[MAX_BUFFER_SIZE] = {'\0'};
        EnvVarNode *node = NULL;
        char *equals = NULL;
        int length = -1;

        /* Ignore comment lines */
        if ('#' == lineBuffer[0])
            continue;

        /* Ignore lines that do not have an '=' char */
        if ((equals = strchr(lineBuffer,DELIM_PARAM)) == NULL)
            continue;

        /* Read the property and store in the cache */
        length = equals - lineBuffer;
        strncpy( name,lineBuffer,length);
        name[length] = '\0'; /* have to null-term */

        length = lineBuffer + strlen(lineBuffer) - equals + 1;
        strncpy( value, equals+1, length);
        value[ length] = '\0' ;

        /* Trim all whitespace from name and value strings */
        trim( name,trimname);
        trim( value,trimvalue);

	/*Creating nodes for the Parameter list*/
	node = ( EnvVarNode*)malloc(sizeof(EnvVarNode));
        if(node) {
            node->name = strdup( trimname);
            node->value = strdup( trimvalue);
            /* Insert at the front of the list */
            node->next = g_envCache;
            g_envCache = node;
        } else {
            /*Deleting the existing parameter list.*/
	    config_release();

	    if(fclose(fpDev)!=0) {
		printf("rdkc_env_add_dev_config : Error while closing the file\n");
            }
	        /*release the lock in case of failure*/
            if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
                perror("rdkc_env_add_dev_config : Error while Releasing mutex!!");
                return RDKC_FAILURE;
            }

            return RDKC_FAILURE;
        }
    }

    /*release the lock in case of success*/
    if(mutexRelease(&g_rdkc_config_file_mutex)!=0) {
        perror("rdkc_env_add_dev_config : Error while Releasing mutex!!");
        return RDKC_FAILURE;
    }

    /*close the default config file*/
    if(fclose(fpDev)!=0) {
        printf("rdkc_env_add_dev_config : Error while closing the file\n");
        return RDKC_FAILURE;
    }
    return RDKC_SUCCESS;
}

/**
 * @brief  This function is used to parse the user configuration file and modifies the link list.
 * @param  dirPath Path of the user config directory.
 * @return returns RDKC_SUCCESS on success.
 */

static int rdkc_env_add_usr_config(const char* dirPath)
{
    FILE   *fpUsr = NULL;
    DIR    *usrConfigDir = NULL;
    struct dirent *usrConfigFile = NULL;
    char   filename[MAX_BUFFER_SIZE] = {'\0'};
    char   fullpath[MAX_BUFFER_SIZE] = {'\0'};
    char   param[MAX_BUFFER_SIZE] = {'\0'};
    char   value[MAX_BUFFER_SIZE] = {'\0'};
    int    length = -1;
    char   *period = NULL;

    /*accquire the lock*/
    if(mutexAcquire(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_env_add_usr_config : Error while acquiring mutex!!\n");
        return RDKC_FAILURE;
    }

    /*open the directory where the user config file exists*/
    if ((usrConfigDir = opendir (dirPath)) != NULL) {
        while ((usrConfigFile = readdir (usrConfigDir)) != NULL) {

            strcpy(filename,usrConfigFile->d_name);

            /*Ignore the current and parent directory,as well as invalid files*/
            if('.' == filename[0] || (period=strchr(filename,DELIM_FILENAME))==NULL)
                continue;

            length=period-filename;
            strncpy(param,filename,length);
            param[length]='\0';

            /*Generate the fullpath of user config file*/
            strcpy(fullpath,dirPath);
            strcat(fullpath,filename);

            /*opening the user config file*/
            if((fpUsr=fopen(fullpath,"r"))==NULL) {
                //printf("ERROR!  Could not open configuration file!\n");
                //printf("Tried %s\n", fullpath);

                /*release the lock in case of failure*/
                if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
                    perror("rdkc_env_add_usr_config : Error while Releasing mutex!!");
                    return RDKC_FAILURE;
                }
                return RDKC_FAILURE;
            }

            fgets(value,MAX_BUFFER_SIZE+1,fpUsr);
	    value[strlen(value)-1] = '\0';

            /*close the user config file*/
            if(fclose(fpUsr)!=0) {

                /*release the lock in case of failure*/
                if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
                    perror("rdkc_env_add_usr_config : Error while Releasing mutex!!");
                    return RDKC_FAILURE;
                }

                return RDKC_FAILURE;
            }
 
            if(update_param_list(param,value)!=RDKC_SUCCESS) {

                /*release the lock in case of failure*/
                if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
                    perror("rdkc_env_add_usr_config : Error while Releasing mutex!!");
                    return RDKC_FAILURE;
                }

                return RDKC_FAILURE;
            }
        }

        //Closing the Directory
        closedir(usrConfigDir);

        /*release the lock in case os succsess*/
        if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
            perror("rdkc_env_add_usr_config : Error while Releasing mutex!!");
            return RDKC_FAILURE;
        }

        return RDKC_SUCCESS;
    } else {

        printf("rdkc_env_add_usr_config : Unable to open usr_config directory.\n");

        /*release the lock in case of failure*/
        if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
            perror("rdkc_env_add_usr_config : Error while Releasing mutex!!");
            return RDKC_FAILURE;
        }
        return RDKC_FAILURE;
    }
}

/**
 * @brief This function is used to get the value of the specified environment variable.
 *
 * @param name is a pointer to the name of the target environment variable.
 * @return The NULL on failure,otherwise target environment variable is returned.
 */

const char* rdkc_envGet(const char *name){

    EnvVarNode *node = g_envCache;

    if(NULL == node) {
        printf("rdkc_envGet : null check error\n");
        return NULL;
    }

    if(mutexAcquire(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_envGet : Error while accquiring mutex!!\n ");
        return NULL;
    }

    while (node != NULL) {
        /* Env var name match */
        if (strcmp(name,node->name) == 0) {
            if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
                perror("rdkc_envGet : Error while releasing mutex!!\n ");
                return NULL;
            }
            /* return the value */
            return node->value;
        }
        node = node->next;
    }

    if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
         perror("rdkc_envGet : Error while releasing mutex!!\n");
         return NULL;
    }
    return NULL;
}

/**
 * @brief This function is used to set the value of the specified user config parameter 
 *
 * @param name is a pointer to the name of the target environment variable.
 * @param value is a pointer to a NULL terminated value string.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

int rdkc_envSet(const char* name ,const char* value)
{
    EnvVarNode *node = g_envCache;

   if(NULL==node) {
        printf("rdkc_envSet : node : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==g_usrConfigLoc) {
        printf("rdkc_envSet : g_usrConfigLoc : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==name) {
        printf("rdkc_envSet : name : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==value) {
        printf("rdkc_envSet : value : null check error!!\n");
        return RDKC_FAILURE;
    }

    /*accquire the mutex lock*/
    if(mutexAcquire(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_envSet : Error  while acquiring mutex!!\n");
        return RDKC_FAILURE;
    }

    /* write change to the user configuration file*/
    if(set_usr_config(name,value)!=RDKC_SUCCESS) {

        if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
            perror("rdkc_envSet : Error while releasing mutex!!\n");
            return RDKC_FAILURE;
        }

        return RDKC_FAILURE;
    }
#if 0
    /* Search for an existing entry and change the value in parameter list */
    if(update_param_list(name,value)!=RDKC_SUCCESS) {
        if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
            perror("Error while releasing mutex!!\n");
            return RDKC_FAILURE;
        }
        return RDKC_FAILURE;
    }
#endif

    /*release mutex on success*/
    if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_envSet : Error while releasing mutex!!\n");
        return RDKC_FAILURE;
    }
    return RDKC_SUCCESS;
}

/**
 * @brief This function is used to get the value of the specified user config parameter
 *
 * @param name is a pointer to the name of the target environment variable.
 * @param value is a pointer to a NULL terminated value string.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

int rdkc_get_user_setting(const char* name ,char* value)
{
    EnvVarNode *node = g_envCache;

    if(NULL==node) {
        printf("rdkc_get_user_setting : node : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==g_usrConfigLoc) {
        printf("rdkc_get_user_setting : g_usrConfigLoc : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==name) {
        printf("rdkc_get_user_setting : name : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==value) {
        printf("rdkc_get_user_setting : value : null check error!!\n");
        return RDKC_FAILURE;
    }

    /*accquire the mutex lock*/
    if(mutexAcquire(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_get_user_setting : Error  while acquiring mutex!!\n");
        return RDKC_FAILURE;
    }

    /* write change to the user configuration file*/
    if(get_usr_config(name,value)!=RDKC_SUCCESS) {

        if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
            perror("rdkc_get_user_setting : Error while releasing mutex!!\n");
            return RDKC_FAILURE;
        }

        return RDKC_FAILURE;
    }
#if 0
    /* Search for an existing entry and change the value in parameter list */
    if(update_param_list(name,value)!=RDKC_SUCCESS) {
        if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
            perror("Error while releasing mutex!!\n");
            return RDKC_FAILURE;
        }
        return RDKC_FAILURE;
    }
#endif

    /*release mutex on success*/
    if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_get_user_setting : Error while releasing mutex!!\n");
        return RDKC_FAILURE;
    }
    return RDKC_SUCCESS;
}



/**
 * @brief This function is used to set the value of the specified user config parameter 
 *
 * @param name is a pointer to the name of the target environment variable.
 * @param value is a pointer to a NULL terminated value string.
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

int rdkc_set_user_setting(const char* name ,const char* value)
{
    EnvVarNode *node = g_envCache;

   if(NULL==node) {
        printf("rdkc_set_user_setting : node : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==g_usrConfigLoc) {
        printf("rdkc_set_user_setting : g_usrConfigLoc : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==name) {
        printf("rdkc_set_user_setting : name : null check error!!\n");
        return RDKC_FAILURE;
    }

    if(NULL==value) {
        printf("rdkc_set_user_setting : value : null check error!!\n");
        return RDKC_FAILURE;
    }

    /*accquire the mutex lock*/
    if(mutexAcquire(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_set_user_setting : Error  while acquiring mutex!!\n");
        return RDKC_FAILURE;
    }

    /* write change to the user configuration file*/
    if(set_usr_config(name,value)!=RDKC_SUCCESS) {

        if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
            perror("rdkc_set_user_setting : Error while releasing mutex!!\n");
            return RDKC_FAILURE;
        }

        return RDKC_FAILURE;
    }
#if 0
    /* Search for an existing entry and change the value in parameter list */
    if(update_param_list(name,value)!=RDKC_SUCCESS) {
        if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
            perror("Error while releasing mutex!!\n");
            return RDKC_FAILURE;
    	}
	return RDKC_FAILURE;
    }
#endif
    
    /*release mutex on success*/
    if(mutexRelease(&g_rdkc_config_file_mutex)!=RDKC_SUCCESS) {
        perror("rdkc_set_user_setting : Error while releasing mutex!!\n");
        return RDKC_FAILURE;
    }    
    return RDKC_SUCCESS;
}

/**
 * @brief To initialize the mutex and parameter list from default config and user config.
 *
 * @return RDKC_SUCCESS on success,otherwise RDKC_FAILURE on failure.
 */

int config_init()
{
    DIR* dir = NULL;

    /*Checking if the Parameter list is already initialized*/
    if(g_envCache){
	    printf("config_init : Parameter list already initialized\n");
        return RDKC_SUCCESS;
    }

    /*Initializing mutex*/
    if(mutexInit(&g_rdkc_config_file_mutex)==-1) {
        perror("config_init : Error while initializing mutex!!!\n");
        return RDKC_FAILURE;
    }
    /* Initialize parameters from default developer config */
    if(rdkc_env_add_dev_config(DEFAULT_DEV_CONFIG_PATH) !=RDKC_SUCCESS)
    {
        /* Initialize parameter name with default parameter values*/
        if(rdkc_env_add_dev_config(DEV_CONFIG_PATH)!=RDKC_SUCCESS)
	    {
            printf("config_init : An Error Occured while initializing default parameter list\n");
            return RDKC_FAILURE;
        }
    }

    /* Get the path where user config files are avaialble */
    if((g_usrConfigLoc = rdkc_envGet(USER_CONFIG_PARAMETER))==NULL) {
        printf("config_init : User Config cannot be Located.\n");
        return RDKC_FAILURE;
    }

    errno = 0;
    dir = opendir(g_usrConfigLoc);
#if 0
    /* Update the parameters from usr_config if it exists */
    if(dir) {
        /* Update the parameter value with user config files */
        if(rdkc_env_add_usr_config(g_usrConfigLoc) != RDKC_SUCCESS) {
            printf("Error occured while modifying the parameter list from user configuration.\n");
            return RDKC_FAILURE;
        }

        return RDKC_SUCCESS;
    }
    /* if usr_config does not exists already , create it */
    else if(ENOENT == errno) {

        int status = -1;

        status = mkdir(g_usrConfigLoc, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

        if(status < 0){
            perror("Error creating usr_config directory\n");
            return RDKC_FAILURE;
        }

        printf("Created directory \"%s\"", g_usrConfigLoc);

        return RDKC_SUCCESS;
    }

    else {
        printf("Error Creating directory in path \"%s\"", g_usrConfigLoc);
        return RDKC_FAILURE;
    }
#endif
   if(ENOENT == errno) {
        int status = -1;

        status = mkdir(g_usrConfigLoc, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

        if(status < 0){
            printf("config_init : Error creating usr_config directory\n");
            closedir(dir);
	    return RDKC_FAILURE;
        }

        printf("config_init : Created directory \"%s\"", g_usrConfigLoc);

        /*  Close the user config directory */
        closedir(dir);
	return RDKC_SUCCESS;
    }
    return RDKC_SUCCESS;
}

