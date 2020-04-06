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
#define CATCH_CONFIG_MAIN
#ifdef CATCH_VERSION_1
#include <catch/catch.hpp>
#else
#include <catch2/catch.hpp>
#endif


#include "backend/Video.hpp"
#include "ui/Config.hpp"
#include "ui/utils.hpp"
#include "utils/utils.hpp"
#include "ui/history.hpp"

#include <sstream>

using namespace std;
/*
 * Unit tests
 */

TEST_CASE("video")
{
        Video video("uuid","instance","video","channel name","account name",3);
        SECTION("title"){
                REQUIRE(video.get_title() == "video" );
        }
        SECTION("url"){
                REQUIRE(video.get_watch_url() == "https://instance/videos/watch/uuid" );
        }
        SECTION("channel name"){
                REQUIRE(video.get_channel_name() == "channel name" );
        }
        SECTION("account name"){
                REQUIRE(video.get_author_name() == "account name" );
        }
        SECTION("duration"){
                REQUIRE(video.get_duration() == 3);
        }
        SECTION("host"){
                REQUIRE(video.get_host() == "instance");
        }
}

TEST_CASE("Config")
{
        Config config; 
        config.set_path("src/tests/resources/config_test");
        config.set_blacklist_path("src/tests/resources/blacklist_test");
        SECTION("player"){ 
                REQUIRE(config.get_player() == "player");
        }
        SECTION("player args"){ 
                REQUIRE(config.get_player_args() == "command with --arguments=true");
        }
        SECTION("instance"){ 
                REQUIRE(config.get_instance() == "instance_domain");
        }
        SECTION("torrent downloader args"){
                REQUIRE(config.get_torrent_downloader() == "transmission-cli");
        }
        SECTION("torrent downloader args"){
                REQUIRE(config.get_torrent_downloader_args() == " trans-args");
        }
        SECTION("use-raw-url true"){ 
                REQUIRE(config.get_use_raw_url() == true);
        }
        config.set_path("src/tests/resources/config_testbis");
        SECTION("use-raw-url false"){ 
                REQUIRE(config.get_use_raw_url() == false);
        }
        SECTION("max-history"){
                REQUIRE(config.get_max_hist_lines()==100);
        }
        SECTION("Instance blacklist"){
                REQUIRE(config.is_blacklisted("bad.com"));
                REQUIRE(config.is_blacklisted("dontwant.org"));
                REQUIRE(!config.is_blacklisted("want.com"));
                REQUIRE(!config.is_blacklisted("good.com"));
        }
        Config configWhite;
        configWhite.set_blacklist_path("src/tests/resources/whitelist_test");
        SECTION("Instance whitelist"){
                REQUIRE(!configWhite.is_blacklisted("bad.com"));
                REQUIRE(!configWhite.is_blacklisted("dontwant.org"));
                REQUIRE(configWhite.is_blacklisted("want.com"));
                REQUIRE(configWhite.is_blacklisted("good.com"));
        }
}
TEST_CASE("utils")
{
        SECTION("comp_str"){

                REQUIRE(comp_str_ignore_spaces(string("test"),string("test")));
                REQUIRE(comp_str_ignore_spaces(string("test"),string(" test")));
                REQUIRE(comp_str_ignore_spaces(string("test"),string(" test ")));
                REQUIRE(comp_str_ignore_spaces(string("test"),string("test ")));
                REQUIRE(!comp_str_ignore_spaces(string("test"),string("patate")));
                REQUIRE(!comp_str_ignore_spaces(string("test"),string(" patate")));
                REQUIRE(!comp_str_ignore_spaces(string("test"),string("patate ")));
                REQUIRE(!comp_str_ignore_spaces(string("test"),string(" patate ")));
        }
        SECTION("split at first"){
                string test = "test";
                tuple<string,string> split1 = split_at_first(test,string("e"));
                REQUIRE(get<0>(split1) == string("t"));
                REQUIRE(get<1>(split1) == string("st"));
        }
}

TEST_CASE("ui utils")
{
        SECTION("is_int"){ 
                REQUIRE(is_int("321") == true);
                REQUIRE(is_int("ezaz") == false);
                REQUIRE(is_int("32eza") == false);
                REQUIRE(is_int("eza32") == false);
                REQUIRE(is_int("") == false);
        }
        SECTION("print_centered"){ 
                stringstream output_stream;
                print_centered("321", 20,output_stream);
                REQUIRE(output_stream.str()  == "        321\n");
                output_stream.str("");
                output_stream.clear();
                print_centered("321", 2, output_stream);
                REQUIRE(output_stream.str()  == "321\n");
        }
        SECTION("is_url"){
                REQUIRE(is_url("https://test.com/eaz/uuid-e--az/?test=3"));
                REQUIRE(is_url("https://test.social/eaz/uuid-e--az/?test=3"));
 
                REQUIRE(!is_url("test"));
                REQUIRE(!is_url("45"));
                REQUIRE(!is_url("/path/to/file"));
        }

        SECTION("clean_date"){
                REQUIRE(clean_date("2063-04-04T20:20:20.640Z") == "2063-04-04 at 20:20:20");
        }

        SECTION("nice duration"){
                REQUIRE(duration_to_string(3) == "00:03");
                REQUIRE(duration_to_string(13) == "00:13");
                REQUIRE(duration_to_string(61) == "01:01");
                REQUIRE(duration_to_string(71) == "01:11");
                REQUIRE(duration_to_string(671) == "11:11");
                REQUIRE(duration_to_string(3661) == "01:01:01");
        }

        SECTION("get instance and uuid"){
                tuple<string , string> instance_and_url = get_instance_and_uuid_from_url("https://instance/uuid");
                REQUIRE(get<0>(instance_and_url)=="instance");
                REQUIRE(get<1>(instance_and_url)=="uuid");
        }

}

TEST_CASE("history")
{
        History hist;
        hist.set_path("src/tests/resources/history_cmds","src/tests/resources/history_viewing");
        SECTION("views"){
                REQUIRE(hist.is_viewed("view1"));
                REQUIRE(hist.is_viewed("view2"));
                REQUIRE(hist.is_viewed("view3"));
                REQUIRE(!hist.is_viewed("viewadded"));
                hist.add_view("viewadded");
                REQUIRE(hist.is_viewed("viewadded"));
        }

        SECTION("cmds"){
                vector<string> cmds = hist.get_cmd_hist();
                REQUIRE(cmds.size()==3);
                REQUIRE(cmds[0]=="cmd1");
                REQUIRE(cmds[1]=="cmd2");
                REQUIRE(cmds[2]=="cmd3");
                hist.add_cmd("cmdadded");
                cmds = hist.get_cmd_hist();
                REQUIRE(cmds.size()==4);
                REQUIRE(cmds[0]=="cmd1");
                REQUIRE(cmds[1]=="cmd2");
                REQUIRE(cmds[2]=="cmd3");
                REQUIRE(cmds[3]=="cmdadded");
        }
}
