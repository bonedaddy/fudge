libs/%.o: CFLAGS+=-Ilibs

include libs/arch/rules.mk
include libs/fudge/rules.mk
include libs/posix/rules.mk
include libs/kernel/rules.mk
include libs/mboot/rules.mk
include libs/$(ARCH)/rules.mk
