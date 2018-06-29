main.pdf: $(wildcard *.md)
	pandoc main.md -o $@ -H macros.tex                          \
	       --lua-filter ../../pandoc-tangle/include.lua         \
	       --lua-filter ../../pandoc-tangle/njr-thesis.lua      \
	       --filter pandoc-citeproc --bibliography=cite.bib     \
	       --variable geometry:margin=1cm
