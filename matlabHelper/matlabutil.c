
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    matlabUtil.c                                                     */
/*                                                                           */
/* PURPOSE: This file contains utility functions that are called from        */
/*          the Automation examples for MATLAB.                              */
/*                                                                           */
/* NOTES:   The matlabUtil.h file contains the prototypes of these functions */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include "matlabUtil.h"
#include "matlabsrvr.h"
#include <utility.h>
#include <userint.h>
#include <formatio.h>

/*---------------------------------------------------------------------------*/
/* This function minimizes/maximizes the MATLAB window.                      */
/* hMatlab is the handle to the MATLAB Application Object.                   */
/* Pass 0 in minmaxFlag to minimize, and 1 to maximize the window.           */
/*---------------------------------------------------------------------------*/
int MinMaxMatlab(CAObjHandle hMatlab, int minmaxFlag)
{
    HRESULT     stat    =   0;
    
    if (minmaxFlag == 0)
    {
        /* Minimize the MATLAB window */
        stat = MLApp_DIMLAppMinimizeCommandWindow (hMatlab, NULL);
        if (stat < 0) return ERROR_MATLAB_MINMAX;             
    }
    else
    {
        /* Maximize the MATLAB window */
        stat = MLApp_DIMLAppMaximizeCommandWindow (hMatlab, NULL);
        if (stat < 0) return ERROR_MATLAB_MINMAX;  
    }
    return SUCCESS;

}

/*---------------------------------------------------------------------------*/
/* This function executes a MATLAB command.                                  */
/* hMatlab is the handle to the MATLAB Application Object.                   */
/* command is the MATLAB command string to be executed.                      */              
/*---------------------------------------------------------------------------*/
int RunMatlabCommand(CAObjHandle hMatlab, char *command)
{
    char        *matlabMesg =   NULL;
    HRESULT     stat        =   0;
    
    stat = MLApp_DIMLAppExecute (hMatlab, NULL, command, &matlabMesg);
    if (stat < 0)
    {
        if (matlabMesg != NULL) CA_FreeMemory(matlabMesg);
        return ERROR_MATLAB_EXECUTE;
    }
    if (matlabMesg != NULL) CA_FreeMemory(matlabMesg);
    return SUCCESS;
}

/*---------------------------------------------------------------------------*/
/* This function executes a MATLAB .m file.                                  */
/* hMatlab is the handle to the MATLAB Application Object.                   */
/* mFilePath is the full path to the .m file to run.                         */
/*---------------------------------------------------------------------------*/
int RunMatlabScript(CAObjHandle hMatlab, char *mFilePath)
{
    int     index                           =   -1;
    char    fName[MAX_FILENAME_LEN];
    char    dirName[MAX_DIRNAME_LEN];
    char    driveName[MAX_DRIVENAME_LEN];
    char    command[300];
    int     result                          =   0;

    /* Split the path name into the drive, directory, and file names */
    SplitPath (mFilePath, driveName, dirName, fName);
    if (driveName && dirName && fName)
    {
        /* Add the m-file directory to MATLAB's search path*/
        Fmt(command,"%s<path(path,'%s%s');",driveName,dirName); 
        RunMatlabCommand(hMatlab, command);   
        
        /* Copy the .m file name into the command string */
        Fmt(command,"%s<%s",fName);
        /* Remove the ".m" extension from the command */
        index=strlen(command)-2;
        command[index]='\0';
        
        /* Execute the .m file */
        result = RunMatlabCommand(hMatlab, command);
        return result;
    }
    else
    {
        return ERROR_FILE_PATH;
    }
                
}

/*---------------------------------------------------------------------------*/
/* This function sends a string to MATLAB       .                            */
/* hMatlab is the handle to the MATLAB Application Object.                   */
/* matStringName is the name of the MATLAB variable to store the  string in. */
/* CVIstring is the actual string to be transferred to MATLAB                */
/* MATLAB does not support BSTRs, and so we send the string as a double [].  */
/*---------------------------------------------------------------------------*/
int SendString(CAObjHandle hMatlab, char *matStringName, char *CVIString)
{
    double          *buffer_r         =   NULL;
    double          *buffer_i         =   NULL;
    char            command[300];
    int             index           =   0;
    int             i               =   0;
    int             result          =   0;
    
    if(CVIString == NULL)
    {
        return ERROR_EMPTY_STRING;
    }
    index=strlen(CVIString);
    /* Convert the string into a double array */
    buffer_r=(double *)malloc(index*sizeof(double));
    buffer_i=(double *)malloc(index*sizeof(double));
    for(i=0;i<index;++i)
    {
        buffer_r[i]=(double)CVIString[i];
        buffer_i[i]=0.0;
    }
        
    
    /* Send the double array to MATLAB */
    result = SendMatrix(hMatlab, matStringName, buffer_r, buffer_i, index, 1);
    free(buffer_r);
    free(buffer_i);
    if (result != SUCCESS) return result;
    
    /* Convert the double array into a MATLAB string */ 
    Fmt(command,"%s<%s=transpose(%s)",matStringName,matStringName);
    result = RunMatlabCommand(hMatlab, command);
    if (result != SUCCESS) return result;
    Fmt(command,"%s<%s=char(%s)",matStringName,matStringName);
    result = RunMatlabCommand(hMatlab, command);
    if (result != SUCCESS) return result;
    
    return SUCCESS;
}

/*---------------------------------------------------------------------------*/
/* This function gets a string from MATLAB      .                            */
/* hMatlab is the handle to the MATLAB Application Object.                   */
/* matStringName is the name of the MATLAB variable to get the  string from. */
/* The transferred string will be stored in *cString.                        */
/* MATLAB does not support BSTRs, and so we get the string as a double [].   */
/*---------------------------------------------------------------------------*/
int GetString(CAObjHandle hMatlab, char *matStringName, char **cString)
{
    double          *buffer     =   NULL;
    double          *dummy      =   NULL;
    unsigned        dim1        =   0;
    unsigned        dim2        =   0;
    int             i           =   0;
    char            command[300];
    int             result      =   0;
    
    /* convert the MATLAB string into a double matrix */
    Fmt(command,"%s<CVIString=transpose(double(%s));",matStringName);
    result = RunMatlabCommand(hMatlab,command);
    if (result != SUCCESS) return result;

    /* get the matrix into CVI and convert into string */
    result = GetMatrix(hMatlab, "CVIString", &buffer, &dummy, &dim1, &dim2);
    if (result != SUCCESS) return result;
    if (buffer == NULL)
    {
        return ERROR_NULL_POINTER;
    }
    
    *cString=(char *)malloc((dim1+1)*sizeof(char));
    for(i=0;i<dim1;++i)
        (*cString)[i]=(char)buffer[i];
    (*cString)[dim1]='\0';
    if (buffer != NULL) CA_FreeMemory(buffer);  
    if (dummy != NULL) CA_FreeMemory(dummy);
    
    /* clear the temporary MATLAB variable created */
    result = RunMatlabCommand(hMatlab, "clear CVIString;");
    if (result != SUCCESS) return result;
    return SUCCESS; 
}

/*---------------------------------------------------------------------------*/
/* This function sends a matrix to MATLAB.                                   */
/* hMatlab is the handle to the MATLAB Application Object.                   */
/* matlabName is the MATLAB variable that will contain the matrix.           */
/* matrixReal contains the real part of the matrix.                          */
/* matrixImag contains the imaginary part of the matrix.                     */
/* dim1 is the number of rows in the matrix.                                 */
/* dim2 is the number of columns in the matrix.                              */
/*---------------------------------------------------------------------------*/
int SendMatrix(CAObjHandle hMatlab, char *matlabName, double *matrixReal, 
                    double *matrixImag, unsigned dim1, unsigned dim2)
{
    LPSAFEARRAY         saReal  =   NULL;
    LPSAFEARRAY         saImag  =   NULL;
    HRESULT             stat;
    
    /* Check that valid pointers have been passed for the matrices */
    if((!matrixReal) && (!matrixImag))
    {
        return ERROR_INVALID_MATRIX;
    }
    
    /* Convert the real and imaginary matrices into SAFEARRAYs */
    if(matrixReal)
    {
        stat = CA_Array2DToSafeArray (matrixReal, CAVT_DOUBLE, dim1, 
                dim2, &saReal);
        if (stat < 0)
        {
            return ERROR_ARRAY_CONVERSION;
        }
    }
    if(matrixImag)
    {
        stat = CA_Array2DToSafeArray (matrixImag, CAVT_DOUBLE, dim1, 
                dim2, &saImag);
        if (stat < 0)
        {
            return ERROR_ARRAY_CONVERSION;
        }
    }
    
    /* Send the matrices into MATLAB */
    stat = MLApp_DIMLAppPutFullMatrix (hMatlab, NULL, matlabName, "base",
                                (SAFEARRAY *)saReal, (SAFEARRAY *)saImag);
    if (stat < 0)
    {
        return ERROR_MATRIX_TRANSFER;
    }
    if (saReal != NULL) CA_SafeArrayDestroy(saReal);    
    if (saImag != NULL) CA_SafeArrayDestroy(saImag);
    return SUCCESS;
}

/*---------------------------------------------------------------------------*/
/* This function gets a matrix from MATLAB.                                  */
/* hMatlab is the handle to the MATLAB Application Object.                   */
/* matlabName is the MATLAB variable that contains the matrix to get.        */
/* matrixReal will get the real part of the matrix.                          */
/* matrixImag will get the imaginary part of the matrix.                     */
/* dim1 will get the number of rows in the matrices.                         */
/* dim2 will get the number of columns in the matrices.                      */
/*---------------------------------------------------------------------------*/
int GetMatrix(CAObjHandle hMatlab, char *matlabName, double **matrixReal, 
                    double **matrixImag, unsigned *dim1, unsigned *dim2)
{
    SAFEARRAY       *saReal     =   NULL;
    SAFEARRAY       *saImag     =   NULL;
    HRESULT         stat;

    /* Get the matrix's real and imaginary parts from MATLAB */ 
    stat = MLApp_DIMLAppGetFullMatrix (hMatlab, NULL, matlabName,
                                       "base", &saReal, &saImag);
    if (stat < 0)
    {
        return ERROR_MATRIX_TRANSFER; 
    }
    
    /* Convert the SAFEARRAYs to C datatypes */
    if ((saReal == NULL) && (saImag == NULL))
    {
        return ERROR_NULL_POINTER;
    }
    
    if(saReal)
    {
        stat = CA_SafeArrayGet2DSize (saReal, dim1, dim2);
        if (stat < 0)
        {
            return ERROR_ARRAY_QUERY;
        }
        stat=CA_SafeArrayTo2DArray ((LPSAFEARRAY *)&saReal, CAVT_DOUBLE,
                               matrixReal, dim1, dim2);
        if (stat < 0)
        {
            return ERROR_ARRAY_CONVERSION;
        }
        
    }
    if(saImag)
    {
        stat = CA_SafeArrayGet2DSize (saImag, dim1, dim2);
        if (stat < 0)
        {
            return ERROR_ARRAY_QUERY;
        }
        stat=CA_SafeArrayTo2DArray ((LPSAFEARRAY *)&saImag, CAVT_DOUBLE,
                               matrixImag, dim1, dim2);
        if (stat < 0)
        {
            return ERROR_ARRAY_CONVERSION;
        }
    }
    
    return SUCCESS;
    
}
