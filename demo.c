#define UTF8_PROGRESS_IMPLEMENTATION
#include "utf8_progress.h"

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

int main() {
	utf8_progress_t p[6];
	uint32_t pv;
	uint8_t i;
	char frame;
	
	//Initialize 5 progress bars
	utf8_progress_init(p+0, UTF8_PROGRESS_TYPE_ASCII, 12);
	utf8_progress_set_range(p+0, 0, 100);
	
	utf8_progress_init(p+1, UTF8_PROGRESS_TYPE_BLOCK, 20);
	utf8_progress_set_range(p+1, 0, 100);
	utf8_progress_set_colors(p+1, 2, 0);
	
	utf8_progress_init(p+2, UTF8_PROGRESS_TYPE_BAR, 15);
	utf8_progress_set_range(p+2, 50, 150);
	utf8_progress_set_colors(p+2, 9, 15);
	utf8_progress_invert(p+2);
	
	utf8_progress_init(p+3, UTF8_PROGRESS_TYPE_FADE, 7);
	utf8_progress_set_range(p+3, 100, 200);
	utf8_progress_set_colors(p+3, 11, 0);
	
	utf8_progress_init(p+4, UTF8_PROGRESS_TYPE_PIE, 5);
	utf8_progress_set_range(p+4, 100, 200);
	utf8_progress_set_colors(p+4, 0, 11);
	utf8_progress_invert(p+4);
	
	utf8_progress_init(p+5, UTF8_PROGRESS_TYPE_FALLING, 10);
	utf8_progress_set_range(p+5, 0, 200);
	utf8_progress_set_colors(p+5, 12, 0);
	
	//Increment and render each progress bar
	//Some will render prior to their minimum being reached
	//All will render past their maximum being reached
	for(pv=0; pv<210; pv++) {
		for( i=0; i<6; i++ ) {
			if( ! p[i].invert ) {
				frame = '>';
			}
			else {
				frame = '<';
			}
			printf("%03" PRId32 "/%03" PRId32 "/%03" PRId32 " %c",p[i].range.min,pv,p[i].range.max,frame);
			utf8_progress_render(p+i,pv);
			printf("%c\n",frame);
		}
		fflush(0);
		usleep(100000);
		printf("\x1b[6A");
	}
	printf("\x1b[6B");
}