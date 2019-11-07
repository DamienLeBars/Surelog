/*
 Copyright 2019 Alain Dargelas

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * File:   StringUtils.cpp
 * Author: alain
 *
 * Created on March 14, 2017, 10:43 PM
 */

#include "StringUtils.h"
#include <algorithm>
#include <locale>
#include <regex>
#include <sstream>
#include <iostream>

using namespace SURELOG;

StringUtils::StringUtils() {}

StringUtils::StringUtils(const StringUtils& orig) {}

StringUtils::~StringUtils() {}

std::string StringUtils::to_string(double a_value, const int n) {
  std::ostringstream out;
  out.precision(n);
  out << std::fixed << a_value;
  return out.str();
}

void StringUtils::tokenizeMulti(std::string str, std::string separator,
                                std::vector<std::string>& args) {
  std::string tmp;
  unsigned int sepSize = separator.size();
  unsigned int stringSize = str.size();
  for (unsigned int i = 0; i < stringSize; i++) {
    bool isSeparator = true;
    for (unsigned int j = 0; j < sepSize; j++) {
      if (i + j >= stringSize) break;
      if (str[i + j] != separator[j]) {
        isSeparator = false;
        break;
      }
    }
    if (isSeparator) {
      i = i + sepSize - 1;
      args.push_back(tmp);
      tmp = "";
      if (i == (str.size() - 1)) args.push_back(tmp);
    } else if (i == (str.size() - 1)) {
      tmp += str[i];
      args.push_back(tmp);
      tmp = "";
    } else {
      tmp += str[i];
    }
  }
}

void StringUtils::tokenize(std::string str, std::string separator,
                           std::vector<std::string>& args) {
  std::string tmp;
  unsigned int sepSize = separator.size();
  unsigned int stringSize = str.size();
  for (unsigned int i = 0; i < stringSize; i++) {
    bool isSeparator = false;
    for (unsigned int j = 0; j < sepSize; j++) {
      if (str[i] == separator[j]) {
        isSeparator = true;
        break;
      }
    }
    if (isSeparator) {
      args.push_back(tmp);
      tmp = "";
      if (i == (str.size() - 1)) args.push_back(tmp);
    } else if (i == (str.size() - 1)) {
      tmp += str[i];
      args.push_back(tmp);
      tmp = "";
    } else {
      tmp += str[i];
    }
  }
}

void StringUtils::tokenizeBalanced(std::string str, std::string separator,
                                   std::vector<std::string>& args) {
  std::string tmp;
  unsigned int sepSize = separator.size();
  unsigned int stringSize = str.size();
  int level = 0;
  bool inDoubleQuote = false;
  for (unsigned int i = 0; i < stringSize; i++) {
    if (str[i] == '"') {
      if (inDoubleQuote == false) {
        level++;
        inDoubleQuote = true;
      } else {
        level--;
        inDoubleQuote = false;
      }
    }
    if (str[i] == '(' || str[i] == '[' || str[i] == '{') {
      level++;
    }
    if (str[i] == ')' || str[i] == ']' || str[i] == '}') {
      level--;
    }
    bool isSeparator = false;
    for (unsigned int j = 0; j < sepSize; j++) {
      if (str[i] == separator[j]) {
        if (level == 0) isSeparator = true;
        break;
      }
    }
    if (isSeparator) {
      args.push_back(tmp);
      tmp = "";
      if (i == (str.size() - 1)) args.push_back(tmp);
    } else if (i == (str.size() - 1)) {
      tmp += str[i];
      args.push_back(tmp);
      tmp = "";
    } else {
      tmp += str[i];
    }
  }
}

void StringUtils::replaceInTokenVector(std::vector<std::string>& tokens,
                                       std::vector<std::string> pattern,
                                       std::string news) {
  unsigned int patternIndex = 0;
  std::vector<std::string>::iterator itr;
  bool more = true;
  while (more) {
    more = false;
    for (itr = tokens.begin(); itr != tokens.end(); itr++) {
      if (*itr == pattern[patternIndex]) {
        patternIndex++;
        if (patternIndex == pattern.size()) {
          *itr = news;
          patternIndex = 0;
          itr = tokens.erase(itr - (pattern.size() - 1), itr);
          more = true;
        }
      } else {
        patternIndex = 0;
      }
    }
  }
}

void StringUtils::replaceInTokenVector(std::vector<std::string>& tokens,
                                       std::string pattern, std::string news) {
  unsigned int tokensSize = tokens.size();
  for (unsigned int i = 0; i < tokensSize; i++) {
    if (tokens[i] == pattern) tokens[i] = news;
  }
}

std::string StringUtils::getFirstNonEmptyToken(
    std::vector<std::string>& tokens) {
  unsigned int tokensSize = tokens.size();
  for (unsigned int i = 0; i < tokensSize; i++) {
    if (tokens[i] != " ") return tokens[i];
  }
  return "";
}

std::string& StringUtils::trim(std::string& str) { return ltrim(rtrim(str)); }

std::string& StringUtils::ltrim(std::string& str) {
  auto it2 = std::find_if(str.begin(), str.end(), [](char ch) {
    return !std::isspace<char>(ch, std::locale::classic());
  });
  str.erase(str.begin(), it2);
  return str;
}

std::string& StringUtils::rtrim(std::string& str) {
  auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) {
    return !std::isspace<char>(ch, std::locale::classic());
  });
  str.erase(it1.base(), str.end());
  return str;
}

std::string& StringUtils::rtrimEqual(std::string& str) {
  auto it1 = std::find_if(str.rbegin(), str.rend(),
                          [](char ch) { return (ch == '='); });
  if (it1 != str.rend()) str.erase(it1.base() - 1, str.end());
  return str;
}

std::string& StringUtils::rtrim(std::string& str, char c) {
  auto it1 = std::find_if(str.rbegin(), str.rend(),
                          [c](char ch) { return (ch == c); });
  if (it1 != str.rend()) str.erase(it1.base() - 1, str.end());
  return str;
}

std::string& StringUtils::ltrim(std::string& str, char c) {
  auto it1 =
      std::find_if(str.begin(), str.end(), [c](char ch) { return (ch == c); });
  if (it1 != str.end()) str.erase(str.begin(), it1 + 1);
  return str;
}

bool StringUtils::ltrimStat(std::string& str, char c) {
  auto it1 =
      std::find_if(str.begin(), str.end(), [c](char ch) { return (ch == c); });
  if (it1 != str.end()) {
    str.erase(str.begin(), it1 + 1);
    return true;
  }
  return false;
}

std::string StringUtils::leaf(std::string str) {
  std::string result;
  char c = '.';
  auto it1 = std::find_if(str.rbegin(), str.rend(),
                          [c](char ch) { return (ch == c); });
  if (it1 != str.rend()) str.erase(str.begin(), it1.base());
  return str;
  return result;
}

std::string& StringUtils::getRootFileName(std::string& str) {
  bool found = true;
  while (found) {
    found = ltrimStat(str, '/');
  }
  return str;
}

std::string StringUtils::replaceAll(std::string str, const std::string& from,
                                    const std::string& to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length();  // Handles case where 'to' is a substring of 'from'
  }
  return str;
}

std::string StringUtils::eliminateRelativePath(std::string path) {
  return replaceAll(path, "..", "__");
}

std::string StringUtils::getLineInString(std::string& bulk, unsigned int line) {
  std::string lineText;
  unsigned int size = bulk.size();
  const char* str = bulk.c_str();
  unsigned int count = 1;
  for (unsigned int i = 0; i < size; i++) {
    if (line == count) {
      lineText += str[i];
    }
    if (str[i] == '\n') count++;
    if (count > line) break;
  }

  return lineText;
}

std::string StringUtils::removeComments(std::string text) {
  std::string result;
  char c1 = '\0';
  bool inComment = 0;
  for (unsigned int i = 0; i < text.size(); i++)
    {
      char c2 = text[i];
      if ((c2 == '/') && (c1 == '/')) {
          inComment = true;
          result.erase(result.end()-1);
        }
      if (c2 == '\n')
        inComment = false;
      if (!inComment)
        result +=c2;
      c1 = c2;
    }
  
  return result;
}


 // Update the input string.
void autoExpandEnvironmentVariables( std::string & text ) {
    static std::regex env( "\\$\\{([^}]+)\\}" );
    std::smatch match;
    while ( std::regex_search( text, match, env ) ) {
        const char * s = getenv( match[1].str().c_str() );
        const std::string var( s == NULL ? "" : s );
        text.replace( match[0].first, match[0].second, var );
    }
}

// Leave input alone and return new string.
std::string StringUtils::evaluateEnvVars(std::string text) {
    std::string input = text;
    autoExpandEnvironmentVariables( input );
    return input;
}
