#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void statistic(int * arr, int arrSize, float * avr, int * max, int * min){
    float sum =0;
    int fi = 0;

 //////////////////////////과제/////////////////////////////
    while(fi < arrSize){ //avg구하기 시작
        sum += arr[fi];
        fi++;
    }
    *avr = sum / arrSize;  //avg구함

     
    //맥스구하기 시작
    *min = arr[0];
    *max = arr[0];
    for(fi = 1; fi < arrSize; fi++){
        if (*max < arr[fi]){
            *max = arr[fi];  //비교해서 arr [fi]값이 *max값보다 크면 *max값 갱신
        }
        if (*min > arr [fi]){
            *min = arr [fi]; //비교해서 arr [fi]값이 *min값보다 작으면 *min값 갱신
        }

    }

    //////////////////////////과제//////////////////////////

}



int main (int argc, void * argv[]){
    int n = 0;
    int *ptr = NULL;
    float avr = 0;
    int max = 0, min = 0 , fi;

    srand((unsigned)time(NULL));

    if(argc < 2){
        printf("CHK cmd Input!!!\n");
        return -1;
    }

    n = atoi(argv[1]);

    if (n<1){

        printf("CHK cmd input!!!!\n");

        return -1;

    }

    printf("N = %d\n",n);

//////////////////////과제//////////////////////////

    ptr = (int *)calloc( n ,sizeof(int));

    for (fi=0; fi<n; fi++){
        ptr[fi] = rand() % 101;
        printf("ptr[%d] = %d\n",fi ,ptr[fi]);
    }
    
    statistic(ptr, n, &avr, &max, &min);

//////////////////////과제//////////////////////////

    printf("average : %f\n",avr);
    printf("max : %d\n",max);
    printf("min : %d\n",min);
    free(ptr);
    return 0;

   

} 