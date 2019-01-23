// BF code generator
// Input: BF source
// Output: Translated C source.

#include <cstdio>

constexpr char kHeader[] = R"(
#include <stdio.h>

int main(int argc, char** argv) {
  char data[30000] = {0};
  char* p = (char*)&data;
)";

constexpr char kFooter[] = R"(
  return 0;
}
)";

#define P(s) printf("%s\n", s)

int main(int argc, char** argv) {
  FILE* f = NULL;
  if (argc == 1) {
    f = stdin;
  } else if (argc == 2) {
    f = fopen(argv[1], "r");
  } else {
    fprintf(stderr, "usage: bfgen [file] (or stdin)\n");
    return 1;
  }

  P(kHeader);
  int c;
  while ((c = getc(f)) != EOF) {
    switch ((char)c) {
      case '>': P("++p;"); break;
      case '<': P("--p;"); break;
      case '+': P("++*p;"); break;
      case '-': P("--*p;"); break;
      case '.': P("putchar(*p);"); break;
      case ',': P("*p = getchar();"); break;
      case '[': P("while (*p) {"); break;
      case ']': P("}"); break;
      default: break; // ignore.
    }
  }
  P(kFooter);
  if (f != stdin) {
    fclose(f);
  }
  return 0;
}
