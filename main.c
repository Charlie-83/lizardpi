#include <epaper.h>
#include <stdlib.h>

int main() {
  if (DEV_Module_Init() != 0) {
    return -1;
  }
  EPD_1IN54B_V2_Init();
  EPD_1IN54B_V2_Clear();
  DEV_Delay_ms(200);

  UBYTE *BlackImage, *RedImage;
  UWORD Imagesize =
      ((EPD_1IN54B_V2_WIDTH % 8 == 0) ? (EPD_1IN54B_V2_WIDTH / 8)
                                      : (EPD_1IN54B_V2_WIDTH / 8 + 1)) *
      EPD_1IN54B_V2_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    return -1;
  }
  if ((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for red memory...\r\n");
    return -1;
  }
  Paint_NewImage(BlackImage, EPD_1IN54B_V2_WIDTH, EPD_1IN54B_V2_HEIGHT, 90,
                 WHITE);
  Paint_NewImage(RedImage, EPD_1IN54B_V2_WIDTH, EPD_1IN54B_V2_HEIGHT, 90,
                 WHITE);

  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_DrawString_EN(5, 70, "hello world", &Font16, WHITE, BLACK);

  Paint_SelectImage(RedImage);
  Paint_Clear(WHITE);
  Paint_DrawString_EN(5, 90, "hello world", &Font20, BLACK, WHITE);

  EPD_1IN54B_V2_Display(BlackImage, RedImage);
  DEV_Delay_ms(2000);

  EPD_1IN54B_V2_Sleep();
  free(BlackImage);
  free(RedImage);
  BlackImage = NULL;
  RedImage = NULL;
  DEV_Delay_ms(2000);
  printf("close 5V, Module enters 0 power consumption ...\r\n");
  DEV_Module_Exit();
  return 0;
}
