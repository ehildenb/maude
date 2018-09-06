# Settings
# --------

meta_names:=cterms           eqform      foform     mconstruction \
            metalevel-ext    mtemplate   mtransform narrowing     \
            structured-names unification variables
meta_files:=$(patsubst %, tools/meta/%.maude, $(meta_names))

lmc_names:=lgraph-search lmc
lmc_files:=$(patsubst %, tools/lmc/%.maude, $(lmc_names))

system_names:=bakery     bank-account bitswap  caut \
              choice     cycle        dijkstra imp  \
              nim        oc           qlock    rw   \
              thermostat token        vending
system_files:=$(patsubst %, systems/%.maude, $(system_names))

system_tests:=../tests/systems/bitswap.maude

tangler_repo=$(CURDIR)/pandoc-tangle
tangler=$(tangler_repo)/tangle.lua
LUA_PATH=$(tangler_repo)/?.lua;;
export LUA_PATH

# Main Targets
# ------------

.PHONY: all tools systems clean

all: \
	tools \
	systems

clean:
	rm -rf $(meta_files) $(system_files)

# Tangler
# -------

$(tangler):
	git submodule update --init -- $(tangler_repo)

# Tangled Files
# -------------

tools: $(meta_files) $(lmc_files)
tools/meta/%.maude: tools/meta.md/%.md $(tangler)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

tools/lmc/%.maude: tools/lmc.md/%.md
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

systems: $(system_files) $(system_tests)
systems/%.maude: systems.md/%.md $(tangler)
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

../tests/systems/%.maude: systems.md/%.md $(tangler)
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.test $< > $@

# Papers and write-ups
# --------------------

systems.md/bitswap.pdf: systems.md/bitswap.md systems/bitswap.maude Makefile
	PATH=$$(pwd)/../src/Main:$$PATH pandoc --filter panpipe -t latex $< -o $@ --filter pandoc-citeproc
