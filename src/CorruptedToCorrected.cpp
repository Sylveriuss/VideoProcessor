#include <iostream>
#include <fstream>
#include <sstream>

#include "VideoProcessor.h"


/**
 * @brief The Configuration struct to read configuration from file.
 */
struct Configuration
{
    bool debug = false;
    std::string video_filename = "";
    std::string output_directory = "";
    int matches_threshold = 190;
    bool reverse_video = false;

    void update(std::string key, std::string value)
    {
        try
        {
            if ( key == "debug" )
            {
                debug = (value == "true");
            }
            else if ( key == "reverse_video" )
            {
                reverse_video = (value == "true");
            }
            else if ( key == "video_filename" )
            {
                video_filename = value;
            }
            else if ( key == "output_directory" )
            {
                output_directory = value;
            }
            else if ( key == "matches_threshold" )
            {
                matches_threshold = std::atoi(value.c_str());
            }
        }
        catch (...)
        {
            std::cerr << "Cannot read configuration of " << key << " " << value << std::endl;
        }
    }


    bool readConfiguration(char* filename)
    {
        std::cout << "Reading configuration file " << filename << std::endl;

        std::ifstream is(filename, std::ios::binary);
        is.seekg (0, std::ios::end);
        long length = is.tellg();
        is.seekg (0, std::ios::beg);
        char *buffer = new char [length];
        is.read (buffer,length);

        std::istringstream is_file= std::istringstream( std::string( buffer ) );
        std::string line;
        while( std::getline(is_file, line) )
        {
          std::istringstream is_line(line);
          std::string key;
          if( std::getline(is_line, key, '=') )
          {
            std::string value;
            if( std::getline(is_line, value) )
               update(key, value);
          }
        }
        return isValid();
    }

    bool isValid()
    {
        return (video_filename != "") && (output_directory != "");
    }
};




int main(int argc, char* argv[] )
{
    if ( argc < 2 )
    {
        std::cerr << "usage: ./CorruptedToCorrected <configuration_filename>" << std::endl;
        return -1;
    }

    Configuration config;
    if (config.readConfiguration(argv[1]))
    {
        VideoProcessor vid_proc;
        return vid_proc.correctVideoTimeline(config.video_filename
                                             , config.output_directory
                                             , config.matches_threshold
                                             , config.debug
                                             , config.reverse_video);
    }
}
