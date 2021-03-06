#pragma once
#include <gecode/set.hh>
#include <gecode/search.hh>
#include <string>
#include <vector>
#include <iostream>
#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
using namespace Gecode;

class AConstraintGraph : public IntMinimizeSpace 
{
protected:
	int n = 0;
	int classesNumber = 1;
	std::vector<std::string> names;
	SetVarArray classes;
	SetVarArray edges;
	IntVarArray degreeSequence;
	IntVarArray degreeSequenceSorted;
	IntVarArray classesDegreeSequences;
	IntVarArray classesDegreeSequencesSorted;
	IntVarArray classesHashes;
	IntVarArray classesHashesSorted;
	IntVar costValue;
public:
	AConstraintGraph();
	int push_vertex(std::string name);
	void push_edge(int i, int j, int cl);
	void finalize_vertexes();
	void finalize();
	AConstraintGraph(bool share, AConstraintGraph &s);
	virtual Space *copy(bool share) {
		return new AConstraintGraph(share, *this);
	}
	virtual IntVar cost(void) const;
	~AConstraintGraph();
	void applyKDegree(int k);
	void print(std::ostream& os) const;
	void setN(int n) { this->n = n; }
	void setClassesNumber(int n) { this->classesNumber = n; }
	void debug();
};

