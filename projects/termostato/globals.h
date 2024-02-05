#define BUZZER_PIN 21
#define SD_CS_PIN 17

#define IDLE_TIME 30
#define IDLE_BACKLIGHT_BRIGHTNESS 0.4
#define SCHEDULE_FILENAME "/schedule.dat"
//#define FS_NO_GLOBALS

int idle_time = IDLE_TIME;
bool sdcard_alert = false;
bool alert = false;
