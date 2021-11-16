#include <iostream>

#include <libKitsunemimiSakuraHardware/host.h>
#include <libKitsunemimiJson/json_item.h>

int main()
{
    Kitsunemimi::Sakura::Host host;
    host.initHost();

    Kitsunemimi::Json::JsonItem json;
    std::string errorMessage = "";
    const bool success = json.parse(host.toJsonString(), errorMessage);
    if(success == false) {
        std::cout<<"error: "<<errorMessage<<std::endl;
        return 1;
    }

    std::cout<<json.toString(true)<<std::endl;

    return 0;
}
