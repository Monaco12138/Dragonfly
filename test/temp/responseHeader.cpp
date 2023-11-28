#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <fstream>
#include <sstream>
std::string getResponseHeader(
    std::string httpVersion, std::string statusCode, std::string acceptRange,
    int contentLength, std::string contentType, std::string tileIdx,
    std::string quality) {
  std::stringstream header;

  time_t now = time(0);
  tm *gmtm = gmtime(&now);
  std::string dt(asctime(gmtm));

  header << boost::format("HTTP%s %s\r\n") % httpVersion % statusCode;
  header << boost::format("Tile-Index: %s\r\n") % tileIdx;
  header << boost::format("Tile-Quality:%s\r\n") % quality;
  header << boost::format("Date: %s\r\n") % dt.erase(dt.size() - 1);
  header << boost::format("Accept-Ranges: %s\r\n") % acceptRange;
  header << boost::format("Content-Length: %d\r\n") % contentLength;
  header << boost::format("Content-Type: %s\r\n") % contentType;
  header << "\r\n";
  return header.str();
}


int main() {
    
    int fileSize = 1440; //byte

    std::string header( getResponseHeader(
        "1.1", "200 OK", "Bytes", fileSize, "video/m4s",
        "1_21", "0"));

    std::cout << header << std::endl;
    
    return 0;
}