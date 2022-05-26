
default: all
##############################################################
PASSH=$(shell command -v passh)
GIT=$(shell command -v git)
SED=$(shell command -v gsed||command -v sed)
NODEMON=$(shell command -v nodemon)
FZF=$(shell command -v fzf)
BLINE=$(shell command -v bline)
UNCRUSTIFY=$(shell command -v uncrustify)
PWD=$(shell command -v pwd)
FIND=$(shell command -v find)
##############################################################
DIR=$(shell $(PWD))
M1_DIR=$(DIR)
LOADER_DIR=$(DIR)/loader
EMBEDS_DIR=$(DIR)/embeds
VENDOR_DIR=$(DIR)/vendor
PROJECT_DIR=$(DIR)
MESON_DEPS_DIR=$(DIR)/meson/deps
VENDOR_DIR=$(DIR)/vendor
DEPS_DIR=$(DIR)/deps
BUILD_DIR=$(DIR)/build
ETC_DIR=$(DIR)/etc
DOCKER_DIR=$(DIR)/docker
LIST_DIR=$(DIR)/list
##############################################################
TIDIED_FILES = \
			   $(LIST_DIR)/*.h \
			   $(LIST_DIR)/*.c
##############################################################
CD_LOADER = cd $(LOADER_DIR)
CD_PROJECT = cd $(PROJECT_DIR)
CD_M1 = cd $(M1_DIR)
##############################################################
EMBEDDED_PALETTES_DIR = $(EMBEDS_DIR)
EMBEDDED_PALETTES_FILE = $(EMBEDDED_PALETTES_DIR)/tbl1.c
EMBEDDED_PALETTES_LIMIT = 10000
##############################################################
embed-palettes: dirs-embeds
	@eval ~/repos/c_embed/embed/build/embed -o $(EMBEDDED_PALETTES_FILE) -z -t embedded_palettes_table  $(shell find $(ETC_DIR)/palettes -type f|sort -u|shuf|head -n $(EMBEDDED_PALETTES_LIMIT))

test-list:
	@$(BUILD_DIR)/list/list palettes

tests: embed-palettes test-list


all: ensure do-list test-list
	@make do-m1

do-build: do-m1-test

MESON_DIRS = m1

clean: 
	@rm -rf $(EMBEDS_DIR) build

ensure: dirs-embeds embed-palettes

setup: embed-palettes
	@clib i

dirs-embeds:
	@mkdir -p $(EMBEDS_DIR)

do-list: ensure embed-palettes do-list-test

do-list-meson: 
	@eval $(CD_M1) && { meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; }

do-list-build: do-list-meson
	@eval $(CD_M1) && { ninja -C build; }

do-list-test: do-list-build
	@eval $(CD_M1) && { ninja test -C build -v; }
do-list: ensure do-list-test

test: do-list-test

uncrustify:
	@$(UNCRUSTIFY) -c etc/uncrustify.cfg --replace $(TIDIED_FILES) 
	@shfmt -w scripts/*.sh

uncrustify-clean:
	@find  . -type f -name "*unc-back*"|xargs -I % unlink %

fix-dbg:
	@$(SED) 's|, % s);|, %s);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % lu);|, %lu);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % d);|, %d);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % zu);|, %zu);|g' -i $(TIDIED_FILES)

tidy: uncrustify uncrustify-clean fix-dbg

dev-all: all

pull:
	@git pull

dev-clean: clean dev

dev: pull tidy nodemon

dev-loader:
	@$(PASSH) -L .nodemon.log $(NODEMON) -w '*/meson.build' --delay 1 -i '*/subprojects' -I  -w 'include/*.h' -w meson.build -w src -w Makefile -w loader/meson.build -w loader/src -w loader/include -i '*/embeds/*' -e tpl,build,sh,c,h,Makefile -x env -- bash -c 'make do-loader||true'

nodemon:
	@$(PASSH) -L .nodemon.log $(NODEMON) -w '*/meson.build' --delay 1 -i '*/subprojects' -I  -w 'include/*.h' -w meson.build -w src -w Makefile -w loader/meson.build -w loader/src -w loader/include -i '*/embeds/*' -e tpl,build,sh,c,h,Makefile -x env -- bash -c 'make dev-all||true'


