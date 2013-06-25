## Makefile for creating html output for plots

all: root rivet

root: plotscript convertpng makehtml
	@./plotscript.sh RT constant
	@./convertpng.sh
	@./makehtml.sh

rivet: plotscript convertpng makehtml
	@./plotscript.sh T3 constant rivet
	@./convertpng.sh rivet
	@./makehtml.sh rivet

plotscript: plotscript.sh
	@chmod u+x plotscript.sh

convertpng: convertpng.sh
	@chmod u+x convertpng.sh

makehtml: makehtml.sh
	@chmod u+x makehtml.sh

clean:
	@rm -r html || :
	@rm -r png || :
	@rm -r dat || :
	@rm -r pdf || :
	@rm -r rivet_html || :
	@rm -r rivet_png || :
	@rm -r rivet_dat || :
	@rm -r rivet_pdf || :
	@echo "Removed all created directories."
