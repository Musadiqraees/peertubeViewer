/*
 Copyright 2019 Sosthène Guédon

 This file is part of peertube-viewer.

    peertube-viewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License

    peertube-viewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.  You should have received a copy of the GNU General Public License along with peertube-viewer.  If not, see <https://www.gnu.org/licenses/>.

*/



#include "ui/peertube-viewer.hpp"
#include "ui/utils.hpp"
#include "utils/utils.hpp"


#include <readline/readline.h>
#include <readline/history.h>
#include <iomanip>
#include <term.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;


void Peertube_viewer::init(int argc,char ** argv)
{
        string config_path = getenv("HOME") + string("/.config/peertube-viewer/config");
        string blacklist_path = getenv("HOME") + string("/.config/peertube-viewer/blacklist");
        string cmd_history_path= getenv("HOME") + string("/.cache/peertube-viewer/cmds");
        string viewing_history_path= getenv("HOME") + string("/.cache/peertube-viewer/views");
        string cache_dir = getenv("HOME")+string("/.cache/peertube-viewer");

        //testing cache directory existance
        {
                struct stat buffer;
                if(stat(cache_dir.c_str(),&buffer) != 0)
                        mkdir(cache_dir.c_str(),700);

        }
        history.set_path(cmd_history_path,viewing_history_path);
        config.set_path(config_path);
        config.set_blacklist_path(blacklist_path);

        query = config.parse_cmd_options(argc,argv);
        instance.set_instance(config.get_instance());
        add_history_to_readline();

        char term_buffer[2048];
        terminfo_success = (tgetent (term_buffer, getenv("TERM")) > 0) ? true : false;
}

void Peertube_viewer::main_loop()
{

        string query_without_spaces = query;
        replace_all(query_without_spaces,string(" "),string(""));
        if(is_url(query_without_spaces)){
                tuple<string,string> instance_and_uuid = get_instance_and_uuid_from_url(query_without_spaces);
                Video video_to_play(get<1>(instance_and_uuid), get<0>(instance_and_uuid));
                if(config.is_blacklisted(video_to_play.get_host())) {
                        cerr << "This video is from a blacklisted instance and will not be played" << endl;
                        return;
                }
                display_video_info(video_to_play);
                if(config.get_use_raw_url() || config.get_use_torrent()||config.get_select_quality()){
                        play_video(video_to_play,do_quality_seletion(video_to_play));
                }else {
                        play_video(video_to_play,video_to_play.get_video_files()[0]);
                }
                return;
        }

        loop_continue = true;
        while(loop_continue){
                if(query != ""){
                        try{
                                search_results = instance.search_video(query);
                        } catch(string &e){
                                if(e.find(string("Unable to fetch :")) != string::npos)
                                        cerr << "Connection error while fetching search results : "  << e << endl;

                                else
                                        cerr << "Invalid search results formatting : " << e << endl;
                        }
                        add_history(query.c_str());
                }

                display_search_results();
                handle_input();
                
        }
}

void Peertube_viewer::display_search_results()
{
        clear_search_results();
        for(unsigned int i =0;i< search_results.size();i++ ){
                cout << (char) 27<< "[1m" << i << " : ";

                if(!history.is_viewed(search_results[i].get_watch_url()))
                        cout << (char) 27<< "[0m";//bold if the video has been viewed
                cout << search_results[i].get_title();
                if(history.is_viewed(search_results[i].get_watch_url()))
                        cout << (char) 27<< "[0m";

                string video_info = '['+ duration_to_string(search_results[i].get_duration()) + ']' +search_results[i].get_channel_name() + "--" +search_results[i].get_host();

                cout << "  " << video_info<< endl;
        }
}

                
void Peertube_viewer::handle_input()
{
        char* line = readline(":");
        if(line == nullptr){
                cout << endl;
                loop_continue = false;
                return;
        }
        string user_command(line);

        if(comp_str_ignore_spaces(string(":q"),user_command)){
                loop_continue = false;
                history.add_cmd(user_command);
                return;
        }

        if(is_int(user_command)){
                int user_choice = stoi(user_command);
                if((unsigned int) user_choice >= search_results.size()){
                        query = "";
                        return;
                }
                if(display_video_info(search_results[user_choice])) {
                        if(config.get_use_raw_url() || config.get_use_torrent() || config.get_select_quality())
                                play_video(search_results[user_choice],do_quality_seletion(search_results[user_choice]));
                        else 
                                play_video(search_results[user_choice],search_results[user_choice].get_video_files()[0]);
                }
        }else {
                query = user_command;
                history.add_cmd(user_command);
        }
}

Video_file Peertube_viewer::do_quality_seletion(Video video)
{
        int terminal_width = terminfo_success ?  tgetnum("co") : 20;
        print_centered("VIDEO QUALITY SELECTION",terminal_width,cout);
        cout << setfill('=') << setw(terminal_width) <<'=' << endl;
        vector<Video_file> video_files = video.get_video_files();

        for(unsigned int i =0;i< video_files.size();i++ ){
                cout << (char) 27<< "[1m" << i << " : ";
                cout << (char) 27<< "[0m";
                string file_info = video_files[i].get_resolution_label();
                cout << "  " << file_info<< endl;
        }

        char* line = readline(":");
        if(line == nullptr){
                cout << endl;
                loop_continue = false;
                return video_files[0];
        }
        string user_command(line);

        if(is_int(user_command)){
                int user_choice = stoi(user_command);
                return video_files[user_choice];
        }
        return video_files[0];
}

void Peertube_viewer::play_video(Video video,Video_file video_file)
{
        if(config.is_blacklisted(video.get_host())) {
                return;
        }

        history.add_view(video.get_watch_url());
        string command_args;
        string player_args;
        string player;

        if(config.get_use_torrent()){
                command_args = video_file.get_torrent_url();
                player = config.get_torrent_downloader();
                player_args = config.get_torrent_downloader_args();
        }else{
                player = config.get_player();
                player_args = config.get_player_args();

                if(config.get_use_raw_url())
                        command_args = video_file.get_file_url();
                else
                        command_args = video.get_watch_url();
        }
        pid_t pid = fork();
        int status;
        if(pid == 0){
            if(player_args != ""){
                execlp(player.c_str(),player.c_str(),player_args.c_str(),command_args.c_str(), (char *) NULL);
            }else {
                execlp(player.c_str(),player.c_str(),command_args.c_str(), (char *) NULL);
            }
        }else {
            while(wait(&status) != pid);
        }
} 
bool Peertube_viewer::display_video_info(Video &video,ostream &output_stream)
{
        if(config.is_blacklisted(video.get_host())) {
                cerr << "This video is from a blacklisted instance and will not be played" << endl;
                return false;
        }

        int terminal_width = terminfo_success ?  tgetnum("co") : 20;
        try{
                video.update();
        } catch(string &e){
                if(e.find(string("Unable to fetch :")) != string::npos){
                        cerr << "Connection error  while fetching video information : "  << e << endl;
                return false;
                } else if(e == "error while parsing video information"){
                        cerr << "Could not parse video information : "  << e << endl;
                }
                return false;
        }

        output_stream << setfill('=') << setw(terminal_width) <<'=' << endl;
        output_stream << video.get_title() << endl;
        output_stream << setfill('=') << setw(terminal_width) <<'=' << endl;
        print_centered("DESCRIPTION",terminal_width,output_stream);
        output_stream << setfill('=') << setw(terminal_width) <<'=' << endl;
        output_stream << video.get_full_description() << endl;
        output_stream << setfill('=') << setw(terminal_width) <<'=' << endl;
        print_centered("VIDEO INFORMATIONS",terminal_width,output_stream);
        output_stream << setfill('=') << setw(terminal_width) <<'=' << endl;
        output_stream << (char) 27<< "[1m" << "duration : " << (char) 27<< "[0m" << duration_to_string(video.get_duration()) << endl;
        output_stream << (char) 27<< "[1m" << "views    : " << (char) 27<< "[0m" << video.get_views() << endl;
        output_stream << (char) 27<< "[1m" << "likes    : " << (char) 27<< "[0m" << video.get_likes() << endl;
        output_stream << (char) 27<< "[1m" << "dislikes : " << (char) 27<< "[0m" << video.get_dislikes() << endl;
        output_stream << (char) 27<< "[1m" << "release  : " << (char) 27<< "[0m" << clean_date(video.get_date()) << endl;
        output_stream << (char) 27<< "[1m" << "channel  : " << (char) 27<< "[0m" << video.get_channel_name() << endl;
        output_stream << (char) 27<< "[1m" << "author   : " << (char) 27<< "[0m" << video.get_author_name() << endl;
        output_stream << setfill('=') << setw(terminal_width) <<'=' << endl;
        return true;
} 

void Peertube_viewer::add_history_to_readline()
{
        for(string s:history.get_cmd_hist()){
                add_history(s.c_str());
        }
}

void Peertube_viewer::close()
{
        history.save(config.get_max_hist_lines());
}
void Peertube_viewer::clear_search_results()
{
        for(auto iter = search_results.begin(); iter != search_results.end();iter++){
                if(config.is_blacklisted(iter->get_host())){
                        iter--;
                        search_results.erase(iter+1);
                }
        }
}
