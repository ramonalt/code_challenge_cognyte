# Client/Server Code Challenge for Cognyte 

This repository contains code for a client-server application developed as part of a code challenge for Cognyte. The application allows clients to connect to a server and send data through the shell. This README provides information on setting up, configuring, and running the server and client components.

## Table of Contents
- [Setup](#setup)
- [Updating the Configuration](#updating-the-configuration)
- [Running the Server/Client](#running-the-serverclient)
- [Code Structure](#code-structure)
- [Environment Variables](#environment-variables)

## Setup
To get started with the project, follow these steps:

1. Clone this repository to your local machine.
2. Open your terminal and navigate to the project's root directory.
3. Run the following command to set up the project:

   ```bash
   source ./setup.sh
   ```

This script will install the necessary dependencies, set environment variables, and build the project using CMake

## Updating the Configuration:

The server and client configuration is managed through environment variables. To update the configuration, follow these steps:

1. Open the config.sh script in a text editor.
2. Modify the environment variables as needed, along with their units and descriptions.
3. Save the changes.
4. Run the following command in your shell to apply the updated configuration:
    ```bash
    source ./config.sh
    ```

## Running the Server/Client

To run the server and client components, follow these steps:

### Server

1. Open your terminal.
2. Navigate to the project's root directory.
3. Run the server using the following command:

   ```bash
   ./build/server
   ```

### Client 
1. Open a separate terminal.
2. Navigate to the project's root directory.
3. Start the client using the following command:

   ```bash
   ./build/client
   ```

4. After starting the client, you will be prompted to insert data that you wish to send through the shell. Follow the prompts and input the data as required.


## Code Structure

The project's code is organized into several key components:

#### `src/client.cpp`

- This file contains the client application responsible for connecting to the server and sending data through the shell using `std::cin`.

#### `src/server.cpp`

- `server.cpp` is a critical component of the project that handles multiple TCP connections from clients. It processes incoming data and stores it into different files, each with a size not exceeding the value defined by the `COGNYTE_SERVER_MAX_FILE_SIZE` environment variable.
   The files saved by the server are stored with the following format: `{prefix}_{current_date}_{client_id}_{file_id}`.
- The server also includes a connection timer, specified by `COGNYTE_SERVER_CONNECTION_TIMER`, which determines how long the server should wait for data. If no data is received within this timer's duration, the connection is gracefully closed.

#### `src/file_writer.hpp`

- This header file, `file_writer.hpp`, is responsible for managing the writing of data from each connection to the disk. It ensures that data is correctly organized and stored based on the server's logic.

#### `src/configuration.hpp`

- `configuration.hpp` is a critical part of the project that handles the parsing of configuration environment variables. It allows for easy customization and control of server behavior by reading and interpreting the environment variables set during configuration.

#### `src/atomic_queue.hpp`

- `atomic_queue.hpp` exposes the necessary `std::queue` member functions while encapsulating them in a mutex to make it thread-safe.

## Environment Variables
The following environment variables control the server's behavior:

- * COGNYTE_SERVER_PREFIX * : Sets the prefix for the files that will store received data.
- * COGNYTE_SERVER_IP * : Specifies the IP address to which the server will listen for incoming TCP connections.
- * COGNYTE_SERVER_PORT * : Specifies the port on which the server will listen for incoming TCP connections.
- * COGNYTE_SERVER_MAX_FILE_SIZE * : Sets the maximum allowable size for a single file. If received data exceeds this size, the server will split it into multiple files, each no larger than the specified maximum size.
- * COGNYTE_SERVER_CONNECTION_TIMER * : Sets a timer that dictates how long the server should wait for incoming data. If no data is received within the timer's duration, the connection will be closed.

## Questions

1. Dependencies versions? 
   - Boost 1.74
   - g++ 11.4
   - Cmake 3.22.1
   check the `setup.sh` for more information on the used dependencies

2. Time spent? 4-5 hours.

3. Challenges? Nothing big, maybe getting used to boost and cmake since I've not used them in a while.