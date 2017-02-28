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
void AConstraintGraph::push_edge(int i, int j, int cl)
{
	Matrix<SetVarArray> classesMatrix(classes, classesNumber, n);
	if (i == j)
		return;
	//dom(*this, edges[i], SRT_SUP, j);
	//dom(*this, edges[j], SRT_SUP, i);
	dom(*this, classesMatrix(cl, i), SRT_SUP, j);
	dom(*this, classesMatrix(cl, j), SRT_SUP, i);
	//this->debug();
}
void AConstraintGraph::finalize_vertexes()
{
	std::cout << "finalize vertexes" << std::endl;
	if (n == 0)
		n = this->names.size();
	//std::cout << n;
	//assert(false);
	classes = SetVarArray(*this, n * classesNumber, IntSet::empty, IntSet(0, n - 1));
	Matrix<SetVarArray> classesMatrix(classes, classesNumber, n);
	edges = SetVarArray(*this, n, IntSet::empty, IntSet(0, n - 1));
	for (int i = 0; i < n; i++)
	{
		rel(*this, SOT_DUNION, classesMatrix.row(i), edges[i]);
	}
	for (int i = 0; i < classesNumber; i++)
	{
		channel(*this, classesMatrix.col(i), classesMatrix.col(i));
	}
	classesDegreeSequences = IntVarArray(*this, n * classesNumber, 0, n - 1);
	classesDegreeSequencesSorted = IntVarArray(*this, n * classesNumber, 0, n - 1);
	Matrix<IntVarArray> degreeSequencesMatrix(classesDegreeSequences, classesNumber, n);
	Matrix<IntVarArray> degreeSequencesSortedMatrix(classesDegreeSequencesSorted, classesNumber, n);
	int *k = new int[classesNumber];
	int kf = 1;
	for (int i = classesNumber -1; i >= 0; i--, kf *= n)
		k[i] = kf;
	classesHashes = IntVarArray(*this, n, 0, kf);
	classesHashesSorted = IntVarArray(*this, n, 0, kf);
	sorted(*this, classesHashes, classesHashesSorted);
	for (int j = 0; j < n; j++)
	{
		linear(*this, IntArgs(classesNumber, k), degreeSequencesMatrix.row(j), IRT_EQ, classesHashes[j]);
	}
	delete k;
	for (int j = 0; j < classesNumber; j++)
	{
		for (int i = 0; i < n; i++)
		{
			cardinality(*this, classesMatrix(j, i), degreeSequencesMatrix(j, i));
		}
		sorted(*this, degreeSequencesMatrix.col(j), degreeSequencesSortedMatrix.col(j));
	}
	degreeSequence = IntVarArray(*this, n, 0, n - 1);
	degreeSequenceSorted = IntVarArray(*this, n, 0, n - 1);
	costValue = IntVar(*this, 0, n * (n - 1));
    channel(*this, edges, edges);
	for (int i = 0; i < n; i++)
	{
		cardinality(*this, edges[i], degreeSequence[i]);
	}
	sorted(*this, degreeSequence, degreeSequenceSorted);
	//linear(*this, degreeSequenceSorted, IRT_EQ, costValue);
	rel(*this, costValue == degreeSequencesMatrix(0, 0) + degreeSequencesMatrix(0, 1) + degreeSequencesMatrix(0, 2) + degreeSequencesMatrix(0, 7));
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
	degreeSequence.update(*this, share, s.degreeSequence);  
	degreeSequenceSorted.update(*this, share, s.degreeSequenceSorted);
	costValue.update(*this, share, s.costValue);
	classes.update(*this, share, s.classes);
	classesDegreeSequences.update(*this, share, s.classesDegreeSequences);
	classesDegreeSequencesSorted.update(*this, share, s.classesDegreeSequencesSorted);
	classesHashes.update(*this, share, s.classesHashes);
	classesHashesSorted.update(*this, share, s.classesHashesSorted);
	n = s.n;
	classesNumber = s.classesNumber;
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
	Matrix<IntVarArray> degreeSequencesSortedMatrix(classesDegreeSequencesSorted, classesNumber, n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < classesNumber; j++)
		{
			count(*this, degreeSequencesSortedMatrix.slice(j, j + 1, fmax(0, i - k + 1), fmin(n, i + k)), degreeSequencesSortedMatrix(j, i), IRT_GQ, k);
		}
		count(*this, degreeSequenceSorted.slice(fmax(0, i - k + 1), 1, 2 * k - 1), degreeSequenceSorted[i], IRT_GQ, k);
		count(*this, classesHashesSorted.slice(fmax(0, i - k + 1), 1, 2 * k - 1), classesHashesSorted[i], IRT_GQ, k);
	}
	/*for (int i = 0; i < n; i += k)
	{
		count(*this, degreeSequenceSorted.slice(fmax(0, i - k + 1), 1, 2 * k - 1), degreeSequenceSorted[i], IRT_GQ, k);
		//nvalues(*this, degreeSequenceSorted.slice(i, -1, k), IRT_LQ, 2);
		nvalues(*this, degreeSequenceSorted.slice(i, 1, k), IRT_LQ, 2);
	}*/
	/*int *koef = new int[n]; 
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
	*///nvalues(*this, degreeSequenceSorted, IRT_LQ, n / k);
	//branch(*this, edges, SET_VAR_NONE(), SET_VAL_MIN_EXC());
	//branch(*this, degreeSequenceSorted, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
	//branch(*this, degreeSequenceSorted, tiebreak(INT_VAR_DEGREE_MAX(), INT_VAR_NONE()), INT_VAL_MIN());
	//branch(*this, degreeSequenceSorted, INT_VAR_NONE(), INT_VAL_MIN());
	//branch(*this, degreeSequence, INT_VAR_NONE(), INT_VAL_MIN());
	//branch(*this, edges, SET_VAR_NONE(), SET_VAL_MIN_INC());
	branch(*this, classesHashesSorted, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, classesHashes, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, classesDegreeSequencesSorted, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, classesDegreeSequences, INT_VAR_NONE(), INT_VAL_MIN());
	branch(*this, classes, SET_VAR_NONE(), SET_VAL_MIN_INC());
	
}

void AConstraintGraph::debug()
{
	print(std::cout);
}
void AConstraintGraph::print(std::ostream& os) const {
	Matrix<SetVarArray> classesMatrix(classes, classesNumber, n);
	Matrix<IntVarArray> degreeSequencesMatrix(classesDegreeSequences, classesNumber, n);
	Matrix<IntVarArray> degreeSequencesSortedMatrix(classesDegreeSequencesSorted, classesNumber, n);
	for (int i = 0; i < classesNumber; i++)
	{
		os << "class " << (i + 1) << ":" << std::endl;
		os << "  Edges" << classesMatrix.col(i) << std::endl;
		os << "  Degree sequence:" << degreeSequencesMatrix.col(i) << std::endl;
		os << "  Degree sequence sorted :" << degreeSequencesSortedMatrix.col(i) << std::endl;
	}
	os << std::endl << "Edges" << edges << std::endl;
	os <<  "Degree sequence :" << degreeSequence << std::endl;
	os << "Degree sequence sorted :" << degreeSequenceSorted << std::endl;
	os << "Hashes :" << classesHashes << std::endl;
	os << "Hashes Sorted :" << classesHashesSorted << std::endl;
	os << "Cost: " << costValue << std::endl;
}