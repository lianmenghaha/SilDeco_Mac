#g++ -std=c++11 -m64 -g -o sildeco sildeco.cpp -I/home/ga49zos/tools/opt/gurobi800/linux64/include/ -L/home/ga49zos/tools/opt/gurobi800/linux64/lib/ -lgurobi_g++5.2 -lgurobi80 -lpthread -lm
g++ -std=c++11 -m64 -g -o sildeco sildeco.cpp -I/home/ga49zos/tools/opt/gurobi810/linux64/include/ -L/home/ga49zos/tools/opt/gurobi810/linux64/lib/ -lgurobi_g++5.2 -lgurobi81 -lpthread -lm

if [ "$1" == "1" ]; then
	echo "
		Case: layer1
		"
	time ./sildeco ../ExtractedInfo/output0712/LAYER1/objects.txt ../ExtractedInfo/output0712/LAYER1/conflict.txt ../ExtractedInfo/output0712/LAYER1/unit.txt 30 ../Result/LAYER1
elif [ "$1" == "2" ]; then
	echo "
		Case: layer2
		"
	time ./sildeco ../ExtractedInfo/output0712/LAYER2/objects.txt ../ExtractedInfo/output0712/LAYER2/conflict.txt ../ExtractedInfo/output0712/LAYER2/unit.txt 30 ../Result/LAYER2
elif [ "$1" == "3" ]; then
	echo "
		Case: layer3
		"
	time ./sildeco ../ExtractedInfo/output0712/LAYER3/objects.txt ../ExtractedInfo/output0712/LAYER3/conflict.txt ../ExtractedInfo/output0712/LAYER3/unit.txt 30 ../Result/LAYER3
elif [ "$1" == "4" ]; then
	echo "
		Case: IDE
		"
	time ./sildeco ../ExtractedInfo/output0712/171130-IDELayout-final/objects.txt ../ExtractedInfo/output0712/171130-IDELayout-final/conflict.txt ../ExtractedInfo/output0712/171130-IDELayout-final/unit.txt 30 ../Result/171130-IDELayout-final
elif [ "$1" == "5" ]; then
	echo "
		Case: Heater
		"
	time ./sildeco ../ExtractedInfo/output0712/171130-MicroHeaterLayout-final/objects.txt ../ExtractedInfo/output0712/171130-MicroHeaterLayout-final/conflict.txt ../ExtractedInfo/output0712/171130-MicroHeaterLayout-final/unit.txt 120 ../Result/71130-MicroHeaterLayout-final
elif [ "$1" == "6" ]; then
	echo "
		Case: 2-Comb
		"
	time ./sildeco ../ExtractedInfo/output0712/180523IDEPITCH70MERGED/objects.txt ../ExtractedInfo/output0712/180523IDEPITCH70MERGED/conflict.txt ../ExtractedInfo/output0712/180523IDEPITCH70MERGED/unit.txt 30 ../Result/180523IDEPITCH70MERGED
elif [ "$1" == "7" ]; then
	echo "
		Case: 2-Comb
		"
	time ./sildeco ../ExtractedInfo/output0720/180724EDAIDEPRINTS/objects.txt ../ExtractedInfo/output0720/180724EDAIDEPRINTS/conflict.txt ../ExtractedInfo/output0720/180724EDAIDEPRINTS/unit.txt 30 ../Result/180724EDAIDEPRINTS
else
	echo "NON-supported case!!!!!"
fi
# example:
# $1 (input, obj path): 	../ExtractedInfo/output0418/layer1/objects.txt
# $2 (input, obj conflict: 	../ExtractedInfo/output0418/layer1/conflict.txt
# $3 (time threshold): 		30
# $4 (output common name):	../Result/layer1
