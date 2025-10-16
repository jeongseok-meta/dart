/* Simple test to verify CCD library compiles */
#include "dart/collision/hit/ccd/ccd.h"
#include "dart/collision/hit/ccd/vec3.h"

int main(void) {
    dart_ccd_t ccd;
    dart_ccd_vec3_t v1, v2;

    /* Initialize */
    DART_CCD_INIT(&ccd);

    /* Set some values */
    dartCcdVec3Set(&v1, 1.0, 2.0, 3.0);
    dartCcdVec3Set(&v2, 4.0, 5.0, 6.0);

    /* Test a function */
    dart_ccd_real_t dist = dartCcdVec3Dist2(&v1, &v2);

    return 0;
}
