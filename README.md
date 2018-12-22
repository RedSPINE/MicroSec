# MicroSec Test

All the .txt files are not meant to be read. They are only there to be used by programs.

## Scripting part

The command line takes an url and a sequence like you asked. But it runs indefinitely.
It only shows to the user which url is being visited and when there's a match. All results are put in the file output.txt.
To change the target port of the client, there's a pre-processor constant at the beginning of the file.
I wanted to do the IP blocking script at the end but I finally didn't had the time.

## C part

The server can be run like a service using the .service file placed in /etc/systemd/system/. 
That's the fastest way I found to do this. I don't really know if that's what you expected.
Also the client doesn't close itself, each attempt to just set a basic "on/off" variable resulted in a segfault, and I don't really know why.
The server forces the script to end after a determined amount of time, set in pre-processor constant under the name TIME_TO_CRAWL.
I wanted the server to read output.txt as it's being written by the script but didn't succeed.

## Python part

I tried to start it one hour before the end but well, I don't have any experience in using python for running c programs.
Also I couldn't think of any smart way to setup my server.

## Conclusion

I clearly don't have enough experience to do the test in only 6 hours. At the end, I was too exhausted at the end to think right. The code is kinda "dirty" too, maybe I should have spent more time on the internet to look for similar use cases but I chose to try to do it by myself for the biggest part. I just copied a basic TCP client server to save time for the C part.
The script works fine for me, tell me if it doesn't for you.