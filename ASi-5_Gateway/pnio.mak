#---------------------------------------------------------------------#
#                                                                     #
#                        Bihl+Wiedemann GmbH                          #
#                                                                     #
#                                                                     #
#        project: Control_III                                         #
#    module name: enip.mak                                            #
#         author: Christian Sommerfeld                                #
#           date: 2015-02-26                                          #
#                                                                     #
#    version: 1.0 first version                                       #
#             1.1 added compiler switches -o                          #
#             1.2 added new compiler switches                         #
#                 for size and thumb mode                             #
#             1.3 msys2 added 				                          #
#                                                                     #
#                                                                     #
#---------------------------------------------------------------------#

export PATH  := ../../Compiler/altera/mentor/gnu/arm/baremetal/bin:../../Compiler/Tools:../../Compiler/msys32/usr/bin

TARGET        = control
DEBUG=0


#---------------------------------------------------------------------#
#   stuff to be executed unconditionally                              #
#---------------------------------------------------------------------#

ifeq ($(MAKELEVEL),0)
	DUMMY:=$(shell echo -en "" \
			"-----------------------\n" \
			" $(TARGET) -- Release  \n" \
			"-----------------------\n" \
			"\n" \
			>&2)

	OUTDIR=.release
	FIND= ../../Compiler/Tools/find.exe
	YHEX= ../../Compiler/Tools/yhex.exe
	
	export OUTDIR
	DUMMY:=$(shell if ! [ -d $(OUTDIR) ]; then mkdir $(OUTDIR); fi)

endif

#---------------------------------------------------------------------#
#   macros                                                            #
#---------------------------------------------------------------------#

#-- sources ----------------------------------------------------------#

SRC =	startup.c \
		main.c	\
		control_io.c

#-- command line flags -----------------------------------------------#

CFLAGS   = \
	-mcpu=arm9tdmi \
	-Wall \
	-Wpointer-arith \
	-Wstrict-prototypes \
	-Winline -Wundef \
	-g \
	-I. \
	-fno-schedule-insns \
	-ffunction-sections \
	-fdata-sections \
	-fno-exceptions \
	-ansi \
	-std=gnu99 \
	-DSIGNATURE=4444 \
	-MD \
	-MP \
	-MF $(OUTDIR)/$(notdir $(<:.c=.d))

ifeq ($(strip $(DEBUG)),0)
	CFLAGS += -O0
else 
ifeq ($(strip $(DEBUG)),1)
	CFLAGS += -Os
else 
ifeq ($(strip $(DEBUG)),2)
	CFLAGS += -O3
else 
	CFLAGS += -O3
endif
endif
endif

LDFLAGS  = \
	-Wl,-Map=$(OUTDIR)/$(TARGET).map,--cref \
	-mcpu=arm9tdmi \
	-Wl,--gc-sections \
	-Wl,-static \
	-Tpnio.ld

#-- tools ------------------------------------------------------------#

CC       = arm-altera-eabi-gcc
OBJCOPY  = arm-altera-eabi-objcopy -S -g
OBJDUMP  = arm-altera-eabi-objdump
SIZE_C	 = arm-altera-eabi-size
SIZE     = ../../Compiler/Tools/calc_altera_eabi.sh
ROMSIZE  = 0x40000
RAMSIZE  = 0x40000
NM       = arm-altera-eabi-nm
REMOVE   = rm -f
COPY     = cp
SHELL    = sh


#---------------------------------------------------------------------#

OBJ = $(addprefix $(OUTDIR)/,$(notdir $(SRC:.c=.o) ))

.PRECIOUS: $(OBJ)


#---------------------------------------------------------------------#
#   pattern rules                                                     #
#---------------------------------------------------------------------#

# create object files from C source files.

$(OUTDIR)/%.o : %.c
	@echo -e "\n$< ..."
	$(CC) -c $(CFLAGS) $< -o $@

# create ELF output file from object files.

%.elf: $(OBJ)
	#@echo -e "\nwriting $@ ..."
	$(CC) $(CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

# create final output file from ELF output file

%.bin: %.elf
	#@echo -e "\nwriting $@ ..."
	$(OBJCOPY) -O binary -R .funcarea $< $@


%.bin: $(OUTDIR)/%.bin
	#@echo "writing $@ ..."
	$(YHEX) --verbosity=2 --overwrite-query=off \
		--input=$< --format=bin \
		--procedure=sum:method=sum16le --range=0,$$((($$($(FIND) "$<" -printf "%s") + 1 ) | 1)) \
		--output=$@ --format=bin

# create a symbol table from ELF output file.

%.sym: %.elf
	@echo -e "\nwriting $@ ..."
	$(NM) -n $< > $@

# create extended listing file from ELF output file.

%.lss: %.elf
	@echo -e "\nwriting $@ ..."
	$(OBJDUMP) -h -S $< > $@

#---------------------------------------------------------------------#
#   description blocks                                                #
#---------------------------------------------------------------------#

.SECONDARY:

#-- default target ---------------------------------------------------#

.PHONY: all
all: ctrl bin sym show_size lss

#-- clean target -----------------------------------------------------#

.PHONY: clean
clean:
	@echo -e "\ncleaning up ..."
	rm -rf $(OUTDIR)/
	rm -rf $(TARGET).bin
	@echo -e "\ncleaning done ..."

#-- secondary targets ------------------------------------------------#

.PHONY: show_size
show_size:
	$(SIZE) $(OUTDIR)/$(TARGET).elf $(ROMSIZE) $(RAMSIZE) $(SIZE_C); 
	#@if [ -f $(OUTDIR)/$(TARGET).elf ]; then echo; $(SIZE) -A $(OUTDIR)/$(TARGET).elf; echo; fi

.PHONY: ctrl
bin:  $(TARGET).bin

.PHONY: bin
bin:  $(OUTDIR)/$(TARGET).bin

.PHONY: sym
sym:  $(OUTDIR)/$(TARGET).sym

.PHONY: lss
lss:  $(OUTDIR)/$(TARGET).lss

.PHONY: elf
lss:  $(OUTDIR)/$(TARGET).elf

.PHONY: everything
everything: bin mot lss xlist sym show_size

#---------------------------------------------------------------------#
#   invocation file creation                                          #
#---------------------------------------------------------------------#

#---------------------------------------------------------------------#
#   explicit dependents                                               #
#---------------------------------------------------------------------#

-include $(wildcard $(OUTDIR)/*.d) dummy.d

#---------------------------------------------------------------------#
#   eof                                                               #
#---------------------------------------------------------------------#
