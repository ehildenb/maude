main.pdf main.tex gen.md: $(wildcard *.md) cite.bib template.latex
	pandoc main.md -o $@ -H macros.tex --template template.latex        \
	       --lua-filter ../../pandoc-tangle/include.lua         \
	       --lua-filter ../../pandoc-tangle/njr-thesis.lua      \
	       --filter pandoc-citeproc --bibliography=cite.bib     \
	       --pdf-engine=xelatex --top-level-division=chapter
