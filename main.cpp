#include <iostream>
#include <string>
#include <assert.h>
#include <leveldb/db.h>
#include <vector>
#include "utils.h"
#include <fstream>
#include "tests.h"

using namespace std;

int main(void)
{
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

	delete db;

	return 0;
}

