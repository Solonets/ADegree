#include "SampleLoader.h"




SampleLoader::SampleLoader()
{
	this->setClassesNumber(2);
}

void SampleLoader::load_names()
{
	this->push_vertex("1");
	this->push_vertex("2");
	this->push_vertex("3");
	this->push_vertex("4");
	this->push_vertex("5");
	this->push_vertex("6");
	this->push_vertex("7");
	this->push_vertex("8");
}

void SampleLoader::load_edges()
{
	this->push_edge("1", "4", 1);
	this->push_edge("1", "2", 1);
	this->push_edge("1", "5", 0);
	this->push_edge("1", "3", 0);
	this->push_edge("1", "8", 0);
	this->push_edge("2", "4", 0);
	this->push_edge("2", "6", 1);
	this->push_edge("2", "7", 0);
	this->push_edge("3", "5", 1);
	this->push_edge("4", "6", 1);
	this->push_edge("7", "6", 1);
}
