// Copyright 2016 Chirstopher Torres
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

namespace {
void ParseCommandLine(int argc, char** argv)
{
  cxxopts::Options options(argv[0], "ROSE Data converter");
  
  try {
    std::string config_file_path = "";
    options.add_options()
    ("f,file",  "Output file path", cxxopts::value<std::string>(config_file_path)
      ->default_value("server.ini"), "FILE_PATH")
    ("l,level", "Logging level (0-9)", cxxopts::value<int>()
      ->default_value("3"), "LEVEL")
    ("h,help",  "Print this help text")
    ;
  
    options.parse(argc, argv);
    
    // Check to see if the user wants to see the help text
    if (options.count("help"))
    {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }
    
    // SETUP THE OPTIONS HERE!!!
    if( options.count("level") )
	  spdlog::set_level( (spdlog::level::level_enum)options["level"].as<int>() );
  }
  catch (const cxxopts::OptionException& ex) {
    std::cout << ex.what() << std::endl;
    std::cout << options.help({"", "Group"}) << std::endl;
    exit(1);
  }
}
} // end namespace
//*/

#include "stb.h"
#include "safedelete.hpp"
#include "flatfilesystem.hpp"

int main(int argc, char* argv[]) {
//  try {
//    size_t q_size = 4096; //queue size must be power of 2
//    spdlog::set_async_mode(q_size);
//    auto console = spdlog::stdout_color_mt("console");
//    ParseCommandLine(argc, argv);
    
    // LOAD STUFF HERE!!!
    std::string path = "./";
    FlatFileSystem* fs = new FlatFileSystem(path.c_str());
    FileSystem::SetFileSystem(fs);
    
    ROSE::STB* item = new ROSE::STB("list_weapon.stb");
    
    LOG("Loaded %i Rows with %i Columns.\n",
    item->Rows(),
    item->Columns());
    
    SAFE_DELETE(item);
    SAFE_DELETE(fs);

//    spdlog::drop_all();
//  }
//  catch (const spdlog::spdlog_ex& ex) {
//     std::cout << "Log failed: " << ex.what() << std::endl;
//  }
  return 0;
}
