# minitalk

`minitalk` is a client-server pair which can transfer messages locally from the client to the server using POSIX signals. Its purpose is to demonstrate some of the strengths and weaknesses of standard signals.

WARNING: `minitalk` was designed to be used within a specific virtual environment. As such, performance will vary on other machines.

## Installation

```
git clone git@github.com:tmorris42/minitalk.git
cd minitalk
make
```

## Usage

Run the server which will output the server's PID.

```
> ./server
949
```

In a separate terminal, run the client providing the server's PID and the message to send
```
> ./client 949 'Hello, World!'
```

The server should then print the message

```
> ./server
949
Hello, World!
```
