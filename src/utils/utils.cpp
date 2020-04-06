
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

#include "utils/utils.hpp"

void replace_all(string& input,const string& search,const string& replace)
{
        size_t pos = 0;
        while ((pos = input.find(search, pos)) != std::string::npos) {
                input.replace(pos, search.length(), replace);
                pos += replace.length();
        }
}

bool comp_str_ignore_spaces(string s1,string s2)
{
        string::iterator it1 = s1.begin();
        string::iterator it2 = s2.begin();
        string::iterator end1 = s1.end();
        string::iterator end2 = s2.end();
        while (true) {
                if(it2 == end2 && it1 == end1){
                        return true;
                }else if(it2==end2){
                        return false;
                }else if(*it1 == *it2){
                        it1++;
                        it2++;
                }else if(*it2 == ' '){
                        it2++;
                }else if(*it1 != *it2){
                        return false;
                }

        }
}

tuple<string, string> split_at_first(string s,const string& delimiter){
        size_t pos = s.find(delimiter);
        if (pos != string::npos)
                return tuple<string, string>(s.substr(0,pos),s.substr(pos+1));
        else 
                return tuple<string, string>(s,string(""));
}
