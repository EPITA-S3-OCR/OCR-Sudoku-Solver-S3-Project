#include "lines.h"

void drawLine(SDL_Surface *surface, int xStart, int yStart, int xEnd, int yEnd,
              SDL_Color color)
{
  int x0 = xStart;
  int y0 = yStart;

  int x1 = xEnd;
  int y1 = yEnd;

  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;

  int err = dx + dy;

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
  (void)color;
  SDL_Color colors[] = {
      {255, 0, 0, 255},     {0, 255, 0, 255},   {0, 0, 255, 255},
      {255, 255, 0, 255},   {255, 0, 255, 255}, {0, 255, 255, 255},
      {255, 255, 255, 255},
  };
  size_t      nbColors = sizeof(colors) / sizeof(SDL_Color);
  size_t      cpt      = 0;
  LinkedList *current  = lines;
  while (current->next != NULL)
  {
    current = current->next;

    Line *line = (Line *)current->value;
    drawLine(surface, line->x1, line->y1, line->x2, line->y2,
             colors[cpt % (nbColors - 1)]);
    cpt++;
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

double getAngle(Line *l)
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
    double angle_1 = getAngle(l1);
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
  LinkedList *current = lines;

  double cos_angle = cos(angle);
  double sin_angle = sin(angle);

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

void getIntersection(Line l1, Line l2, int *x, int *y, int width, int height)
{
  double x1 = l1.x1;
  double x2 = l1.x2;
  double y1 = l1.y1;
  double y2 = l1.y2;

  double x3 = l2.x1;
  double x4 = l2.x2;
  double y3 = l2.y1;
  double y4 = l2.y2;

  double a1 = y2 - y1;
  double b1 = x1 - x2;
  double c1 = a1 * x1 + b1 * y1;

  double a2 = y4 - y3;
  double b2 = x3 - x4;
  double c2 = a2 * x3 + b2 * y3;

  double determinant = a1 * b2 - a2 * b1;
  if (determinant == 0)
  {
    // The lines are parallel
    *x = -1;
    *y = -1;
    return;
  }
  else
  {
    double a = (b2 * c1 - b1 * c2) / determinant;
    double b = (a1 * c2 - a2 * c1) / determinant;
    // test if the intersection is within the bounds of the image
    if (a < 0 || a > width || b < 0 || b > height)
    {
      *x = -1;
      *y = -1;
      return;
    }
    *x = (int)a;
    *y = (int)b;
  }
}

SudokuCell makeSquare(int x1, int y1, int x2, int y2, int x3, int y3, int x4,
                      int y4)
{
  // get the bottom left point
  int xBottomLeft = fmin(fmin(x1, x2), fmin(x3, x4));
  int yBottomLeft = fmin(fmin(y1, y2), fmin(y3, y4));
  // get the top right point
  int xTopRight = fmax(fmax(x1, x2), fmax(x3, x4));
  int yTopRight = fmax(fmax(y1, y2), fmax(y3, y4));
  // get the bottom right point
  int xBottomRight = fmax(fmax(x1, x2), fmax(x3, x4));
  int yBottomRight = fmin(fmin(y1, y2), fmin(y3, y4));
  // get the top left point
  int xTopLeft = fmin(fmin(x1, x2), fmin(x3, x4));
  int yTopLeft = fmax(fmax(y1, y2), fmax(y3, y4));

  SudokuCell cell = {
      .xBottomLeft  = xBottomLeft,
      .yBottomLeft  = yBottomLeft,
      .xBottomRight = xBottomRight,
      .yBottomRight = yBottomRight,
      .xTopLeft     = xTopLeft,
      .yTopLeft     = yTopLeft,
      .xTopRight    = xTopRight,
      .yTopRight    = yTopRight,

  };
  return cell;
}

LinkedList squareDetection(LinkedList *lines, int width, int height)
{
  LinkedList  start   = {NULL, NULL, NULL};
  LinkedList *squares = &start;

  for (LinkedList *n1 = lines->next; n1->next != NULL; n1 = n1->next)
  {

    for (LinkedList *n2 = n1->next; n2->next != NULL; n2 = n2->next)
    {
      if (n1 == n2)
        continue;

      Line *line1 = (Line *)n1->value;
      Line *line2 = (Line *)n2->value;
      // Get all line that actualLine have a intersection point with
      int intX, intY;
      getIntersection(*line1, *line2, &intX, &intY, width, height);
      if (intX == -1)
        continue;
      // ALL INTERSECTED EDGES
      for (LinkedList *n3 = lines->next; n3->next != NULL; n3 = n3->next)
      {
        if (n2 == n3)
          continue;

        Line *line3 = (Line *)n3->value;
        int   intX2, intY2;
        getIntersection(*line2, *line3, &intX2, &intY2, width, height);

        if (intX2 == -1)
          continue;
        // ALL INTERSECTED EDGES

        for (LinkedList *n4 = n3->next; n4->next != NULL; n4 = n4->next)
        {
          if (n4 == n3)
            continue;

          Line *line4 = (Line *)n4->value;
          int   intX3, intY3;
          getIntersection(*line3, *line4, &intX3, &intY3, width, height);

          if (intX3 == -1)
            continue;

          // DOES K have intersection with h

          int intX4, intY4;
          getIntersection(*line4, *line1, &intX4, &intY4, width, height);

          if (intX4 != -1)
          {
            double distance1
                = sqrt(pow(intX - intX2, 2) + pow(intY - intY2, 2));

            double distance2
                = sqrt(pow(intX2 - intX3, 2) + pow(intY2 - intY3, 2));

            double distance3
                = sqrt(pow(intX3 - intX4, 2) + pow(intY3 - intY4, 2));

            double distance4
                = sqrt(pow(intX4 - intX, 2) + pow(intY4 - intY, 2));

            if (distance1 < 10)
              continue;
            if (fabs(distance1 - distance2) > 5
                || fabs(distance2 - distance3) > 5
                || fabs(distance3 - distance4) > 5
                || fabs(distance4 - distance1) > 5)
            {
              continue;
            }

            // test if any of the lines pointer is equal to another
            if (n1 == n2 || n1 == n3 || n1 == n4 || n2 == n3 || n2 == n4
                || n3 == n4)
            {
              // printf("EQUAL\n");
              continue;
              // printf("line1 : (%d, %d) -> (%d, %d)\n", line1->x1, line1->y1,
              //        line1->x2, line1->y2);
              // printf("line2 : (%d, %d) -> (%d, %d)\n", line2->x1, line2->y1,
              //        line2->x2, line2->y2);
              // printf("line3 : (%d, %d) -> (%d, %d)\n", line3->x1, line3->y1,
              //        line3->x2, line3->y2);
              // printf("line4 : (%d, %d) -> (%d, %d)\n", line4->x1, line4->y1,
              //        line4->x2, line4->y2);
              // printf("line1 : %p\n", line1);
              // printf("line2 : %p\n", line2);
              // printf("line3 : %p\n", line3);
              // printf("line4 : %p\n", line4);
            }
            // printf("SQUARE FOUND\n");
            SudokuCell cell = makeSquare(intX, intY, intX2, intY2, intX3,
                                         intY3, intX4, intY4);
            // printf("xBottomLeft: %d yBottomLeft: %d\nxBottomRight: %d "
            //        "yBottomRight: %d\nxTopLeft: %d yTopLeft: %d\nxTopRight:
            //        "
            //        "%d yTopRight: %d\n",
            //        cell.xBottomLeft, cell.yBottomLeft, cell.xBottomRight,
            //        cell.yBottomRight, cell.xTopLeft, cell.yTopLeft,
            //        cell.xTopRight, cell.yTopRight);

            SudokuCell *cellPtr = malloc(sizeof(SudokuCell));
            if (cellPtr == NULL)
            {
              printf("malloc failed\n");
            }
            *cellPtr = cell;
            squares  = lkAppend(squares, cellPtr);
          }
        }
      }
    }
  }
  return start;
}

LinkedList squareFilter(LinkedList *squares)
{
  LinkedList  start          = {NULL, NULL, NULL};
  LinkedList *filteredSquare = &start;
  // get the minimum square size
  // int minSize = 100000;
  // for (LinkedList *n = squares->next; n->next != NULL; n = n->next)
  // {
  //   SudokuCell *cell = (SudokuCell *)n->value;
  //   int         size = abs(cell->xBottomRight - cell->xBottomLeft);
  //   if (size < minSize)
  //     minSize = size;
  // }
  int minSize = 100;
  // printf("minSize : %d\n", minSize);
  for (LinkedList *n = squares->next; n->next != NULL; n = n->next)
  {
    SudokuCell *cell = (SudokuCell *)n->value;
    int         size = fmax(abs(cell->xBottomLeft - cell->xBottomRight),
                            abs(cell->xBottomLeft - cell->xTopLeft));
    if (size > minSize * 1.5 || size < minSize * 1)
    {
      // printf("size : %d\n", size);
      // printf("x  : bl = %d br = %d tl = %d tr = %d\n", cell->xBottomLeft,
      //        cell->xBottomRight, cell->xTopLeft, cell->xTopRight);
      continue;
      // printf("%d\n", abs(cell->xBottomLeft - cell->xBottomRight));
      // printf("%d\n", abs(cell->xBottomLeft - cell->xTopLeft));
      // printf("%d\n", abs(cell->xBottomRight - cell->xTopRight));
      // printf("%d\n", abs(cell->xTopLeft - cell->xTopRight));
    }
    filteredSquare = lkAppend(filteredSquare, cell);
  }
  return start;

  // get the bigest square
  // int maxSize = 0;
  // for (LinkedList *n = squares->next; n->next != NULL; n = n->next)
  // {
  //   SudokuCell *cell = (SudokuCell *)n->value;
  //   int         size = fmax(abs(cell->xBottomLeft - cell->xBottomRight),
  //                           abs(cell->xBottomLeft - cell->xTopLeft));
  //   if (size > maxSize)
  //     maxSize = size;
  // }
  // // get only the squares that are bigger than 80% of the bigest square
  // for (LinkedList *n = squares->next; n->next != NULL; n = n->next)
  // {
  //   SudokuCell *cell = (SudokuCell *)n->value;
  //   int         size = fmax(abs(cell->xBottomLeft - cell->xBottomRight),
  //                           abs(cell->xBottomLeft - cell->xTopLeft));
  //   if (size == maxSize)
  //   {
  //     filteredSquare = lkAppend(filteredSquare, cell);
  //   }
  // }
  // return start;
}

SudokuCell selectLeftMostCell(LinkedList *squares)
{
  int threshold = 15;

  SudokuCell *cell         = (SudokuCell *)squares->next->value;
  SudokuCell  selectedCell = *cell;
  // int         cpt          = 0;
  // Uint32      red          = SDL_MapRGB(surface->format, 255, 0, 0);
  for (LinkedList *n = squares->next; n->next != NULL; n = n->next)
  {
    SudokuCell *cell = (SudokuCell *)n->value;

    if ((cell->xBottomLeft <= selectedCell.xBottomLeft
         && abs(cell->yBottomLeft - selectedCell.yBottomLeft) < threshold)
        || (cell->yBottomLeft <= selectedCell.yBottomLeft
            && abs(cell->xBottomLeft - selectedCell.xBottomLeft) < threshold))
    {
      selectedCell = *cell;
    }
  }
  return selectedCell;
}
SudokuCell selectRightMostCell(LinkedList *squares)
{
  int threshold = 15;

  SudokuCell *cell         = (SudokuCell *)squares->next->value;
  SudokuCell  selectedCell = *cell;
  // int         cpt          = 0;
  // Uint32      red          = SDL_MapRGB(surface->format, 255, 0, 0);
  for (LinkedList *n = squares->next; n->next != NULL; n = n->next)
  {
    SudokuCell *cell = (SudokuCell *)n->value;

    if ((cell->xTopRight >= selectedCell.xTopRight
         && abs(cell->yTopRight - selectedCell.yTopRight) < threshold)
        || (cell->yTopRight >= selectedCell.yTopRight
            && abs(cell->xTopRight - selectedCell.xTopRight) < threshold))
    {
      selectedCell = *cell;
    }
  }
  return selectedCell;
}

void drawSquares(SDL_Surface *surface, LinkedList *squares, SDL_Color color)
{
  LinkedList *current = squares;
  while (current->next != NULL)
  {
    current          = current->next;
    SudokuCell *cell = (SudokuCell *)current->value;
    if (cell == NULL)
    {
      printf("cell is null\n");
    }
    // printf(
    //     "x1 : %d, y1 : %d\n x2 : %d, y2 : %d\n x3 : %d, y3 : %d\n x4 : %d,
    //     y4 "
    //     ": %d\n",
    //     cell->xBottomLeft, cell->yBottomLeft, cell->xBottomRight,
    //     cell->yBottomRight, cell->xTopRight, cell->yTopRight,
    //     cell->xTopLeft, cell->yTopLeft);
    drawLine(surface, cell->xBottomLeft, cell->yBottomLeft, cell->xTopRight,
             cell->yTopRight, color);
    drawLine(surface, cell->xBottomRight, cell->yBottomRight, cell->xTopLeft,
             cell->yTopLeft, color);
    drawLine(surface, cell->xBottomLeft, cell->yBottomLeft, cell->xTopLeft,
             cell->yTopLeft, color);
    drawLine(surface, cell->xBottomRight, cell->yBottomRight, cell->xTopRight,
             cell->yTopRight, color);
  }
}

void lineAverage(LinkedList *lines)
{
  int threshold = 10;

  LinkedList *line1 = lines;
  LinkedList *line2 = lines;

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
      line2     = line2->next;
      Line  *l2 = line2->value;
      double x1 = l1->x1;
      double x2 = l1->x2;
      double y1 = l1->y1;
      double y2 = l1->y2;

      double x3 = l2->x1;
      double x4 = l2->x2;
      double y3 = l2->y1;
      double y4 = l2->y2;

      double a1 = y2 - y1;
      double b1 = x1 - x2;
      double c1 = a1 * x1 + b1 * y1;

      double a2 = y4 - y3;
      double b2 = x3 - x4;
      double c2 = a2 * x3 + b2 * y3;

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
