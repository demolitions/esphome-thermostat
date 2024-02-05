bool heater_logic_running = false;

void heater_activate(bool newstate)
{
  if (newstate != heater_relay->state)
  {
    if (newstate)
    {
      heater_relay->turn_on();
    }
    else
    {
      heater_relay->turn_off();
    }
  }
}
void backlight_idle()
{
  if (floor(back_light->current_values.get_brightness() * 100) > floor(IDLE_BACKLIGHT_BRIGHTNESS * 100))
  {
    ESP_LOGD("backlight", "Backlight current value: %3.2f", back_light->current_values.get_brightness());
    esphome::light::LightCall call = back_light->make_call();
    call.set_brightness(IDLE_BACKLIGHT_BRIGHTNESS);
    call.set_transition_length(100);
    call.perform();
  }
}
void backlight_full()
{
  if (back_light->current_values.get_brightness() < 1)
  {
    esphome::light::LightCall call = back_light->make_call();
    call.set_brightness(1);
    call.set_transition_length(100);
    call.perform();
  }
  idle_time = IDLE_TIME;
}

void cron_second()
{
  if (idle_time > 0)
  {
    idle_time--;
  }
  else
  {
    backlight_idle();
  }
}

void heater_logic()
{
  if (heater_logic_running)
  {
    return;
  }
  heater_logic_running = true;
  if (therm_mode->state == "Manual")
  {
    if (sensor_temp->state + temp_offset->state < manual_temp_target->state)
    {
      heater_activate(true);
    }
    if (sensor_temp->state >= manual_temp_target->state)
    {
      heater_activate(false);
    }
  }
  else if (therm_mode->state == "Winter")
  {
    // figure out in which setpoint we are
    int current_day = day_stupid_to_int(rtc_time->now().day_of_week);
    int current_hour = rtc_time->now().hour;
    int current_minute = rtc_time->now().minute;
    ESP_LOGI("heater_logic", "==========================================================================================");
    ESP_LOGI("heater_logic", "Running as Winter with day:%d, time: %02d:%02d", current_day, current_hour, current_minute);
    setpoint cursor, toSet, yesterday;
    toSet.day = -1;
    toSet.hour = -1;
    toSet.minute = -1;
    toSet.temperature = -1;
    yesterday.day = -1;
    yesterday.hour = -1;
    yesterday.minute = -1;
    yesterday.temperature = -1;
    for (int i = 0; i < schedule.size(); i++)
    {
      cursor = (setpoint)schedule.get(i);
      if (cursor.day == current_day)
      {
        if (
            ((cursor.hour < current_hour) || ((cursor.hour == current_hour) && (cursor.minute <= current_minute))) &&
            ((cursor.hour > toSet.hour) || ((cursor.hour == toSet.hour) && (cursor.minute > toSet.minute))))
        {
          // may be the right one
          toSet.day = cursor.day;
          toSet.hour = cursor.hour;
          toSet.minute = cursor.minute;
          toSet.temperature = cursor.temperature;
        }
      }
      else if (((current_day > 1) && (cursor.day == current_day - 1)) || ((current_day == 1) && (cursor.day == 7)))
      {
        if ((yesterday.hour <= cursor.hour) && (yesterday.minute <= cursor.minute))
        {
          yesterday.day = cursor.day;
          yesterday.hour = cursor.hour;
          yesterday.minute = cursor.minute;
          yesterday.temperature = cursor.temperature;
        }
      }
    }
    ESP_LOGI("heater_logic", "toSet    : %d - %02d:%02d @ %d", toSet.day, toSet.hour, toSet.minute, toSet.temperature);
    ESP_LOGI("heater_logic", "yesterday: %d - %02d:%02d @ %d", yesterday.day, yesterday.hour, yesterday.minute, yesterday.temperature);
    ESP_LOGI("heater_logic", "schedule was %d setpoints long", schedule.size());

    if ((toSet.hour == -1) && (yesterday.temperature != 0))
    {
      // Didn't find a valid setpoint in today, apply last setpoint from yesterday
      ESP_LOGI("heater_logic", "Setting winter target temp from yesterday's last setpoint: %d", yesterday.temperature);
      if (yesterday.temperature != (int)winter_temp_target->state)
      {
        // winter_temp_target->set(yesterday.temperature);
        setNumber(winter_temp_target, yesterday.temperature);
      }
    }
    else if (toSet.temperature != 0)
    {
      ESP_LOGI("heater_logic", "Setting winter target temp from today's setpoint: %d", toSet.temperature);
      if (toSet.temperature != (int)winter_temp_target->state)
      {
        // winter_temp_target->set(toSet.temperature);
        setNumber(winter_temp_target, toSet.temperature);
      }
    }
    // in the end, apply the same logic as the manual mode
    if (sensor_temp->state + temp_offset->state < winter_temp_target->state)
    {
      heater_activate(true);
    }
    if (sensor_temp->state >= winter_temp_target->state)
    {
      heater_activate(false);
    }
    ESP_LOGI("heater_logic", "==========================================================================================");
  }
  else if (therm_mode->state == "Summer")
  {
    heater_activate(false);
  }
  heater_logic_running = false;
}

void changeOperatingMode(std::__cxx11::string newMode)
{
  bool changed = false;
  ESP_LOGD("modeChange", "NewMode: %s", newMode.c_str());
  if (newMode == "Manual")
  {
    if (main_display->get_active_page() != page_manual_idle)
      changed = true;
    main_display->show_page(page_manual_idle);
  }
  if (newMode == "Summer")
  {
    if (main_display->get_active_page() != page_summer_idle)
      changed = true;
    main_display->show_page(page_summer_idle);
  }
  if (newMode == "Winter")
  {
    if (main_display->get_active_page() != page_winter_idle)
      changed = true;
    main_display->show_page(page_winter_idle);
  }
  heater_logic();
  // if (changed)
  // {
  //   delay(1);
    // sound_modechange->execute();
    // if (main_display->get_active_page() == page_winter_idle)
    // {
      // loadFromSDCard();
    // }
  // }
}

void logic_init()
{
  main_display->show_page(page_initialization);
  vTaskDelay(100);
  changeOperatingMode(therm_mode->state);
  sdcard_load();
  heater_logic();
}
