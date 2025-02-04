#include "fcntl.h"
#include "stat.h"
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  // Check if at least one file was given
  if (argc == 1) {
    write(2, "No files provided.\n", 19);
    exit();
  }

  // Loop through each file provided
  for (int i = 1; i < argc; ++i) {
    int fd = open(argv[i], O_RDONLY);
    if (fd < 0) { // Corrected error check
      write(2, "Error opening file ", 19);
      write(2, argv[i], strlen(argv[i]));
      write(2, "\n", 1);
      continue;
    }

    char buffer[1024];
    int bytesRead;
    int prevWasNewline = 0; // No bool in Xv6, using int

    // Read the file in chunks
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
      for (int j = 0; j < bytesRead; ++j) {
        if (buffer[j] == '\n') {
          if (!prevWasNewline) {
            write(1, "\n", 1);
          }
          prevWasNewline = 1;
        } else {
          write(1, &buffer[j], 1);
          prevWasNewline = 0;
        }
      }
    }

    if (bytesRead < 0) { // Corrected read error check
      write(2, "Error reading file ", 19);
      write(2, argv[i], strlen(argv[i]));
      write(2, "\n", 1);
    }

    close(fd);
  }

  exit();
}
