#include <iostream>
#include <fstream>
#include <vector>
#include <random>
using namespace std;

//TODO: change me
const string distribution = "pure random";
const int nrolls = 100000;
std::default_random_engine generator;

int main() {
	std::ofstream outputfile;
        outputfile.open ("../metadata_file.txt", std::ofstream::out);
        if (outputfile.fail()) {
                cerr << "Failed to open metadata file\n";
		return 1;
	}

	std::ifstream inputfile;
	inputfile.open ("metadata_pool.txt", std::ifstream::in);
	if (inputfile.fail()) {
		cerr << "Failed to open metadata file\n";
		return 1;
	}

	// reading metadata pool
	string mdata;
	vector<string> v;
	while (inputfile >> mdata) {
		v.push_back(mdata);
		//cout << "Reading metadata " << mdata << endl;
	}
	std::normal_distribution<double> d(v.size() / 2, v.size() / 10);

//	cout << 

	outputfile << distribution << endl;

	int i = 0;
	while (i < nrolls) {
		int tmp = (int) round(d(generator));
//		cout << tmp << endl;
		if (tmp >= 0 && tmp <= v.size() - 1) {
			outputfile << v[tmp] << endl;
			i++;
		}
	}
	inputfile.close();
	outputfile.close();
	return 0;
}
