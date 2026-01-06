#ifndef OLED_SH1106_H
#define OLED_SH1106_H

#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0, 22, 21, U8X8_PIN_NONE
);

class ProductionDashboard {
public:
  void init() {
    u8g2.begin();
    u8g2.setI2CAddress(0x3C * 2);  // From your scanner
  }
  
  void showSplash() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_pressstart2p_8f);
    u8g2.drawStr(10, 25, "FISH");
    u8g2.drawStr(20, 55, "FEEDER");
    u8g2.sendBuffer();
    delay(2000);
  }
  
  void updateDashboard(SystemState* state) {
    u8g2.clearBuffer();
    
    // Header
    u8g2.setFont(u8g2_font_7x14B_tf);
    u8g2.drawStr(0, 12, "SMART FEEDER v2.0");
    
    // Big time
    u8g2.setFont(u8g2_font_fub42_tn);
    DateTime now = RTC.now();
    char timebuf[9];
    sprintf(timebuf, "%02d:%02d", now.hour(), now.minute());
    u8g2.drawStr(5, 52, timebuf);
    
    // Status bars
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 62);
    u8g2.printf("T:%.1f pH:%.1f Q:%d", 
                state->temp, state->ph, state->water_quality_index);
    
    u8g2.sendBuffer();
  }
};

ProductionDashboard OLED;

#endif
