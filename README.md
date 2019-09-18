# Data manipulator

This program was made for a selecrtion process in a company. The task was to organize the information contained in a .json file.
The original file had details on buy events and products, and we had to group the transactions into a timeline, ordered by
date, with a specific formatting.

## How to use

### Compiling

Simply compile the source code (timeliner.cpp) with your favorite C++ compiler. The program uses the Json library by Niels Lohmann,
so don't forget to place the folder 'nlohmann' in the same directory as main.cpp.

### Run from terminal

To run the application, use the terminal and give as inputs (1) the file with the events and (2) the name of the output file in
which the timeline should be saved (remember the name that you gave to the program when compiling):

`./timeliner events.json timeline.json`

The file I tested it on is events.json. which is also available in this repository.

**WARNING:** the program will overwrite the output file in case it already exists. It will not confirm if the user really wants
that.
