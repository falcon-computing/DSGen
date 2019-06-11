
// This file includes the common functionalities related to file and string

#ifndef _FILE_PARSER_H_INCLUDED_
#define _FILE_PARSER_H_INCLUDED_
#include <stdio.h>
#include <string.h>
#include <cstdlib>

#include <string>
#include <vector>
#include <iostream>
using namespace std;

string read_string_from_file(string sFilename);
void write_string_into_file(string str, string sFilename);
void append_string_into_file(string str, string sFilename);
vector<string> get_files_in_dir(string sDir);
int test_file_for_read(string sFilename);

// Get the sub string from start to the delimited flag
string get_sub_delimited(string str, int start, char delimited,
                         int size = -1); // delimited flag is a char
string get_sub_delimited_sub(string str, int start, string delimited,
                             int size = -1); // delimited flag is a string
string get_sub_delimited_multi(
    string str, int start, string delimited,
    int size = -1); // delimited flag is any char in a string

void str_split(string str, char delimited, vector<string> &sub_strs);
void str_split(string str, string delimited, vector<string> &sub_strs);
vector<string> str_split(string str, char delimited);
vector<string> str_split(string str, string delimited);
vector<vector<string>> str_split_2d(string str, string outer_delimited,
                                    string inner_delimited);
string str_merge(char delimited, const vector<string> &sub_strs);
string str_replace(string str, string org_sub, string new_sub);
string str_remove_space(string str_in);

vector<string> str_vector_add(vector<string> vec0, vector<string> vec1);
vector<string> str_vector_sub(vector<string> vec0, vector<string> vec1);
vector<string> str_vector_and(vector<string> vec0, vector<string> vec1);
int str_vector_find(vector<string> vec, string str); // -1 if not found
void str_vector_reverse(vector<string> &vec);

int isSpace(char c);
int isSpace(string str);

string FormatSpace(string str); // remove spaces and quotations
string RemoveQuote(string str);
void RemoveSpace(string &str);

string get_lower_case(string str);
string get_upper_case(string str);

// string my_itoa(int i);
string my_itoa(long long i);
string my_itoa_hex(int i);
string my_itoa_hex(long long i);
int my_atoi(string s);
int my_atoi_hex(string s); // translate the hex string into integer

double my_atod(string s);
string my_dtoa(double d);

string get_filename_ext(string filename);

#endif //_FILE_PARSER_H_INCLUDED_
