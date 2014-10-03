#include <iostream>
#include <fstream>
#include <exception>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include "stack.cpp"

using namespace std;

const map<string, string> SYMBOLS = {{"{","}"}, {"[","]"}, {"(",")"}, {"/*","*/"}};
const vector<string> OPENING_SYMBOLS = {"{", "[", "(", "/*"};
const vector<string> CLOSING_SYMBOLS = {"}", "]", ")", "*/"};

class balance_exception : exception
{
public:
    balance_exception(string opening, string closing, int line_num=-1)
        : opening{opening}, closing{closing}, line_num{line_num}
    {}

    virtual const char* what() const throw()
    {
        ostringstream ss {"Balancing error: "};

        if (opening.empty()) {
            ss << "No matching opening symbol found for ";
            ss << closing;
        } else if (closing.empty()) {
            ss << "No matching closing symbol found for ";
            ss << opening;
        } else {
            ss << opening << " or ";
            ss << closing << " does not match";
        }

        ss << " (line ";
        if (line_num > 0) {
            ss << line_num;
        } else {
            ss << "unknown";
        }
        ss << ")";

        return ss.str().c_str();
    }

    string opening, closing;
    int line_num;
};

// Returns true if opening and closing are matching symbols
// False if closing is not a closing symbol
// Throws a balance_exception if closing is a non-matching closing symbol
bool symbol_matches(string opening, string closing)
{
    if (SYMBOLS.find(opening)->second == closing) {
        // value at key 'opening' equals 'closing'; they match
        return true;
    }

    // checks if a pair with value 'closing' exists in the map
    auto it = find_if(SYMBOLS.cbegin(), SYMBOLS.cend(),
            [closing](pair<string,string> p){return p.second == closing;});
    if (it != SYMBOLS.cend()) {
        // 'closing' is a valid closing symbol, but does not match opening
        throw balance_exception(opening, closing);
    } else {
        // 'closing' is not a closing symbol
        return false;
    }
}

void check_line(string line, stack<string>& symbol_stack)
{
    // check each char: does it match top?
    // yes: pop and continue
    // no: throw error
    
    for (char c : line) {
        //TODO: more logic to ignore literals etc
        //TODO: need to deal with /* and */
        string sym{c};

        if (sym == "/") {
            // (/*) This one is tricky - 2 chars
        }

        if (SYMBOLS.count(sym) > 0) {
            // sym is an opening symbol
            symbol_stack.push(sym);
            continue;
        }

        try {
            if (symbol_matches(symbol_stack.top(), sym)) {
                // sym is the matching closing symbol
                symbol_stack.pop();
                continue;
            }
        } catch (const out_of_range& oor) {
            // the stack is empty but a closing symbol was found
            throw balance_exception("", sym);
        }
    }
}

void check_balancing(string file_name)
{
    ifstream infile(file_name);
    string line;
    int line_num{1};
    stack<string> symbol_stack;

    while (infile >> line) {
        try {
            check_line(line, symbol_stack);
            ++line_num;
        } catch (const balance_exception& be) {
            throw balance_exception(be.opening, be.closing, line_num);
        }
    }

    if (!symbol_stack.empty()) {
        // not all opening symbols were closed
        throw balance_exception(symbol_stack.top(), "", --line_num);
    }
}

int main()
{
    string file_name;
    cout << "Enter the name of the file to be checked: " << endl;
    cin >> file_name;

    try{
        check_balancing(file_name);
        cout << file_name << ": all symbols balance" << endl;
    }catch (const balance_exception& be) {
        cerr << be.what() << endl;
    }
}
