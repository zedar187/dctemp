TARGET = main.elf
KOS_ROMDISK_DIR = romdisk

OBJS = \
	main.o \
	Vec2.o \
	Vec3.o \
	AssetManager.o \
	Material.o \
	romdisk.o

# The rm-elf step is to remove the target before building, to force the
# re-creation of the rom disk.
all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
#	-rm -f $(TARGET) $(OBJS) romdisk.*
	-rm -f $(TARGET) $(OBJS)

rm-elf:
	-rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $(TARGET) $(KOS_START) \
		$(OBJS) $(OBJEXTRA) -lstdc++ -lkosutils -lmp3 -lm -lGL $(KOS_LIBS)

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist:
	rm -f $(OBJS)
	$(KOS_STRIP) $(TARGET)
