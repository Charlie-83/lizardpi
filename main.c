#include <epaper.h>

int main() {
    if(DEV_Module_Init()!=0){
        return -1;
    }
    EPD_1IN54B_V2_Init();
    DEV_Delay_ms(200);
    Paint_Clear(0xFF);
    const char* str = "Hello World!";
    Paint_DrawString_EN(0, 0, str, &Font12, BLACK, WHITE);
}
