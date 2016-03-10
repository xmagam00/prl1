# vychozi obrazky
SRC=snimac.jpg
# pozadovany format pro LaTeX
DEST=snimac.jpg

.SUFFIXES: .fig .eps .png .jpg

all: $(DEST)

.fig.eps:
	fig2dev -L eps $< $@

.png.eps:
	convert $< $@

.jpg.eps:
	convert $< $@
