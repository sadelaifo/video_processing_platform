#include <iostream>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <leveldb/db.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include "utils.h"
#include <fstream>
#include "tests.h"

int verbose = 0;
int nfiles = 0;
using namespace std;

void build_db(leveldb::DB* db);
void read_db(leveldb::DB* db);
inline string get_key(int video_id, int feature_id);
inline string get_file_name(int i);
const string path = "generate_test_cases/test_cases/";
const int MAX_NUM_FEATURES = 128;
int main(int argc, char* argv[])
{
	if (argc == 1) {
		cout << "Error, Usage: ./main [FileCounts] [Operation]\n";
		cout << "[[FileCounts]] can be get from "<< path << " folder\n";
		cout << "You can get this by running:" << endl;
		cout << "ls | grep file_ -c" << endl;
		cout << "[[Operation]] can be either 'R' or 'W'\n";
		return 1;
	}
	if (argc != 3) {
		nfiles = atoi(argv[1]);
	}
	nfiles = atoi(argv[1]);
	char operation = argv[2][0];
	if (!(operation == 'R' || operation == 'r' || operation == 'W' || operation == 'w')) {
		cout << "Error, Operation can only be R or W\n";
		return 1;
	}

	leveldb::DB *db;
	leveldb::Options options;
	options.create_if_missing = true;

	leveldb::Status status = leveldb::DB::Open(options, "mydb", &db);
	//   assert(status.ok());

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		delete db;
		return 1;
	}
	/*
	   string key1 = "video_1";
	   video_value_struct video_object;

	   if (test_1(key1, video_object, db) != 0) {
	   delete db;
	   return 1;
	   }
	   if (test_2(key1, db) != 0) {
	   delete db;
	   return 1;
	   }
	   if (test_3(key1, db) != 0) {
	   delete db;
	   return 1;
	   }
	   if (test_4(key1, db) != 0) {
	   delete db;
	   return 1;
	   }
	   if (test_5(key1, db) != 0) {
	   delete db;
	   return 1;
	   }
	 */
	if (operation == 'w' || operation == 'W') {
		build_db(db);
	} else {
		read_db(db);
	}
	delete db;

	return 0;
}

void read_db(leveldb::DB* db) {
	int num_operations = 10000000;
	for (int i = 0; i < num_operations; i++) {
		int video_id = (int) rand() % nfiles;
		int feature_id = (int) rand() % MAX_NUM_FEATURES;
		string key = get_key(video_id, feature_id);
		string value;
		leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &value);

		if (!s.ok()) {
			cerr << "Failed to open db\n";
			return;
		}
	}
}

void build_db(leveldb::DB* db) {
	string key;
	string value;
	int nfeatures;
	string fname;
	FILE* featureFile;
	int fd;
	struct stat file_stats;
	for (int i = 0; i < nfiles; i++) {
		nfeatures = (int) rand() % MAX_NUM_FEATURES;
		fname = get_file_name(i);
		featureFile = fopen(fname.data(), "r");
		fd = fileno(featureFile); 
		fstat(fd, &file_stats);
		int file_size = (int) file_stats.st_size;
		char* buf = new char[file_size];
		fread(buf, file_size, sizeof(char), featureFile);
		for (int j = 0; j < nfeatures; j++) {
			key = get_key(i, (int) rand() % MAX_NUM_FEATURES);
			db->Put(leveldb::WriteOptions(), key, string(buf));
		}
		fclose(featureFile);
		delete buf;
	}
}

inline string get_file_name(int i) {
	string fname;
	fname = path;
	fname += string("file_");
	fname += to_string(i);
	fname +=  string(".txt");	
	return fname;
}

inline string get_key(int video_id, int feature_id) {
	string k = to_string(video_id);
	k += to_string(feature_id);
	return k;
}
