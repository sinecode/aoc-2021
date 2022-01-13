#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grid_size
{
  size_t rows;
  size_t cols;
} grid_size;

void compute_grid_size(const char *input_file, grid_size *gs)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  fp = fopen(input_file, "r");
  size_t max_x = 0;
  size_t max_y = 0;
  size_t y = 0;
  while (getline(&line, &len, fp) != -1) {
    if (max_x == 0) {  // Assuming all rows have equal length
      size_t x = 0;
      while (isdigit(line[x]) && x < len) {
        x++;
      }
      max_x = x;
    }
    y++;
  }
  max_y = y;
  if (line) free(line);
  fclose(fp);
  gs->rows = max_y;
  gs->cols = max_x;
}

void parse_grid(char *input_file, int **grid)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  fp = fopen(input_file, "r");
  size_t i = 0;
  while (getline(&line, &len, fp) != -1) {
    size_t j = 0;
    while (isdigit(line[j]) && j < len) {
      grid[i][j] = line[j] - '0';
      j++;
    }
    i++;
  }
  if (line) free(line);
  fclose(fp);
}

void display_grid(int **grid, grid_size *gs)
{
  for (size_t i = 0; i < gs->rows; i++) {
    for (size_t j = 0; j < gs->rows; j++) {
      fprintf(stdout, "%d ", grid[i][j]);
    }
    fprintf(stdout, "\n");
  }
}

void display_visited(bool **visited, grid_size *gs)
{
  for (size_t i = 0; i < gs->rows; i++) {
    for (size_t j = 0; j < gs->rows; j++) {
      fprintf(stdout, "%d ", visited[i][j]);
    }
    fprintf(stdout, "\n");
  }
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "Please provide the input file as argument\n");
    exit(EXIT_FAILURE);
  }

  grid_size *gs = malloc(sizeof(grid_size));
  compute_grid_size(argv[1], gs);
  int **grid = malloc(sizeof(int *) * gs->rows);
  for (size_t i = 0; i < gs->rows; i++) {
    grid[i] = malloc(sizeof(int) * gs->cols);
  }

  bool **visited = malloc(sizeof(bool *) * gs->rows);
  for (size_t i = 0; i < gs->rows; i++) {
    visited[i] = malloc(sizeof(bool) * gs->cols);
  }
  for (size_t i = 0; i < gs->rows; i++) {
    for (size_t j = 0; j < gs->cols; j++) {
      visited[i][j] = false;
    }
  }

  parse_grid(argv[1], grid);
  display_grid(grid, gs);

  size_t i = 0;
  size_t j = 0;
  size_t i_dest = gs->rows;
  size_t j_dest = gs->cols;
  int path_weight = 0;
  visited[0][0] = true;
  while (i != i_dest && j != j_dest) {
    int min_neighbor = INT_MAX;
    size_t new_i = -1;
    size_t new_j = -1;
    if (i != 0 && !visited[i - 1][j]) {
      if (grid[i - 1][j] < min_neighbor) {
        min_neighbor = grid[i - 1][j];
        new_i = i - 1;
        new_j = j;
      }
    }
    if (j != 0 && !visited[i][j - 1]) {
      if (grid[i][j - 1] < min_neighbor) {
        min_neighbor = grid[i][j - 1];
        new_i = i;
        new_j = j - 1;
      }
    }
    if (i + 1 < gs->rows && !visited[i + 1][j]) {
      if (grid[i + 1][j] < min_neighbor) {
        min_neighbor = grid[i + 1][j];
        new_i = i + 1;
        new_j = j;
      }
    }
    if (j + 1 < gs->cols && !visited[i][j + 1]) {
      if (grid[i][j + 1] < min_neighbor) {
        min_neighbor = grid[i][j + 1];
        new_i = i;
        new_j = j + 1;
      }
    }
    visited[new_i][new_j] = true;
    path_weight += grid[new_i][new_j];
    i = new_i;
    j = new_j;
  }

  display_visited(visited, gs);

  for (size_t i = 0; i < gs->rows; i++) {
    free(visited[i]);
  }
  free(visited);
  for (size_t i = 0; i < gs->rows; i++) {
    free(grid[i]);
  }
  free(grid);
  free(gs);

  fprintf(stdout, "%d\n", path_weight);

  exit(EXIT_SUCCESS);
}
