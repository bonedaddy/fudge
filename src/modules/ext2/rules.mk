M:=\
    $(DIR_SRC)/modules/ext2/ext2.ko \

N:=\
    $(DIR_SRC)/modules/ext2/ext2.ko.map \

O:=\
    $(DIR_SRC)/modules/ext2/main.o \
    $(DIR_SRC)/modules/ext2/filesystem.o \
    $(DIR_SRC)/modules/ext2/protocol.o \

L:=\
    $(DIR_SRC)/fudge/fudge.a \

include $(DIR_MK)/kmod.mk
