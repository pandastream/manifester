#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * m3u8_extention(char* filename) {
  char* bandwidth=NULL;
  
  char* ptr=NULL;
  char* ptr1=NULL;
  char* ptr2=NULL;
  
  ptr1 = strrchr(filename, '_');  
  if(ptr1 == NULL)
    return NULL;
  ptr2 = strchr(ptr1, 'k');
  if(ptr2 == NULL)
    return NULL;
  
  ptr = ptr1+1;
  int i = 0;
  while(*ptr && (ptr != ptr2)) {
    bandwidth = (char*) realloc (bandwidth, i+1 * sizeof(char));
    bandwidth[i] = *ptr;
    ptr++;
    i++;
  }
  
  char* ret_extention = NULL;
  ret_extention = malloc(sizeof(char) * 1024);
  sprintf(ret_extention, "#EXT-X-STREAM-INF:PROGRAM-ID=1,BANDWIDTH=%s000\n", bandwidth);

  free(bandwidth);
  return ret_extention;
}


char * m3u_extention(char * type, char* filename) {
 
  if(strncmp(type, "M3U8", 4) == 0) {
    return m3u8_extention(filename);
  }
  
  return NULL;
}


int write_header(FILE *file) {
  if(!file) {
    fprintf(stderr, "Error opening file\n");
    return -1;
  }
  
  fprintf(file, "#EXTM3U\n");
  return 1;
}

int write_index(FILE *file, char *index_filename) {
  
  if(!file) {
    fprintf(stderr, "write_index. Error opening file\n");
    return -1;
  }
  
  char * ext = m3u_extention("M3U8", index_filename);
  if(ext){
    fprintf(file, "%s", ext);
    free(ext);
  }

  fprintf(file, "%s\n", index_filename);
  
  return 1;
}


int write_m3u8(char * output_filename, char **indexes) {
  FILE *output_file = NULL;
  char **pt_indexes = NULL;
  
  output_file = fopen(output_filename, "w");
  if (!output_file) {
      fprintf(stderr, "Could not open (%s), no index file will be created\n", output_filename);
      return -1;
  }

  pt_indexes = indexes+2;

  write_header(output_file); 
  while(*pt_indexes != NULL) {
    write_index(output_file, *pt_indexes);
    pt_indexes = pt_indexes + 1;
  }

  fclose(output_file);
  return 1;
}


int main(int argc, char **argv) {
  char *output_filename = NULL;
  
  if (argc  < 3) {
      fprintf(stderr, "Usage: %s <output_file> <file1> [<file2> ...]\n", argv[0]);
      exit(1);
  }

  output_filename = argv[1];
  write_m3u8(output_filename, argv++);
  
  return 0;
}



