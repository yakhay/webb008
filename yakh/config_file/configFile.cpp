#include "configFile.hpp"

//------------------------------global function using ----------------------------//
int check_end(std::string str)
{
    int position;
    position = str.find("{") + 1;
    while (str[position] && (str[position] == ' ' || str[position] == '\t' || str[position] == '\n'))
        position++;
    if (str[position] != '}' )
    {
        return -1;
    }
    position++;
     while (str[position] && (str[position] == ' ' || str[position] == '\t' || str[position] == '\n'))
            position++;
    if (str[position ] == '\0' )
        return 0;
    return -1;

}
std::string cut_and_paste(std::string &string_to_pars,int position ,int position_end)
{
    std::string s1;
    std::string s2;
    s1 = string_to_pars.substr(0,position);
    s2 = string_to_pars.substr(position_end ,strlen(string_to_pars.c_str()) - position_end);
    return (s1 + s2);

}
//------------------------------end global function using ----------------------------//


//------------------------------config_file to many server ----------------------------//
manyServer::manyServer()
{
    _count_server = 0;
}
int ft_check_buffer(std::string str)
{
    int position = 0;
    while (str[position] && (str[position] == ' ' || str[position] == '\t' || str[position] == '\n'))
        position++;
    if (str[position] != '\0')
        return -1;
    return 0;
}
manyServer::manyServer(std::string file)
{
    int i;
    size_t position = 0 ,position_end = 0;;
    std::string line, buffer;
    std::ifstream inputFile;

    position = file.find_last_of(".");
    if (position == std::string::npos)
    {
        std::cout<<"file name has error"<<std::endl;
        exit (0);
    }
    else
    {

        if (file.substr(position) != ".conf")
        {
            std::cout<<"file name has error"<<std::endl;
            exit (0);
        }
    }
    inputFile.open(file);
    if (!inputFile.is_open())
    {
        std::cout<<"failed to open file"<<std::endl;
        exit (0);
    }
    while (std::getline(inputFile, line)) {
        buffer = buffer + line + "\n";
    }
    position = buffer.find("server {");
    if (position == std::string::npos)
    {
        std::cout<<"file name has error "<<std::endl;
        exit (0);
    }
    i = 0;
    while (position != std::string::npos && position_end != std::string::npos)
    {
        position = buffer.find("server {",position_end);
        if (position == std::string::npos)
            break ;
         position_end = buffer.find("server {",position + 1);
        if (position_end == std::string::npos)
            _name_server.push_back(one_server(buffer.substr(position, buffer.size() - position)));
        else
            _name_server.push_back(one_server(buffer.substr(position,position_end - position)));
        i++;
    }
    if (i == 0)
    {
         std::cout<<"file not has server"<<std::endl;
             exit (0);
    }
    inputFile.close();
}

//------------------------------ end config_file to many server ----------------------------//





//------------------------------pars string to prameter server ----------------------------//
std::string initialisation_param(std::string &string_to_pars,std::string parame , int c)
{
    size_t position = 0 ,position_end = 0;
    std::string line;
    position = string_to_pars.find(parame);
    if (c == 1 && position == std::string::npos)
    {
       std::cout<< "error in "<<parame<< std::endl;
        exit(0);
    }
    if (c == 0 && position == std::string::npos)
        return (NULL);
    position_end = string_to_pars.find(";",position);
    if ( position_end == std::string::npos)
    {
        std::cout<<parame<< std::endl;
        exit(0);
    }
    
    line = string_to_pars.substr(position, position_end + 1 - position);
    string_to_pars = cut_and_paste(string_to_pars,position ,position_end + 1);
    int i = parame.size();
    
    while (line[i] && (line[i] == '\t' || line[i] == ' '))
        i++;
    if (line[i] != ':')
    {
        std::cout<< "error in "<<parame<< std::endl;
        exit(0);
    }
    
    i++;
    while (line[i] && (line[i] == '\t' || line[i] == ' '))
        i++;
     if (line[i] == ';')
    {
        std::cout<< "error in "<<parame<< std::endl;
        exit(0);
    }
    position_end = line.find(";",i);
    position_end--;
    while (position_end > 0 && (line[position_end] == '\t' || line[position_end] == ' '))
        position_end--;
    
    return (line.substr(i,position_end + 1 - i));
}

one_server::one_server(std::string string_to_pars)
{
    size_t position = 0 ,position_end = 0;
    std::string location,new_string;
    std::string s1, s2;
    position = string_to_pars.find("location");
    if (position == std::string::npos)
    {
        std::cout<<"error in creation server";
        exit(0);
    }
    while (position != std::string::npos && position_end != std::string::npos)
    {
        position = string_to_pars.find("location");
        if ( position == std::string::npos)
            break ;
        position_end = string_to_pars.find("}",position);
        if ( position_end == std::string::npos)
        {
            std::cout<<"error location"<< std::endl;
            exit(0);
        }
       
       _location.push_back(Location(string_to_pars.substr(position,position_end + 1 - position)));
        string_to_pars = cut_and_paste(string_to_pars,position ,position_end + 1);
    }
    _server_name = initialisation_param(string_to_pars,"server_name", 1);
    listen = std::stoi(initialisation_param(string_to_pars,"listen", 1));
    client_body_size =  initialisation_param(string_to_pars,"client_max_body_size", 1);             
    _Host = initialisation_param(string_to_pars,"host", 1);
    int i =0, j;
    position = 0 ;
    position_end = 0;
    while (position != std::string::npos && position_end != std::string::npos)
    {
        position = string_to_pars.find("error_page");
        if (position == std::string::npos)
            break ;
        position_end = string_to_pars.find(";",position);
        if ( position_end == std::string::npos)
        {
            std::cout<<"--error location"<< std::endl;
            exit(0);
        }
        new_string = string_to_pars.substr(position + 10,position_end  - (position + 10));
        string_to_pars  = cut_and_paste(string_to_pars,position ,position_end + 1);
        j = 0;
        while (new_string[j] && (new_string[j] == '\t' || new_string[j] == ' '))
            j++;
        s1 = new_string.substr(j,3);
        j = j + 3;
        while (new_string[j] && (new_string[j] == '\t' || new_string[j] == ' '))
            j++;
        if (new_string[j] != ':')
        {

            std::cout<< new_string <<"error location"<< std::endl;
            exit(0);
        }
        j++;
        while (new_string[j] && (new_string[j] == '\t' || new_string[j] == ' '))
            j++;
        position_end--; 
        while (position_end > 0 && (new_string[position_end] == '\t' || new_string[position_end] == ' '))
            position_end--;
        s2 = new_string.substr(j,position_end - j - 1);
        error_page[s1] = s2;
        i++;
    }
    if (check_end(string_to_pars) == -1)
    {
        std::cout <<"error in the parametre server"<<  std::endl;
        exit(0);
    }
    sort(this->_location.begin(), this->_location.end(), cmpr);
}
//------------------------------end pars string to prameter server ----------------------------//






//------------------------------pars string to prameter location --------------------------------//
Location::Location(std::string location)
{
    int begn = 9, end;
    size_t position = 0 ,position_end = 0;
    std::string line, s1,s2;
    int i =0;
    size_t j , k;
    while (location[begn] && location[begn] != '/')
        begn++;
    
    end = begn;
    while (location[end] && (location[end] != ' ' && location[end] != '\t' && location[end] != '{'))
        end++;
    _name_Location = location.substr(begn, end - begn);
    location = cut_and_paste(location, begn, end);
    _root = initialisation_param(location,"root", 1);
    if (access(_root.c_str(), F_OK) != 0)
    {
        std::cerr<<"invalid Root path!"<<std::endl;
        exit(1);
    }
    if (location.find(" index ") != std::string::npos)
        index = strdup(initialisation_param(location," index ",0).c_str());
    else
        index = NULL;
    if (location.find("upload_path") != std::string::npos)
    {
        upload_path = strdup(initialisation_param(location,"upload_path",1).c_str());
        if (access(upload_path, F_OK) != 0)
        {
            std::cerr<<"invalid Upload path!"<<std::endl;
            exit(1);
        }
    }
    else
        upload_path = NULL;
    if (location.find("return") != std::string::npos)
        _return = strdup(initialisation_param(location,"return",0).c_str());
    else
        _return  = NULL;
    _autoindex = 0;
    _upload= 0;
    cgi_status = 0;
    if (initialisation_param(location,"cgi_status", 1) == "on")
        cgi_status = 1;
    if (initialisation_param(location,"autoindex", 1) == "on")
        _autoindex = 1;
    if (initialisation_param(location,"upload", 1) == "on")
        _upload= 1;
    position = location.find("allow_methods");
    position_end = location.find(";" ,position);
    line = location.substr(position, position_end + 1 - position);
    location =  cut_and_paste(location, position, position_end + 1);
    if (line.find("GET") != std::string::npos)
        _allow_methods.push_back("GET");
    if (line.find("POST") != std::string::npos)
        _allow_methods.push_back("POST");
    if (line.find("DELETE") != std::string::npos)
        _allow_methods.push_back("DELETE");
    position = 0 ;
    position_end = 0;
    while (position != std::string::npos && position_end != std::string::npos)
    {
        position = location.find("cgi_path");
        if (position == std::string::npos)
            break;
        position_end = location.find(";",position);
        if (position_end == std::string::npos)
        {
            std::cout << "error cgi_path" << std::endl;
           break; 
        }
        line = location.substr(position, position_end + 1 - position);
        location = cut_and_paste(location, position, position_end + 1);
        j = 8;
        while (line[j] && (line[j] == ' ' || line[j] == '\t'))
            j++;
        k = j;
       
        while (line[k] && (line[k] != ' ' && line[k] != '\t' && line[k] != ':'))
            k++;
        if (k != j)
            s1 = line.substr(j ,k-j);
        else
        {
            std::cout << "error cgi_path" << std::endl;
            exit(0);
        }
         
        j = line.find(":") + 1;
        if(j == std::string::npos )
        {
            std::cout << "error cgi_path" << std::endl;
            exit(0);
        }
         while (line[j] && (line[j] == ' ' || line[j] == '\t'))
            j++;
        k = j;
        while (line[k] && (line[k] != ' ' && line[k] != '\t' && line[k] != ';'))
            k++;
        if (k != j)
            s2 = line.substr(j ,k-j);
        cgi[s1] = s2;
        i++;
    }
    if (check_end(location) == -1)
    {
        std::cout<<"error in the parametre location"<< std::endl;
        exit(0);
    }
}

bool cmpr(const Location& loc1, const Location& loc2)
{
    return loc1._name_Location.size() < loc2._name_Location.size();
}

//------------------------------end pars string to prameter location ----------------------------//



// int main()
// {
//     manyServer yas("file.conf");
//     int i = 0;
//     // std::cout<< yas._name_server[0]._server_name<<std::endl;
//     // std::cout<< yas._name_server[0]._Host<<std::endl;
//     // std::cout<< yas._name_server[0].listen<<std::endl;
//     // std::cout<< yas._name_server[0].client_body_size<<std::endl;
//     while (i < 4)
//     {
//         std::cout<< yas._name_server[0]._location[i]._autoindex<<std::endl;
//         std::cout<< yas._name_server[0]._location[i]._name_Location<<std::endl;
//         std::cout<< yas._name_server[0]._location[i]._root<<std::endl;
//         std::cout<< yas._name_server[0]._location[i]._upload<<std::endl;
//         printf("%s\n",yas._name_server[0]._location[i].upload_path);
//         printf("%s\n",yas._name_server[0]._location[i].index);
//         printf("%s\n",yas._name_server[0]._location[i]._return);
//         std::cout << "--"<<yas._name_server[0]._location[i].cgi.size() << std::endl;
//         // std::cout<< yas._name_server[0]._location[i].index<<std::endl;
//         // std::cout<< yas._name_server[0]._location[i]._return<<std::endl;
//         i++;
//     }
    
    
//}



Location::~Location(){}
Location::Location(){}

std::string Location::get_upload_path() const
{
    return (this->_root + "/" + this->upload_path + "/");
}

std::string Location::get_cgi(std::string extension) const
{
    std::map<std::string, std::string>::const_iterator  it = cgi.cbegin();
    std::map<std::string, std::string>::const_iterator  eit = cgi.cend();
    while (it != eit)
    {
        if (it->first == extension)
            return (it->second);
        it++;
    }
    return (""); 
}

bool Location::is_index(const std::string& name) const
{
    return (name == this->index);
}
//---------------------------one_server
Location    one_server::get_location(std::string url) const
{
    std::vector<Location>::const_reverse_iterator   it = this->_location.crbegin();
    std::vector<Location>::const_reverse_iterator   eit = this->_location.crend();
    size_t                                          pos;
    while (it != eit)
    {
        std::cout<<"one_server::"<<it->_name_Location<<"$\n";
        pos = url.find(it->_name_Location);
        if (pos != std::string::npos)
            return (*it);
        it++;
    }
    std::cout<<"get_location"<<std::endl;
    throw (404);
}

std::string    one_server::get_path(const std::string& url) const
{
    std::vector<Location>::const_reverse_iterator   it = this->_location.crbegin();
    std::vector<Location>::const_reverse_iterator   eit = this->_location.crend();
    size_t                                          pos;

    while (it != eit)
    {
        pos = url.find(it->_name_Location);
        if (pos != std::string::npos)
            break;
        it++;
    }
    std::cout<<"get_path"<<std::endl;
    if (it == eit)
        throw (404);
    return (it->_root + "/" + url.substr(pos + it->_name_Location.length()));
}

std::string one_server::get_error_page(std::string status) const
{
    std::map<std::string, std::string>::const_iterator  it = this->error_page.cbegin();
    std::map<std::string, std::string>::const_iterator  eit = this->error_page.cend();

    while (it != eit)
    {
        if (it->first == status)
            return (it->second);
        it++;
    }
    return ("");
}
