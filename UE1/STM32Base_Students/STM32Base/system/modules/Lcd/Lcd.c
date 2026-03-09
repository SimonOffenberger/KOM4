#include "Lcd.h"
#include "Gpio.h"
#include <stdlib.h>
#include <stm32f072xb.h>

RGB565_t const RGB565_BLACK = {.R = 0x00, .G = 0x00, .B = 0x00};
RGB565_t const RGB565_GREY = {.R = 0x0F, .G = 0x1F, .B = 0x0F};
RGB565_t const RGB565_WHITE = {.R = 0x1F, .G = 0x3F, .B = 0x1F};
RGB565_t const RGB565_RED = {.R = 0x1F, .G = 0x00, .B = 0x00};
RGB565_t const RGB565_GREEN = {.R = 0x00, .G = 0x3F, .B = 0x00};
RGB565_t const RGB565_BLUE = {.R = 0x00, .G = 0x00, .B = 0x1F};
RGB565_t const RGB565_LIGHT_BLUE = {.R = 0x10, .G = 0x2F, .B = 0x1F};

/***************************************************************************
Sends a single data or command byte to the LCD.
data: The data / commmand byte to send
type: Indicates whether the byte is a data or command byte.
***************************************************************************/
static inline void Lcd_Write(const uint8_t data, const bool command)
{

  // Construct the BitSet Field of the BSR Register
  register uint_fast16_t BSRR = nComData(command) | CS(LOW) | WRITE(LOW) | READ(HIGH) | data;

  // set and clear the corrosponding bits.
  // Try to clear the upper 12 Pins for Port E
  // Dont clear Pins 13-15 because this is the SPI Interface
  GPIOE->BSRR = (((0xFFF) << 16)) | BSRR;
  __NOP();
  // Let Write Line get high
  GPIOE->BSRR = WRITE(HIGH);
  __NOP();
}

/******************************************************************************
Initializes the LCD module and the required GPIOs.

returns: Zero on success, an error code otherwise.
  0: Success
  1: Required GPIOs could not be initialized.
******************************************************************************/
int Lcd_Initialize(void)
{

  // Initialize the Datalines with default value low
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_D0, GPIO_MODE_OUTPUT, false));
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_D1, GPIO_MODE_OUTPUT, false));
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_D2, GPIO_MODE_OUTPUT, false));
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_D3, GPIO_MODE_OUTPUT, false));
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_D4, GPIO_MODE_OUTPUT, false));
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_D5, GPIO_MODE_OUTPUT, false));
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_D6, GPIO_MODE_OUTPUT, false));
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_D7, GPIO_MODE_OUTPUT, false));

  // Set Chip Select
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_CS, GPIO_MODE_OUTPUT, false));

  // Set Line for Data / Command information set initialy to Data
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_DC, GPIO_MODE_OUTPUT, false));

  // Set the Line for Read / Write Access high -> Inactive!
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_RD, GPIO_MODE_OUTPUT, true));

  // Set the Line for Reset Signal -> Active low!
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_RES, GPIO_MODE_OUTPUT, true));

  // Signale for write enable to the LED -> Active low
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TFT_WR, GPIO_MODE_OUTPUT, true));

  // Initialize the LCD Display
  Lcd_Write(0x11, true); // exit SLEEP mode
  Lcd_Write(0x36, true);
  Lcd_Write(0x00, false); // MADCTL: memory data access control (portrait/landscape)
  Lcd_Write(0x3A, true);
  Lcd_Write(0x65, false); // COLMOD: Interface Pixel format
  Lcd_Write(0xB2, true);
  Lcd_Write(0x0C, false);
  Lcd_Write(0x0C, false);
  Lcd_Write(0x00, false);
  Lcd_Write(0x33, false);
  Lcd_Write(0x33, false); // PORCTRK: Porch setting
  Lcd_Write(0xB7, true);
  Lcd_Write(0x35, false); // GCTRL: Gate Control
  Lcd_Write(0xBB, true);
  Lcd_Write(0x2B, false); // VCOMS: VCOM setting
  Lcd_Write(0xC0, true);
  Lcd_Write(0x2C, false); // LCMCTRL: LCM Control
  Lcd_Write(0xC2, true);
  Lcd_Write(0x01, false);
  Lcd_Write(0xFF, false); // VDVVRHEN: VDV and VRH Command Enable
  Lcd_Write(0xC3, true);
  Lcd_Write(0x11, false); // VRHS: VRH Set
  Lcd_Write(0xC4, true);
  Lcd_Write(0x20, false); // VDVS: VDV Set
  Lcd_Write(0xC6, true);
  Lcd_Write(0x0F, false); // FRCTRL2: Frame Rate control in normal mode
  Lcd_Write(0xD0, true);
  Lcd_Write(0xA4, false);
  Lcd_Write(0xA1, false); // PWCTRL1: Power Control 1
  Lcd_Write(0xE0, true);
  Lcd_Write(0xD0, false);
  Lcd_Write(0x00, false);
  Lcd_Write(0x05, false);
  Lcd_Write(0x0E, false);
  Lcd_Write(0x15, false);
  Lcd_Write(0x0D, false);
  Lcd_Write(0x37, false);
  Lcd_Write(0x43, false);
  Lcd_Write(0x47, false);
  Lcd_Write(0x09, false);
  Lcd_Write(0x15, false);
  Lcd_Write(0x12, false);
  Lcd_Write(0x16, false);
  Lcd_Write(0x19, false); // PVGAMCTRL: Positive Voltage Gamma control
  Lcd_Write(0xE1, true);
  Lcd_Write(0xD0, false);
  Lcd_Write(0x00, false);
  Lcd_Write(0x05, false);
  Lcd_Write(0x0D, false);
  Lcd_Write(0x0C, false);
  Lcd_Write(0x06, false);
  Lcd_Write(0x2D, false);
  Lcd_Write(0x44, false);
  Lcd_Write(0x40, false);
  Lcd_Write(0x0E, false);
  Lcd_Write(0x1C, false);
  Lcd_Write(0x18, false);
  Lcd_Write(0x16, false);
  Lcd_Write(0x19, false); // NVGAMCTRL: Negative Voltage Gamma control
  Lcd_Write(0x29, true);  // Dsiplay: On

  return OK;
}

/******************************************************************************
Defines the area to which pixel data will be written to.
Commands:
CASET (2Ah): Column Address Set
RASET (2Bh): Row Address Set
0: Success
1: All pixels are outside the available display area.
2: No pixels to draw.
3: One or more pixels are outside the available display area.
******************************************************************************/
static int Lcd_SetArea(
    const uint_fast16_t xStart,
    uint_fast16_t xEnd,
    const uint_fast16_t yStart,
    uint_fast16_t yEnd)
{

  int retval = OK;

  // Check if Area is inside the display area
  if (xStart >= LCD_WIDTH)
    return RETVAL_AREA_OUTSIDE_OF_DISPLAY;
  if (yStart >= LCD_HEIGHT)
    return RETVAL_AREA_OUTSIDE_OF_DISPLAY;

  // Check if there are Pixels to draw
  if (xStart >= (xEnd + 1) && yStart >= (yEnd + 1))
    return RETVAL_NO_PIXELS_TO_DRAW;

  // Check if end is outside of Display
  if (xEnd >= LCD_WIDTH)
  {
    xEnd = LCD_WIDTH - 1;
    retval = RETVAL_SOME_PIXEL_OUTSIDE;
  }
  if (yEnd >= LCD_HEIGHT)
  {
    yEnd = LCD_HEIGHT - 1;
    retval = RETVAL_SOME_PIXEL_OUTSIDE;
  }

  // Set Colom
  Lcd_Write(CASET, COMMAND);
  Lcd_Write(xStart >> 8, DATA);
  Lcd_Write(xStart & 0xFF, DATA);
  Lcd_Write((xEnd) >> 8, DATA);
  Lcd_Write((xEnd) & 0xFF, DATA);

  // Set Row
  Lcd_Write(RASET, COMMAND);
  Lcd_Write(yStart >> 8, DATA);
  Lcd_Write(yStart & 0xFF, DATA);
  Lcd_Write((yEnd) >> 8, DATA);
  Lcd_Write((yEnd) & 0xFF, DATA);

  return retval;
}

/**
 * @brief Function to transmit the pixel Data to the LCD
 *
 * @param color of the pixel
 * @return int
 */
static inline int Lcd_TransmitPixelData(const RGB565_t *const color)
{

  if (color == NULL)
    return NOK;

  // Bitzuordnung f�r Pixel:
  // R1[4:0], G1[5:0], B1[4:0] =>
  // Byte1 Red Bit7..3 mit Green Bit2..0 und Byte2 Green Bit7..5 Blue Bit 4..0
  Lcd_Write((color->R << 3) | (color->G >> 3), DATA);
  Lcd_Write(((color->G & 0x3) << 5) | (color->B), DATA);

  return OK;
}

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
    const RGB565_t F_color)
{

  RETURN_ON_ERROR(Lcd_Fill(x, y, width, height, B_color));
  RETURN_ON_ERROR(Lcd_Fill(x, y + height / 2 - 1, width, 2, F_color));
  RETURN_ON_ERROR(Lcd_Fill(x + width / 2, y, 2, height, F_color));

  return OK;
}

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
void Lcd_Draw_HollowBox(const uint_fast16_t x, const uint_fast16_t y, const uint_fast16_t width, const uint_fast16_t height, const uint_fast16_t border_width, const RGB565_t B_color)
{
  Lcd_Fill(x, y, width, border_width, B_color);                         // Top Border
  Lcd_Fill(x, y + height - border_width, width, border_width, B_color); // Bottom Border
  Lcd_Fill(x, y, border_width, height, B_color);                        // Left Border
  Lcd_Fill(x + width - border_width, y, border_width, height, B_color); // Right Border
}

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
void Lcd_Draw_DottedLine(const uint_fast16_t x, const uint_fast16_t y, const uint_fast16_t width, const uint_fast16_t height, const RGB565_t Dot_color, const RGB565_t Spacing_color)
{

  // Determine whether the line is horizontal or vertical
  if (width > height)
  {
    // Horizontal Line
    for (uint_fast16_t i = 0; i < width; i += height * 2)
    {
      Lcd_Fill(x + i, y, height, height, Dot_color);
      Lcd_Fill(x + i + height, y, height, height, Spacing_color);
    }
  }
  else
  {
    // Vertical Line
    for (uint_fast16_t i = 0; i < height; i += width * 2)
    {
      Lcd_Fill(x, y + i, width, width, Dot_color);
      Lcd_Fill(x, y + i + width, width, width, Spacing_color);
    }
  }
}

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
    const RGB565_t color)
{
  int retval = OK;

  retval = Lcd_SetArea(x, x + width - 1, y, y + height - 1);

  // There are no pixels to draw return right away
  if (retval == RETVAL_AREA_OUTSIDE_OF_DISPLAY || retval == RETVAL_NO_PIXELS_TO_DRAW)
    return retval;

  Lcd_Write(WRITE_COLOR, COMMAND);

  // transmit for the amout of pixel in area the color
  for (uint_fast32_t w_pos = 0; w_pos < (width * height); w_pos++)
  {
    // Only handle Pixels outside of area if this is the case
    // because Mod and Div are slow!
    if (retval == RETVAL_SOME_PIXEL_OUTSIDE)
    {
      uint_fast32_t x_pos = x + w_pos % width;
      uint_fast32_t y_pos = y + w_pos / width;

      // only Print valid Pixels
      if (x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
      {
        RETURN_ON_ERROR(Lcd_TransmitPixelData(&color));
      }
    }
    else
    {
      RETURN_ON_ERROR(Lcd_TransmitPixelData(&color));
    }
  }

  return retval;
}

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
    const uint_fast16_t x,
    const uint_fast16_t y,
    const uint_fast16_t width,
    const uint_fast16_t height,
    const RGB565_t *data)
{

  if (data == NULL)
    return NOK;

  int retval = OK;

  retval = Lcd_SetArea(x, x + width - 1, y, y + height - 1);

  if (retval == RETVAL_AREA_OUTSIDE_OF_DISPLAY || retval == RETVAL_NO_PIXELS_TO_DRAW)
    return retval;

  Lcd_Write(WRITE_COLOR, COMMAND);

  // transmit for the amout of pixel in area the color
  for (uint_fast32_t w_pos = 0; w_pos < (width * height); w_pos++)
  {
    if (retval == RETVAL_SOME_PIXEL_OUTSIDE)
    {
      uint_fast32_t x_pos = x + w_pos % width;
      uint_fast32_t y_pos = y + w_pos / width;

      if (x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
      {
        RETURN_ON_ERROR(Lcd_TransmitPixelData(&data[w_pos]));
      }
    }
    else
    {
      RETURN_ON_ERROR(Lcd_TransmitPixelData(&data[w_pos]));
    }
  }

  return retval;
}

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
                            const RGB565_t color)
{
  int dx = abs((int)x1 - (int)x0);
  int sx = (x0 < x1) ? 1 : -1;
  int dy = -abs((int)y1 - (int)y0);
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx + dy;
  int e2;

  // Loop until the end point is reached
  while (x0 != x1 || y0 != y1)
  {

    Lcd_Fill(x0 - Line_Width / 2, y0 - Line_Width / 2, Line_Width, Line_Width, color);

    e2 = 2 * err;

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

  // Draw the final endpoint (previous loop stops *before* plotting it)
  Lcd_Fill(x1 - Line_Width / 2, y1 - Line_Width / 2, Line_Width, Line_Width, color);
}

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
    const uint_fast16_t x,
    const uint_fast16_t y,
    const uint_fast16_t width,
    const uint_fast16_t height,
    const uint8_t *data,
    const RGB565_t foreground,
    const RGB565_t background)
{
  if (data == NULL)
    return NOK;

  int retval = OK;

  retval = Lcd_SetArea(x, x + width - 1, y, y + height - 1);

  // There are no pixels to draw return right away
  if (retval == RETVAL_AREA_OUTSIDE_OF_DISPLAY || retval == RETVAL_NO_PIXELS_TO_DRAW)
    return retval;

  Lcd_Write(WRITE_COLOR, COMMAND);

  // transmit for the amout of pixel in area the color
  for (uint_fast32_t w_pos = 0; w_pos < (width * height); w_pos++)
  {

    // each bit represents a pixel value
    // 1 -> color of forground
    // 0 -> color of background
    uint_fast32_t data_index = w_pos >> 3;
    uint_fast8_t byte_index = w_pos & 0x7;

    const RGB565_t *color;

    if (data[data_index] >> byte_index & 0x1)
    {
      color = &foreground;
    }
    else
    {
      color = &background;
    }

    // Only handle Pixels outside of area if this is the case
    // because Mod and Div are slow!
    if (retval == RETVAL_SOME_PIXEL_OUTSIDE)
    {
      uint_fast32_t x_pos = x + w_pos % width;
      uint_fast32_t y_pos = y + w_pos / width;

      if (x_pos < LCD_WIDTH && y_pos < LCD_HEIGHT)
      {
        RETURN_ON_ERROR(Lcd_TransmitPixelData(color));
      }
    }
    else
    {
      RETURN_ON_ERROR(Lcd_TransmitPixelData(color));
    }
  }

  return retval;
}

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
    uint_fast16_t *const x,
    uint_fast16_t *const y,
    const char character,
    const RGB565_t foreground,
    const RGB565_t background,
    const Font_t *const font)
{

  if (x == NULL)
    return NOK;
  if (y == NULL)
    return NOK;
  if (font == NULL)
    return NOK;

  // Extract the values from pointer
  uint_fast16_t x_val = *x;
  uint_fast16_t y_val = *y;
  uint_fast16_t space_count = 0;
  char c = character;
  int return_val = OK;

  do
  {
    // only allow valid Characters that are present in the font
    if (character != '\n')
    {

      // print multiple Spaces for a Tab Character
      if (character == TAB_CHAR)
        c = ' ';

      // get the width of the specified Character
      uint16_t width = font->GetGlyph(c)->Width;
      int retval = OK;
      // Draw Character
      retval = Lcd_DrawMonochrome(x_val, y_val, width,
                                  font->LineHeight,
                                  font->GetGlyph(c)->Data, foreground, background);

      // no pixels to draw return
      if (retval == RETVAL_AREA_OUTSIDE_OF_DISPLAY || retval == RETVAL_NO_PIXELS_TO_DRAW)
        return retval;

      // Store Returnvalue for later
      if (retval == RETVAL_SOME_PIXEL_OUTSIDE)
        return_val = retval;

      // calculate new start coordinate
      x_val += width;

      // if the character does not fit on the screen insert Linefeed
      if (((x_val + width) >= (LCD_WIDTH - 1)))
      {
        x_val = 0;
        y_val = y_val + font->LineHeight;
      }

      // if the end of the display is reached wrap around
      if (((y_val + font->LineHeight) >= (LCD_HEIGHT - 1)))
      {
        y_val = 0;
        x_val = 0;
      }
    }
    // Insert Linefeed
    else if (character == '\n')
    {
      x_val = 0;
      y_val = y_val + font->LineHeight;
    }
    // increment Spacecount for handling Tabs
    space_count++;

    // for Tabs Print multiple spaces
  } while (character == TAB_CHAR && space_count < TAB_TO_SPACE_COUNT);

  // Update Pointer
  (*x) = x_val;
  (*y) = y_val;

  return return_val;
}

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
3: One or more pixels are outside the available display area. <-- Does not happen
  Line feeds are added if the edge of the display is reached!
******************************************************************************/
int Lcd_Print(
    uint_fast16_t x,
    uint_fast16_t y,
    char const *string,
    const RGB565_t foreground,
    const RGB565_t background,
    const Font_t *font)
{

  if (string == NULL)
    return NOK;
  if (font == NULL)
    return NOK;

  // call Print Character for each char of the string
  for (int i = 0; string[i] != '\0'; i++)
  {
    RETURN_ON_ERROR(Lcd_PrintCharacter(&x, &y, string[i], foreground, background, font));
  }

  return OK;
}