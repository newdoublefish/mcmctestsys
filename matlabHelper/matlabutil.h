/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    matlabUtil.h                                                     */
/*                                                                           */
/* PURPOSE: This file contains the prototypes of the utility functions       */
/*          that are defined in matlabUtil.c and are called from the         */
/*          Automation examples with MATLAB.                                 */
/*                                                                           */
/* NOTES:                                                                    */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include <cviauto.h>

/*---------------------------------------------------------------------------*/
/* User-defined Error Codes                                                  */
/*---------------------------------------------------------------------------*/
#define SUCCESS                  0
#define ERROR_WRONG_HANDLE      -1
#define ERROR_MATLAB_EXECUTE    -2
#define ERROR_FILE_PATH         -3
#define ERROR_EMPTY_STRING      -4
#define ERROR_INVALID_MATRIX    -5
#define ERROR_ARRAY_CONVERSION  -6
#define ERROR_MATRIX_TRANSFER   -7
#define ERROR_ARRAY_QUERY       -8
#define ERROR_NULL_POINTER      -9
#define ERROR_MATLAB_MINMAX     -10

/*---------------------------------------------------------------------------*/
/* Function Proto-types                                                      */
/*---------------------------------------------------------------------------*/
int     MinMaxMatlab(CAObjHandle hMatlab, int minmaxFlag);   
int     RunMatlabCommand(CAObjHandle matlabHdl, char *command);
int     RunMatlabScript(CAObjHandle matlabHdl, char *mFilePath);
int     SendString(CAObjHandle matlabHdl, char *matStringName, char *CVIString);
int     GetString(CAObjHandle matlabHdl, char *matStringName, char **cString);
int     SendMatrix(CAObjHandle matlabHdl, char *matlabName, double *matrixReal, double *matrixImag, unsigned dim1, unsigned dim2);
int     GetMatrix(CAObjHandle matlabHdl, char *matlabName, double **matrixReal, double **matrixImag, unsigned *dim1, unsigned *dim2);

