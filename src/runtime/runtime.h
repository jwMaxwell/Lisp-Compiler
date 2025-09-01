#ifndef RUNTIME_H
#define RUNTIME_H

#include <cstdint>
#include <vector>

extern "C" {

typedef struct Value Value;
typedef struct Cons Cons;

enum Tag { T_NIL = 0, T_NUMBER, T_STRING, T_SYMBOL, T_CONS, T_BOOL };

struct Value {
  int32_t tag;     // type
  void *payload;   // value
  uint8_t marked;  // gc mark bit
  uint8_t _pad[3]; // TODO: Figure out what this is for
  Value *gc_next;  // intrusive LL for gc sweeps
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

void print_value(Value *v);

// GC API
void gc_collect();
void gc_set_stack_bottom(void *stack_bottom);
void gc_root_push(Value *);
void gc_root_pop_n(size_t n);
}

#endif // RUNTIME_H
