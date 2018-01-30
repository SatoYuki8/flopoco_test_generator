#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <gmp.h>

typedef struct{
  int d_c;
  char *data[16];
}two_param;

two_param split(char *str){
  char *cp;
  char *tmp;
  two_param data;

  cp = str;
  tmp = strtok(cp, " ");

  int i=0;

  while(tmp != NULL){
    data.d_c = i;
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
  char *a;

  char *exp_a;
  char *frac_a;

  char *in_a;

  char *zero_frac;
  char *zero_exp;
  char *inf;

  int i;

  int line=0;             /* Input file's line */
  int in_flag=0;
  int out_flag=0;

  int inf_flag=0;
  int nan_flag=0;

  zero_frac = (char *)calloc(frac+1, sizeof(char));
  zero_exp = (char *)calloc(exp+1, sizeof(char));
  inf = (char *)calloc(exp+1, sizeof(char));
  
  zero_padding(zero_exp, exp);
  zero_padding(zero_frac, frac);
  one_padding(inf, exp);
 
  while (fgets(str, 64, fp) != NULL){
    
    line++;
    
    if(line%2==1){
      in_flag = 1; out_flag = 0;
    }else{
      in_flag = 0; out_flag = 1;
    }      

    exp_a = (char *)calloc(exp+1, sizeof(char));
    frac_a = (char *)calloc(frac+1, sizeof(char));
    in_a = (char *)calloc(bit_width*2, sizeof(char));
        
    data = split(str);

    if(in_flag){
      for (i=0;i<data.d_c;i++){
	a = (char *)calloc(bit_width*2, sizeof(char));
	
	strcpy(a, data.data[i]);
	
	strncpy(exp_a, a+1, exp);
	strncpy(frac_a, a+1+exp, frac);
	
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
	    inf_flag = 1;
	  }
	  else{
	    str_front_push(a, '1');
	    str_front_push(a, '1');
	    strcpy(in_a, a);
	    nan_flag=1;
	  }
	}else if(a[1] == NULL){
	  strcpy(in_a, a);      
	}else{
	  str_front_push(a, '1');
	  str_front_push(a, '0');
	  strcpy(in_a, a);
	}
	
 	  gmp_printf("%s ", in_a);  

	free(a);
    }    

      printf("\n");

    }else if(out_flag){
      for (i=0;i<data.d_c;i++){
	a = (char *)calloc(bit_width*2, sizeof(char));
	
	strcpy(a, data.data[i]);

	strncpy(exp_a, a+1, exp);
	strncpy(frac_a, a+1+exp, frac);

	if(strlen(a) != exp+frac+1){
	  strcpy(in_a, a);
	}else if(nan_flag){
	  str_front_push(a, '1');
	  str_front_push(a, '1');
	  strcpy(in_a, a);
	  nan_flag = 0;
	}else if(inf_flag){
	  str_front_push(a, '0');
	  str_front_push(a, '1');
	  strcpy(in_a, a);
	  inf_flag = 0;
	}else if(!strncmp(exp_a, zero_exp, bit_width)){
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
	    inf_flag=0;
	  }
	  else{
	    str_front_push(a, '1');
	    str_front_push(a, '1');
	    strcpy(in_a, a);
	    nan_flag=0;
	  }	  
	}else{
	  str_front_push(a, '1');
	  str_front_push(a, '0');
	  strcpy(in_a, a);
	}

	gmp_printf("%s ", in_a);  

	free(a);
      }
    
      printf("\n");      
    }


    free(exp_a);
    free(frac_a);
    free(in_a);
  }
  
  free(zero_exp);
  free(zero_frac);
  free(inf);

  fclose(fp);
  

  return 0;  
}
