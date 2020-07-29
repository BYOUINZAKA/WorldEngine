#ifndef IRUNABLUE_H
#define IRUNABLUE_H

#include "forward/creature.h"

class Creature::IRunablue {
public:
    IRunablue();
    virtual void run() = 0;
};

#endif  // IRUNABLUE_H
