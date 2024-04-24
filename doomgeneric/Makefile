################################################################
#
# $Id:$
#
# $Log:$
#
USE_TOOLCHAIN:=1

ifdef USE_TOOLCHAIN
#	@echo using toolchain
	TOOLCHAIN_PATH:=../../../../toolchain/cross
#	@echo adding to path $(CURDIR)/$(TOOLCHAIN_PATH)/bin:$(PATH)
	export PATH := $(CURDIR)/$(TOOLCHAIN_PATH)/bin:$(PATH)

	TC_PREFIX:=smith-

	CC:=$(TC_PREFIX)gcc
	CXX:=$(TC_PREFIX)g++
	LD:=$(TC_PREFIX)ld
	AS:=nasm
#nasm
#as -msyntax=intel -mnaked-reg
else

	CC:=gcc
	CXX:=g++
	LD:=ld
	AS:=nasm

endif

VB=''


#-ffreestanding -nostdlib-ffreestanding -nostdlib
CFLAGS+=-fno-stack-protector -mno-shstk -mno-red-zone -fno-pie -no-pie -nostdlib -ffreestanding -fpermissive
LDFLAGS+=-L../../../../libd/ 
CFLAGS+=-D_DEFAULT_SOURCE # -DUSEASM
LIBS=-ld

# subdirectory for objects
OBJDIR=build
OUTPUT=doomos

SRC_DOOM = dummy.o am_map.o doomdef.o doomstat.o dstrings.o d_event.o d_items.o d_iwad.o d_loop.o d_main.o d_mode.o d_net.o f_finale.o f_wipe.o g_game.o hu_lib.o hu_stuff.o info.o i_cdmus.o i_endoom.o i_joystick.o i_scale.o i_sound.o i_system.o i_timer.o memio.o m_argv.o m_bbox.o m_cheat.o m_config.o m_controls.o m_fixed.o m_menu.o m_misc.o m_random.o p_ceilng.o p_doors.o p_enemy.o p_floor.o p_inter.o p_lights.o p_map.o p_maputl.o p_mobj.o p_plats.o p_pspr.o p_saveg.o p_setup.o p_sight.o p_spec.o p_switch.o p_telept.o p_tick.o p_user.o r_bsp.o r_data.o r_draw.o r_main.o r_plane.o r_segs.o r_sky.o r_things.o sha1.o sounds.o statdump.o st_lib.o st_stuff.o s_sound.o tables.o v_video.o wi_stuff.o w_checksum.o w_file.o w_main.o w_wad.o z_zone.o w_file_stdc.o w_file_dls.o i_input.o i_video.o doomgeneric.o 
OBJS += $(addprefix $(OBJDIR)/, $(SRC_DOOM))

all:	 $(OUTPUT)

install: all
	cp $(OUTPUT) /home/dylan/code/randos/util/initrd/root/usr/bin
	cp $(OUTPUT) /home/dylan/code/randos/dbg

clean:
	rm -rf $(OBJDIR)
	rm -f $(OUTPUT)
	rm -f $(OUTPUT).gdb
	rm -f $(OUTPUT).map

$(OUTPUT):	$(OBJS)
	@echo [Linking $@]
#@echo $(VB)$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS) -o $(OUTPUT) -Wl,-Map,$(OUTPUT).map
	$(CC) $(CFLAGS)  -o $(OUTPUT) $(OBJS) -L$(CURDIR) -ld
#-Wl,-Map,$(OUTPUT).map
	@echo [Size]
	-$(CROSS_COMPILE)size $(OUTPUT)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o:	%.c
	@echo [Compiling $<]
	$(VB)$(CC) $(CFLAGS) -I/home/dylan/code/randos/libd  -L$(CURDIR) -ld -c $< -o $@

print:
	@echo OBJS: $(OBJS)

