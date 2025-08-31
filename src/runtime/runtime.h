#ifndef RUNTIME_H
#define RUNTIME_H

#include <cstdint>

extern "C" {

typedef struct Value Value;
typedef struct Cons Cons;

enum Tag { T_NIL = 0, T_NUMBER, T_STRING, T_SYMBOL, T_CONS, T_BOOL };

struct Value {
  int32_t tag;
  void *payload;
};

struct Cons {
  Value *car;
  Value *cdr;
};

Value *make_nil();
Value *make_bool(int b);
Value *make_number(double v);
Value *make_string(const char *s);
Value *make_symbol(const char *s);
Cons *make_cons(Value *car, Value *cdr);
Value *cons(Value *car, Value *cdr); // returns Value* wrapper for Cons
Value *car(Value *v);
Value *cdr(Value *v);
Value *is_atom(Value *v);
Value *eq(Value *a, Value *b);
Value *get_add(Value *a, Value *b);
Value *get_sub(Value *a, Value *b);
Value *get_mul(Value *a, Value *b);
Value *get_div(Value *a, Value *b);

void print_value(Value *v); // convenience, prints readable form
}

#endif // RUNTIME_H
