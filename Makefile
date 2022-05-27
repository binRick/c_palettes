
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
FIND_PALETTES_CMD = cd $(ETC_DIR)/palettes && find . -type f|sort -u|shuf|head -n $(EMBEDDED_PALETTES_LIMIT)
##################################
RANDOM_PALETTE_FILE = $(shell $(FIND_PALETTES_CMD)|shuf|head -n1|gsed 's|^./||'g)
RANDOM_PALETA_PALETTE_FILE = $(shell $(FIND_PALETTES_CMD)|grep ^paleta/|shuf|head -n1|gsed 's|^./||'g)
RANDOM_PALETTE_NAME = $(shell basename $(RANDOM_PALETTE_FILE))
RANDOM_PALETA_PALETTE_NAME = $(shell basename $(RANDOM_PALETA_PALETTE_FILE))

##############################################################
r:
	@echo $(RANDOM_PALETTE_FILE)
	@echo $(RANDOM_PALETTE_NAME)

embed-palettes: dirs-embeds
	@cd $(ETC_DIR)/palettes && sh -c '~/repos/c_embed/embed/build/embed -o $(EMBEDDED_PALETTES_FILE) -z -t embedded_palettes_table `$(FIND_PALETTES_CMD)`'

do-list-build: do-list-meson
	@eval $(CD_M1) && { ninja -C build; }

	@eval $(CD_M1) && { ninja test -C build -v; }

do-list-meson: 
	@eval $(CD_M1) && { meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; }

do-list: ensure embed-palettes \
	do-list-build \
	do-list-test 

do-list-test: \
	do-list-test-base \
	do-list-test-stats \
	do-list-test-names \
	do-list-test-files \
	do-list-test-paleta \
	do-list-test-restore

do-list-test-base:
	@clear
	@ansi --save-palette
do-list-test-restore:
	@eval ansi --restore-palette
do-list-test-kfc:
	@$(BUILD_DIR)/list/list --mode=palette-file-content  --file=kfc/dark/vscode
	@echo "VSCODE" && sleep 2
do-list-test-paleta:
	@$(BUILD_DIR)/list/list --mode=palette-file-content  --file=paleta/vscode|paleta
	@echo "VSCODE" && sleep 2
do-list-test-stats:
	@$(BUILD_DIR)/list/list --mode=load  --verbose
	@$(BUILD_DIR)/list/list --mode=parse --verbose
	@$(BUILD_DIR)/list/list --mode=palette-files
	@$(BUILD_DIR)/list/list --mode=palette-names
do-list-test-names:
	@$(BUILD_DIR)/list/list --mode=palette-name-exists --palette=$(RANDOM_PALETTE_NAME)
	@$(BUILD_DIR)/list/list --mode=palette-name-exists-qty --palette=$(RANDOM_PALETTE_NAME)
	@$(BUILD_DIR)/list/list --mode=palette-name-exists-qty --palette=BAD_PALETTE_NAME
	@$(BUILD_DIR)/list/list --mode=palette-name-files --palette=$(RANDOM_PALETTE_NAME)
	@$(BUILD_DIR)/list/list --mode=palette-name-files --palette=vscode
	@$(BUILD_DIR)/list/list --mode=palette-name-files --palette=BAD_PALETTE_NAME
do-list-test-files:
	@$(BUILD_DIR)/list/list --mode=palette-file-exists --file=$(RANDOM_PALETTE_FILE)
	@$(BUILD_DIR)/list/list --mode=palette-file-exists --file=./$(RANDOM_PALETTE_FILE)
	@$(BUILD_DIR)/list/list --mode=palette-file-content --palette=$(RANDOM_PALETTE_FILE)

#	$(BUILD_DIR)/list/list --mode=palette-file-content --palette=BAD_PALETTE_NAME
#	@$(BUILD_DIR)/list/list --mode=colors --palette=vscode

list-test: do-list do-list-test

tests: list-test
test: tests

do-build: do-list
all: list-test


clean: 
	@rm -rf $(EMBEDS_DIR) build *.png

ensure: dirs-embeds embed-palettes

setup: embed-palettes
	@clib i

dirs-embeds:
	@mkdir -p $(EMBEDS_DIR)



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

dev: nodemon

dev-loader:
	@$(PASSH) -L .nodemon.log $(NODEMON) -w '*/meson.build' --delay 1 -i '*/subprojects' -I  -w 'include/*.h' -w meson.build -w src -w Makefile -w loader/meson.build -w loader/src -w loader/include -i '*/embeds/*' -e tpl,build,sh,c,h,Makefile -x env -- bash -c 'make do-loader||true'

nodemon:
	@$(PASSH) -L .nodemon.log $(NODEMON) -w . -w '*/meson.build' --delay 1 -i '*/subprojects' -I  -w 'include/*.h' -w meson.build -w src -w Makefile -w loader/meson.build -w loader/src -w loader/include -i '*/embeds/*' -e tpl,build,sh,c,h,Makefile -x env -- bash -c 'make test||true'


