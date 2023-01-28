# TCP Knock Knock server

  A TCP server written in C, which uses the knock knock protocol. Demonstrates the Client-Server paradigm, by which the server sends out the joke, and the client can respond appropriately.

## Compiling

  Compile with - 'gcc -pedantic-errors -Wall -ansi -o server server.c'
  Run with - './server <port number>'

  Or in one line -
  'gcc -pedantic-errors -Wall -ansi -o server server.c && ./server <port number>'

  -pedantic-errors -> Shows all strict warnings
  -Wall -> Shows all warnings 
  -ansi -> ansi C/ C99 language standard

  To connect to the server on another machine/different terminal -
  'nc -C localhost <port number>'

## How does it work?

  Store different parts of the joke in 2D arrays, incrementers used to access the indexes.

  Server writes to the client, and the client responds with the correct response. Server will continue to send the same thing if a response is not 
  recognised/sensible, for example - 
  
  <img width="542" alt="Screenshot 2023-01-28 at 16 35 36" src="https://user-images.githubusercontent.com/68609221/215279257-2aae1b2f-26fe-488f-8531-    
                                                               3018c5d215ea.png">

## Interesting points
  
    A structure is used to store some information about the user, name and age. If I was to expand on this in the future, age could be evaluated, and some more adult jokes could be used, based on the age.

  Dynamic memory allocation is used for the structure, and the name inside the structure. I decided to use this, as for the name, nobody knows how long a name will be, by using this, memory can be allocated from the heap, for whatever length of name the user decides to input (within reason)

  The function read_line_from_network() is used to continually read from the network until all characters have been read. This is because only a specific number of bytes can be read at one time, and by using this function that I have written, it allows for all the data to be captured by the buffer.
