# Settings
# --------

meta_names:=cterms            foform      mconstruction metalevel-ext            \
            mtemplate         mtransform  narrowing     nelson-oppen-combination \
            structured-names  unification variables
meta_files:=$(patsubst %, tools/meta/%.maude, $(meta_names))

system_names:=bakery bank-account caut choice \
              cycle  dijkstra     imp  nim \
              oc     qlock        rw   thermostat \
              token  vending
system_tests:=nelson-oppen/integer-list
system_files:=$(patsubst %, systems/%.maude,          $(system_names)) \
              $(patsubst %, ../tests/systems/%.maude, $(system_tests))

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

tools: $(meta_files)
tools/meta/%.maude: tools/meta.md/%.md $(tangler)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

systems: $(system_files)
systems/%.maude: systems.md/%.md $(tangler)
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@
../tests/systems/%.maude: systems.md/%.md $(tangler)
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.test $< > $@
