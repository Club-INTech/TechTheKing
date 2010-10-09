#!/usr/bin/ruby -I../lib

require "Match"

robot=Robot.new(:jaune)
robot.demarrer
robot.recalage

carte=CarteTechTheFruit.new
match=Match.new

match.donnerRessources(robot,carte)
match.sequence
