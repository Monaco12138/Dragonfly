#include <iostream>
#include <string>
#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_string(name, "chaibli", "your name");
int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    google::InitGoogleLogging( argv[0] );
    google::SetLogDestination(google::INFO, "log.txt");
    for ( int i=0; i < argc; i++ ) {
        std::cout << argv[i] << std::endl;
    }
    LOG(INFO) << "test log info";
    LOG(WARNING) << "test log warning";
    //LOG(ERROR) << "test log error";
    //LOG(FATAL) << "test log fatal";
    std::cout << "your name is: " << FLAGS_name << std::endl;

    google::ShutdownGoogleLogging();
    return 0;
}