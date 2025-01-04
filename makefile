GCCPARAMS = -m32 -g -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fno-stack-protector
ASPARAMS = --32
LDPARAMS = -melf_i386

BUILD_DIR = build
SRC_DIR = .

objects = $(BUILD_DIR)/loader.o $(BUILD_DIR)/gdt.o $(BUILD_DIR)/port.o $(BUILD_DIR)/interrupts.o $(BUILD_DIR)/interruptstubs.o $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	gcc $(GCCPARAMS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(BUILD_DIR)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $(BUILD_DIR)/$@ $(objects)


%.s: %.cpp
	gcc $(GCCPARAMS) -S -o $@ $<

mykernel.iso: mykernel.bin
	mkdir -p iso/boot/grub
	cp $(BUILD_DIR)/mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso

install: mykernel.bin
	sudo cp $(BUILD_DIR)/$< /boot/mykernel.bin

clean:
	rm -rf $(BUILD_DIR) mykernel.iso
	rm -rf iso

build: mykernel.iso

clean-build: clean build
	
clean-mid:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/mykernel.bin

# always run with qemu 32 bit
run: mykernel.iso
	qemu-system-i386 -cdrom mykernel.iso

make-run: clean-build run

run-vb: mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm 'l_os' &

debug: mykernel.bin
	mkdir -p $(BUILD_DIR)
	cp $(BUILD_DIR)/mykernel.bin $(BUILD_DIR)/mykernel.bin
	qemu-system-i386 -s -S -kernel $(BUILD_DIR)/mykernel.bin 
	
make-debug: clean-build debug
