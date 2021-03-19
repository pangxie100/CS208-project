#include <stdio.h>
#include "client_func_yml.h"
#include <time.h>
#include <sys/time.h>

#define TIME_COUNT 1 

int main(int argc,char **argv)
{
#ifdef TIME_COUNT
    double t0, t1;
    double t_total = 0.;
    struct timeval time;
    gettimeofday(&time, NULL); // from sys/time.h
    t0 = time.tv_sec + 1e-6 * time.tv_usec; // micro second, 1s = 1 * 10^6 micro seconds
#endif
    for (int i = 0; i < 1927; i++){
        client_yml("128.110.96.108");
    }
#ifdef TIME_COUNT
    gettimeofday(&time, NULL); // from sys/time.h
    t1 = time.tv_sec + 1e-6 * time.tv_usec;
    t_total += (t1 - t0);
    printf("t_total: %f s\n", t_total);
#endif
    return 0;
}