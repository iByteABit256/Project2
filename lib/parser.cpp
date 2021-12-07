#include <iostream>
#include <string>
#include <fstream>

#include "parser.h"
#include "cluster.h"

using namespace std;

vector<Point *> parseinputfile(string inputfile,int &d,int &n){

	string line;
	vector<Point *> points;

	fstream input;

	// Tab seperated dimensions (?)
	string delimiter = " ";

	input.open(inputfile);
	if(!input){
		cerr << "Input file can't be opened" << endl;
		return points;
	}

	int npoints = 0, i = 0, j;
	size_t pos;

	if(input.is_open()){
		// Parse file
		while(getline(input,line)){
			npoints++;
			points.resize(npoints);
			j = 0;
			pos = 0;
			string token;
			string index;
			vector<float> coords;

			while((pos = line.find(delimiter)) != string::npos){
				if(j++==0){
					// Ignore first column
					index = line.substr(0, pos);
					line.erase(0, pos + delimiter.length());
					continue;
				}
				token = line.substr(0, pos);
				
				// Add float to point
				coords.push_back(stof(token));
				
				line.erase(0, pos + delimiter.length());
			}
			// Add float after last delimiter - removed because of space after last coord
			// coords.push_back(stof(line));

			if(i==0){
				// Set dimension according to first row
				d = j-1;
			}
			
			if(j-1 != d){
				cerr << "Invalid point " << index << ", ignoring." << endl;
				continue;
			}
			points[i] = new Point(coords,index);
			i++;
		}
		input.close();
		n=i;
	}

	return points;
}

struct Config parseConfig(std::string configfile){
	string line;
	fstream input;
	size_t pos;
	string delimiter = ":";
	
	struct Config conf = {-1,-1,-1,-1,-1,-1};

	input.open(configfile);

	if(input.is_open()){
		while(getline(input,line)){
			pos = line.find(delimiter);
			if(pos == string::npos){
				cerr << "Invalid line in config file, ignoring." << endl;
				continue;
			}
			
			string item = line.substr(0,pos);	
			int value;
			
			try{
				value = stoi(line.substr(pos+1));
			}catch(exception& e){
				cerr << "Invlid config value: " << e.what() << endl;
				cerr << "Ignoring." << endl;
				continue;
			}
			
			if(item == "number_of_clusters"){
				conf.number_of_clusters = value;
			}else if(item == "number_of_vector_hash_tables"){
				conf.number_of_vector_hash_tables = value;
			}else if(item == "number_of_vector_hash_functions"){
				conf.number_of_vector_hash_functions = value;
			}else if(item == "max_number_M_hypercube"){
				conf.max_number_M_hypercube = value;
			}else if(item == "number_of_hypercube_dimensions"){
				conf.number_of_hypercube_dimensions = value;
			}else if(item == "number_of_probes"){
				conf.number_of_probes = value;
			}else{
				cerr << "Invalid item in config file, ignoring." << endl;
			}
		}
		input.close();
	}else{
		cerr << "Could not open config file" << endl;
	}
	
	return conf;
}
