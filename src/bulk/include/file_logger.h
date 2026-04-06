#pragma once

#include "observer.h"

class FileLogger : public IObserver {
public:
    void onBlock(const Block& block) override;
};
