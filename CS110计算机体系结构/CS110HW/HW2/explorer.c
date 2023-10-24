#include "explorer.h"
#include "dir.h"
#include "file.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>
/*Allocate a new piece of memory and create an explorer object on
 * it.
 * Also create an empty root directory and set current working directory to
 * root.*/
struct explorer *explorer_new(void) {
  /* Allocate memory */
  struct explorer *exp = calloc(1, sizeof(struct explorer));
  /* Initialization */
  exp->cwd = exp->root = dir_new("root");
  return exp;
}

void explorer_release(struct explorer *exp) {
  /*Check for null pointer*/ 
  if (!exp) {
    return;
  }
  /*Relase the resources*/ 
  dir_release(exp->root);
  free(exp);
}

bool explorer_read(const struct explorer *exp, const char *name, int offset,
                   int bytes, char *buf) {
  /*Initialization*/ 
  struct node *sub = NULL;
  /*Check for null pointer*/ 
  if (!exp) {
    return false;
  }
  /*Check if the file exists*/ 
  sub = dir_find_node(exp->cwd, name);
  if (!sub || sub->is_dir) {
    return false;
  }
  /*Read the file*/ 
  return file_read(sub->inner.file, offset, bytes, buf);
}

bool explorer_write(struct explorer *exp, const char *name, int offset,
                    int bytes, const char *buf) {
  /*Initialization*/ 
  struct node *sub = NULL;
  /*Check for null pointer*/ 
  if (!exp) {
    return false;
  }
  /*Check if the file exisits*/ 
  sub = dir_find_node(exp->cwd, name);
  if (!sub || sub->is_dir) {
    return false;
  }
  /*Write the file*/ 
  return file_write(sub->inner.file, offset, bytes, buf);
}

bool explorer_create(struct explorer *exp, char *name, int type) {
  /*Check for valid arguments*/ 
  if (!exp || dir_find_node(exp->cwd, name)) {
    return false;
  }
  /*Add file*/ 
  return dir_add_file(exp->cwd, type, name);
}

bool explorer_mkdir(struct explorer *exp, char *name) {
  /*Check for valid arguments*/ 
  if (!exp || dir_find_node(exp->cwd, name)) {
    return false;
  }
  /*Add subdir*/ 
  return dir_add_subdir(exp->cwd, name);
}

bool explorer_delete(struct explorer *exp, const char *name) {
  /* Check for null pointer*/
  if (!exp) {
    return false;
  }
  /*Delete the node*/ 
  return dir_delete(exp->cwd, name);
}

bool explorer_cdpar(struct explorer *exp) {
  if (!exp || exp->cwd == exp->root) {
    return false;
  }
  exp->cwd = exp->cwd->parent;
  return true;
}
/*Change current working directory to a subdir whose name matches
 * with `name`*/
bool explorer_chdir(struct explorer *exp, const char *name) {
  struct node *sub = NULL;
  if (!exp || !name) {
    return false;
  }
  /*Check if the subdir exists*/ 
  sub = dir_find_node(exp->cwd, name);
  if (!sub || !sub->is_dir) {
    return false;
  }
  /*Change the current working directory*/
  exp->cwd = sub->inner.dir;
  return true;
}
/*Register a new callback function for filetype number
 * `filetype`*/
bool explorer_support_filetype(struct explorer *exp, open_func callback,int filetype) {
  /*Check for valid arguments*/
  if (!exp || callback == NULL || filetype < 0 || filetype >= MAX_FT_NO) {
    return false;
  }
  /*Check if the callback is registered*/
  if (exp->ft_open[filetype] != NULL) {
    return false;
  }
  /*Register the callback */ 
  exp->ft_open[filetype] = callback;
  return true;
}
/*Register a new callback function for filetype number
 * `filetype`*/
bool explorer_open(const struct explorer *exp, const char *name) {
  struct node *sub = NULL;
  struct file *file = NULL;
  /*Check for valid arguments*/
  if (!exp || name == NULL) {
    return false;
  }
  /*Check if the subdir exists*/ 
  sub = dir_find_node(exp->cwd, name);
  if (!sub || sub->is_dir) {
    return false;
  }
  /*Open the file*/ 
  file = sub->inner.file;
  if (!exp->ft_open[file->type]) {
    return false;
  }
  /*Open the file*/
  exp->ft_open[file->type](file);
  return true;
}
/*Helper function for explorer_search_recursive*/ 
static void search(struct node *node, char **path, const char *name,
                   find_func callback) {
  struct directory *dir = NODE_DIR(node);
  int i = 0, pathlen = strlen(*path);
  /*realloc the file */ 
  *path = realloc(*path, pathlen + strlen(node->name) + 2);
  strcat(*path, "/");
  strcat(*path, node->name);
  /*Check if the node is a directory*/ 
  if (node->is_dir) {
    for (i = 0; i < dir->size; i++) {
      search(dir->subordinates[i], path, name, callback);
    }
  } else if (strncmp(node->name, name, MAX_NAME_LEN) == 0) {
    callback(*path, NODE_FILE(node));
  }
  /* Remove the current node from the path*/
  *path = realloc(*path, pathlen + 1);
  (*path)[pathlen] = '\0';
}
/*Recursively search for files with name equal to `name` in all
 * the descendants of the current working directory.*/
void explorer_search_recursive(struct explorer *exp, const char *name,
                               find_func callback) {
  char *path = NULL;
  if (!exp || !name || !callback) {
    return;
  }
  /*Search for the file*/ 
  path = calloc(1, 1);
  search(exp->cwd->base, &path, name, callback);
  free(path);
}

bool explorer_contain(struct explorer *exp, const char *name) {
  return exp ? dir_find_node(exp->cwd, name) != NULL : false;
}
