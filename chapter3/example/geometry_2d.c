#include <math.h>
#include "geometry.h"

cartesian_pos_2d_t convert_to_2d_cartesion_pos(
    const polar_pos_2d_t* polar_pos
) {
    cartesian_pos_2d_t result;
    result.x = polar_pos->length * cos_deg(polar_pos->angle);
    result.y = polar_pos->length * sin_deg(polar_pos->angle);
    return result;
}

polar_pos_2d_t convert_to_2d_polar_pos(
    const cartesian_pos_2d_t* cartesian_pos
) {
    polar_pos_2d_t result;
    result.length = sqrt(cartesian_pos->x * cartesian_pos->x + \
    cartesian_pos->y * cartesian_pos->y);
    result.angle = to_degree(atan(cartesian_pos->y / \
    cartesian_pos->x));
    return result;
}

