# video_processing_platform

### Usage:
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
This project depends on leveldb. To correctly install levelDB:

0. Install snappy on your machine
```
sudo apt-get install libsnappy-dev
```
1. follow github readme.md:
	https://github.com/google/leveldb
	
2. go to "build/", and copy libleveldb.* to "/usr/lib"
```
sudo cp libleveldb.* /usr/lib
```
3. go to "Project_directory/include", and copy entire 'leveldb/' to '/usr/include/'
```
sudo cp -r leveldb /usr/include/
```
4. In makefile, when you compile, add '-lpthread' '-lsnappy' '-lleveldb' flag.

### Tests:
This demo contains 4 tests
1. Adding a new video entry & Adding up to 5 types of metadata
2. Deleting a victim metadata when adding 6th metadata
3. Deleting a metadata based on user needs.
4. Reading a single metadata.
