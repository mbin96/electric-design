#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 100

int main(){
    FILE *ori;
    FILE *cp;
    char buffer[BUFSIZE];
    int count;

    ori = fopen("ori.png","rb");
    cp = fopen("copy.png","wb");

    if (ori == NULL || cp == NULL){
        printf("fail to open file\n");
        return -1;

    }

    while (1){
        count = fread(buffer, 1, BUFSIZE, ori);
        if(count < BUFSIZE){
            if(feof(ori) != 0){
                fwrite(buffer, 1,count, cp);
                printf("copy success!!\n");
                break;
            }else{
                printf("copt fail\n");
                break;
            }

        }else{
            fwrite(buffer, 1, BUFSIZE, cp);
        }
    }

    fclose(ori);
    fclose(cp);
    return 0;

}