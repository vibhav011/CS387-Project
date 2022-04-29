# ENO: A solution to ACIDity

This project is a course project under [Prof. Umesh Bellur](https://www.cse.iitb.ac.in/~umesh/index.html) and aims to add ACID properties to ToyDB.

## Building
`cd` inside the `receiver` directory and run `make`. This will create two executables: `daemon` and `dbprocess`.

## Running
- First run the executable `daemon`. This will initialize the database (and recover if the database crashed in the last run), and communicate with the worker processes.
- Then run the executable `dbprocess`. This process will connect with `daemon` and can be used to send queries to it.

## Contributors
- [Abhinav Gupta](https://github.com/guptabhinav49)
- [Ankit Kumar Jain](https://github.com/akj0811)
- [Tulip Pandey](https://github.com/TULIP16)
- [Vibhav Aggarwal](https://github.com/vibhav011)