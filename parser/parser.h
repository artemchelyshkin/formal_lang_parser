#pragma once

#include <iostream>
#include <stack>
#include <string>

const int limit = 1000000;


struct _language {

  void not_x() {
    largest_string = -1;
    largest_suffix = 0;
  }

  void empty_lang() {
    largest_string = 0;
    largest_suffix = 0;
  }

  void x() {
    largest_string = 1;
    largest_suffix = 1;
  }

  int largest_string = 0;

  int largest_suffix = 0;

  _language operator + () {
    _language new_lang;
    if (this->largest_string > 0) {
      new_lang.largest_string = limit;
      new_lang.largest_suffix = limit;
    }
    else {
      new_lang.largest_string = 0;
      new_lang.largest_suffix = this->largest_suffix;
    }
    return new_lang;
  }

  _language operator | (_language another) {
    _language new_lang;
    new_lang.largest_string = std::max(this->largest_string, another.largest_string);
    new_lang.largest_suffix = std::max(this->largest_suffix, another.largest_suffix);
    return new_lang;
  }

  _language operator ^ (_language another) {
    _language new_lang;
    new_lang.largest_string = (this->largest_string >= 0 && another.largest_string >= 0) ?
      this->largest_string + another.largest_string : -1;
    new_lang.largest_string = (new_lang.largest_string > limit) ? limit : new_lang.largest_string;
    new_lang.largest_suffix = (another.largest_string >= 0) ?
      std::max(this->largest_suffix + another.largest_string, another.largest_suffix) : another.largest_suffix;
    new_lang.largest_suffix = (new_lang.largest_suffix > limit) ? limit : new_lang.largest_suffix;
    return new_lang;
  }

};

struct parse_reg_expression {

  char character;

  std::stack<_language> converted_reg_expression;

  void process(char elem) {
    switch (elem) {
    case '.':
      check_size_of_stack(elem);
      process_concat();
      break;
    case '+':
      check_size_of_stack(elem);
      process_plus();
      break;
    case '*':
      check_size_of_stack(elem);
      process_klini_star();
      break;
    default:
      check_symbol(elem);
      process_symbol(elem);
      break;
    }
  }

  int get_result() {
    if (converted_reg_expression.size() > 1) {
      throw std::invalid_argument{ "Error while proceesing expression: invalid expression" };
    }
    if (converted_reg_expression.size() == 0) {
      throw std::logic_error{ "Expression hasn't been parsed" };
    }
    else {
      auto final_lang = converted_reg_expression.top();
      return std::max(final_lang.largest_string, final_lang.largest_suffix);
    }
  }

private:

  void process_concat() {
    _language right = converted_reg_expression.top();
    converted_reg_expression.pop();
    _language left = converted_reg_expression.top();
    converted_reg_expression.pop();
    converted_reg_expression.push(left ^ right);
  }

  void process_plus() {
    _language right = converted_reg_expression.top();
    converted_reg_expression.pop();
    _language left = converted_reg_expression.top();
    converted_reg_expression.pop();
    converted_reg_expression.push(left | right);
  }

  void process_klini_star() {
    _language reg = converted_reg_expression.top();
    converted_reg_expression.pop();
    converted_reg_expression.push(+reg);
  }

  void process_symbol(char elem) {
    _language new_lang;
    if (elem == '1') {
      new_lang.empty_lang();
    }
    if (elem != character) {
      new_lang.not_x();
    }
    if (elem == character) {
      new_lang.x();
    }
    converted_reg_expression.push(new_lang);
  }

  void check_symbol(char elem) {
    if (!(elem >= 'a' && elem <= 'c') && elem != '1') {
      throw std::invalid_argument{ "Error while proceesing expression: alphabet doesn't contain element " + std::to_string(elem) };
    }
  }

  void check_size_of_stack(char elem) {
    switch (elem) {
    case '*':
      if (converted_reg_expression.size() < 1) {
        throw std::invalid_argument{ "Error while processing expression: operator * expected 1 expression, 0 given" };
      }
      break;
    case '.':
      if (converted_reg_expression.size() < 2) {
        throw std::invalid_argument{ "Error while processing expression: operator . expected 2 expressions, " +
            std::to_string(converted_reg_expression.size()) + " given" };
      }
      break;
    case '+':
      if (converted_reg_expression.size() < 2) {
        throw std::invalid_argument{ "Error while processing expression: operator + expected 2 expressions, " +
          std::to_string(converted_reg_expression.size()) + " given" };
      }
      break;
    }
  }

};

class reg_expression {
private:

  std::string rev_polish_notation;

  parse_reg_expression parser;

  bool initialized_symbol = false;

  bool parsed = false;

  bool initialized_string = false;

public:

  void expression(std::string str) {
    rev_polish_notation = str;
    initialized_string = true;
  }

  reg_expression(std::string str, char symb) : rev_polish_notation(str) {
    parser.character = symb;
    initialized_symbol = 1, initialized_string = 1;
    parse();
  }

  reg_expression() {}

  void letter_x(char symb) {
    parser.character = symb;
    initialized_symbol = true;
  }

  void parse() {
    if (!initialized_string) {
      throw std::logic_error{ "Error : reverse polish notation hasn't been declared" };
    }
    if (!initialized_symbol) {
      throw std::logic_error{ "Error : symbol hasn't been declared" };
    }
    parser.converted_reg_expression = std::stack<_language>();
    for (auto symbol = rev_polish_notation.begin(); symbol != rev_polish_notation.end(); ++symbol) {
      char elem = *symbol;
      parser.process(elem);
    }
    parsed = true;
  }

  int find_largest_suffix() {
    if (!parsed) {
      throw std::logic_error{ "Error : The expression hasn't been parsed" };
    }
    return parser.get_result();
  }

};
