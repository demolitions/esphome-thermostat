typedef struct
{
  int day;
  int hour;
  int minute;
  int temperature;
} setpoint;

AonLinkedList<setpoint> schedule = AonLinkedList<setpoint>();
AonLinkedList<setpoint> clipboard = AonLinkedList<setpoint>();
int schedule_current_day = 1;
int schedule_edit_index = -1;
setpoint schedule_edit_setpoint;
int schedule_edit_current_item = 0;
bool newpoint = false;

setpoint setpoint_clone(setpoint src)
{
  setpoint dest;
  dest.day = src.day;
  dest.hour = src.hour;
  dest.minute = src.minute;
  dest.temperature = src.temperature;
  return dest;
}

bool add_to_schedule(int day, int hour, int minute, int temperature)
{
  setpoint toAdd;
  setpoint lastpoint;
  toAdd.day = day;
  toAdd.hour = hour;
  toAdd.minute = minute;
  toAdd.temperature = temperature;
  if (schedule.size() > 0)
  {
    for (int i = 0; i < schedule.size(); i++)
    {
      lastpoint = (setpoint)schedule.get(i);
      if (toAdd.day < lastpoint.day)
      {
        schedule.add(i, toAdd);
        return true;
      }
      else if (toAdd.day > lastpoint.day)
      {
        continue;
      }
      else // same day
      {
        if (toAdd.hour < lastpoint.hour)
        {
          schedule.add(i, toAdd);
          return true;
        }
        else if (toAdd.hour > lastpoint.hour)
        {
          continue;
        }
        else // same day && hour
        {
          if (toAdd.minute < lastpoint.minute)
          {
            schedule.add(i, toAdd);
            return true;
          }
          else if (toAdd.minute > lastpoint.minute)
          {
            continue;
          }
          else
          {
            return false;
          }
        }
      }
    }
  }
  schedule.add(toAdd);
  return true;
}

bool add_to_schedule(setpoint toAdd)
{
  return add_to_schedule(toAdd.day, toAdd.hour, toAdd.minute, toAdd.temperature);
}

void remove_from_schedule(int day, int hour, int minute)
{
  setpoint toRemove;
  for (int i = 0; i < schedule.size(); i++)
  {
    toRemove = (setpoint)schedule.get(i);
    if ((toRemove.day == day) && (toRemove.hour == hour) && (toRemove.minute == minute))
    {
      schedule.remove(i);
      return;
    }
  }
}

void remove_from_schedule(setpoint toAdd)
{
  remove_from_schedule(toAdd.day, toAdd.hour, toAdd.minute);
}

void remove_day_from_schedule(int dayToRemove)
{
  for (int i = 0; i < schedule.size(); i++)
  {
    setpoint cur = schedule.get(i);
    if (cur.day == dayToRemove)
    {
      schedule.remove(i);
      i--;
    }
  }
}

void setpoint_clear(setpoint *point)
{
  point->day = 0;
  point->hour = 0;
  point->minute = 0;
  point->temperature = 0;
}

void write_buffer(int num)
{
  sound_click->execute();
  if (schedule_edit_current_item == 0)
  {
    if (schedule_edit_setpoint.hour == 0)
    {
      schedule_edit_setpoint.hour = num;
    }
    else if (schedule_edit_setpoint.hour < 10)
    {
      schedule_edit_setpoint.hour = (schedule_edit_setpoint.hour * 10) + num;
    }
    else
    { // >= 10
      schedule_edit_setpoint.hour = ((schedule_edit_setpoint.hour - (int)(floor(schedule_edit_setpoint.hour / 10)) * 10) * 10) + num;
    }
  }
  else if (schedule_edit_current_item == 1)
  {
    if (schedule_edit_setpoint.minute == 0)
    {
      schedule_edit_setpoint.minute = num;
    }
    else if (schedule_edit_setpoint.minute < 10)
    {
      schedule_edit_setpoint.minute = (schedule_edit_setpoint.minute * 10) + num;
    }
    else
    { // >= 10
      schedule_edit_setpoint.minute = ((schedule_edit_setpoint.minute - (int)(floor(schedule_edit_setpoint.minute / 10)) * 10) * 10) + num;
    }
  }
  else if (schedule_edit_current_item == 2)
  {
    if (schedule_edit_setpoint.temperature == 0)
    {
      schedule_edit_setpoint.temperature = num;
    }
    else if (schedule_edit_setpoint.temperature < 10)
    {
      schedule_edit_setpoint.temperature = (schedule_edit_setpoint.temperature * 10) + num;
    }
    else
    { // >= 10
      schedule_edit_setpoint.temperature = ((schedule_edit_setpoint.temperature - (int)(floor(schedule_edit_setpoint.temperature / 10)) * 10) * 10) + num;
    }
  }
}

void clear_buffer()
{
  if (schedule_edit_current_item == 0)
  {
    schedule_edit_setpoint.hour = 0;
  }
  else if (schedule_edit_current_item == 1)
  {
    schedule_edit_setpoint.minute = 0;
  }
  else if (schedule_edit_current_item == 1)
  {
    schedule_edit_setpoint.minute = 0;
  }
}

bool check_hour()
{
  return (schedule_edit_setpoint.hour >= 0 && schedule_edit_setpoint.hour <= 23);
}
bool check_minute()
{
  return (schedule_edit_setpoint.minute >= 0 && schedule_edit_setpoint.minute <= 59);
}
bool check_temperature()
{
  return (schedule_edit_setpoint.temperature >= 14 && schedule_edit_setpoint.temperature <= 26);
}
void parse_buffer()
{
  if (schedule_edit_current_item == 0 && check_hour())
  {
    sound_click->execute();
    schedule_edit_current_item = 1;
  }
  else if (schedule_edit_current_item == 1 && check_minute())
  {
    sound_click->execute();
    schedule_edit_current_item = 2;
  }
  else if (schedule_edit_current_item == 2 && check_temperature())
  {
    sound_click->execute();
    schedule_edit_current_item = 0;
  }
  else
  {
    sound_wrong->execute();
  }
}

bool store_buffer(int index)
{
  if (check_hour() && check_minute() && check_temperature())
  {
    if (index > -1)
    {
      schedule.remove(index);
    }
    schedule_edit_setpoint.day = schedule_current_day;
    return add_to_schedule(schedule_edit_setpoint);
  }
  return false;
}

bool store_buffer()
{
  return store_buffer(-1);
}

int day_stupid_to_int(int stupid)
{
  switch (stupid)
  {
  case 2: // Monday
    return 1;
  case 3: // Tuesday
    return 2;
  case 4: // Wednesday
    return 3;
  case 5: // Thursday
    return 4;
  case 6: // Friday
    return 5;
  case 7: // Saturday
    return 6;
  }
  return 7; // 1 - Sunday
}

int day_int_to_stupid(int day)
{
  switch (day)
  {
  case 1:
    return 2; // Monday
  case 2:
    return 3; // Tuesday
  case 3:
    return 4; // Wednesday
  case 4:
    return 5; // Thursday
  case 5:
    return 6; // Friday
  case 6:
    return 7; // Saturday
  }
  return 1; // Sunday
}

const char *day_name(int dayNum)
{
  switch (dayNum)
  {
  case 1:
    return "Mon";
  case 2:
    return "Tue";
  case 3:
    return "Wed";
  case 4:
    return "Thu";
  case 5:
    return "Fri";
  case 6:
    return "Sat";
  }
  return "Sun";
}

const char *day_name_short(int dayNum)
{
  switch (dayNum)
  {
  case 1:
    return "Mo";
  case 2:
    return "Tu";
  case 3:
    return "We";
  case 4:
    return "Th";
  case 5:
    return "Fr";
  case 6:
    return "Sa";
  }
  return "Su";
}

int day_from_name_short(std::string day_name)
{
  if (day_name == "Mo")
  {
    return 1;
  }
  else if (day_name == "Tu")
  {
    return 2;
  }
  else if (day_name == "We")
  {
    return 3;
  }
  else if (day_name == "Th")
  {
    return 4;
  }
  else if (day_name == "Fr")
  {
    return 5;
  }
  else if (day_name == "Sa")
  {
    return 6;
  }
  return 7;
}
int day_from_name(std::string day_name)
{
  if (day_name == "Mon")
  {
    return 1;
  }
  else if (day_name == "Tue")
  {
    return 2;
  }
  else if (day_name == "Wed")
  {
    return 3;
  }
  else if (day_name == "Thu")
  {
    return 4;
  }
  else if (day_name == "Fri")
  {
    return 5;
  }
  else if (day_name == "Sat")
  {
    return 6;
  }
  return 7;
}
