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

/*
 * Class to allow interface with a peertube instance
 */

#include <vector>
#include <string>

#include "backend/Video.hpp"

using namespace std;

class Instance_interface{
        private:
        string m_instance_url; 

        public:
        Instance_interface();

        void set_instance(string instance_url);
        
        vector<Video> search_video(string query);
};
