#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

char* input_formula();           //数式入力
void resolve(char* input);       //数式分解



int main(void)
{
  char *input;
  input = input_formula();
  resolve(input);  
  return 0;
}


char* input_formula() //数式入力
{
  char *c = readline(">>>");
  return c;
}

void resolve(char* input) //数式分解
{
  char *str = (char *)calloc(10,sizeof(char));
  char **token = (char **)calloc(10,sizeof(char*));

  int i,j;
  j = 0;
  for(i = 0; input[i] != '\0'; i++){
    if(input[i] == '(' || input[i] == ')' || input[i] == ' '){
      input[i] = '\0';
      str = (char *)calloc(10,sizeof(char));  
      str = input + i + 1;
      token[j] = str;
      j++;
    }
  }
  for(i = 0; i < (j-1); i++){
    printf("%s\n",token[i]);
  }
  for(i = 0; i < j; i++){
    if(token[i] != NULL){
      printf("token[%d]:%p\n",i,token[i]);
      free(token[i]);
      token[i] = NULL;
    }
  }

  if(token != NULL){
    free(token);
    token = NULL;
  }				     

}				     
