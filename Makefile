###############################################################################
# Makefile de prueba para la GBA                                              #
###############################################################################

# Cadena de desarrollo
TARGET      = arm-gba-elf
TOOLCHAIN   = /opt/gba/bin
AS          = $(TOOLCHAIN)/$(TARGET)-as
CC          = $(TOOLCHAIN)/$(TARGET)-gcc
LD          = $(TOOLCHAIN)/$(TARGET)-ld
GDB         = $(TOOLCHAIN)/$(TARGET)-insight
OBJCOPY     = $(TOOLCHAIN)/$(TARGET)-objcopy
GFX2GBA     = gfx2gba/gfx2gba-static
WAV2GBA     = wav2gba/wav2gba
BIN2C       = bin2c/bin2c

# Flags
INCDIR      = -I include -I lib
LIBDIR      = -L lib
ASFLAGS     = $(INCDIR) -mcpu=arm7tdmi -gstabs
CFLAGS      = $(INCDIR) -g -Wall -mcpu=arm7tdmi -c
LDFLAGS     = $(LIBDIR) -Tcart.ld -nostartfiles
LIBS        = -lgba

# Programas
PROG1       = juego

LIBGBA      = lib/libgba.a
STARTUP     = startup.o
FONDO       = fondo
FONDO2      = fondo2
LUFFYPATOS  = luffy_patos.h
SOUND_GOLPE = golpe

OBJS        = $(STARTUP) $(PROG1).o
ELFS        = $(PROG1).elf
BINS        = $(PROG1).gba

# Inicialización del depurador
GDBINI      = gdb.ini 

# Construcción
all : $(BINS) $(ELFS) $(OBJS)

$(LIBGBA):
	make -C lib

luffy_patos:
	wine wintools/gifs2sprites.exe 256 _$(LUFFYPATOS) imagenes/luffy*.gif imagenes/pato*.gif

$(SOUND_GOLPE).bin : sonido/$(SOUND_GOLPE).wav
	$(WAV2GBA) sonido/$(SOUND_GOLPE).wav $(SOUND_GOLPE).bin

$(SOUND_GOLPE).c : $(SOUND_GOLPE).bin
	$(BIN2C) $(SOUND_GOLPE).bin $(SOUND_GOLPE).c

$(FONDO).raw.c : imagenes/$(FONDO).bmp
	$(GFX2GBA) -fsrc -p$(FONDO).pal $<

$(FONDO2).raw.c : imagenes/$(FONDO2).bmp
	$(GFX2GBA) -fsrc -p$(FONDO2).pal $<

$(PROG1).o : $(PROG1).c $(FONDO).raw.c $(FONDO2).raw.c
	$(CC) $(CFLAGS) $(PROG1).c -o $(PROG1).o

%.elf : %.o $(STARTUP) $(LIBGBA)
	$(LD) $(LDFLAGS) $(STARTUP) $< -o $@ $(LIBS)

%.gba : %.elf
	$(OBJCOPY) -O binary $<  $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

# Simulacion
%.sim : %.gba
	$(VBA) $< &

# Depuración
%.deb: %.elf $(GDBINI)
	$(GDB) $< --command=$(GDBINI) &

$(GDBINI):
	echo "target remote | $(VBA) -Gpipe" > $(GDBINI)
	echo "load" >> $(GDBINI)

clean:
	make -C lib clean
	rm -f $(OBJS)
	rm -f $(ELFS)
	rm -f $(BINS)
	rm -f $(FONDO).raw.c $(FONDO).pal.c $(FONDO2).raw.c $(FONDO2).pal.c
	rm -f $(GDBINI) *~ include/*~
