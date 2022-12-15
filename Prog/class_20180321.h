using namespace std;
class var;
class object
{
	public:
		object();
		string name;	
		double pt, dt; // printing time and drying time
		var *vpst, *vlayer; // printing start time, layer assignment
		set<object*> setbo; // objects [printed after current object is dry]
		set<object*> setco; // objects [printed after current object is dry] or [dry before current object is printed]
};
object::object()
{
	vpst=NULL;
}
class var
{
        public:
                var();
                int typ, index; // 0: continuous, 1: binary, 2: integer
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
