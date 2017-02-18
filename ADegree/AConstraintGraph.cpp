#include "AConstraintGraph.h"
#include <gecode/gist.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <math.h>

AConstraintGraph::AConstraintGraph()
{
	n = 0;
}

int AConstraintGraph::push_vertex(std::string name)
{
	names.push_back(name);
	return names.size() - 1;
}
void AConstraintGraph::push_edge(int i, int j)
{
	if (i == j)
		return;
	dom(*this, edges[i], SRT_SUP, j);
	dom(*this, edges[j], SRT_SUP, i);
	//this->debug();
}
void AConstraintGraph::finalize_vertexes()
{
	std::cout << "finalize vertexes" << std::endl;
	if (n == 0)
		n = this->names.size();
	//std::cout << n;
	//assert(false);
	edges = SetVarArray(*this, n, IntSet::empty, IntSet(0, n - 1));
	degreeSequence = IntVarArray(*this, n, 0, n - 1);
	degreeSequenceSorted = IntVarArray(*this, n, 0, n - 1);
	costValue = IntVar(*this, 0, (n - 1) * (n - 1));
    channel(*this, edges, edges);
	for (int i = 0; i < n; i++)
	{
		cardinality(*this, edges[i], degreeSequence[i]);
	}
	sorted(*this, degreeSequence, degreeSequenceSorted);
	linear(*this, degreeSequenceSorted, IRT_EQ, costValue);
	//rel(*this, cost = )
	//assert(false);
}

void AConstraintGraph::finalize()
{
	std::cout << "finalize" << std::endl;
	//cardinality(*this, in[0], 3, 3);
	//no self loops
	for (int i = 0; i < n; i++)
		dom(*this, edges[i], SRT_DISJ, i);
	//branch(*this, out, SET_VAR_NONE(), SET_VAL_MIN_INC());
}

AConstraintGraph::AConstraintGraph(bool share, AConstraintGraph & s) : IntMinimizeSpace(share, s)
{
	//std::cout << "copy" << std::endl;
	edges.update(*this, share, s.edges);
	costValue.update(*this, share, s.costValue);
	degreeSequence.update(*this, share, s.degreeSequence);  
	degreeSequenceSorted.update(*this, share, s.degreeSequenceSorted);
	//c.update(*this, share, s.c);
	//debug();
}

IntVar AConstraintGraph::cost(void) const
{
	return costValue;
}

AConstraintGraph::~AConstraintGraph()
{
}

void AConstraintGraph::applyKDegree(int k)
{
	for (int i = 0; i < n; i++)
	{
		count(*this, degreeSequenceSorted.slice(fmax(0, i - k + 1), 1, 2 * k - 1), degreeSequenceSorted[i], IRT_GQ, k);
	}
	int *koef = new int[n]; 
	for (int i = 0; i < n; i++)
	{

		for (int j = 0; j < n; j++)
		{
			if (j < n - i)
				koef[j] = -1;
			else
				koef[j] = 1;
		}
		linear(*this,
			IntArgs(n, koef),
			degreeSequenceSorted,
			IRT_LQ,
			i * (i - 1));
	}
	delete[] koef;
	//nvalues(*this, degreeSequenceSorted, IRT_LQ, n / k);
	//branch(*this, edges, SET_VAR_NONE(), SET_VAL_MIN_EXC());
	branch(*this, degreeSequenceSorted, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, edges, SET_VAR_NONE(), SET_VAL_MIN_INC());
}

void AConstraintGraph::debug()
{
	print(std::cout);
}
void AConstraintGraph::print(std::ostream& os) const {
	os << edges << std::endl;
	os << degreeSequence << std::endl;
	os << degreeSequenceSorted << std::endl;
	os << costValue << std::endl;
}