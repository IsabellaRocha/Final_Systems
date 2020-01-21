# Final_Systems

## Project: Car Rental Management System ##
Team Members: Joseph Lee, Isabella Rocha

Our project is a management system for a hypothetical car rental service. Users may rent and return cars. Users may request specific models and may specify on what days they will need it. Users may also cancel requests and return the cars earlier. However, if all cars available in that model are currently rented out, they will have to wait for a car to be returned if they want to rent it.

### Instructions ###
* When you are opening the program for the first time, please run make in order to compile all the files, and then run ./control -c in order to create the shared memory
* After that, simply run make run and follow the prompts on the screen and enter responses in the requested format, separating dates by dash and making sure to type your inputs in all lower case except when typing in the car model: type that exactly as it appears on screen
* If the instructions are not clear on what to do next after you've finished an operation, enter 'back' to return to the display menu
* We recommend testing all the features within one run of the program. Logging in and Logging out preserves all features but exiting the program does not. We've explained more below on the bug that happens when the program closes.
* If you format incorrectly and run into an error, you must remove the shared memory and semaphores using ./control -r, then restart the program

### Features ###
* Creating and logging into accounts using files
* Renting and returning cars using shared memory and semaphores
* Running subprocesses using exec and forking

## Devlog ##
* 1/5
  * Isabella: Created a headers file with a vehicle struct and a make file
* 1/8
  * Isabella: Started making the display screens, the first one prompting the user to log in or create a new account, and then a menu with options to view your account, log out, view available cars, and view rented cars
* 1/9
  * Isabella: Created structs for months, days, and user
  * Joseph: Helped figure out what elements are needed in the structs, figured out what we need to store in our shared memory
* 1/10
  * Isabella: Started creating users
  * Joseph: Created initialize and rent files to implement shared memory to store which cars have been rented out and the available cars
* 1/13
  * Isabella: Added clearing the output screen, fixed main, added creating users to a file to store their username and password separated by a comma, made a parse args to read the file in order to verify users and log in
  * Joseph: Continued implementing shared memory, worked on memory issues in parse args
* 1/14
  * Isabella: Got logging in to mostly work, added logging out
* 1/15
  * Isabella: Logging in works now, edited headers file, stored username and password in struct after logging in to keep track of who's logged in, finished viewAccount, made it so if a username is taken, you cannot create another user with that username, fixed exiting the program, removed spaces from input in case user accidentally puts spaces before or after
  * Joseph: Fixed memory issue in removeSpace that prevented our user text file from having more than 55 characters
* 1/17
  * Isabella: Created separate shared memories for rented cars and available cars
* 1/19
  * Isabella: Added cars to our shared memory, got viewing available cars to work, edited how our shared memory works, updated the users struct, utilized forking and execvp to access shared memory commands in our rent and return files
  * Joseph: Changed car storing, shared memory, and semaphore structure to allow renting out by days and months. Continued working on the renting car prompts and returning car structure/format/prompts
* 1/20
  * Isabella: Helped try to figure out shared memory and semaphores, debugged, added cost to users to limit how much they can spend
  * Joseph: Reorganized files, wrote the shared memory and semaphore code for return_car and rent, added using shared memory and semaphores in user when logging in and creating an account, debugged
* 1/21
  * Joseph: Fixed bugs

## Bugs ##
* We have issue with shared memory after the program clases. Everything works even when logging out. The shared memory is updated. However, when exiting the program, which is a simple stopping the while loop, the updates made on the shared memory is somehow lost after the program is closed.
