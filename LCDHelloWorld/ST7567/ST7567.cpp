/* mbed library for the mbed Lab Board  128*32 pixel LCD
 * use C12832 controller
 * Copyright (c) 2012 Peter Drescher - DC2PD
 * Released under the MIT License: http://mbed.org/license/mit
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
/* 
 * Modified for the LPC General Purpose Shield 128*64 pixel LCD
 * use ST7567 controller
 *
 * Copyright (c) 2015 Toyomasa Watarai
 */


// 13.10.12    initial design
// 25.10.12    add autorefresh of screen
// 25.10.12    add standart font
// 20.12.12    add bitmap graphics
// 27.07.15    modified to 128x64 ST7567 contoller

#include "ST7567.h"
#include "mbed.h"
#include "Small_7.h"


ST7567::ST7567(PinName mosi, PinName sck, PinName reset, PinName a0, PinName ncs, const char* name)
    : GraphicsDisplay(name), _spi(mosi, NC, sck), _reset(reset), _A0(a0), _CS(ncs)
{
    orientation = 1;
    draw_mode = LCD_NORMAL;
    char_x = 0;
    lcd_reset();
}


int ST7567::width()
{
    if (orientation == 0 || orientation == 2)
        return MAX_PIXEL_Y;
    else
        return MAX_PIXEL_X;
}

int ST7567::height()
{
    if (orientation == 0 || orientation == 2)
        return MAX_PIXEL_X;
    else
        return MAX_PIXEL_Y;
}


void ST7567::invert(unsigned int o)
{
    if(o == 0)
        wr_cmd(0xA6);
    else
        wr_cmd(0xA7);
}


void ST7567::set_contrast(unsigned int o)
{
    contrast = o;
    wr_cmd(0x81);      //  set volume
    wr_cmd(o & 0x3F);
}

unsigned int ST7567::get_contrast(void)
{
    return(contrast);
}


// write command to lcd controller

void ST7567::wr_cmd(unsigned char cmd)
{
    _A0 = 0;
    _CS = 0;
    _spi.write(cmd);
    _CS = 1;
}

// write data to lcd controller

void ST7567::wr_dat(unsigned char dat)
{
    _A0 = 1;
    _CS = 0;
    _spi.write(dat);
    _CS = 1;
}

// reset and init the lcd controller

void ST7567::lcd_reset()
{

    _spi.format(8,3);                 // 8 bit spi mode 3
    _spi.frequency(10000000);         // 10 Mhz SPI clock
    _A0 = 0;
    _CS = 1;
    _reset = 0;                       // display reset
    wait_us(50);
    _reset = 1;                       // end reset
    wait_ms(5);

    /* Start Initial Sequence ----------------------------------------------------*/

    wr_cmd(0xE2);            // S/W RESWT
    wr_cmd(0xA3);            // LCD bias
    wr_cmd(0xAF);            // Display ON
    wr_cmd(0xA0);            // segment direction.
    wr_cmd(0xC8);            // Common Direction.
    wr_cmd(0x22);            // Regultion resistor select  //25

    wr_cmd(0x81);            // EV Select.
    wr_cmd(0x3f);            // Select EV value.

    wr_cmd(0x2f);            // Power control

    wr_cmd(0x40);            // Initial display line 40
    wr_cmd(0xB0);            // Set page address
    wr_cmd(0x10);            // Set coloumn addr  MSB
    wr_cmd(0x00);            // Set coloumn addr LSB
    wr_cmd(0xAF);            // Display ON
    wr_cmd(0xA4);            // A5 .Normal display, all pixels OFF.
    wr_cmd(0xA6);            // A7 .Normal display (Inverse Pixel)

    // clear and update LCD
    memset(buffer, 0x00, LCD_FB_SIZE);  // clear display buffer
    copy_to_lcd();
    auto_up = 1;              // switch on auto update

    locate(0,0);
    set_font((unsigned char*)Small_7);  // standart font
}

// set one pixel in buffer

void ST7567::pixel(int x, int y, int color)
{
    // first check parameter
    if(x > MAX_PIXEL_X || y > MAX_PIXEL_Y || x < 0 || y < 0) return;

    if(draw_mode == LCD_NORMAL) {
        if(color == 0)
            buffer[x + ((y/8) * 128)] &= ~(1 << (y%8));  // erase pixel
        else
            buffer[x + ((y/8) * 128)] |= (1 << (y%8));   // set pixel
    } else { // XOR mode
        if(color == 1)
            buffer[x + ((y/8) * 128)] ^= (1 << (y%8));   // xor pixel
    }
}

// update lcd

void ST7567::copy_to_lcd(void)
{
    for (int page = 0; page < 8; page++) {
        wr_cmd(0x00);        // set column low nibble 0
        wr_cmd(0x10);        // set column hi  nibble 0
        wr_cmd(0xB0|page);   // set page address  0
        _A0 = 1;

        for(int i=page*128; i<(page*128+128); i++) {
            wr_dat(buffer[i]);
        }
    }
}

void ST7567::cls(void)
{
    memset(buffer, 0x00, LCD_FB_SIZE);  // clear display buffer
    copy_to_lcd();
}


void ST7567::line(int x0, int y0, int x1, int y1, int color)
{
    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;

    dx = x1-x0;
    dy = y1-y0;

    //  if (dx == 0) {        /* vertical line */
    //      if (y1 > y0) vline(x0,y0,y1,color);
    //      else vline(x0,y1,y0,color);
    //      return;
    //  }

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    //  if (dy == 0) {        /* horizontal line */
    //      if (x1 > x0) hline(x0,x1,y0,color);
    //      else  hline(x1,x0,y0,color);
    //      return;
    //  }

    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {

            pixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        pixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
            pixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        pixel(x0, y0, color);
    }
    if(auto_up) copy_to_lcd();
}

void ST7567::rect(int x0, int y0, int x1, int y1, int color)
{

    if (x1 > x0) line(x0,y0,x1,y0,color);
    else  line(x1,y0,x0,y0,color);

    if (y1 > y0) line(x0,y0,x0,y1,color);
    else line(x0,y1,x0,y0,color);

    if (x1 > x0) line(x0,y1,x1,y1,color);
    else  line(x1,y1,x0,y1,color);

    if (y1 > y0) line(x1,y0,x1,y1,color);
    else line(x1,y1,x1,y0,color);

    if(auto_up) copy_to_lcd();
}

void ST7567::fillrect(int x0, int y0, int x1, int y1, int color)
{
    int l,c,i;
    if(x0 > x1) {
        i = x0;
        x0 = x1;
        x1 = i;
    }

    if(y0 > y1) {
        i = y0;
        y0 = y1;
        y1 = i;
    }

    for(l = x0; l<= x1; l ++) {
        for(c = y0; c<= y1; c++) {
            pixel(l,c,color);
        }
    }
    if(auto_up) copy_to_lcd();
}



void ST7567::circle(int x0, int y0, int r, int color)
{

    int draw_x0, draw_y0;
    int draw_x1, draw_y1;
    int draw_x2, draw_y2;
    int draw_x3, draw_y3;
    int draw_x4, draw_y4;
    int draw_x5, draw_y5;
    int draw_x6, draw_y6;
    int draw_x7, draw_y7;
    int xx, yy;
    int di;
    //WindowMax();
    if (r == 0) {       /* no radius */
        return;
    }

    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < height()) {
        pixel(draw_x0, draw_y0, color);     /* 90 degree */
    }

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0) {
        pixel(draw_x2, draw_y2, color);    /* 270 degree */
    }

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < width()) {
        pixel(draw_x4, draw_y4, color);     /* 0 degree */
    }

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5>=0) {
        pixel(draw_x5, draw_y5, color);     /* 180 degree */
    }

    if (r == 1) {
        return;
    }

    di = 3 - 2*r;
    xx = 0;
    yy = r;
    while (xx < yy) {

        if (di < 0) {
            di += 4*xx + 6;
        } else {
            di += 4*(xx - yy) + 10;
            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }
        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

        if ( (draw_x0 <= width()) && (draw_y0>=0) ) {
            pixel(draw_x0, draw_y0, color);
        }

        if ( (draw_x1 >= 0) && (draw_y1 >= 0) ) {
            pixel(draw_x1, draw_y1, color);
        }

        if ( (draw_x2 <= width()) && (draw_y2 <= height()) ) {
            pixel(draw_x2, draw_y2, color);
        }

        if ( (draw_x3 >=0 ) && (draw_y3 <= height()) ) {
            pixel(draw_x3, draw_y3, color);
        }

        if ( (draw_x4 <= width()) && (draw_y4 >= 0) ) {
            pixel(draw_x4, draw_y4, color);
        }

        if ( (draw_x5 >= 0) && (draw_y5 >= 0) ) {
            pixel(draw_x5, draw_y5, color);
        }
        if ( (draw_x6 <=width()) && (draw_y6 <= height()) ) {
            pixel(draw_x6, draw_y6, color);
        }
        if ( (draw_x7 >= 0) && (draw_y7 <= height()) ) {
            pixel(draw_x7, draw_y7, color);
        }
    }
    if(auto_up) copy_to_lcd();
}

void ST7567::fillcircle(int x, int y, int r, int color)
{
    int i,up;
    up = auto_up;
    auto_up = 0;   // off
    for (i = 0; i <= r; i++)
        circle(x,y,i,color);
    auto_up = up;
    if(auto_up) copy_to_lcd();
}

void ST7567::setmode(int mode)
{
    draw_mode = mode;
}

void ST7567::locate(int x, int y)
{
    char_x = x;
    char_y = y;
}



int ST7567::columns()
{
    return width() / font[1];
}



int ST7567::rows()
{
    return height() / font[2];
}



int ST7567::_putc(int value)
{
    if (value == '\n') {    // new line
        char_x = 0;
        char_y = char_y + font[2];
        if (char_y >= height() - font[2]) {
            char_y = 0;
        }
    } else {
        character(char_x, char_y, value);
        if(auto_up) copy_to_lcd();
    }
    return value;
}

void ST7567::character(int x, int y, int c)
{
    unsigned int hor,vert,offset,bpl,j,i,b;
    unsigned char* zeichen;
    unsigned char z,w;

    if ((c < 31) || (c > 127)) return;   // test char range

    // read font parameter from start of array
    offset = font[0];                    // bytes / char
    hor = font[1];                       // get hor size of font
    vert = font[2];                      // get vert size of font
    bpl = font[3];                       // bytes per line

    if (char_x + hor > width()) {
        char_x = 0;
        char_y = char_y + vert;
        if (char_y >= height() - font[2]) {
            char_y = 0;
        }
    }

    zeichen = &font[((c - ' ') * offset) + 4]; // start of char bitmap
    w = zeichen[0];                            // width of actual char
    // construct the char into the buffer
    for (j=0; j<vert; j++) {  //  vert line
        for (i=0; i<hor; i++) {   //  horz line
            z =  zeichen[bpl * i + ((j & 0xF8) >> 3)+1];
            b = 1 << (j & 0x07);
            if (( z & b ) == 0x00) {
                pixel(x+i,y+j,0);
            } else {
                pixel(x+i,y+j,1);
            }

        }
    }

    char_x += w;
}


void ST7567::set_font(unsigned char* f)
{
    font = f;
}

void ST7567::set_auto_up(unsigned int up)
{
    if (up)
        auto_up = 1;
    else
        auto_up = 0;
}

unsigned int ST7567::get_auto_up(void)
{
    return (auto_up);
}

void ST7567::print_bm(Bitmap bm, int x, int y)
{
    int h,v,b;
    char d;

    for(v=0; v < bm.ySize; v++) {   // lines
        for(h=0; h < bm.xSize; h++) { // pixel
            if(h + x > (MAX_PIXEL_X-1)) break;
            if(v + y > (MAX_PIXEL_Y-1)) break;
            d = bm.data[bm.Byte_in_Line * v + ((h & 0xF8) >> 3)];
            b = 0x80 >> (h & 0x07);
            if((d & b) == 0) {
                pixel(x+h,y+v,0);
            } else {
                pixel(x+h,y+v,1);
            }
        }
    }

}
