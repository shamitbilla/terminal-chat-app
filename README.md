# Terminal Chat Application 

## Overview

The Chat Application in C is a simple networking project allowing multiple users to log in with unique usernames and engage in real-time conversations. Developed out of a passion for networking principles and a desire to understand how such systems operate, this application provides a user-friendly platform for seamless communication. It serves as both a learning tool for exploring networking concepts and a practical demonstration of client-server architectures and socket programming. With its straightforward interface and intuitive functionalities, it offers an opportunity for enthusiasts to deepen their understanding of networked applications while fostering connections with friends and peers.



## Features

The Chat Application in C boasts the following key features:

- **Real-time Communication**: Users can engage in real-time conversations with friends and peers over a network.

- **User Authentication**: Users can log in with unique usernames, enabling personalized interactions and identity recognition.

- **Multithreading**: Utilizes multithreading in both the client and server components to handle multiple connections concurrently, ensuring efficient communication.

- **JSON Handling**: Incorporates concepts of JSON (JavaScript Object Notation) for data serialization and deserialization, facilitating efficient and structured communication between the client and server.

- **NCurses Interface**: Utilizes the NCurses library to provide a user-friendly and interactive command-line interface for the client application, enhancing the user experience.

**Communication Protocol**

The Chat Application in C utilizes a simple TCP (Transmission Control Protocol) socket for communication between the client and server. The protocol involves the following steps:

1. **Connection Establishment**:
   - The client initiates a connection request to the server using TCP/IP sockets.
   - The server accepts the connection and establishes communication with the client.

2. **User Authentication**:
   - Upon successful connection, the client sends a JSON-formatted message containing the user's login credentials (e.g., username) to the server for authentication.

3. **Message Exchange**:
   - Once authenticated, users can exchange messages in real-time.
   - Messages are serialized into JSON format before being sent over the network.
   - The server receives incoming messages, processes them, and broadcasts them to all connected clients.

4. **Disconnection**:
   - Users can gracefully disconnect from the chat application, closing their connection with the server.

By leveraging TCP sockets and JSON for communication, the chat application ensures reliable and structured data exchange, while multithreading enhances scalability and performance, enabling seamless communication among multiple users.



## Getting Started

To get started with the Chat Application in C, follow these simple steps:

1. **Clone the Repository**:
   - Open your terminal or command prompt.
   - Navigate to the directory where you want to clone the repository.
   - Run the following command:

     ```
     git clone https://github.com/shamitbilla/terminal-chat-app.git
     ```

2. **Compile the Code**:
   - Navigate to the directory containing the cloned repository.
   - Use the provided Makefile to compile the code. Run the following command:

     ```
     make
     ```
   This command will compile both the server and client components.

3. **Run the Server**:
   - After compiling the code, you can start the server component.
   - Run the following command:

     ```
     ./server
     ```
   This will start the server, allowing clients to connect and engage in conversations.

4. **Run the Client**:
   - Open a new terminal or command prompt window.
   - Navigate to the directory containing the compiled code.
   - Run the following command to start the client:

     ```
     ./client
     ```
   This will launch the client application, allowing you to log in with your username and start chatting with friends.

5. **Start Chatting**:
   - Once the client is running, follow the on-screen instructions to log in with your username and start chatting.
   - You can send messages, receive messages from other users, and enjoy real-time communication over the network.

Now you're all set to explore the Chat Application in C! Enjoy connecting with friends and peers in real time.

