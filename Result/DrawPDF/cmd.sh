echo ../$1 | asy -o $1.eps draw.asy && epspdf $1.eps
	open "$1.pdf"
	rm "$1.eps"
# (for Mac Comp.) open "$1.pdf"
