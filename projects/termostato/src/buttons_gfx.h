void display_draw_button(int x, int y, int w, int h, esphome::display::Font *font, const char *txt, TextAlign aln, bool touchable, bool border)
{
  // preset as TOP_LEFT
  int bx = x;
  int by = y;
  if (aln == TextAlign::BOTTOM_LEFT)
    by = y - h;
  else if (aln == TextAlign::CENTER_LEFT)
    by = y - (h / 2);
  else if (aln == TextAlign::TOP_RIGHT)
    bx = x - w;
  else if (aln == TextAlign::CENTER_RIGHT)
  {
    bx = x - w;
    by = y - (h / 2);
  }
  else if (aln == TextAlign::CENTER)
  {
    bx = x - (w / 2);
    by = y - (h / 2);
  }
  else if (aln == TextAlign::BOTTOM_CENTER)
  {
    bx = x - (w / 2);
    by = y - h;
  }
  else if (aln == TextAlign::BOTTOM_RIGHT)
  {
    bx = x - w;
    by = y - h;
  }
  else if (aln == TextAlign::TOP_CENTER)
  {
    bx = x - (w / 2);
  }

  if (border)
  {
    main_display->rectangle(bx, by, w, h, COL_WHITE);
  }
  main_display->print(bx + (w / 2), by + (h / 2), font, COL_WHITE, TextAlign::CENTER, txt);
  if (touchable)
  {
    buttons_add(bx, by, w, h, txt);
  }
}
void display_draw_button(int x, int y, int w, int h, esphome::display::Font *font, const char *txt, TextAlign aln, bool touchable)
{
  display_draw_button(x, y, w, h, font, txt, aln, touchable, true);
}
void display_draw_button(int x, int y, int w, int h, esphome::display::Font *font, const char *txt, TextAlign aln)
{
  display_draw_button(x, y, w, h, font, txt, aln, true);
}
void display_draw_button(int x, int y, int w, int h, esphome::display::Font *font, const char *txt)
{
  display_draw_button(x, y, w, h, font, txt, TextAlign::TOP_LEFT);
}
void display_draw_button(int x, int y, esphome::display::Font *font, const char *txt, TextAlign aln)
{
  display_draw_button(x, y, BTN_WIDTH, BTN_HEIGHT, font, txt, aln);
}
void display_draw_button(int x, int y, esphome::display::Font *font, const char *txt)
{
  display_draw_button(x, y, BTN_WIDTH, BTN_HEIGHT, font, txt);
}
void display_draw_button(int x, int y, int w, int h, const char *txt, TextAlign aln)
{
  display_draw_button(x, y, w, h, mainFont, txt, aln);
}
void display_draw_button(int x, int y, int w, int h, const char *txt)
{
  display_draw_button(x, y, w, h, mainFont, txt);
}
void display_draw_button(int x, int y, const char *txt, TextAlign aln)
{
  display_draw_button(x, y, mainFont, txt, aln);
}
void display_draw_button(int x, int y, const char *txt)
{
  display_draw_button(x, y, mainFont, txt);
}
