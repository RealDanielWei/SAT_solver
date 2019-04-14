#include "Base.h"

int main() {

	CNF mycnf("test.cnf");
	//CNF mycnf("bibd-8-70-35-4-15_glb.cnf");
	//CNF mycnf("sdiv15prop.cnf");
	//mycnf.show();
	//varset vset("011");
	varset vset(mycnf.nliteral);
	mycnf.DPLL_search(vset,true);
	vset.show();
	/*
	for (int i = 0; i < mycnf.nclause; i++) {
		for (int j = 0; j < mycnf.clauses[i]->termnum; j++) {
			cout << mycnf.clauses[i]->int_to_boolean(mycnf.clauses[i]->values[j],vset);
		}
		cout << endl;
	}
	*/
	/*
	for (int i = 0; i < mycnf.nclause; i++) {
		cout << "clause_i="<<i<<" "<<mycnf.clauses[i]->evaluate(vset).value << " " << mycnf.clauses[i]->evaluate(vset).status << endl;
	}
	CNF_status cnf_status = mycnf.evaluate(vset);
	cout << "CNF_value=" << cnf_status.value << " CNF_status=" << cnf_status.status << endl;
	*/
	system("pause");
	return 0;
}