#include "dir.h"
#include "explorer.h"
#include "file.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>
/* Add a file to the directory. */
static bool dir_add_sub(struct directory *dirnode, struct node *sub){
  /* Initialization */
  struct node **new_subordinates = NULL;
  /* Check for null pointer */
  if (!dirnode || !sub) {
    return false;
  }
  /* Check if the name is already used */
  if (dir_find_node(dirnode, sub->name)) {
    return false;
  }
  /* Check if the capacity is enough */
  if (dirnode->size >= dirnode->capacity) {
    /* Enlarge the capacity */
    new_subordinates = calloc(2 * dirnode->capacity, sizeof(struct node));
    /* Copy the data */
    memcpy(new_subordinates, dirnode->subordinates,dirnode->capacity * sizeof(struct node));
    /* Release the old memory */
    free(dirnode->subordinates);
    /* Update the pointer */
    dirnode->subordinates = new_subordinates;
    /* Update the capacity */
    dirnode->capacity *= 2;
  }
  /* Add the file */
  dirnode->subordinates[dirnode->size] = sub;
  dirnode->size++;
  return true;
}

struct directory *dir_new(char *name) {
  /* Initialization */
  struct directory *dir = NULL;
  /* Check for null pointer */
  if (!name) {
    return NULL;
  }
  /* Allocate memory */
  dir = calloc(1, sizeof(struct directory));
  dir->capacity = DEFAULT_DIR_SIZE;
  /* Allocate memory for subordinates */
  dir->subordinates = calloc(dir->capacity, sizeof(struct node));
  dir->parent = NULL;
  /* Create base node */
  dir->base = node_new(true, name, dir);
  return dir;
}
#include <stdio.h>
void dir_release(struct directory *dir) {
  /* Initialization */
  int i = 0;
  if (!dir) {
    return;
  }
  /* Release all the subordiniates */
  for (i = 0; i < dir->size; i++) {
    node_release(dir->subordinates[i]);
  }
  /* Release the resources */
  /* Check if base has already been released.*/
  if (dir->base) {
    dir->base->inner.dir = NULL;
    node_release(dir->base);
  }
  /* Release data and self. */
  free(dir->subordinates);
  free(dir);
}

struct node *dir_find_node(const struct directory *dir, const char *name) {
  /* Initialization */
  int i = 0;
  /* Check for null pointer */
  if (!dir || !name) {
    return NULL;
  }
  /* Find the node */
  for (i = 0; i < dir->size; i++) {
    /* Check if the name is the same */
    if (strcmp(dir->subordinates[i]->name, name) == 0) {
      return dir->subordinates[i];
    }
  }
  return NULL;
}

bool dir_add_file(struct directory *dir, int type, char *name) {
  /* Initialization */
  struct file *file = NULL;
  struct node *node = NULL;
  /* Check for null pointer */
  if (!dir || !name) {
    return false;
  }
  /* Check if the name is already used */
  if (dir_find_node(dir, name)) {
    return false;
  }
  /* Create the file */
  file = file_new(type, name);
  if (!file) {
    return false;
  }
  /* Create the node */
  node = node_new(false, name, file);
  if (!node) {
    file_release(file);
    return false;
  }
  /* Add the node */
  if (!dir_add_sub(dir, node)) {
    node_release(node);
    return false;
  }
  return true;
}

bool dir_add_subdir(struct directory *dir, char *name) {
  /* Initialization */
  struct directory *sub = NULL;
  struct node *node = NULL;
  /* Check for null pointer */
  if (!dir || !name) {
    return false;
  }
  /* Check if the name is already used */
  if (dir_find_node(dir, name)) {
    return false;
  }
  /* Create the directory */
  sub = dir_new(name);
  if (!sub) {
    return false;
  }
  /* Create the node */
  node = node_new(true, name, sub);
  if (!node) {
    dir_release(sub);
    return false;
  }
  /* Add the node */
  if (!dir_add_sub(dir, node)) {
    node_release(node);
    return false;
  }
  /* Set the parent */
  sub->parent = dir;
  return true;
}

bool dir_delete(struct directory *dir, const char *name) {
  /* Initialization */
  int i = 0;
  /* Check for null pointer */
  if (!dir || !name) {
    return false;
  }
  /* Find the node */
  for (i = 0; i < dir->size; i++) {
    if (strcmp(dir->subordinates[i]->name, name) == 0) {
      /* Release the node */
      node_release(dir->subordinates[i]);
      /* Move the last node to the position */
      dir->subordinates[i] = dir->subordinates[dir->size - 1];
      /* Decrease the size */
      dir->size--;
      return true;
    }
  }
  return false;
}

