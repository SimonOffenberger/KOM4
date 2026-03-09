#ifndef LCD_H
#define LCD_H

#include <stdint.h>


/******************************************************************************
Configuration
******************************************************************************/
#define NULL 0

// Return Values
#define OK 0
#define RETVAL_AREA_OUTSIDE_OF_DISPLAY 1
#define RETVAL_NO_PIXELS_TO_DRAW 2
#define RETVAL_SOME_PIXEL_OUTSIDE 3
#define NOK 4

// Pin levels
#define LOW 0
#define HIGH 1

#define COMMAND 1
#define DATA 0

#define PIN_WRITE   10
#define PIN_READ    11
#define PIN_COMMAND 9
#define PIN_CS      8
                  
#define nComData(x) ((!x)<<PIN_COMMAND)     //(((x == true) ? 0 : 1) << PIN_COMMAND)
#define CS(x)       (x<<PIN_CS)
#define READ(x)     (x<<PIN_READ)
#define WRITE(x)    (x<<PIN_WRITE)


#define CASET       0x2A
#define RASET       0x2B
#define WRITE_COLOR 0x2C

#define TAB_TO_SPACE_COUNT 2
#define TAB_CHAR 9

#define RETURN_ON_ERROR(expression)   \
    {                                 \
      int ___retval = expression;        \
      if(___retval != OK) return ___retval; \
    }    


/******************************************************************************
Defines the value to use if the LCD should be used in portrait mode.
******************************************************************************/
#define LCD_MODE_PORTRAIT  1

/******************************************************************************
Defines the value to use if the LCD should be used in landscape mode.
******************************************************************************/
#define LCD_MODE_LANDSCAPE 2

/******************************************************************************
Configures the mode in which the LCD will be used.
******************************************************************************/
#define LCD_MODE LCD_MODE_PORTRAIT


/******************************************************************************
Definitions
******************************************************************************/

typedef struct RGB565_s { uint16_t B:5; uint16_t G:6; uint16_t R:5; } RGB565_t;
typedef struct Image_s { uint16_t Width; uint16_t Height; RGB565_t const * Data; } Image_t;
typedef struct MonochromeImage_s { uint16_t Width; uint16_t Height; uint8_t const * Data; } MonochromeImage_t;
typedef struct Glyph_s { uint8_t const * Data; uint16_t Width; uint64_t:16; } Glyph_t;
typedef struct Font_s { Glyph_t const * (*GetGlyph)(char); uint16_t LineHeight; uint64_t:16; } Font_t;

extern RGB565_t const RGB565_BLACK;
extern RGB565_t const RGB565_GREY;
extern RGB565_t const RGB565_WHITE;
extern RGB565_t const RGB565_RED;
extern RGB565_t const RGB565_GREEN;
extern RGB565_t const RGB565_BLUE;
extern RGB565_t const RGB565_LIGHT_BLUE;

extern Font_t const Consolas;
extern Font_t const SegoeUI;
extern Image_t const Image;
//extern MonochromeImage_t const Image2;
extern MonochromeImage_t const MonochromeImage;

#if LCD_MODE == LCD_MODE_PORTRAIT

/******************************************************************************
Defines the width of the display in pixel.
******************************************************************************/
#define LCD_WIDTH  240

/******************************************************************************
Defines the height of the display in pixel.
******************************************************************************/
#define LCD_HEIGHT 320
	
#endif

#if LCD_MODE == LCD_MODE_LANDSCAPE

/******************************************************************************
Defines the width of the display in pixel.
******************************************************************************/
#define LCD_WIDTH  320
	
/******************************************************************************
Defines the height of the display in pixel.
******************************************************************************/
#define LCD_HEIGHT 240

#endif


/******************************************************************************
Initializes the LCD module and the required GPIOs.

returns: Zero on success, an error code otherwise.
  0: Success
  1: Required GPIOs could not be initialized.
******************************************************************************/
int Lcd_Initialize(void);

/******************************************************************************
Fills the specified rectangular region with the specified color.

x: The X coordinate of the upper left corner of the rectangle in pixels.
y: Thy Y coordiante of the upper left corner of the rectangle in pixels.
width: The width of the rectangle in pixels.
height: The height of the rectangle in pixels.
color: The RGB565 color to use.

returns: Zero on success, an error code otherwise.
0: Success
1: All pixels are outside the available display area.
2: No pixels to draw.
3: One or more pixels are outside the available display area.

Pixels outside the display are are ignored. Other pixels are still set.
******************************************************************************/
int Lcd_Fill(
 const uint_fast16_t x,
 const uint_fast16_t y,
 const uint_fast16_t width,
 const uint_fast16_t height,
 RGB565_t      color);


/**
 * @brief Fills the specifier rectangular region with a cross
 * 
 * @param x Coordinate x
 * @param y Coordinate y
 * @param width Width of the cross
 * @param height Height of the cross
 * @param B_color Background Color
 * @param F_color Foreground Color
 * @return int Error code
 */
int Draw_Cross(
		const uint_fast16_t x,
		const uint_fast16_t y,
		const uint_fast16_t width,
		const uint_fast16_t height,
		const RGB565_t B_color,
	  const RGB565_t F_color);

/**
 * @brief Draws a hollow rectangle (box outline) on the LCD.
 * 
 * Draws four border lines (top, bottom, left, right) to create a hollow
 * rectangular outline.
 * 
 * @param x X-coordinate of the upper left corner in pixels.
 * @param y Y-coordinate of the upper left corner in pixels.
 * @param width Width of the box in pixels.
 * @param height Height of the box in pixels.
 * @param border_width Width of the border lines in pixels.
 * @param B_color Border color.
 */
void Lcd_Draw_HollowBox(
		const uint_fast16_t x,
		const uint_fast16_t y,
		const uint_fast16_t width,
		const uint_fast16_t height,
    const uint_fast16_t border_width,
		const RGB565_t B_color);


/**
 * @brief Draws a dotted or dashed line on the LCD.
 * 
 * Creates a line pattern with alternating solid and transparent segments.
 * Automatically detects horizontal or vertical orientation based on dimensions.
 * 
 * @param x X-coordinate of the starting point in pixels.
 * @param y Y-coordinate of the starting point in pixels.
 * @param width Width of the line region in pixels.
 * @param height Height of the line region in pixels.
 * @param Dot_color Color of the visible segments.
 * @param Spacing_color Color of the gaps (typically background color).
 */
void Lcd_Draw_DottedLine(
		const uint_fast16_t x,
		const uint_fast16_t y,
		const uint_fast16_t width,
		const uint_fast16_t height,
		const RGB565_t Dot_color,
		const RGB565_t Spacing_color
  );

/**
 * @brief Draw a line using the Bresenham algorithm. (see:https://de.wikipedia.org/wiki/Bresenham-Algorithmus )
 *
 * This function draws a line between two points (x0, y0) and (x1, y1)
 * using only integer arithmetic. Each step sets a pixel (or pixel block)
 * on the LCD.
 */ 
void Lcd_DrawLine_Bresenham(uint_fast16_t x0, uint_fast16_t y0,
                            uint_fast16_t x1, uint_fast16_t y1,
														uint_fast16_t Line_Width,
                            const RGB565_t color);


/******************************************************************************
Draws the specified RGB565 image at the specified location.

x: The X coordinate of the upper left corner of the image in pixels.
y: Thy Y coordiante of the upper left corner of the image in pixels.
width: The width of the image in pixels.
height: The height of the image in pixels.
data: A pointer to the image data (RGB565 color array).

returns: Zero on success, an error code otherwise.
0: Success
1: All pixels are outside the available display area.
2: No pixels to draw.
3: One or more pixels are outside the available display area.

Pixels outside the display are are ignored. Other pixels are still set.
******************************************************************************/
int Lcd_Draw(
 const uint_fast16_t    x,
 const uint_fast16_t    y,
 const uint_fast16_t    width,
 const uint_fast16_t    height,
 const RGB565_t       * data);

/******************************************************************************
Draws the specified monochrome image at the specified location.

x: The X coordinate of the upper left corner of the image in pixels.
y: Thy Y coordiante of the upper left corner of the image in pixels.
width: The width of the image in pixels.
height: The height of the image in pixels.
data: A pointer to the image data (monochrome/bit array).
foreground: The foreground color to use.
background: The background color to use.

returns: Zero on success, an error code otherwise.
0: Success
1: All pixels are outside the available display area.
2: No pixels to draw.
3: One or more pixels are outside the available display area.

Pixels outside the display are are ignored. Other pixels are still set.
******************************************************************************/
int Lcd_DrawMonochrome(
 const uint_fast16_t   x,
 const uint_fast16_t   y,
 const uint_fast16_t   width,
 const uint_fast16_t   height,
 const uint8_t       * data,
 const RGB565_t        foreground,
 const RGB565_t        background);

/******************************************************************************
Prints the specified character at the specified location and advances the
position by the width of the character.

x: The X coordinate of the upper left corner of the character in pixels.
y: Thy Y coordiante of the upper left corner of the character in pixels.
character: The character to draw.
foreground: The foreground color to use.
background: The background color to use.

returns: Zero on success, an error code otherwise.
0: Success
1: All pixels are outside the available display area.
2: No pixels to draw.
3: One or more pixels are outside the available display area.
******************************************************************************/
int Lcd_PrintCharacter(
 uint_fast16_t * const  x,
 uint_fast16_t * const y,
 const char            character,
 const RGB565_t        foreground,
 const RGB565_t        background,
 const Font_t * const  font);

/******************************************************************************
Prints the specified string at the specified location.

x: The X coordinate of the upper left corner of the character in pixels.
y: Thy Y coordiante of the upper left corner of the character in pixels.
string: A zero terminated character string.
foreground: The foreground color to use.
background: The background color to use.

returns: Zero on success, an error code otherwise.
0: Success
1: All pixels are outside the available display area.
2: No pixels to draw.
3: One or more pixels are outside the available display area.
******************************************************************************/
int Lcd_Print(
 uint_fast16_t   x,
 uint_fast16_t   y,
 char const    * string,
 const RGB565_t        foreground,
 const RGB565_t        background,
 const Font_t  * const font);

#endif // LCD_H
