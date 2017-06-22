#include "Safety.h"
#include <avr/wdt.h>

namespace safety {
    
    void setup() {
        wdt_enable(WDTO_4S);
    }
    
    void tick() {
        wdt_reset();
    }
    
    void shutdown() {
        wdt_disable();
    }
    
}
