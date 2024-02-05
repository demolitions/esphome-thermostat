#include "SD.h"

bool sdcard_mount()
{
  bool mounted = false;
  int tries = 5;
  while(!mounted && tries > 0)
  {
    digitalWrite(SD_CS_PIN, LOW);
    delay(100);
    digitalWrite(SD_CS_PIN, HIGH);
    delay(100);
    digitalWrite(SD_CS_PIN, LOW);
    delay(100);
    ESP_LOGD("sdcard_mount", "Mounting SD Card");
    if (SD.begin(SD_CS_PIN, SPI, 200000U, "/sd", 5, false))
    {
      ESP_LOGD("sdcard_mount", "Card Mounted");
      mounted = true;
    } else {
      ESP_LOGD("sdcard_mount", "Card Mount Failed");
      SD.end();
      tries = tries - 1;
      delay(200);
    }
  }
  if(!mounted){
    return false;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    ESP_LOGD("sdcard_mount", "No SD card attached");
    return false;
  }

  ESP_LOGD("sdcard_mount", "SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    ESP_LOGD("sdcard_mount", "MMC");
  }
  else if (cardType == CARD_SD)
  {
    ESP_LOGD("sdcard_mount", "SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    ESP_LOGD("sdcard_mount", "SDHC");
  }
  else
  {
    ESP_LOGD("sdcard_mount", "UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  ESP_LOGD("sdcard_mount", "SD Card Size: %lluMB\n", cardSize);
  return true;
}

void sdcard_umount()
{
  ESP_LOGD("sdcard_umount", "Unmounting SD Card");
  SD.end();
}

void sdcard_load()
{
  File file;
  char b;
  String buffer;
  ESP_LOGD("sdcard_load", "Loading from SD Card");
  if (sdcard_mount())
  {
    file = SD.open(SCHEDULE_FILENAME, FILE_READ);
    if (file)
    {
      /* row: DHHMMTT
       * D - Day (int(1)) 1-7 starting from Monday
       * HH - Hours (int(2)) 00-23
       * MM - Minutes (int(2)) 00-59
       * TT - Temperature (int(2)) 14-26
       */
      setpoint toAdd;
      schedule.clear();
      file.setTimeout(100);
      buffer = file.readStringUntil('\n');
      while (file.available() > 2)
      {
        toAdd.day = file.parseInt();
        toAdd.hour = file.parseInt();
        toAdd.minute = file.parseInt();
        toAdd.temperature = file.parseInt();
        ESP_LOGD("sdcard_load", "%d,%d,%d,%d\n", toAdd.day, toAdd.hour, toAdd.minute, toAdd.temperature);
        add_to_schedule(toAdd);
      }
      file.close();
    }
    else
    {
      ESP_LOGD("sdcard_write", "File not found: %s", SCHEDULE_FILENAME);
    }
    sdcard_alert = false;
    sdcard_umount();
  }
  else
  {
    sdcard_alert = true;
  }
}
void sdcard_write()
{
  File file;
  char b;
  ESP_LOGD("sdcard_write", "Writing to SD Card");
  if (sdcard_mount())
  {
    file = SD.open(SCHEDULE_FILENAME, FILE_WRITE);
    if (file)
    {
      /* row: DHHMMTT
       * D - Day (int(1)) 1-7 starting from Monday
       * HH - Hours (int(2)) 00-23
       * MM - Minutes (int(2)) 00-59
       * TT - Temperature (int(2)) 14-26
       */
      // header
      file.print("day,hour,minute,temperature\n");
      for (int i = 0; i < schedule.size(); i++)
      {
        setpoint cur = schedule.get(i);

        file.printf("%d,%d,%d,%d\n", cur.day, cur.hour, cur.minute, cur.temperature);
        ESP_LOGD("sdcard_write", "%d,%d,%d,%d\n", cur.day, cur.hour, cur.minute, cur.temperature);
      }
      file.close();
      sdcard_alert = false;
    }
    else
    {
      ESP_LOGD("sdcard_write", "Unable to create file: %s", SCHEDULE_FILENAME);
      sdcard_alert = true;
    }
    sdcard_umount();
  }
  else
  {
    sdcard_alert = true;
  }
}
