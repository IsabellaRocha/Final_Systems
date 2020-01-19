# Systems Final Project #

Team Members: Joseph Lee, Isabella Rocha

## Project: Car Rental Management System ##

Our project will be a management system for a hypothetical car rental service. Users may in which users can view available cars, and rent and return cars. Users may request specific models and may specify on what days they will need it. Users may also cancel requests and return the cars earlier. However, if all cars available in that model are currently rented out, they will have to wait for a car to be returned if they want to immediately rent it. Users can also queue to be next in line for a rental in case the car is returned earlier or a rental is canceled.

#### User Interface: ####
* The user will first be prompted with a login screen, asking them to either create a new account or log in to an existing account
* Once either a new account is created or an account is logged into, the screen will then switch to a menu in which the user has the options to view available cars, view cars that are currently being rented out, view the current car they have rented out if any, return a car if they already have one in their possession, or log out.
* If they choose to view available cars, they can then select a car to rent out, in which it will then display more information about the car and prompt asking whether or not they are sure this is the car they want.
* They may choose to rent it now, or reserve it for certain days.
* They may also cancel that reserve or return their cars earlier


#### Tasks Implemented: ####

This project will utilize
* Working with files to store user information (So info isn’t lost in between sessions)
* Allocating memory to create additional users
* Shared memory and semaphores to save data on how many cars are available and which ones are already rented out

#### Data structures: ####
* Structs
  * A car struct with vehicle characteristics
  * A day struct with number of cars left for every model and a pointer to a queue*
  * A user struct for username, passwords, and current car
* Arrays
  * Array of available cars
  * Array of days*
  * Array of cars currently rented out
* Queues*
  * Queues for each model



#### Project Break Down: ####
* (1/6) Have all basic display screens setup (Isabella)
* (1/7) Users can create and log in to accounts (Isabella)
* (1/7) Users can request to view available cars (Joseph)
* (1/9) Users can request cars and view cars in their possession (Joseph)
* (1/11) Users can return cars and shared memory and semaphores are implemented (Isabella/Joseph)
* (1/14) Users can specify on what days they want the cars* (Joseph)
* (1/14) Users can queue to use cars on certain days in case the car is returned earlier or a rental is canceled (Isabella)
* (1/15) Final debugging (Isabella/Joseph)

Lines denoted by a * are extra-features that will be done after the minimum-viable product
