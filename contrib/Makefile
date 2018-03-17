.PHONY: all tools systems

all: \
	tools \
	systems

# Tangler
# -------

tangler_repo=$(CURDIR)/pandoc-tangle
tangler=$(tangler_repo)/tangle.lua
LUA_PATH=$(tangler_repo)/?.lua;;
export LUA_PATH

$(tangler):
	git submodule update --init -- $(tangler_repo)

# Tangled Files
# -------------

tools: \
	tools/meta/metalevel-ext.maude \
	tools/meta/structured-names.maude \
	tools/meta/cterms.maude \
	tools/meta/foform.maude \
	tools/meta/mtemplate.maude \
	tools/meta/mtransform.maude \
	tools/meta/mconstruction.maude \
	tools/meta/unification.maude \
	tools/meta/variables.maude \
	tools/meta/narrowing.maude

systems: \
	systems/nim.maude \
	systems/caut.maude

tools/meta/%.maude: tools/meta.md/%.md $(tangler)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

systems/%.maude: systems.md/%.md $(tangler)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@
