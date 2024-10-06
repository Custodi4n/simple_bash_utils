#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void catOnly(char* buffer, FILE* f);
void catB(char* buffer, FILE* f);
void print_visible_e(char c);
void catEV(char* buffer, FILE* f);
void catE(char* buffer, FILE* f);
void print_visible(char c);
void catV(char* buffer, FILE* f);
void catN(char* buffer, FILE* f);
void catS(char* buffer, FILE* f);
void catT(char* buffer, FILE* f);
void print_visible_t(char c);
void catTV(char* buffer, FILE* f);

void catOnly(char* buffer, FILE* f) {
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      putchar(buffer[i]);
    }
  }
}

void catB(char* buffer, FILE* f) {
  int i = 1;
  size_t bytesRead;
  bool newLine = true;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t j = 0; j < bytesRead; j++) {
      if (newLine && buffer[j] != '\n') {
        printf("%6d\t", i++);
        newLine = false;
      }
      putchar(buffer[j]);
      if (buffer[j] == '\n') {
        newLine = true;
      }
    }
  }
}

void print_visible_e(char c) {
  if (c >= 32 && c <= 126) {
    putchar(c);
  } else if (c == '\t') {
    putchar(c);
  } else if (c == '\n') {
    putchar('$');
    putchar(c);
  } else if (c < 32) {
    putchar('^');
    putchar(c + 64);
  } else if (c == 127) {
    putchar('^');
    putchar('?');
  } else {
    printf("M-");
    print_visible_e(c - 128);
  }
}

void catEV(char* buffer, FILE* f) {
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      print_visible_e(buffer[i]);
    }
  }
}

void catE(char* buffer, FILE* f) {
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      if (buffer[i] == '\n') {
        putchar('$');
      }
      putchar(buffer[i]);
    }
  }
}

void print_visible(char c) {
  if (c >= 32 && c <= 126) {
    putchar(c);
  } else if (c == '\t') {
    putchar(c);
  } else if (c == '\n') {
    putchar(c);
  } else if (c < 32) {
    putchar('^');
    putchar(c + 64);
  } else if (c == 127) {
    putchar('^');
    putchar('?');
  } else {
    printf("M-");
    print_visible(c - 128);
  }
}

void catV(char* buffer, FILE* f) {
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      print_visible(buffer[i]);
    }
  }
}

void catN(char* buffer, FILE* f) {
  int lineNumber = 1;
  size_t bytesRead;
  bool newLine = true;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      if (newLine) {
        printf("%6d\t", lineNumber++);
        newLine = false;
      }
      if (buffer[i] == '\n') {
        newLine = true;
      }
      putchar(buffer[i]);
    }
  }
}

void catS(char* buffer, FILE* f) {
  size_t bytesRead;
  bool last_line_empty = false;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      if (buffer[i] == '\n' && buffer[i + 1] == '\n') {
        if (!last_line_empty) {
          putchar(buffer[i]);
          last_line_empty = true;
        }
        if (last_line_empty) {
          continue;
        }

      } else {
        putchar(buffer[i]);
        last_line_empty = false;
      }
    }
  }
}

void catT(char* buffer, FILE* f) {
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      if (buffer[i] == '\t') {
        printf("^I");
      } else {
        putchar(buffer[i]);
      }
    }
  }
}

void print_visible_t(char c) {
  if (c >= 32 && c <= 126) {
    putchar(c);
  } else if (c == '\t') {
    printf("^I");
  } else if (c == '\n') {
    putchar(c);
  } else if (c < 32) {
    putchar('^');
    putchar(c + 64);
  } else if (c == 127) {
    putchar('^');
    putchar('?');
  } else {
    printf("M-");
    print_visible_t(c - 128);
  }
}

void catTV(char* buffer, FILE* f) {
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, 1024, f)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      print_visible_t(buffer[i]);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3 && argc != 2) {
    printf("Usage: %s <flag> <filename>\n", argv[0]);
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  char* flag = argv[1];
  char* filename = argv[2];
  char* filename_only = argv[1];
  char buffer[1024];
  if (argc == 2) {
    FILE* file = fopen(filename_only, "r");
    if (file == NULL) {
      perror("Error opening file");
      return 1;
    } else {
      catOnly(buffer, file);
      fclose(file);
      return 0;
    }
  } else {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
      perror("Error opening file");
      return 1;
    }
    if (strcmp(flag, "-b") == 0) {
      catB(buffer, file);
    } else if (strcmp(flag, "-E") == 0) {
      catE(buffer, file);
    } else if (strcmp(flag, "-e") == 0) {
      catEV(buffer, file);
    } else if (strcmp(flag, "-v") == 0) {
      catV(buffer, file);
    } else if (strcmp(flag, "-n") == 0) {
      catN(buffer, file);
    } else if (strcmp(flag, "-s") == 0) {
      catS(buffer, file);
    } else if (strcmp(flag, "-T") == 0) {
      catT(buffer, file);
    } else if (strcmp(flag, "-t") == 0) {
      catTV(buffer, file);
    } else {
      printf("Invalid flag\n");
      fclose(file);
      return 1;
    }
    fclose(file);
  }
  return 0;
}
