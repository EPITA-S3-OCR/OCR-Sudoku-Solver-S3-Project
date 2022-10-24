#include "lines.h"

void drawLine(SDL_Surface *surface, int xStart, int yStart, int xEnd, int yEnd,
              SDL_Color color)
{
  int    x0    = xStart;
  int    y0    = yStart;

  int    x1    = xEnd;
  int    y1    = yEnd;

  int    dx    = abs(x1 - x0);
  int    sx    = x0 < x1 ? 1 : -1;
  int    dy    = -abs(y1 - y0);
  int    sy    = y0 < y1 ? 1 : -1;

  int    err   = dx + dy;

  int    w     = surface->w;
  int    h     = surface->h;
  Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);
  while (1)
  {
    if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
    {
      putPixel(surface, x0, y0, pixel);
    }

    if (x0 == x1 && y0 == y1)
      break;

    int e2 = 2 * err;

    if (e2 >= dy)
    {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}

void drawLines(SDL_Surface *surface, LinkedList *lines, SDL_Color color)
{
  LinkedList *current = lines;
  while (current->next != NULL)
  {
    current    = current->next;

    Line *line = (Line *)current->value;
    drawLine(surface, line->x1, line->y1, line->x2, line->y2, color);
  }
}

double line_angle(Line *l1, Line *l2)
{
  double x1    = l1->x2 - l1->x1;
  double y1    = l1->y2 - l1->y1;
  double x2    = l2->x2 - l2->x1;
  double y2    = l2->y2 - l2->y1;
  double dot   = x1 * x2 + y1 * y2;
  double det   = x1 * y2 - y1 * x2;
  double angle = atan2(det, dot);

  return angle;
}

double get_angle(Line *l)
{
  double x1    = l->x2 - l->x1;
  double y1    = l->y2 - l->y1;
  double angle = atan2(y1, x1);
  return angle;
}

double min(double a, double b)
{
  if (a < b)
  {
    return a;
  }
  return b;
}

double getRotationAngle(LinkedList *lines)
{
  double      max_angle = 0;
  LinkedList *current   = lines;
  while (current->next != NULL)
  {
    current        = current->next;
    Line  *l1      = (Line *)current->value;
    double angle_1 = get_angle(l1);
    if (radiansToDegrees(fabs(angle_1)) > 80)
    {
      continue;
    }
    else
    {
      if (max_angle < fabs(angle_1))
      {
        max_angle = angle_1;
      }
    }
  }
  return max_angle;
}

void rotateLines(LinkedList *lines, double angle, int width, int height)
{
  // go through all lines
  // rotate their points from 0,0 by angle in radians
  LinkedList *current   = lines;

  double      cos_angle = cos(angle);
  double      sin_angle = sin(angle);

  while (current->next != NULL)
  {
    current = current->next;
    Line *l = (Line *)current->value;
    int   new_x, new_y;
    convertPoints(l->x1, l->y1, &new_x, &new_y, width, height, cos_angle,
                  sin_angle);
    l->x1 = new_x;
    l->y1 = new_y;
    convertPoints(l->x2, l->y2, &new_x, &new_y, width, height, cos_angle,
                  sin_angle);
    l->x2 = new_x;
    l->y2 = new_y;
  }
}

void get_intersection(Line l1, Line l2, int *x, int *y)
{
  int x1          = l1.x1;
  int y1          = l1.y1;
  int x2          = l1.x2;
  int y2          = l1.y2;

  int x3          = l2.x1;
  int y3          = l2.y1;
  int x4          = l2.x2;
  int y4          = l2.y2;

  int A1          = y2 - y1;
  int B1          = x1 - x2;
  int C1          = A1 * x1 + B1 * y1;

  int A2          = y4 - y3;
  int B2          = x3 - x4;
  int C2          = A2 * x3 + B2 * y3;

  int determinant = A1 * B2 - A2 * B1;
  if (determinant == 0)
  {
    *x = -1;
    *y = -1;
    return;
  }
  else
  {
    *x = (B2 * C1 - B1 * C2) / determinant;
    *y = (A1 * C2 - A2 * C1) / determinant;
  }
}

// LinkedList *square_detection(LinkedList *lines, int width, int height)
// {
//   LinkedList *current = lines;
//   LinkedList *squares = {NULL, NULL, NULL};
//   while (current->next != NULL)
//   {
//     current = current->next;
//     Line *l1 = (Line *)current->value;
//     LinkedList *current2 = lines;
//     while (current2->next != NULL)
//     {
//       current2 = current2->next;
//       Line *l2 = (Line *)current2->value;
//       if (l1 != l2)
//       {
//         int x, y;
//         get_intersection(*l1, *l2, &x, &y)
//         // get interesction, if interesction find other lines by looping
//         over
//         // the rest of the lines for an interesction that
//       }
//     }
//   }
// }

// void get_length(int x1, int y1, int x2, int y2)
// {
//   return
// }

void lineAverage(LinkedList *lines)
{
  int         threshold = 10;

  LinkedList *line1     = lines;
  LinkedList *line2     = lines;

  while (line1 == NULL || line1->next != NULL)
  {
    line1    = line1->next;
    Line *l1 = line1->value;
    line2    = lines;
    while (line2 == NULL || line2->next != NULL)
    {
      line2    = line2->next;
      Line *l2 = line2->value;
      if (l1 != l2)
      {
        if (abs(l1->x1 - l2->x1) < threshold
            && abs(l1->y1 - l2->y1) < threshold
            && abs(l1->x2 - l2->x2) < threshold
            && abs(l1->y2 - l2->y2) < threshold)
        {
          l1->x1 = (l1->x1 + l2->x1) / 2;
          l1->y1 = (l1->y1 + l2->y1) / 2;
          lkRemove(line2);
        }
        if (line2 == NULL)
        {
          if (abs(l1->x2 - l2->x2) < threshold
              && abs(l1->y2 - l2->y2) < threshold)
          {
            l1->x2 = (l1->x2 + l2->x2) / 2;
            l1->y2 = (l1->y2 + l2->y2) / 2;
            lkRemove(line2);
          }
        }
      }
    }
  }
}

void point(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  // put pixel in 10x10 square around x,y
  for (int i = -10; i < 10; i++)
  {
    for (int j = -10; j < 10; j++)
    {
      if (x + i >= 0 && x + i < surface->w && y + j >= 0 && y + j < surface->h)
      {
        putPixel(surface, x + i, y + j, pixel);
      }
    }
  }
}

void highlight(SDL_Surface *surface, LinkedList *lines)
{
  // find intersections between lines

  LinkedList *line1 = lines;
  LinkedList *line2 = lines;

  while (line1->next != NULL)
  {
    line1    = line1->next;
    Line *l1 = line1->value;
    line2    = lines;
    while (line2->next != NULL)
    {
      line2              = line2->next;
      Line  *l2          = line2->value;
      double x1          = l1->x1;
      double x2          = l1->x2;
      double y1          = l1->y1;
      double y2          = l1->y2;

      double x3          = l2->x1;
      double x4          = l2->x2;
      double y3          = l2->y1;
      double y4          = l2->y2;

      double a1          = y2 - y1;
      double b1          = x1 - x2;
      double c1          = a1 * x1 + b1 * y1;

      double a2          = y4 - y3;
      double b2          = x3 - x4;
      double c2          = a2 * x3 + b2 * y3;

      double determinant = a1 * b2 - a2 * b1;
      if (determinant == 0)
      {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        continue;
      }
      else
      {
        double x     = (b2 * c1 - b1 * c2) / determinant;
        double y     = (a1 * c2 - a2 * c1) / determinant;
        Uint32 pixel = SDL_MapRGB(surface->format, 0, 0, 255);
        point(surface, (int)x, (int)y, pixel);
      }
    }
  }
}
