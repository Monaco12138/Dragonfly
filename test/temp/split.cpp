#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <vector>
#include <string>

int main() {

    std::vector< std::string >  requestsVecTemp;
    std::string data = "one\r\n\r\ntwo\r\n\r\nthree\r\n\r\nfour";
    std::cout << data << std::endl;


    boost::algorithm::split_regex( requestsVecTemp, data, boost::regex("\r\n\r\n") );

    std::vector< std::string> requestCopy;
    requestCopy = requestsVecTemp;
    for (auto str : requestCopy ){
        std::cout << str << std::endl;
    }
    
    return 0;
}