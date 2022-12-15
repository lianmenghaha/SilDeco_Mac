#g++ -std=c++11 -m64 -g -o sildeco sildeco.cpp -I/home/ga49zos/tools/opt/gurobi800/linux64/include/ -L/home/ga49zos/tools/opt/gurobi800/linux64/lib/ -lgurobi_g++5.2 -lgurobi80 -lpthread -lm
#g++ -std=c++11 -m64 -g -o sildeco sildeco.cpp -I/home/ge59sik/tools/opt/gurobi902/linux64/include/ -L/home/ge59sik/tools/opt/gurobi902/linux64/lib/ -lgurobi_g++5.2 -lgurobi90 -lpthread -lm
g++ -std=c++11 -m64 -g -o sildeco sildeco.cpp -I/Library/gurobi950/macos_universal2/include -L/Library/gurobi950/macos_universal2/lib -lgurobi_c++ -lgurobi95 -lpthread -lm


if [ "$1" == "0" ]; then
	echo "
		Case: 2-Comb
		"
	time ./sildeco ../ExtractedInfo/output20190325/201903251056/190325EDAIDESFORTMT/objects.txt ../ExtractedInfo/output0720/180724EDAIDEPRINTS/conflict.txt ../ExtractedInfo/output20190325/201903251056/190325EDAIDESFORTMT/unit.txt 30 ../Result/0325-2-comb
elif [ "$1" == "1" ]; then
	echo "
		Case: 0326-1
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_1/190326EDALAPLACE1/objects.txt ../ExtractedInfo/output20190326/260319/20190326_1/190326EDALAPLACE1/conflict.txt ../ExtractedInfo/output20190326/260319/20190326_1/190326EDALAPLACE1/unit.txt 30 ../Result/0326-1
elif [ "$1" == "2" ]; then
	echo "
		Case: 0326-2
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_2/190326EDALAPLACE2/objects.txt ../ExtractedInfo/output20190326/260319/20190326_2/190326EDALAPLACE2/conflict.txt ../ExtractedInfo/output20190326/260319/20190326_2/190326EDALAPLACE2/unit.txt 30 ../Result/0326-2
elif [ "$1" == "3" ]; then
	echo "
		Case: 0326-3
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_3/190326EDALAPLACE3/objects.txt ../ExtractedInfo/output20190326/260319/20190326_3/190326EDALAPLACE3/conflict.txt ../ExtractedInfo/output20190326/260319/20190326_3/190326EDALAPLACE3/unit.txt 30 ../Result/0326-3
elif [ "$1" == "4" ]; then
	echo "
		Case: 0326-4
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_4/190326EDALAPLACE4/objects.txt ../ExtractedInfo/output20190326/260319/20190326_4/190326EDALAPLACE4/conflict.txt ../ExtractedInfo/output20190326/260319/20190326_4/190326EDALAPLACE4/unit.txt 30 ../Result/0326-4
elif [ "$1" == "5" ]; then
	echo "
		Case: 0326-5
		"
	time ./sildeco ../ExtractedInfo/output20190326/260319/20190326_5/190326EDALAPLACE5/objects.txt ../ExtractedInfo/output20190326/260319/20190326_5/190326EDALAPLACE5/conflict.txt ../ExtractedInfo/output20190326/260319/20190326_5/190326EDALAPLACE5/unit.txt 30 ../Result/0326-5
elif [ "$1" == "6" ]; then
	echo "
		Case: 200527
		"
	time ./sildeco ../ExtractedInfo/output20200527/X2003DMFLAYOUTV02FROMQCADCOMPLETE/objects.txt ../ExtractedInfo/output20200527/X2003DMFLAYOUTV02FROMQCADCOMPLETE/conflict.txt ../ExtractedInfo/output20200527/X2003DMFLAYOUTV02FROMQCADCOMPLETE/unit.txt 600 ../Result/20200527
elif [ "$1" == "7" ]; then
	echo "
		Case: 20200715
		"
	time ./sildeco ../ExtractedInfo/output20200715/X2003DMFLAYOUTV02FROMQCADCOMPLETE/objects.txt ../ExtractedInfo/output20200715/X2003DMFLAYOUTV02FROMQCADCOMPLETE/conflict.txt ../ExtractedInfo/output20200715/X2003DMFLAYOUTV02FROMQCADCOMPLETE/unit.txt 600 ../Result/20200715
elif [ "$1" == "8" ]; then
	echo "
                Case: 20201016
                "
        time ./sildeco ../ExtractedInfo/output20201016/X2003DMFLAYOUTV02FROMQCADCOMPLETE/objects.txt ../ExtractedInfo/output20201016/X2003DMFLAYOUTV02FROMQCADCOMPLETE/conflict.txt ../ExtractedInfo/output20201016/X2003DMFLAYOUTV02FROMQCADCOMPLETE/unit.txt 1200 ../Result/20201016
elif [ "$1" == "9" ]; then
        echo "
                Case: 20201103
                "
        time ./sildeco ../ExtractedInfo/output20201103/X201030ML2/objects.txt ../ExtractedInfo/output20201103/X201030ML2/conflict.txt ../ExtractedInfo/output20201103/X201030ML2/unit.txt 1200 ../Result/20201103
elif [ "$1" == "10" ]; then
        echo "
                Case: 171130microheater
                "
        time ./sildeco ../ExtractedInfo/171130microheater/objects.txt ../ExtractedInfo/171130microheater/conflict.txt ../ExtractedInfo/171130microheater/unit.txt 1200 ../Result/171130microheater
elif [ "$1" == "11" ]; then
        echo "
                Case: output-ph
                "
        time ./sildeco ../ExtractedInfo/output-ph/objects.txt ../ExtractedInfo/output-ph/conflict.txt ../ExtractedInfo/output-ph/unit.txt 12000 ../Result/output-ph
elif [ "$1" == "12" ]; then
        echo "
                Case: LAYER3
                "
        time ./sildeco ../ExtractedInfo/LAYER3/objects.txt ../ExtractedInfo/LAYER3/conflict.txt ../ExtractedInfo/LAYER3/unit.txt 1200 ../Result/LAYER3
elif [ "$1" == "13" ]; then
        echo "
                Case: NEW2022
                "
        time ./sildeco ../ExtractedInfo/NEW2022/objects.txt ../ExtractedInfo/NEW2022/conflict.txt ../ExtractedInfo/NEW2022/unit.txt 12000 ../Result/NEW2022
else
	echo "NON-supported case!!!!!"
fi
# example:
# $1 (input, obj path): 	../ExtractedInfo/output0418/layer1/objects.txt
# $2 (input, obj conflict): 	../ExtractedInfo/output0418/layer1/conflict.txt
# $3 (input, obj tile info): 	../ExtractedInfo/output0418/layer1/unit.txt
# $4 (time threshold): 		30
# $5 (output common name):	../Result/layer1
