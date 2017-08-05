//
// Created by Lior on 06/08/2017.
//

#include "move.h"

Location Move::getTo() const
{
    return to;
}

void Move::setTo(const Location &value)
{
    to = value;
}