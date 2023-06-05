#include "archive.h"

archiver::archiver(std::vector<std::string> input, std::string path, std::string archive_name) {
    for (std::string i : input) {
        names_of_files.push_back(i); // rewrite all names of files
    }
    path_of_archive = path;
    link = path + '\\';
    link = link + archive_name + ".haf";
    archiver_name = archive_name;
}

void archiver::delete_extra_files(std::vector<std::string> unexists_files) {
    for (int i = 0; i < unexists_files.size(); i++) {
        std::string file_to_delete = path_of_archive + '\\' + unexists_files[i];
        remove(file_to_delete.c_str());
    }
}

void archiver::get_info() {
    char buffer;
    std::string size_of_header;
    std::string info;
    std::string add;
    std::ifstream archive;
    long long head = 0;
    archive.open(link, std::ios_base::in | std::ios_base::binary);
    while (buffer != '|') {
        archive.read(&buffer, 1);
        size_of_header += buffer;
        head++;
    }

    long long size_header = atoll(size_of_header.c_str());
    header_size = size_header + head;

    for (long long i = 1; i < size_header; i++) {
        archive.read(&buffer, 1);
        info += buffer;
    }
    int name_number = 0;
    for (int i = 0; i < info.size(); i++) {
        if (info[i] != '|') {
            add +=info[i];
        } else {
            if (name_number % 2 == 0) {
                names_of_files.push_back(add);
            } else {
                size_of_files.push_back(atoll(add.c_str()));
            }
            name_number++;
            add.clear();
        }
    }

}

void archiver::information_about_archive() {
    std::ifstream read_file;
    std::ofstream information_archive;
    std::string information = path_of_archive + '\\' + "Information.txt";
    std::string all_about_files;
    remove(information.c_str());
    information_archive.open(information, std::ios_base::out | std::ios_base::binary);
    size_of_files.clear();

    for (std::string i : names_of_files) {
        read_file.open(path_of_archive + '\\' + i, std::ios_base::binary | std::ios_base::in | std::ios_base::out);
        read_file.seekg(0, std::ios::end);
        long long count_bytes = read_file.tellg();
        // read_file.seekg(0, std::ios::beg);
        size_of_files.push_back(count_bytes);
        all_about_files += i;
        all_about_files += '|';
        all_about_files += std::to_string(count_bytes);
        all_about_files += '|';
        read_file.close();
    }
    long long size = all_about_files.size() + 1;
    information_archive << size << '|';
    information_archive << all_about_files;
    information_archive.close();
}

void archiver::create_archive() {
    char byte;
    std::ifstream current_file;
    std::ofstream archive;
    std::ifstream information_file;
    information_about_archive();
    std::string information = path_of_archive + '\\' + "Information.txt";
    archive.open(link, std::ios_base::binary | std::ios_base::out);
    information_file.open(information, std::ios_base::binary | std::ios_base::in);
    information_file.seekg(0, std::ios::end);
    long long size_of_information = information_file.tellg();
    information_file.seekg(0, std::ios::beg);

    for (long long i = 0; i < size_of_information; i++) {
        information_file.read(&byte, 1); // & - take an adress
        archive.write(&byte, 1);
    }

    information_file.close();
    remove(information.c_str());

    for (long long i = 0; i < names_of_files.size(); i++) {
        current_file.open(path_of_archive + '\\' + names_of_files[i], std::ios_base::binary | std::ios_base::in);

        for (long long j = 0; j < size_of_files[i]; j++) {
            current_file.read(&byte, 1); // & - take an adress
            archive.write(&byte, 1);
        }
        current_file.close();
    }
    archive.close();
}

void archiver::append(std::string new_file) {
    get_info();
    std::vector<std::string> unexist_files;
    std::ifstream current_file;

    for (int i = 0; i < names_of_files.size(); i++) {
        current_file.open(path_of_archive + '\\' + names_of_files[i], std::ios_base::in);
        if (!(current_file.is_open())) {
            unexist_files.push_back(names_of_files[i]);
        }
    }

    extract();
    names_of_files.push_back(new_file);
    information_about_archive();
    create_archive();
    delete_extra_files(unexist_files);
}

void archiver::list_of_names() {
    get_info();

    for (int i = 0; i < names_of_files.size(); i++) {
        std::cout << names_of_files[i] << "\n";
    }

}

void archiver::extract(std::vector<std::string> names) {
    std::ifstream archive;
    std::ofstream current;

    if (names.empty()) {
        names = names_of_files;
    }

    std::vector<long long> array_sizes = size_of_files;
    char byte;
    archive.open(link, std::ios_base::in | std::ios_base::binary);

    for (int j = 1; j < size_of_files.size(); j++) {
        array_sizes[j] += array_sizes[j - 1];
    }

    for (long long i = 0; i < names.size(); i++) {

        std::string file_name = path_of_archive + '\\' + names[i];
        auto it = find(names_of_files.begin(), names_of_files.end(), names[i]);
        long long index = it - names_of_files.begin(); // calculating the index
        archive.seekg(header_size + array_sizes[index] - size_of_files[index] - 1,  std::ios_base::beg);
        current.open(file_name, std::ios_base::out | std::ios_base::binary);

        for (long long j = 0; j < size_of_files[index]; j++) {
            archive.read(&byte, 1);
            current.write(&byte, 1);
        }
        current.close();
        current.clear();
    }
    archive.close();
}

void archiver::delete_file(std::string name_of_delete_file) {
    get_info();
    std::vector<std::string> unexist_files;
    std::ifstream current_file;

    for (int i = 0; i < names_of_files.size(); i++) {
        current_file.open(path_of_archive + '\\' + names_of_files[i], std::ios_base::in);
        if (!(current_file.is_open())) {
            unexist_files.push_back(names_of_files[i]);
        }
    }

    auto it = find(names_of_files.begin(), names_of_files.end(), name_of_delete_file);
    names_of_files.erase(it);
    extract();
    information_about_archive();
    create_archive();
    delete_extra_files(unexist_files);
}

void archiver::concatenate(std::vector<std::string> names_of_archives, int len_of_codeword) {
    std::string object_1 = names_of_archives[0];
    std::vector<std::string> nam_1 = {};
    archiver archive_1 = {nam_1, path_of_archive, object_1};

    std::string object_2 = names_of_archives[1];
    std::vector<std::string> nam_2 = {};
    archiver archive_2 = {nam_2, path_of_archive, object_2};
    archive_1.decode(len_of_codeword);
    archive_2.decode(len_of_codeword);

    archive_1.get_info();
    archive_1.extract();
    archive_2.get_info();
    archive_2.extract();

    for (std::string i : archive_1.names_of_files) {
        names_of_files.push_back(i);
    }

    for (std::string i : archive_2.names_of_files) {
        names_of_files.push_back(i);
    }

    std::set<std::string> name;

    for (int i = 0; i < names_of_files.size(); i++) {
        name.insert(names_of_files[i]);
    }
    std::copy(name.begin(), name.end(), names_of_files.begin());
    information_about_archive();
    create_archive();
    delete_extra_files(names_of_files); // update
    archive_1.coding(len_of_codeword);
    archive_2.coding(len_of_codeword);
}

void archiver::to_binary(unsigned char byte, std::vector<int>& arr) {
    for (int j = 0; j < 8; j++) {
        if (byte & 1) {
            arr.push_back(1);
        } else {
            arr.push_back(0);
        }
        byte = byte >> 1;
    }
}

int archiver::from_binary(std::vector<int> &arr) {
    int number = 0;
    for (int i = 7; i >= 0; ) {
        if (arr[i] == 1) {
            number += int(pow(2, i));
        }
        i--;
    }
    return number;
}

void archiver::coding(int len_of_word) {
    unsigned char byte;
    char byte_for_read;
    char byte_for_write;

    std::vector<int> word;
    std::vector<int> answer;
    std::vector<int> word_to_hemming(len_of_word);
    std::ifstream archiver_file;
    std::ofstream encode_file;
    std::string name_of_file = path_of_archive + '\\' + archiver_name + "_encoding.haf";

    archiver_file.open(link, std::ios_base::in | std::ios_base::binary);
    encode_file.open(name_of_file, std::ios_base::out | std::ios_base::binary);

    while (!(archiver_file.eof())) {
        archiver_file.read(&byte_for_read, 1);

        if (archiver_file.eof()) {
            break;
        }

        byte = (unsigned char)byte_for_read;
        to_binary(byte, word);

        while (word.size() >= len_of_word) {
            for (int i = 0; i < word_to_hemming.size(); i++) {
                word_to_hemming[i] = word[i];
            }
            word.erase(word.begin(), word.begin() + word_to_hemming.size());
            answer = findHammingCode(word_to_hemming);
            for (int i: answer) {
                byte_for_write = (i == 1 ? '1' : '0');
                encode_file.write(&byte_for_write, 1);
            }
        }
    }

    if (word.size() != 0) { // a special case
        int size = len_of_word - word.size();
        for (int i = 0; i < size; i++) {
            word.push_back(0);
        }
        answer = findHammingCode(word);

        for (int i: answer) {
            byte_for_write = (i == 1 ? '1' : '0');
            encode_file.write(&byte_for_write, 1);
        }
    }
    encode_file.close();
    archiver_file.close();
    std::remove(link.c_str());
    std::rename(name_of_file.c_str(), link.c_str());
}

void archiver::decode(int len_of_word) {
    std::ifstream archiver_file;
    std::ofstream unencode_file;
    int len_of_hamming_word;

    char byte;
    char byte_for_write;
    std::vector<int> buffer_for_hamming;
    std::vector<int> undecoded;
    std::vector<int> buffer_for_char;
    std::string name_of_file = path_of_archive + '\\' + "unencoding_archive.haf";
    archiver_file.open(link, std::ios_base::in | std::ios_base::binary);
    unencode_file.open(name_of_file, std::ios_base::out | std::ios_base::binary);

    int number_parity_bit = 1;

    while (pow(2, number_parity_bit) < (len_of_word + number_parity_bit + 1)) {
        number_parity_bit++; // len of coded word
    }

    len_of_hamming_word = len_of_word + number_parity_bit;

    std::vector<int> for_hamming(len_of_hamming_word);

    while (!(archiver_file.eof())) { // decoding
        archiver_file.read(&byte, 1);

        if (archiver_file.eof()) {
            break;
        }

        byte == '1' ? buffer_for_hamming.push_back(1) : buffer_for_hamming.push_back(0);

        if (buffer_for_hamming.size() == len_of_hamming_word) {
            decode_hamming(buffer_for_hamming, undecoded);
            buffer_for_hamming.clear();
        }

        while (undecoded.size() >= 8) {
            for (int i = 0; i < 8; i++) {
                buffer_for_char.push_back(undecoded[i]);
            }
            byte_for_write = (char)from_binary(buffer_for_char);
            buffer_for_char.clear();
            undecoded.erase(undecoded.begin(), undecoded.begin() + 8);
            unencode_file.write(&byte_for_write, 1);
        }
    }
    archiver_file.close();
    unencode_file.close();
    std::remove(link.c_str());
    std::rename(name_of_file.c_str(), link.c_str());
}