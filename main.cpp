#include <iostream>
#include <string>
#include <assert.h>
#include <leveldb/db.h>
#include <vector>
#include "utils.h"

using namespace std;

int put_leveldb();

int main(void)
{
	leveldb::DB *db;
	leveldb::Options options;
	options.create_if_missing = true;

	leveldb::Status status = leveldb::DB::Open(options, "testdb", &db);
	//   assert(status.ok());

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		delete db;
		return 1;
	}

	string key1 = "video_1";
	video_value_struct video_object;
	
	video_object.name = key1;
	video_object.path = "path_1";
	video_object.add_metadata("human", key1 + "_human_path");

	string token1 = video_object.marshall();

	status = db->Put(leveldb::WriteOptions(), key1, token1);

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		delete db;
		return 1;
	} else {
		cout << "Sucess: Value Stored\n";
	}

	string token2;
	cout << token1 << endl;
	status = db->Get(leveldb::ReadOptions(), key1, &token2);
	cout << token2 << endl;	
	video_value_struct video_object_2;

	if (unmarshall(token2, &video_object_2) != 0) {
		cerr << "unmarshall failed\n";	
	};

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		delete db;
		return 1;
	} else {
		cout << "Name:		" << video_object_2.name << endl;
		cout << "path:		" << video_object_2.path << endl;
		cout << "metadata:	" << video_object_2.metadata_array[0].metadata_name << endl;
		cout << "metadata path:	" << video_object_2.metadata_array[0].metadata_path << endl;
	}

	delete db;

	return 0;
}


