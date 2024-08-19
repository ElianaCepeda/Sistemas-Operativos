#include <stdio.h>
#include <string.h>

int main() {
  char *p;
  char*q = NULL;
  printf("Dirección de p = %s\n",p);

  strcpy(p,"Hello, I'm the best in Operating Systems");
  printf("%s\n",p);
  printf("About to copy \"Goodbye\" to q\n");
  strcpy(q, "Goodbye");
  printf("String copied\n");
  printf("%s\n",q);
  return 0;
}