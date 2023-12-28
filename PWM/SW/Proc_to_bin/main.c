#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

void main(int argc, char **argv) {
    int i = 0;
    char filename[1000];
    char output[100];
    sprintf(filename, "/proc/motor_ctrl_log");
    FILE *input_file;
    FILE *output_file = fopen("motor_ctrl_log.bin", "w");
    //FILE *f = fopen()

    long long unsigned int time;
    int state, late;
    while(1)
    {
        input_file = fopen(filename, "r");
        if(input_file == NULL)
        {
            printf("Log file closed.\n");
            break;
        }
        printf("Waiting for new input...\n");
        usleep(2500005);
        while(fscanf(input_file, "%llu %d %d", &time, &state, &late)  == 3)
        {
            printf("time = %llu, ", time/1000000); //converted to ms
            printf("state = %d, ", state);
            printf("late = %d\n", late);
            fprintf(output_file, "%llu %d %d\n", time, state, late);
            i++;
        }
        fclose(input_file); //refresh file
        
    }
    printf("STEVICA: %d", i);
    fclose(output_file);
}