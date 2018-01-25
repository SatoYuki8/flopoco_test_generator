#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <gmp.h>

typedef struct{
  char *data[2];
}two_param;

two_param split(char *str){
  char *cp;
  char *tmp;
  two_param data;

  cp = str;
  tmp = strtok(cp, " ");

  int i=0;

  while(tmp != NULL){
    data.data[i++] = tmp;
    tmp = strtok(NULL, " ");
  }

  return data;
}

int InputArgCheck(int ac, char *av[]){ 
  if(ac == 1){
    printf("Input File , Exp Size and Frac Size.\n");
    return 1;
  }

  if(ac > 4){
    printf("Too many argment.\n");
    return 1;
  }
    
  return 0;
}

void str_front_push(char *str, char c){
  if (c == '\0') return;
  for(;;){
    char t = *str;
    if((*str++ = c) == '\0') return;
    c = t;
  }
}

void zero_padding(char *str, int n){
  int i;
  for(i=0;i<n;i++){
    str[i] = '0';
  }  
  return;
}

void one_padding(char *str, int n){
  int i;
  for(i=0;i<n;i++){
    str[i] = '1';
  }  
  return;
}


int main(int ac, char *av[]){  
  FILE *fp;

  if(InputArgCheck(ac, av)) return 0;

  if ((fp = fopen(av[1], "r")) == NULL){
    printf("FILE is not find.\n");
    return 1;
  }
  
  int exp = atoi(av[2]);
  int frac = atoi(av[3]);
  int bit_width = (1+exp+frac)*2;

  two_param data;
  char str[64];
  char *a, *b;

  char *exp_a, *exp_b;
  char *frac_a, *frac_b;

  char *in_a;
  char *in_b;

  char *zero_frac;
  char *zero_exp;
  char *inf;

  a = (char *)calloc(bit_width*2, sizeof(char));
  b = (char *)calloc(bit_width*2, sizeof(char));
  exp_a = (char *)calloc(exp+1, sizeof(char));
  exp_b = (char *)calloc(exp+1, sizeof(char));
  frac_a = (char *)calloc(frac+1, sizeof(char));
  frac_b = (char *)calloc(frac+1, sizeof(char));
  in_a = (char *)calloc(bit_width*2, sizeof(char));
  in_b = (char *)calloc(bit_width*2, sizeof(char));
  zero_frac = (char *)calloc(frac+1, sizeof(char));
  zero_exp = (char *)calloc(exp+1, sizeof(char));
  inf = (char *)calloc(exp+1, sizeof(char));
  
  zero_padding(zero_exp, exp);
  zero_padding(zero_frac, frac);
  one_padding(inf, exp);

  while (fgets(str, 64, fp) != NULL){
    data = split(str);

    strcpy(a, data.data[0]);
    strcpy(b, data.data[1]);

    strncpy(exp_a, a+1, exp);
    strncpy(exp_b, b+1, exp);
    strncpy(frac_a, a+1+exp, frac);
    strncpy(frac_b, b+1+exp, frac);    
    
    if(!strncmp(exp_a, zero_exp, bit_width)){
      if(!strncmp(frac_a, zero_frac, bit_width)){
	str_front_push(a, '0');
	str_front_push(a, '0');
	strcpy(in_a, a);
      }
      else{
	str_front_push(a, '1');
	str_front_push(a, '0');
	strcpy(in_a, a);
      }
    }else if(!strncmp(exp_a, inf, bit_width)){
      if(!strncmp(frac_a, zero_frac, bit_width)){
	str_front_push(a, '0');
	str_front_push(a, '1');
	strcpy(in_a, a);
      }
      else{
	str_front_push(a, '1');
	str_front_push(a, '1');
	strcpy(in_a, a);
      }
    }else if(a[1] == NULL){
      strcpy(in_a, a);      
    }else{
	str_front_push(a, '1');
	str_front_push(a, '0');
	strcpy(in_a, a);
    }

    if(!strncmp(exp_b, zero_exp, bit_width)){
      if(!strncmp(frac_b, zero_frac, bit_width)){
	str_front_push(b, '0');
	str_front_push(b, '0');
	strcpy(in_b, b);
      }
      else{
	str_front_push(b, '1');
	str_front_push(b, '0');
	strcpy(in_b, b);
      }
    }else if(!strncmp(exp_b, inf, bit_width)){
      if(!strncmp(frac_b, zero_frac, bit_width)){
	str_front_push(b, '0');
	str_front_push(b, '1');
	strcpy(in_b, b);
      }
      else{
	str_front_push(b, '1');
	str_front_push(b, '1');
	strcpy(in_b, b);
      }
    }else{
	str_front_push(b, '1');
	str_front_push(b, '0');
	strcpy(in_b, b);
    }

    gmp_printf("%s %s \n", in_a, in_b);  
    
  }
  
  free(exp_a);
  free(exp_b);
  free(frac_a);
  free(frac_b);
  free(in_a);
  free(in_b);
  free(zero_exp);
  free(zero_frac);
  free(inf);

  fclose(fp);
  
  return 0;  
}
