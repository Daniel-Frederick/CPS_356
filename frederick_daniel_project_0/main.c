#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int sumUD = 0;
  int nonUDCount = 0;

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

    // Read the file in chunks and analyse each chunk
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
      for (ssize_t j = 0; j < bytesRead; ++j) {
        if (buffer[j] == 'U' || buffer[j] == 'D') {
          sumUD++;
        } else if (!isspace((unsigned char)buffer[j])) {
          nonUDCount++;
        }
      }
    }

    close(fd);
  }

  printf("'U' or 'D' chars: %d\n", sumUD);
  fprintf(stderr, "Non 'U' or 'D' chars: %d\n", nonUDCount);

  // Return appropriate status code based on input
  if (sumUD > 0) {
    return 0;
  } else {
    return 1;
  }
}
