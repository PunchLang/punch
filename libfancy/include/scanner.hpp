/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <string>
#include <vector>
#include <tuple>

#include <boost/optional.hpp>
#include <boost/none.hpp>

typedef unsigned int uint;
typedef std::tuple<uint, uint> position;

class Scanner
{
public:
  virtual ~Scanner() {}

  virtual boost::optional<char> current_char() = 0;
  virtual boost::optional<char> next_char() = 0;
  virtual boost::optional<char> previous_char() = 0;
  virtual void pop() = 0;
  virtual void flush_line() = 0;
  virtual ::position position() = 0;

};

class StringScanner : public Scanner {

public:
  StringScanner(const std::string& in)
    : chars(in.begin(), in.end()), size (in.size()), index(0) {}
  boost::optional<char> current_char() override;
  boost::optional<char> next_char() override;
  boost::optional<char> previous_char() override;
  void pop() override;
  void flush_line() override;
  ::position position() override;

private:
  std::vector<char> chars;
  uint size;
  uint index;
};

class LineScanner : public Scanner {

public:
  LineScanner(const std::string&);
  boost::optional<char> current_char() override;
  boost::optional<char> next_char() override;
  boost::optional<char> previous_char() override;
  void pop() override;
  void flush_line() override;
  ::position position() override;

private:

  boost::optional<char> read_char(::position);

  std::vector<std::tuple<uint, std::vector<char>>> lines;
  ::position current;
  ::position next;
  ::position previous;
};


#endif /* _SCANNER_H_ */
