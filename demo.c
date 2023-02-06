#define UTF8_PROGRESS_IMPLEMENTATION
#include "utf8_progress.h"

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

int main() {
	utf8_progress_t p[4];
	uint32_t pv;
	uint8_t i;
	
	//Initialize 4 progress bars
	utf8_progress_init(p+0, UTF8_PROGRESS_TYPE_BLOCK, 20);
	utf8_progress_set_range(p+0, 0, 100);
	
	utf8_progress_init(p+1, UTF8_PROGRESS_TYPE_BAR, 15);
	utf8_progress_set_range(p+1, 50, 150);
	utf8_progress_set_colors(p+1, 9, 15);
	
	utf8_progress_init(p+2, UTF8_PROGRESS_TYPE_PIE, 5);
	utf8_progress_set_range(p+2, 100, 200);
	utf8_progress_set_colors(p+2, 0, 11);
	
	utf8_progress_init(p+3, UTF8_PROGRESS_TYPE_FALLING, 10);
	utf8_progress_set_range(p+3, 0, 200);
	utf8_progress_set_colors(p+3, 12, 0);
	
	//Increment and render each progress bar forever
	for(pv=0; pv<210; pv++) {
		for( i=0; i<4; i++ ) {
			printf("%03" PRId32 "/%03" PRId32 "/%03" PRId32 " (",p[i].range.min,pv,p[i].range.max);
			utf8_progress_render(p+i,pv);
			printf(")\n");
		}
		fflush(0);
		usleep(100000);
		printf("\x1b[4A");
	}
	printf("\x1b[4B");
}