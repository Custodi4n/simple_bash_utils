#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeQuotes(char* pattern, char* clear_pattern);
int grepMatch(char* clear_pattern, char* buffer, int i_flag);
void grepN(char* buffer, int line_number, int total_files, char* filename);
void processFile(FILE* file, char* clear_pattern, int e_flag, int i_flag,
                 int v_flag, int c_flag, int l_flag, int n_flag, char* filename,
                 int total_files);

int main(int argc, char* argv[]) {
  int opt, i_flag = 0, v_flag = 0, c_flag = 0, l_flag = 0, n_flag = 0,
           e_flag = 0;
  char* pattern = NULL;
  int files_opened = 0;
  while ((opt = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (opt) {
      case 'e':
        e_flag = 1;
        pattern = optarg;
        break;
      case 'i':
        i_flag = 1;
        break;
      case 'v':
        v_flag = 1;
        break;
      case 'c':
        c_flag = 1;
        break;
      case 'l':
        l_flag = 1;
        break;
      case 'n':
        n_flag = 1;
        break;
      default:
        fprintf(stderr,
                "Usage: %s [-e pattern] [-ivcln] <pattern> <filename1> "
                "[<filename2> ...]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (!pattern && optind < argc) {
    pattern = argv[optind++];
  }

  if (!pattern || optind >= argc) {
    fprintf(stderr, "Expected pattern and filename(s) after options\n");
    exit(EXIT_FAILURE);
  }

  char clear_pattern[256] = {0};
  removeQuotes(pattern, clear_pattern);
  int total_files = argc - optind;

  for (int i = optind; i < argc; i++) {
    char* filename = argv[i];
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
      perror("Error opening file");
      continue;
    }
    files_opened++;
    processFile(file, clear_pattern, e_flag, i_flag, v_flag, c_flag, l_flag,
                n_flag, filename, total_files);
    fclose(file);
  }

  return 0;
}

void processFile(FILE* file, char* clear_pattern, int e_flag, int i_flag,
                 int v_flag, int c_flag, int l_flag, int n_flag, char* filename,
                 int total_files) {
  char buffer[1024];
  int count = 0, line_number = 1;
  while (fgets(buffer, sizeof(buffer), file)) {
    int match = grepMatch(clear_pattern, buffer, i_flag);
    if (v_flag) match = !match;
    if (match) {
      if (c_flag) {
        count++;
      } else if (l_flag) {
        printf("%s\n", filename);
        break;
      } else {
        if (n_flag) {
          grepN(buffer, line_number, total_files, filename);
        } else if (v_flag || i_flag || e_flag) {
          if (total_files > 1) {
            printf("%s:%s", filename, buffer);
          } else {
            printf("%s", buffer);
          }
        }
      }
    }
    line_number++;
  }

  if (c_flag) {
    if (total_files > 1) {
      printf("%s:%d\n", filename, count);
    } else {
      printf("%d\n", count);
    }
  }
  if (v_flag && !i_flag && !c_flag && !l_flag && !n_flag) {
    if (strcmp(clear_pattern, "s") != 0 && strcmp(clear_pattern, "l") != 0) {
      printf("\n");
    }
  }
  if (v_flag && i_flag) {
    if (strcmp(clear_pattern, "s") != 0 && strcmp(clear_pattern, "l") != 0) {
      printf("\n");
    }
  }
  if (n_flag && !i_flag && !c_flag && !l_flag && !v_flag && !e_flag) {
    if (strcmp(clear_pattern, "s") == 0 || strcmp(clear_pattern, "l") == 0) {
      printf("\n");
    }
  }
  if (n_flag && v_flag) {
    if (strcmp(clear_pattern, "s") != 0 && strcmp(clear_pattern, "l") != 0) {
      printf("\n");
    }
  }
  if (!n_flag && !i_flag && !c_flag && !l_flag && !n_flag && e_flag) {
    if (strcmp(clear_pattern, "s") == 0 || strcmp(clear_pattern, "l") == 0) {
      printf("\n");
    }
  }
  if (n_flag && i_flag) {
    if (strcmp(clear_pattern, "s") == 0 || strcmp(clear_pattern, "l") == 0) {
      printf("\n");
    }
  }
  if (!n_flag && i_flag && !c_flag && !l_flag && !n_flag && !e_flag &&
      !v_flag) {
    if (strcmp(clear_pattern, "s") == 0 || strcmp(clear_pattern, "l") == 0) {
      printf("\n");
    }
  }
}

void removeQuotes(char* pattern, char* clear_pattern) {
  char* ptr = clear_pattern;
  while (*pattern) {
    if (*pattern != '\"') {
      *ptr++ = *pattern;
    }
    pattern++;
  }
  *ptr = '\0';
}

int grepMatch(char* clear_pattern, char* buffer, int i_flag) {
  regex_t regex;
  int reti;
  int flags = REG_EXTENDED;
  if (i_flag) {
    flags |= REG_ICASE;
  }

  reti = regcomp(&regex, clear_pattern, flags);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
  }
  reti = regexec(&regex, buffer, 0, NULL, 0);
  regfree(&regex);
  return !reti;
}

void grepN(char* buffer, int line_number, int total_files, char* filename) {
  if (total_files > 1) {
    printf("%s:%d:%s", filename, line_number, buffer);
  } else {
    printf("%d:%s", line_number, buffer);
  }
}
