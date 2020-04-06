/*
 Copyright 2019 Sosthène Guédon

 This file is part of peertube-viewer.

    peertube-viewer is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3 of the License

    peertube-viewer is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with peertube-viewer.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "ui/utils.hpp"
#include <iomanip>
#include <boost/regex.hpp>

bool is_int(string str)
{
        if(str.size() == 0)
                return false;
        for(char c:str)
                if(!isdigit(c))
                        return false;

        return true;
}

void print_centered(string to_print,int screen_width,ostream & output_stream)
{
        if( (int) to_print.size() >= screen_width){
                output_stream << to_print.c_str() << endl;
                return;
        }
        
        int before = (screen_width - to_print.size() ) /2;
        
        output_stream << setfill(' ') << setw(before) <<' ';
        output_stream << to_print.c_str() << endl;
}

string clean_date(string date)
{
        string to_ret = "";
        for(char c : date)
                if(c == 'T')
                        to_ret += " at ";
                else if(c == '.')
                        break;
                else
                        to_ret += c;
        return to_ret;
}

string duration_to_string(unsigned int duration)
{
        stringstream to_ret;
        unsigned int hours = duration / 3600;
        unsigned int minutes= ((duration / 60) % 60);
        unsigned int seconds= (duration % 60);



        if(hours > 0){
                if(hours < 10)
                        to_ret << '0' << hours;
                else
                        to_ret << hours;
                to_ret << ':';
        }
        if(minutes < 10)
                to_ret << "0" << minutes;
        else
                to_ret << minutes;

        to_ret << ':';

        if(seconds < 10)
                to_ret << "0" << seconds;
        else
                to_ret << seconds;

        

        return to_ret.str();
}

bool is_url(string to_test)
{
        return boost::regex_match(to_test,boost::regex("[-a-zA-Z0-9@:%_\\+.~#?&//=]{2,256}\\.[a-z]{2,}\\b(\\/[-a-zA-Z0-9@:%_\\+.~#?&//=]*)?"));
}


tuple<string,string> get_instance_and_uuid_from_url(string url)
{
        string instance="";
        string uuid="";


        if(url.size()< 8)
                throw "invalid url";

        auto char_iter = url.begin();
        if(url[5] == 's'){
                char_iter++;
        }
        for(int i =0 ; i<8;i++)
                char_iter ++;

        for(;*char_iter != '/';char_iter++){
                instance += *char_iter;
        }

        auto char_iter_from_end = url.end();
        for(char_iter_from_end-- ;*char_iter_from_end != '/'; char_iter_from_end--){
                uuid = *char_iter_from_end + uuid;
        }

        
        

        return make_tuple(instance,uuid);
}




