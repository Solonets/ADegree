#pragma once
#include "AConstraintGraph.h"
#include <string>
#include <map>
class GraphLoader
{
protected:
	AConstraintGraph *graph;
	enum{INIT, NAMES, EDGES} stage;
	std::map<std::string, int> index;
public:
	GraphLoader();
	virtual ~GraphLoader() {};
	virtual void load_names() {};
	virtual void load_edges() {};
	void push_vertex(std::string);
	void push_edge(int i, int j, int cl);
	void push_edge(std::string in, std::string out, int cl);
	void setVertexNumber(int n);
	void setClassesNumber(int n);
	AConstraintGraph* get();
};

