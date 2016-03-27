# vychozi obrazky
SRC=mes.png
# pozadovany format pro LaTeX
DEST=mes.eps

.SUFFIXES: .fig .eps .png .jpg

all: $(DEST)

.fig.eps:
	fig2dev -L eps $< $@

.png.eps:
	convert $< $@

.jpg.eps:
	convert $< $@
