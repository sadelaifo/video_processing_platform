#include "utils.h"

video_value_struct::video_value_struct() {
	name = '\0';
	path = '\0';
	metadata_array.clear();
	timestamp = static_cast<long int> (time(NULL));
}

video_value_struct::video_value_struct(string name, string path) {
	this->name = name;
	this->path = path;
	metadata_array.clear();
	timestamp = static_cast<long int> (time(NULL));
//	timestamp = std::chrono::system_clock::now();
}

video_value_struct::~video_value_struct(){
	name.clear();
	path.clear();
	metadata_array.clear();
	timestamp = 0;
}

int video_value_struct::clear() {
	name.clear();
	path.clear();
	metadata_array.clear();
	timestamp = 0;
	return 0;
}

int video_value_struct::add_metadata(string metadata_name, string metadata_path) {
//	if (this->

	video_metadata_struct metadata;
	metadata.metadata_name = metadata_name;
	metadata.metadata_path = metadata_path;
	metadata.timestamp =  static_cast<long int> (time(NULL));
	metadata.access_frequency = 0;
	metadata_array.push_back(metadata);

//decide_eviction_victim

	return 0;
}

string video_value_struct::marshall() {
//	typedef duration <int, std::ratio<1> > seconds_type;

	std::string token;
	token =  this->name + delimiter;
       	token += this->path + delimiter;
	token += to_string(this->timestamp) + delimiter;
	size_t counter = 0;
	while (this->metadata_array.size() > counter) {
		token += this->metadata_array[counter].metadata_name + delimiter;
		token += this->metadata_array[counter].metadata_path + delimiter;
		token += to_string(this->metadata_array[counter].timestamp) + delimiter;
		token += to_string(this->metadata_array[counter].access_frequency) + delimiter;
		counter++;
	}
	
	return token;
}

int unmarshall(string& s, video_value_struct* obj) {

	size_t pos = 0;
	std::string token;

	// get name
	if ((pos = s.find(delimiter)) == std::string::npos) {	
		return 1;
	}
	token = s.substr(0, pos);
	obj->name = token;
	s.erase(0, pos + delimiter.length());

	// get path
	if ((pos = s.find(delimiter)) == std::string::npos) {
		return 1;
	}	
	token = s.substr(0, pos);
	obj->path = token;
	s.erase(0, pos + delimiter.length());

	// get timestamp
        if ((pos = s.find(delimiter)) == std::string::npos) {
                return 1;
        }
        token = s.substr(0, pos);
        obj->timestamp = (long int) stoi(token);
        s.erase(0, pos + delimiter.length());

	while ((pos = s.find(delimiter)) != std::string::npos) {
		video_metadata_struct tmp;

		// get metadata 1 name
		token = s.substr(0, pos);
		tmp.metadata_name = token;
		s.erase(0, pos + delimiter.length());

		// get metadata 1 path
		if ((pos = s.find(delimiter)) == std::string::npos) {
			return 1;
		}
		token = s.substr(0, pos);
		tmp.metadata_path = token;
		s.erase(0, pos + delimiter.length());

		// get metadata 1 timestamp
                if ((pos = s.find(delimiter)) == std::string::npos) {
                        return 1;
                }
                token = s.substr(0, pos);
                tmp.timestamp = (long int) stoi(token);
                s.erase(0, pos + delimiter.length());

		// get metadata 1 access frequency
                if ((pos = s.find(delimiter)) == std::string::npos) {
                        return 1;
                }
                token = s.substr(0, pos);
                tmp.access_frequency = stoi(token);
                s.erase(0, pos + delimiter.length());

		obj->metadata_array.push_back(tmp);
	}
	return 0;
}

int marshall(string& token, video_value_struct* obj) {
	token =  obj->name + delimiter;
	token += obj->path + delimiter;
        token += to_string(obj->timestamp) + delimiter;
	size_t counter = 0;
	while (obj->metadata_array.size() > counter) {
		token += obj->metadata_array[counter].metadata_name + delimiter;
		token += obj->metadata_array[counter].metadata_path + delimiter;
		token += to_string(obj->metadata_array[counter].timestamp) + delimiter;
		token += to_string(obj->metadata_array[counter].access_frequency) + delimiter;
		counter++;
	}

	return 0;
}

int add_metadata(leveldb::DB* db, string key, string metadata_name, string metadata_path) {
	std::string token;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &token);
	
	video_value_struct obj;
	unmarshall(token, &obj);

	if (obj.metadata_array.size() >= max_num_metadata_types) {
		std::string victim = decide_eviction_victim(&obj);
		if (verbose) {
			cout << "[INFO] metadata >= 5. Removing existing metadata " << victim << " for space\n";
		}
		for (size_t i = 0; i < obj.metadata_array.size(); i++) {
			if (obj.metadata_array[i].metadata_name == victim) {
				obj.metadata_array.erase(obj.metadata_array.begin() + i);
				break;
			}
		}
		
		if (marshall(token, &obj) != 0) {
			return 1;
		}		
	}

	if (!s.ok()) {
		return 1;
	}

	video_metadata_struct vms;
	vms.metadata_name = metadata_name;
	vms.metadata_path = metadata_path;
	vms.timestamp = static_cast<long int> (time(NULL));
	vms.access_frequency = 0;

	obj.metadata_array.push_back(vms);

	marshall(token, &obj);

	s = db->Delete(leveldb::WriteOptions(), key);
	if (!s.ok()) {
		return 1;
	}

	s = db->Put(leveldb::WriteOptions(), key, token);
	if (!s.ok()) {
		return 1;
	}

	return 0;
}

int delete_metadata(leveldb::DB* db, string key, string metadata_name) {
	// read database and delete metadata entry	
	size_t pos = 0;
	std::string token;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &token);
	
	if (!s.ok()) {
		return 1;
	}

	// find loc of metadata_name
	if ((pos = token.find(metadata_name)) == std::string::npos) {
                return 1;
        }	
	
	string new_token = token.substr(0, pos);

	// find loc of metadata_path
	if ((pos = token.find(delimiter, pos + metadata_name.length() + delimiter.length())) == std::string::npos) {
		return 1;
	}
	
	// find loc of timestamp
        if ((pos = token.find(delimiter, pos + delimiter.length())) == std::string::npos) {
                return 1;
        }
	// find loc of access_frequency
	if ((pos = token.find(delimiter, pos + delimiter.length())) == std::string::npos) {
                return 1;
        }

	assert(pos + delimiter.length() < token.length());
	new_token += token.substr(pos + delimiter.length(), token.length() - 1); 
	
	if (verbose) {
		cout << new_token << endl;
	}
	// TODO: remove actual metadata path
	
	// write deleted entry string back to database
	s = db->Put(leveldb::WriteOptions(), key, new_token);
	if (!s.ok()) {
		return 1;
	}

	return 0;
}

double compute_f_score(size_t timestamp, size_t access_frequency) {
//	return (int) ((double)timestamp * weights[0] + (double)access_frequency * weights[1]) / 2;
//	double f2 = (double) access_frequency;
	double f1 = (double) timestamp;
	return -f1;
}

// evict entry with largest f score
std::string decide_eviction_victim(video_value_struct* obj) {
	size_t id = 0;
	double max_score = 0;
	double f_score = 0;
	for (size_t i = 0; i < obj->metadata_array.size(); i++) {
		f_score = compute_f_score(obj->metadata_array[i].timestamp, obj->metadata_array[i].access_frequency);
		if (f_score >= max_score) {
			max_score = f_score;
			id = i;
		}
	}
	return obj->metadata_array[id].metadata_name;
}

int get_metadata_path_given_metadata_name(leveldb::DB* db, string key, string metadata_name, string* metadata_path) {
	string token;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &token);
	if (!s.ok()) {
		return 1;
	}
	
	video_value_struct obj;
	if (unmarshall(token, &obj) != 0) {
		return 1;
	}
	for (size_t i = 0; i < obj.metadata_array.size(); i++) {
		if (metadata_name == obj.metadata_array[i].metadata_name) {
			*metadata_path = obj.metadata_array[i].metadata_path;
			obj.metadata_array[i].access_frequency += 1;
			marshall(token, &obj);
			db->Put(leveldb::WriteOptions(), key, token);
			return 0;
		}
	}
	return 1;
}
