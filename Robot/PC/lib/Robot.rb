# encoding: utf-8

# Cet fichier contient la classe Robot

require "Detection"
require "Asservissement"
require "Actionneurs"
require "Capteurs"

# Cette classe est le robot (couche finale). Elle contient toutes les fonctions
# accessibles par les scripts.
class Robot


        # Initialise les connexions avec les liaisons série avec les Arduinos.
        # Attributions dynamiques des identifiants ttyUSB.
        def initialize 

        end

        # Démarre chaque service. Si activeTimer est vrai, alors le robot
        # s'arrête au bout de 90 secondes.
        def demarrer
        end
        
   	# demarre le timer du robot et arrete au bout de 90 secondes
        def demarrerTimer
        end

	# arrete le timer 
        def arreterTimer
        end

        # Arrêt des services
        def arreter
        end

        # Reset du robot
        def reset
        end

        # Abscisse du robot
        def x
        end

        # Ordonnée du robot
        def y
        end

        # Orientation du robot par rapport à (Ox)
        def angle
        end

        # Desactive l'asservissement
        def desactiveAsservissementRotation
        end

        # Desactive l'asservissement
        def desactiveAsservissementTranslation
        
        end

        # Déplace le robot en x, y avec une orientation angle
        # Renvoi vrai si aucun stop durant la manoeuvre
        def goTo 
        end

        # Change l'orientation du robot par rapport à la position du robot
        def tourneDe angle
        end

	# active l'évitement
        def activeEvitement
        end
        
        # désactive l'évitement
        def desactiveEvitement
        end
        
        # l'évitement est-il activé
        def evitement?
        end
end
