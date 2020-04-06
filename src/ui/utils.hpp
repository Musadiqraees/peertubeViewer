/*
 Copyright 2019 Sosthène Guédon

 This file is part of peertube-viewer.

    peertube-viewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License

    peertube-viewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with peertube-viewer.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>
#include <iostream>
#include <tuple>

using namespace std;

void print_centered(string to_print,int screen_width,ostream &output_stream = cout);
bool is_int(string str);

//make a date from the peertube api (ex :2063-04-04T20:20:20.640Z) readable by a human
string clean_date(string date);

string duration_to_string(unsigned int duration);


bool is_url(string to_test);

tuple<string,string> get_instance_and_uuid_from_url(string url);


