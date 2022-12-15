void funcReadFile(const char*);
void funcWriteFile(const char*);
void funcPathDeco();
void funcPathtoObj();
void funcObjConf();
void funcModel_Var();
void funcModel_Cons();
void funcGurobi(int, double, double, double, double, int, int);
void funcConsClear();
void funcScreenMessage();

// ReadFile
double maxX, maxY;
double snmvt;
set<object*> seto;
map<string, object*> mapnameo;
// funcModel_Var
var *maxT, *maxL;

// Model
int nCons, objcons;
map<int,int> mapcs; // use cons index to map sense: 1=g, 0=e, -1=l
map<int,double> mapcc; // use cons index to map right side constant
map<int,set<var*> > mapcv; // use cons index to map corresponding vars
vector<var*> vvar;
set<int> qcons;
