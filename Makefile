demo: demo.c utf8_progress.h
	$(CC) -o $@ $<

clean:
	rm -f demo

