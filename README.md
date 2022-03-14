# HTTP Log Monitoring Console Program

 The solution is structured in the following way : Parser, Buffer and Alert.

 The main idea :
  1. Read a line from the input file in the main loop
  2. Parse the line
  3. Update the buffer with the information
  4. Check for the Alerts

 ## Detailing solution:

###  1. Read the file
  - Simple getline of the file using the standard library (for c++, no bottleneck here)

###  2. Parse the line
  - Parse using boost functions to return an object representing the line
    - For simplicity, I prefered using it than regular regex

### 3. Update the buffer with the parsed info
  - After reading, I update the buffer;
  - The idea was to do a kind of circular buffer, only storing the max window required for analysis, in this case, 120 seconds
  - Since we can have multiple lines per second/timestamp, we should aggregate them
  - For this buffer, I used a hahsmap, where the key is the timestamp (so each second will have one line) and the value is an object counting the number of connections on that specific second and a list of requests that were made

### 4. Check Alerts
  - I created two kind of Alerts : a recurrent (based on frequency, in our case, 10s) and an average one;
  - For the average one, I used the idea of a sliding window, so everytime a new value arrives I delete the oldest one and recalculate the average. If the average is bigger than our threshold, we have an alert.
  - For the recurrent alarm (10s one), I go through the buffer (since every line is a second) and analyze the information to show how many connections on this 10 second windows happened
    - To detect when to launch this alarm in a simple manner, I just used t modulo 10 (% 10) to detect when to show the alerts;


### Improvements/Limitations:
  - Add unit tests and integration tests : due the lack of available time, there are just few testcases
  - I did not consider a change of the header in terms of format and order
  - I did not check for possible errors in the input file
  - Instead of reading the line and parsing the content, read the words and place them in a new object directly to avoid going through the string again (even though c++ probably won't struggle with it)
  - The buffer has a size of the main window we want to analyze (i.e. 120s), however, if we wanted to have two big windows to analyze this would be a problem for the current design.
    - Another solution could be : have a bigger buffer, and the alerts would be calculated just like the 10s one. (but we would do it in linear time, expensive on every check)
    - Another solution could be : for each alarm, we would have a buffer (based on the size of the alarm), but it would cost extra space and time to copy to all of them
  - The analysis of the requests/sections should be done in a more intelligent way. I did not finish it but in my solution we should iterate a lot, I would need to think for a better solution on this one.
  - The Alert logic is based on states (hightraffic and normal traffic), I used an if, in this small program it is ok, but I guess if we wanted to expand would be better to think about a good desing pattern to avoid a lot of if's
  - Increase the test coverage and its scope


#### p.s.: to make it simple, I just created .h files in the cpp project

## How to run the program
  - Add the home of boost and google test to the cmakefile (ThirdPartyInc_DIR and ThirdPartyLib_DIR)
  - mkdir build ; cd build ; cmake ../ ; make
  - Running executable : ./http_log_monitoring
  - Running tests : ./Google_Tests_run
  - You can find an example of output of the sample here : doc/ouput_example.txt

