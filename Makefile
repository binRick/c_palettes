default: local-all
##############################################################
include submodules/c_deps/etc/includes.mk
##############################################################
PALETTES_UTILS_DIR=$(DIR)/palette-utils
ETC_THEMES_DIR=$(ETC_DIR)/themes
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
local-all: embed-palettes yaml2json all

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
python-venv-yaml2json:
	@[[ -f $(YAML2JSON_BIN) ]] || { $(SOURCE_VENV_CMD) && pip3 install json2yaml; }
ensure: \
	dirs-embeds embed-palettes embed-themes
local-setup: embed-palettes setup
	@clib i
dirs-embeds:
	@mkdir -p $(EMBEDS_DIR)
