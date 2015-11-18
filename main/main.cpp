// -*- compile-command: "cd ../build && make"; -*-
/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <tokenizer.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  boost::filesystem::path exe = argv[0];
  std::string exe_name = exe.stem().string();

  std::string input_file;

  po::options_description desc("Usage " + exe_name + " [FILE]: \nAllowed options");
  desc.add_options()
      ("help,h", "shows this help")
      ("input-file,f", po::value<std::string>(&input_file), "input file")
      ;

  po::positional_options_description p;
  p.add("input-file", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(desc)
                .positional(p)
                .run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 0;
  }

  if (vm.count("input-file")) {
    std::unique_ptr<Scanner> scanner{new LineScanner(input_file)};
    Tokenizer tokenizer(scanner);
    for (auto it = tokenizer.begin(); it != tokenizer.end(); ++it) {
      std::cout << (*it).DebugInfo() << ", ";
    }
  }

  return 0;
}
