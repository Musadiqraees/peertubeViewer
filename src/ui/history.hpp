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


#include <vector>
#include <unordered_set>
#include <string>

using namespace std;

//class handling viewing history and search history

class History{
        private:
        string m_cmd_file_path;//path to the command(search) history
        string m_viewing_file_path;//path to the wiewing history

        vector<string> m_cmd_values;//last commands used
        unordered_set<string> m_viewing_values;//viewed videos (their urls)
        vector<string> m_viewing_values_ordered;//viewed videos in order for saving purposes

        void import_cmd();//import command history
        void import_viewing();//import viewing history

        public:
        History();
        void set_path(string cmd_path,string viewing_path);

        bool is_viewed(string video_url);//get if the video has be viewed based on its url
        const vector<string>& get_cmd_hist();//returns the command history

        void add_cmd(string cmd);//adding command
        void add_view(string video_url);//adding view to the history

        void save(unsigned int max_lines);//saves the history to the corresponding files


};
