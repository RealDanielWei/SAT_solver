#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

struct varset {
	int num;
	bool *values;
};

class clause {
public:
	int termnum;
	int* values;
	clause(string s,stringstream& strstream,int nliterals) {
		strstream.clear();
		strstream.str(s);
		int index, n = 0,*temp=new int[nliterals];
		while (strstream>>index,index!=0)
		{
			temp[n] = index;
			n++;
		}
		this->termnum = n;
		this->values = (int*)malloc(n * sizeof(int));
		for (int i = 0; i < n; i++) {
			this->values[i] = temp[i];
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
			if (s[0] != 'c') {
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
					
				}
			}
		}
	}

	void show() {
		for (int i = 0; i < this->nclause; i++) {
			(*this->clauses[i]).show();
		}
	}
};