#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <stdio.h>

using namespace std;

typedef struct {
	int video_id;
	int feature_id;
} vpmh;

typedef struct {
        int frame_id;           // frame ID
	double conf;            // confidence
	short b0;
	short b1;
	short b2;
	short b3;               // assume this is a bounding box
} bb;

const int nfiles = 1000000; 		// 1M feature files
const int file_size = 16 * 1024; 	// define feature file size
const int max_feature_supported = 128;

void write_to_file(const int fid, const int file_size_this);

int main() {
	std::default_random_engine generator;
	std::normal_distribution<double> d(file_size, file_size / 5);

	for (int i = 0; i < nfiles; i++) {
		int file_size_this = (int) d(generator);
		file_size_this = file_size_this / sizeof(bb) * sizeof(bb);
		file_size_this += sizeof(vpmh);
		write_to_file(i, file_size_this);
	}
}

void write_to_file(const int fid, const int file_size_this) {
	char buf[file_size_this];
	string fname;
	fname += string("test_cases/file_");
	fname += to_string(fid);
	fname +=  string(".txt");
	vpmh* ptr_header = (vpmh*) buf;
	ptr_header->video_id = fid;
	ptr_header->feature_id = (int) rand() % max_feature_supported;
	bb* ptr = (bb*) (buf + sizeof(vpmh));
	int num_bbs = (file_size_this - sizeof(vpmh)) / sizeof(bb);
	//              cout << "file size is " << file_size_this << endl;
	for (int i = 0; i < num_bbs; i++) {
		ptr->frame_id 	= i;
		ptr->conf       = rand();
		ptr->b0         = (int) rand();
		ptr->b1         = (int) rand();
		ptr->b2         = (int) rand();
		ptr->b3         = (int) rand();
		ptr++;
	}
	FILE* outputfile;
	outputfile = fopen(fname.data(), "w");
	fwrite(buf, sizeof(char), file_size_this, outputfile);
	fclose(outputfile);
}
