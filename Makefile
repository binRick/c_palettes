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
EMBED_BINARY=$(shell command -v embed)
JQ_BIN=$(shell command -v jq)
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
MENU_DIR=$(DIR)/menu
DOCKER_DIR=$(DIR)/docker
LIST_DIR=$(DIR)/list
SOURCE_VENV_CMD=$(DIR)/scripts
PALETTES_UTILS_DIR=$(DIR)/palette-utils
VENV_DIR=$(DIR)/.venv
ETC_THEMES_DIR=$(ETC_DIR)/themes
SCRIPTS_DIR=$(DIR)/scripts
YAML2JSON_BIN=$(VENV_DIR)/bin/yaml2json
TEST_THEME_YAML_FILE=$(ETC_DIR)/test-theme.yaml
TEST_THEME_JSON_FILE=$(ETC_DIR)/test-theme.json
SOURCE_VENV_CMD = source $(VENV_DIR)/bin/activate
NORMALIZE_KITTY_THEME_CMD = $(SCRIPTS_DIR)/normalize_kitty_theme.sh
NORMALIZED_THEMES_DIR = $(ETC_DIR)/normalized-themes
##############################################################
TIDIED_FILES = \
			   $(LIST_DIR)/*.h $(LIST_DIR)/*.c \
			   $(MENU_DIR)/*.c $(MENU_DIR)/*.h \
			   palette*/*.c palette*/*.h \
			   palettes-test/*.c palettes-test/*.h
##############################################################
CD_LOADER = cd $(LOADER_DIR)
CD_PROJECT = cd $(PROJECT_DIR)
CD_M1 = cd $(M1_DIR)
CD_PALETTE_UTILS_DIR = cd palette-utils
##############################################################
EMBEDDED_PALETTES_DIR = $(EMBEDS_DIR)
EMBEDDED_PALETTES_FILE = $(EMBEDDED_PALETTES_DIR)/tbl1.c
EMBEDDED_PALETTES_LIMIT = 995
EMBEDDED_THEMES_LIMIT = 995
EMBEDDED_THEMES_FILE = $(EMBEDDED_PALETTES_DIR)/themes.c
FIND_PALETTES_CMD = cd $(ETC_DIR)/palettes && find . -type f|sort -u|shuf|head -n $(EMBEDDED_PALETTES_LIMIT)
FIND_THEMES_CMD = find $(DIR)/submodules/themes/mbadolato/iTerm2-Color-Schemes/alacritty -type f -name "*.yml"|sort -u|shuf|head -n $(EMBEDDED_THEMES_LIMIT)
##################################
RANDOM_PALETTE_FILE = $(shell $(FIND_PALETTES_CMD)|shuf|head -n1|gsed 's|^./||'g)
RANDOM_PALETA_PALETTE_FILE = $(shell $(FIND_PALETTES_CMD)|grep ^paleta/|shuf|head -n1|gsed 's|^./||'g)
RANDOM_PALETTE_NAME = $(shell basename $(RANDOM_PALETTE_FILE))
RANDOM_PALETA_PALETTE_NAME = $(shell basename $(RANDOM_PALETA_PALETTE_FILE))
##############################################################
all: embed-palettes yaml2json setup-etc-themes do-palette-utils do-parser

setup-etc-themes: 
	@[[ -d $(ETC_THEMES_DIR) ]] || mkdir -p "$(ETC_THEMES_DIR)"
	@[[ -d $(NORMALIZED_THEMES_DIR) ]] || mkdir -p "$(NORMALIZED_THEMES_DIR)"
	@true


kovidgoyal_kitty-themes-normalize: \
	kovidgoyal_kitty-themes-files-setup \
	kovidgoyal_kitty-themes-files-copy-etc 

#kovidgoyal_kitty-themes:
#	@make kovidgoyal_kitty-themes-files-setup && make kovidgoyal_kitty-themes-files-ls | ./scripts/normalize_kitty_theme.sh etc/normalized-themes/kovindgoyal

kovidgoyal_kitty-themes-files-copy-etc: 
	@find $(ETC_THEMES_DIR)/kovidgoyal -type f -name "*.conf"

kovidgoyal_kitty-themes-files-ls: setup-etc-themes kovidgoyal_kitty-themes-files-setup
	@find submodules/themes/kovidgoyal/kitty-themes/themes -type f -name "*.conf"


kovidgoyal_kitty-themes-files-setup: setup-etc-themes
	@[[ -d $(NORMALIZED_THEMES_DIR)/kovidgoyal ]] || mkdir -p $(NORMALIZED_THEMES_DIR)/kovindgoyal
	@true





#cat submodules/themes/kovidgoyal/kitty-themes/themes/zenburned.conf|grep -v '^#'|tr -s ' '|tr ' ' '='|grep -v '^$'
#cat submodules/themes/rajasegar/alacritty-themes/themes/vscode.dark.yml|yaml2json | jq '.colors'    
#cat submodules/themes/mbadolato/iTerm2-Color-Schemes/kitty/synthwave.conf|grep -v '^#'|tr -s ' '|tr ' ' '
#cat submodules/themes/dexpota/kitty-themes/themes/snazzy.conf|grep -v '^#'|tr -s ' '|tr ' ' '='|grep -v '^$'
r:
	@echo $(RANDOM_PALETTE_FILE)
	@echo $(RANDOM_PALETTE_NAME)
mkdirs:
	@mkdir -p embeds

embed-palettes: mkdirs setup-etc-themes
	@cd $(ETC_DIR)/palettes && sh -c '$(EMBED_BINARY) -o $(EMBEDDED_PALETTES_FILE) -z -t embedded_palettes_table `$(FIND_PALETTES_CMD)`'

json-convert-themes: sync-themes setup-etc-themes
	@true


#	@find etc/themes/*.yml -type f -name "*.yml" | while read -r f; do yaml2json < $$f | jq -Mrc | tee etc/themes/`basename $$f .yml`.json; done | pv -l >/dev/null

do-embed-themes: setup-etc-themes
	@mkdir embeds||true

#	@cd $(ETC_THEMES_DIR) && sh -c '$(EMBED_BINARY) -o $(EMBEDDED_THEMES_FILE) -z -t embedded_themes_table *.json'
#
#
embed-themes: setup-etc-themes sync-themes json-convert-themes do-embed-themes
sync-themes:	
	@rsync submodules/themes/mbadolato/iTerm2-Color-Schemes/alacritty/*.yml $(ETC_THEMES_DIR)/.
	@cd $(ETC_THEMES_DIR) && `for f in *\ *; do mv "$$f" "$${f// /_}"; done`

do-palette-utils-meson: 
	@eval cd . && {  meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; }
do-palette-utils-build:
	@eval cd . && { ninja -C build; }
	@eval cd . && { ninja test -C build -v; }

do-palette-utils: do-palette-utils-meson do-palette-utils-build

do-list-meson: 
	@eval $(CD_M1) && { meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; }
do-list-build: do-list-meson
	@eval $(CD_M1) && { ninja -C build; }
	@eval $(CD_M1) && { ninja test -C build -v; }

do-list: ensure embed-palettes \
	do-list-build \
	do-list-test 

do-palette-utils: ensure embed-palettes \
	do-palette-utils-build \
	do-palette-utils-test 

do-palette-utils-test: 
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

parser-tests:
	@./build/parser/parser -v --help

extended-parser-tests: parser-tests
	@clear
	@./build/parser/parser -v -m args
	@./build/parser/parser -v -m load
	@./build/parser/parser -v -m parse
	@./build/parser/parser -v -m files
	@./build/parser/parser -v -m names
	@./build/parser/parser -v -m hash
	@./build/parser/parser -v -m sync
	@./build/parser/parser -v -m ids
	@./build/parser/parser -v -m db

yaml2json: python-venv python-venv-yaml2json test-yaml2json

test-yaml2json:
	@$(SOURCE_VENV_CMD) && yaml2json $(TEST_THEME_YAML_FILE) $(TEST_THEME_JSON_FILE)
	@jq < $(TEST_THEME_JSON_FILE)
	

python-venv: do-python-venv
do-python-venv:
	@[[ -f $(VENV_DIR)/bin/activate ]] ||  { python3 -m venv $(VENV_DIR) && $(SOURCE_VENV_CMD); }

python-venv-yaml2json:
	@[[ -f $(YAML2JSON_BIN) ]] || { $(SOURCE_VENV_CMD) && pip3 install json2yaml; }


do-parser: parser-tests
tests: do-build parser-tests
test: tests

do-build: do-list do-palette-utils

clean: do-clean
do-clean: 
	@rm -rf $(EMBEDS_DIR) build *.png
	@rm -rf $(VENV_DIR)
	@rm -rf $(TEST_THEME_JSON_FILE)
	@rm -rf $(ETC_THEMES_DIR)
	@rm -rf $(NORMALIZED_THEMES_DIR)

ensure: \
	dirs-embeds embed-palettes embed-themes

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
	@$(PASSH) -L .nodemon.log $(NODEMON) -V -i build -w . -w '*/meson.build' --delay 1 -i '*/subprojects' -I  -w 'include/*.h' -w meson.build -w src -w Makefile -w loader/meson.build -w loader/src -w loader/include -i '*/embeds/*' -e tpl,build,sh,c,h,Makefile -x env -- bash -c 'make||true'


git-pull:
	@git pull --recurse-submodules
git-submodules-pull-master:
	@git submodule foreach git pull origin master --jobs=10
git-submodules-update:
	@git submodule update --init
