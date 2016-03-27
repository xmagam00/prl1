# vychozi obrazky
SRC=mes.png diagram.jpg
# pozadovany format pro LaTeX
DEST=mes.eps diagram.eps

.SUFFIXES: .fig .eps .png .jpg

all: $(DEST)

.fig.eps:
	fig2dev -L eps $< $@

.png.eps:
	convert $< $@

.jpg.eps:
	convert $< $@
