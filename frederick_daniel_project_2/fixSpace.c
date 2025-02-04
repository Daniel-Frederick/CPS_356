#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h> // Added for boolean type
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Check if at least one file was given
  if (argc == 1) {
    fprintf(stderr, "No files provided.\n");
    return 1;
  }

  // Loop through each file provided
  for (int i = 1; i < argc; ++i) {
    int fd = open(argv[i], O_RDONLY);
    if (fd == -1) {
      // If opening the file fails, print an error and continue to the next file
      perror("Error opening file");
      continue;
    }

    char buffer[1024];
    ssize_t bytesRead;
    bool prevWasNewline = false;

    // Read the file in chunks and analyse each chunk
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
      for (ssize_t j = 0; j < bytesRead; ++j) {
        if (buffer[j] == '\n') {
          if (!prevWasNewline) {
            putchar('\n');
          }
          prevWasNewline = true;
        } else {
          putchar(buffer[j]);
          prevWasNewline = false;
        }
      }
    }

    if (bytesRead == -1) {
      perror("Error reading file");
    }

    close(fd);
  }

  return 0;
}
