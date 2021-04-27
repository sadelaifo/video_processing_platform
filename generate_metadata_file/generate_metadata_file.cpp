#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
using namespace std;

//TODO: change me
const string distribution = "normal distribution, sigma=size / 5";
const int nrolls = 100000;

int main() {
	std::default_random_engine generator;
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
	// TODO: change me
	std::normal_distribution<double> d(v.size() / 2, v.size() / 5);
	int* a = new int[v.size()];
//	cout << 

	outputfile << distribution << endl;

	size_t i = 0;
	int min1 = nrolls;
	while (i < nrolls) {
		size_t tmp = (size_t) round(d(generator));
//		cout << tmp << endl;
		if (tmp >= 0 && tmp <= v.size() - 1) {
			outputfile << v[tmp] << endl;
			a[tmp]++;
			i++;
		}
	}

	

	for (i = 0; i < v.size(); i++) {
		if (a[i] < min1) {
			min1 = a[i];
		}
	}
	min1 = min1 > 0 ? min1 : 1;
	for (i = 0; i < v.size(); i++) {
		cout << setw(10) << left <<  v[i];
		for (int j = 0; j < a[i] / 250 ; j++) {
			cout << "*";
		}
		cout << endl;
	}

	inputfile.close();
	outputfile.close();
	return 0;
}
