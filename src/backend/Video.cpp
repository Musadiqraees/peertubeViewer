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

#include "backend/Video.hpp"
#include "backend/network/curl_request.hpp"
#include "utils/utils.hpp"

#include <iostream>


#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/error.h>
#include <rapidjson/error/en.h>


Video::Video(string uuid, string host,string title,string channel, string author,int duration):m_uuid(uuid), m_title(title), m_host(host), m_channel_name(channel), m_author_name(author), m_duration(duration),m_has_info(false)
{
}

string Video::get_title()
{
        return m_title;
}

string Video::get_host()
{
        return m_host;
}

string Video::get_raw_url()
{
        update();
        return m_video_files[0].get_file_url();
}

string Video::get_torrent_url()
{
        update();
        return m_video_files[0].get_torrent_url();
}

string Video::get_channel_name()
{
        return m_channel_name;
}

string Video::get_author_name()
{
        return m_author_name;
}

string Video::get_date()
{
        return m_date;
}

string Video::get_watch_url()
{
        string to_return = "https://";
        to_return += m_host;
        to_return += "/videos/watch/";
        to_return += m_uuid;
        return to_return;
}

string Video::get_full_description()
{
        return m_full_description;
}

string Video::get_short_description()
{
        return m_short_description;
}

int Video::get_views()
{
        return m_views;
}

int Video::get_duration()
{
        return m_duration;
} 
int Video::get_likes()
{
        return m_likes;
}

int Video::get_dislikes()
{
        return m_dislikes;
}

vector<Video_file> Video::get_video_files(){
        update();
        return m_video_files;
}

void Video::update()
{
        if(m_has_info)
                return;
        m_has_info = true;

        string json_api_result = curl_request("https://" + m_host + "/api/v1/videos/" + m_uuid);
        replace_all(json_api_result,string("\n"),string(""));
        rapidjson::Document json_value;
        rapidjson::ParseResult error = json_value.Parse(json_api_result.c_str());

        if(!error)
                throw string("error while parsing video information");

        if(json_value.HasMember("description") && json_value["description"].IsString())
                m_short_description = json_value["description"].GetString();
        else
                m_short_description = "unkonwn";
        

        if(json_value.HasMember("files") &&\
                json_value["files"].IsArray() &&\
                json_value["files"].Size() >= 1){
                for(unsigned int i=0;i<json_value["files"].Size();i++){
                        if(json_value["files"][i].IsObject() &&\
                        json_value["files"][i].HasMember("fileUrl") &&\
                        json_value["files"][i]["fileUrl"].IsString() &&\
                        json_value["files"][i].HasMember("torrentUrl") &&\
                        json_value["files"][i]["torrentUrl"].IsString() &&\
                        json_value["files"][i].HasMember("resolution") &&\
                        json_value["files"][i]["resolution"].HasMember("label") &&\
                        json_value["files"][i]["resolution"]["label"].IsString()) {
                                m_video_files.push_back(Video_file(json_value["files"][i]["fileUrl"].GetString(),
                                json_value["files"][i]["torrentUrl"].GetString(),
                                json_value["files"][i]["resolution"]["label"].GetString()));
                        }
                }
        }
        if(m_video_files.size() == 0) 
                throw string("error while parsing video information");
                

        if(json_value.HasMember("name") && json_value["name"].IsString())
                m_title= json_value["name"].GetString();
        else
                m_title = "unknown";

        if(json_value.HasMember("views") && json_value["views"].IsInt())
                m_views = json_value["views"].GetInt();
        else
                m_views = -1;

        if(json_value.HasMember("likes") && json_value["likes"].IsInt())
                m_likes = json_value["likes"].GetInt();
        else
                m_likes = -1;

        if(json_value.HasMember("dislikes") && json_value["dislikes"].IsInt())
                m_dislikes = json_value["dislikes"].GetInt();
        else
                m_dislikes = -1;

        if(json_value.HasMember("duration") && json_value["duration"].IsInt())
                m_duration = json_value["duration"].GetInt();
        else
                m_duration = -1;


        if(json_value.HasMember("channel") &&\
                json_value["channel"].IsObject() &&\
                json_value["channel"].HasMember("displayName") &&\
                json_value["channel"]["displayName"].IsString())

                m_channel_name = json_value["channel"]["displayName"].GetString();
        else
                m_channel_name = "unknown";

        if(json_value.HasMember("account") &&\
                json_value["account"].IsObject() &&\
                json_value["account"].HasMember("displayName") &&\
                json_value["account"]["displayName"].IsString())

                m_author_name = json_value["account"]["displayName"].GetString();
        else
                m_author_name = "unknown";

        if(json_value.HasMember("publishedAt") && json_value["publishedAt"].IsString())
                m_date = json_value["publishedAt"].GetString();
        else
                m_date = "unknown";

        string json_description_api_result = curl_request("https://" + m_host + "/api/v1/videos/" + m_uuid + "/description");

        replace_all(json_description_api_result,string("\n"),string(""));
        
        rapidjson::Document json_description;
        rapidjson::ParseResult error_description = json_description.Parse(json_description_api_result.c_str());

        if(!error_description){
                m_full_description = m_short_description;
                return;
        }

        if(json_description.HasMember("description") && json_description["description"].IsString())
                m_full_description= json_description["description"].GetString();
        else{
                m_full_description= m_short_description;
        }
}

