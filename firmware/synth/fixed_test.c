#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "fixed.h"

#define APPROX_EQ(a, b) \
  (fabs(a - b) < 0.0001)

#define ASSERT_EQ_FLOAT(x, f) \
  if (!APPROX_EQ(Fixed1616AsFloat(x), f)) { \
    printf("Error: Expected %f, but got %f\n", f, Fixed1616AsFloat(x));\
  }

void PrintParts(Fixed1616 f) {
  printf("   %d:%d\n", f.full >> 16, f.full & 0xFFFF);
  printf("   %f\n", Fixed1616AsFloat(&f));
}

int main() {
  Fixed1616 f;

  for (int i = 0; i < 1000; ++i) {
    f = Fixed1616FromFloat(0.1 * i);
    ASSERT_EQ_FLOAT(&f, 0.1 * i);

    f = Fixed1616FromFloat(i);
    ASSERT_EQ_FLOAT(&f, (float) i);
  }

  printf("--------\n");
  printf("| 1.5\n");
  f = Fixed1616FromFloat(1.5);
  PrintParts(f);

  printf("| Plus 10\n");
  Fixed1616AddInt(&f, 10);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 11.5);

  printf("| Times 2\n");
  Fixed1616MulInt(&f, 2);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 23.0);

  printf("--------\n");
  printf("| 1.5\n");
  f = Fixed1616FromFloat(1.5);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 1.5);

  printf("| Times 3\n");
  Fixed1616MulInt(&f, 3);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 4.5);

  printf("--------\n");
  printf("| 0.5\n");
  f = Fixed1616FromFloat(0.5);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 0.5);

  printf("| Times 0.5\n");
  Fixed1616Mul(&f, Fixed1616FromFloat(0.5));
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 0.25);

  printf("--------\n");
  printf("| 10\n");
  f = Fixed1616FromInt(10);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 10.0);

  printf("| Divided by 4\n");
  Fixed1616DivInt(&f, 4);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 2.5);

  printf("--------\n");
  printf("| 10\n");
  f = Fixed1616FromInt(10);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 10.0);

  printf("| Divided by 2.5\n");
  Fixed1616Div(&f, Fixed1616FromFloat(2.5));
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 4.0);

  printf("--------\n");
  printf("| 1.5\n");
  f = Fixed1616FromFloat(1.5);
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 1.5);

  printf("| Times 2.0\n");
  Fixed1616Mul(&f, Fixed1616FromFloat(2));
  PrintParts(f);
  ASSERT_EQ_FLOAT(&f, 3.0);
  return 0;
}
