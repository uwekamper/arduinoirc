![IRC client on a green screen monitor](https://github.com/uwekamper/arduinoirc/arduinoirc.jpg)

This is an Internet Relay Chat (IRC) client running on an Arduino with Ethernet Shield.

After turning on the Arduino, the code automatically connects to a given IRC server and joins a channel. It then outputs all the messages in the channel to the serial port.

To view the chat contents on an external monitor we attached a second monitor via serial cross-over cable. The second arduino runs the TVOut library and serves as a dumb serial terminal.
