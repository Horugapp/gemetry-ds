CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy

# Nombre del proyecto actualizado a GeometryDash
TARGET = geometrydash

# Rutas de devkitPro
ifeq ($(DEVKITPRO),)
DEVKITPRO = /opt/devkitpro
endif

LIBNDS = $(DEVKITPRO)/libnds
INCLUDE = -I$(LIBNDS)/include -I.

# Banderas de compilación
CFLAGS = -O2 -Wall -mthumb -specs=nds.specs $(INCLUDE)
LDFLAGS = -L$(LIBNDS)/lib -lnds9 -lgba -lm

# Archivos
OBJS = main.o

all: $(TARGET).nds

$(TARGET).nds: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).nds

$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET).elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.nds
