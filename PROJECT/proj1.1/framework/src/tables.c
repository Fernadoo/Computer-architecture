
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMBOLTBL_NON_UNIQUE = 0;
const int SYMBOLTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_sym(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time.
   If memory allocation fails, you should call allocation_failed().
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
  /* YOUR CODE HERE */
  SymbolTable * table = (SymbolTable *)malloc(sizeof(SymbolTable));
  if (table == NULL){
    allocation_failed();
  }
  table->len = 0;
  table->cap = 100;
  table->mode = mode;
  table->tbl = (Symbol *)malloc(sizeof(Symbol) * table->cap);
  if (table->tbl == NULL){
    allocation_failed();
  }
  return table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    /* YOUR CODE HERE */
    int i;
    for (i = 0; i < table->len; i++){
      free(table->tbl[i].name);
    }

    free(table->tbl);
    free(table);
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE.
   1. ADDR is given as the byte offset from the first instruction.
   2. The SymbolTable must be able to resize itself as more elements are added.

   3. Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   4. If ADDR is not word-aligned, you should call addr_alignment_incorrect()
   and return -1.

   5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists
   in the table, you should call name_already_exists() and return -1.

   6.If memory allocation fails, you should call allocation_failed().

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
  /* YOUR CODE HERE */
  /* not word-aligned */
  /*char *tmp ;*/
  int i;
  if (addr % 4 != 0){
    addr_alignment_incorrect();
    return -1;
  }
  /* name already exists */
  if (table->len > 0){
    if (table->mode == SYMBOLTBL_UNIQUE_NAME) {
      for (i = 0; i < table->len; ++i){
        if (strcmp(table->tbl[i].name, name)== 0){
          name_already_exists(name);
          return -1;
        }
      }
    }
  }

  /* if exceeded, create new and copy */
  /*
  if (table->len+1 > table->cap){
    SymbolTable* newtable = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (newtable == NULL){
      allocation_failed();
    }
    newtable->len = newtable->len;
    newtable->cap = table->cap * 2;
    newtable->mode = table->mode;
    newtable->tbl = (Symbol *)malloc(sizeof(Symbol) * newtable->cap);
    if (newtable->tbl == NULL){
      allocation_failed();
    }
    for (int i = 0; i < table->len; i++){
      newtable->tbl[i] = table->tbl[i];
    }

    free_table(table);
    table = newtable;
  }
  */
  /* add the new symbol */

  table->len += 1;
  /*Symbol newsymbol;*/


/*tmp =(char*)malloc((sizeof(char)+1)*strlen(name));
  strcpy(tmp,name);*/

  /*printf("%p\n", tmp);*/


  /*newsymbol.name = tmp;
  newsymbol.addr = addr;
  table->tbl[table->len - 1] = newsymbol;*/
  table->tbl[table->len-1].name = (char*)malloc((sizeof(char)+1)*strlen(name));
  strcpy(table->tbl[table->len-1].name,name);
  table->tbl[table->len-1].addr = addr;

  /*free(tmp);*/
  return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
  /* YOUR CODE HERE */
  int i;
  for (i =0; i < table->len; i++){
    if (strcmp(name,table->tbl[i].name) == 0){
      return table->tbl[i].addr;
    }
  }
  return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
  /* YOUR CODE HERE */
  int i;
  for (i = 0; i < table->len; i++){
  /*printf("addr: %d name: %s\n",table->tbl[i].addr, table->tbl[i].name);*/
    write_sym(output, table->tbl[i].addr, table->tbl[i].name);
  }
}
