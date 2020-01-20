# Final_Systems

## Project: Car Rental Management System ##
Team Members: Joseph Lee, Isabella Rocha

# Devlog #
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
* 1/19
  * Joseph: Changed car storing, shared memory, and semaphore structure to allow renting out by days and months. Continued working on the renting car prompts and returning car structure/format/prompts
