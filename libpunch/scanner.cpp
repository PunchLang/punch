/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <lang/scanner.hpp>
#include <fstream>

using namespace punch::lang;

boost::optional<char> StringScanner::current_char() {
  if (this->index < this->size) {
    return this->chars.at(this->index);
  }
  else {
    return boost::none;
  }
}

boost::optional<char> StringScanner::next_char() {
  if ((this->index + 1) < this->size) {
    return this->chars.at(this->index + 1);
  }
  else {
    return boost::none;
  }
}

boost::optional<char> StringScanner::previous_char() {
  if ((this->index) > 0) {
    return this->chars.at(this->index - 1);
  }
  else {
    return boost::none;
  }
}

void StringScanner::pop() {
  if (this->index < this->size) {

    if (this->current_char() == '\n') {
      line+=1;
      col = 1;
    }
    else {
      col+=1;
    }

    this->index += 1;
  }
}

void StringScanner::flush_line() {
  while (current_char()) {
    if (*current_char() != '\n') {
      pop();
    }
    else {
      pop();
      break;
    }
  }
}

position StringScanner::get_position() const {
  return std::make_tuple(line, col);
}


LineScanner::LineScanner(const std::string& file) :
  current (std::make_tuple(0, 0)),
  next (std::make_tuple(0,1)),
  previous (std::make_tuple(0,-1))
{
  std::ifstream infile(file);

  for(std::string line; std::getline(infile, line);) {
    std::vector<char> chars(line.begin(), line.end());
    this->lines.push_back(std::make_tuple(line.size(), chars));
  }

  infile.close();
}

boost::optional<char> LineScanner::read_char(::position pos) {

  int l,c;
  std::tie(l,c) = pos;

  if (l < this->lines.size()) {
    if (c < 0) {
      return boost::none;
    }
    else if (c < std::get<0>(this->lines.at(l))) {
      return std::get<1>(this->lines.at(l)).at(c);
    }
    else { // insert \n as last char of line;
      return '\n';
    }
  }
  else {
    return boost::none;
  }
}

boost::optional<char> LineScanner::current_char() {
  return this->read_char(this->current);
}

boost::optional<char> LineScanner::next_char() {
  return this->read_char(this->next);
}

boost::optional<char> LineScanner::previous_char() {
  return this->read_char(this->previous);
}

void LineScanner::pop() {
  int cl, cc;
  std::tie(cl, cc) = this->current;
  this->previous = this->current;

  if (cl < this->lines.size()) {
    if (cc >= std::get<0>(this->lines.at(cl))) {
      this->current = std::make_tuple(cl + 1, 0);
    }
    else {
      this->current = std::make_tuple(cl, cc + 1);
    }
  }

  int nl, nc;
  std::tie(nl, nc) = this->next;

  if (nl < this->lines.size()) {
    if (nc >= std::get<0>(this->lines.at(nl))) {
      this->next = std::make_tuple(nl + 1, 0);
    }
    else {
      this->next = std::make_tuple(nl, nc + 1);
    }
  }
}

void LineScanner::flush_line() {

  this->previous = std::make_tuple(std::get<0>(this->current) + 1,  -1);
  this->current = std::make_tuple(std::get<0>(this->current) + 1,  0);
  this->next = std::make_tuple(std::get<0>(this->next) + 1, 1);
}

position LineScanner::get_position() const {
  int l, c;
  std::tie(l,c) = this->current;

  return std::make_tuple(l + 1, c + 1);
}
