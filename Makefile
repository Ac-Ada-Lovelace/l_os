#
# 定义编译参数
GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -fno-stack-protector
GCCPARAMS_D = $(GCCPARAMS) -g
ASPARAMS = --32
LDPARAMS = -melf_i386
UPDATEPARAMS = $(filter-out -fno-leading-underscore, $(GCCPARAMS))
# 定义目录
BUILD_DIR = build
RELEASE_DIR = $(BUILD_DIR)/release
DEBUG_DIR = $(BUILD_DIR)/debug
ISO_DIR = iso
RELEASE_ISO_DIR = $(BUILD_DIR)/release_iso
DEBUG_ISO_DIR = $(BUILD_DIR)/debug_iso
SRC_RIR=src
UPDATE_DIR=$(BUILD_DIR)/update
# 定义目标文件
OBJECTS = loader.o gdt.o driver.o port.o interruptstubs.o interrupts.o keyboard.o kernel.o mouse.o
objects_release = $(addprefix $(RELEASE_DIR)/, $(OBJECTS))
objects_debug = $(addprefix $(DEBUG_DIR)/, $(OBJECTS))

# 目录创建规则
$(RELEASE_DIR) $(DEBUG_DIR):
	mkdir -p $@

# 普通模式的编译规则
$(RELEASE_DIR)/%.o: $(SRC_RIR)/%.cpp | $(RELEASE_DIR)
	gcc $(GCCPARAMS) -c -o $@ $<

$(RELEASE_DIR)/%.o: $(SRC_RIR)/%.s | $(RELEASE_DIR)
	as $(ASPARAMS) -o $@ $<

# 调试模式的编译规则
$(DEBUG_DIR)/%.o: %.cpp | $(DEBUG_DIR)
	gcc $(GCCPARAMS_D) -c -o $@ $<

	
build: mykernel.iso

run: mykernel.iso
	qemu-system-i386 -cdrom $(RELEASE_ISO_DIR)/mykernel.iso

runb: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBoxVM --startvm 'l_os' &

# 普通模式的构建规则
mykernel.bin: $(SRC_RIR)/linker.ld $(objects_release)
	ld $(LDPARAMS) -T $< -o $@ $(objects_release)

mykernel.iso: mykernel.bin
	$(call create_iso, $(RELEASE_ISO_DIR))

# 调试模式的构建规则
debug-build: $(objects_debug)
	ld $(LDPARAMS) -T linker.ld -o mykernel.bin $(objects_debug)
	$(call create_iso, $(DEBUG_ISO_DIR))

# 创建ISO镜像的函数
define create_iso
	mkdir -p $(ISO_DIR)/boot/grub
	mv mykernel.bin $(ISO_DIR)/boot/mykernel.bin
	echo 'set timeout=0'                      > $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0'                     >> $(ISO_DIR)/boot/grub/grub.cfg
	echo ''                                  >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  boot'                            >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}'                                 >> $(ISO_DIR)/boot/grub/grub.cfg
	echo $1
	mkdir -p $1

	grub-mkrescue --output=mykernel.iso ./$(ISO_DIR)
	mv mykernel.iso $1
	rm -rf $(ISO_DIR)
endef
# grub-mkrescue --output=$1/mykernel.iso ./$(ISO_DIR)

# 安装规则
install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

# 清理规则
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) mykernel.bin $(RELEASE_ISO_DIR) $(DEBUG_ISO_DIR) $(ISO_DIR)


update:
	mkdir -p $(UPDATE_DIR)
	bear -- make update-impl

update-impl:$(addprefix $(UPDATE_DIR)/, $(OBJECTS))
#	ld $(LDPARAMS) -T $(SRC_RIR)/linker.ld -o mykernel.bin $(addprefix $(UPDATE_DIR)/, $(OBJECTS))

$(UPDATE_DIR)/%.o: $(SRC_RIR)/%.cpp | $(RELEASE_DIR)
	gcc $(UPDATEPARAMS) -c -o $@ $<

$(UPDATE_DIR)/%.o: $(SRC_RIR)/%.s | $(RELEASE_DIR)
	as $(ASPARAMS) -o $@ $<

