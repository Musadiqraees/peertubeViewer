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

#include <string>

using namespace std;

//Class that represents one video file
//Stores its url and its quality a

class Video_file {
        private:
        string m_file_url;
        string m_torrent_url;
        string m_resolution_label;

        public:
        Video_file(string file_url,string torrent_url,string label);

        string get_resolution_label();
        string get_torrent_url();
        string get_file_url();

};
