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

#include "backend/Instance_interface.hpp"
#include <iostream>

#include "backend/network/curl_request.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/error.h>
#include <rapidjson/error/en.h>

#include "utils/utils.hpp"

Instance_interface::Instance_interface()
{
}

void Instance_interface::set_instance(string instance_url)
{
        m_instance_url = instance_url;
}

vector<Video> Instance_interface::search_video(string query)
{
        string api_url = "https://" + m_instance_url + "/api/v1/";
        string search_url = api_url + "search/videos/?search=" + query;
        string json_api_result= curl_request(search_url);
        replace_all(json_api_result,string("\n"),string(""));
        rapidjson::Document json_value;
        rapidjson::ParseResult error = json_value.Parse(json_api_result.c_str());

        if(!error)
                throw string("search result parsing error");

        if(!json_value.HasMember("data"))
                throw string("search result parsing error (no data)");

        vector<Video> to_return;
        if(!json_value["data"].IsArray())
                throw string("search result parsing error (bad data)");

        for(auto itr = json_value["data"].Begin(); itr != json_value["data"].End();itr++){
                if(!itr->IsObject())
                        throw "search result parsing error (bad video)";
                if(!itr->HasMember("name"))
                        throw "search result parsing error (no title)";
                if(!(*itr)["name"].IsString())
                        throw "search result parsing error (bad title)";
                if(!itr->HasMember("uuid"))
                        throw "search result parsing error (no uuid)";
                if(!(*itr)["uuid"].IsString())
                        throw "search result parsing error (bad uuid)";
                if(!itr->HasMember("duration"))
                        throw "search result parsing error (no duration)";
                if(!(*itr)["duration"].IsInt())
                        throw "search result parsing error (bad duration)";
                if(!itr->HasMember("account"))
                        throw "search result parsing error (no account)";
                if(!(*itr)["account"].IsObject())
                        throw "search result parsing error (bad account)";
                if(!(*itr)["account"].HasMember("displayName"))
                        throw "search result parsing error (no account name)";
                if(!(*itr)["account"]["displayName"].IsString())
                        throw "search result parsing error (bad account name)";

                string channel_name;
                string host;
                if(itr->HasMember("channel") && (*itr)["channel"].IsObject() && (*itr)["channel"].HasMember("host") && (*itr)["channel"]["host"].IsString()){
                        host = (*itr)["channel"]["host"].GetString();
                        channel_name =(*itr)["channel"]["displayName"].GetString();
                }else
                        throw "search result parsing error (bad host)";

                string account_name = (*itr)["account"]["displayName"].GetString();

                string title = (*itr)["name"].GetString();
                string uuid= (*itr)["uuid"].GetString();
                int duration = (*itr)["duration"].GetInt();
                to_return.push_back(Video(uuid,host,title,channel_name,account_name,duration));
        }

        return to_return;
}
