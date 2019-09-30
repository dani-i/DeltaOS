####################################################################################################

OS_NAME := DeltaOS

####################################################################################################
#===================================================================================================
# Macros.
#===================================================================================================

_PRINT_SEPARATOR := @echo "================================================================";
_PRINT_NEW_LINE := @echo "";

####################################################################################################

#CC := "/opt/cross/bin/x86_64-elf-gcc"
CC := gcc
AS := nasm

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
			-Wwrite-strings -Wconversion \
			-Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes

CC_FLAGS := -o2 -g -masm=intel $(WARNINGS)
AS_FLAGS := -f elf64
LD_FLAGS := -m elf_x86_64 -nodefaultlibs

####################################################################################################

LINKER_LD := "configs/linker.ld"
GRUB_CFG := "configs/grub.cfg"

####################################################################################################

.PHONY: all boot build clean create_directories iso tasks todolist

####################################################################################################
#===================================================================================================
# Default rule.
#===================================================================================================

all: boot

####################################################################################################
#===================================================================================================
# Directories and files.
#===================================================================================================
# src/
# build/    bin/
#           obj/
#           os/     boot/       grub/       grub.cfg
#                               DeltaOS.bin
# iso/
#===================================================================================================
# Directory paths.
#===================================================================================================

DIR_SRC := ./src/

DIR_BUILD := ./build/
DIR_BIN := $(DIR_BUILD)bin/
DIR_OBJ := $(DIR_BUILD)obj/
DIR_OS := $(DIR_BUILD)os/
DIR_BOOT := $(DIR_OS)boot/
DIR_GRUB := $(DIR_BOOT)grub/

DIR_ISO := ./iso/

#===================================================================================================
# Creates all the needed directories.
#===================================================================================================

create_directories:
	$(_PRINT_SEPARATOR)
	@echo "---- Creating the required directories ...";

	@mkdir -p $(DIR_BIN);
	@mkdir -p $(DIR_OBJ);

	@mkdir -p $(DIR_GRUB);

	@mkdir -p $(DIR_ISO);

	@echo "---------------- Required directories created. ----------------";
	$(_PRINT_SEPARATOR)

####################################################################################################
#===================================================================================================
# Files.
#===================================================================================================

CC_AS := $(shell find . -name "*.c" -o -name "*.h" -o -name "*.asm")

SRC_A_INTERRUPTS := "./src/interrupts.asm"
SRC_A_BOOT := "./src/boot.asm"

SRC_C_INTERRUPTS := "./src/interrupts.c"
SRC_C_KEYBOARD := "./src/keyboard.c"
SRC_C_DISPLAY := "./src/display.c"
SRC_C_KERNEL := "./src/kernel.c"
SRC_C_TIMER := "./src/timer.c"
SRC_C_PIC := "./src/pic.c"

OBJ_A_INTERRUPTS := "./build/obj/a_interruptsa.o"
OBJ_A_BOOT := "./build/obj/a_boot.o"

OBJ_C_INTERRUPTS := "./build/obj/c_interrupts.o"
OBJ_C_KEYBOARD := "./build/obj/c_keyboard.o"
OBJ_C_DISPLAY := "./build/obj/c_display.o"
OBJ_C_KERNEL := "./build/obj/c_kernel.o"
OBJ_C_TIMER := "./build/obj/c_timer.o"
OBJ_C_PIC := "./build/obj/c_pic.o"

OBJS :=  \
	$(OBJ_A_INTERRUPTS) \
	$(OBJ_A_BOOT) \
	$(OBJ_C_INTERRUPTS) \
	$(OBJ_C_KEYBOARD) \
	$(OBJ_C_DISPLAY) \
	$(OBJ_C_KERNEL) \
	$(OBJ_C_TIMER) \
	$(OBJ_C_PIC)

FILE_BIN := $(DIR_BIN)$(OS_NAME).bin
FILE_ISO := $(DIR_ISO)$(OS_NAME).iso

####################################################################################################
#===================================================================================================
# Complies and links the OS.
#===================================================================================================

build: | clean create_directories
	$(_PRINT_SEPARATOR)
	@echo "---- Compiling ...";

	@$(AS) $(AS_FLAGS) $(SRC_A_INTERRUPTS) -o $(OBJ_A_INTERRUPTS);
	@$(AS) $(AS_FLAGS) $(SRC_A_BOOT) -o $(OBJ_A_BOOT);

	@$(CC) $(CC_FLAGS) -I $(DIR_SRC) -c $(SRC_C_INTERRUPTS) -o $(OBJ_C_INTERRUPTS);
	@$(CC) $(CC_FLAGS) -I $(DIR_SRC) -c $(SRC_C_KEYBOARD) -o $(OBJ_C_KEYBOARD);
	@$(CC) $(CC_FLAGS) -I $(DIR_SRC) -c $(SRC_C_DISPLAY) -o $(OBJ_C_DISPLAY);
	@$(CC) $(CC_FLAGS) -I $(DIR_SRC) -c $(SRC_C_KERNEL) -o $(OBJ_C_KERNEL);
	@$(CC) $(CC_FLAGS) -I $(DIR_SRC) -c $(SRC_C_TIMER) -o $(OBJ_C_TIMER);
	@$(CC) $(CC_FLAGS) -I $(DIR_SRC) -c $(SRC_C_PIC) -o $(OBJ_C_PIC);

	@echo "---- Linking ...";
	@$(LD) $(OBJS) -n -T $(LINKER_LD) -o $(FILE_BIN);

	@echo "---------------- Build finished. ----------------"
	$(_PRINT_SEPARATOR)

#===================================================================================================
# Creates the ISO file.
#===================================================================================================

iso: | build
	$(_PRINT_SEPARATOR)
	@echo "---- Copying files ...";

	@cp $(FILE_BIN) $(DIR_BOOT);
	@cp $(GRUB_CFG) $(DIR_GRUB);

	@echo "---- Creating the iso file ...";
	@grub-mkrescue -o $(FILE_ISO) $(DIR_OS);

	@echo "---------------- ISO file created. ----------------";
	$(_PRINT_SEPARATOR)

#===================================================================================================
# Boots the operating system.
#===================================================================================================

boot: | iso
	$(_PRINT_SEPARATOR)
	@echo "---- Booting the OS ..."

	@qemu-system-x86_64 -cdrom $(FILE_ISO) -m 1536M -monitor stdio -net none

	@echo ""
	@echo "---------------- OS closed. ----------------";
	$(_PRINT_SEPARATOR)

#===================================================================================================
# Cleans the project directories.
#===================================================================================================

clean:
	$(_PRINT_SEPARATOR)
	@echo "---- Cleaning ...";

	@# Build directory.
	@if [ -d $(DIR_BUILD) ]; then \
		echo "-------- Build directory deleted."; rm -rf $(DIR_BUILD); \
	 else \
		echo "-------- Nothing to do, build directory not present."; fi

	@# ISO directory.
	@if [ -d $(DIR_ISO) ]; then \
		echo "-------- ISO directory deleted."; rm -rf $(DIR_ISO); \
	 else \
		echo "-------- Nothing to do, ISO directory not present."; fi

	@# OS directory.
	@if [ -d $(DIR_OS) ]; then \
		echo "-------- OS directory deleted."; rm -rf $(DIR_OS); \
	 else \
		echo "-------- Nothing to do, OS directory not present."; fi

	@echo "---------------- Cleaning finished. ----------------";
	$(_PRINT_SEPARATOR)

#===================================================================================================
# Displays all the issues (FIXME) and tasks (TODO).
#===================================================================================================

tasks:
	$(_PRINT_SEPARATOR)
	@echo "---- Issues that need to be fixed.";

	@-for file in $(CC_AS:Makefile=); do \
		fgrep -H -e FIXME $$file; done; true 

	@echo "---- Tasks that need work.";

	@-for file in $(CC_AS:Makefile=); do \
		fgrep -H -e TODO $$file; done; true

	$(_PRINT_SEPARATOR)

####################################################################################################
