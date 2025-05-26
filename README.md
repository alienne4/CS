# CS
Alice Cania - Computer Security Project

In order to correctly and efficiently solve the three required tasks regarding 
the return-to-libc attack, we have to go through a set of steps such that all of our 
practices end up the way we expect them to. We will be working in the 
SEEDUbuntu 16.04 (32 bit) VM, running on Oracle VirtualBox. 
First and foremost, we will disable Address Space Randomization, in the 
benefit of our buffer-overflow attack, guessing addresses being a critical step for it. 
Let us not forget that the /bin/sh symbolic link points to the /bin/dash shell. 
Because we are running a 16.04 version of Ubuntu, the dash shell has a 
countermeasure that prevents itself from being executed in a Set-UID process which 
changes the effective user ID to the process’s real user ID. Because we are targeting 
a Set-UID program with our attack, using the system( ) function to run a command, 
the countermeasure activates. To disable the protection, we make use of a custom 
shell, “zsh”, already installed in the SEEDUbuntu 16.04 VM, linking /bin/sh to it. 
We will compile our program, retlib.c, in a way such that we turn off The 
StackGuard Protection Scheme (otherwise buffer overflows will not work) and 
turning on Non-Executable Stack Protection (to prove this countermeasure does 
not work).  

The retlib.c program firstly reads an input of size 300 bytes from “badfile” 
into a buffer of size BUF_SIZE (in our case, the default value of 12 bytes). A buffer 
overflow will occur because fread( ) does not check the buffer boundary. Because 
this program is a root-owned Set-UID program, we can construct the “badfile” in a 
way such that when the vulnerable program copies the file contents into its buffer, a 
root shell can be spawned. We will now compile the code and turn it into a root-owned Set-UID program. 

Because we are using Linux, when a program runs, the libc library will be 
loaded into memory. The library will be always loaded in the same memory address 
(for the same program) because we have turned off the address space randomization. 
We will be using the GNU Debugger (gdb) in order to easily find the address of the 
system( ) function from libc. We will also create de “badfile”, using the touch 
command. It is also important to use the run command once, in order for the library 
code to be called.

For our attack to work properly, we will jump to the system( ) function and 
we will make it execute the "/bin/sh" program. We must put the "/bin/sh" command 
string in memory and we need to get its address so we can pass it to system( ). 
Environment variables a are great way in which we can reach our goal. 
We will define a new shell variable called MYSHELL, we will make it 
contain the string "/bin/sh" and we will also check that it gets printed out by the env 
command running inside the child process. When we execute a program from a shell 
prompt, the shell spawns a child process to execute the program, and all the exported 
shell variables become the environment variables of the child process.

As I have mentioned before, the MYSHELL variable’s address will be used 
as an argument to the system( ) function. 

I have used each of the addresses found during the steps from above and have 
stored them accordingly in the buf array. We should set the address of system( ) at 
bof's return address (&buf[24]), where ebp (The Frame Pointer) now stands. 
Furthermore, ebp + 4 (&buf[28]) is treated as the return address of  system( ). We 
can put the exit( ) address here so that on system( ) return, exit( ) is called and the 
program doesn’t crash. The argument of system( ) needs to be put at ebp + 8 
(&buf[32]) for our attack to be executed successfully. 
When the function bof( ), from retlib, returns, it will return to the system( ) 
function and execute system("/bin/sh"). In this way, we will gain access to the root 
shell.
