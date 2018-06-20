main.pdf: $(wildcard *.md)
	pandoc main.md --lua-filter ../../pandoc-tangle/include.lua -o $@ -H macros.tex \
	       --variable geometry:margin=1cm
