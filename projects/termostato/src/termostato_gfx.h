typedef struct
{
  int temp;
  int start;
} graph_span;

AonLinkedList<graph_span> graph = AonLinkedList<graph_span>();

void display_clear()
{
  main_display->fill(COL_BLACK);
  buttons.clear();
}

Color display_get_fill_color()
{
  if (therm_mode->state == "Manual")
    return COL_ORANGE;
  if (therm_mode->state == "Winter")
    return COL_CYAN;
  return COL_GREY;
}

void display_draw_heater_icon()
{
  if (heater_relay->state)
  {
    main_display->image(290, 0, image_heater_on, COL_RED);
  }
  else
  {
    main_display->image(290, 0, image_heater_off, COL_BLUE);
  }
}

void display_draw_temp_current_icon()
{
  if (heater_relay->state)
  {
    main_display->image(290, 0, image_heater_on, COL_RED);
  }
  else
  {
    main_display->image(290, 0, image_heater_off, COL_BLUE);
  }
}

void display_draw_circle_temp(float temp)
{
  main_display->filled_circle(160, 120, 90, display_get_fill_color());
  main_display->filled_circle(160, 120, 60, COL_BLACK);
  main_display->printf(160, 120, tempFont, COL_WHITE, TextAlign::CENTER, "%02.1f", temp);
  main_display->print(160, 140, mainFont, COL_WHITE, TextAlign::TOP_CENTER, "°C");
}

void display_draw_circle_temp_small(float temp)
{
  main_display->filled_circle(160, 120, 60, display_get_fill_color());
  main_display->filled_circle(160, 120, 50, COL_BLACK);
  main_display->printf(160, 120, tempFont, COL_WHITE, TextAlign::CENTER, "%02.1f", temp);
  main_display->print(160, 140, mainFont, COL_WHITE, TextAlign::TOP_CENTER, "°C");
}

void display_draw_bottom_target_temp()
{
  main_display->printf(160, 240, smallFont, COL_WHITE, TextAlign::BOTTOM_CENTER, "Target: %-2.1f°C", manual_temp_target->state);
}

void display_draw_bottom_winter_target_temp()
{
  main_display->printf(160, 240, smallFont, COL_WHITE, TextAlign::BOTTOM_CENTER, "Target: %-2.1f°C", winter_temp_target->state);
}

void display_draw_upper_left_time()
{
  // main_display->image(0, 0, image_clock, COL_ORANGE);
  main_display->printf(35, 0, smallFont, COL_WHITE, TextAlign::TOP_CENTER, "%3s", day_name(day_stupid_to_int(rtc_time->now().day_of_week)));
  main_display->printf(35, 20, smallFont, COL_WHITE, TextAlign::TOP_CENTER, "%02d:%02d", rtc_time->now().hour, rtc_time->now().minute);
}

void display_draw_common_items()
{
  display_draw_upper_left_time();
  display_draw_heater_icon();
  main_display->printf(320, 240, smallFont, COL_WHITE, TextAlign::BOTTOM_RIGHT, "%s", therm_mode->state.c_str());
  if (alert)
  {
    main_display->image(300, 200, image_alert, COL_RED);
  }
  if (sdcard_alert)
  {
    main_display->image(260, 200, image_storage, COL_RED);
  }
}

void display_draw_current_temp_corner()
{
  main_display->image(0, 210, image_temp, COL_GREEN);
  main_display->printf(30, 240, smallFont, COL_WHITE, TextAlign::BOTTOM_LEFT, "%-2.1f°C", sensor_temp->state);
}

void display_page_manual_idle()
{
  display_clear();
  display_draw_common_items();
  display_draw_circle_temp(sensor_temp->state);
  display_draw_bottom_target_temp();
}
void display_page_manual_menu()
{
  display_clear();
  display_draw_common_items();
  display_draw_circle_temp_small(manual_temp_target->state);
  display_draw_button(160, 180, "Back", TextAlign::TOP_CENTER);
  display_draw_button(100, 120, "- 1", TextAlign::CENTER_RIGHT);
  display_draw_button(220, 120, "+ 1", TextAlign::CENTER_LEFT);
  display_draw_button(160, 60, "Winter", TextAlign::BOTTOM_RIGHT);
  display_draw_button(160, 60, "Summer", TextAlign::BOTTOM_LEFT);
  display_draw_current_temp_corner();
}
void display_page_summer_idle()
{
  display_clear();
  display_draw_common_items();
  display_draw_circle_temp(sensor_temp->state);
}
void display_page_winter_idle()
{
  display_clear();
  display_draw_common_items();
  display_draw_circle_temp(sensor_temp->state);
  display_draw_bottom_winter_target_temp();
}
void display_page_summer_menu()
{
  display_clear();
  display_draw_common_items();
  display_draw_circle_temp_small(sensor_temp->state);
  display_draw_button(160, 180, "Back", TextAlign::TOP_CENTER);
  display_draw_button(100, 120, "Winter", TextAlign::CENTER_RIGHT);
  display_draw_button(220, 120, "Manual", TextAlign::CENTER_LEFT);
}
void display_page_winter_menu()
{
  display_clear();
  display_draw_common_items();
  display_draw_circle_temp_small(sensor_temp->state);
  display_draw_button(160, 180, "Back", TextAlign::TOP_CENTER);
  display_draw_button(100, 120, "Manual", TextAlign::CENTER_RIGHT);
  display_draw_button(220, 120, "Summer", TextAlign::CENTER_LEFT);
  display_draw_button(160, 60, "Program", TextAlign::BOTTOM_CENTER);
}
void display_page_winter_schedule()
{
  display_clear();
  display_draw_common_items();
  display_draw_current_temp_corner();
  display_draw_button(100, 240, "Back", TextAlign::BOTTOM_LEFT);
  display_draw_button(320, 100, 80, 40, "Copy", TextAlign::BOTTOM_RIGHT);
  if (clipboard.size() > 0)
  {
    display_draw_button(320, 140, 80, 40, "Paste", TextAlign::BOTTOM_RIGHT);
  }
  display_draw_button(320, 200, 80, 40, "Add", TextAlign::BOTTOM_RIGHT);
  // Display weekday buttons - have to dothem one by one since the RTC lib number is shit
  main_display->filled_rectangle(35 + (schedule_current_day * 30), 0, 30, 30, COL_ORANGE);
  for (int i = 1; i <= 7; i++)
  {
    display_draw_button(35 + (i * 30), 0, 30, 30, day_name_short(i), TextAlign::TOP_LEFT);
  }
  int x = 4;
  int y = 40;
  int h = 0;
  int v = 200;
  int lastTemp = 0;
  String index;
  graph_span yesterday;
  setpoint last;
  graph.clear();
  for (int i = 0; i < schedule.size(); i++)
  {
    setpoint cur = schedule.get(i);
    if (cur.day == schedule_current_day)
    {
      if (cur.temperature >= 20)
      {
        main_display->filled_rectangle(x, y, 58, 40, COL_LIGHTRED);
      }
      else
      {
        main_display->filled_rectangle(x, y, 58, 40, COL_LIGHTBLUE);
      }
      index = String(i, DEC);
      // ESP_LOGD("setpoint_add", "%d : %s", i, index.c_str());
      buttons_add(x, y, 58, 40, index);
      main_display->rectangle(x, y, 58, 40, COL_WHITE);
      main_display->printf(x + 29, y + 10, smallFont, COL_WHITE, TextAlign::CENTER, "%02d:%02d", cur.hour, cur.minute);
      main_display->printf(x + 29, y + 30, smallFont, COL_WHITE, TextAlign::CENTER, "%02d", cur.temperature);
      x = x + 58;
      if (x >= 230)
      {
        x = 4;
        y = y + 40;
      }
      graph_span now;
      now.start = (cur.hour * 10) + (int)ceil(cur.minute / 10);
      now.temp = cur.temperature;
      graph.add(now);
    }
    else if (((schedule_current_day > 1) && (cur.day == schedule_current_day-1)) || ((schedule_current_day == 1) && (cur.day == 7)))
    {
      if (yesterday.start < cur.hour)
      {
        yesterday.temp = cur.temperature;
      }
    }
  }
  yesterday.start = 0;
  graph.unshift(yesterday);
  int lastx = 0;
  for (int i = 1; i < graph.size(); i++)
  {
    graph_span cur = graph.get(i);
    graph_span last = graph.get(i - 1);
    int y = 210 - last.temp;
    main_display->filled_rectangle(last.start, y, cur.start - last.start - 1, last.temp - 10, (last.temp >= 20) ? COL_LIGHTRED : COL_LIGHTBLUE);
    if (i == graph.size() - 1)
    {
      main_display->filled_rectangle(cur.start, 210 - cur.temp, 240 - cur.start, cur.temp - 10, (cur.temp >= 20) ? COL_LIGHTRED : COL_LIGHTBLUE);
    }
  }
}
void display_page_winter_edit()
{
  display_clear();
  display_draw_common_items();
  display_draw_current_temp_corner();
  // Functions
  main_display->filled_rectangle(35 + (schedule_current_day * 30), 0, 30, 30, COL_ORANGE);
  main_display->rectangle(35 + (schedule_current_day * 30), 0, 30, 30, COL_WHITE);
  main_display->printf(50 + (schedule_current_day * 30), 15, mainFont, COL_WHITE, TextAlign::CENTER, "%s", day_name_short(schedule_current_day));
  display_draw_button(100, 240, "Back", TextAlign::BOTTOM_LEFT);
  if (newpoint)
  {
    // Add setpoint
    display_draw_button(30, 200, "Store", TextAlign::BOTTOM_LEFT);
  }
  else
  {
    // Edit setpoint
    display_draw_button(0, 200, 80, 40, "Save", TextAlign::BOTTOM_LEFT);
    display_draw_button(80, 200, 80, 40, "Delete", TextAlign::BOTTOM_LEFT);
  }
  // Hour
  if (schedule_edit_current_item == 0)
  {
    main_display->filled_rectangle(0, 40, 140, 40, COL_CYAN);
  }
  display_draw_button(0, 40, 80, 40, "Hour", TextAlign::TOP_LEFT);
  main_display->printf(110, 60, mainFont, COL_WHITE, TextAlign::CENTER, "%02d", schedule_edit_setpoint.hour);
  // Minute
  if (schedule_edit_current_item == 1)
  {
    main_display->filled_rectangle(0, 80, 140, 40, COL_CYAN);
  }
  display_draw_button(0, 80, 80, 40, "Minute", TextAlign::TOP_LEFT);
  main_display->printf(110, 100, mainFont, COL_WHITE, TextAlign::CENTER, "%02d", schedule_edit_setpoint.minute);
  // Temperature
  if (schedule_edit_current_item == 2)
  {
    main_display->filled_rectangle(0, 120, 140, 40, COL_CYAN);
  }
  display_draw_button(0, 120, 80, 40, "Temp", TextAlign::TOP_LEFT);
  main_display->printf(110, 140, mainFont, COL_WHITE, TextAlign::CENTER, "%02d", schedule_edit_setpoint.temperature);
  // Keypad
  display_draw_button(160, 40, 40, 40, "7", TextAlign::TOP_LEFT);
  display_draw_button(200, 40, 40, 40, "8", TextAlign::TOP_LEFT);
  display_draw_button(240, 40, 40, 40, "9", TextAlign::TOP_LEFT);
  display_draw_button(160, 80, 40, 40, "4", TextAlign::TOP_LEFT);
  display_draw_button(200, 80, 40, 40, "5", TextAlign::TOP_LEFT);
  display_draw_button(240, 80, 40, 40, "6", TextAlign::TOP_LEFT);
  display_draw_button(160, 120, 40, 40, "1", TextAlign::TOP_LEFT);
  display_draw_button(200, 120, 40, 40, "2", TextAlign::TOP_LEFT);
  display_draw_button(240, 120, 40, 40, "3", TextAlign::TOP_LEFT);
  main_display->filled_rectangle(160, 160, 40, 40, COL_RED);
  display_draw_button(160, 160, 40, 40, "C", TextAlign::TOP_LEFT);
  display_draw_button(200, 160, 40, 40, "0", TextAlign::TOP_LEFT);
  main_display->filled_rectangle(240, 160, 40, 40, COL_GREEN);
  display_draw_button(240, 160, 40, 40, "OK", TextAlign::TOP_LEFT);
}

void display_page_initialization()
{
  display_clear();
  main_display->printf(160, 120, mainFont, COL_WHITE, TextAlign::CENTER, "Initializing...");
}