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

#include <iostream>
#include <string>

#include "ui/Config.hpp"
#include "ui/history.hpp"
#include "backend/Instance_interface.hpp"

using namespace std;

//handles ui

class Peertube_viewer{
        private:
        inline static string query;
        inline static Config config;
        inline static History history;
        inline static Instance_interface instance;
        inline static vector<Video> search_results;
        inline static bool loop_continue;
        inline static bool terminfo_success; //true if terminal information is available

        static void display_search_results();
        static void clear_search_results();
        static void handle_input();
        static bool display_video_info(Video &video,ostream &output_stream = cout);
        static void play_video(Video video,Video_file video_file);
        static void add_history_to_readline();
        static Video_file do_quality_seletion(Video video);

        public:
        static void init(int argc, char **argv);
        static void main_loop();
        static void close();

};
