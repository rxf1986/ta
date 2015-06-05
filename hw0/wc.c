#include <ctype.h>
#include <stdio.h>

void wc(FILE *ofile, FILE *infile, char *inname) {
   char c1, c;
   int n_char, n_word, n_line;
   c = getc(infile);

   if(c == EOF) {
       goto out;
   }

   c1 = c;
   n_char=1;
   while((c = getc(infile)) != EOF ) {
       n_char++;
       if(c == '\n') {
           n_line++;
           if (c1 != ' ')
               n_word++;
       }
       if(c == ' ') {
           if (c1 != ' ')
               n_word++;
       }
       c1 = c;
   }
 
out:  
   printf("%d %d %d %s\n", n_line, n_word, n_char, inname);
   
}

int main (int argc, char *argv[]) {
    FILE *fp;
    fp = fopen(argv[1], "r");
    
    if (fp ==NULL) {
      printf("open file error\n");
      return -1;
    }
    
    wc(NULL, fp, argv[1]);
    
    return 0;
}
