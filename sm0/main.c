#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int sumUD = 0;
  int nonUDCount = 0;

  // Step 1: Check if at least one file was provided
  if (argc == 1) {
    fprintf(stderr, "No files provided.\n");
    return 1; // Return 1 since no files were given
  }

  // Step 2: Loop through each file provided in the command-line arguments
  for (int i = 1; i < argc; ++i) {
    // Step 2.1: Try to open the file in read-only mode
    int fd = open(argv[i], O_RDONLY);
    if (fd == -1) {
      // If opening the file fails, print an error and continue to the next file
      perror("Error opening file");
      continue;
    }

    // Step 2.2: Set up a buffer to store the file's contents in chunks
    char buffer[1024]; // Buffer to hold up to 1024 bytes at a time
    ssize_t bytesRead; // Variable to store how many bytes were read in each
                       // iteration

    // Step 2.3: Read the file in chunks and process each chunk
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
      // Step 2.3.1: Loop through each byte in the buffer
      for (ssize_t j = 0; j < bytesRead; ++j) {
        // Step 2.3.2: Check if the character is 'U' or 'D'
        if (buffer[j] == 'U' || buffer[j] == 'D') {
          sumUD++; // Increment count of 'U' and 'D'
        }
        // Step 2.3.3: Check if the character is a letter but not 'U' or 'D'
        else if ((buffer[j] >= 'A' && buffer[j] <= 'Z') ||
                 (buffer[j] >= 'a' && buffer[j] <= 'z')) {
          nonUDCount++; // Increment count of non-'U'/'D' alphabetic characters
        }
      }
    }

    // Step 2.4: If reading the file encountered an error, report it
    if (bytesRead == -1) {
      perror("Error reading file");
    }

    // Step 2.5: Close the file after processing
    close(fd);
  }

  // Step 3: Output the results
  // Step 3.1: Output the total count of 'U' and 'D' characters
  printf("%d\n", sumUD);

  // Step 3.2: Output the count of non-'U' and non-'D' alphabetic characters to
  // stderr
  fprintf(stderr, "Non-'U' and 'D' characters: %d\n", nonUDCount);

  // Step 4: Return appropriate status code based on findings
  if (sumUD > 0) {
    return 0; // Return 0 if at least one 'U' or 'D' was found
  } else {
    return 1; // Return 1 if no 'U' or 'D' was found
  }
}
