IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS= -lbcm2835

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = XboxI2C.o PiPROM.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

PiPROM.a: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: p1
.PHONY: clean

p1: CFLAGS+= -DRASPI_1
p1b: CFLAGS+= -DRASPI_BPLUS
p2: CFLAGS+= -DRASPI_2
p3: CFLAGS+= -DRASPI_2

p1 p1b p2 p3: PiPROM.a

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
