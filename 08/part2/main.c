#include <ctype.h>
#include <math.h>
#include <stdbool.h>
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

typedef struct digit
{
  char *segments;  // NULL-terminated string
  int n;
} digit;

digit *digit_init(char *segments, int n)
{
  digit *d = malloc(sizeof(digit));
  d->segments = malloc(strlen(segments) + 1);
  strcpy(d->segments, segments);
  d->n = n;
  return d;
}

void digit_free(digit *d)
{
  if (d == NULL) return;
  free(d->segments);
  free(d);
}

bool has_segments(digit *d, char *segments)
{
  if (strlen(d->segments) != strlen(segments)) return false;
  for (size_t i = 0; i < strlen(segments); i++) {
    bool found = false;
    for (size_t j = 0; j < strlen(d->segments); j++) {
      if (d->segments[j] == segments[i]) {
        found = true;
        break;
      }
    }
    if (!found) {
      return false;
    }
  }
  return true;
}

bool contains_chr(char *str, char chr)
{
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == chr) return true;
  }
  return false;
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

  long sum = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    entry *e = entry_init(line);
    digit **digits = calloc(10, sizeof(digit *));
    for (size_t i = 0; i < e->signal_patterns_count; i++) {
      char *signal_pattern = e->signal_patterns[i];
      switch (strlen(signal_pattern)) {
        case 2:
          digits[1] = digit_init(signal_pattern, 1);
          break;
        case 3:
          digits[7] = digit_init(signal_pattern, 7);
          break;
        case 4:
          digits[4] = digit_init(signal_pattern, 4);
          break;
        case 7:
          digits[8] = digit_init(signal_pattern, 8);
          break;
      }
    }

    // Find 0, 6 and 9
    for (size_t i = 0; i < e->signal_patterns_count; i++) {
      char *signal_pattern = e->signal_patterns[i];
      if (strlen(signal_pattern) == 6) {
        char missing_segment = '\0';
        for (size_t j = 0; j < strlen(digits[8]->segments); j++) {
          if (!contains_chr(signal_pattern, digits[8]->segments[j])) {
            missing_segment = digits[8]->segments[j];
            break;
          }
        }
        if (contains_chr(digits[1]->segments, missing_segment)) {
          digits[6] = digit_init(signal_pattern, 6);
        } else if (contains_chr(digits[4]->segments, missing_segment)) {
          digits[0] = digit_init(signal_pattern, 0);
        } else {
          digits[9] = digit_init(signal_pattern, 9);
        }
      }
    }

    // Find 2, 3 and 5
    for (size_t i = 0; i < e->signal_patterns_count; i++) {
      char *signal_pattern = e->signal_patterns[i];
      if (strlen(signal_pattern) == 5) {
        char first_missing_segment = '\0';
        char second_missing_segment = '\0';
        for (size_t j = 0; j < strlen(digits[6]->segments); j++) {
          if (!contains_chr(signal_pattern, digits[6]->segments[j])) {
            if (first_missing_segment == '\0') {
              first_missing_segment = digits[6]->segments[j];
            } else {
              second_missing_segment = digits[6]->segments[j];
            }
          }
        }
        if (second_missing_segment == '\0') {
          digits[5] = digit_init(signal_pattern, 5);
        } else if (contains_chr(digits[4]->segments, first_missing_segment) &&
                   contains_chr(digits[4]->segments, second_missing_segment)) {
          digits[2] = digit_init(signal_pattern, 2);
        } else {
          digits[3] = digit_init(signal_pattern, 3);
        }
      }
    }

    int *out_digits = malloc(sizeof(int) * e->output_digits_count);
    for (size_t i = 0; i < e->output_digits_count; i++) {
      char *output_digit_segments = e->output_digits[i];
      for (int n = 0; n < 10; n++) {
        digit *d = digits[n];
        if (has_segments(d, output_digit_segments)) {
          out_digits[i] = n;
        }
      }
    }

    for (size_t i = 0; i < e->output_digits_count; i++) {
      sum += out_digits[i] * pow(10, e->output_digits_count - i - 1);
    }

    free(out_digits);
    for (size_t i = 0; i < 10; i++) {
      digit_free(digits[i]);
    }
    free(digits);
    entry_free(e);
  }

  if (line) free(line);
  fclose(fp);

  fprintf(stdout, "%ld\n", sum);
  exit(EXIT_SUCCESS);
}
