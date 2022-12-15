SHELL = /bin/bash
TARGET = sildeco
TEMPLATEFILE = IEEEtran.cls
BIBFILE = IEEEabrv.bib abrv.bib bioman.bib bioeda.bib other.bib
FIGDIR = Fig
#CONFPAPER=06105314.pdf

TEX_SRC = $(wildcard *.tex) 
EPS = $(patsubst %.jpg,%.eps,$(wildcard $(FIGDIR)/*.jpg))\
      $(patsubst %.asy,%.eps,$(wildcard $(FIGDIR)/*.asy))
PSLATEX = $(patsubst %.fig,%.pstex_t,$(wildcard $(FIGDIR)/*.fig))


$(TARGET).dvi: $(TEX_SRC) $(EPS) $(PSLATEX) $(TEMPLATEFILE) bib
	latex $(TARGET)
	@echo "" #a blank line
	latex $(TARGET)

#%.eps:: %.fig
#	fig2dev -L eps < $< > $@

%.pstex_t:: %.fig
	fig2dev -L pstex < $< > $(patsubst %.pstex_t,%.pstex,$@)
	fig2dev -L pstex_t -F -p $(patsubst %.pstex_t,%.pstex,$@) < $< > $@
	sed -i 's/\\SetFigFont{[0-9]*}{[0-9]*.[0-9]*}//' $@ #remove font size, which should be set directly by latex codes in XFig 
	if [ "$*" == "$(FIGDIR)/s27_no_io" ]; then sed -i 's/7.500\ slw/12.000\ slw/' $*.pstex; fi  #chage drawing line width to 12/7.5 times, xfig only supports integer times

%.eps:: %.jpg
	convert $< $@

%.eps:: %.asy
	asy -cd $(FIGDIR) $(patsubst $(FIGDIR)/%,%,$<)

pdf:	$(TARGET).dvi
	#dvips -Ppdf -Pdownload35 -t letter $(TARGET).dvi
	#ps2pdf -dEmbedAllFonts=true -dPDFSETTINGS=/prepress $(TARGET).ps
	dvipdf  $(TARGET).dvi
	#pdftk $(TARGET).pdf $(CONFPAPER) cat output $(TARGET)_combined.pdf
	#rm $(TARGET).pdf
	#rm $(TARGET).dvi

bib:	$(BIBFILE)
	latex $(TARGET)
	bibtex $(TARGET)

clean::
	rm -f *.ps *.dvi *.log *.aux *.bbl *.toc *.lof *.lot \
	      *.blg *.bak *~ *.figure *.table *.sec $(EPS) \
	      $(FIGDIR)/*.pstex_t $(FIGDIR)/*.pstex

