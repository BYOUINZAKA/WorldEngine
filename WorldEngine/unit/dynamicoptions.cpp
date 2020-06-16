#include "dynamicoptions.h"

DynamicOptions::DynamicOptions(QObject *parent) : QObject(parent)
{
}

int32_t DynamicOptions::Length = 1000;
int32_t DynamicOptions::Width = 1000;
