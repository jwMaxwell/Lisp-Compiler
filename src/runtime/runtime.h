#ifndef RUNTIME_H
#define RUNTIME_H

#include <cstdint>
#include <vector>

extern "C" {

typedef struct Value Value;
typedef struct Cons Cons;

enum Tag { T_NIL = 0, T_NUMBER, T_STRING, T_SYMBOL, T_CONS, T_BOOL };

struct Value {
  int32_t tag; // type
  union {
    bool boolean;
    double num;
    void *ptr; // for NIL values
    char *sym;
    char *str;
    Cons *cons;
  } payload;
  uint8_t marked; // gc mark bit
  Value *gc_next; // intrusive LL for gc sweeps
};

struct Cons {
  Value *car;
  Value *cdr;
};

Value *box_nil();
Value *box_bool(int b);
Value *box_number(double v);
Value *box_string(const char *s);
Value *box_symbol(const char *s);

bool unbox_bool(Value *v);
double unbox_number(Value *v);
char *unbox_string(Value *v);
char *unbox_symbol(Value *v);

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
