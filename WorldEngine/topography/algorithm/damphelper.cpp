#include "damphelper.h"

DampHelper::DampHelper(Topography *model)
    : model{model},
      flags(model->getRealSize(), 0)
{
}
