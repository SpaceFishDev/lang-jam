# MakeStringsDoThings

# How To Build
You need basic C build tools. You should have everything needed, but if you do come into issues run the following on debian.
`sudo apt-get install build-essential`
on windows you'll need gnu make and gcc.
To build, go into the lang/ directory and run
`make all` to build and run
or
`make build` to just build the project.
# Idea

Without numbers, how do you do anything?
Consider the following string.
"ABC" 
there are 3 characters, this denotes the numerical value of 3

We could use this FOR EVERYTHING

define a function for example
```c
label_example: 
	
branch label_example  // branch to label if args (top two values on stack) is equal so in this case it would
goto end

end:
"Hello"
print // prints 5 and pops it from stack
"Hello"
puts // prints hello
"ABC" // pushes
add "CBA" // removes 3 from stack and adds 6 to stack
dup // duplicates it
print // prints 6
puts // prints ABCCBA
```
all strings are both numerical values and pointers
and a function could be used as a number

# END GOAL
Rule 110. plain and simple.
