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
	funcReadFile(argv[1],argv[2]);
	cout<<endl<<"PHASE 1: MINIMIZATION OF LAYER NUMBER"<<endl<<endl;
	funcAnaInput();
	funcModel_P1Var();
	funcModel_P1Cons();
	funcGurobi(0, 0.05, atoi(argv[3]), 0.0, 0.01, 1, 1); // min/max, heu, time, absgap, gap, display, focus (quick feas.)
	cout<<endl<<"PHASE 2: MINIMIZATION OF LAYER NUMBER AND DRYING TIME"<<endl<<endl;
	funcAnaLayerInfo();
	funcModel_P2Var();
	funcModel_P2Cons();
	funcGurobi(0, 0.05, atoi(argv[3]), 0.0, 0.01, 1, 1); // min/max, heu, time, absgap, gap, display, focus (quick feas.)
	funcWriteFile(argv[4]);
	funcScreenMessage();
}
void funcAnaLayerInfo()
{
	// remove objective of phase 1: end
	maxL->ub=maxL->intrval; // define max layer number
}
void funcModel_P2Var()
{
	cout<<"Start building variables (phase 2):"<<endl;
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
		(*setit)->bvl=new var *[maxL->intrval+1];
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	for(int i=1;i<=maxL->intrval;i++)
	{
		(*setit)->bvl[i]=new var;
		(*setit)->bvl[i]->index=vvar.size();
		(*setit)->bvl[i]->lb=0;
		(*setit)->bvl[i]->ub=1;
		(*setit)->bvl[i]->typ=0; // binary
		vvar.push_back((*setit)->bvl[i]);
	}
	for(int i=1;i<=maxL->intrval;i++)
	{
		layer *l=new layer;
		l->varea=new var;
		l->varea->index=vvar.size();
		l->varea->lb=0;
		l->varea->ub=Mval;
		l->varea->typ=2; // continuous
		vvar.push_back(l->varea);
		l->vdis=new var;
		l->vdis->index=vvar.size();
		l->vdis->lb=0;
		l->vdis->ub=Mval;
		l->vdis->typ=2; // continuous
		vvar.push_back(l->vdis);
		l->vdrytime=new var;
		l->vdrytime->index=vvar.size();
		l->vdrytime->lb=0;
		l->vdrytime->ub=Mval;
		l->vdrytime->typ=2; // continuous
		vvar.push_back(l->vdrytime);
		vecl.push_back(l);
	}
	cout<<"End building variables (phase 2). "<<vvar.size()<<" variables."<<endl;
}
void funcModel_P2Cons()
{
	cout<<"Start building constraints (phase 2):"<<endl;
	// build relation among obj->vl and obj->bvl
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		object *o=*setit;
		// o->vl = sum_i i*o->bvl[i]
		nCons++;
		mapcv[nCons].insert(o->vl);
		o->vl->cons_i.insert(nCons);
		o->vl->coef_cons[nCons]=1;
		for(int i=1;i<=maxL->intrval;i++)
		{
			mapcv[nCons].insert(o->bvl[i]);
			o->bvl[i]->cons_i.insert(nCons);
			o->bvl[i]->coef_cons[nCons]=-1*(i+1);
		}
		mapcs[nCons]=0;
		mapcc[nCons]=-1;
	}
	// calculate total pattern area within the same layer
	for(int i=0;i<vecl.size();i++)
	{
		// vecl[i]->varea = sum o->bvl[i]*o->area
	}
	// calculate total pattern distance within the same layer
	for(int i=0;i<vecl.size();i++)
	{
		// o1->bvl[i]=1 and o2->bvl[i]=1 imply vdis[make_pair(o1,o2)]=1
		// vecl[i]->vdis = sum vdis[make_pair(o1,o2)]*mapoodis[make_pair(o1,o2)]
	}
	for(int i=0;i<vecl.size();i++)
	{
		mapcv[nCons].insert(vecl[i]->vdrytime);
		vecl[i]->vdrytime->cons_i.insert(nCons);
		vecl[i]->vdrytime->coef_cons[nCons]=1;
	}
	cout<<"End building constraints (phase 2). "<<nCons<<" constraints."<<endl;
}
void funcAnaInput()
{
	seto.erase(mapnameo["Chip"]);
	objchip=mapnameo["Chip"];
}
void funcModel_P1Var()
{
	cout<<"Start building variables (phase 1):"<<endl;
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		(*setit)->vl=new var;
		(*setit)->vl->index=vvar.size();
		(*setit)->vl->lb=1;
		(*setit)->vl->ub=seto.size(); // max layer
		(*setit)->vl->typ=1; // integer
		vvar.push_back((*setit)->vl);
	}
	maxL=new var;
	maxL->index=vvar.size();
	maxL->lb=1;
	maxL->ub=seto.size(); // max layer
	maxL->typ=1; // integer
	vvar.push_back(maxL);
	cout<<"End building variables (phase 1). "<<vvar.size()<<" variables."<<endl;
}
void funcModel_P1Cons()
{
	cout<<"Start building constraints (phase 1):"<<endl;
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
	mapcv[nCons].insert(maxL);
	maxL->cons_i.insert(nCons);
	maxL->coef_cons[nCons]=1;
	cout<<"End building constraints (phase 1). "<<nCons<<" constraints."<<endl;
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
		cout<<"Layer"<<i<<": ";
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
		fstream WriteFile;
		ostringstream convi;
		convi<<i;
		string strtmp=filename;
		strtmp+="_";
		strtmp+=convi.str();
		strtmp+=".txt";
		WriteFile.open(strtmp.c_str(), fstream::out);
		// chip
		WriteFile<<"-1"<<endl;
		WriteFile<<objchip->path.size()<<endl;
		for(int j=0;j<objchip->path.size();j++)
			WriteFile<<objchip->path[j].first<<" "<<objchip->path[j].second<<endl;
		// normal objects
		for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
		if((*setit)->vl->intrval==i)
		{
			WriteFile<<"-1"<<endl;
			WriteFile<<(*setit)->path.size()<<endl;
			for(int j=0;j<(*setit)->path.size();j++)
				WriteFile<<(*setit)->path[j].first<<" "<<(*setit)->path[j].second<<endl;
		}
		WriteFile.close();
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
void funcReadFile(const char* filename_obj, const char* filename_conf)
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
