#define _GNU_SOURCE

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cloud_line
{
  int x1;
  int x2;
  int y1;
  int y2;
};

void parse_line(struct cloud_line *cl, char *text_line)
{
  char *next_num = text_line;

  while (!isdigit(*next_num)) {
    next_num++;
  }
  cl->x1 = strtol(next_num, &next_num, 10);

  while (!isdigit(*next_num)) {
    next_num++;
  }
  cl->y1 = strtol(next_num, &next_num, 10);

  while (!isdigit(*next_num)) {
    next_num++;
  }
  cl->x2 = strtol(next_num, &next_num, 10);

  while (!isdigit(*next_num)) {
    next_num++;
  }
  cl->y2 = strtol(next_num, &next_num, 10);
}

bool is_horizontal(struct cloud_line *cl) { return cl->x1 == cl->x2; }

bool is_vertical(struct cloud_line *cl) { return cl->y1 == cl->y2; }

bool is_diagonal_top_left_bottom_right(struct cloud_line *cl)
{
  return cl->x1 <= cl->x2 && cl->y1 <= cl->y2;
}

bool is_diagonal_top_right_bottom_left(struct cloud_line *cl)
{
  return cl->x1 >= cl->x2 && cl->y1 <= cl->y2;
}

bool is_diagonal_bottom_left_top_right(struct cloud_line *cl)
{
  return cl->x1 <= cl->x2 && cl->y1 >= cl->y2;
}

bool is_diagonal_bottom_right_top_left(struct cloud_line *cl)
{
  return cl->x1 >= cl->x2 && cl->y1 >= cl->y2;
}

// Order the coordinates.
// WARNING: only applicable when the orientation is horizontal or vertical.
void order_coord(struct cloud_line *cl)
{
  if (cl->x2 < cl->x1) {
    int temp = cl->x1;
    cl->x1 = cl->x2;
    cl->x2 = temp;
  }

  if (cl->y2 < cl->y1) {
    int temp = cl->y1;
    cl->y1 = cl->y2;
    cl->y2 = temp;
  }
}

// Dynamic linked list to count the cloud points
struct point_counter
{
  int x;
  int y;
  int count;
  struct point_counter *next;
};

struct point_counter *init_point_counter(int x, int y)
{
  struct point_counter *pc = malloc(sizeof(struct point_counter));
  pc->x = x;
  pc->y = y;
  pc->count = 1;
  pc->next = NULL;
  return pc;
}

void free_point_counter(struct point_counter *first)
{
  if (first == NULL) return;
  struct point_counter *current = first;
  struct point_counter *next = current->next;
  while (next != NULL) {
    free(current);
    current = next;
    next = current->next;
  }
  free(current);
}

void add_point(struct point_counter *first, int x, int y)
{
  struct point_counter *current = first;
  while (true) {
    if (current->x == x && current->y == y) {
      current->count++;
      break;
    }
    if (current->next == NULL) {
      current->next = init_point_counter(x, y);
      break;
    }
    current = current->next;
  }
}

void add_cloud_line(struct point_counter *first, struct cloud_line *cl)
{
  if (is_horizontal(cl)) {
    order_coord(cl);
    for (int current_y = cl->y1; current_y <= cl->y2; current_y++) {
      add_point(first, cl->x1, current_y);
    }
  } else if (is_vertical(cl)) {
    order_coord(cl);
    for (int current_x = cl->x1; current_x <= cl->x2; current_x++) {
      add_point(first, current_x, cl->y1);
    }
  } else if (is_diagonal_top_left_bottom_right(cl)) {
    int current_x = cl->x1;
    int current_y = cl->y1;
    while (current_x <= cl->x2 && current_y <= cl->y2) {
      add_point(first, current_x, current_y);
      current_x++;
      current_y++;
    }
  } else if (is_diagonal_top_right_bottom_left(cl)) {
    int current_x = cl->x1;
    int current_y = cl->y1;
    while (current_x >= cl->x2 && current_y <= cl->y2) {
      add_point(first, current_x, current_y);
      current_x--;
      current_y++;
    }

  } else if (is_diagonal_bottom_left_top_right(cl)) {
    int current_x = cl->x1;
    int current_y = cl->y1;
    while (current_x <= cl->x2 && current_y >= cl->y2) {
      add_point(first, current_x, current_y);
      current_x++;
      current_y--;
    }
  } else if (is_diagonal_bottom_right_top_left(cl)) {
    int current_x = cl->x1;
    int current_y = cl->y1;
    while (current_x >= cl->x2 && current_y >= cl->y2) {
      add_point(first, current_x, current_y);
      current_x--;
      current_y--;
    }
  }
}

void display_point_counters(struct point_counter *first)
{
  struct point_counter *current = first;
  while (current != NULL) {
    fprintf(stdout, "(%d,%d) -> %d\n", current->x, current->y, current->count);
    current = current->next;
  }
}

int count_points(struct point_counter *first)
{
  int rst = 0;
  struct point_counter *current = first;
  while (current != NULL) {
    if (current->count >= 2) {
      rst++;
    }
    current = current->next;
  }
  return rst;
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

  // dummy
  struct point_counter *first = init_point_counter(-1, -1);

  while ((read = getline(&line, &len, fp)) != -1) {
    struct cloud_line *cl = malloc(sizeof(struct cloud_line));
    parse_line(cl, line);
    add_cloud_line(first, cl);
    free(cl);
  }

  fclose(fp);
  if (line) free(line);

  fprintf(stdout, "%d\n", count_points(first));
  free_point_counter(first);
  exit(EXIT_SUCCESS);
}
