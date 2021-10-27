
QEMU 		:= qemu-system-x86_64
QEMUFLAGS	:= -m 4G -bios OVMF.fd -drive file=fat:rw:base/ -net none -serial stdio

run: core
	mkdir -p base/efi/boot/
	cp core/nuboot.efi base/efi/boot/bootx64.efi
	$(QEMU) $(QEMUFLAGS)

core:
	make -C core/

clean:
	make -C core/ clean

.PHONY: core