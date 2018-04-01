#include <stdio.h>
void serial_display(float *data, int n, int num);
enum upper_computer{
    CommChart ,
    SerialChart,
    Ming_you_Ke_Chuang
};
int main(void)
{
    int i = 0;
    float var[3];
    while(i < 100){
        i+=5;
        var[0] = i;
        var[1] = i+1;
        var[2] = i+2;
        serial_display(var, sizeof(var)/sizeof(var[0]), CommChart);
        serial_display(var, sizeof(var)/sizeof(var[0]), SerialChart);
    }
    return 0;
}
void serial_display(float *data, int n, int num)
{
    int i = 0;
    if(num == CommChart){
        printf("%c",'{');
        while(i < n-1){
            printf("%f",*data++);
            printf("%c",',');
            i++;
        }
        printf("%f",*data);
        printf("%c",'}');
    }else if(num == SerialChart){
        while(i < n-1){
            printf("%f",*data++);
            printf("%c",',');
            i++;
        }
        printf("%f\n",*data);
    }
    else if(num == SerialChart){
        while(i < n-1){
            printf("%f",*data++);
            printf("%c",',');
            i++;
        }
        printf("%f\n",*data);
    }
}
