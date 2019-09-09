M:=\
    $(DIR_SRC)/modules/arch/x86/vbe/vbe.ko.0 \

O:=\
    $(DIR_SRC)/modules/arch/x86/vbe/main.o \

L:=\
    $(DIR_LIB)/fudge/fudge.a \

include $(DIR_MK)/kmod.mk
