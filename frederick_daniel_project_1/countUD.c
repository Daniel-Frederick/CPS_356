#include "fcntl.h"
#include "stat.h"
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int sumUD = 0;
  int nonUDCount = 0;

  // Check if at least one file was given
  if (argc == 1) {
    printf(2, "No files provided.\n");
    exit();
  }

  // Loop through each file provided
  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);
    if (fd < 0) {
      printf(2, "Error opening file %s\n", argv[i]);
      continue;
    }

    char buffer[1024];
    int bytesRead;

    // Read the file in chunks and analyse each chunk
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
      for (int j = 0; j < bytesRead; j++) {
        if (buffer[j] == 'U' || buffer[j] == 'D') {
          sumUD++;
        } else if (buffer[j] != ' ' && buffer[j] != '\n' && buffer[j] != '\r' &&
                   buffer[j] != '\t') {
          nonUDCount++;
        }
      }
    }
    close(fd);
  }

  printf(1, "'U' or 'D' chars: %d\n", sumUD);
  printf(2, "Non 'U' or 'D' chars: %d\n", nonUDCount);

  exit();
}
