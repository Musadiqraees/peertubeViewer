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

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <unordered_set>


using namespace std;

/*
 * Represents the current config
 */

class Config
{
        private:
        string m_player;
        string m_player_args;
        string m_instance;
        string m_torrent_downloader;
        string m_torrent_downloader_args;
        unordered_set<string> m_listed_instances;

        bool m_use_torrent = false;
        bool m_select_quality=false;
        bool m_use_raw_url = false;
        bool m_is_whitelist=false;

        int m_max_hist_lines=1000;
        

        void parse_config_file(string path);
        void parse_blacklist_file(string path);
        public:


        Config();
        void set_path(string path); //set path to config file

        string parse_cmd_options(int argc, char** argv);

        string get_instance();
        string get_player();
        string get_player_args();
        string get_torrent_downloader_args();
        string get_torrent_downloader();

        bool get_use_torrent();
        bool get_use_raw_url();
        bool get_select_quality();

        bool print_help();

        int get_max_hist_lines();

        void set_blacklist_path(string path); //set path to instance blacklist file
        bool is_blacklisted(string instance);

};
