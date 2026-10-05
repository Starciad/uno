#include <stdlib.h>
#include "random.h"

int random_int(const int min, const int max)
{
    return min + rand() % (max - min + 1);
}
