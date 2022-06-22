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
all: embed-palettes yaml2json do-build do-test

setup-etc-themes: 
	@[[ -d $(ETC_THEMES_DIR) ]] || mkdir -p "$(ETC_THEMES_DIR)"
	@[[ -d $(NORMALIZED_THEMES_DIR) ]] || mkdir -p "$(NORMALIZED_THEMES_DIR)"
	@true


mkdirs:
	@[[ -d embeds ]] || mkdir -p embeds
	@true

embed-palettes: mkdirs setup-etc-themes
	@cd $(ETC_DIR)/palettes && sh -c '$(EMBED_BINARY) -o $(EMBEDDED_PALETTES_FILE) -z -t embedded_palettes_table `$(FIND_PALETTES_CMD)`'

json-convert-themes: sync-themes setup-etc-themes
	@true

do-embed-themes: setup-etc-themes
	@[[ -d embeds ]] || mkdir embeds
	@true

embed-themes: setup-etc-themes sync-themes json-convert-themes do-embed-themes
sync-themes:	
	@rsync submodules/themes/mbadolato/iTerm2-Color-Schemes/alacritty/*.yml $(ETC_THEMES_DIR)/.
	@cd $(ETC_THEMES_DIR) && `for f in *\ *; do mv "$$f" "$${f// /_}"; done`
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
install: do-install
do-install: all
	@meson install -C build
do-meson:
	@eval cd . && {  meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; echo MESON OK; }
do-build: do-meson
	@meson compile -C build
do-test:
	@passh meson test -C build -v
clean: do-clean
do-clean: 
	@rm -rf $(EMBEDS_DIR) build *.png
	@rm -rf $(TEST_THEME_JSON_FILE)
	@rm -rf $(ETC_THEMES_DIR)
	@rm -rf $(NORMALIZED_THEMES_DIR)
do-clean-venv:	
	@rm -rf $(VENV_DIR)
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
