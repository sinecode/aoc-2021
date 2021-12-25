#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct entry
{
  char **signal_patterns;
  size_t signal_patterns_count;
  char **output_digits;
  size_t output_digits_count;
} entry;

entry *entry_init(char *line)
{
  entry *e = malloc(sizeof(entry));

  e->signal_patterns_count = 10;
  e->signal_patterns = malloc(sizeof(char *) * e->signal_patterns_count);
  char *start_substr = line;
  for (size_t i = 0; i < e->signal_patterns_count; i++) {
    char *end_substr = start_substr;
    while (isalpha(*end_substr)) {
      end_substr++;
    }
    size_t substr_size = end_substr - start_substr;
    e->signal_patterns[i] = malloc(sizeof(char) * substr_size + 1);
    strncpy(e->signal_patterns[i], start_substr, substr_size);
    // strncpy doesn't set NULL terminator
    e->signal_patterns[i][substr_size] = '\0';
    // Skip space
    start_substr = end_substr + 1;
  }

  while (!isalpha(*start_substr)) {
    start_substr++;
  }

  e->output_digits_count = 4;
  e->output_digits = malloc(sizeof(char *) * e->output_digits_count);
  for (size_t i = 0; i < e->output_digits_count; i++) {
    char *end_substr = start_substr;
    while (isalpha(*end_substr)) {
      end_substr++;
    }
    size_t substr_size = end_substr - start_substr;
    e->output_digits[i] = malloc(sizeof(char) * substr_size + 1);
    strncpy(e->output_digits[i], start_substr, substr_size);
    // strncpy doesn't set NULL terminator
    e->output_digits[i][substr_size] = '\0';
    // Skip space
    start_substr = end_substr + 1;
  }

  return e;
}

void entry_free(entry *e)
{
  if (e == NULL) return;
  free(e->signal_patterns);
  free(e->output_digits);
  free(e);
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "Please provide the input file as argument\n");
    exit(EXIT_FAILURE);
  }

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(argv[1], "r");
  int easy_digits_count = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    entry *e = entry_init(line);
    for (size_t i = 0; i < e->output_digits_count; i++) {
      size_t len = strlen(e->output_digits[i]);
      if (len == 2 || len == 3 || len == 4 || len == 7) {
        easy_digits_count++;
      }
    }
    entry_free(e);
  }
  if (line) free(line);
  fclose(fp);

  fprintf(stdout, "%d\n", easy_digits_count);
  exit(EXIT_SUCCESS);
}
