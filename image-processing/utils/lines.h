#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../utils/linked_list.h"
#include "../utils/utils.h"

struct Line
{
  int x1, y1, x2, y2;
} typedef Line;

/**
 * @brief Rotate lines from the list by the given angle.
 *
 * @param lines
 * @param angle
 * @param width
 * @param height
 */
void rotateLines(LinkedList *lines, double angle, int width, int height);

/**
 * @brief Draw a line from (xStart, yStart) to (xEnd, yEnd) with the color
 * (SDL_Color)
 *
 * @param surface
 * @param xStart
 * @param yStart
 * @param xEnd
 * @param yEnd
 * @param color
 */
void drawLine(SDL_Surface *surface, int xStart, int yStart, int xEnd, int yEnd,
              SDL_Color color);

/**
 * @brief Draw a LinkedList of lines.
 *
 * @param surface
 * @param lines
 * @param color
 */
void drawLines(SDL_Surface *surface, LinkedList *lines, SDL_Color color);

/**
 * @brief Get the rotation angle of the lines.
 *
 * @param lines
 * @return double
 */
double getRotationAngle(LinkedList *lines);

/**
 * @brief Simplify lines by removing the ones that are too close to each other.
 *
 * @param lines
 */
void lineAverage(LinkedList *lines);

/**
 * @brief Highlight the intersections of the lines.
 *
 * @param surface
 * @param lines
 */
void highlight(SDL_Surface *surface, LinkedList *lines);