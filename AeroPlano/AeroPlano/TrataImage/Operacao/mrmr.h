#ifndef MRMR_H
#define MRMR_H

#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;
#define NR_END 1
#define FREE_ARG char*
#define M 7
#define NSTACK 50

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>



enum FeaSelectionMethod
{ MID, MIQ };

class MRMR
{
public:
    MRMR();


private:
//    void sort(unsigned long n, float arr[], float brr[]);
//    int *ivector(long nl, long nh);
//    void free_ivector(int *v,long nl, long nh);
//    long *mRMR_selection (DataTable * myData, long nfea, FeaSelectionMethod mymethod);
//    double calMutualInfo (DataTable * myData, long v1, long v2);
//    double compute_mutualinfo (double *pab, long pabhei, long pabwid);

};

#endif // MRMR_H
