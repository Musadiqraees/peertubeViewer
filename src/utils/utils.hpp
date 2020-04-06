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
#include <tuple>

using namespace std;

void replace_all(string& input,const string& search,const string& replace);

//Compare 2 strings, s2 is stripped of spaces 
bool comp_str_ignore_spaces(string s1,string s2);

tuple<string, string> split_at_first(string s,const string& delimiter);

