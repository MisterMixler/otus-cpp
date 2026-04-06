#pragma once

#include "observer.h"

class ConsoleLogger : public IObserver {
public:
    void onBlock(const Block& block) override;
};
