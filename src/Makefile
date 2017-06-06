include ../common.mk
TARGET = ../tirf
OBJ = pixel.o tools.o sift_features.o

debug: CFLAGS += -g
debug: $(TARGET)

all: CFLAGS += -O3
all: $(TARGET)

$(TARGET): main.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

pixel.o: pixel.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

tools.o: tools.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

sift_features.o: sift_features.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

clean:
	rm $(TARGET)
	rm *.o
