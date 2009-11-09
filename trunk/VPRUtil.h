#ifndef VPRUTIL_H

#include <builtin_types.h>

/** \brief Make a tent function in volume from points.
 * The first must contain the object coordinate system points.
 */
bool MakeTent(const size_t, const float3*, const int[3], const float3&, const float3&, float*);

#endif
