// BF interpreter

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::string read_file(const char* filename) {
  std::ifstream t(filename);
  std::stringstream buf;
  buf << t.rdbuf();
  return buf.str();
}

void interpret(const char* filename) {
  std::string prog = read_file(filename);
  char data[30000] = {0};
  // Stack of matching opening brackets, most recent last.
  std::vector<int> opening_brackets;
  int di = 0;
  int pi = 0;
  // When skipping forward through a [] block, counts the level of nesting.
  int skip_level = 0;
  for (; pi < prog.size(); pi++) {
#if 0
    fprintf(stderr, "f: %s, pi: %d, prog: %c, "
            "di: %d, data: %d, skip:%d, ob:%d\n",
            filename, pi, prog[pi], di, data[di],
            skip_level, opening_brackets.size());
#endif
    const char c = prog[pi];

    if (skip_level > 0) {
      switch (c) {
        case '[': skip_level++; break;
        case ']': skip_level--; break;
        default: break;
      }
      continue;
    }

    switch (c) {
      case '>': di++; break;
      case '<': di--; break;
      case '+': data[di]++; break;
      case '-': data[di]--; break;
      case '.': putchar(data[di]); break;
      case ',': data[di] = getchar(); break;
      case '[': {
        if (data[di] == 0) {
          skip_level++;
        } else {
          opening_brackets.push_back(pi);
        }
        break;
      }
      case ']': {
        assert(skip_level == 0);
        // This should be non-empty, but bad user program could have
        // ] without matching [.
        // TODO: Handle more gracefully.
        assert(!opening_brackets.empty());
        if (data[di] != 0) {
          // For loop will increment us to the next instruction.
          pi = opening_brackets.back();
        } else {
          opening_brackets.pop_back();
        }
        break;
      }
      default: break; // ignore.
    }
  }
  assert(skip_level == 0);
  assert(opening_brackets.empty());
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: bfi [file]\n");
    return 1;
  }
  interpret(argv[1]);
  return 0;
}
