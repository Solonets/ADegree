#include "GraphLoader.h"



GraphLoader::GraphLoader()
{
	this->graph = new AConstraintGraph();
	this->stage = INIT;
}
void GraphLoader::setVertexNumber(int n)
{
	assert(this->stage == INIT || this->stage == NAMES);
	this->stage = NAMES;
	graph->setN(n);
}
void GraphLoader::setClassesNumber(int n)
{
	assert(this->stage == INIT || this->stage == NAMES);
	graph->setClassesNumber(n);
}
void GraphLoader::push_vertex(std::string name)
{
	assert(this->stage == INIT || this->stage == NAMES);
	this->stage = NAMES;
	index[name] = this->graph->push_vertex(name);
}
void GraphLoader::push_edge(int i, int j, int cl)
{
	assert(this->stage == NAMES || this->stage == EDGES);
	if (this->stage == NAMES)
	{
		this->graph->finalize_vertexes();
		this->stage = EDGES;
	}
	graph->push_edge(i, j, cl);
}

void GraphLoader::push_edge(std::string in, std::string out, int cl)
{
	std::map<std::string, int>::iterator it_in = index.find(in);
	std::map<std::string, int>::iterator it_out = index.find(out);
	assert(it_in != index.end() && it_out != index.end());
	//std::cout << it_in->second << " " << it_out->second << std::endl;
	this->push_edge(it_in->second, it_out->second, cl);
}

AConstraintGraph * GraphLoader::get()
{
	this->load_names();
	this->load_edges();
	graph->finalize();
	return graph;
}
