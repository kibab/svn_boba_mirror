
#include "include.h"
#include "widget.h"

void Widget::Resize()
{
  Canvas.x = 0;
  Canvas.y = YDISP;
  Canvas.x2 = ScreenW()-1;
  Canvas.y2 = ScreenH()-1;
}

void Widget::Resize(RECT *rc)
{
  Canvas = *rc;
}

void Widget::Resize(int x, int y, int x2, int y2)
{
  Canvas.x = x;
  Canvas.y = y;
  Canvas.x2 = x2;
  Canvas.y2 = y2;
}

//---------------------------------------------------------------//
//                                LogWidget                      //
//---------------------------------------------------------------//

char clLOGWIDGETBG  [4] = {0, 0, 0, 100};
char clLOGWIDGETTEXT[4] = {0, 255, 0, 100};
char clLOGWIDGETSCROLLBAR[4] = {0, 255, 0, 100};

void LogWidget::onKey(int key_code, int key_msg)
{
  if (key_msg==KEY_DOWN)
  {
    switch (key_code)
    {
    case LEFT_BUTTON:
      line_offset -= 50;
      if (line_offset<0)
        line_offset = 0;
      break;
    case RIGHT_BUTTON:
      line_offset += 50;
      break;
    case UP_BUTTON:
      ScrollUp();
      break;
    case DOWN_BUTTON:
      ScrollDown();
      break;
    }
  }
}

void LogWidget::ScrollDown()
{
  if (last_line->index<last_line->next->index)
  {
    last_line = last_line->next;
    first_line = first_line->next;
  }
}

void LogWidget::ScrollUp()
{
  if (first_line->index>first_line->prev->index)
  {
    last_line = last_line->prev;
    first_line = first_line->prev;
  }
}

void LogWidget::onRedraw()
{
  DrawRectangle(Canvas.x, Canvas.y, Canvas.x2, Canvas.y2, 0, 0, clLOGWIDGETBG);
  if (!log)
  {
    ShowMSG(2, (int)"no log");
    return;
  }
  LockSched();
  while (last_line->next->index>last_line->index && last_line->next->index-first_line->index<lines_on_page-1)
    last_line = last_line->next;
  UnlockSched();

  int y = Canvas.y;
  Log::LogLine *curr_line=first_line;
  for (int i = first_line->index; i<=last_line->index; i++)
  {
    DrawScrollString(curr_line->str, Canvas.x, y+1, Canvas.x2-6, y+line_height-1, line_offset+1, FONT_SMALL, 0, clLOGWIDGETTEXT, 0);
    y += line_height;
    curr_line = curr_line->next;
  }

  DrawLine(Canvas.x2-3, Canvas.y+1, Canvas.x2-3, Canvas.y2-1, 0, clLOGWIDGETSCROLLBAR);
  if (log->log_start->prev->index - log->log_start->index)
  {
    int ys1 = (first_line->index*(Canvas.y2-Canvas.y-2)) /
              (log->log_start->prev->index - log->log_start->index);
    int ys2 = (last_line->index*(Canvas.y2-Canvas.y-2)) /
              (log->log_start->prev->index - log->log_start->index);
    DrawRectangle(Canvas.x2-5, Canvas.y+ys1+2, Canvas.x2-1, Canvas.y+ys2-1, 0, 0, clLOGWIDGETSCROLLBAR);
  }
}

LogWidget::LogWidget()
{
  Canvas.x = 0;
  Canvas.y = YDISP;
  Canvas.x2 = ScreenW()-1;
  Canvas.y2 = ScreenH()-1;

  log = Log::Active;
  first_line = log->log_start;
  last_line = first_line->prev;
  line_height = GetFontYSIZE(FONT_SMALL)+2;
  lines_on_page = (Canvas.y2-Canvas.y)/line_height;
  //if ((Canvas.y2-Canvas.y)%line_height)
  //  lines_on_page++;
  line_offset = 0;
}

LogWidget::~LogWidget()
{

}

//---------------------------------------------------------------//
//                                InfoWidget                     //
//---------------------------------------------------------------//

char clINFOWIDGETBG[4] = {255, 0, 0, 100};

void InfoWidget::onKey(int key_code, int key_msg)
{

}

void InfoWidget::onRedraw()
{
  DrawRectangle(Canvas.x, Canvas.y, Canvas.x2, Canvas.y2, 0, 0, clINFOWIDGETBG);
}

InfoWidget::InfoWidget()
{
  Canvas.x = 0;
  Canvas.y = YDISP;
  Canvas.x2 = ScreenW()-1;
  Canvas.y2 = ScreenH()-1;
}

InfoWidget::~InfoWidget()
{

}

//---------------------------------------------------------------//
//                                ListWidget                     //
//---------------------------------------------------------------//

char clLISTWIDGETBG[4] = {0, 0, 255, 100};

void ListWidget::onKey(int key_code, int key_msg)
{

}

void ListWidget::onRedraw()
{
  DrawRectangle(Canvas.x, Canvas.y, Canvas.x2, Canvas.y2, 0, 0, clLISTWIDGETBG);
}

ListWidget::ListWidget()
{
  Canvas.x = 0;
  Canvas.y = YDISP;
  Canvas.x2 = ScreenW()-1;
  Canvas.y2 = ScreenH()-1;
}

ListWidget::~ListWidget()
{

}

