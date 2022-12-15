#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<map>
#include<list>
#include<set>
#include<string>
#include<cstring>
#include<ctime>
#include<cmath>
#include<random>
#include<algorithm>
#include"gurobi_c++.h"
#include"class.h"
#include"gv.h"
#define Mval 1000000
#define print_coef 1
#define dry_coef 2
using namespace std;
int main(int argc, char* argv[])
{
	funcReadFile(argv[1],argv[2], argv[3]);
	cout<<endl<<"PHASE 1: MINIMIZE LAYER NUMBER"<<endl<<endl;
	funcP1Prepare();
	cout<<"Start building variables (phase 1):"<<endl;
	funcModel_P1Var();
	cout<<"End building variables (phase 1). "<<vvar.size()<<" variables."<<endl;
	cout<<"Start building constraints (phase 1):"<<endl;
	funcModel_P1Cons();
	cout<<"End building constraints (phase 1). "<<nCons<<" constraints."<<endl;
	funcGurobi(0, 0.05, atoi(argv[4]), 0.0, 0.01, 1, 1); // min/max, heu, time, absgap, gap, display, focus (quick feas.)
	funcScreenMessage();
	cout<<endl<<"PHASE 2: MINIMIZE DRYING TIME WITH FIXED LAYER NUMBER"<<endl<<endl;
	funcP2Prepare();
	cout<<"Start building variables (phase 2):"<<endl;
	funcModel_P2Var();
	cout<<"End building variables (phase 2). "<<vvar.size()<<" variables."<<endl;
	cout<<"Start building constraints (phase 2):"<<endl;
	funcModel_P2Cons();
	cout<<"End building constraints (phase 2). "<<nCons<<" constraints."<<endl;
	funcGurobi(0, 0.05, atoi(argv[4]), 0.0, 0.01, 1, 1); // min/max, heu, time, absgap, gap, display, focus (quick feas.)
	funcWriteFile(argv[5]);
	funcScreenMessage();
}
void funcP1Prepare()
{
	// remove redundant info
	seto.erase(mapnameo["Chip"]);
	objchip=mapnameo["Chip"];
	// drying time calculation for each object
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		object *o=*setit;
		o->dttile=new double *[maxXtile];
		for(int i=0;i<maxXtile;i++)
			o->dttile[i]=new double [maxYtile];
		for(int i=0;i<maxXtile;i++)
		for(int j=0;j<maxYtile;j++)
			o->dttile[i][j]=0;
	}
	default_random_engine generator;
	normal_distribution<double> distribution(0.0,(maxXtile+maxYtile)/8);
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		object *o=*setit;
		int nrolls=10000; // number of experiments
		int *ploc=new int [maxXtile+maxYtile];
		for(int i=0;i<maxXtile+maxYtile;i++)
			ploc[i]=0;
		for(int i=0;i<nrolls;i++)
		{
			double exresult=distribution(generator);
			if(exresult>=0&&exresult<maxXtile+maxYtile)
				ploc[int(exresult)]++;
		}
		for(auto tileloc:o->settileloc)
		for(int i=0;i<maxXtile;i++)
		for(int j=0;j<maxYtile;j++)
			o->dttile[i][j]+=ploc[abs(tileloc.first-i)+abs(tileloc.second-j)];
		for(int i=0;i<maxXtile;i++)
		for(int j=0;j<maxYtile;j++)
			o->dttile[i][j]=o->dttile[i][j]/nrolls;
	}
}
void funcP2Prepare()
{
	// limit max layer number
	maxL->ub=maxL->intrval;
}
void funcModel_P1Var()
{
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		(*setit)->vl=new var;
		(*setit)->vl->index=vvar.size();
		(*setit)->vl->lb=1; // layer index starts from 1
		(*setit)->vl->ub=seto.size(); // max layer
		(*setit)->vl->typ=1; // integer
		vvar.push_back((*setit)->vl);
	}
	maxL=new var;
	maxL->index=vvar.size();
	maxL->lb=1; // layer index starts from 1
	maxL->ub=seto.size(); // max layer
	maxL->typ=1; // integer
	vvar.push_back(maxL);
}
void funcModel_P1Cons()
{
	nCons=-1;
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		object *o=*setit;
		// for no-merge conflict
		for(set<object*>::iterator setitt=o->setco.begin();setitt!=o->setco.end();setitt++)
		{
			// o->vl + 1 <= on->vl ||
			// on->vl + 1 <= o->vl
			object *on=*setitt;
			var *q=new var;
			q->index=vvar.size();
			q->lb=0;
			q->ub=1;
			q->typ=0; // binary variable
			vvar.push_back(q);
			// o->vl + 1 <= on->vl + M*q
			nCons++;
			mapcv[nCons].insert(o->vl);
			o->vl->cons_i.insert(nCons);
			o->vl->coef_cons[nCons]=1;
			mapcv[nCons].insert(on->vl);
			on->vl->cons_i.insert(nCons);
			on->vl->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q);
			q->cons_i.insert(nCons);
			q->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1;
			// on->vl + 1 <= o->vl + M*(1-q)
			nCons++;
			mapcv[nCons].insert(on->vl);
			on->vl->cons_i.insert(nCons);
			on->vl->coef_cons[nCons]=1;
			mapcv[nCons].insert(o->vl);
			o->vl->cons_i.insert(nCons);
			o->vl->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q);
			q->cons_i.insert(nCons);
			q->coef_cons[nCons]=Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=Mval-1;
		}
		// for no-absorption conflict
		for(set<object*>::iterator setitt=o->setbo.begin();setitt!=o->setbo.end();setitt++)
		{
			// o->vl + 1 <= on->vl, print small pattern first
			object *on=*setitt;
			// o->vl + 1 <= on->vl
			nCons++;
			mapcv[nCons].insert(o->vl);
			o->vl->cons_i.insert(nCons);
			o->vl->coef_cons[nCons]=1;
			mapcv[nCons].insert(on->vl);
			on->vl->cons_i.insert(nCons);
			on->vl->coef_cons[nCons]=-1;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1;
		}
		// o->vl <= maxL
		nCons++;
		mapcv[nCons].insert(o->vl);
		o->vl->cons_i.insert(nCons);
		o->vl->coef_cons[nCons]=1;
		mapcv[nCons].insert(maxL);
		maxL->cons_i.insert(nCons);
		maxL->coef_cons[nCons]=-1;
		mapcs[nCons]=-1;
		mapcc[nCons]=0;
	}
	// obj	
	nCons++;
	objcons=nCons;
	// minimize maxL
	mapcv[objcons].insert(maxL);
	maxL->cons_i.insert(objcons);
	maxL->coef_cons[objcons]=1;
}
void funcModel_P2Var()
{
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
		(*setit)->bvl=new var *[maxL->intrval+1];
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	for(int i=1;i<=maxL->intrval;i++) // layer index starts from 1
	{
		(*setit)->bvl[i]=new var;
		(*setit)->bvl[i]->index=vvar.size();
		(*setit)->bvl[i]->lb=0;
		(*setit)->bvl[i]->ub=1;
		(*setit)->bvl[i]->typ=0; // binary
		vvar.push_back((*setit)->bvl[i]);
	}
	for(int i=1;i<=maxL->intrval;i++) // layer index starts from 1
	{
		layer *l=new layer;
		l->vmaxdt=new var;
		l->vmaxdt->index=vvar.size();
		l->vmaxdt->lb=0;
		l->vmaxdt->ub=Mval;
		l->vmaxdt->typ=2; // continuous
		vvar.push_back(l->vmaxdt);
		l->vdttile=new var **[maxXtile];
		for(int j=0;j<maxXtile;j++)
			l->vdttile[j]=new var *[maxYtile];
		for(int j=0;j<maxXtile;j++)
		for(int k=0;k<maxYtile;k++)
		{
			l->vdttile[j][k]=new var;
			l->vdttile[j][k]->index=vvar.size();
			l->vdttile[j][k]->lb=0;
			l->vdttile[j][k]->ub=Mval;
			l->vdttile[j][k]->typ=2; // continuous
			vvar.push_back(l->vdttile[j][k]);	
		}
		vecl.push_back(l);
	}
}
void funcModel_P2Cons()
{
	// build relation among obj->vl and obj->bvl
	for(auto o:seto)
	{
		// o->vl = sum_i i*o->bvl[i]
		nCons++;
		mapcv[nCons].insert(o->vl);
		o->vl->cons_i.insert(nCons);
		o->vl->coef_cons[nCons]=1;
		for(int i=1;i<=maxL->intrval;i++)
		{
			mapcv[nCons].insert(o->bvl[i]);
			o->bvl[i]->cons_i.insert(nCons);
			o->bvl[i]->coef_cons[nCons]=-1*i;
		}
		mapcs[nCons]=0;
		mapcc[nCons]=0;
	}
	// modeling drying time for each tile for each layer
	for(int i=0;i<vecl.size();i++) // vecl still starts from 0
	for(int j=0;j<maxXtile;j++)
	for(int k=0;k<maxYtile;k++)
	{
		// vecl[i]->vdttile[j][k] = sum_o o->dttime[j][k]*o->bvl[i+1]
		nCons++;
		mapcv[nCons].insert(vecl[i]->vdttile[j][k]);
		vecl[i]->vdttile[j][k]->cons_i.insert(nCons);
		vecl[i]->vdttile[j][k]->coef_cons[nCons]=1;
		for(auto o:seto)
		{
			mapcv[nCons].insert(o->bvl[i+1]);
			o->bvl[i+1]->cons_i.insert(nCons);
			o->bvl[i+1]->coef_cons[nCons]=-1*o->dttile[j][k];
		}
		mapcs[nCons]=0;
		mapcc[nCons]=0;
	}
	for(int i=0;i<vecl.size();i++) // vecl still starts from 0
	for(int j=0;j<maxXtile;j++)
	for(int k=0;k<maxYtile;k++)
	{
		// vecl[i]->vmaxdt >= vecl[i]->vdttile[j][k]
		nCons++;
		mapcv[nCons].insert(vecl[i]->vmaxdt);
		vecl[i]->vmaxdt->cons_i.insert(nCons);
		vecl[i]->vmaxdt->coef_cons[nCons]=1;
		mapcv[nCons].insert(vecl[i]->vdttile[j][k]);
		vecl[i]->vdttile[j][k]->cons_i.insert(nCons);
		vecl[i]->vdttile[j][k]->coef_cons[nCons]=-1;
		mapcs[nCons]=1;
		mapcc[nCons]=0;
	}
	// minimize sum_l l->vmaxdt
	for(int i=0;i<vecl.size();i++) // vecl still starts from 0
	{
		mapcv[objcons].insert(vecl[i]->vmaxdt);
		vecl[i]->vmaxdt->cons_i.insert(objcons);
		vecl[i]->vmaxdt->coef_cons[objcons]=1;
	}
}
void funcScreenMessage()
{
	// object info
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		object *o=*setit;
		cout<<o->name<<" "<<o->area<<" ";
		if(o->setco.size()>0)
			cout<<"setco: ";
		for(set<object*>::iterator setitt=o->setco.begin();setitt!=o->setco.end();setitt++)
			cout<<(*setitt)->name<<" ";
		if(o->setbo.size()>0)
			cout<<"| setbo: ";
		for(set<object*>::iterator setitt=o->setbo.begin();setitt!=o->setbo.end();setitt++)
			cout<<(*setitt)->name<<" ";
		cout<<endl;
	}
	for(int i=1;i<=maxL->intrval;i++)
	{
		cout<<"Layer"<<i;
		if(vecl.size()==0) // P1
		{
			double **dttile, maxdt;
			dttile=new double *[maxXtile];
			for(int j=0;j<maxXtile;j++)
				dttile[j]=new double [maxYtile];
			for(int j=0;j<maxXtile;j++)
			for(int k=0;k<maxYtile;k++)
				dttile[j][k]=0;
			for(auto o:seto)
			if(o->vl->intrval==i)
			for(int j=0;j<maxXtile;j++)
			for(int k=0;k<maxYtile;k++)
				dttile[j][k]+=o->dttile[j][k];
			maxdt=0;
			for(int j=0;j<maxXtile;j++)
			for(int k=0;k<maxYtile;k++)
			if(maxdt<dttile[j][k])
				maxdt=dttile[j][k];
			cout<<"("<<maxdt<<")";
		}
		else // P2
			cout<<"("<<vecl[i-1]->vmaxdt->dourval<<")";
		cout<<": ";
		for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
		if((*setit)->vl->intrval==i)
			cout<<(*setit)->name<<", ";
		cout<<endl;
	}
	cout<<"Total layer: "<<maxL->intrval<<endl;
}
void funcWriteFile(const char* filename)
{
	for(int i=1;i<=maxL->intrval;i++)
	{
		ostringstream convi;
		convi<<i;
		fstream WriteFile_oa;
		string strtmp=filename;
		strtmp+="_objassign";
		strtmp+=convi.str();
		strtmp+=".txt";
		WriteFile_oa.open(strtmp.c_str(), fstream::out);
		// chip
		WriteFile_oa<<"-1"<<endl;
		WriteFile_oa<<objchip->path.size()<<endl;
		for(int j=0;j<objchip->path.size();j++)
			WriteFile_oa<<objchip->path[j].first<<" "<<objchip->path[j].second<<endl;
		// normal objects
		for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
		if((*setit)->vl->intrval==i)
		{
			WriteFile_oa<<"-1"<<endl;
			WriteFile_oa<<(*setit)->path.size()<<endl;
			for(int j=0;j<(*setit)->path.size();j++)
				WriteFile_oa<<(*setit)->path[j].first<<" "<<(*setit)->path[j].second<<endl;
		}
		WriteFile_oa.close();
		fstream WriteFile_heat;
		strtmp=filename;
		strtmp+="_heat";
		strtmp+=convi.str();
		strtmp+=".txt";
		WriteFile_heat.open(strtmp.c_str(), fstream::out);
		for(int k=0;k<maxYtile;k++)
		for(int j=0;j<maxXtile;j++)
			WriteFile_heat<<j+1<<" "<<k+1<<" "<<vecl[i-1]->vdttile[j][k]->dourval<<endl;
		WriteFile_heat.close();
	}
	fstream WriteFile_tot;
	string strtmp=filename;
	strtmp+="_tot.txt";
	WriteFile_tot.open(strtmp.c_str(), fstream::out);
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		WriteFile_tot<<"-1"<<endl;
		WriteFile_tot<<(*setit)->path.size()<<endl;
		for(int j=0;j<(*setit)->path.size();j++)
			WriteFile_tot<<(*setit)->path[j].first<<" "<<(*setit)->path[j].second<<endl;
	}
	WriteFile_tot.close();
}
void funcReadFile(const char* filename_obj, const char* filename_conf, const char* filename_tile)
{
	// read path info of objects
	fstream ReadFile_obj;
	ReadFile_obj.open(filename_obj, fstream::in);
	string strtmp;
	ReadFile_obj>>strtmp; // the name of the first object
	while(1)
	{
		if(ReadFile_obj.eof())
			break;
		object *o=new object;
		o->name=strtmp;
		mapnameo[strtmp]=o;
		seto.insert(o);
		ReadFile_obj>>o->area;
		while(1)
		{
			ReadFile_obj>>strtmp; // must be object name ,"line*:", or file end
			if(ReadFile_obj.eof()||strtmp[strtmp.size()-1]!=':')
				break;
			ReadFile_obj>>strtmp; // location of bending
			size_t strloc1=0;
			size_t strloc2=strtmp.find('+');
			double x, y;
			x=atof(strtmp.substr(strloc1,strloc2-strloc1).c_str());
			strloc1=strloc2+1;
			strloc2=strtmp.find('i',strloc1);
			y=atof(strtmp.substr(strloc1,strloc2-strloc1).c_str());
			o->path.push_back(make_pair(x,y));
		}
		if(ReadFile_obj.eof())
			break;
	}
	ReadFile_obj.close();
	// read conflict info of objects
	fstream ReadFile_conf;
	ReadFile_conf.open(filename_conf, fstream::in);
	while(1)
	{
		string strtmp;
		getline(ReadFile_conf,strtmp);
		if(ReadFile_conf.eof())
			break;
		if(strtmp.compare("no merge conflict:")==0)
		while(1)
		{
			ReadFile_conf>>strtmp;
			if(strtmp.compare("FIN")==0)
				break;		
			object *o=mapnameo[strtmp];
			int nobj;
			ReadFile_conf>>nobj;
			for(int i=0;i<nobj;i++)
			{
				ReadFile_conf>>strtmp;
				o->setco.insert(mapnameo[strtmp]);
			}
		}	
		if(strtmp.compare("no absorption conflict:")==0)
		while(1)
		{
			ReadFile_conf>>strtmp;
			if(strtmp.compare("FIN")==0)
				break;		
			object *o=mapnameo[strtmp];
			int nobj;
			ReadFile_conf>>nobj;
			for(int i=0;i<nobj;i++)
			{
				ReadFile_conf>>strtmp;
				o->setbo.insert(mapnameo[strtmp]);
			}
		}	
		if(strtmp.compare("distance:")==0)
		while(1)
		{
			ReadFile_conf>>strtmp;
			if(strtmp.compare("FIN")==0)
				break;		
			object *o1=mapnameo[strtmp];
			ReadFile_conf>>strtmp;
			object *o2=mapnameo[strtmp];
			double dis;
			ReadFile_conf>>dis;
			mapoodis[make_pair(o1,o2)]=dis;
			mapoodis[make_pair(o2,o1)]=dis;
		}
	}
	ReadFile_conf.close();
	// read possessed tile of objects
	fstream ReadFile_tile;
	ReadFile_tile.open(filename_tile, fstream::in);
	ReadFile_tile>>maxXtile;
	ReadFile_tile>>maxYtile;
	ReadFile_tile>>tiledim;
	while(1)
	{
		ReadFile_tile>>strtmp; // obj name
		if(strtmp.compare("FIN")==0)
				break;		
		object *o=mapnameo[strtmp];
		ReadFile_tile>>o->ntile;
		for(int i=0;i<o->ntile;i++)
		{
			ReadFile_tile>>strtmp; // tile coordinate
			strtmp=strtmp.substr(1,strtmp.size()-2);
			int x, y;
			size_t strloc=strtmp.find(',');
			x=atoi(strtmp.substr(0,strloc).c_str());
			y=atoi(strtmp.substr(strloc+1,strtmp.size()-strloc-1).c_str());
			o->settileloc.insert(make_pair(x-1,y-1));
		}
	}
	ReadFile_tile.close();
}
void funcGurobi(int opt, double heu, double time, double absgap, double gap, int idis, int focus)
{
	cout<<"Gurobi start"<<endl;
	GRBVar *x=new GRBVar [vvar.size()];
//	try{
	GRBEnv env = GRBEnv();
	GRBModel model = GRBModel(env);
	model.getEnv().set(GRB_DoubleParam_TimeLimit, time); // e.g. 900
	model.getEnv().set(GRB_DoubleParam_MIPGapAbs, absgap); // e.g. 4020
	model.getEnv().set(GRB_DoubleParam_MIPGap, gap); // e.g. 0.02
	model.getEnv().set(GRB_DoubleParam_Heuristics, heu); // default 0.05
	model.getEnv().set(GRB_IntParam_OutputFlag, idis);
	model.getEnv().set(GRB_IntParam_MIPFocus, focus);
	map<var*,string> mapvs;
	for(int i=0;i<vvar.size();i++)
	{
		ostringstream convi;
		convi<<i;
		mapvs[vvar[i]]='x'+convi.str();
	}
	for(int i=0;i<vvar.size();i++)
	{
		if(vvar[i]->typ==0)
		x[i]=model.addVar(vvar[i]->lb,vvar[i]->ub,0.0,GRB_BINARY,mapvs[vvar[i]]);
		else if(vvar[i]->typ==1)
		x[i]=model.addVar(vvar[i]->lb,vvar[i]->ub,0.0,GRB_INTEGER,mapvs[vvar[i]]);
		else if(vvar[i]->typ==2)
		x[i]=model.addVar(vvar[i]->lb,vvar[i]->ub,0.0,GRB_CONTINUOUS,mapvs[vvar[i]]);
	}
	model.update();
	for(int i=0;i<=nCons;i++)
	if(mapcv[i].size()!=0) // cons with 0 var is eliminated
	{
		ostringstream convi;
		convi<<i;
		if(qcons.find(i)==qcons.end())
		{
			GRBLinExpr expr;
			for(set<var*>::iterator setit=mapcv[i].begin();setit!=mapcv[i].end();setit++)
				expr+=x[(*setit)->index]*((*setit)->coef_cons[i]);
			if(i!=objcons)
			{
				string name='c'+convi.str();
				if(mapcs[i]==1)
					model.addConstr(expr,GRB_GREATER_EQUAL,mapcc[i],name);
				else if(mapcs[i]==-1)
					model.addConstr(expr,GRB_LESS_EQUAL,mapcc[i],name);
				else if(mapcs[i]==0)
					model.addConstr(expr,GRB_EQUAL,mapcc[i],name);
			}
			else
			{
				if(opt==0)
					model.setObjective(expr,GRB_MINIMIZE);
				else
					model.setObjective(expr,GRB_MAXIMIZE);
			}
		}
		else
		{
			GRBQuadExpr expr;
			for(set<var*>::iterator setit=mapcv[i].begin();setit!=mapcv[i].end();setit++)
			{
				expr+=((*setit)->coef_cons[i])*x[(*setit)->index];
				for(set<var*>::iterator setitt=(*setit)->coef_setvar[i].begin();setitt!=(*setit)->coef_setvar[i].end();setitt++)
					expr+=((*setit)->coef_varcons[make_pair(i,*setitt)])*x[(*setit)->index]*x[(*setitt)->index];
			}
			if(i!=objcons)
			{
				string name='c'+convi.str();
				if(mapcs[i]==1)
					model.addQConstr(expr,GRB_GREATER_EQUAL,mapcc[i],name);
				else if(mapcs[i]==-1)
					model.addQConstr(expr,GRB_LESS_EQUAL,mapcc[i],name);
				else if(mapcs[i]==0)
					model.addQConstr(expr,GRB_EQUAL,mapcc[i],name);
			}
			else
			{
				if(opt==0)
					model.setObjective(expr,GRB_MINIMIZE);
				else
					model.setObjective(expr,GRB_MAXIMIZE);
			}
		}
	}
//	mycallback cb(time, absgap);
//	model.setCallback(&cb);
	int optite=0;
	model.optimize();
	if(model.get(GRB_IntAttr_Status)==GRB_INFEASIBLE)
	{
		model.getEnv().set(GRB_DoubleParam_TimeLimit, 3600); // e.g. 900
		model.computeIIS();
		model.write("model.ilp");
		exit(1);
	}
	else
	while(model.get(GRB_IntAttr_SolCount)==0)
	{
		time+=5;
		optite++;
		model.getEnv().set(GRB_DoubleParam_TimeLimit, time); // e.g. 900
		model.optimize();
	}
//	} catch (GRBException e) {
//		cout << "Error number: " << e.getErrorCode() << endl;
//		cout << e.getMessage() << endl;
//	} catch (...) {
//		cout << "Error during optimization" << endl;
//	}
	for(int i=0;i<vvar.size();i++)
	{
		if(vvar[i]->typ==0||vvar[i]->typ==1)
		{
			if(x[i].get(GRB_DoubleAttr_X)-(int)x[i].get(GRB_DoubleAttr_X)<0.5)
				vvar[i]->intrval=(int)x[i].get(GRB_DoubleAttr_X);
			else
				vvar[i]->intrval=(int)x[i].get(GRB_DoubleAttr_X)+1;
		}
		else if(vvar[i]->typ==2)
			vvar[i]->dourval=x[i].get(GRB_DoubleAttr_X);
		else
			cout<<"new type"<<endl;
	}
	cout<<"Gurobi end"<<endl;
}
