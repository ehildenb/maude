# Settings
# --------

base_md:=$(wildcard tools/base.md/*.md)
base_maude:=$(patsubst tools/base.md/%.md, tools/base/%.maude, $(base_md))

meta_md:=$(wildcard tools/meta.md/*.md)
meta_maude:=$(patsubst tools/meta.md/%.md, tools/meta/%.maude, $(meta_md))

lmc_md:=$(wildcard tools/lmc.md/*.md)
lmc_maude:=$(patsubst tools/lmc.md/%.md, tools/lmc/%.maude, $(lmc_md))

fvp_md:=$(wildcard tools/fvp.md/*.md)
fvp_maude:=$(patsubst tools/fvp.md/%.md, tools/fvp/%.maude, $(fvp_md))

systems_md:=$(wildcard systems.md/*.md)
systems_maude:=$(patsubst systems.md/%.md, systems/%.maude, $(systems_md))

tangler_repo=$(CURDIR)/pandoc-tangle
tangler=$(tangler_repo)/tangle.lua
LUA_PATH=$(tangler_repo)/?.lua;;
export LUA_PATH

# Main Targets
# ------------

.PHONY: all tools systems clean

all: \
	tangle \
	media

tangle: \
	tools \
	systems

clean:
	rm -rf $(base_maude) $(meta_maude) $(lmc_maude) $(fvp_maude) $(systems_maude)

# Tangler
# -------

$(tangler):
	git submodule update --init -- $(tangler_repo)

# Tangled Files
# -------------

tools: $(base_maude) $(meta_maude) $(lmc_maude) $(fvp_maude)

tools/base/%.maude: tools/base.md/%.md $(tangler)
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

tools/meta/%.maude: tools/meta.md/%.md $(tangler)
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

tools/lmc/%.maude: tools/lmc.md/%.md
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

tools/fvp/%.maude: tools/fvp.md/%.md
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

systems: $(systems_maude)
systems/%.maude: systems.md/%.md $(tangler)
	mkdir -p $(dir $@)
	pandoc --from markdown --to $(tangler) --metadata=code:.maude $< > $@

# Testing
# -------

load_maude:=$(meta_maude) $(lmc_maude) $(fvp_maude) $(systems_maude)
test-load: $(load_maude:=.load)

%.load: %
	./scripts/test-load $*

# Papers and write-ups
# --------------------

media:
media/%.pdf: media/%.md media/citations.bib
	cd media \
	    && cat $(notdir $^) \
	     | pandoc --from markdown --to latex \
	              --table-of-contents --filter pandoc-citeproc \
	              --output $(notdir $@) -

systems.md/bitswap.pdf: systems.md/bitswap.md systems/bitswap.maude Makefile
	PATH=$$(pwd)/../src/Main:$$PATH pandoc --filter panpipe -t latex $< -o $@ --filter pandoc-citeproc
