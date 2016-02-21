#pragma once

#include "yaml-cpp/yaml.h"

#ifdef WIN32
    #ifdef _DEBUG
        #define YAML_PATH "C:\\yaml-cpp\\lib\\Debug\\"
        #define YAML_EXT_STR "d.lib"
    #else
        #define YAML_PATH "C:\\yaml-cpp\\lib\\Release\\"
        #define YAML_EXT_STR ".lib"
    #endif

    #pragma comment(lib, YAML_PATH "libyaml-cppmt" YAML_EXT_STR)
#endif


class CRConfig {
public:
    CRConfig() {
        this->width = 640;
        this->height = 480;
        this->frameRate = 500.0;
        this->bufferSize = 500;

        this->type = "keyboard";
        this->host = "";
        this->port = 0;

        this->method = "diff";

        this->debug = true;
    }

    CRConfig(std::string file) {
        this->config = YAML::LoadFile(file);

        this->width = this->config["camera"]["width"].as<int>();
        this->height = this->config["camera"]["height"].as<int>();
        this->frameRate = this->config["camera"]["frame_rate"].as<float>();
        this->bufferSize = this->config["camera"]["buffer_size"].as<int>();

        this->type = this->config["socket"]["type"].as<std::string>();
        this->host = this->config["socket"]["host"].as<std::string>();
        this->port = this->config["socket"]["port"].as<int>();

        this->method = this->config["tracking"]["method"].as<std::string>();

        this->debug = this->config["debug"].as<bool>();
    }

    virtual ~CRConfig() {}

    float frameRate;
    int width;
    int height;
    int bufferSize;

    std::string type;
    std::string host;
    int port;

    std::string method;

    bool debug;

private:
    YAML::Node config;

};

