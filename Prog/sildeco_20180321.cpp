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
#define Mval 1000000
#include"gurobi_c++.h"
#include"class.h"
#include"gv.h"
using namespace std;
int main(int argc, char* argv[])
{
	funcReadFile(argv[1]);
	funcModel_Var();
	funcConsClear();
	funcModel_Cons();
	funcGurobi(0, 0.05, atoi(argv[3]), 0.0, 0.01, 1, 1); // min/max, heu, time, absgap, gap, display, focus (quick feas.)
	funcScreenMessage();
	funcWriteFile(argv[2]);
}
void funcModel_Var()
{
	cout<<"Start building variables"<<endl;
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		(*setit)->vpst=new var;
		(*setit)->vpst->index=vvar.size();
		(*setit)->vpst->lb=0;
		(*setit)->vpst->ub=Mval;
		(*setit)->vpst->typ=2; // continuous
		vvar.push_back((*setit)->vpst);
		(*setit)->vlayer=new var;
		(*setit)->vlayer->index=vvar.size();
		(*setit)->vlayer->lb=0;
		(*setit)->vlayer->ub=seto.size(); // max layer
		(*setit)->vlayer->typ=1; // integer
		vvar.push_back((*setit)->vlayer);
	}
	maxT=new var;
	maxT->index=vvar.size();
	maxT->lb=0;
	maxT->ub=Mval;
	maxT->typ=2; // continuous
	vvar.push_back(maxT);
	maxL=new var;
	maxL->index=vvar.size();
	maxL->lb=0;
	maxL->ub=seto.size(); // max layer
	maxL->typ=1; // integer
	vvar.push_back(maxL);
	cout<<"End building variables"<<endl;
}
void funcModel_Cons()
{
	cout<<"Start building constraints"<<endl;
	set<pair<object*, object*> > setpairo;
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		object *o1=*setit;
		// for every 2o,
		// o1->vpst + o1->pt <= o2->vpst && o1->vlayer <= o2->vlayer ||
		// o2->vpst + o2->pt <= o1->vpst && o2->vlayer <= o1->vlayer 
		for(set<object*>::iterator setitt=seto.begin();setitt!=seto.end();setitt++)
		if(o1!=*setitt)
		{
			object *o2=*setitt;
			var *q[2];
			for(int i=0;i<2;i++)
			{
				q[i]=new var;
				q[i]->index=vvar.size();
				q[i]->lb=0;
				q[i]->ub=1;
				q[i]->typ=0; // binary variable
				vvar.push_back(q[i]);
			}
			// o1->vpst + o1->pt <= o2->vpst + q[0]*M 
			nCons++;
			mapcv[nCons].insert(o1->vpst);
			o1->vpst->cons_i.insert(nCons);
			o1->vpst->coef_cons[nCons]=1;
			mapcv[nCons].insert(o2->vpst);
			o2->vpst->cons_i.insert(nCons);
			o2->vpst->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q[0]);
			q[0]->cons_i.insert(nCons);
			q[0]->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1*o1->pt;
			// o1->vlayer <= o2->vlayer + q[0]*M 
			nCons++;
			mapcv[nCons].insert(o1->vlayer);
			o1->vlayer->cons_i.insert(nCons);
			o1->vlayer->coef_cons[nCons]=1;
			mapcv[nCons].insert(o2->vlayer);
			o2->vlayer->cons_i.insert(nCons);
			o2->vlayer->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q[0]);
			q[0]->cons_i.insert(nCons);
			q[0]->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=0;
			// o2->vpst + o2->pt <= o1->vpst + q[1]*M
			nCons++;
			mapcv[nCons].insert(o2->vpst);
			o2->vpst->cons_i.insert(nCons);
			o2->vpst->coef_cons[nCons]=1;
			mapcv[nCons].insert(o1->vpst);
			o1->vpst->cons_i.insert(nCons);
			o1->vpst->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q[1]);
			q[1]->cons_i.insert(nCons);
			q[1]->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1*o2->pt;
			// o2->vlayer <= o1->vlayer + q[1]*M 
			nCons++;
			mapcv[nCons].insert(o2->vlayer);
			o2->vlayer->cons_i.insert(nCons);
			o2->vlayer->coef_cons[nCons]=1;
			mapcv[nCons].insert(o1->vlayer);
			o1->vlayer->cons_i.insert(nCons);
			o1->vlayer->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q[1]);
			q[1]->cons_i.insert(nCons);
			q[1]->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=0;
			// q0 + q1 = 1
			nCons++;
			for(int i=0;i<2;i++)
			{
				mapcv[nCons].insert(q[i]);
				q[i]->cons_i.insert(nCons);
				q[i]->coef_cons[nCons]=1;
			}
			mapcs[nCons]=0;
			mapcc[nCons]=1;
		}
		// for bigo,
		// o1->vpst + o1->pt + o1->dt <= o2->vpst && o1->vlayer + 1 <= o2->vlayer
		for(set<object*>::iterator setitt=o1->setbo.begin();setitt!=o1->setbo.end();setitt++)
		{
			object *o2=*setitt;
			// o1->vpst + o1->pt + o1->dt <= o2->vpst
			nCons++;
			mapcv[nCons].insert(o1->vpst);
			o1->vpst->cons_i.insert(nCons);
			o1->vpst->coef_cons[nCons]=1;
			mapcv[nCons].insert(o2->vpst);
			o2->vpst->cons_i.insert(nCons);
			o2->vpst->coef_cons[nCons]=-1;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1*o1->pt-o1->dt;
			// o1->vlayer + 1 <= o2->vlayer
			nCons++;
			mapcv[nCons].insert(o1->vlayer);
			o1->vlayer->cons_i.insert(nCons);
			o1->vlayer->coef_cons[nCons]=1;
			mapcv[nCons].insert(o2->vlayer);
			o2->vlayer->cons_i.insert(nCons);
			o2->vlayer->coef_cons[nCons]=-1;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1;
		}
		// for confo,
		// o1->vpst + o1->pt + o1->dt <= o2->vpst && o1->vlayer + 1 <= o2->vlayer ||
		// o2->vpst + o2->pt + o2->dt <= o1->vpst && o2->vlayer + 1 <= o1->vlayer 
		for(set<object*>::iterator setitt=o1->setco.begin();setitt!=o1->setco.end();setitt++)
		if(setpairo.find(make_pair(o1,*setitt))==setpairo.end())
		{
			object *o2=*setitt;
			setpairo.insert(make_pair(o1,o2));
			setpairo.insert(make_pair(o2,o1));
			var *q[2];
			for(int i=0;i<2;i++)
			{
				q[i]=new var;
				q[i]->index=vvar.size();
				q[i]->lb=0;
				q[i]->ub=1;
				q[i]->typ=0; // binary variable
				vvar.push_back(q[i]);
			}
			// o1->vpst + o1->pt + o1->dt <= o2->vpst + q[0]*M 
			nCons++;
			mapcv[nCons].insert(o1->vpst);
			o1->vpst->cons_i.insert(nCons);
			o1->vpst->coef_cons[nCons]=1;
			mapcv[nCons].insert(o2->vpst);
			o2->vpst->cons_i.insert(nCons);
			o2->vpst->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q[0]);
			q[0]->cons_i.insert(nCons);
			q[0]->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1*o1->pt-o1->dt;
			// o1->vlayer + 1 <= o2->vlayer + q[0]*M
			nCons++;
			mapcv[nCons].insert(o1->vlayer);
			o1->vlayer->cons_i.insert(nCons);
			o1->vlayer->coef_cons[nCons]=1;
			mapcv[nCons].insert(o2->vlayer);
			o2->vlayer->cons_i.insert(nCons);
			o2->vlayer->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q[0]);
			q[0]->cons_i.insert(nCons);
			q[0]->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1;
			// o2->vpst + o2->pt + o2->dt <= o1->vpst + q[1]*M
			nCons++;
			mapcv[nCons].insert(o2->vpst);
			o2->vpst->cons_i.insert(nCons);
			o2->vpst->coef_cons[nCons]=1;
			mapcv[nCons].insert(o1->vpst);
			o1->vpst->cons_i.insert(nCons);
			o1->vpst->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q[1]);
			q[1]->cons_i.insert(nCons);
			q[1]->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1*o2->pt-o2->dt;
			// o2->vlayer + 1 <= o1->vlayer + q[1]*M
			nCons++;
			mapcv[nCons].insert(o2->vlayer);
			o2->vlayer->cons_i.insert(nCons);
			o2->vlayer->coef_cons[nCons]=1;
			mapcv[nCons].insert(o1->vlayer);
			o1->vlayer->cons_i.insert(nCons);
			o1->vlayer->coef_cons[nCons]=-1;
			mapcv[nCons].insert(q[1]);
			q[1]->cons_i.insert(nCons);
			q[1]->coef_cons[nCons]=-1*Mval;
			mapcs[nCons]=-1;
			mapcc[nCons]=-1;
			// q0 + q1 = 1
			nCons++;
			for(int i=0;i<2;i++)
			{
				mapcv[nCons].insert(q[i]);
				q[i]->cons_i.insert(nCons);
				q[i]->coef_cons[nCons]=1;
			}
			mapcs[nCons]=0;
			mapcc[nCons]=1;
		}
		// o1->vpst + o1->pt + o1->dt <= maxT
		nCons++;
		mapcv[nCons].insert(o1->vpst);
		o1->vpst->cons_i.insert(nCons);
		o1->vpst->coef_cons[nCons]=1;
		mapcv[nCons].insert(maxT);
		maxT->cons_i.insert(nCons);
		maxT->coef_cons[nCons]=-1;
		mapcs[nCons]=-1;
		mapcc[nCons]=-1*o1->pt-o1->dt;
		// o1->vlayer <= maxL
		nCons++;
		mapcv[nCons].insert(o1->vlayer);
		o1->vlayer->cons_i.insert(nCons);
		o1->vlayer->coef_cons[nCons]=1;
		mapcv[nCons].insert(maxL);
		maxL->cons_i.insert(nCons);
		maxL->coef_cons[nCons]=-1;
		mapcs[nCons]=-1;
		mapcc[nCons]=0;
	}
	// obj	
	nCons++;
	objcons=nCons;
	// minimize maxT
	mapcv[nCons].insert(maxT);
	maxT->cons_i.insert(nCons);
	maxT->coef_cons[nCons]=1;
	// minimize maxL
	mapcv[nCons].insert(maxL);
	maxL->cons_i.insert(nCons);
	maxL->coef_cons[nCons]=100;
	cout<<"End building constraints"<<endl;
}
void funcScreenMessage()
{
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		cout<<(*setit)->name<<" printing start time: "<<(*setit)->vpst->dourval;
		cout<<", printing duration: "<<(*setit)->pt<<", drying duration: "<<(*setit)->dt<<endl;
		cout<<"setbo: ";
		for(set<object*>::iterator setitt=(*setit)->setbo.begin();setitt!=(*setit)->setbo.end();setitt++)
			cout<<(*setitt)->name<<" ";
		cout<<endl;
		cout<<"setco: ";
		for(set<object*>::iterator setitt=(*setit)->setco.begin();setitt!=(*setit)->setco.end();setitt++)
			cout<<(*setitt)->name<<" ";
		cout<<endl;
	}
	cout<<"Total required time (when all patterns are dry): "<<maxT->dourval<<endl;
	for(int i=0;i<=maxL->intrval;i++)
	{
		cout<<"Layer"<<i<<": ";
		for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
		if((*setit)->vlayer->intrval==i)
			cout<<(*setit)->name<<", ";
		cout<<endl;
	}
	cout<<"Total layer: "<<maxL->intrval+1<<endl;
}
void funcWriteFile(const char* filename)
{
	fstream WriteFile;
	WriteFile.open(filename, fstream::out);
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
	{
		WriteFile<<(*setit)->name<<" printing start time: "<<(*setit)->vpst->dourval;
		WriteFile<<", printing duration: "<<(*setit)->pt<<", drying duration: "<<(*setit)->dt<<endl;
		WriteFile<<"setbo: ";
		for(set<object*>::iterator setitt=(*setit)->setbo.begin();setitt!=(*setit)->setbo.end();setitt++)
			WriteFile<<(*setitt)->name<<" ";
		WriteFile<<endl;
		WriteFile<<"setco: ";
		for(set<object*>::iterator setitt=(*setit)->setco.begin();setitt!=(*setit)->setco.end();setitt++)
			WriteFile<<(*setitt)->name<<" ";
		WriteFile<<endl;
	}
	WriteFile<<"Total required time (when all patterns are dry): "<<maxT->dourval<<endl;
	for(int i=0;i<=maxL->intrval;i++)
	{
		WriteFile<<"Layer"<<i<<": ";
		for(set<object*>::iterator setit=seto.begin();setit!=seto.end();setit++)
		if((*setit)->vlayer->intrval==i)
			WriteFile<<(*setit)->name<<", ";
		WriteFile<<endl;
	}
	WriteFile<<"Total layer: "<<maxL->intrval+1<<endl;
}
void funcReadFile(const char* filename)
{
	fstream ReadFile;
	ReadFile.open(filename, fstream::in);
	while(1)
	{
		string strtmp;
		ReadFile>>strtmp;
		if(ReadFile.eof())
			break;
		if(strtmp.compare("#")==0) // comment
			getline(ReadFile,strtmp);
		else if(strtmp.compare("safe_nozzle_moving_time:")==0)
			ReadFile>>snmvt;	
		else if(strtmp.compare("Part1:")==0) // object declaration
		{
			while(1)
			{
				ReadFile>>strtmp;
				if(strtmp.compare("FIN")==0)
					break;
				else if(strtmp.compare("#")==0) // comment
					getline(ReadFile,strtmp);
				else
				{
					object *o=new object;
					seto.insert(o);
					o->name=strtmp;
					ReadFile>>o->pt;
					ReadFile>>o->dt;
					mapnameo[o->name]=o;
				}
			}
		}
		else if(strtmp.compare("Part2:")==0) // printing order specification
		{
			while(1)
			{
				ReadFile>>strtmp;
				if(strtmp.compare("FIN")==0)
					break;
				else if(strtmp.compare("#")==0) // comment
					getline(ReadFile,strtmp);
				else
				{
					string bigoname;
					ReadFile>>bigoname;
					mapnameo[strtmp]->setbo.insert(mapnameo[bigoname]);
				}
			}
		}
		else if(strtmp.compare("Part3:")==0) // conflict object specification
		{
			while(1)
			{
				ReadFile>>strtmp;
				if(strtmp.compare("FIN")==0)
					break;
				else if(strtmp.compare("#")==0) // comment
					getline(ReadFile,strtmp);
				else
				{
					int nconfobj;
					ReadFile>>nconfobj;
					for(int i=0;i<nconfobj;i++)
					{
						string confoname;
						ReadFile>>confoname;
						mapnameo[strtmp]->setco.insert(mapnameo[confoname]);
					}
				}
			}
		}
		else
		{
			cout<<"ERROR: NON-SUPPORTED CATEGORY OF INPUT FILE: "<<strtmp<<endl;
			exit(1);
		}
	}
}
void funcConsClear()
{
	for(int i=0;i<vvar.size();i++)
	{
		mapcv[i].clear();
		vvar[i]->cons_i.clear();
		vvar[i]->coef_cons.clear();
	}
	mapcv.clear();
	mapcs.clear();
	mapcc.clear();
	nCons=-1;
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
