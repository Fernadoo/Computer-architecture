#include "skiplist.h"
#include <iostream>
#include <string>
using namespace std;

string b2s(bool b) {
    return b ? string("true") : string("false");
}

int main(void) {
    skiplist<int, int>::iterator it;
    skiplist<int, int>::const_iterator const_it;

    cout << "\n/* Testing initialization... */" << endl;
    skiplist<int, int> sl(10);
    sl.print_list();

    cout << "\n/* Testing insertion... */" << endl;
    cout << "  Inserting key: " << 5 << ", value: " << -32 << endl;
    sl.insert(5, -32);
    cout << "  Inserting key: " << 7 << ", value: " << 912 << endl;
    sl.insert(7, 912);
    cout << "  Inserting key: " << 13 << ", value: " << 277 << endl;
    cout << "    -- Inserting returns bool ?: " << b2s(sl.insert(13,277).second) << endl;
    cout << "  Inserting key: " << 13 << ", value: " << 977 << endl;
    cout << "    -- Inserting returns bool ?: " << b2s(sl.insert(13,977).second) << endl;
    cout << "  Inserting key: " << 9 << ", value: " << 211 << endl;
    it = sl.insert(9, 211).first;
    cout << "    -- Inserting returns correct iter ?: " << b2s(it->first == 9) << endl;
    sl.print_list();

    cout << "\n/* Testing erase... */" << endl;
    cout << "  Erasing key: " << 9 << endl;
    cout << "    -- Erasing returns bool ?: " << b2s(sl.erase(9)) << endl;
    sl.print_list();
    cout << "  Erasing key: " << 9 << "again" << endl;
    cout << "    -- Erasing returns bool ?: " << b2s(sl.erase(9)) << endl;
    sl.print_list();

    cout << "\n/* Testing find... */" << endl;
    cout << "  Finding key: " << 13 << endl;
    cout << "    -- Finding returns end() ?: " << b2s(sl.find(13) == sl.end()) << endl;
    cout << "  Finding key: " << 11 << endl;
    cout << "    -- Finding returns end() ?: " << b2s(sl.find(11) == sl.end()) << endl;
    cout << "  Finding key: " << 0  << endl;
    cout << "    -- Finding returns end() ?: " << b2s(sl.find(0) == sl.end()) << endl;
    sl.print_list();
    //
    // cout << "\n/* Testing begin, end and iterator... */" << endl;
    // cout << "  Making iterator `it' pointing to begin()" << endl;
    // it = sl.begin();
    // cout << "    -- `it' points to begin() - key 5 ?: " << b2s(it->first == 5) << endl;
    // cout << "  Doing `it++' increment..." << endl;
    // it++;
    // cout << "    -- `it' points to next one - key 7 ?: " << b2s(it->first == 7) << endl;
    // cout << "    -- `it' equals iter by find(7) ?: " << b2s(it == sl.find(7)) << endl;
    // cout << "    -- `it' not equal iter by find(5) ?: " << b2s(it != sl.find(5)) << endl;
    // cout << "    -- `it' now equals end() ?: " << b2s(it == sl.end()) << endl;
    // cout << "  Modify value to be " << 720 << "." << endl;
    // (*it).second = 720;
    // sl.print_list();
    // cout << "  Doing `++it' and `it++' increment" << endl;
    // ++it;
    // it++;
    // cout << "    -- `it' now equals end() ?: " << b2s(it == sl.end()) << endl;
    //
    // cout << "\n/* Testing const_iterator... */" << endl;
    // cout << "  Making iterator `const_it' pointing to begin()" << endl;
    // const skiplist<int, int> &const_sl = const_cast<skiplist<int, int> &>(sl);
    // const_it = const_sl.begin();
    // cout << "    -- `const_it' points to begin() - key 5 ?: " << b2s(const_it->first == 5) << endl;
    // cout << "  Doing three increments" << endl;
    // ++const_it;
    // const_it++;
    // ++const_it;
    // cout << "    -- `const_it' now equals end() ?: " << b2s(const_it == const_sl.end()) << endl;

    cout << "\n/* Test finish! */\n" << endl;
}
