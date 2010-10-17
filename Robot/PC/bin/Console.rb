#!/usr/bin/ruby -I../lib
# encoding: utf-8

# Ici on peut appeler directement les fonctions du Robot
# Cela permet de faire du debug et des tests

require "readline"
require "Robot"

robot = Robot.new()
robot.demarrer

begin
        while line = Readline.readline("> ", true)
	        if line != ""
	                if line == "exit"
	                        break
	                else
	                        cmd = line.split(" ") 
	                        fonction = cmd.first
	                        cmd.reverse!.pop
	                        args = cmd.reverse!.collect! {|x| x.to_f}
	                        puts robot.send(fonction, *args)
	                end    
	        end
	end
rescue Interrupt => e
        robot.arreter
	exit
end
