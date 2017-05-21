include ../common.mk
TARGET = ../tirf

all: $(TARGET)

$(TARGET): main.c pixel.o grayscale.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

pixel.o: pixel.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

grayscale.o: grayscale.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

clean:
	rm $(TARGET)
	rm *.o
