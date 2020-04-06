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

#include "Config.hpp"
#include <iostream>
#include <exception>
#include <boost/program_options.hpp>
#include "ui/version.hpp"
#include "utils/utils.hpp"

namespace po = boost::program_options;

Config::Config()
{
        m_instance = "framatube.org";
        m_player= "mpv";
        m_player_args= "";
        m_use_raw_url = false;
        m_torrent_downloader = "transmission-cli";
        m_torrent_downloader_args = "";
}

void Config::set_path(string path)
{
        parse_config_file(path);
}

void Config::parse_config_file(string path)
{

        ifstream file(path);
        string line = "";
        if(file.is_open()){
                while (getline(file, line))
                {
                        tuple<string,string> res = split_at_first(line, string("="));
                        if(comp_str_ignore_spaces(string("instance"),get<0>(res))){
                                m_instance = get<1>(res);
                                //remove spaces from url
                                auto end_pos = std::remove(m_instance.begin(), m_instance.end(), ' ');
                                m_instance.erase(end_pos, m_instance.end());

                        }
                        if(comp_str_ignore_spaces(string("player"),get<0>(res))){
                                m_player= get<1>(res); 
                        }
                        if(comp_str_ignore_spaces(string("player-args"),get<0>(res))){
                                m_player_args=get<1>(res); 
                        }
                        if(comp_str_ignore_spaces(get<0>(res),string("torrent-downloader"))){
                                m_torrent_downloader= get<1>(res);
                        }
                        if(comp_str_ignore_spaces(get<0>(res),string("torrent-downloader-args"))){
                                m_torrent_downloader_args= get<1>(res);
                        }
                        if(comp_str_ignore_spaces(string("use-raw-url"),get<0>(res))){
                                if(comp_str_ignore_spaces(string("true"),get<1>(res)))
                                        m_use_raw_url = true;
                                else 
                                        m_use_raw_url = false;
                        }
                        if(comp_str_ignore_spaces(string("max-hist-lines"),get<0>(res))){
                                m_max_hist_lines = stoi(get<1>(res));
                        }
        }
                file.close();
        } else {
                cout << "No config file, create one at  ~/.config/peertube-viewer/config" << endl;
                cout << "Using default config" << endl << endl << endl;
        }

}


string Config::get_player()
{
        return m_player;
}
string Config::get_player_args()
{
        return m_player_args;
}

string Config::get_torrent_downloader()
{
        return m_torrent_downloader;
}

string Config::get_torrent_downloader_args()
{
        return m_torrent_downloader_args;
}

bool Config::get_use_torrent()
{
        return m_use_torrent;
}

bool Config::get_select_quality()
{
        return m_select_quality;
}

string Config::get_instance()
{
        return m_instance;
}

bool Config::get_use_raw_url()
{
        return m_use_raw_url;
}

string Config::parse_cmd_options(int argc, char** argv)
{
        po::options_description allowed_options("Options");

        po::positional_options_description query_options;
        query_options.add("query", -1);

        allowed_options.add_options()
                ("help,h", "produce this help message")
                ("version,v", "display version number")
                ("query,q",po::value<vector<string> >(), "the initial query to be loaded if it is a url, it will try to play it as a video")
                ("instance", po::value<string>(), "instance to be browsed")
                ("player", po::value<string>(), "player to play the videos with")
                ("player-args", po::value<string>(), "arguments to be passed to the player")
                ("torrent-downloader", po::value<string>(), "choose the torrent software to download the videos with")
                ("torrent-downloader-args", po::value<string>(), "arguments to be passed to the torrent downloader")
                ("use-raw-url", "the raw url will be passed to the player. It may be neccessary for players without native support for peertube such as vlc. Some players (ex : mpv) may be able to show the video title in their interface if this option isn't used")
                ("select-quality,s", "When playing a video with this option, the user will be prompted to chose the video quality")
                ("use-torrent", "will download the video via the torrent downloader instead of playing it")
                ("print-default-config", "print the default confing to stdout");

        po::variables_map options;
        try{
                po::store(po::command_line_parser(argc, argv).options(allowed_options).positional(query_options).run(), options);
                po::notify(options);    
        }catch(exception &e){
                cerr << e.what() << endl;
                cout << allowed_options;
                exit(0);
        }

        if(options.count("help")){
                cout << allowed_options << endl;
                cout << "Exemple : peertube-viewer blender video --instance=video.blender.org --player=mpv" << endl;
                cout << "you can set the player and the default instance in ~/.config/peertube-viewer/config" << endl;

                exit(0);
        }
        if(options.count("version")){
                cout << "Version :" << VERSION << endl;
                exit(0);
        }

        if(options.count("use-raw-url"))
                m_use_raw_url = true;

        if(options.count("use-torrent"))
                m_use_torrent = true;

        if(options.count("select-quality"))
                m_select_quality = true;

        if(options.count("print-default-config")){
                cout << "player=mpv" << endl;
                cout << "player-args=" << endl;
                cout << "torrent-downloader=transmission-cli" << endl;
                cout << "torrent-downloader-args=" << endl;
                cout << "instance=framatube.org"<<endl;
                cout << "max-hist-lines=1000"<<endl;
                cout << "use-raw-url=false";
                exit(0);
        }

        if(options.count("player"))
                m_player = options["player"].as<string>();
        if(options.count("torrent-downloader"))
                m_torrent_downloader= options["torrent-downloader"].as<string>();
        if(options.count("player-args"))
                m_player_args = options["player-args"].as<string>();
        if(options.count("torrent-downloader-args"))
                m_torrent_downloader_args= options["torrent-downloader-args"].as<string>();
        if(options.count("instance"))
                m_instance = options["instance"].as<string>();

        string query = "";
        if(options.count("query"))
                for(string s : options["query"].as< vector<string> >())
                        query += s + ' ';

        return query;
}

int Config::get_max_hist_lines()
{
        return m_max_hist_lines;
}

void Config::set_blacklist_path(string path)
{
        parse_blacklist_file(path);
}

void Config::parse_blacklist_file(string path)
{

        ifstream file(path);
        string line = "";
        if(file.is_open()){
                if(!getline(file,line)){
                        return;
                }
                if(line=="#whitelist"){
                        m_is_whitelist = true;
                }else {
                        m_listed_instances.insert(line);
                }

                while (getline(file, line))
                {
                        m_listed_instances.insert(line);
                }
                file.close();
        }
}

bool Config::is_blacklisted(string host){
        return !((m_listed_instances.count(host) != 0) ==  m_is_whitelist);
}
