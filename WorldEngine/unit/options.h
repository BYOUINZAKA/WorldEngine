#ifndef __UNIT_OPTIONS_H
#define __UNIT_OPTIONS_H

#define GET_VALID_VALUE(type, val)                         \
    (((val) >= (decltype(val))(Options::Max##type))        \
         ? ((decltype(val))(Options::Max##type))           \
         : (((val) <= (decltype(val))(Options::Min##type)) \
                ? ((decltype(val))(Options::Min##type))    \
                : (val)))

enum class Options
{
    MaxAltitude = 1000,
    MinAltitude = -300,
    MaxTemp = 45,
    MinTemp = -30,
    MaxDamp = 100,
    MinDamp = 0,
    Horizontal = 0,
    BaseTemp = 20,
    WaterTemp = 10
};

#endif
