#include "SECDRuntime.h"

void SECDRuntime::reset() {
    stack = {};
    code = {};
    env = {};
}