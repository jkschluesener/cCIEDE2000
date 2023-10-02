/*
The functions herein were taken with minor modifications from:
https://github.com/gfiumara/CIEDE2000/
MIT License

Commit Hash af1de42515f3916c16e75980a4635962489af56a from 2017-Oct-26, accessed 26. September 2023
*/

#include <math.h>
#include <stdio.h>
#include <float.h>
#include "ciede2000.h"

double deg2Rad(double deg)
{
    return (deg * (PI / 180.0));
}

double rad2Deg(double rad)
{
    return ((180.0 / PI) * rad);
}

double CIEDE2000(LAB lab1, LAB lab2)
{
    /*
     * "For these and all other numerical/graphical delta E00 values
     * reported in this article, we set the parametric weighting factors
     * to unity(i.e., k_L = k_C = k_H = 1.0)." (Page 27).
     */
    const double k_L = 1.0, k_C = 1.0, k_H = 1.0;
    const double deg360InRad = deg2Rad(360.0);
    const double deg180InRad = deg2Rad(180.0);
    const double pow25To7 = 6103515625.0; /* pow(25, 7) */

    /*
     * Step 1
     */
    /* Equation 2 */
    double C1 = sqrt((lab1.a * lab1.a) + (lab1.b * lab1.b));
    double C2 = sqrt((lab2.a * lab2.a) + (lab2.b * lab2.b));
    /* Equation 3 */
    double barC = (C1 + C2) / 2.0;
    /* Equation 4 */
    double G = 0.5 * (1 - sqrt(pow(barC, 7) / (pow(barC, 7) + pow25To7)));
    /* Equation 5 */
    double a1Prime = (1.0 + G) * lab1.a;
    double a2Prime = (1.0 + G) * lab2.a;
    /* Equation 6 */
    double CPrime1 = sqrt((a1Prime * a1Prime) + (lab1.b * lab1.b));
    double CPrime2 = sqrt((a2Prime * a2Prime) + (lab2.b * lab2.b));
    /* Equation 7 */
    double hPrime1;
    if (lab1.b == 0 && a1Prime == 0)
        hPrime1 = 0.0;
    else
    {
        hPrime1 = atan2(lab1.b, a1Prime);
        /*
         * This must be converted to a hue angle in degrees between 0
         * and 360 by addition of 2 to negative hue angles.
         */
        if (hPrime1 < 0)
            hPrime1 += deg360InRad;
    }
    double hPrime2;
    if (lab2.b == 0 && a2Prime == 0)
        hPrime2 = 0.0;
    else
    {
        hPrime2 = atan2(lab2.b, a2Prime);
        /*
         * This must be converted to a hue angle in degrees between 0
         * and 360 by addition of 2 to negative hue angles.
         */
        if (hPrime2 < 0)
            hPrime2 += deg360InRad;
    }

    /*
     * Step 2
     */
    /* Equation 8 */
    double deltaLPrime = lab2.l - lab1.l;
    /* Equation 9 */
    double deltaCPrime = CPrime2 - CPrime1;
    /* Equation 10 */
    double deltahPrime;
    double CPrimeProduct = CPrime1 * CPrime2;
    if (CPrimeProduct == 0)
        deltahPrime = 0;
    else
    {
        /* Avoid the fabs() call */
        deltahPrime = hPrime2 - hPrime1;
        if (deltahPrime < -deg180InRad)
            deltahPrime += deg360InRad;
        else if (deltahPrime > deg180InRad)
            deltahPrime -= deg360InRad;
    }
    /* Equation 11 */
    double deltaHPrime = 2.0 * sqrt(CPrimeProduct) * sin(deltahPrime / 2.0);

    /*
     * Step 3
     */
    /* Equation 12 */
    double barLPrime = (lab1.l + lab2.l) / 2.0;
    /* Equation 13 */
    double barCPrime = (CPrime1 + CPrime2) / 2.0;
    /* Equation 14 */
    double barhPrime, hPrimeSum = hPrime1 + hPrime2;
    if (CPrime1 * CPrime2 == 0)
    {
        barhPrime = hPrimeSum;
    }
    else
    {
        if (fabs(hPrime1 - hPrime2) <= deg180InRad)
            barhPrime = hPrimeSum / 2.0;
        else
        {
            if (hPrimeSum < deg360InRad)
                barhPrime = (hPrimeSum + deg360InRad) / 2.0;
            else
                barhPrime = (hPrimeSum - deg360InRad) / 2.0;
        }
    }
    /* Equation 15 */
    double T = 1.0 - (0.17 * cos(barhPrime - deg2Rad(30.0))) +
               (0.24 * cos(2.0 * barhPrime)) +
               (0.32 * cos((3.0 * barhPrime) + deg2Rad(6.0))) -
               (0.20 * cos((4.0 * barhPrime) - deg2Rad(63.0)));
    /* Equation 16 */
    double deltaTheta = deg2Rad(30.0) *
                        exp(-pow((barhPrime - deg2Rad(275.0)) / deg2Rad(25.0), 2.0));
    /* Equation 17 */
    double R_C = 2.0 * sqrt(pow(barCPrime, 7.0) / (pow(barCPrime, 7.0) + pow25To7));
    /* Equation 18 */
    double S_L = 1 + ((0.015 * pow(barLPrime - 50.0, 2.0)) / sqrt(20 + pow(barLPrime - 50.0, 2.0)));
    /* Equation 19 */
    double S_C = 1 + (0.045 * barCPrime);
    /* Equation 20 */
    double S_H = 1 + (0.015 * barCPrime * T);
    /* Equation 21 */
    double R_T = (-sin(2.0 * deltaTheta)) * R_C;

    /* Equation 22 */
    double deltaE = sqrt(
        pow(deltaLPrime / (k_L * S_L), 2.0) +
        pow(deltaCPrime / (k_C * S_C), 2.0) +
        pow(deltaHPrime / (k_H * S_H), 2.0) +
        (R_T * (deltaCPrime / (k_C * S_C)) * (deltaHPrime / (k_H * S_H))));

    return (deltaE);
}

void deltaE_matrix(const double *matrix_in, int n, const double *pixel, double *matrix_out)
{

    for (int row = 0; row < n; row++)
    {
        LAB lab1 = {pixel[0],
                    pixel[1],
                    pixel[2]};

        LAB lab2 = {matrix_in[row * 3],
                    matrix_in[row * 3 + 1],
                    matrix_in[row * 3 + 2]};

        matrix_out[row] = CIEDE2000(lab1, lab2);
    }
}

int deltaE_min(const double *matrix_in, int n, const double *pixel)
{

    int lowest_row = 0;
    double lowest_deltaE = DBL_MAX;
    double current_deltaE = 0;

    for (int row = 0; row < n; row++)
    {
        LAB lab1 = {pixel[0],
                    pixel[1],
                    pixel[2]};

        LAB lab2 = {matrix_in[row * 3],
                    matrix_in[row * 3 + 1],
                    matrix_in[row * 3 + 2]};

        current_deltaE = CIEDE2000(lab1, lab2);

        if (current_deltaE < lowest_deltaE)
        {
            lowest_deltaE = current_deltaE;
            lowest_row = row;
        }
    }
    return lowest_row;
}
