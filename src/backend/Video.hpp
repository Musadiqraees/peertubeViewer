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
#include <vector>

#include "backend/Video_file.hpp"

/*
 * Class that represent a peertube video an allows to get info on it
 */


using namespace std;

class Video {
        private:
        vector<Video_file> m_video_files; //url to the raw video file and not the webpage
        string m_uuid;
        string m_title;
        string m_host; //instance hosting the video
        string m_short_description;
        string m_full_description;
        string m_channel_name;
        string m_author_name;
        string m_date;

        int m_views;
        int m_likes;
        int m_dislikes;
        int m_duration;

        bool m_has_info; // true if all info has been fetched via update()


        public:
        Video(string uuid, string host,string title="unknwon",string channel="unknown", string author="unknown",int duration=-1);
        void update();// fetches the video info (uuid,description...)

        string get_title();
        string get_full_description();
        string get_short_description();
        string get_watch_url();
        string get_raw_url();

        vector<Video_file> get_video_files();

        string get_torrent_url();
        string get_channel_name();
        string get_author_name();
        string get_date();
        string get_host();

        int get_views();
        int get_duration();
        int get_likes();
        int get_dislikes();

};
