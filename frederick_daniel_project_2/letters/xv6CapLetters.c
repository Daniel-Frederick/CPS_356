#include "fcntl.h"
#include "stat.h"
#include "types.h"
#include "user.h"

char upper(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + 'A';
  }
  return c;
}

int my_isspace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
         c == '\v';
}

int my_isalpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int main(int argc, char *argv[]) {
  // Check if at least one file was given
  if (argc == 1) {
    write(2, "No files provided.\n", 19);
    exit();
  }

  // Loop through each file provided
  for (int i = 1; i < argc; ++i) {
    int fd = open(argv[i], O_RDONLY);
    if (fd < 0) { // Fixed error check
      write(2, "Error opening file ", 19);
      write(2, argv[i], strlen(argv[i]));
      write(2, "\n", 1);
      continue;
    }

    char buffer[1024];
    int bytesRead;

    // Read the file in chunks and analyze each chunk
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
      for (int j = 0; j < bytesRead; ++j) {
        if (my_isalpha(buffer[j])) {
          printf(1, "%c", upper(buffer[j]));
        } else if (my_isspace(buffer[j])) {
          printf(1, "\n");
        }
      }
    }

    close(fd);
  }

  exit();
}
