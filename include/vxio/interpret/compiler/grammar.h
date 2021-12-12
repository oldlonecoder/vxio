//
// Created by oldlonecoder on 21-12-12.
//

//#ifndef VXIO_GRAMMAR_H
//#define VXIO_GRAMMAR_H

/**********************************************************************************************************************************
    Copyright 2021 Serge Lussier (oldlonecoder), lussier.serge@gmail.com
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
    THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*************************************************************************************************************************************/


#pragma once

#include <vxio/util/logger.h>
#include <vxio/lexer/token_data.h>
#include <map>
#include <stack>

namespace vxio
{
struct rule;

class grammar;

/*!
 * @brief
 * @todo Holly shit!! Confused on X flag: Accept state and Directive Flag!!!! ^^ ?
 *
 */
struct term_properties
{
    uint8_t Z: 1; ///< Zero or one (optional * )
    uint8_t R: 1; ///< Repeat      (        + )
    uint8_t L: 1; ///< List        (one of  ~ ?)
    uint8_t X: 1; ///< directive    ( exclude from normal ast build : ast directive [Implicit specific Parser term_rule)
    uint8_t S: 1; ///< State!! 0 = unset/rejected; 1 = set/accepted !!!
    
    term_properties &operator|(term_properties const &_)
    {
        Z |= _.Z;
        R |= _.R;
        L |= _.L;
        X |= _.X;
        //s = A.s;
        return *this;
    }
    
    term_properties &operator+()
    {
        R = 1;
        return *this;
    }
    term_properties &operator*()
    {
        Z = 1;
        return *this;
    }
    term_properties &operator!()
    {
        X = 1;
        return *this;
    }
    term_properties &operator~()
    {
        L = 1;
        return *this;
    }
    void Reset()
    { Z = R = L = X = S = 0; }
    std::string operator()();
    [[nodiscard]] bool is_optional() const
    { return Z != 0; }
    [[nodiscard]] bool is_oneof() const
    { return L != 0; }
    [[nodiscard]] bool is_strict() const
    { return Z == 0 && L == 0 && R == 0; }
    [[nodiscard]] bool is_repeat() const
    { return R != 0; }
    [[nodiscard]] bool accepted() const
    { return S != 0; }
    void accept()
    { S = 1; }
    void reject()
    { S = 0; }
};

struct term
{
    mutable term_properties a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match
    
    enum class type : uint8_t
    {
        //term,
        r = 0,
        s,
        m,
        nil
    };
    
    void Accept()
    { a.accept(); }
    void Reject()
    { a.reject(); }
    
    term::type _type  = term::type::nil;
    
    union info
    {
        rule          *r;
        vxio::type::T sem;
        mnemonic      m;
    }          object = {nullptr};
    
    using collection = std::vector<term>;
    using iterator = collection::iterator;
    using const_iterator = collection::const_iterator;
    
    // pas le choix... 'y faut un parser....
    
    term() = default;
    term(rule *r, term_properties a_ = {0, 0, 0, 0, 0});
    term(vxio::type::T a_sem, term_properties a_ = {0, 0, 0, 0, 0});
    term(mnemonic a_code, term_properties a_ = {0, 0, 0, 0, 0});
    
    term(const std::string &a_lexem);
    
    term(term &&_t) noexcept;
    term(const term &_t);
    
    term &operator=(term &&_t) noexcept;
    term &operator=(const term &_t);
    
    bool operator==(const term &t) const;
    bool operator==(const token_data &t) const;
    bool operator!=(const token_data &t) const;
    
    operator bool()
    { return _type != term::type::nil; }
    ~term();
    
    term &operator*()
    {
        *a;
        return *this;
    }
    term &operator+()
    {
        +a;
        return *this;
    }
    term &operator!()
    {
        !a;
        return *this;
    }
    term &operator~()
    {
        ~a;
        return *this;
    }
    
    std::string operator()() const;
    
    bool IsRule() const
    { return _type == type::r; }
    bool IsSemantic() const
    { return _type == type::s; }
    bool IsMnemonic() const
    { return _type == type::m; }
    bool IsNull() const
    { return _type == type::nil; }
    
    static term Query(const char *C_);
    static term Query(vxio::type::T T_);
    static term Query(mnemonic M_);
};

struct term_seq
{
    term_properties a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match
    
    //using data = std::vector<term_list_t>;
    
    term::collection terms;
    
    using collection = std::vector<term_seq>;
    using const_iterator = collection::const_iterator;
    using iterator = collection::iterator;
    using stack = std::stack<iterator>;
    
    term_seq() = default;
    
    term::const_iterator begin() const
    { return terms.cbegin(); }
    
    bool end(term::const_iterator t) const
    {
        return terms.cend() == t;
    }
    
    term next(term::const_iterator &it) const;
    
    ~term_seq()
    {
        terms.clear();
    }
    term_seq &operator<<(term a_t);
    
    // Emplace_back:
    term_seq &operator<<(vxio::type::T a_t);
    term_seq &operator<<(mnemonic a_t);
    term_seq &operator<<(rule *a_t);
};

struct rule
{
    term_seq::collection sequences;
    // ---------------------------------------
    term_seq::iterator   seq; // Temporary held for building this rule.
    // ---------------------------------------
    //term_seq::stack seq_stack;
    
    using collection = std::map<std::string, rule *>;
    using iterator = collection::const_iterator;
    
    term_properties a      = {0, 0, 0, 0, 0}; /// Volatile attribute infos. ( Copied into the rule's recursion context  )
    term_properties inject = {0, 0, 0, 0, 0};
    
    std::string _id;
    
    rule() = default;
    rule(const std::string &a_id);
    
    rule(int)
    {}
    ~rule();
    bool empty() const
    { return sequences.empty() ? true : sequences.begin()->terms.empty(); }
    void inject_attr(term_properties a_a)
    { inject = a_a; }
    rule &new_sequence();
    rule &operator|(rule *_r);
    //rule& operator |(const char*   _t);
    rule &operator|(vxio::type::T _t);
    rule &operator|(mnemonic _t);
    std::string name()
    { return _id; }
    term_seq::const_iterator Begin() const
    { return sequences.cbegin(); }
    bool End(term_seq::const_iterator s) const
    { return s == sequences.cend(); }
};

/*!
    @brief the tea gammar class .

    @author &copy;2021, oldlonecoder (lussier.serge@gmail.com).

    @note offset really don't know why offset need to do a complete syntax grammar (generator) for the tea++ interpreter.
    My first intention was to create a specific arithmetic expression parser and then create a hand made parser for the the rest of the syntax.
*/
class grammar
{

public:
    grammar();
    
    ~grammar();
    iostr &text()
    { return _Text; }
    rem::code build();
    
    void Dump();

private:
    enum state_mac
    {
        st_begin = 0, ///< about to define new rule: cursor on rule id.
        st_init_rule, ///< initialize sequence of terms: cursor on ':'.
        st_seq,       ///< back and forth between st_seq<->st_init_term.
        st_option,    ///< one of ' *+? ' option state.
        st_init_term, ///< query {rule | type semantic | e_code | litteral} : cursor on identifier.
        st_add_seq,   ///< terminate current sequence and start a new sequence on the rule: cursor on ','.
        st_end_rule   ///< terminate rule : cursor on '.'.
    };
    
    term_properties a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match
    
    state_mac               _state = grammar::st_begin;
    
    [[maybe_unused]] int Init();
    static rule::collection _Rules;
    rule                    *_Rule = nullptr;
    static rule *QueryRule(const std::string &a_id);
    
    using RuleScanner = rem::code(grammar::*)(iostr::Iterator &);
    iostr::word::list_t     tokens;
    iostr                   _Text;
    
    using Dictionary = std::map<char, grammar::RuleScanner>;
    static Dictionary grammar_dictionnary;

public:
    const rule *operator[](const std::string &r_id) const
    {
        return _Rules[r_id];
        //rule* r = _Rules[r_id]; rem::code (const rule*)r;
    }
    
    static bool built()
    { return _Rules.size() != 0; }
    
    static void init_rules();

private:
    grammar::Dictionary::iterator RuleIt;
    
    //--------------- Rules builders -------------------
    
    rem::code ParseIdentifier(iostr::Iterator &crs);
    rem::code EnterRuleDef(iostr::Iterator &crs);
    rem::code NewSequence(iostr::Iterator &crs);
    rem::code EndRule(iostr::Iterator &crs);
    rem::code SetRepeat(iostr::Iterator &crs);
    rem::code SetOptional(iostr::Iterator &crs);
    rem::code EnterLitteral(iostr::Iterator &crs);
    rem::code SetOneof(iostr::Iterator &crs);
    rem::code SetDirective(iostr::Iterator &crs);
};
}
//#endif //VXIO_GRAMMAR_H
