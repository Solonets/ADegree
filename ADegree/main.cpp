#include "GraphLoader.h"
#include "TsvLoader.h"
#include "SampleLoader.h"
#include "AConstraintGraph.h"
#include "RandomLoader.h"
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include <gecode/driver.hh>
#include <gecode/gist.hh>
#include <cstdlib>
#include <chrono>

using namespace Gecode;
int main(int argc, char* argv[]) {
	//GraphLoader *gl = new TsvLoader("data/wiki/articles.tsv", "data/wiki/links.tsv");
	//GraphLoader *gl = new SampleLoader();
	GraphLoader *gl = new RandomLoader(100, 40, 105);
	AConstraintGraph *graph = NULL;
	graph = gl->get();
	delete gl;
	graph->applyKDegree(3);
	graph->debug();
	auto t1 = std::chrono::high_resolution_clock::now();
	if (true)
	{
		Search::Options o;
		o.threads = 2;
		BAB<AConstraintGraph> e(graph, o);
		bool solution = false;
		while (AConstraintGraph* s = e.next()) {
			s->debug();
			std::cout << "Solution" << std::endl;
			solution = true;
			delete s;
			//break;
		}
		if (!solution)
		{
			std::cout << "No Solution" << std::endl;
		}
	}
	else
	{
		Gist::Print<AConstraintGraph> p("Print solution");
		Gist::Options o;
		o.inspect.click(&p);
		Gist::bab(graph, o);
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = t2 - t1;
	std::cout << duration.count() <<  " seconds "<< std::endl;
	delete graph;
	//system("pause");
	return 0;
}
