# Large Project 0 By Daniel Frederick

My Device Drivers:

For the project, I used this format:
When a read gets called, it will print "Read <>" and whatever it reads will be in the "<>."
When a write gets called, it will print "Bytes written: " and whatever it will print out the number of bytes written after the ':.'

1. Since this device driver can only read '\0', when a read is called what happens is it just fills all the space with '\0' and returns the amount of space that was available.

To Test it, I just changed the third parameter number on line 14 in testzero.c to read a different amount, but it only ever reads '\0' and returns the given number. When Changing the parameter, be reasonable, otherwise it will not work.

It is read only because doing a write will ways print out -1. Line 17 in testzero.c, I test if it is able to write but bytes_written will only ever return -1.

2. The write function in kkernel/wnull.c just returns the number of bytes to write. In testwnull.c, on line 17, you can change the third parameter number to whatever it will print that number.

It looks like this if n = 6: "Bytes written: 6"

Since it is write only, I test if it can read on line 14 in testwnull.c, but it will only ever return -1.

3. I made two "Read <>" for this device driver. The first one prints out the current value of the xv6 kernel's internal tick counter, and the other returns how many elements are in the buffer. 

The number of elements in the buffer will always be the kernel's tick counter number of characters plus one.

4. This program uses fork() to create different processes to try and block one the child process from reading from the buffer before the parent processes can write to the buffer.
It does not fully work but I gave it my best shot :)

