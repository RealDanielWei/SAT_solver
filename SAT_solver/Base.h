#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

enum Shader
{
	undecided,decided
};

struct clause_status {
	bool value;
	Shader status;
};

struct CNF_status {
	bool value;
	Shader status;
};

struct varset {
	int num;
	bool *values;
	Shader *shaders;
	varset(string s) {
		this->num = s.length();
		this->values = new bool[this->num];
		this->shaders = new Shader[this->num];
		for (int i = 0; i < this->num; i++) {
			if (s[i] == '1') {
				this->values[i] = true;
				this->shaders[i] = decided;
			}
			else {
				if (s[i]=='0') {
					this->values[i] = false;
					this->shaders[i] = decided;
				}
				else {
					this->values[i] = false;
					this->shaders[i] = undecided;
					cout << "This varset is wrong!" << endl;
				}
			}
		}
	}
	varset(int n) {
		if (n <= 0) {
			cout << "Length of varset must be positive!" << endl;
		}
		this->num = n;
		this->values = new bool[this->num];
		this->shaders = new Shader[this->num];
		for (int i = 0; i < this->num; i++) {
			this->values[i] = false;
			this->shaders[i] = undecided;
		}
	}

	void show() {
		for (int i = 0; i < this->num; i++) {
			if (this->shaders[i] == undecided) {
				cout << '-';
			}
			else {
				if (this->values[i] == true) {
					cout << '1';
				}
				else {
					cout << '0';
				}
			}
		}
		cout << endl;
	}
};

class clause {
public:
	int termnum;
	int* values;
	clause(string s,stringstream& strstream,int nliterals) {
		strstream.clear();
		strstream.str(s);
		int index;
		vector<int> temp;
		while (strstream>>index,index!=0)
		{
			temp.push_back(index);
		}
		this->termnum = temp.size();
		this->values = new int[this->termnum];
		for (int i = 0; i < this->termnum; i++) {
			this->values[i] = temp[i];
		}
	}

	bool int_to_boolean(int number, varset& vset) {
		if (number > 0) {
			return vset.values[abs(number) - 1];
		}
		else {
			if (number < 0) {
				return !vset.values[abs(number) - 1];
			}
			else {
				cout << "This clause is wrong!" << endl;
				return false;
			}
		}
	}

	clause_status evaluate(varset& vset) {
		bool undecided_exist = false;
		for (int i = 0; i < this->termnum; i++) {
			if (vset.shaders[abs(this->values[i])-1] == decided) {
				if (int_to_boolean(this->values[i],vset) == true) {
					return clause_status{ true,decided };
				}
			}
			else {
				undecided_exist = true;
			}
		}
		if (undecided_exist == true) {
			return clause_status{ false,undecided };
		}
		else {
			return clause_status{ false,decided };
		}
	}

	void show() {
		for (int i = 0; i < this->termnum; i++) {
			cout << this->values[i] << " ";
		}
		cout << endl;
	}
};

class CNF {
public:
	int nclause, nliteral;
	clause **clauses;

	CNF(string filepath) {
		ifstream file(filepath);
		string s;
		stringstream sstream;
		int clausen = 0;
		while (!file.eof()) {
			getline(file, s);
			if (s!="" && s[0] != 'c') {
				if (s[0] == 'p') {
					s = s.substr(6);
					sstream.str(s);
					sstream >> this->nliteral;
					sstream >> this->nclause;
					this->clauses = new clause *[nclause];
				}
				else {
					this->clauses[clausen] = new clause(s, sstream, this->nliteral);
					clausen++;
					cout << "\rParsering File... Number of clauses=" << clausen << flush << " Loading Status: " << 100 * clausen / this->nclause << "%";
				}
			}
		}
		cout << endl;
		cout << "CNF Loading finished!" << endl;
	}

	CNF_status evaluate(varset& vset) {
		bool false_exist = false;
		for (int i = 0; i < this->nclause; i++) {
			clause_status cl_status = this->clauses[i]->evaluate(vset);
			if (cl_status.status == undecided) {
				return CNF_status{ false,undecided };
			}
			else {
				if (cl_status.value == false) {
					false_exist = true;
				}
			}
		}
		if (false_exist == true) {
			return CNF_status{ false,decided };
		}
		else {
			return CNF_status{ true,decided };
		}
	}

	void show() {
		for (int i = 0; i < this->nclause; i++) {
			(*this->clauses[i]).show();
		}
	}

	bool DPLL_branch_search(varset& vset,int i,bool set,bool trace) {
		vset.shaders[i] = decided;
		vset.values[i] = set;
		if (trace) {
			cout << "switch to";
			vset.show();
		}
		if (i == vset.num - 1) {
			CNF_status cnfstatus = (this->evaluate(vset));
			if (cnfstatus.status == undecided) {
				cout << "Wrong evaluation!" << endl;
			}
			if (trace) {
				cout << "The following node has been searched:" << endl;
				vset.show();
				cout << "The result is:" << cnfstatus.value << endl;
			}
			if (cnfstatus.value == false) {
				vset.shaders[i] = undecided;
			}
			return cnfstatus.value;
		}
		if (vset.shaders[i + 1] == undecided) {
			bool flag=this->DPLL_branch_search(vset, i + 1, false,trace);
			if (flag == true) {
				return true;
			}
			else {
				flag = this->DPLL_branch_search(vset, i + 1, true,trace);
				if (flag == true) {
					return true;
				}
				else {
					vset.shaders[i] = undecided;
					return false;
				}
			}
			
			

		}
		else {
			bool flag = this->DPLL_branch_search(vset, i + 1, vset.values[i + 1], trace);
			if (flag == true) {
				return true;
			}
			else {
				vset.shaders[i] = undecided;
				return false;
			}
		}

	}

	bool DPLL_search(varset& vset,bool trace) {
		bool flag = this->DPLL_branch_search(vset, 0, false, trace);
		if (flag == true) {
			return true;
		}
		flag = this->DPLL_branch_search(vset, 0, true, trace);
		if (flag == true) {
			return true;
		}
		cout << "Unsatisfiable!" << endl;
		return false;
	}
};