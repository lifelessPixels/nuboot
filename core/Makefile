
COMPILER 	:= clang++
LINKER		:= lld-link

COMPFLAGS	:= \
			   -Wall -Wextra -Werror \
			   -std=c++20 \
			   -target x86_64-unknown-windows \
			   -ffreestanding \
			   -fshort-wchar \
			   -mno-red-zone \
			   -MMD \
			   -Ilib/efiplusplus/include/

LINKFLAGS	:= \
			   -target x86_64-unknown-windows \
			   -nostdlib \
			   -Wl,-entry:nuboot_main \
			   -Wl,-subsystem:efi_application \
			   -fuse-ld=$(LINKER)

# -Wl,-subsystem:efi_boot_service_driver for modules

SOURCES		:= $(shell find . -name "*.cpp")
OBJECTS		:= $(SOURCES:.cpp=.o)
DEPS		:= $(SOURCES:.cpp=.d)

OUTPUT		:= nuboot.efi

all: $(OBJECTS)
	$(COMPILER) -o $(OUTPUT) $^ $(LINKFLAGS)

%.o: %.cpp
	$(COMPILER) -c -o $@ $< $(COMPFLAGS)

-include $(DEPS)

clean:
	rm -f $(OBJECTS)
	rm -f $(DEPS)

.PHONY: clean