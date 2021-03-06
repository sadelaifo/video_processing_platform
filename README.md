# video_processing_platform

### Usage & Introduction:
1. Folder 'generate_metadata_file' contains the way to generate metadata testing file
2. Open file 'metadata_pool.txt' and edit the pools of metadata entries
3. Open file 'generate_metadata_file' and edit parameters listed there. This file generates metadata read queries used in test_5 of the main program. You may edit npoll -- # of queries, distribution -- name of the random distribution, d -- distribution function (normal distribution by default).
4. Once finishing editing, do
```
make
./generate_metadata_file
```
This will generate a metadata query file in the top directory. 

### Start demo
1. To view the demo, simply

```
make
./main
```

### Dependencies
This project is tested functional on Ubuntu 18.04.

This project depends on leveldb. To correctly install levelDB:

0. Install snappy on your machine
```
sudo apt-get install libsnappy-dev
```
1. follow github readme.md to build leveldb:
	https://github.com/google/leveldb
	
2. go to "Project_directory/build/" created in step 1, and copy libleveldb.* to "/usr/lib/"
```
sudo cp libleveldb.* /usr/lib/
```
3. go to "Project_directory/include/", and copy entire 'leveldb/' directory to '/usr/include/'
```
sudo cp -r leveldb /usr/include/
```
4. In makefile, when you compile, add '-lpthread' '-lsnappy' '-lleveldb' flag.

### Tests:
This demo contains:
1. Adding a new video entry & Adding up to 5 types of metadata
2. Deleting a victim metadata when adding 6th metadata
3. Deleting a metadata based on user needs.
4. Reading a single metadata.
5. Metadata eviction/replacement test.

### Sample output:
```
./generate_metadata_file
```
![image](https://user-images.githubusercontent.com/16581163/116299424-aebea100-a76b-11eb-8021-6b4c8419021a.png)

metadata file being generated can be viewed in 'metadata_file.txt'
```
./main
```
too long to display



