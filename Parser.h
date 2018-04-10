#ifndef LEXICALSCANNER_PARSER_H
#define LEXICALSCANNER_PARSER_H
//
//  Parser.h
//  Lexical Scanner
//
//  Created by Demondre Livingston on 3/12/18.
//  Copyright © 2018 Demondre Livingston. All rights reserved.
//
#include <cstdio>

class Parser {

public:

    void program();

    void keyword();

    void declare();

    void assign();

    void expr();

    void term();

    void funcname();

    void ident();

    void declareident();

    void programm();
};

#endif /* Parser_hpp */


