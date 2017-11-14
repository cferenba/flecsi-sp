/*~-------------------------------------------------------------------------~~*
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~-------------------------------------------------------------------------~~*/
///////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Utilities to process command line arguments.
///////////////////////////////////////////////////////////////////////////////
#pragma once

// user includes
#include "flecsi-sp/common/parse_arguments.h"

// system includes
#include <getopt.h>

namespace flecsi{
namespace sp {
namespace burton {

///////////////////////////////////////////////////////////////////////////////
//! \brief Process the argument list for this app.
///////////////////////////////////////////////////////////////////////////////
inline auto process_arguments(int argc, char** argv) 
{

  // the usage stagement
  auto print_usage = [&argv]() {
    std::cout << "Usage: " << argv[0] 
              << " [--input INPUT_FILE]"
              << " [--mesh MESH_FILE]"
              << " [--catalyst PYTHON_SCRIPT]"
              << " [--help]"
              << std::endl << std::endl;
    std::cout << "\t--input INPUT_FILE:\t Override the input file "
              << "with INPUT_FILE." << std::endl;
    std::cout << "\t--mesh MESH_FILE:\t Override the mesh file "
              << "with MESH_FILE." << std::endl;
    std::cout << "\t--catalyst PYTHON_SCRIPT:\t Load catalyst with "
              << "using PYTHON_SCRIPT." << std::endl;
    std::cout << "\t--help:\t Print a help message." << std::endl;
  };

  // Define the options
  struct option long_options[] =
    {
      {"help",            no_argument, 0, 'h'},
      {"input",     required_argument, 0, 'i'},
      {"mesh",      required_argument, 0, 'm'},
      {"catalyst",  required_argument, 0, 'c'},
      {0, 0, 0, 0}
    };
  const char * short_options = "hi:m:c:";

  // parse the arguments
  auto args =
    flecsi::sp::common::parse_arguments(argc, argv, long_options, short_options);
  
  // process the simple ones
  if ( args.count("h") ) print_usage();

  return args;
}

} // namespace
} // namespace
} // namespace
 
