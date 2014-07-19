////////////////////////////////////////////////////////////////////////////////
// Kar Epker's Implementation of Little et. al. TSP Algorithm
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <memory>
#include <string>

#include "graph/factory.hpp"
#include "graph/graph.hpp"
#include "interaction.hpp"
#include "path.hpp"
#include "tsp_solver/factory.hpp"
#include "tsp_solver/tsp_solver.hpp"
#include "util.hpp"

#include <gflags/gflags.h>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;

static bool ValidateGraph(const char* flag_name, const string& value);
static bool ValidateSolver(const char* flag_name, const string& value);

DEFINE_string(graph, "manhattan", "Type of graph to construct");
DEFINE_string(solver, "little", "Type of solver to use");

const bool graph_validated{gflags::RegisterFlagValidator(
		&FLAGS_graph, &ValidateGraph)};
const bool solver_validated{gflags::RegisterFlagValidator(
		&FLAGS_solver, &ValidateSolver)};

int main(int argc, char* argv[]) {
	// parse and validate flags
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	if (!graph_validated || !solver_validated)
	{ cerr << "Incorrect graph or solver flag given!" << endl; }

	unique_ptr<Graph> graph{CreateGraph(FLAGS_graph, cin)};
	unique_ptr<TSPSolver> tsp_solver{CreateTSPSolver(FLAGS_solver)};

	// solve the graph
	try {
		cout << tsp_solver->Solve(*graph) << endl;
	} catch (ImplementationError& ie) {
		cerr << "Implementation Error: " << ie.what() << endl;
		return 2;
	} catch (...) {
		cerr << "Unknown Error!" << endl;
		return 3;
	}

	return 0;
}

bool ValidateGraph(const char* flag_name, const string& value)
{ return IsValidGraphType(value); }

bool ValidateSolver(const char* flag_name, const string& value)
{ return IsValidTSPSolverType(value); }
