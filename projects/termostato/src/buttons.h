#define BTN_WIDTH 90
#define BTN_HEIGHT 40

// #define BTN_TEXT_X (BTN_WIDTH / 2)
// #define BTN_TEXT_Y (BTN_HEIGHT / 2)

struct button
{
  int x;
  int y;
  int w;
  int h;
  String txt;
};

AonLinkedList<button> buttons = AonLinkedList<button>();

void buttons_add(int x, int y, int w, int h, String txt)
{
  button newbtn;
  newbtn.x = x;
  newbtn.y = y;
  newbtn.w = w;
  newbtn.h = h;
  newbtn.txt = txt;
  buttons.add(newbtn);
}
void buttons_add(int x, int y, String txt)
{
  buttons_add(x, y, BTN_WIDTH, BTN_HEIGHT, txt);
}
