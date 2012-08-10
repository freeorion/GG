/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003-2008 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@gmail.com */
   
#include <GG/StatementParser.h>

#include <GG/adobe/implementation/token.hpp>


namespace {

    adobe::aggregate_name_t constant_k = { "constant" };
    adobe::aggregate_name_t if_k       = { "if" };
    adobe::aggregate_name_t else_k     = { "else" };
    adobe::aggregate_name_t return_k   = { "return" };

    struct array_t_push_back_
    {
        template <typename Arg1, typename Arg2, typename Arg3 = void, typename Arg4 = void, typename Arg5 = void>
        struct result;

        template <typename Arg2, typename Arg3, typename Arg4, typename Arg5>
        struct result<adobe::array_t, Arg2, Arg3, Arg4, Arg5>
        { typedef void type; };

        template <typename Arg2>
        void operator()(adobe::array_t& array, Arg2 arg2) const
            { adobe::push_back(array, arg2); }

        template <typename Arg2, typename Arg3>
        void operator()(adobe::array_t& array, Arg2 arg2, Arg3 arg3) const
            {
                adobe::push_back(array, arg2);
                adobe::push_back(array, arg3);
            }

        template <typename Arg2, typename Arg3, typename Arg4>
        void operator()(adobe::array_t& array, Arg2 arg2, Arg3 arg3, Arg4 arg4) const
            {
                adobe::push_back(array, arg2);
                adobe::push_back(array, arg3);
                adobe::push_back(array, arg4);
            }

        template <typename Arg2, typename Arg3, typename Arg4, typename Arg5>
        void operator()(adobe::array_t& array, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) const
            {
                adobe::push_back(array, arg2);
                adobe::push_back(array, arg3);
                adobe::push_back(array, arg4);
                adobe::push_back(array, arg5);
            }
    };

    const boost::phoenix::function<array_t_push_back_> push;

}

using namespace GG;

statement_parser_rules::statement_parser_rules(
    const lexer& tok,
    const expression_parser_rules::expression_rule& expression
) {
    namespace qi = boost::spirit::qi;
    using qi::_1;
    using qi::_2;
    using qi::_3;
    using qi::_4;
    using qi::_a;
    using qi::_b;
    using qi::_c;
    using qi::_d;
    using qi::_r1;
    using qi::_val;
    using qi::eps;
    using qi::lit;

    const boost::spirit::lex::token_def<adobe::name_t>& constant =
        tok.keywords.find(constant_k)->second;
    const boost::spirit::lex::token_def<adobe::name_t>& if__ =
        tok.keywords.find(if_k)->second;
    const boost::spirit::lex::token_def<adobe::name_t>& else__ =
        tok.keywords.find(else_k)->second;
    const boost::spirit::lex::token_def<adobe::name_t>& return__ =
        tok.keywords.find(return_k)->second;

    const_declaration
        =     constant
        >     tok.identifier [_a = _1]
        >   - (
                   lit(':')
                >  expression(_b)
              )
        >     lit(';')
              [
                  push(_val, _a, _b, adobe::const_decl_k)
              ]
        ;

    declaration
        =     tok.identifier [_a = _1]
        >>  - (
                   lit(':')
                >  expression(_b)
              )
        >     lit(';')
              [
                  push(_val, _a, _b, adobe::decl_k)
              ]
        ;

    assignment
        =     tok.identifier [_a = _1]
        >>    lit('=') [push(_val, _a, adobe::lvalue_k)]
        >     expression(_val)
        >     lit(';')
              [
                  push(_val, adobe::assign_k)
              ]
        ;

    block
        =     '{'
        >   * statement [push(_r1, _1)]
        >     '}'
        ;

    if_statements
        =     block(_r1)
        |     statement [push(_r1, _1)]
        ;

    if_
        =     if__
        >     '('
        >     expression(_b)
        >     ')'
        >     if_statements(_c)
        >   - (
                   else__
               >   if_statements(_d)
              )
        >     eps
              [
                  push(_val, _b, _c, _d, adobe::stmt_ifelse_k)
              ]
        ;

    return_
        =     return__
        >     expression(_val)
        >     lit(';')
              [
                  push(_val, adobe::return_k)
              ]
        ;

    statement
        %=    const_declaration
        |     return_
        |     if_
        |     assignment
        |     declaration
        ;


    // define names for rules, to be used in error reporting
#define NAME(x) x.name(#x)
    NAME(statement);
    NAME(const_declaration);
    NAME(declaration);
    NAME(assignment);
    NAME(return_);
#undef NAME

    qi::on_error<qi::fail>(statement, report_error(_1, _2, _3, _4));
}
