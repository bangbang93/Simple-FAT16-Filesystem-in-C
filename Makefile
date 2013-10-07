all:
	gcc diskinfo.c readImg.c -o diskinfo
	gcc disklist.c readImg.c -o disklist
	gcc diskget.c readImg.c -o diskget

clean:
	rm diskinfo diskget 
 
