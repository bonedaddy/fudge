M:=\
    $(DIR_SRC)/modules/udp/udp.ko \

N:=\
    $(DIR_SRC)/modules/udp/udp.ko.map \

O:=\
    $(DIR_SRC)/modules/udp/main.o \

L:=\
    $(DIR_LIB)/fudge/fudge.a \

include $(DIR_MK)/kmod.mk
