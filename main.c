#include <epaper.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define SHT40_ADDRESS 0x44

int main() {
  int fd = wiringPiI2CSetup(SHT40_ADDRESS);
  if (fd == -1) {
    printf("Failed to setup SHT40\n");
    return -1;
  }
  if (DEV_Module_Init() != 0) {
    return -1;
  }
  EPD_1IN54B_V2_Init();
  EPD_1IN54B_V2_Clear();
  delay(200);

  UBYTE *BlackImage, *RedImage;
  UWORD Imagesize =
      ((EPD_1IN54B_V2_WIDTH % 8 == 0) ? (EPD_1IN54B_V2_WIDTH / 8)
                                      : (EPD_1IN54B_V2_WIDTH / 8 + 1)) *
      EPD_1IN54B_V2_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\n");
    return -1;
  }
  if ((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for red memory...\n");
    return -1;
  }
  Paint_NewImage(BlackImage, EPD_1IN54B_V2_WIDTH, EPD_1IN54B_V2_HEIGHT, 90,
                 WHITE);
  Paint_NewImage(RedImage, EPD_1IN54B_V2_WIDTH, EPD_1IN54B_V2_HEIGHT, 90,
                 WHITE);
  Paint_SelectImage(RedImage);
  Paint_Clear(WHITE);

  while (1) {
    const uint8_t tx_bytes[1] = {0xfd};
    wiringPiI2CRawWrite(fd, tx_bytes, 1);
    delay(10);
    uint8_t rx_bytes[6];
    wiringPiI2CRawRead(fd, rx_bytes, 6);
    uint16_t t_ticks = rx_bytes[0] * 256 + rx_bytes[1];
    int t_deg = -45 + 175 * t_ticks / 65535;

    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawNum(5, 70, t_deg, &Font16, WHITE, BLACK);
    EPD_1IN54B_V2_Display(BlackImage, RedImage);
    delay(2000);
  }

  EPD_1IN54B_V2_Sleep();
  free(BlackImage);
  free(RedImage);
  BlackImage = NULL;
  RedImage = NULL;
  delay(2000);
  printf("close 5V, Module enters 0 power consumption ...\n");
  DEV_Module_Exit();
  return 0;
}
