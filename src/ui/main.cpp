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

#include "ui/peertube-viewer.hpp"

int main(int argc, char** argv){
        try{
                Peertube_viewer::init(argc,argv);
                Peertube_viewer::main_loop();
                Peertube_viewer::close();
        }catch (string &e){
                cout << "Fatal error : " << e << endl;
                cout << "Cannot continue " << endl;
                cout << "Please report the error at : https://gitlab.com/SostheneGuedon/peertube-viewer/issues/" << endl;
        }catch (exception &e){
                cout << "Fatal error : " << e.what() << endl;
                cout << "Cannot continue " << endl;
                cout << "Please report the error at : https://gitlab.com/SostheneGuedon/peertube-viewer/issues/" << endl;

        }catch (...){
                cout << "Fatal error, Could not continue " << endl;
                cout << "Please report the error at : https://gitlab.com/SostheneGuedon/peertube-viewer/issues/" << endl;
        }
}
