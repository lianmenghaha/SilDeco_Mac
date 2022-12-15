import graph;
usepackage ("lmodern");
// size (400, 400, IgnoreAspect);
defaultpen (font ("ptmr8t", 7pt) + 0.1);
DefaultHead.size=new real(pen p=currentpen) {return 2mm;};

pen printpen= font ("ptmr8t", 1pt) + 0.01;
pen labelpen= font ("ptmr8t", 12pt) + 0.1;
pen apen= font ("ptmr8t", 10pt) + 0.1;
pen bpen= font ("ptmr8t", 9pt) + 1.2;
pen cpen= font ("ptmr8t", 7pt) + 0.5;
pen dpen= font ("ptmr8t", 12pt) + 2;
pen epen= font ("ptmr8t", 9pt) + 1.5;
pen fpen= Helvetica (series="b", shape="n");
pen graypen= font ("ptmr8t", 14pt) + lightgray;
pen graypen2= font ("ptmr8t", 14pt) + gray(0.6);
pen greenpen= font ("ptmr8t", 14pt) + rgb(0,1,0) + 1;
pen darkbluepen= font ("ptmr8t", 14pt) + rgb(0,0,0.3);
pen bluepen= font ("ptmr8t", 14pt) + rgb(0,0,1) + 1.5;
pen redpen= font ("ptmr8t", 14pt) + rgb(1,0,0) + 1.5;
string[] cn={ "1", "2", "3", "4", "5"};

string fname = stdin;
file fin = input (fname);
real[] nodes = fin; //means row unlimited, column 1


int state = 0;
int c_num=0;
pair oploc, opwh, opnextloc;
real matsidelen, curtime;
int count=0, choosepen;
real exinfo;
real zoom = 100;
int intnfw, intncw;
real wdis=200/30, newmidx, upy, lowy, mulindex, upx, lowx, mindis;
real[] wex1, wex2, fpx, fpy, cpx, cpy;
for (int i = 0; i < nodes.length;)
{
	if (nodes[i] < -0.5 && nodes[i] > -1.5) { // == -1, new obj
        	i = i + 1;
		int pathlen = (int) nodes[i];
        	i = i + 1;
		path p; 
		for (int j=0; j<pathlen; j=j+1)
		{
			p=p--(nodes[i],nodes[i+1]);
			i = i + 2;
		}
		p=p--(nodes[i-pathlen*2],nodes[i-pathlen*2+1])--cycle;
		draw(p,printpen);
	}
}

//xaxis(BottomTop,LeftTicks);
//yaxis(LeftRight,RightTicks);

//picture pic;
//label(pic, graphic("../tb_top.ps"),(100,100),NE);
//clip(pic,(255,446)--(255,566)--(562,566)--(562,446)--cycle);
//add(currentpicture, pic.fit(),(50,50),N);
