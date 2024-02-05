String button_detect_touch(int x, int y)
{
  // ESP_LOGD("button_detect_touch", "Detected Touch at X:%-3d Y:%-3d", x, y);
  for (int i = 0; i < buttons.size(); i++)
  {
    button cur = buttons.get(i);
    // ESP_LOGD("button_detect_touch", "Confronting with X>%-3d X<%-3d Y>%-3d Y<%-3d", cur.x, cur.x + cur.w, cur.y, cur.y + cur.h);
    if (x >= cur.x && x <= cur.x + cur.w && y >= cur.y && y <= cur.y + cur.h)
    {
      ESP_LOGD("button_detect_touch", "Touched button: %s", cur.txt.c_str());
      return cur.txt;
    }
  }
  ESP_LOGD("button_detect_touch", "Touched no buttons");
  return "_BUTTON_NONE_";
}

void touch_detect(int x, int y)
{
  backlight_full();
  if (therm_mode->state == "Manual") // ---------- MANUAL MODE
  {
    if (main_display->get_active_page() == page_manual_idle)
    {
      sound_click->execute();
      main_display->show_page(page_manual_menu);
    }
    else if (main_display->get_active_page() == page_manual_menu)
    {
      // Start checking all coordinates for buttons
      String btn = button_detect_touch(x, y);
      if (btn == "Back")
      {
        sound_click->execute();
        main_display->show_page(page_manual_idle);
      }
      else if (btn == "- 1")
      {
        sound_click->execute();
        // manual_temp_target->set(manual_temp_target->state - 1.0);
        setNumber(manual_temp_target ,manual_temp_target->state - 1.0);
        heater_logic();
      }
      else if (btn == "+ 1")
      {
        sound_click->execute();
        setNumber(manual_temp_target, manual_temp_target->state + 1.0);
        heater_logic();
      }
      else if (btn == "Summer")
      {
        sound_modechange->execute();
        setSelect(therm_mode, "Summer");
      }
      else if (btn == "Winter")
      {
        sound_modechange->execute();
        setSelect(therm_mode, "Winter");
      }
      else
      {
        sound_wrong->execute();
      }
    }
  }
  else if (therm_mode->state == "Summer") // ---------- SUMMER MODE
  {
    if (main_display->get_active_page() == page_summer_idle)
    {
      sound_click->execute();
      main_display->show_page(page_summer_menu);
    }
    else if (main_display->get_active_page() == page_summer_menu)
    {
      // Start checking all coordinates for buttons
      String btn = button_detect_touch(x, y);
      if (btn == "Back")
      {
        sound_click->execute();
        main_display->show_page(page_summer_idle);
      }
      else if (btn == "Manual")
      {
        sound_modechange->execute();
        setSelect(therm_mode, "Manual");
      }
      else if (btn == "Winter")
      {
        sound_modechange->execute();
        setSelect(therm_mode, "Winter");
      }
      else
      {
        sound_wrong->execute();
      }
    }
  }
  else if (therm_mode->state == "Winter") // ---------- WINTER MODE
  {
    if (main_display->get_active_page() == page_winter_idle)
    {
      sound_click->execute();
      main_display->show_page(page_winter_menu);
    }
    else if (main_display->get_active_page() == page_winter_menu)
    {
      // Start checking all coordinates for buttons
      String btn = button_detect_touch(x, y);
      if (btn == "Back")
      {
        sound_click->execute();
        main_display->show_page(page_winter_idle);
      }
      else if (btn == "Manual")
      {
        sound_modechange->execute();
        setSelect(therm_mode, "Manual");
      }
      else if (btn == "Summer")
      {
        sound_modechange->execute();
        setSelect(therm_mode, "Summer");
      }
      else if (btn == "Program")
      {
        sound_click->execute();
        clipboard.clear();
        sdcard_load();
        main_display->show_page(page_winter_schedule);
      }
      else
      {
        sound_wrong->execute();
      }
    }
    else if (main_display->get_active_page() == page_winter_schedule)
    {
      // Start checking all coordinates for buttons
      String btn = button_detect_touch(x, y);
      if (btn == "Back")
      {
        sound_click->execute();
        clipboard.clear();
        sdcard_load();
        main_display->show_page(page_winter_idle);
      }
      else if (btn == "Add")
      {
        sound_click->execute();
        clipboard.clear();
        setpoint_clear(&schedule_edit_setpoint);
        schedule_edit_setpoint.day = schedule_current_day;
        schedule_edit_current_item = 0;
        newpoint = true;
        main_display->show_page(page_winter_edit);
      }
      else if (btn == "Copy")
      {
        sound_click->execute();
        clipboard.clear();
        for (int i = 0; i < schedule.size(); i++)
        {
          setpoint cur = schedule.get(i);
          if (cur.day == schedule_current_day)
          {
            clipboard.add(setpoint_clone(cur));
          }
        }
      }
      else if (btn == "Paste")
      {
        sound_click->execute();
        remove_day_from_schedule(schedule_current_day);
        for (int i = 0; i < clipboard.size(); i++)
        {
          setpoint cur = clipboard.get(i);
          cur.day = schedule_current_day;
          add_to_schedule(cur);
        }
        sdcard_write();
        sdcard_load();
      }
      else if (btn == "Mo" || btn == "Tu" || btn == "We" || btn == "Th" || btn == "Fr" || btn == "Sa" || btn == "Su")
      {
        sound_click->execute();
        schedule_current_day = day_from_name_short(btn.c_str());
        // ESP_LOGD("Button", "Button: %s", btn);
        // ESP_LOGD("Button", "Day: %d", schedule_current_day);
      }
      else if (btn != "_BUTTON_NONE_")
      {
        // btn.remove(0, 3);
        sound_click->execute();
        clipboard.clear();
        setpoint_clear(&schedule_edit_setpoint);
        schedule_edit_index = btn.toInt();
        setpoint toEdit = schedule.get(schedule_edit_index);
        schedule_edit_setpoint.day = toEdit.day;
        schedule_edit_setpoint.hour = toEdit.hour;
        schedule_edit_setpoint.minute = toEdit.minute;
        schedule_edit_setpoint.temperature = toEdit.temperature;
        schedule_edit_current_item = 0;
        newpoint = false;
        main_display->show_page(page_winter_edit);
      }
      else
      {
        sound_wrong->execute();
      }
    }
    else if (main_display->get_active_page() == page_winter_edit)
    {
      // Start checking all coordinates for buttons
      String btn = String(button_detect_touch(x, y));
      if (btn == "Back")
      {
        sound_click->execute();
        main_display->show_page(page_winter_schedule);
      }
      else if (btn == "Store")
      {
        if (store_buffer())
        {
          sound_click->execute();
          sdcard_write();
          sdcard_load();
          main_display->show_page(page_winter_schedule);
        }
        else
        {
          sound_wrong->execute();
        }
      }
      else if (btn == "Save")
      {
        if (store_buffer(schedule_edit_index))
        {
          sound_click->execute();
          sdcard_write();
          sdcard_load();
          main_display->show_page(page_winter_schedule);
        }
        else
        {
          sound_wrong->execute();
        }
      }
      else if (btn == "Delete")
      {
        sound_click->execute();
        schedule.remove(schedule_edit_index);
        sdcard_write();
        sdcard_load();
        main_display->show_page(page_winter_schedule);
      }
      else if (btn == "Hour")
      {
        sound_click->execute();
        schedule_edit_current_item = 0;
      }
      else if (btn == "Minute")
      {
        sound_click->execute();
        schedule_edit_current_item = 1;
      }
      else if (btn == "Temp")
      {
        sound_click->execute();
        schedule_edit_current_item = 2;
      }
      else if (btn == "0" || btn == "1" || btn == "2" || btn == "3" || btn == "4" || btn == "5" || btn == "6" || btn == "7" || btn == "8" || btn == "9")
      {
        write_buffer(btn.toInt());
      }
      else if (btn == "C")
      {
        sound_click->execute();
        clear_buffer();
      }
      else if (btn == "OK")
      {
        parse_buffer();
      }
      else
      {
        sound_wrong->execute();
      }
    }
  }
}
