# UDP-Client-Server

In this assignment, you will develop a simulation application that has a sender and
receiver in C. The sender sends two types of packets using UDP sockets every 100
ms(milliseconds) and every 150 ms repeatedly. The packet structure should contain
a packet type( 1 or 2), sequence number, byte array of 1024 bytes (payload), and a
trailer for the checksum of the entire packet ( bytewise XOR). The receiver
application receives these packets using 4 threads. The first thread checks the
received packets for errors. The second thread processes the packets of type 1. The
third thread processes the packets of type 2. The fourth thread periodically (every
300th ms) prints the number of packets of each type received. You may use the
pthread library and any synchronization primitives.
