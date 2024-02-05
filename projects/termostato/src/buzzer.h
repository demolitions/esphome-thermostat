#if __has_include("termostato/src/main.cpp")
#include "termostato/src/main.cpp"
#endif

#define PlayToneEvent (1 << 2) //100

typedef struct
{
  double frequency;
  int length;
  bool staccato;
  double volume;
} t_tone;

EventGroupHandle_t event_group;
AonLinkedList<t_tone> tones = AonLinkedList<t_tone>();
TaskHandle_t buzzer_playToneTask = NULL;
int pwm_channel = 1;

void buzzer_addTone(double frequency, int length, double volume, bool staccato)
{
  t_tone newtone;
  newtone.frequency = frequency;
  newtone.length = length;
  newtone.staccato = staccato;
  newtone.volume = 255 - (127 * volume);
  tones.add(newtone);
}
void buzzer_addTone(double frequency, int length, double volume)
{
  buzzer_addTone(frequency, length, volume, false);
}
void buzzer_addTone(double frequency, int length, bool staccato)
{
  buzzer_addTone(frequency, length, 1, staccato);
}
void buzzer_addTone(double frequency, int length)
{
  buzzer_addTone(frequency, length, 1, false);
}
void buzzer_addTone(double frequency)
{
  buzzer_addTone(frequency, 100, 1, false);
}

void buzzer_playTone(t_tone tone)
{
  ledcWrite(pwm_channel, tone.volume);
  ledcWriteTone(pwm_channel, tone.frequency);
  vTaskDelay(tone.length / portTICK_PERIOD_MS);
  if (tone.staccato)
  {
    ledcWriteTone(pwm_channel, 0);
  }
}

void buzzer_playTonesLoop(void *params)
{
  for (;;)
  {
    xEventGroupWaitBits(event_group, PlayToneEvent, pdFALSE, pdFALSE, portMAX_DELAY);
    while (tones.size() > 0)
    {
      buzzer_playTone(tones.shift());
    }
    ledcWriteTone(pwm_channel, 0);
    xEventGroupClearBits(event_group, PlayToneEvent);
  }
}

void buzzer_playTones()
{
  xEventGroupSetBits(event_group, PlayToneEvent);
}

void buzzer_setup(int pin, int pwm_channel)
{
  event_group = xEventGroupCreate();
  xEventGroupClearBits(event_group, 0xFF);
  ledcSetup(pwm_channel, 1E5, 12);
  ledcAttachPin(pin, pwm_channel);
  xTaskCreate(buzzer_playTonesLoop, "playTones", 10000, NULL, 0, &buzzer_playToneTask);
  ledcWriteTone(pwm_channel, 0);
}

void buzzer_setup(int pin)
{
  buzzer_setup(pin, 1);
}
