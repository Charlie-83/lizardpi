#include <epaper.h>
#include <stdlib.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define SHT40_ADDRESS 0x44

void read_sensor(int sensor, int *t_deg, int *th_pRH) {
  const uint8_t tx_bytes[1] = {0xfd};
  wiringPiI2CRawWrite(sensor, tx_bytes, 1);
  delay(10);
  uint8_t rx_bytes[6];
  wiringPiI2CRawRead(sensor, rx_bytes, 6);

  uint16_t t_ticks = rx_bytes[0] * 256 + rx_bytes[1];
  *t_deg = -45 + 175 * t_ticks / 65535;

  uint16_t rh_ticks = rx_bytes[3] * 256 + rx_bytes[4];
  *th_pRH = -6 + 125 * rh_ticks / 65535;
  if (*th_pRH > 100)
    *th_pRH = 100;
  else if (*th_pRH < 0)
    *th_pRH = 0;
}

int main() {
  int sensor_0 = wiringPiI2CSetupInterface("/dev/i2c-1", SHT40_ADDRESS);
  if (sensor_0 == -1) {
    printf("Failed to setup SHT40\n");
    return -1;
  }
  int sensor_1 = wiringPiI2CSetupInterface("/dev/i2c-3", SHT40_ADDRESS);
  if (sensor_1 == -1) {
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

  time_t rawtime;
  struct tm *time_data;
  while (1) {
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    int t_deg;
    int th_pRH;

    read_sensor(sensor_0, &t_deg, &th_pRH);
    Paint_DrawNum(50, 50, t_deg, &Font24, BLACK, WHITE);
    Paint_DrawNum(100, 50, th_pRH, &Font24, BLACK, WHITE);

    read_sensor(sensor_1, &t_deg, &th_pRH);
    Paint_DrawNum(50, 100, t_deg, &Font24, BLACK, WHITE);
    Paint_DrawNum(100, 100, th_pRH, &Font24, BLACK, WHITE);

    time(&rawtime);
    time_data = localtime(&rawtime);
    PAINT_TIME paint_time = {
        .Sec = time_data->tm_sec,
        .Min = time_data->tm_min,
        .Hour = time_data->tm_hour,
        .Day = time_data->tm_mday,
        .Month = time_data->tm_mon,
        .Year = time_data->tm_year,
    };
    Paint_DrawTime(5, 5, &paint_time, &Font24, BLACK, WHITE);

    EPD_1IN54B_V2_Display(BlackImage, RedImage);
    delay(180000);
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
