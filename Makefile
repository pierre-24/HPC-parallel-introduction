all: help

help:
	@echo "Please use \`make <target>' where <target> is one of"
	@echo "  clean      to clean all LaTeX auxiliary files"
	@echo "  help       to get this help"

clean:
	rm -f *.aux *.log *.out *.thm *.toc *.glg *.glo *.gls *.glsdefs *.ist *.gz *.bbl *.blg
	rm -f *converted-to*

