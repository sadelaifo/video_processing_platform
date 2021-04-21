#include <iostream>
#include <string>
#include <assert.h>
#include <leveldb/db.h>
#include <vector>
#include "utils.h"

using namespace std;

int put_leveldb();
void print_video_struct_object(video_value_struct& video_object_2);
void print_video_struct_object(string obj);
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
	
	cout << "Test 1: testing database R&W ...\n";

	string key1 = "video_1";
	video_value_struct video_object;
	
	video_object.name = key1;
	video_object.path = "path_1";
	video_object.add_metadata("human", key1 + "_human_path");
	video_object.add_metadata("car", key1 + "_car_path");
	video_object.add_metadata("apple", key1 + "_apple_path");
	video_object.add_metadata("cloud", key1 + "_cloud_path");
	video_object.add_metadata("dog", key1 + "_dog_path");
//	video_object.add_metadata("cat", key1 + "_cat_path");

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
	cout << "Storing marshalled value into database... \n";
	cout << token1 << endl;
	status = db->Get(leveldb::ReadOptions(), key1, &token2);

	cout << "***********************************" << endl;
	cout << "Retrieving database value for video " << key1 << endl;
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
		print_video_struct_object(video_object_2);		
	}

	cout << "***********************************" << endl << endl;

	cout << "Test 2: testing metadata eviction\n";
	add_metadata(db, key1, "flower", "_flower_path");
	
	string token3;
	status = db->Get(leveldb::ReadOptions(), key1, &token3);

//	cout << token3 << endl;

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		delete db;
		return 1;
	}

	print_video_struct_object(token3);

	cout << "***********************************" << endl << endl;
	cout << "Test 3: testing metadata removal\n";
	cout << "...Deleting car...\n";
	delete_metadata(db, key1, "car");

	status = db->Get(leveldb::ReadOptions(), key1, &token3);

        if (!status.ok()) {
                cerr << status.ToString() << endl;
                delete db;
                return 1;
        }

        print_video_struct_object(token3);

	cout << "***********************************" << endl << endl;
	cout << "Test 4: testing metadata read\n";

	get_metadata_path_given_metadata_name(db, key1, "human", &token3);

	cout << "Reading metadata...\n";
	cout << "Video Name: 	" << key1 << endl;
	cout << "metadata name:	" << "human" << endl;
	cout << "metadata path:	" << token3 << endl;

	delete db;

	return 0;
}

void print_video_struct_object(video_value_struct& video_object_2) {
	cout << "Parsing metadata: \n";
	cout << "Name:          " << video_object_2.name << endl;
	cout << "path:          " << video_object_2.path << endl;
	cout << "timestamp:	" << asctime(localtime(&video_object_2.timestamp));
	for (size_t i = 0; i < video_object_2.metadata_array.size(); i++) {
		cout << "metadata:		" << video_object_2.metadata_array[i].metadata_name << endl;
		cout << "metadata path:		" << video_object_2.metadata_array[i].metadata_path << endl;
		cout << "timestamp:		" << asctime(localtime(&video_object_2.metadata_array[i].timestamp));
		cout << "access frequency:	" << video_object_2.metadata_array[i].access_frequency << endl;
	}
}

void print_video_struct_object(string obj) {
	video_value_struct video_object_2;
	unmarshall(obj, &video_object_2);
	print_video_struct_object(video_object_2);
}

