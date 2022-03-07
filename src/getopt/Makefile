.POSIX:
.SUFFIXES:

# TODO: Sync with mkconfig for like EXE_OUT or something
test: test.o
	$(CC) -o $@ $<

getopt.$(STATIC_EXT): getopt.o
	$(STATIC_CMD) $(STATIC_OUT) $@ $<

getopt.o: getopt.c
	$(CC) $(CFLAGS) $<

test.o: test.c
	$(CC) -I.. $(CFLAGS) $<

clean:
	rm -rf *.o *.a *.$(SHARED_EXT) test
