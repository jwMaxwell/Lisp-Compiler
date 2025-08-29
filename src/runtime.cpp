#include "runtime.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, Value *> symbol_table;

extern "C" {
Value *mkval(int tag, void *payload) {
  Value *v = (Value *)std::malloc(sizeof(Value));
  v->tag = tag;
  v->payload = payload;
  return v;
}

Value *make_nil() {
  static Value *NIL = nullptr;
  if (!NIL) {
    NIL = mkval(T_NIL, nullptr);
  }
  return NIL;
}

Value *make_bool(int b) { return mkval(T_BOOL, (void *)(intptr_t)b); }

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

int is_atom(Value *v) {
  if (!v)
    return 1;
  return v->tag != T_CONS;
}

int eq(Value *a, Value *b) { return a == b; }

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
    printf("%s", (v->payload ? "true" : "false"));
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
