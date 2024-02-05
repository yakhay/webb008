/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhay <yakhay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:40:51 by wbouwach          #+#    #+#             */
/*   Updated: 2024/02/04 19:36:50 by yakhay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "getMethod.hpp"

bool isDirectory(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (dir != nullptr) {
        closedir(dir);
        return true;
    }
    return false;
}


std::string determine_content(const std::string& fileExtension)
{
    return (mimetype.get(fileExtension, 1));
}

void send_response_200(const std::string &filename, const std::string &contentType, int newsockfd, cl& client, one_server& server) {
    if (client.test2 == 0)
    {

        client.fileStream.open(filename, std::ios::binary);
        if (!client.fileStream.is_open())
            throw (404);

        struct stat statbuf;
        if (stat(filename.c_str(), &statbuf) == -1)
            throw (404);

        size_t size = statbuf.st_size;
        std::cout << "path: " << filename << std::endl;

        client.res.set_header("Content-Length", std::to_string(size));
        client.res.set_header("Content-type", contentType);
        client.res.set_Status(200, server);
        std::string response = client.res.prepare_respons();
        int dd = send(newsockfd, response.c_str(), response.size(), 0);

        if (dd < 0) {
            perror("webserver (write) failed");
            client.fileStream.close();
            return;
        }
        client.test2 = 1;
    }
    else
    {

            // if (client.lock == 0)
            // {
            client.fileStream.read(client.buffer.data(), client.buffer_size);
        //     client.lock = 1;
        // }
        // else
        // {
            int valwrite = send(newsockfd, client.buffer.data(), client.fileStream.gcount(), 0);
            if (valwrite < 0) {
            perror("webserver (write) failed");
            client.fileStream.close();
            return;
            }
        //     client.lock = 0;
        // }
        
    }
    if (client.fileStream.eof())
    {
        client.end_send = 1;
        if (filename == "/tmp/listing.html")
            std::remove("/tmp/listing.html");
    }
}

void get_method(request &req, manyServer *server, int newsockfd, int idx, cl& client) {
    std::string new_path;
    std::string contentType;
    Location    location;
    location = server->_name_server[idx].get_location(req.get_path().substr(0, req.get_path().find("?")));
    if (find(location._allow_methods.cbegin(), location._allow_methods.cend(), "GET") == location._allow_methods.cend())
        throw (405);
    std::vector<class Location>::iterator it;
    it = server->_name_server[idx]._location.begin();
    while (it != server->_name_server[idx]._location.end())
    {
        if (req.get_path().find(it->_name_Location) != std::string::npos)
        {
            
            new_path = it->_root + req.get_path().substr(it->_name_Location.size(),req.get_path().size() - it->_name_Location.size());
            break;
        }
        it++;
    }
    size_t find = req.get_path().find_last_of("/");  
    if (find != std::string::npos && (req.get_path().c_str())[find + 1] == '\0')
    {
        std::string str_ret = req.get_path().substr(0,req.get_path().size() - 1);
        std::string response = "HTTP/1.1 301 Moved Permanently\r\n";
        response += "Location: "+  str_ret +"\r\n\r\n";
        int dd = send(newsockfd, response.c_str(), response.size(), 0);
        close(newsockfd);
        client.end_send = 1;
        return ;
    }
    if (access(new_path.c_str(), R_OK ) != 0)
    {
        if (access(new_path.c_str(), R_OK | X_OK) != 0)
            throw (404);
    }
    if (it->_return)
    {
        std::string str_ret(it->_return , strlen(location._return));
        std::string response = "HTTP/1.1 301 Moved Permanently\r\n";
        response += "Location: "+  str_ret +"\r\n\r\n";
        int dd = send(newsockfd, response.c_str(), response.size(), 0);
        close(newsockfd);
        client.end_send = 1;
        return ;
    }
    else if (!isDirectory(new_path)) {
        if(location.cgi_status == 0)
        {
            if (new_path.find_last_of(".") == std::string::npos)
                contentType = "application/octet-stream";
            else
            contentType = determine_content(new_path.substr(new_path.find_last_of(".") + 1));
            send_response_200(new_path, contentType, newsockfd, client,server->_name_server[idx]);
        }
        else
        {
            if ((new_path.find_last_of('.') != std::string::npos) && !(location.get_cgi(new_path.substr(new_path.find_last_of('.'))).empty()))
            {
                std::cout<<"RUN CGI FROM GET2"<<std::endl;
                std::string msg;
            
                client.req.set_cgi_script(new_path);
                msg = client.cgi.cgi_run(server->_name_server[idx], client, "");
                write(newsockfd, msg.c_str(), msg.length());
            }
            else
            {
                if (new_path.find_last_of(".") == std::string::npos)
                contentType = "application/octet-stream";
                else
                contentType = determine_content(new_path.substr(new_path.find_last_of(".") + 1));
                send_response_200(new_path, contentType, newsockfd, client, server->_name_server[idx]);
            }
        }
         }
        else if (location.index) {
            std::string index = (location.index);
            new_path = new_path + "/" + index;
                if (location.cgi_status == 1)
                {
                    if ((new_path.find_last_of('.') != std::string::npos) && !(location.get_cgi(new_path.substr(new_path.find_last_of('.'))).empty()))
                {
                    std::string msg;
                
                    client.req.set_cgi_script(new_path);
                    msg = client.cgi.cgi_run(server->_name_server[idx], client, "");
                    write(newsockfd, msg.c_str(), msg.length());
                }
                else
                {
                    if (new_path.find_last_of(".") == std::string::npos)
                        contentType = "application/octet-stream";
                    else
                    contentType = determine_content(new_path.substr(new_path.find_last_of(".") + 1));
                    send_response_200(new_path, contentType, newsockfd, client, server->_name_server[idx]);
                }
                return ;
            } else {
                if (new_path.find_last_of(".") == std::string::npos)
                    contentType = "application/octet-stream";
                else
                contentType = determine_content(index.substr(index.find_last_of(".") + 1));
                send_response_200(new_path +"/" + index, contentType, newsockfd, client, server->_name_server[idx]);
        }
    }
    else if (location._autoindex == 1) {
        if (client.test3 == 0) {
            
            std::fstream outputFile("/tmp/listing.html", std::ios::in | std::ios::out | std::ios::app);
            outputFile.seekp(0, std::ios::beg);
            outputFile << "";
            if (!outputFile.is_open()) {
                std::cerr << "Error opening the file: " << std::endl;
                return;
            }
            client.test3 = 1;

            std::string buffer = "<html><head>\n<title>Directory Listing</title></head><body>\n<h1>Directory Listing</h1><ul>\n";
            DIR *dir = opendir(new_path.c_str());

            if (dir == nullptr) {
                std::cerr << "Error opening directory." << std::endl;
                return;
            }
            struct dirent *entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_REG) {
                    std::string filename = entry->d_name;
                    if (buffer.find(entry->d_name) == std::string::npos)
                        buffer = buffer + "<li> <a href=\"" + req.get_path() + "/" + filename + "\">" + filename + "</a></li> \n";
                    else
                        break;
                }
            }
            outputFile << buffer + "</ul></body></html>\r\n";
            outputFile.close();
            closedir(dir);
            contentType = "text/html"; 
        }
        send_response_200("/tmp/listing.html", contentType, newsockfd, client, server->_name_server[idx]);
    } 
    else {
         throw (403);
        client.end_send = 1;
    }
}
