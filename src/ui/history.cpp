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

#include "history.hpp"
#include <iostream>
#include <fstream>

History::History()
{
}

void History::set_path(string cmd_path,string viewing_path)
{
        m_cmd_file_path = cmd_path;
        m_viewing_file_path = viewing_path;
        import_cmd();
        import_viewing();
}

void History::import_cmd()
{
        ifstream cmd_file(m_cmd_file_path);
        string line ="";
        if(cmd_file.is_open()){
                while (getline(cmd_file, line))
                {
                        m_cmd_values.push_back(line);
                }
                cmd_file.close();
        }
}

void History::import_viewing()
{
        ifstream view_file(m_viewing_file_path);
        string line ="";
        if(view_file.is_open()){
                while (getline(view_file, line))
                {
                        m_viewing_values.emplace(line);
                        m_viewing_values_ordered.push_back(line);
                }
                view_file.close();
        }
}

bool History::is_viewed(string video_url)
{
        return m_viewing_values.count(video_url) > 0;
}

const vector<string>& History::get_cmd_hist()
{
        return m_cmd_values;
}


void History::add_cmd(string cmd)
{
        m_cmd_values.push_back(cmd);
}

void History::add_view(string video_url)
{
        m_viewing_values.emplace(video_url);
        m_viewing_values_ordered.push_back(video_url);
}

void History::save(unsigned int max_lines)
{
        ofstream viewing_file;
        viewing_file.open(m_viewing_file_path);
        unsigned int begin = m_viewing_values_ordered.size() > max_lines ? m_viewing_values_ordered.size() -max_lines:0;
        for(;begin<m_viewing_values_ordered.size();begin++){
                viewing_file << m_viewing_values_ordered[begin]<<endl;
        }
        viewing_file.close();
        
        ofstream cmd_file;
        cmd_file.open(m_cmd_file_path);
        begin = m_cmd_values.size() > max_lines ? m_cmd_values.size() -max_lines:0;
        for(;begin<m_cmd_values.size();begin++){
                cmd_file << m_cmd_values[begin]<<endl;
        }
        cmd_file.close();
}

