# Ce fichier définit la classe permettant de se déplacer
# Cette classe les déplacements (haut niveau) en commande pour l'asservissement (bas niveau)

require "Float"
require "Fixnum"
require "Log"
require "Point"
require "Vecteur"
require "SerieThread"
require "Position"


# Cette classe gère l'asservissement

class Asservissement

  # Précision du goTo : cercle à partir duquel on n'envoit plus de 
  # nouvelle consigne
  attr_accessor :precisionSimple,:sens

  # Précision du goTo : cercle à partir duquel on déblocage le déroulement
  # de la stratégie
  attr_accessor :precisionDouble

  # Initialise l'asservissement à partir d'un périphérique série, valeurs 
  # par défaut pour la précision
  def initialize peripherique, positionParDefaut = Position.new(0, 0, 0)
    @log = Logger.instance

    if peripherique == nil
      raise "Pas de carte pour Asservissement" 
    end

    @log.debug "Asservissement sur " + peripherique

    @precisionSimple = 50
    @precisionDouble = 20

    @drapeauArret = false

    @interface = InterfaceAsservissement.new peripherique, 57600, positionParDefaut
    @interface.demarrer

    @interface.changerVitesse([3000, 3000])
    @interface.changerPWM([2000, 2046])
    @log.debug "Asservissement prêt"
    @sens = 1
  end

  def position
  end

  def demarrer
  end

  def arreter
  end

  def reset
  end

  def remiseAZero nouvellePosition = Position.new(0, 0, 0)
  end

  def goto(where)
  end

  def avancer distance
  end

  def tourner angle
  end

  def tourneDe angleDonne
  end

  def desactiveAsservissement
  end

  def desactiveAsservissementTranslation
  end

  def desactiveAsservissementRotation
  end

  def codeuses
  end

  def blocageTranslation
  end

  def blocageRotation
  end

  def blocage
  end

  def recalageZone(zone)
  end

  def recalageMatch
  end

  def arret
  end

  def stop
  end

  def stopUrgence
  end

  def sens
  end

  def changerVitesse(rotation, translation)
  end


  def alignement angle
  end

end
