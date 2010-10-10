# integrer le gros passage commente au code si dessous

=begin
# Ce fichier contient la classe d'interfaçage de l'asservissement. Elle permet 
# une communication avec l'Arduino d'asservissement.
# Author::    Guillaume Rose  (mailto:guillaume.rose@gmail.com)
# Copyright:: Copyright (c) 2010 INTech - TechTheFruit
# License::   GPL

require "SerieThread"
require "Log"

require "Position"

# Cette classe convertit les consignes en commandes en ticks. Elle hérite des
# fonctions permettant la liaison série.
#--
#  * Ajouter des mutex entre callback et l'envoi de position (problème de timing possible)
#++

class InterfaceAsservissement < SerieThread

  # Contient la position du robot à chaque instant
  attr_accessor :position, :sens

  # Constantes permettant la conversion en coordonnées polaires et 
  # cartésiennes
  attr_accessor :conversionTicksDistance, :conversionTicksAngle

  attr_reader :PWM, :Vitesse

  # * Initialise la liaison série avec un périphérique à une vitesse donnée
  # * Définit les valeurs par défaut aux constantes
  def initialize(peripherique = "/dev/ttyUSB0", vitesse = 57600, positionParDefaut = Position.new(0, 0, 0))
    super(peripherique, vitesse)

    @log = Logger.instance

    @position = positionParDefaut
    @sens = 1

    @conversionTicksDistance = (9.6769 * 1)
    @conversionTicksAngle = 1528.735

    @offsetAngulaire = positionParDefaut.angle
    @offsetG = @conversionTicksAngle * positionParDefaut.angle / 2
    @offsetD = -1 * @offsetG

    @encodeurPrecedentG = @offsetG
    @encodeurPrecedentD = @offsetD

    @blocageTranslation = 0
    @blocageRotation = 0

    @skip = false

    @commandeAngle = ""
    @commandeDistance = ""

    @vecteurDeplacement = Vecteur.new

    @PWM = [1023, 1023]
    @Vitesse = [3000, 3000]
  end

  # Surcharge de la fonction callback héritée de SerieThread afin de 
  # calculer à chaque nouvelle réception de données la nouvelle position
  # du robot.
  def callback retour
    if @skip
      donnees = retour.split(" ")
      @skip = false if donnees[0].to_i.abs <= 10 && donnees[1].to_i.abs <= 10
    else
      donnees = retour.split(" ")

      return false if donnees.size != 4

      encodeurG = @offsetG + -1 * (donnees[0].to_i)
      encodeurD = @offsetD + 1  * (donnees[1].to_i)

      @blocageTranslation = donnees[3].to_i
      @blocageRotation = donnees[2].to_i

      distance = (encodeurG - @encodeurPrecedentG + encodeurD - @encodeurPrecedentD) / @conversionTicksDistance

      return false if distance > 1000

      anciennePosition = @position.clone

      @position.x += distance * Math.cos(@position.angle)
      @position.y += distance * Math.sin(@position.angle)

      ancienVecteurDeplacement = @vecteurDeplacement
      # puts @vecteurDeplacement.inspect
      @vecteurDeplacement = Vecteur.new(anciennePosition, @position)

      v = Vecteur.new
      v.x = Math.cos(@position.angle)
      v.y = Math.sin(@position.angle)

      signe = v.produitScalaire(@vecteurDeplacement)
      if signe >= 0
        @sens += 1 if @sens < 20
      else
        @sens -= 1 if @sens > -20
      end

      # puts @sens

      @position.angle = (encodeurG - encodeurD) / (@conversionTicksAngle)

      # puts donnees.inspect
      #puts @position.inspect

      @encodeurPrecedentG = encodeurG
      @encodeurPrecedentD = encodeurD
      # @log.debug "Réception codeuses : " + donnees.inspect
    end
  end

  # Envoi d'une consigne en distance et en angle au robot relatif par
  # rapport à sa position courante
  def envoiConsigne distance, angle		
    a = ((angle - @offsetAngulaire) * @conversionTicksAngle + @encodeurPrecedentG - @encodeurPrecedentD).to_i
    d = (distance * @conversionTicksDistance + @encodeurPrecedentG + @encodeurPrecedentD).to_i

    distanceFormate, angleFormate, commandeDistance, commandeAngle = formatageConsigne d, a

    # if @commandeAngle != commandeAngle + angleFormate
    ecrire commandeAngle + angleFormate
    @commandeAngle = commandeAngle + angleFormate
    # end
    # if @commandeDistance != commandeDistance + distanceFormate
    ecrire commandeDistance + distanceFormate
    @commandeDistance = commandeDistance + distanceFormate
    # end
  end

  # Envoi d'une consigne en distance et en angle absolue
  def envoiConsigneBrute distance, angle
    distanceFormate, angleFormate, commandeDistance, commandeAngle = formatageConsigne distance, angle

    # if @commandeAngle != commandeAngle + angleFormate
    ecrire commandeAngle + angleFormate
    @commandeAngle = commandeAngle + angleFormate
    # end
    # if @commandeDistance != commandeDistance + distanceFormate
    ecrire commandeDistance + distanceFormate
    @commandeDistance = commandeDistance + distanceFormate
    # end
  end

  # Envoi d'une consigne en angle
  def envoiConsigneAngle angle
    a = ((angle - @offsetAngulaire) * @conversionTicksAngle + @encodeurPrecedentG - @encodeurPrecedentD).to_i
    distanceFormate, angleFormate, commandeDistance, commandeAngle = formatageConsigne 0, a

    # if @commandeAngle != commandeAngle + angleFormate
    ecrire commandeAngle + angleFormate
    @commandeAngle = commandeAngle + angleFormate
    # end
  end

  # Envoi d'une consigne en distance
  def envoiConsigneDistance distance
    d = (distance * @conversionTicksDistance + @encodeurPrecedentG + @encodeurPrecedentD).to_i
    distanceFormate, angleFormate, commandeDistance, commandeAngle = formatageConsigne d, 0

    # if @commandeDistance != commandeDistance + distanceFormate
    ecrire commandeDistance + distanceFormate
    @commandeDistance = commandeDistance + distanceFormate
    # end
  end

  # Demande au robot d'activer l'envoi des données de roues codeuses sur
  # la liaison série
  def activeOdometrie
    ecrire "c"
  end

  # Désactive l'envoi de données
  def desactiveOdometrie
    ecrire "d"
  end

  # Bascule l'état de l'asservissement d'un état vers un autre 
  # (tout ou rien)
  def desactiveAsservissementRotation
    ecrire "h"
  end

  def desactiveAsservissementTranslation
    ecrire "i"
  end

  def desactiveAsservissement
    ecrire "h"
    ecrire "i"
  end

  # Reset du périphérique
  # * Désactivation de l'odométrie
  # * Remise à zéro des consignes et des codeuses
  def reset
    desactiveOdometrie
    ecrire "j"
  end

  # Remise à zéro des codeuses et de la consigne
  def remiseAZero nouvellePosition
    ecrire "j"

    @skip = true

    @position = nouvellePosition

    @offsetAngulaire = nouvellePosition.angle
    @offsetG = (@conversionTicksAngle * nouvellePosition.angle / 2).to_i
    @offsetD = (-1 * @offsetG).to_i

    @encodeurPrecedentG = @offsetG.to_i
    @encodeurPrecedentD = @offsetD.to_i
  end

  # Formate les consignes afin de les transmettre
  def formatageConsigne distance, angle
    if angle > 0
      commandeAngle = "g"
    else
      commandeAngle = "b"
      angle *= -1
    end

    if distance > 0
      commandeDistance = "f"
    else
      commandeDistance = "a"
      distance *= -1
    end

    [distance.to_i.to_s.rjust(8, "0"), angle.to_i.to_s.rjust(8, "0"), commandeDistance, commandeAngle]
  end

  # Renvoi l'état des codeuses gauche et droite
  def codeuses
    [@encodeurPrecedentG, @encodeurPrecedentD]
  end

  # Renvoi l'état des asservissements
  def blocage
    [@blocageTranslation, @blocageRotation]
  end

  # Renvoi vrai si l'asservissement en translation est bloqué
  def blocageTranslation
    @blocageTranslation
  end

  # Renvoi vrai si l'asservissement en rotation est bloqué
  def blocageRotation
    @blocageRotation
  end

  # Arrêt progressif du robot
  def stop
    ecrire "n"
  end

  # Arrêt brutal du robot
  def stopUrgence
    ecrire "o"
  end

  # Change la vitesse du robot (rotation, translation)
  def changerVitesse(valeur)
    valeur = [0, 0] if valeur.size != 2
    ecrire "r" + valeur[0].to_s.rjust(8, "0")
    ecrire "l" + valeur[1].to_s.rjust(8, "0")
    @Vitesse = valeur
  end

  # Change l'accélération du robot (rotation, translation)
  def changerAcceleration(valeur)
    valeur = [0, 0] if valeur.size != 2
    ecrire "q" + valeur[0].to_s.rjust(8, "0")
    ecrire "k" + valeur[1].to_s.rjust(8, "0")
  end

  # Change la valeur du PWN (rotation, translation)
  def changerPWM(valeur)
    valeur = [0, 0] if valeur.size != 2
    ecrire "t" + valeur[0].to_s.rjust(8, "0")
    ecrire "p" + valeur[1].to_s.rjust(8, "0")
    @PWM = valeur
  end

  # Change la valeur de KP (rotation, translation)
  def changerKp(valeur)
    valeur = [0, 0] if valeur.size != 2
    ecrire "s" + valeur[0].to_s.rjust(8, "0")
    ecrire "m" + valeur[1].to_s.rjust(8, "0")
  end

  # Change la valeur de KP (rotation, translation)
  def changerKd(valeur)
    valeur = [0, 0] if valeur.size != 2
    ecrire "v" + valeur[0].to_s.rjust(8, "0")
    ecrire "u" + valeur[1].to_s.rjust(8, "0")
  end

end



=end

require "Float"
require "Fixnum"

require "Log"

require "Point"
require "Vecteur"

require "InterfaceAsservissement"

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

    #@interface.conversionTicksDistance = 9.50
    #@interface.conversionTicksAngle = 1530,9

    @interface.changerVitesse([3000, 3000])
    # @interface.changerAcceleration([10, 10])
    @interface.changerPWM([2000, 2046])
    # @interface.changerKp([0, 0])
    # @interface.changerKd([0, 0])

    @log.debug "Asservissement prêt"
    @sens = 1
  end

  # Position du robot
  def position
    @interface.position
  end

  # Reset l'Arduino et active l'odométrie
  def demarrer
    @log.debug "Activation de l'odométrie"
    @interface.activeOdometrie
  end

  # Arrête le robot
  def arreter
    @log.debug "Arrêt de l'asservissement"
    @interface.stopUrgence

    @log.debug "Désactivation de l'odométrie"
    @interface.desactiveOdometrie
  end

  # Reset l'odométrie, des consignes et des codeuses sur l'arduino
  def reset
    @interface.reset
  end

  # Remise à zéro de l'arduino, changement de la position du robot
  def remiseAZero nouvellePosition = Position.new(0, 0, 0)
    @log.debug "Remise à zéro des consignes et des codeuses"
    @interface.remiseAZero nouvellePosition
  end

  # Se déplace en absolu à la destination indiquée
  def goToEx destination, *condition
    begin
      @drapeauArret = false

      evaluationConditionArret(*condition)

      # Zone 51 ...
      v = Vecteur.new(position, destination)
      if v.norme < 500
        corrigeTrajectoire destination, :angulaire
      end

      corrigeTrajectoire destination	               

      i = 0
      @interface.changerPWM([2046,2046])
      while i < 200 && (distance = Vecteur.new(position, destination).norme) >= @precisionDouble
        # En cas de stop, on arrête de corriger la position
        # On sort alors du goTo
        evaluationConditionArret(*condition)

        if distance >= @precisionSimple
          corrigeTrajectoire destination
        end

        i += 1
        sleep 0.05
      end

      if i >= 200
        @log.debug "Problème pour atteindre la destination (timeout)"
        raise "timeout"
      end

      # tourneDe(destination.angle - position.angle, *condition)

      @log.debug "Fin Aller à : " + position.x.to_s + ", " + position.y.to_s

      return true
    rescue  RuntimeError => e
      puts "gotoEx"
      puts e
      if e.to_s.include? "blocageTrans"
        @log.debug "BlocageTrans"
        stopUrgence
        puts "stop urgence"
      end
      if e.to_s == "stop"
        @log.debug "Arret demandé"
        arret
      end
      if e.to_s == "timeout"
        @log.debug "Timeout"
      end
      raise e
    end
    true
  end

  def goTo *param
    begin
      goToEx *param
    rescue
      return false
    end
    true
  end

  def avancer distance
    @interface.changerPWM([2046,2046])
    puts "avancer",distance
    @interface.envoiConsigneDistance(distance)

  end

  def tourner angle
    @interface.changerPWM([3000,500])
    puts "tourner",angle
    @interface.envoiConsigneAngle(angle)

  end

  def returnPoint(debut,position,sens)
    vd = Vecteur.new(position,debut )
    vd = vd.normalise
    v = vd * 150
    o = vd.ortho	
    if sens == 0
      o = o * -1
    end
    o = o * 40
    vr = sum(v, o)
    npos = Position.new(position.x + vr.x,position.y + vr.y)
    return npos
  end

  def goToDep destination, *condition
    puts "goToDep"
    depart = position.clone
    begin
      goToEx destination, :blocageTranslation, *condition
    rescue  RuntimeError => e
      puts "goToDep",e
      @interface.changerPWM([1023,200])

      puts "reaction stop urgence"
      distRec = 150
      distRot = 0.2
      if e.to_s == "blocageTransAVG"
        @log.debug "Blocage AVG"
        #pos = position
        #npos = returnPoint(depart,pos,1)
        #puts "rescuePoint"
        #pos.prettyprint				
        #npos.prettyprint
        tourner distRot
        sleep 0.5	
        @interface.changerPWM([1023,1023])			
        avancer -distRec
        sleep 1
        #goTo npos, :bypass

      elsif e.to_s == "blocageTransAVD"
        @log.debug "Blocage AVD"
        tourner -distRot
        sleep 0.5	
        @interface.changerPWM([1023,1023])			
        avancer -distRec
        sleep 1

      elsif e.to_s == "blocageTransARG"
        @log.debug "Blocage ARG"
        tourner -distRot
        sleep 0.5
        @interface.changerPWM([1023,1023])				
        avancer distRec
        sleep 1

      elsif e.to_s == "blocageTransARD"
        @log.debug "Blocage ARD"
        tourner distRot
        sleep 0.5	
        @interface.changerPWM([1023,1023])			
        avancer distRec
        sleep 1
        #goTo npos, :bypass
      elsif e.to_s == "stop"
        @interface.changerPWM([1023,1023])
        return true
      end
      @interface.changerPWM([1023,1023])
      return false
    end
    return true
  end


  # Corrige la trajectoire pour atteindre la destination
  def corrigeTrajectoire destination, *condition
    v = Vecteur.new(position, destination)

    angleAFaire = (v.angle - position.angle).modulo2

    if (angleAFaire > Math::PI / 2) || (angleAFaire < -Math::PI / 2)
      consigneDistance = -1 * v.norme
      if consigneDistance > 0 
        @sens = 1
      else 
        @sens = -1
      end

      if (angleAFaire - Math::PI - position.angle).abs > Math::PI
        consigneAngle = (angleAFaire + Math::PI ).modulo2
      else
        consigneAngle = (angleAFaire - Math::PI).modulo2 
      end 
    else
      consigneDistance = v.norme
      consigneAngle = angleAFaire
    end

    if condition.empty?
      @interface.envoiConsigne consigneDistance, consigneAngle
    else
      tourneDe consigneAngle
    end
  end

  def evaluationConditionArret *item
    if !item.include?(:bypass)
      if item.empty?
        item = [:blocageTranslation]
      end
      raise "stop" if @drapeauArret == true
      item.each { |f|  
        send(f)
      }
    end
  end

  # Tourne relativement à la position du robot
  def tourneDe angleDonne
    @interface.changerPWM([3000, 700])
    @drapeauArret = false

    positionInitiale = position.angle

    evaluationConditionArret(:blocageRotation)

    @interface.envoiConsigneAngle angleDonne	               

    i = 0
    while i < 180 && (position.angle - positionInitiale - angleDonne).abs >= 0.05
      evaluationConditionArret(:blocageRotation)

      i += 1
      sleep 0.05
    end

    if i >= 180
      @log.debug "Problème pour atteindre la rotation (timeout)"
      raise "timeout"
    end

    @log.debug "Rotation finie"

    true
  end


  # Désactive l'asservissement
  def desactiveAsservissement
    @interface.desactiveAsservissementTranslation
    @interface.desactiveAsservissementRotation
  end

  # Désactive l'asservissement en translation
  def desactiveAsservissementTranslation
    @interface.desactiveAsservissementTranslation
  end

  # Désactive l'asservissement en rotation	
  def desactiveAsservissementRotation
    @interface.desactiveAsservissementRotation
  end

  # Renvoi l'état des codeuses
  def codeuses
    @interface.codeuses
  end

  def blocageTranslation
    raise "blocageTransAVD" if @interface.blocageTranslation == -1 #and @sens == 1
    raise "blocageTransAVG" if @interface.blocageTranslation == -2 #and @sens == 1
    raise "blocageTransARD" if @interface.blocageTranslation == 1 #and @sens == -1
    raise "blocageTransARG" if @interface.blocageTranslation == 2 #and @sens == -1
  end

  def blocageRotation
    raise "blocageROT" if @interface.blocageRotation != 0
  end

  def blocage
    blocageTranslation
    blocageRotation
  end


  def recalage
    @interface.changerVitesse([1300, 500])
    remiseAZero Position.new(310, 310, Math::PI/2)
    @interface.changerPWM([700, 2046])
    goTo Position.new(310, -2500, Math::PI/2) , :blocageTranslation
    goTo Position.new(310, -2500, Math::PI/2), :blocageTranslation
    sleep 0.5		
    remiseAZero Position.new(position.x, 170, Math::PI/2+0.0085)
    sleep 0.5
    # return true
    @interface.changerPWM([2046, 2046])
    goTo Position.new(position.x, 310), :bypass
    sleep 0.5
    position.prettyprint		
    tourneDe -Math::PI/2 
    @interface.changerPWM([700, 2046]) 
    goTo Position.new(-2500, position.y, 0), :blocageTranslation
    goTo Position.new(-2500, position.y, 0), :blocageTranslation
    sleep 0.5
    remiseAZero Position.new(170, position.y, 0.0085)
    sleep 0.5
    # return true
    @interface.changerPWM([2046,2046])
    # sleep 1  
    goTo Position.new(310, position.y, 0), :bypass                
    position.prettyprint		
    #goTo Position.new(310, 310, 0), :bypass
    sleep 0.5
    alignement Math::PI
    @interface.changerVitesse([2000, 1000])
    sleep 1
    position.prettyprint
  end	

  def recalage3
    @interface.changerVitesse([1300, 500])
    remiseAZero Position.new(310, -310, -Math::PI/2)
    @interface.changerPWM([300, 1023])
    goTo Position.new(310, 2500, -Math::PI/2) , :blocageTranslation
    goTo Position.new(310, 2500, -Math::PI/2), :blocageTranslation
    sleep 0.5		
    remiseAZero Position.new(position.x, -170, -Math::PI/2+0.0085)
    sleep 0.5
    # return true
    @interface.changerPWM([1023, 1023])
    goTo Position.new(position.x, -310), :bypass
    sleep 0.5
    position.prettyprint		
    tourneDe Math::PI/2 
    @interface.changerPWM([300, 1023]) 
    goTo Position.new(-2500, position.y, 0), :blocageTranslation
    goTo Position.new(-2500, position.y, 0), :blocageTranslation
    sleep 0.5
    remiseAZero Position.new(170, position.y, 0.0085)
    sleep 0.5
    # return true
    @interface.changerPWM([1023, 1023])
    # sleep 1  
    goTo Position.new(310, position.y, 0), :bypass                
    position.prettyprint		
    #goTo Position.new(310, 310, 0), :bypass
    sleep 0.5
    alignement Math::PI
    @interface.changerVitesse([2000, 1000])



    # @interface.changerVitesse([1000, 1000])
    # 
    # @interface.changerPWM([300, 1023])
    # 
    # goTo Position.new(-400, 0, 0), :blocageTranslation
    # sleep 1
    # goTo Position.new(-400, 0, 0), :blocageTranslation
    # remiseAZero Position.new(170, 0, 0)
    # sleep 1
    # @interface.changerPWM([1023, 1023])
    # # sleep 1  
    # goTo Position.new(285, -position.y, -Math::PI/2), :bypass
    # tourneDe -Math::PI/2 
    # @interface.changerPWM([300, 1023])
    # goTo Position.new(300, 2500, -Math::PI/2), :blocageTranslation
    # sleep 1
    # goTo Position.new(300, 2500, -Math::PI/2), :blocageTranslation
    # remiseAZero Position.new(position.x, -170, -Math::PI/2)
    # # sleep 1
    # @interface.changerPWM([1023, 1023])
    # goTo Position.new(position.x, -300, -Math::PI/2), :bypass
    # goTo Position.new(300, -300, 0)
    # 
    # @interface.changerVitesse([2000, 1000])
  end

  def recalage2 direction, sens, coordonneeReset, avancementMax = 1500
    positionIntermediaire = position.clone

    if direction == :x
      # Alignement
      positionIntermediaire.angle = positionIntermediaire.angle - (positionIntermediaire.angle % 2 * Math::PI)

      if positionIntermediaire.angle > Math::PI 
        positionIntermediaire.angle -= 2 * Math::PI
      end

      positionIntermediaire.prettyprint

      goTo positionIntermediaire

      # Recule ou avance sans asservissement rotation
      @interface.changerVitesse([1000, 1000])
      @interface.changerPWM([300, 1024])

      if sens == :positif
        positionIntermediaire.x += avancementMax
      else
        positionIntermediaire.x -= avancementMax
      end

      positionIntermediaire.prettyprint  

      goTo positionIntermediaire, :blocageTranslation

      sleep 1

      # On se cale bien 
      puts goTo positionIntermediaire, :blocageTranslation

      # Reset
      remiseAZero Position.new(coordonneeReset, position.y, position.angle)
    else
      positionIntermediaire.angle = positionIntermediaire.angle - (positionIntermediaire.angle % 2 * Math::PI) + (Math::PI / 2)

      if positionIntermediaire.angle > Math::PI 
        positionIntermediaire.angle -= 2 * Math::PI
      end

      goTo positionIntermediaire

      @interface.changerVitesse([1000, 1000])
      @interface.changerPWM([300, 1024])

      if sens == :positif
        positionIntermediaire.y += avancementMax
      else
        positionIntermediaire.y -= avancementMax
      end

      goTo positionIntermediaire, :blocageTranslation

      sleep 1

      goTo positionIntermediaire, :blocageTranslation

      remiseAZero Position.new(position.x, coordonneeReset, position.angle)
    end

    @interface.changerVitesse([3000, 3000])
    @interface.changerPWM([1023, 1023])
  end

  def arret
    @interface.stop
  end


  # Arrêt progressif du robot
  def stop
    @drapeauArret = true
    @log.debug "Appel fonction stop"
    # @interface.stop
  end

  # Arrêt brutal du robot
  def stopUrgence
    # @drapeauArret = true
    @interface.stopUrgence
  end

  # Sens de déplacement du robot (1 ou 0)
  def sens
    if @interface.sens == 20
      return 1
    end
    if @interface.sens == -20
      return -1
    end
    return 0
  end

  def changerVitesse(rotation, translation)
    @interface.changerVitesse([rotation, translation])
  end


  def alignement angle
    tourneDe((angle - position.angle).modulo2)
  end

end
