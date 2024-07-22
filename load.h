#pragma once

#include "level.h"

class Load : public Level
{
    Q_OBJECT
public:
    Load(const QString& path);
};
