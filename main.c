#include <epaper.h>

int main() {
    if(DEV_Module_Init()!=0){
        return -1;
    }
    printf("Charlie\n");
    EPD_1IN54B_V2_Init();
    EPD_1IN54B_V2_Clear();
    DEV_Delay_ms(200);
    Paint_Clear(0xFF);
}
