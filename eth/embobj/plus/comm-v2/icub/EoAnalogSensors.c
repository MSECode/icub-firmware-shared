/*
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

/* @file       EoAnalogSensors.c
    @brief      This file keeps ...
    @author     marco.accame@iit.it
    @date       Apr 18 2016
**/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h" 
#include "string.h"
#include "stdio.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EoAnalogSensors.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------

enum { eoas_others_numberof = 2 };

// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char * s_eoas_sensors_strings[] =
{
    "eoas_strain",
    "eoas_mais",
    "eoas_accel_mtb_int",
    "eoas_accel_mtb_ext",
    "eoas_gyros_mtb_ext",   
    "eoas_accel_st_lis3x",    
    "eoas_gyros_st_l3g4200d",
    "eoas_imu_acc",
    "eoas_imu_mag",
    "eoas_imu_gyr",
    "eoas_imu_eul",
    "eoas_imu_qua",
    "eoas_imu_lia",
    "eoas_imu_grv",
    "eoas_imu_status",
    "eoas_temperature",
    "eoas_psc_angle",
    "eoas_pos_angle",
    "eoas_ft",
    "eoas_battery"
};  EO_VERIFYsizeof(s_eoas_sensors_strings, eoas_sensors_numberof*sizeof(const char *));    


static const char * s_eoas_sensors_string_unknown = "eoas_unknown";
static const char * s_eoas_sensors_string_none = "eoas_none";


static const eOmap_str_str_u08_t s_boards_map_of_postypes[] =
{
    {"decideg", "eoas_pos_TYPE_decideg", eoas_pos_TYPE_decideg},

    {"none", "eoas_pos_TYPE_none", eoas_pos_TYPE_none},
    {"unknown", "eoas_pos_TYPE_unknown", eoas_pos_TYPE_unknown}    
};  EO_VERIFYsizeof(s_boards_map_of_postypes, (eoas_pos_TYPE_numberof+2)*sizeof(eOmap_str_str_u08_t))


static const eOmap_str_str_u08_t s_boards_map_of_posrots[] =
{
    {"zero", "eoas_pos_ROT_zero", eoas_pos_ROT_zero},
    {"plus180", "eoas_pos_ROT_plus180", eoas_pos_ROT_plus180},
    {"plus090", "eoas_pos_ROT_plus090", eoas_pos_ROT_plus090},
    {"minus090", "eoas_pos_ROT_minus090", eoas_pos_ROT_minus090},
    
    {"none", "eoas_pos_ROT_none", eoas_pos_ROT_none},
    {"unknown", "eoas_pos_ROT_unknown", eoas_pos_ROT_unknown}   
    
};  EO_VERIFYsizeof(s_boards_map_of_posrots, (eoas_pos_ROT_numberof+2)*sizeof(eOmap_str_str_u08_t))

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------



extern const char * eoas_sensor2string(eOas_sensor_t sensor)
{
    const char * ret = s_eoas_sensors_string_unknown;
    
    if(sensor < eoas_sensors_numberof)
    {
        return(s_eoas_sensors_strings[sensor]);
    }
    else if(eoas_none == sensor)
    {
        return(s_eoas_sensors_string_none);
    }


    return(ret);
}


extern eOas_sensor_t eoas_string2sensor(const char * string)
{    
    uint8_t i = 0;
    if(NULL == string)
    {
        return(eoas_unknown);
    }
    
    for(i=0; i<eoas_sensors_numberof; i++)
    {
        if(0 == strcmp(string, s_eoas_sensors_strings[i]))
        {
            return((eOas_sensor_t)(i+0));
        }
    }
    
    if(0 == strcmp(string, s_eoas_sensors_string_none))
    {
        return(eoas_none);
    }        
    
    return(eoas_unknown);    
}

extern const char * eoas_postype2string(eoas_pos_TYPE_t postype, eObool_t usecompactstring)
{    
    const eOmap_str_str_u08_t * map = s_boards_map_of_postypes;
    const uint8_t size = eoas_pos_TYPE_numberof+2;
    const uint8_t value = postype;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str); 
}


extern eoas_pos_TYPE_t eoas_string2postype(const char * string, eObool_t usecompactstring)
{    
    const eOmap_str_str_u08_t * map = s_boards_map_of_postypes;
    const uint8_t size = eoas_pos_TYPE_numberof+2;
    const uint8_t defvalue = eoas_pos_TYPE_unknown;
    
    return((eoas_pos_TYPE_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));     
}

extern const char * eoas_posrot2string(eoas_pos_ROT_t posrot, eObool_t usecompactstring)
{
    const eOmap_str_str_u08_t * map = s_boards_map_of_posrots;
    const uint8_t size = eoas_pos_ROT_numberof+2;
    const uint8_t value = posrot;
    const char * str = eo_common_map_str_str_u08__value2string(map, size, value, usecompactstring);
    
    if(NULL == str)
    {
        str = (eobool_true == usecompactstring) ? (map[size-1].str0) : (map[size-1].str1);
    }
    
    return(str);
}


extern eoas_pos_ROT_t eoas_string2posrot(const char * string, eObool_t usecompactstring)
{    
    const eOmap_str_str_u08_t * map = s_boards_map_of_posrots;
    const uint8_t size = eoas_pos_ROT_numberof+2;
    const uint8_t defvalue = eoas_pos_ROT_unknown;
    
    return((eoas_pos_ROT_t)eo_common_map_str_str_u08__string2value(map, size, string, usecompactstring, defvalue));    
}


enum { in3_mtb_pos = 0, in3_mtb4_pos = 1, in3_strain2_pos = 2, in3_rfe_pos = 3, in3_mtb4c_pos = 4, in3_strain2c_pos = 5 };

static const eObrd_cantype_t s_eoas_inertial3_supportedboards_types[] = { eobrd_cantype_mtb, eobrd_cantype_mtb4, eobrd_cantype_strain2, eobrd_cantype_rfe, eobrd_cantype_mtb4c, eobrd_cantype_strain2c };


extern uint8_t eoas_inertial3_supportedboards_numberof(void)
{
    return sizeof(s_eoas_inertial3_supportedboards_types)/sizeof(eObrd_cantype_t);
}

extern eObrd_cantype_t eoas_inertial3_supportedboards_gettype(uint8_t pos)
{
    if(pos >= eoas_inertial3_supportedboards_numberof())
    {
        return eobrd_cantype_none;
    }
    
    return s_eoas_inertial3_supportedboards_types[pos];
    
}

extern eOresult_t eoas_inertial3_setof_boardinfos_clear(eOas_inertial3_setof_boardinfos_t *set)
{
    if(NULL == set)
    {
        return eores_NOK_generic;
    }
    
    memset(set, 0, sizeof(eOas_inertial3_setof_boardinfos_t));
    uint8_t i;
    for(i=0; i<eOas_inertials3_boardinfos_maxnumber; i++)
    {
        set->data[i].type = eobrd_cantype_none;
    }
        
    return eores_OK;
}


extern eOresult_t eoas_inertial3_setof_boardinfos_add(eOas_inertial3_setof_boardinfos_t *set, const eObrd_info_t *brdinfo)
{
    if((NULL == brdinfo) || (NULL == set))
    {
        return eores_NOK_generic;
    }
    
    if(eobrd_cantype_mtb == brdinfo->type)
    {
        memcpy(&set->data[in3_mtb_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }
    
    if(eobrd_cantype_mtb4 == brdinfo->type)
    {
        memcpy(&set->data[in3_mtb4_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }
    
    if(eobrd_cantype_strain2 == brdinfo->type)
    {
        memcpy(&set->data[in3_strain2_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }    

    if(eobrd_cantype_rfe == brdinfo->type)
    {
        memcpy(&set->data[in3_rfe_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }
    
    if(eobrd_cantype_mtb4c == brdinfo->type)
    {
        memcpy(&set->data[in3_mtb4c_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }

    if(eobrd_cantype_strain2c == brdinfo->type)
    {
        memcpy(&set->data[in3_strain2c_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }       
    
    return eores_NOK_generic;
}


extern const eObrd_info_t * eoas_inertial3_setof_boardinfos_find(const eOas_inertial3_setof_boardinfos_t *set, eObrd_cantype_t brdtype)
{
    if(NULL == set)
    {
        return NULL;
    }
    
    const eObrd_info_t * r = NULL;

    switch(brdtype)
    {
                
        case eobrd_cantype_mtb:
        {
            if(eobrd_cantype_mtb == set->data[in3_mtb_pos].type)
            {
                r = &set->data[in3_mtb_pos];
            }
        } break;
        
        case eobrd_cantype_mtb4:
        {
            if(eobrd_cantype_mtb4 == set->data[in3_mtb4_pos].type)
            {
                r = &set->data[in3_mtb4_pos];
            }
        } break;

        case eobrd_cantype_strain2:
        {
            if(eobrd_cantype_strain2 == set->data[in3_strain2_pos].type)
            {
                r = &set->data[in3_strain2_pos];
            }
        } break;
        
        case eobrd_cantype_rfe:
        {
            if(eobrd_cantype_rfe == set->data[in3_rfe_pos].type)
            {
                r = &set->data[in3_rfe_pos];
            }
        } break;  

        case eobrd_cantype_mtb4c:
        {
            if(eobrd_cantype_mtb4c == set->data[in3_mtb4c_pos].type)
            {
                r = &set->data[in3_mtb4c_pos];
            }
        } break;

        case eobrd_cantype_strain2c:
        {
            if(eobrd_cantype_strain2c == set->data[in3_strain2c_pos].type)
            {
                r = &set->data[in3_strain2c_pos];
            }
        } break;           
        
        default: 
        {
            r = NULL;
        } break;
    }
    
    return r;
}

extern icubCanProto_imu_sensor_t eoas_inertial3_imu_to_canproto(eOas_inertial3_type_t t)
{
    icubCanProto_imu_sensor_t ret = icubCanProto_imu_none;
    
    if((t >= 7) && (t <=13))
    {
        ret = (icubCanProto_imu_sensor_t) ((uint8_t)t-7);
    }
    else if(t == eoas_imu_status)
    {   
        ret = icubCanProto_imu_status;
    }
  
    return ret;    
}

extern eOas_inertial3_type_t eoas_inertial3_canproto_to_imu(uint8_t v)
{
    //enum class imuSensor { acc = 0, mag = 1, gyr = 2, eul = 3, qua = 4, lia = 5, grv = 6, status = 15, none = 16 };
    static const eOas_inertial3_type_t mymap666[7] = 
    {
        eoas_inertial3_imu_acc,
        eoas_inertial3_imu_mag,
        eoas_inertial3_imu_gyr,
        eoas_inertial3_imu_eul,
        eoas_inertial3_imu_qua,
        eoas_inertial3_imu_lia,
        eoas_inertial3_imu_grv  
    };
    
    
    if(v>=7)
    {
        if(15 == v)
        {
            return eoas_inertial3_imu_status;
        }
        else
        {
            return eoas_inertial3_unknown;
        }
    }
    
    return mymap666[v];
}


enum { temp_mtb4_pos = 0, temp_strain2_pos = 1, temp_mtb4c_pos = 2, temp_strain2c_pos = 3 };

static const eObrd_cantype_t s_eoas_temperature_supportedboards_types[] = { eobrd_cantype_mtb4, eobrd_cantype_strain2, eobrd_cantype_mtb4c, eobrd_cantype_strain2c };

extern uint8_t eoas_temperature_supportedboards_numberof(void)
{
    return sizeof(s_eoas_temperature_supportedboards_types);
}

extern eObrd_cantype_t eoas_temperature_supportedboards_gettype(uint8_t pos)
{
    if(pos >= eoas_temperature_supportedboards_numberof())
    {
        return eobrd_cantype_none;
    }
    
    return s_eoas_temperature_supportedboards_types[pos];    
}

extern eOresult_t eoas_temperature_setof_boardinfos_clear(eOas_temperature_setof_boardinfos_t *set)
{
    if(NULL == set)
    {
        return eores_NOK_generic;
    }
    
    memset(set, 0, sizeof(eOas_temperature_setof_boardinfos_t));
    uint8_t i;
    for(i=0; i<eOas_temperature_boardinfos_maxnumber; i++)
    {
        set->data[i].type = eobrd_cantype_none;
    }
        
    return eores_OK;
}


extern eOresult_t eoas_temperature_setof_boardinfos_add(eOas_temperature_setof_boardinfos_t *set, const eObrd_info_t *brdinfo)
{
    if((NULL == brdinfo) || (NULL == set))
    {
        return eores_NOK_generic;
    }    
    
    if(eobrd_cantype_mtb4 == brdinfo->type)
    {
        memcpy(&set->data[temp_mtb4_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }
    
    if(eobrd_cantype_strain2 == brdinfo->type)
    {
        memcpy(&set->data[temp_strain2_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }

    if(eobrd_cantype_mtb4c == brdinfo->type)
    {
        memcpy(&set->data[temp_mtb4c_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }

    if(eobrd_cantype_strain2c == brdinfo->type)
    {
        memcpy(&set->data[temp_strain2c_pos], brdinfo, sizeof(eObrd_info_t));
        return eores_OK;       
    }          
    
    return eores_NOK_generic;
}


extern const eObrd_info_t * eoas_temperature_setof_boardinfos_find(const eOas_temperature_setof_boardinfos_t *set, eObrd_cantype_t brdtype)
{
    if(NULL == set)
    {
        return NULL;
    }
    
    const eObrd_info_t * r = NULL;

    switch(brdtype)
    {                       
        case eobrd_cantype_mtb4:
        {
            if(eobrd_cantype_mtb4 == set->data[temp_mtb4_pos].type)
            {
                r = &set->data[temp_mtb4_pos];
            }
        } break;

        case eobrd_cantype_strain2:
        {
            if(eobrd_cantype_strain2 == set->data[temp_strain2_pos].type)
            {
                r = &set->data[temp_strain2_pos];
            }
        } break;

        case eobrd_cantype_mtb4c:
        {
            if(eobrd_cantype_mtb4c == set->data[temp_mtb4c_pos].type)
            {
                r = &set->data[temp_mtb4c_pos];
            }
        } break;

        case eobrd_cantype_strain2c:
        {
            if(eobrd_cantype_strain2c == set->data[temp_strain2c_pos].type)
            {
                r = &set->data[temp_strain2c_pos];
            }
        } break;
        
        default: 
        {
            r = NULL;
        } break;
    }
    
    return r;
}


// 

static const eObrd_cantype_t s_eoas_ft_supportedboards_types[] = { eobrd_cantype_strain, eobrd_cantype_strain2, eobrd_cantype_strain2c };
static const eObrd_cantype_t s_eoas_bms_supportedboards_types[] = { eobrd_cantype_bms };


extern uint8_t eoas_battery_supportedboards_numberof(void)
{
    return sizeof(s_eoas_bms_supportedboards_types)/sizeof(eObrd_cantype_t);
}

extern uint8_t eoas_ft_supportedboards_numberof(void)
{
    return sizeof(s_eoas_ft_supportedboards_types)/sizeof(eObrd_cantype_t);
}

extern eObrd_cantype_t eoas_battery_supportedboards_gettype(uint8_t pos)
{
    if(pos >= eoas_battery_supportedboards_numberof())
    {
        return eobrd_cantype_none;
    }
    
    return s_eoas_bms_supportedboards_types[pos];    
}

extern eObrd_cantype_t eoas_ft_supportedboards_gettype(uint8_t pos)
{
    if(pos >= eoas_ft_supportedboards_numberof())
    {
        return eobrd_cantype_none;
    }
    
    return s_eoas_ft_supportedboards_types[pos];    
}

extern eObool_t eoas_ft_isboardvalid(eObrd_cantype_t boardtype)
{
    for(uint8_t n=0; n<eoas_ft_supportedboards_numberof(); n++)
    {
        if(boardtype == s_eoas_ft_supportedboards_types[n])
        {
            return eobool_true;
        }
    }
    
    return eobool_false;    
}

extern eObool_t eoas_battery_isboardvalid(eObrd_cantype_t boardtype)
{
    for(uint8_t n=0; n<eoas_battery_supportedboards_numberof(); n++)
    {
        if(boardtype == s_eoas_bms_supportedboards_types[n])
        {
            return eobool_true;
        }
    }
    
    return eobool_false;    
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------



