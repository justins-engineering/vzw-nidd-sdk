CC ?= gcc
DIR ?= .
EXTRA_CFLAGS ?=
EXTRA_LDFLAGS ?=
INSTALL ?= cp -pPR

PREFIX ?= /usr/local
INCLUDE_PATH ?= include
LIBRARY_PATH ?= lib
INSTALL_INCLUDE_PATH = $(PREFIX)/$(INCLUDE_PATH)
INSTALL_LIBRARY_PATH = $(PREFIX)/$(LIBRARY_PATH)

MODULE_PATH = $(DIR)/modules

OPTIMIZATION ?= -O3
WARNINGS = -Wall -Wextra -Wformat -Winline -Werror=format-security -Wstrict-prototypes -Wwrite-strings \
	-Wno-missing-field-initializers -pedantic

CFLAGS ?= -fstack-protector-strong -fanalyzer
LDFLAGS ?= -Wl,-z,relro -Wl,-z,now -Wl,--as-needed

NATIVE ?= 1
ifeq ($(NATIVE), 1)
  arch = -march=native -mtune=native
endif

DEBUG ?= 0
ifeq ($(DEBUG), 1)
EXTRA_CFLAGS += -g -ggdb -fno-omit-frame-pointer
MODULE_CFLAGS = -g -ggdb -fno-omit-frame-pointer
else
EXTRA_CFLAGS += -D_FORTIFY_SOURCE=2
MODULE_CFLAGS = -D_FORTIFY_SOURCE=2 -fstack-protector-strong
endif

MODULE_CFLAGS += $(OPTIMIZATION) $(arch)

linked_libs = -lc -lcurl

CFLAGS += $(OPTIMIZATION) -fPIC $(WARNINGS) $(EXTRA_CFLAGS) $(arch) -I$(DIR)/$(INCLUDE_PATH)
LDFLAGS += -L$(DIR) $(linked_libs) $(EXTRA_LDFLAGS)

subdirs = $(addprefix $(DIR), /json /vzw /curl)

VPATH = $(DIR) $(subdirs) $(DIR)/$(INCLUDE_PATH)

objects = jsmn.o json_helpers.o helpers.o vzw_connect.o credentials.o base64.o

all: libvznidd.so libvznidd.a

$(DIR)/$(INCLUDE_PATH)/jsmn.h:
	$(INSTALL) $(MODULE_PATH)/jsmn/jsmn.h $(DIR)/$(INCLUDE_PATH)

$(DIR)/$(INCLUDE_PATH)/base64.h:
	$(INSTALL) $(MODULE_PATH)/nibble-and-a-half/base64.h $(DIR)/$(INCLUDE_PATH)

$(DIR)/base64.c:
	$(INSTALL) $(MODULE_PATH)/nibble-and-a-half/base64.c $(DIR)

libvznidd.so: $(addprefix $(DIR)/$(INCLUDE_PATH)/, jsmn.h base64.h) $(objects)
	$(CC) -shared $(CFLAGS) $(objects) -o $(DIR)/libvznidd.so $(LDFLAGS)

libvznidd.a: $(addprefix $(DIR)/$(INCLUDE_PATH)/, jsmn.h base64.h) $(objects)
	$(AR) rcs --record-libdeps="$(linked_libs)" libvznidd.a $(objects)

sample: include/vzw_secrets.h $(addprefix $(DIR)/$(INCLUDE_PATH)/, jsmn.h base64.h) $(objects)
	$(CC) $(CFLAGS) $(objects) -o $(DIR)/sample $(LDFLAGS)
	chmod +x $(DIR)/sample

install:
	$(INSTALL) libvznidd.a $(INSTALL_LIBRARY_PATH)/libvznidd.a
	$(INSTALL) libvznidd.so $(INSTALL_LIBRARY_PATH)/libvznidd.so

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf libvznidd.o libvznidd.a libvznidd.so $(DIR)/sample $(objects) \
		$(DIR)/include/jsmn.h $(DIR)/include/base64.h $(DIR)/base64.c

.PHONY: clean install
