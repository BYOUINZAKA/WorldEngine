#ifndef __AREA_OPTIONS_H_
#define __AREA_OPTIONS_H_

#define GET_VALID_VALUE(type, val) (val >= (decltype(val))(Options::Max##type))        \
                                       ? ((decltype(val))(Options::Max##type))         \
                                       : ((val <= (decltype(val))(Options::Min##type)) \
                                              ? ((decltype(val))(Options::Min##type))  \
                                              : val)

enum class Options
{
    MaxAltitude = 1000,
    MinAltitude = -100
};

#endif