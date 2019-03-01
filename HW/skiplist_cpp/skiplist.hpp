/* You may only include these two header files. */
#include <utility>      /* For std::pair. */
#include <functional>   /* For std::less. */
using namespace std;

/* You may finally allowed to use // in C++. */


#define RAND_HALF ((RAND_MAX) / 2)


//
//
// The implementation for skiplist class follows here!
//
//
//                S K I P L I S T
//
//

/* ==================== Begin students' code ==================== */

/* One possible function definition has been filled for you,
 * you may change it as you wish. */

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::skiplist(int height_limit, Compare cmp){
  _level = 1; //the list level
  Height_limit = height_limit; // the height limit
  Cmp = cmp; // pass the compare func
  _size = 0; // pass the size
  header = new _listnode(Key(), Val(), height_limit); // create the header
}

template<class Key, class Val, class Compare>
typename skiplist<Key, Val, Compare>::iterator
skiplist<Key, Val, Compare>::find(const Key &key){
  int curr_lv = _level - 1; // pass the parameter level
  _listnode* curr = header; // pass the header
  while(curr_lv >= 0){ // while the loop
    if (!curr->forward[curr_lv]){ // the end is null
      curr_lv--; // move down
      continue; // and skip to the next level
    }
    if (Cmp(key,curr->forward[curr_lv]->_data.first) == true){ // if the key is less
      curr_lv--; // downward
    }else if (Cmp(key,curr->forward[curr_lv]->_data.first) == false && Cmp(curr->forward[curr_lv]->_data.first,key) == false){
      return iterator(curr->forward[curr_lv]); // if key equals , find it
    }else if (Cmp(key,curr->forward[curr_lv]->_data.first) == false){
      curr = curr->forward[curr_lv]; // if larger, move forward
    }
  }
  return end(); // return nullptr
}

template<class Key, class Val, class Compare>
typename skiplist<Key, Val, Compare>::const_iterator
skiplist<Key, Val, Compare>::find(const Key &key) const{
  int curr_lv = _level - 1; // pass the level
  _listnode* curr = header; // pass the header
  while(curr_lv >= 0){ // while the loop
    if (!curr->forward[curr_lv]){
      curr_lv--; // if the end is null
      continue; // move downward and skip this loop
    }
    if (Cmp(key,curr->forward[curr_lv]->_data.first) == true){
      curr_lv--; // if less, move downward
    }else if (Cmp(key,curr->forward[curr_lv]->_data.first) == false && Cmp(curr->forward[curr_lv].first,key) == false){
      return const_iterator(curr->forward[curr_lv]); // if equals, find it
    }else if (Cmp(key,curr->forward[curr_lv]->_data.first) == false){
      curr = curr->forward[curr_lv]; // if larger, move forward
    }
  }
  return end(); //return null
}

template<class Key, class Val, class Compare>
std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>
skiplist<Key, Val, Compare>::insert(const Key &key, const Val &val){
  int curr_lv = _level - 1; // pass the level
  _listnode* curr = header; //pass the header
  _listnode* trace[Height_limit]; // create a list of trace
  for (int i = 0; i < Height_limit; i++){
    trace[i] = header; // record each node in the trace
  }
  while (curr_lv >= 0){ // while the loop
    if (!curr->forward[curr_lv]){ // if the end is null
      trace[curr_lv] = curr; // record it
      curr_lv--; // move downward
      continue; // and skip this loop
    }
    if (Cmp(key,curr->forward[curr_lv]->_data.first) == true){
      trace[curr_lv] = curr; // if less, move downward
      curr_lv--;
    }else if (Cmp(key,curr->forward[curr_lv]->_data.first) == false && Cmp(curr->forward[curr_lv]->_data.first,key) == false){
      curr->forward[curr_lv]->_data.second = val; // if exists, change the val
      std::pair<skiplist<Key,Val,Compare>::iterator,bool> rtpair = make_pair(iterator(curr->forward[curr_lv]), false);
      return rtpair; // return pair with false
    }else if (Cmp(key,curr->forward[curr_lv]->_data.first) == false){
      curr = curr->forward[curr_lv]; // move forward
    }
  }
  int nodelevel = 0; // declare the nodelevel
  _listnode* newnode = new _listnode(key,val,Height_limit);
  while (nodelevel < Height_limit-1 && rand() < RAND_HALF){
    nodelevel++; // if satisfied, add the nodelevel
  }
  if (nodelevel > _level - 1){
    _level  = nodelevel + 1; // if exceeded, update the listlevel
  }
  for (int i = 0; i <= nodelevel; i++){
    newnode->forward[i] = trace[i]->forward[i];
    trace[i]->forward[i] = newnode; // make the trace online
  }
  _size += 1; // add size
  std::pair<skiplist<Key,Val,Compare>::iterator,bool> rtpair = make_pair(iterator(newnode),true);
  return rtpair; // return a pair with true
}

template<class Key, class Val, class Compare>
bool
skiplist<Key, Val, Compare>::erase(const Key &key){
  _listnode* tmp; // declare a tmp which will be equal to the to-be-deleted node
  int nodelevel = 0; // declare the nodelevel
  int curr_lv = _level - 1; // pass the level
  _listnode* curr = header; // pass the header
  while (curr_lv >= 0){ // while the loop
    if (!curr->forward[curr_lv]){
      curr_lv--; // if the end is null
      continue; // move downward and skip this loop
      // printf("downward\n");
    }
    if (Cmp(key,curr->forward[curr_lv]->_data.first) == true){
      curr_lv--; // if less move forward
      // printf("downward\n");
    }else if (Cmp(key,curr->forward[curr_lv]->_data.first) == false && Cmp(curr->forward[curr_lv]->_data.first,key) == false){
      tmp = curr->forward[curr_lv]; // if equals, find the node
      curr->forward[curr_lv] = curr->forward[curr_lv]->forward[curr_lv];
      if (curr_lv == 0){
        // printf("**********\n");
        // tmp = curr->forward[0];
        delete tmp; //delete the node
        _size -= 1; // reduct the size
        for (int i = 0; i < Height_limit; i++){
          if (header->forward[i]){
            nodelevel += 1;
          }
        } // recalculate the level
        if (nodelevel==0){
          _level = 1; // level 0 => 1
        }else{
          _level = nodelevel;
        }
        return true;
      }
      curr_lv -= 1; // move downward
    }else if (Cmp(key,curr->forward[curr_lv]->_data.first) == false){
      curr = curr->forward[curr_lv]; // move forward
    }
  }
  return false; //return false
}

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::~skiplist(){
  _listnode* curr = header; // the 1st tracer
  _listnode* last = header; // the 2nd tracer
  while (curr){ // ??? while (curr->forward[0]) why not ??
    curr = curr->forward[0];
    delete last; // delete each node
    last = curr; // and move forward
  }
}

template<class Key, class Val, class Compare>
inline bool
skiplist<Key, Val, Compare>::empty() const{
  return _size==0; // check if size == 0
}

template<class Key, class Val, class Compare>
inline int
skiplist<Key, Val, Compare>::size() const{
  return _size; // return size directly
}

template<class Key, class Val, class Compare>
inline int
skiplist<Key, Val, Compare>::level() const{
  return _level; // return level
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::iterator
skiplist<Key, Val, Compare>::begin(){
  return iterator(header->forward[0]); // return a pointer to the first node
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::iterator
skiplist<Key, Val, Compare>::end(){
  _listnode* ptr = nullptr; // return a pointer to the element after the final node
  return iterator(ptr);
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::const_iterator
skiplist<Key, Val, Compare>::begin() const{
  return const_iterator(header->forward[0]); // do as above
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::const_iterator
skiplist<Key, Val, Compare>::end() const{
  _listnode* ptr = nullptr; // do as above
  return const_iterator(ptr);
}


/* This may seem obscure for beginners, so here's a simple explanation:
    - "template<class Key, class Val, class Compare>"
        - We need to use template for generic skiplist class.
    - "std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>"
        - This is the return type of this function, see insert function in skiplist.h.
        - "typename skiplist<Key, Val, Compare>::iterator"
            - `typename` specifies that the type "skiplist<Key, Val, Compare>" should
                have a field called "iterator", and then use it as the type of `first`
                for std::pair.
        - It is analogy to "std::pair<iterator, bool>" in skiplist.h.
    - "skiplist<Key, Val, Compare>::insert(const Key &key, const Val &val)"
        - The real function name, refers to the inline declaration in skiplist.h.
        - `insert` belongs to type "skiplist<Key, Val, Compare>".

    This function may have the most complicated definiton in this file. The
    definition for other functions would be more friendly. You should know how
    other functions are defined after reading this explanation.
 */


/* ==================== End students' code ==================== */



//
//
// The implementation for _listnode subclass follows here!
//
//
//                L I S T   N O D E
//
//

/* ==================== Begin students' code ==================== */
/* One possible function definition has been filled for you,
 * you may change it as you wish. */

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::_listnode::_listnode(const Key& key, const Val& val, int height_limit):
  _data(key,val){
  node_height_limit = height_limit; // pass the height_limit
  forward = new _listnode*[node_height_limit]{}; // create and initialize
}

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::_listnode::~_listnode() { delete[] forward; } // free the forward
/* Simple explanation:
    - The first line is the same template argument as described in the first section.
    - "skiplist<Key, Val, Compare>::_listnode::~_listnode()"
        - This is the destructor function of _listnode subclass, so it does not
            has a return type.
        - `~_listnode` is the destructor function name inside class
            "skiplist<Key, Val, Compare>::_listnode".
 */


/* ==================== End students' code ==================== */



//
//
// The implementation for iterator subclass follows here!
//
//
//                I T E R A T O R
//
//

/* ==================== Begin students' code ==================== */

/* One possible function definition has been filled for you,
 * you may change it as you wish. */

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::iterator::iterator(_listnode *ptr) { nodeptr = ptr; } //the constructor

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::iterator::~iterator() {;} //the deconstructor

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::iterator &
skiplist<Key, Val, Compare>::iterator::operator++(){ //override the ++i
  nodeptr = nodeptr->forward[0];
  return *this;
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::iterator
skiplist<Key, Val, Compare>::iterator::operator++(int){ //override the i++
  iterator tmp = *this;
  nodeptr = nodeptr->forward[0];
  return tmp;
}

template<class Key, class Val, class Compare>
inline bool
skiplist<Key, Val, Compare>::iterator::operator==(const iterator &other) const { // override the ==
  return nodeptr == other.nodeptr;
}

template<class Key, class Val, class Compare>
inline bool
skiplist<Key, Val, Compare>::iterator::operator!=(const iterator &other) const { // override the !=
  return nodeptr != other.nodeptr;
}

template<class Key, class Val, class Compare>
inline std::pair<const Key, Val> &
skiplist<Key, Val, Compare>::iterator::operator*() const { return nodeptr->_data; } //override the *

template<class Key, class Val, class Compare>
inline std::pair<const Key, Val> *
skiplist<Key, Val, Compare>::iterator::operator->() const { return &(nodeptr->_data); } // override the &

/* Simple explanation:
    - The first line is the same template argument as described in the first section.
    - "std::pair<const Key, Val> &"
        - The return type of the dereference operator.
        - This is not the data pair itself, but a reference to the pair.
    - "skiplist<Key, Val, Compare>::iterator::operator*()"
        - The dereference funtion, refers to the inline declaration in skiplist.h.
 */

/* ==================== End students' code ==================== */


//
//
// The implementation for iterator subclass follows here!
//
//
//                C O N S T      I T E R A T O R
//
//
/* ==================== Begin students' code ==================== */
/* Fill in the const_iterator part, this should be similiar to iterator */
template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::const_iterator::const_iterator(_listnode *ptr) { nodeptr = ptr; } // override the constructor

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::const_iterator::~const_iterator() { ; } // override the const_iterator

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::const_iterator &
skiplist<Key, Val, Compare>::const_iterator::operator++(){ // override the ++i
  nodeptr = nodeptr->forward[0];
  return *this;
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::const_iterator
skiplist<Key, Val, Compare>::const_iterator::operator++(int){ // override the i++
  const_iterator tmp = *this;
  nodeptr = nodeptr->forward[0];
  return tmp;
}

template<class Key, class Val, class Compare>
inline bool
skiplist<Key, Val, Compare>::const_iterator::operator==(const const_iterator &other) const { // override the ==
  return nodeptr == other.nodeptr;
}

template<class Key, class Val, class Compare>
inline bool
skiplist<Key, Val, Compare>::const_iterator::operator!=(const const_iterator &other) const { //override the !=
  return nodeptr != other.nodeptr;
}

template<class Key, class Val, class Compare>
inline std::pair<const Key, Val> &
skiplist<Key, Val, Compare>::const_iterator::operator*() const { return nodeptr->_data; } // override the *

template<class Key, class Val, class Compare>
inline std::pair<const Key, Val> *
skiplist<Key, Val, Compare>::const_iterator::operator->() const { return &(nodeptr->_data); } // override the ->

/* ==================== End students' code ==================== */



/* Auxiliary function for printing skiplist. */
template<class Key, class Val, class Compare>
void
skiplist<Key, Val, Compare>::print_list()
{
    /* Show current status. */
    printf("  Skiplist status is now:\n");
    printf("    -- Size: %d, Level: %d, Empty: %d\n", _size, _level, empty());
    /* Loop through list, starting from upper level. */
    for (int idx = _level - 1; idx >= 0; idx--) {
        _listnode *n = (header->forward)[idx];
        _listnode *_n = (header->forward)[0];
        printf("    -");
        /* For every node in list. */
        while (_n != nullptr && n != nullptr) {
            /* If node lives in level idx, print (key, val) pair. */
            if (_n == n) {
                printf(" [%3d, %3d] ", n->_data.first, n->_data.second);
                n = (n->forward)[idx];
            /* Else print blank spaces of same width. */
            } else
                printf("            ");
            _n = (_n->forward)[0];
        }
        /* Newline. */
        printf("\n");
    }
}
