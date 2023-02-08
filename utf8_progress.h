/*
 * Copyright (c) 2023, Daniel Tabor
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   Do this:
      #define UTF8_PROGRESS_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.

   // i.e. it should look like this:
   #include ...
   #include ...
   #include ...
   #define UTF8_PROGRESS_IMPLEMENTATION
   #include "utf8_progress.h"
 */
#ifndef __UTF8_PROGRESS_H__
#define __UTF8_PROGRESS_H__

#include <stdint.h>
#include <stdio.h>

#define UTF8_PROGRESS_TYPE_ASCII   0
#define UTF8_PROGRESS_TYPE_BLOCK   1
#define UTF8_PROGRESS_TYPE_BAR     2
#define UTF8_PROGRESS_TYPE_FADE    3
#define UTF8_PROGRESS_TYPE_PIE     4
#define UTF8_PROGRESS_TYPE_FALLING 5

typedef struct {
	uint8_t use;
	uint8_t fg;
	uint8_t bg;
} utf8_progress_color_t;

typedef struct {
	uint32_t min;
	uint32_t max;
} utf8_progress_range_t;

typedef struct {
	uint8_t type;
	uint8_t invert;
	uint16_t size;
	utf8_progress_color_t color;
	FILE* fp;
	utf8_progress_range_t range;
} utf8_progress_t;

/* Initialize progress bar
 * progress_type is one of: UTF8_PROGRESS_TYPE_...
 * size is the number of characters wide the progress bar will be
 */
int utf8_progress_init(utf8_progress_t* progress, uint8_t progress_type, uint16_t size);

/* Set the range of the progress bar
 * By default, the range is 0-size
 */
int utf8_progress_set_range(utf8_progress_t* progress, uint32_t min, uint32_t max);

/* Set the colors to use when rendering the progress bar
 * By default, colors are disabled.
 * fgcolor and bgcolor must be 0-15.
 */
int utf8_progress_set_colors(utf8_progress_t* progress, uint8_t fgcolor, uint8_t bgcolor);

/* Set the output FILE pointer to use when rendering.
 * By default, stdout is used.
 */
void utf8_progress_set_output(utf8_progress_t* progress, FILE* fp);

/* Render the progress bar at the current cursor position.
 * value is the current value of the progress (typically between the min and max of the range)
 */
void utf8_progress_render(utf8_progress_t* progress, uint32_t value);

#endif //__UTF8_PROGRESS_H__

#ifdef UTF8_PROGRESS_IMPLEMENTATION

static char* utf8_progress_tiles_0[6] = {
	" ","/","-","\\","|","#"};
static char* utf8_progress_tiles_0i[6] = {
	" ","\\","-","/","|","#"};
static char* utf8_progress_tiles_1[9] = {
	"\x20"         /*0x00020*/,
	"\xE2\x96\x8F" /*0x0258F*/,
	"\xE2\x96\x8E" /*0x0258E*/,
	"\xE2\x96\x8D" /*0x0258D*/,
	"\xE2\x96\x8C" /*0x0258C*/,
	"\xE2\x96\x8B" /*0x0258B*/,
	"\xE2\x96\x8A" /*0x0258A*/,
	"\xE2\x96\x89" /*0x02589*/,
	"\xE2\x96\x88" /*0x02588*/ };
static char* utf8_progress_tiles_1i[9] = {
	"\xE2\x96\x88" /*0x02588*/,
	"\xE2\x96\x89" /*0x02589*/,
	"\xE2\x96\x8A" /*0x0258A*/,
	"\xE2\x96\x8B" /*0x0258B*/,
	"\xE2\x96\x8C" /*0x0258C*/,
	"\xE2\x96\x8D" /*0x0258D*/,
	"\xE2\x96\x8E" /*0x0258E*/,
	"\xE2\x96\x8F" /*0x0258F*/,
	"\x20"         /*0x00020*/ };
static char* utf8_progress_tiles_2[9] = {
	"\x20"         /*0x00020*/,
	"\xE2\x96\x81" /*0x02581*/,
	"\xE2\x96\x82" /*0x02582*/,
	"\xE2\x96\x83" /*0x02583*/,
	"\xE2\x96\x84" /*0x02584*/,
	"\xE2\x96\x85" /*0x02585*/,
	"\xE2\x96\x86" /*0x02586*/,
	"\xE2\x96\x87" /*0x02587*/,
	"\xE2\x96\x88" /*0x02588*/ };
static char* utf8_progress_tiles_3[5] = {
	"\x20"         /*0x00020*/,
	"\xE2\x96\x91" /*0x02591*/,
	"\xE2\x96\x92" /*0x02592*/,
	"\xE2\x96\x93" /*0x02593*/,
	"\xE2\x96\x88" /*0x02588*/ };
static char* utf8_progress_tiles_4[5] = {
	"\xE2\x97\x8B" /*0x025CB*/,
	"\xE2\x97\x94" /*0x025D4*/,
	"\xE2\x97\x91" /*0x025D1*/,
	"\xE2\x97\x95" /*0x025D5*/,
	"\xE2\x97\x8F" /*0x025CF*/ };
static char* utf8_progress_tiles_4i[5] = {
	"\xE2\x97\x8F" /*0x025CF*/,
	"\xE2\x97\x95" /*0x025D5*/,
	"\xE2\x97\x91" /*0x025D1*/,
	"\xE2\x97\x94" /*0x025D4*/,
	"\xE2\x97\x8B" /*0x025CB*/};
static char* utf8_progress_tiles_5[13] = { 
	"\x20"             /*0x00020*/,
	"\xF0\x9F\xAC\x80" /*0x1FB00*/,
	"\xF0\x9F\xAC\x83" /*0x1FB03*/,
	"\xF0\x9F\xAC\x8F" /*0x1FB0F*/,
	"\xF0\x9F\xAC\x90" /*0x1FB10*/,
	"\xF0\x9F\xAC\x93" /*0x1FB13*/,
	"\xE2\x96\x8C"     /*0x0258C*/,
	"\xF0\x9F\xAC\x95" /*0x1FB15*/,
	"\xF0\x9F\xAC\x9B" /*0x1FB1B*/,
	"\xF0\x9F\xAC\xB2" /*0x1FB32*/,
	"\xF0\x9F\xAC\xB4" /*0x1FB34*/,
	"\xF0\x9F\xAC\xBA" /*0x1FB3A*/,
	"\xE2\x96\x88"     /*0x02588*/ };
static char* utf8_progress_tiles_5i[13] = { 
	"\x20"             /*0x00020*/,
	"\xF0\x9F\xAC\x81" /*0x1FB01*/,
	"\xF0\x9F\xAC\x87" /*0x1FB07*/,
	"\xF0\x9F\xAC\x9E" /*0x1FB1E*/,
	"\xF0\x9F\xAC\xA0" /*0x1FB20*/,
	"\xF0\x9F\xAC\xA6" /*0x1FB26*/,
	"\xe2\x96\x90"     /*0x02590*/,
	"\xF0\x9F\xAC\xA8" /*0x1FB28*/,
	"\xF0\x9F\xAC\xAB" /*0x1FB2B*/,
	"\xF0\x9F\xAC\xB7" /*0x1FB37*/,
	"\xF0\x9F\xAC\xB8" /*0x1FB38*/,
	"\xF0\x9F\xAC\xBB" /*0x1FB3B*/,
	"\xE2\x96\x88"     /*0x02588*/ };

typedef struct {
	uint8_t invert_color;
	uint8_t chars_len;
	char **chars;
} utf8_progress_type_t;

static utf8_progress_type_t utf8_progress_types[12] = {
	{ 0, 6, utf8_progress_tiles_0},{ 0,  6, utf8_progress_tiles_0i},
	{ 0, 9, utf8_progress_tiles_1},{ 1,  9, utf8_progress_tiles_1i},
	{ 0, 9, utf8_progress_tiles_2},{ 0,  9, utf8_progress_tiles_2},
	{ 0, 5, utf8_progress_tiles_3},{ 0,  5, utf8_progress_tiles_3},
	{ 0, 5, utf8_progress_tiles_4},{ 1,  5, utf8_progress_tiles_4i},
	{ 0,13, utf8_progress_tiles_5},{ 0, 13, utf8_progress_tiles_5i},
};

static uint8_t utf8_fgcolors[16] = {30, 31, 32, 33, 34, 35, 36, 37,  90,  91,  92,  93,  94,  95,  96,  97};
static uint8_t utf8_bgcolors[16] = {40, 41, 42, 43, 44, 45, 46, 47, 100, 101, 102, 103, 104, 105, 106, 107};

int utf8_progress_init(utf8_progress_t* progress, uint8_t progress_type, uint16_t size) {
	if( ! progress ) { return -1; }
	if( progress_type > 5 ) { return -1; }
	if( size == 0 ) { return -1; }
	progress->type = progress_type;
	progress->invert = 0;
	progress->size = size;
	progress->color.use = 0;
	progress->fp = stdout;
	progress->range.min = 0;
	progress->range.max = size;
	return 0;
}

int utf8_progress_set_range(utf8_progress_t* progress, uint32_t min, uint32_t max) {
	if( ! progress ) { return -1; }
	if( max < min ) { return -1; }
	if( max == 0 ) { return -1; }
	progress->range.min = min;
	progress->range.max = max;
	return 0;
}

int utf8_progress_set_colors(utf8_progress_t* progress, uint8_t fgcolor, uint8_t bgcolor) {
	if( ! progress ) { return -1; }
	if( fgcolor > 15 ) { return -1; }
	if( bgcolor > 15 ) { return -1; }
	progress->color.use = 1;
	progress->color.fg = fgcolor;
	progress->color.bg = bgcolor;
}

void utf8_progress_invert(utf8_progress_t* progress) {
	if( ! progress ) { return; }
	progress->invert = 1 - progress->invert;
}

void utf8_progress_set_output(utf8_progress_t* progress, FILE* fp) {
	if( ! progress ) { return; }
	progress->fp = fp;
}

void utf8_progress_render(utf8_progress_t* progress, uint32_t value) {
	utf8_progress_type_t *ptype;
	uint32_t units_per_char;
	uint32_t units;
	uint16_t char_size;
	int i;
	char* c;
	
	if( ! progress ) { return; }
	
	ptype = &(utf8_progress_types[2*progress->type+progress->invert]);
	units_per_char = ptype->chars_len - 1;
	
	if( value < progress->range.min ) {
		units = 0;
	}
	else if( value > progress->range.max ) {
		units = progress->range.max * progress->size * units_per_char;
	}
	else {
		units = ((value-progress->range.min) * progress->size * units_per_char) / (progress->range.max-progress->range.min);
	}
	
	if( progress->color.use ) {
		if( !ptype->invert_color ) {
			fprintf(progress->fp,"\x1b[%d;%dm",utf8_fgcolors[progress->color.fg],utf8_bgcolors[progress->color.bg]);
		}
		else {
			fprintf(progress->fp,"\x1b[%d;%dm",utf8_fgcolors[progress->color.bg],utf8_bgcolors[progress->color.fg]);
		}
	}
	
	if( ! progress->invert ) {
		for( i=0; i<progress->size; i++ ) {
			if( units >= (i+1)*units_per_char ) {
				c = ptype->chars[units_per_char];;
			} else if( units < i*units_per_char ) {
				c = ptype->chars[0];
			} else {
				c = ptype->chars[units%units_per_char];
			}
			fprintf(progress->fp,"%s",c);
		}
	}
	else {
		for( i=0; i<progress->size; i++ ) {
			if( units >= (progress->size-i)*units_per_char ) {
				c = ptype->chars[units_per_char];;
			} else if( units < (progress->size-1-i)*units_per_char ) {
				c = ptype->chars[0];
			} else {
				c = ptype->chars[units%units_per_char];
			}
			fprintf(progress->fp,"%s",c);
		}
	}
	
	if( progress->color.use ) {
		fprintf(progress->fp,"\x1b[0m");
	}
}

#endif //UTF8_PROGRESS_IMPLEMENTATION