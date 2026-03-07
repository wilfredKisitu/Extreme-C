#ifndef GEOMETRY_H
#define GEOMETRY_H

#define PI 3.14159265359

typedef struct {
    double x;
    double y;
} cartesian_pos_2d_t;

typedef struct {
    double length;
    // in degrees
    double angle;
} polar_pos_2d_t;

typedef struct {
    double x;
    double y;
    double z;   
} cartesion_pos_3d_t;

typedef struct{
    double length;
    // in degrees
    double theta;
    // in degrees
    double phi;
} polar_pos_3d_t;

double to_radian(double deg);
double to_degree(double rad);

double cos_deg(double deg);
double acos_deg(double deg);

double sin_deg(double deg);
double asin_deg(double deg);


cartesian_pos_2d_t convert_to_2d_cartesion_pos(
    const polar_pos_2d_t* polar_pos
);
polar_pos_2d_t convert_to_2d_polar_pos(
    const cartesian_pos_2d_t* cartesion_pos
);

cartesion_pos_3d_t convert_to_3d_catersian_pos(
    const polar_pos_3d_t* polar_pos
);
polar_pos_3d_t convert_to_3d_polar_pos(
    const cartesion_pos_3d_t* cartesian_pos
);

#endif