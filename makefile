# 定义编译参数
GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -fno-stack-protector
GCCPARAMS_D = $(GCCPARAMS) -g
ASPARAMS = --32
LDPARAMS = -melf_i386

# 定义目录
BUILD_DIR = build
RELEASE_DIR = $(BUILD_DIR)/release
DEBUG_DIR = $(BUILD_DIR)/debug
ISO_DIR = iso
RELEASE_ISO_DIR = $(BUILD_DIR)/release_iso
DEBUG_ISO_DIR = $(BUILD_DIR)/debug_iso

# 定义目标文件
objects_release = $(RELEASE_DIR)/loader.o $(RELEASE_DIR)/gdt.o $(RELEASE_DIR)/port.o $(RELEASE_DIR)/interruptstubs.o $(RELEASE_DIR)/interrupts.o $(RELEASE_DIR)/keyboard.o $(RELEASE_DIR)/kernel.o
objects_debug = $(DEBUG_DIR)/loader.o $(DEBUG_DIR)/gdt.o $(DEBUG_DIR)/port.o $(DEBUG_DIR)/interruptstubs.o $(DEBUG_DIR)/interrupts.o $(DEBUG_DIR)/keyboard.o $(DEBUG_DIR)/kernel.o

# 普通模式的编译规则
$(RELEASE_DIR)/%.o: %.cpp
	mkdir -p $(RELEASE_DIR)
	gcc $(GCCPARAMS) -c -o $@ $<

$(RELEASE_DIR)/%.o: %.s
	mkdir -p $(RELEASE_DIR)
	as $(ASPARAMS) -o $@ $<

# 调试模式的编译规则
$(DEBUG_DIR)/%.o: %.cpp
	mkdir -p $(DEBUG_DIR)
	gcc $(GCCPARAMS_D) -c -o $@ $<

$(DEBUG_DIR)/%.o: %.s
	mkdir -p $(DEBUG_DIR)
	as $(ASPARAMS) -o $@ $<


build: mykernel.iso

run : mykernel.iso
	qemu-system-i386 -cdrom $(RELEASE_ISO_DIR)/mykernel.iso

# 普通模式的构建规则
mykernel.bin: linker.ld $(objects_release)
	ld $(LDPARAMS) -T $< -o $@ $(objects_release)

mykernel.iso: mykernel.bin
	mkdir -p $(ISO_DIR)/boot/grub
	cp mykernel.bin $(ISO_DIR)/boot/mykernel.bin
	echo 'set timeout=0'                      > $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0'                     >> $(ISO_DIR)/boot/grub/grub.cfg
	echo ''                                  >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  boot'                            >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}'                                 >> $(ISO_DIR)/boot/grub/grub.cfg
	mkdir -p $(RELEASE_ISO_DIR)
	grub-mkrescue --output=$(RELEASE_ISO_DIR)/mykernel.iso $(ISO_DIR)
	rm -rf $(ISO_DIR)
	rm mykernel.bin

# 调试模式的构建规则
debug-build: $(objects_debug)
	ld $(LDPARAMS) -T linker.ld -o mykernel.bin $(objects_debug)
	mkdir -p $(ISO_DIR)/boot/grub
	cp mykernel.bin $(ISO_DIR)/boot/mykernel.bin
	echo 'set timeout=0'                      > $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0'                     >> $(ISO_DIR)/boot/grub/grub.cfg
	echo ''                                  >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  boot'                            >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}'                                 >> $(ISO_DIR)/boot/grub/grub.cfg
	mkdir -p $(DEBUG_ISO_DIR)
	grub-mkrescue --output=$(DEBUG_ISO_DIR)/mykernel.iso $(ISO_DIR)
	rm -rf $(ISO_DIR)
	rm mykernel.bin

# 安装规则
install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

# 清理规则
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) mykernel.bin $(RELEASE_ISO_DIR) $(DEBUG_ISO_DIR) $(ISO_DIR)