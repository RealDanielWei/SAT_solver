#include "Base.h"

int main(int argc,char** argv) {
	if (argc == 1) {
		cout << "No input file!" << endl;
		return 1;
	}
	CNF mycnf(argv[1]);
	varset vset(mycnf.nliteral);
	bool result=mycnf.DPLL_search(vset,false);
	//vset.show();
	if (result) {
		cout << "RESULT: SAT" << endl;
		cout << "ASSIGNMENT: ";
		for (int i = 0; i < vset.num; i++) {
			char assign = '0';
			if (vset.values[i] == true) {
				assign = '1';
			}
			cout << "x" << i + 1 <<"="<< assign << " ";
		}
		cout << endl;
	}
	else {
		cout << "RESULT: UNSAT" << endl;
	}
	return 0;
}