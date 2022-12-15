#g++ -std=c++11 -m64 -g -o sildeco sildeco.cpp -I/home/ga49zos/tools/opt/gurobi800/linux64/include/ -L/home/ga49zos/tools/opt/gurobi800/linux64/lib/ -lgurobi_g++5.2 -lgurobi80 -lpthread -lm
g++ -std=c++11 -m64 -g -o sildeco sildeco.cpp -I/home/ga49zos/tools/opt/gurobi810/linux64/include/ -L/home/ga49zos/tools/opt/gurobi810/linux64/lib/ -lgurobi_g++5.2 -lgurobi81 -lpthread -lm

if [ "$1" == "0" ]; then
	echo "
		Case: 2-Comb
		"
	time ./sildeco ../ExtractedInfo/output20190607/190308IDEFORTMT/objects.txt ../ExtractedInfo/output20190607/190308IDEFORTMT/conflict.txt 30 ../Result/0325-2-comb
elif [ "$1" == "1" ]; then
	echo "
		Case: 0326-1
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_1/190326EDALAPLACE1/objects.txt ../ExtractedInfo/output20190326/260319/20190326_1/190326EDALAPLACE1/conflict.txt 30 ../Result/0326-1
elif [ "$1" == "2" ]; then
	echo "
		Case: 0326-2
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_2/190326EDALAPLACE2/objects.txt ../ExtractedInfo/output20190326/260319/20190326_2/190326EDALAPLACE2/conflict.txt 30 ../Result/0326-2
elif [ "$1" == "3" ]; then
	echo "
		Case: 0326-3
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_3/190326EDALAPLACE3/objects.txt ../ExtractedInfo/output20190326/260319/20190326_3/190326EDALAPLACE3/conflict.txt 30 ../Result/0326-3
elif [ "$1" == "4" ]; then
	echo "
		Case: 0326-4
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_4/190326EDALAPLACE4/objects.txt ../ExtractedInfo/output20190326/260319/20190326_4/190326EDALAPLACE4/conflict.txt 30 ../Result/0326-4
elif [ "$1" == "5" ]; then
	echo "
		Case: 0326-5
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_5/190326EDALAPLACE5/objects.txt ../ExtractedInfo/output20190326/260319/20190326_5/190326EDALAPLACE5/conflict.txt 30 ../Result/0326-5
else
	echo "NON-supported case!!!!!"
fi
# example:
# $1 (input, obj path): 	../ExtractedInfo/output0418/layer1/objects.txt
# $2 (input, obj conflict: 	../ExtractedInfo/output0418/layer1/conflict.txt
# $3 (time threshold): 		30
# $4 (output common name):	../Result/layer1
