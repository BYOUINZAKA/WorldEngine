/*
 * @Author: Hata
 * @Date: 2020-06-13 16:29:44
 * @LastEditors: Hata
 * @LastEditTime: 2020-07-29 10:04:16
 * @FilePath: \WorldEngine\WorldEngine\unit\options.h
 * @Description: Something static config.
 */ 
#ifndef __UNIT_OPTIONS_H
#define __UNIT_OPTIONS_H

#define GET_VALID_VALUE(type, val)                                                                 \
    (((val) >= (decltype(val))(Options::Max##type))                                                \
         ? ((decltype(val))(Options::Max##type))                                                   \
         : (((val) <= (decltype(val))(Options::Min##type)) ? ((decltype(val))(Options::Min##type)) \
                                                           : (val)))

enum class Options {
    MaxAltitude = 800,
    MinAltitude = -100,
    MaxTemp = 45,
    MinTemp = -30,
    MaxDamp = 100,
    MinDamp = 0,
    Horizontal = 0,
    BaseTemp = 20,
    WaterTemp = 10,
    IsWater = 1
};

#endif
