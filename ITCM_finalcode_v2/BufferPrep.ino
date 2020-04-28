// Write union data to transmit buffer in preparation of data transmission NC
void writeBuffer() {
  messageCounter++;                         // Increment message counter
  message.messageCounter = messageCounter;  // Write message counter data to union
  transmitCounter++;                        // Increment data transmission counter

  // Concatenate current message with existing message(s) stored in transmit buffer
  memcpy(transmitBuffer + (sizeof(message) * (transmitCounter + (retransmitCounter * transmitInterval) - 1)),
         message.bytes, sizeof(message)); // Copy message to transmit buffer

#if DEBUG
  printUnion();
  //printUnionBinary(); // Print union/structure in hex/binary
  //printTransmitBuffer();  // Print transmit buffer in hex/binary
#endif
}
