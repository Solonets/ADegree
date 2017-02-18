#include "SampleLoader.h"




void SampleLoader::load_names()
{
	this->push_vertex("A");
	this->push_vertex("B");
	this->push_vertex("C");
	this->push_vertex("D");
}

void SampleLoader::load_edges()
{
	this->push_edge("A", "B");
	this->push_edge("B", "C");
	this->push_edge("C", "D");
	this->push_edge("A", "C");
	this->push_edge("B", "A");
}
