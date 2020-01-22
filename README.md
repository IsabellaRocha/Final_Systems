# Final_Systems

## Project: Car Rental Management System ##
Team Members: Joseph Lee, Isabella Rocha

Our project is a management system for a hypothetical car rental service. Users may rent and return cars. Users may request specific models and may specify on what days they will need it. Users may also cancel requests and return the cars earlier. However, if all cars available in that model are currently rented out, they will have to wait for a car to be returned if they want to rent it.

#### User Interface: ####
* The user will first be prompted with a login screen, asking them to either create a new account or log in to an existing account
* Once either a new account is created or an account is logged into, the screen will then switch to a menu in which the user has the options to view available cars, view their account information, view when they have to return their car, return a car if they already have one in their possession, or log out.
* If they choose to view available cars, they will be shown basic information about all cars we offer. They will be then asked to specify during which days they will need to rent out a car. 
* They will then be shown what cars are available on those days. They will be asked to choose a specific car to rent out, and prompt the user asking whether or not they are sure this is the car they want and alert them of the price before confirming the decision.
* They may choose to rent it now, or reserve it for certain days.
* They may also cancel that reserve by returning their cars earlier


#### Technical Design: ####

This project will utilize
* Working with files to store user information (So info isn’t lost in between sessions)
* Allocating memory to create additional users and cars
* Using data structures efficiently to store relevant information such as userid or rental dates
* Shared memory and semaphores to save data on what car a user is renting out and when, and on which cars are rented out on which days
* Running subprocesses using exec and forking

#### Data structures: ####
* Structs
  * A calendar struct that stores 3 int arrays of size 365, each array representing one unit of the car
  * A car struct with vehicle characteristics such as model name and also stores its own calendar struct
  * A user struct for username, passwords, current car, and rental dates
* Arrays
  * Array of days: a 365 long array where each index represents the day of a full year and stores the userid renting out the unit for that day
  * Array of 10 possible cars to rent
  * Array of 100 possible users

### Instructions ###
* When you are opening the program for the first time, please run make in order to compile all the files, and then run ./control -c in order to create the shared memory
* After that, simply run make run and follow the prompts on the screen and enter responses in the requested format, separating dates by dash and making sure to type your inputs in all lower case except when typing in the car model: type that exactly as it appears on screen
* If the instructions are not clear on what to do next after you've finished an operation, enter 'back' to return to the display menu
* We recommend testing all the features, such as renting, returning, and viewing your account, within one log in session. The reasons why is elaborated on the bugs section at the bottom
* If you format incorrectly and run into an error, you must remove the shared memory and semaphores using ./control -r, then restart the program

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
* We have issue with shared memory. When we log out, we update the shared memory with the new information and we even print its value before detaching it. However, when we call the shared memory to access it, it seems to not have been updated, despite using the correct reading and writing shmflg. It's not a memory copying issue either, because we accurately get the username and we even printed the values before copying it over. This also applies to the shared memory we store regarding the vehicles. Although the bug is not as apparent because we prevent users from renting out more than one car. All the code and structure for renting is in place and has been tested with non-shared memory -- if three units of a car are taken up for a day during one of the days they need to rent a car, it will not be displayed as avaialable. However, when testing this with shared memory, even though we print the updated value before dettaching, we run into the same issue as logging out. Thus, we recommend testing all the features within one login session.
