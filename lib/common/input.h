#ifndef INPUT
#define INPUT

#include <string>

struct searchInfo{
	std::string inputfile, outputfile, queryfile, algorithm, metric;
	int M, probes, L, d, n, k;
    double delta;
    bool continuous;
};

struct clusterInfo{
	int n, d;
	std::string inputfile, outputfile, configurationfile, assignment_method, update_method;
	bool complete = false, silhouettes_enabled = false;
};

struct searchInfo getSearchInfo(int argc, char *argv[]);
struct clusterInfo getClusterInfo(int argc, char *argv[]);

#endif