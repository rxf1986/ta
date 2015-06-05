#include <stdio.h>
#include <ctype.h>
#include <assert.h>
/* function will be invoked */
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *__stream);
int fgetc(FILE *__stream);
void wc(FILE *ofile, FILE *infile, char *inname) {
int newlines, words, bytes;
char c;
int isWord;
assert(ofile != NULL);
assert(infile != NULL);
assert(inname != NULL);
newlines = words = bytes = 0;
isWord = 0;
while((c = (char)fgetc(infile)) != EOF) {
bytes++;
printf("%d\n", c);
putchar(c);
//fputc(stdout, c);
if(c == '\r' || c == '\n') {
newlines++;
isWord = 0;
}
else if((c >= 'a' && c <= 'z')
|| (c >= 'A' && c <= 'Z') ) {
if(!isWord) {
isWord = 1;
words++;
}
}
else if (isWord) {
isWord = 0;
}
}
fprintf(ofile, "%d %d %d - %s\n", newlines, words, bytes, inname);
return;
}
int main (int argc, char *argv[]) {
char *path = argv[1];
FILE *infile;
infile = fopen(path, "r");
if(infile == NULL) {
fprintf(stderr, "failed to open %s!\n", path ? path : "NULL");
return 1;
}
wc(stdout, infile, path);
return 0;
}
