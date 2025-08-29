; ModuleID = 'JIT'
source_filename = "JIT"

@0 = private unnamed_addr constant [15 x i8] c"This is a test\00", align 1
@1 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@2 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@3 = private unnamed_addr constant [2 x i8] c"c\00", align 1
@4 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@5 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@6 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@7 = private unnamed_addr constant [2 x i8] c"c\00", align 1
@8 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@9 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@10 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@11 = private unnamed_addr constant [2 x i8] c"c\00", align 1
@12 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@13 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@14 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@15 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@16 = private unnamed_addr constant [2 x i8] c"c\00", align 1
@17 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@18 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@19 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@20 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@21 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@22 = private unnamed_addr constant [4 x i8] c" | \00", align 1
@23 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@24 = private unnamed_addr constant [4 x i8] c" | \00", align 1

define double @add(double %0, double %1) {
entry:
  %addresult = fadd double %0, %1
  ret double %addresult
}

define double @sub(double %0, double %1) {
entry:
  %subresult = fsub double %0, %1
  ret double %subresult
}

define double @mul(double %0, double %1) {
entry:
  %mulresult = fmul double %0, %1
  ret double %mulresult
}

define double @div(double %0, double %1) {
entry:
  %divresult = fdiv double %0, %1
  ret double %divresult
}

declare ptr @make_nil()

declare ptr @make_bool(i32)

declare ptr @make_number(double)

declare ptr @make_string(ptr)

declare ptr @make_symbol(ptr)

declare ptr @cons(ptr, ptr)

declare ptr @car(ptr)

declare ptr @cdr(ptr)

declare ptr @is_atom(ptr)

declare ptr @eq(ptr, ptr)

declare void @print_value(ptr)

define i32 @main() {
entry:
  %strobj = call ptr @make_string(ptr @0)
  call void @print_value(ptr %strobj)
  %strobj1 = call ptr @make_string(ptr @1)
  call void @print_value(ptr %strobj1)
  %numobj = call ptr @make_number(double 3.000000e+00)
  %numobj2 = call ptr @make_number(double 5.000000e+00)
  %callresult = call double @add(ptr %numobj, ptr %numobj2)
  call void @print_value(double %callresult)
  %strobj3 = call ptr @make_string(ptr @2)
  call void @print_value(ptr %strobj3)
  %0 = call ptr @make_nil()
  %symobj = call ptr @make_symbol(ptr @3)
  %cons = call ptr @cons(ptr %symobj, ptr %0)
  %symobj4 = call ptr @make_symbol(ptr @4)
  %cons5 = call ptr @cons(ptr %symobj4, ptr %cons)
  %symobj6 = call ptr @make_symbol(ptr @5)
  %cons7 = call ptr @cons(ptr %symobj6, ptr %cons5)
  call void @print_value(ptr %cons7)
  %strobj8 = call ptr @make_string(ptr @6)
  call void @print_value(ptr %strobj8)
  %1 = call ptr @make_nil()
  %symobj9 = call ptr @make_symbol(ptr @7)
  %cons10 = call ptr @cons(ptr %symobj9, ptr %1)
  %symobj11 = call ptr @make_symbol(ptr @8)
  %cons12 = call ptr @cons(ptr %symobj11, ptr %cons10)
  %symobj13 = call ptr @make_symbol(ptr @9)
  %cons14 = call ptr @cons(ptr %symobj13, ptr %cons12)
  %callresult15 = call ptr @cdr(ptr %cons14)
  call void @print_value(ptr %callresult15)
  %strobj16 = call ptr @make_string(ptr @10)
  call void @print_value(ptr %strobj16)
  %2 = call ptr @make_nil()
  %symobj17 = call ptr @make_symbol(ptr @11)
  %cons18 = call ptr @cons(ptr %symobj17, ptr %2)
  %symobj19 = call ptr @make_symbol(ptr @12)
  %cons20 = call ptr @cons(ptr %symobj19, ptr %cons18)
  %symobj21 = call ptr @make_symbol(ptr @13)
  %cons22 = call ptr @cons(ptr %symobj21, ptr %cons20)
  %callresult23 = call ptr @car(ptr %cons22)
  call void @print_value(ptr %callresult23)
  %strobj24 = call ptr @make_string(ptr @14)
  call void @print_value(ptr %strobj24)
  %symobj25 = call ptr @make_symbol(ptr @15)
  %3 = call ptr @make_nil()
  %symobj26 = call ptr @make_symbol(ptr @16)
  %cons27 = call ptr @cons(ptr %symobj26, ptr %3)
  %symobj28 = call ptr @make_symbol(ptr @17)
  %cons29 = call ptr @cons(ptr %symobj28, ptr %cons27)
  %callresult30 = call ptr @cons(ptr %symobj25, ptr %cons29)
  call void @print_value(ptr %callresult30)
  %strobj31 = call ptr @make_string(ptr @18)
  call void @print_value(ptr %strobj31)
  %boolobj = call ptr @make_bool(i32 1)
  call void @print_value(ptr %boolobj)
  %strobj32 = call ptr @make_string(ptr @19)
  call void @print_value(ptr %strobj32)
  %boolobj33 = call ptr @make_bool(i32 0)
  call void @print_value(ptr %boolobj33)
  %strobj34 = call ptr @make_string(ptr @20)
  call void @print_value(ptr %strobj34)
  %numobj35 = call ptr @make_number(double 5.000000e+00)
  %numobj36 = call ptr @make_number(double 5.000000e+00)
  %callresult37 = call ptr @eq(ptr %numobj35, ptr %numobj36)
  call void @print_value(ptr %callresult37)
  %strobj38 = call ptr @make_string(ptr @21)
  call void @print_value(ptr %strobj38)
  %numobj39 = call ptr @make_number(double 3.000000e+00)
  %numobj40 = call ptr @make_number(double 5.000000e+00)
  %callresult41 = call ptr @eq(ptr %numobj39, ptr %numobj40)
  call void @print_value(ptr %callresult41)
  %strobj42 = call ptr @make_string(ptr @22)
  call void @print_value(ptr %strobj42)
  %symobj43 = call ptr @make_symbol(ptr @23)
  %callresult44 = call ptr @is_atom(ptr %symobj43)
  call void @print_value(ptr %callresult44)
  %strobj45 = call ptr @make_string(ptr @24)
  call void @print_value(ptr %strobj45)
  %4 = call ptr @make_nil()
  %numobj46 = call ptr @make_number(double 3.000000e+00)
  %cons47 = call ptr @cons(ptr %numobj46, ptr %4)
  %numobj48 = call ptr @make_number(double 2.000000e+00)
  %cons49 = call ptr @cons(ptr %numobj48, ptr %cons47)
  %numobj50 = call ptr @make_number(double 1.000000e+00)
  %cons51 = call ptr @cons(ptr %numobj50, ptr %cons49)
  %callresult52 = call ptr @is_atom(ptr %cons51)
  call void @print_value(ptr %callresult52)
  ret i32 0
}
