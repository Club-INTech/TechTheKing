require "serialport"
require "rubygems"
sp=SerialPort.new("/dev/ttyUSB0",9600)
puts sp
while(1)
	sp.write("?\r\n")
end
puts(sp.getc)
