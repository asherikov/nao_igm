/** 
 * @file
 * @author Alexander Sherikov
 * @date 02.12.2011 15:34:06 MSK
 */



/****************************************
 * INCLUDES 
 ****************************************/
#include <stdio.h>
#include <Eigen/Core>
#include <Eigen/Cholesky>



/****************************************
 * FUNCTIONS 
 ****************************************/

/** \brief Print a [m by n] matrix on the screne

    \param[in] m Number of rows
    \param[in] n Number of columns
    \param[in] A Matrix of dimension [m by n]
    \param[in] description A short description of the array (it is printed above the array)

    \note This is the same function as void Matrix_print(unsigned int m, unsigned int n, double * A, const char * description)
    but I changed the name, because if I link to mbs_simulator, I get duplicate symbols

    \return void
*/
void MatrixPrint(unsigned int m, unsigned int n, double * A, const char * description)
{
    unsigned int  i, j;

    printf(" %s", description);
    for (i=0; i<m; i++)
    {
        printf("\n");
        for (j=0; j<n; j++)
            printf("% f ", A[ j*m + i ]);
    }
    printf("\n");
}
