#include <cvirte.h>		
#include <userint.h>
#include <stdlib.h>
#include <cviauto.h>
#include <formatio.h>
#include <utility.h>
#include "matlabsrvr.h"  
#include "matlabutil.h"

CAObjHandle OpenMatlab(void);

int CloseMatlab(CAObjHandle hMatlab);

int vibDataAnalyse(int hMatlab,double *data,int datalength,double sample,double **getData,double **getF);

