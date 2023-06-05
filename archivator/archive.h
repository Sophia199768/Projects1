#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include "hamming.h"
#include "decoding_hamming.h"

class archiver {
private:
    std::string link;
    std::string path_of_archive;
    std::string archiver_name;
    std::vector<std::string> names_of_files;
    std::vector<long long> size_of_files;
    long long header_size;
public:
    archiver(std::vector<std::string> input, std::string path, std::string archive_name);
    void delete_extra_files(std::vector<std::string> unexists_files);
    void get_info();
    void information_about_archive();
    void create_archive();
    void append(std::string new_file);
    void list_of_names();
    void extract(std::vector<std::string> names = {});
    void delete_file(std::string name_of_delete_file);
    void concatenate(std::vector<std::string> names_of_archives, int len_of_codeword);
    void to_binary(unsigned char byte, std::vector<int>& arr);
    int from_binary(std::vector<int> &arr);
    void coding(int len_of_word);
    void decode(int len_of_word);
};