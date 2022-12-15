using namespace std;
class var;
class object
{
	public:
		// input
		object();
		string name;
		vector<pair<double,double> > path;
		double area;
		int ntile;
		set<object*> setco, setbo; // conflict: no merge, no absorption
		set<tuple<int, int, double> > settileloc;
		double **dttile;
		// model (layer)
		var *vl, **bvl; // layer assignment
};
object::object()
{
	vl=NULL;
	bvl=NULL;
}
class layer
{
        public:
		layer();
		var ***vdttile, *vmaxdt; // drying time for each tile, max drying time
};
layer::layer()
{
	vdttile=NULL;
	vmaxdt=NULL;
}
class var
{
        public:
                var();
                int typ, index; // 0: binary, 1: integer, 2: continuous
                double lb, ub;
                set<int> cons_i;
                map<int,double> coef_cons; // cons_i, coef
                int intrval; // int val
                double dourval; // double val
		// quadratic
		map<int, set<var*> > coef_setvar;
		map<pair<int, var*>, double> coef_varcons;
};
var::var()
{
        typ=-1;
        intrval=-1;
        dourval=-1;
}
