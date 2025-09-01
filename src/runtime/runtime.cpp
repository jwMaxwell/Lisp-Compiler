#include "runtime.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, Value *> symbol_table;

extern "C" {
// GC API
static Value *gc_head = nullptr;
static size_t gc_allocs_since_last = 0;
static size_t gc_threshold = 10000;
static thread_local std::vector<Value *> gc_root_stack;

static inline void
gc_register(Value *v) { // TODO: Might need to pull this out of "C"
  v->marked = 0;
  v->gc_next = gc_head;
  gc_head = v;
  if (++gc_allocs_since_last >= gc_threshold) {
    gc_collect();
  }
}

void gc_root_push(Value *v) { gc_root_stack.push_back(v); }

void gc_root_pop_n(size_t n) {
  while (n--)
    gc_root_stack.pop_back();
}

static void gc_mark(Value *v) {
  if (!v || v->marked)
    return;
  v->marked = 1;

  if (v->tag == T_CONS) {
    Cons *c = (Cons *)v->payload;
    gc_mark(c->car);
    gc_mark(c->cdr);
  }
}

// Mark stored values so they don't get removed
static void gc_mark_symbol_table() {
  for (auto kv : symbol_table) {
    gc_mark(kv.second);
  }
}

// Allow gc to mark NIL as roots
static Value *NIL_singleton = nullptr;
static void gc_mark_singletons() { gc_mark(NIL_singleton); }

static void gc_mark_from_precise_roots() {
  for (Value *v : gc_root_stack)
    gc_mark(v);
}

static void gc_free_value(Value *v) {
  if (!v)
    return;
  switch (v->tag) {
  case T_CONS:
    std::free((Cons *)v->payload);
    break;
  case T_STRING:
  case T_SYMBOL:
    std::free((char *)v->payload);
    break;
  case T_NUMBER:
    std::free((double *)v->payload);
    break;
  case T_BOOL:
    std::free((int *)v->payload);
    break;
  case T_NIL:
    break;
  }
  std::free(v);
}

static void gc_sweep() {
  Value **current = &gc_head;
  while (Value *v = *current) {
    if (!v->marked) {
      *current = v->gc_next;
      gc_free_value(v);
    } else {
      v->marked = 0; // clear for next cycle
      current = &v->gc_next;
    }
  }
}

void gc_collect() {
  gc_mark_singletons();
  gc_mark_symbol_table();
  gc_mark_from_precise_roots();

  gc_sweep();

  gc_allocs_since_last = 0;
}

// LISP API
Value *mkval(int tag, void *payload) {
  Value *v = (Value *)std::malloc(sizeof(Value));
  v->tag = tag;
  v->payload = payload;
  gc_register(v);
  return v;
}

Value *make_nil() {
  static Value *NIL = nullptr;
  if (!NIL) {
    NIL = mkval(T_NIL, nullptr);
    NIL_singleton = NIL;
  }
  return NIL;
}

Value *make_bool(int b) {
  int *p = (int *)std::malloc(sizeof(int));
  *p = b;
  return mkval(T_BOOL, p);
}

Value *make_number(double x) {
  double *p = (double *)std::malloc(sizeof(double));
  *p = x;
  return mkval(T_NUMBER, p);
}

Value *make_string(const char *s) {
  char *copy = (char *)std::malloc(strlen(s) + 1);
  std::strcpy(copy, s);
  return mkval(T_STRING, (void *)copy);
}

Value *make_symbol(const char *s) {
  std::string key(s);
  auto it = symbol_table.find(key);
  if (it != symbol_table.end())
    return it->second;
  char *copy = (char *)std::malloc(key.size() + 1);
  std::strcpy(copy, key.c_str());
  Value *v = mkval(T_SYMBOL, (void *)copy);
  symbol_table.emplace(key, v);
  return v;
}

Cons *make_cons(Value *carv, Value *cdrv) {
  Cons *c = (Cons *)std::malloc(sizeof(Cons));
  c->car = carv;
  c->cdr = cdrv ? (Value *)cdrv : (Value *)make_nil();
  return c;
}

Value *cons(Value *carv, Value *cdrv) {
  Cons *c = make_cons(carv, cdrv);
  return mkval(T_CONS, (void *)c);
}

Value *car(Value *v) {
  if (!v || v->tag != T_CONS)
    return make_nil();
  Cons *c = (Cons *)v->payload;
  return c->car;
}

Value *cdr(Value *v) {
  if (!v || v->tag != T_CONS)
    return make_nil();
  Cons *c = (Cons *)v->payload;
  return c->cdr;
}

Value *is_atom(Value *v) {
  if (!v)
    return make_bool(1);
  return make_bool(v->tag != T_CONS);
}

Value *eq(Value *a, Value *b) {
  if (a->tag != b->tag)
    return make_bool(0);

  if (!a && !b) {
    return make_bool(1);
  } else if (!a || !b) {
    return make_bool(0);
  }

  switch (a->tag) {
  case T_NIL:
    return make_bool(1);
  case T_NUMBER:
    return make_bool(*(double *)a->payload == *(double *)b->payload);
  case T_STRING:
  case T_SYMBOL:
    return make_bool(std::string((char *)a->payload) ==
                     std::string((char *)b->payload));
  case T_BOOL:
    return make_bool(*(int *)a->payload == *(int *)b->payload);
  }
  return make_bool(0);
}

Value *get_add(Value *a, Value *b) {
  return make_number(*(double *)a->payload + *(double *)b->payload);
}

Value *get_sub(Value *a, Value *b) {
  return make_number(*(double *)a->payload - *(double *)b->payload);
}

Value *get_mul(Value *a, Value *b) {
  return make_number(*(double *)a->payload * *(double *)b->payload);
}

Value *get_div(Value *a, Value *b) {
  return make_number(*(double *)a->payload / *(double *)b->payload);
}

void print_value(Value *v) {
  if (!v) {
    printf("nil");
    return;
  }
  switch (v->tag) {
  case T_NIL:
    printf("nil");
    break;
  case T_NUMBER:
    printf("%g", *(double *)v->payload);
    break;
  case T_STRING:
    printf("\"%s\"", (char *)v->payload);
    break;
  case T_SYMBOL:
    printf("%s", (char *)v->payload);
    break;
  case T_BOOL:
    printf("%s", *(int *)v->payload ? "true" : "false");
    break;
  case T_CONS: {
    printf("(");
    Cons *c = (Cons *)v->payload;
    print_value(c->car);
    Value *tail = c->cdr;
    while (tail && tail->tag == T_CONS) {
      printf(" ");
      Cons *cc = (Cons *)tail->payload;
      print_value(cc->car);
      tail = cc->cdr;
    }
    if (tail && tail->tag != T_NIL) {
      printf(" . ");
      print_value(tail);
    }
    printf(")");
  } break;
  default:
    printf("<unknown>");
    break;
  }
}
}
