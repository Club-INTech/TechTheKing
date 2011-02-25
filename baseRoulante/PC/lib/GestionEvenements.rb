# encoding: utf-8

class GestionEvenements

        def initialize(robot, carte)
                @log = Logger.instance

                listeMethodes = methods()
                @listeEvenements = []

                @log.debug "Chargement des évènements"
                listeMethodes.each { |e| 
                        if e[0..1] == "ev"
                                @listeEvenements.push(e)
                                @log.debug "    " + e + " chargé"                                 
                        end
                }
                @log.debug "Evènements chargés"
                
                @carte = carte
                @robot = robot
                
                @strategie = nil
                
                @blocageStrategie = false
        end

        def demarrer delta = 0.1
                if @listeEvenements.empty?
                        @log.debug "Pas d'évènements, arrêt du thread"
                else
                        @log.debug "Démarrage des évènements"
                        
                        setup
                        
                        @ordonnanceur = Thread.new {
                                loop {
                                        @listeEvenements.each { |e| 
                                                send(e)
                                        }
                                        sleep delta
                                }
                        }
                end
        end

        def arreter
                @log.debug "Arrêt des évènements"
                @ordonnanceur.exit
        end
         
end

