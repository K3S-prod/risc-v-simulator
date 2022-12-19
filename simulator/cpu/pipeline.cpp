
#include <string>
#include "cpu.h"

namespace sim {

class Stage {
    std::string name;
public:
    virtual void run() = 0;
};

class Fetch: public Stage {
    void run() {
        return;
    }
};

}

