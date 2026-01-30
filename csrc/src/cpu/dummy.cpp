#include "corepy_kernels.h"

extern "C" {
    int add_one_kernel(int x) {
        return x + 1;
    }
}
