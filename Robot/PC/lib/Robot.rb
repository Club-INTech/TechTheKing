# Cet fichier contient la classe Robot

require "Log"
require "Detection"
require "Position"
require "Asservissement"
require "Actionneurs"
require "Evitement.rb"

# Cette classe est le robot (couche finale). Elle contient toutes les fonctions
# accessibles par les scripts.
class Robot

        # Temps restant, décrémente de seconde en seconde
        attr_reader :tempsRestant

        # Initialise les connexions avec les liaisons série avec les Arduinos.
        # Attributions dynamiques des identifiants ttyUSB.
        def initialize couleur, positionDebut = Position.new(0, 0, 0)
                @evitementactive = false
                @couleur = couleur

                @log = Logger.instance
                @tempsRestant = 88

                identifiantArduino = {
                        0 => "Asservissement", 
                        1 => "Actionneurs", 
                        2 => "Evitement"
                }

                @log.info "Initialisation du robot..."

                detectionPeripherique = Detection.new(identifiantArduino).association
                @asservissement = Asservissement.new detectionPeripherique["Asservissement"], positionDebut
                @actionneurs = Actionneurs.new detectionPeripherique["Actionneurs"]
                @evitement = Evitement.new detectionPeripherique["Evitement"]

                reset

                @log.info "Initialisation finie"

                allumerLed
        end

        # Démarre chaque service. Si activeTimer est vrai, alors le robot
        # s'arrête au bout de 90 secondes.
        def demarrer
                @log.info "Démarrage..."

                @asservissement.demarrer
                @actionneurs.demarrer
                @evitement.demarrer

                @log.info "Robot démarré"
        end
        
   	# demarre le timer du robot et arrete au bout de 90 secondes
        def demarrerTimer
                @timer = Thread.new {
                        @log.info "Démarrage du timer"
                        for i in (1..88)
                                @tempsRestant -= 1
                                sleep 1
                        end

                        desactiveAsservissement

                        @asservissement.arreter
                        @actionneurs.arreter
                        @evitement.arreter
                        @log.info "Fin du temps réglementaire"
                        exit
                }
        end

	# arrete le timer 
        def arreterTimer
                @timer.exit
        end

        # Arrêt des services
        def arreter
                @log.info "Arrêt..."

                @asservissement.arreter
                @actionneurs.arreter
                @evitement.arreter

                @log.info "Robot arrêté"
        end

        # Reset du robot
        def reset
                @log.info "Reset..."
                @asservissement.reset
                @log.info "Reset effectué"
                1
        end

        # Abscisse du robot
        def x
                position.x
        end

        # Ordonnée du robot
        def y
                position.y
        end

        # Orientation du robot par rapport à (Ox)
        def angle
                position.angle
        end

        # Position du robot
        def position
                if @couleur == :jaune
                        @asservissement.position
                else
                        @asservissement.position.symetrie
                end
        end

        # Valeurs des capteurs ultrason
        def ultrasons
                @evitement.ultrasons
        end

        # Contrôle d'état des ultrason haut
        def etatHaut
                @evitement.controleEtatHaut
        end

        # Valeurs des capteurs optiques
        def optiques
                @evitement.optiques
        end

        #place sur rail gauche
        def placeSurRailGauche
                @evitement.placeSurRailGauche
        end

        # place sur rail droit
        def placeSurRailDroit
                @evitement.placeSurRailDroit
        end

	# renvoie combien de place pour les tomates il nous reste
        def placeTotal
                #placeSurRailDroit + placeSurRailGauche
                return (5-@evitement.nombreOranges)
        end

        # Desactive l'asservissement
        def desactiveAsservissement
                @log.debug "Désactive l'asservissement polaire"
                @asservissement.desactiveAsservissement
                1
        end

        # Desactive l'asservissement
        def desactiveAsservissementRotation
                @log.debug "Désactive l'asservissement polaire"
                @asservissement.desactiveAsservissementRotation
                1
        end

        # Desactive l'asservissement
        def desactiveAsservissementTranslation
                @log.debug "Désactive l'asservissement angulaire"
                @asservissement.desactiveAsservissementTranslation
                1
        end

        # Déplace le robot en x, y avec une orientation angle
        # Renvoi vrai si aucun stop durant la manoeuvre
        def goTo x, y, *condition
                if @couleur == :jaune
                        @log.info "Aller à : " + x.to_s + ", " + y.to_s + ", " + angle.to_s
                        @asservissement.goTo Point.new(x, y)
                else
                        @log.info "Aller à : " + x.to_s + ", " + (-y).to_s + ", " + (-angle).to_s
                        @asservissement.goTo Point.new(x, y).symetrie
                end
        end

	# encore un autre goto
        def goToEx x, y, *condition
                if @couleur == :jaune
                        @log.info "Aller à : " + x.to_s + ", " + y.to_s
                        @asservissement.goToEx Point.new(x, y)
                else
                        @log.info "Aller à : " + x.to_s + ", " + (-y).to_s
                        @asservissement.goToEx Point.new(x, y).symetrie
                end
        end

	# encore un autre goto
        def goToDep x, y, *condition
                if @couleur == :jaune
                        @log.info "Aller à : " + x.to_s + ", " + y.to_s + ", " + angle.to_s
                        @asservissement.goToDep Point.new(x, y)
                else
                        @log.info "Aller à : " + x.to_s + ", " + (-y).to_s + ", " + (-angle).to_s
                        @asservissement.goToDep Point.new(x, y).symetrie
                end
        end

	# encore un autre goto
        def goToDep2 x, y, *condition
                i=10
                while i
                        goToDep x, y , *condition
                        i-=1
                end
        end

        # Change l'orientation du robot par rapport à la position du robot
        def tourneDe angle
                begin                
                        @log.info "Tourne de : " + angle.to_s
                        if @couleur != :jaune
                                angle = -angle
                        end
                        @asservissement.tourneDe angle
                        1
                rescue
                        puts "ERREUR"
                end
        end

	# avance d'une certaine distance
        def avancer distance

                @asservissement.avancer(distance)

        end

	# tourne d'un certain angle
        def tourner angle
                if @couleur != :jaune
                        angle = -angle
                end
                @asservissement.tourner(angle)
        end

	# aligne le robot suivant un angle
        def alignement angle
                if @couleur != :jaune
                        angle = -angle
                end
                begin
                        @asservissement.alignement angle
                rescue
                        puts "Erreur"
                end 
                1
        end

	# active l'évitement
        def activeEvitement
                @evitementactive = true
        end
        
        # désactive l'évitement
        def desactiveEvitement
                @evitementactive = false
        end
        
        # l'évitement est-il activé
        def evitement?
                @evitementactive
        end
        
        # Retourne l'état des codeuses pour la calibration
        def codeuses
                @asservissement.codeuses
        end

        # Envoi un signal d'arrêt à l'arduino, sort du goTo en cours
        def stop
                @log.info("Envoi du signal d'arrêt à l'arduino")
                @asservissement.stop
                1
        end

        # Arrêt d'urgence
        def stopUrgence
                @log.info "Arrêt d'urgence"
                @asservissement.stopUrgence
                1
        end

	# se recale suivant la bonne couleur
        def recalage
                if @couleur == :jaune
                        recalageJaune
                else
                        recalageBleu
                end
        end
        
        # Recalage du robot
        def recalageJaune
                @asservissement.recalage                
                1
        end

	# se recale sur le coté bleu
        def recalageBleu
                @asservissement.recalage3
                1
        end

        # Sens positif (1) ou négatif (0) du robot
        def sens
                @asservissement.sens
        end

	# fonction de recalage sur le coté jaune
        def recalageJauneEnCours
                @asservissement.remiseAZero Position.new(300, 300, 0)
                recalageJaune
        end

	# fonction de recalage sur le coté bleu
        def recalageBleuEnCours
                @asservissement.remiseAZero Position.new(300, -300, 0)
                recalageBleu
        end        

        # Allume la led du jumper
        def allumerLed
                @actionneurs.allumerLed
        end

        # Retourne l'état du jumper, prise jack
        def attendreJumper
                @log.info "Attente du jumper"
                while @actionneurs.etatJumper != 1
                        sleep 0.1
                end
                @log.info "Jumper débloqué"
        end

        # Vide les oranges en baissant la fourche
        def baisseFourche
                @actionneurs.baisseFourche
        end

        # Attrape les oranges en relevant la fourche
        def leveFourche
                @actionneurs.leveFourche
        end

	# range la fourche
        def rangeFourche
                @actionneurs.rangeFourche
        end

	# arrete les actionneurs
        def actionneursStopUrgence
                @actionneurs.stopUrgence
        end

	# fait tourner le rouleau dans le sens direct
        def rouleauDirect
                @actionneurs.rouleauDirect
        end

	# fait tourner le rouleau dans le sens indirect
        def rouleauIndirect
                # @log.debug "Envoi signal rouleau"
                @actionneurs.rouleauIndirect
        end

	# arrete le rouleau
        def stopRouleau
                @actionneurs.stopRouleau
        end

	# le sélecteur guide les tomates sur le rail gauche
        def selecteurGauche
                @actionneurs.selecteurGauche
        end

	# le sélecteur guide les tomates sur le rail droit
        def selecteurDroite
                @actionneurs.selecteurDroite
        end

	# met le sélecteur en position intermédiaire
        def selecteurMilieu
                @actionneurs.selecteurMilieu
        end

	# arrete le sélecteur (y compris ses petits mouvements)
        def stopSelecteur
                @actionneurs.stopSelecteur
        end

	# change les paramètres max de vitesse du robot
        def changerVitesse(rotation, translation)
                @asservissement.changerVitesse(rotation, translation)
        end
end
