#ifndef CIEDE2000_H
#define CIEDE2000_H

// custom types
typedef struct
{
    double l;
    double a;
    double b;
} LAB;

// constants
#define PI 3.14159265358979323846

// functions
double deg2Rad(double deg);
double rad2Deg(double rad);
double CIEDE2000(LAB lab1, LAB lab2);
void deltaE_matrix(const double *matrix_in, int n, const double *pixel, double *matrix_out);
int deltaE_min(const double *matrix_in, int n, const double *pixel);

#endif
