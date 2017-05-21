include ../common.mk
TARGET = ../tirf

all: $(TARGET)

$(TARGET): main.c pixel.o tools.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

pixel.o: pixel.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

tools.o: tools.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

clean:
	rm $(TARGET)
	rm *.o
