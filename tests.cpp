#include "tests.h"

void print_database(leveldb::DB* db, string key) {
	string token;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &token);

	if (!s.ok()) {
		cerr << "Failed to open db\n";
		return;
	}
	video_value_struct obj;
	unmarshall(token, &obj);

	print_video_struct_object(obj);	
}

void print_video_struct_object(video_value_struct& video_object_2) {
	cout << "Parsing metadata: \n";
	cout << "Name:          " << video_object_2.name << endl;
	cout << "path:          " << video_object_2.path << endl;
	cout << "timestamp:     " << asctime(localtime(&video_object_2.timestamp));
	for (size_t i = 0; i < video_object_2.metadata_array.size(); i++) {
		cout << "metadata:              " << video_object_2.metadata_array[i].metadata_name << endl;
		cout << "metadata path:         " << video_object_2.metadata_array[i].metadata_path << endl;
		cout << "timestamp:             " << asctime(localtime(&video_object_2.metadata_array[i].timestamp));
		cout << "access frequency:      " << video_object_2.metadata_array[i].access_frequency << endl;
	}
}

void print_video_struct_object(string obj) {
	video_value_struct video_object_2;
	unmarshall(obj, &video_object_2);
	print_video_struct_object(video_object_2);
}


int test_1(string key1, video_value_struct& video_object, leveldb::DB* db) {

	leveldb::Status status;

	cout << "Test 1: testing database R&W ...\n";

	video_object.name = key1;
	video_object.path = "path_1";
	video_object.add_metadata("human", key1 + "_human_path");
	video_object.add_metadata("car", key1 + "_car_path");
	video_object.add_metadata("apple", key1 + "_apple_path");
	video_object.add_metadata("cloud", key1 + "_cloud_path");
	video_object.add_metadata("dog", key1 + "_dog_path");
	//      video_object.add_metadata("cat", key1 + "_cat_path");

	string token1 = video_object.marshall();

	status = db->Put(leveldb::WriteOptions(), key1, token1);

	if (!status.ok()) {
		cerr << status.ToString() << endl;
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
		return 1;
	} else {
		print_video_struct_object(video_object_2);
	}

	cout << "***********************************" << endl << endl;
	return 0;
}

int test_2(string key1, leveldb::DB* db) {
	cout << "Test 2: testing metadata eviction\n";
	add_metadata(db, key1, "flower", "_flower_path");

	string token3;
	leveldb::Status status = db->Get(leveldb::ReadOptions(), key1, &token3);

	//      cout << token3 << endl;

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		return 1;
	}

	print_video_struct_object(token3);

	cout << "***********************************" << endl << endl;
	return 0;
}

int test_3(string key1, leveldb::DB* db) {
	string token3;
	cout << "Test 3: testing metadata removal\n";
	cout << "...Deleting car...\n";
	delete_metadata(db, key1, "car");

	leveldb::Status status = db->Get(leveldb::ReadOptions(), key1, &token3);

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		return 1;
	}

	print_video_struct_object(token3);

	cout << "***********************************" << endl << endl;
	return 0;
}

int test_4(string key1, leveldb::DB* db) {
	string token3;
	cout << "Test 4: testing metadata read\n";

	get_metadata_path_given_metadata_name(db, key1, "human", &token3);

	cout << "Reading metadata...\n";
	cout << "Video Name:    " << key1 << endl;
	cout << "metadata name: " << "human" << endl;
	cout << "metadata path: " << token3 << endl;
	cout << "***********************************" << endl << endl;

	return 0;
}

// testing metadata eviction policy
int test_5(string key1, leveldb::DB* db) {
	cout << "Test 5: testing metadata replacement\n";

	std::ifstream inputfile;
	inputfile.open ("metadata_file.txt", std::ifstream::in);
	if (inputfile.fail()) {
		cerr << "Failed to open metadata file\n";
		return 1;
	}
//	string replacement_policy;
	string distribution;
	string mdata;
	string mpath = " ";
	int hits = 0;
	int total_reads = 0;
	string prev;
	print_database(db, key1);

	getline(inputfile, distribution);
	cout << "distribution:		" << distribution << endl;
	cout << "Replacement Policy:	" << replacement_policy << endl;
	while (inputfile >> mdata) {
		total_reads++;
		int status = get_metadata_path_given_metadata_name(db, key1, mdata, &mpath);
//		cout << mpath << endl;
		if (verbose) {
			cout << "status = " << status << endl;
		}
		if (status != 0) {
			mpath = key1 + "_" + mdata + "_path";
			add_metadata(db, key1, mdata, mpath);
		} else {
			hits++;
		}
		prev = mdata;
		if (prev == mdata) {
			continue;
		}
	}

	cout << "Total metadata reads:	" << total_reads << endl;
	cout << "Total hits:		" << hits << endl;
	cout << "Hit rate:		" << (double) hits / total_reads << endl;

	inputfile.close();
	return 0;
}

