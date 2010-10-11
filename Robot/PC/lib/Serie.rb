# Ce fichier définit les classes de communication série
# Ces classes permettent de recevoir et d'envoyer des données via une interface série

require "serialport"
require "thread"

# Classe virtuelle de communication série, elle ne doit pas être instancié
class Serie
	
	# Initialisation à partir d'un périphérique et d'une vitesse de connexion
	def initialize(peripherique = "/dev/ttyUSB0", vitesse = 57600)
		port_str = peripherique
		baud_rate = vitesse
		data_bits = 8
		stop_bits = 1
		parity = SerialPort::NONE

		@sp = SerialPort.new(
			port_str, 
			baud_rate, 
			data_bits, 
			stop_bits, 
			parity
		)
	end
end

# Classe permettant la communication série et attend la réponce (bloquant)
class SerieSimple < Serie

	# Initialise la connection série
	def initialize(peripherique = "/dev/ttyUSB0", vitesse = 57600)
		super(peripherique,vitesse)
	end

	# Ecrit directement sur le port série
	def ecrire action
		@sp.write action + "\r\n"
	end
	
	# Lit une ligne provenant de la liaison série
	def lire
		retour = ""
		begin
			if ((caractere = @sp.getc) != nil) 
				retour << caractere.chr
			end
		end while caractere != 10
		retour.split("\r\n").first
	end
	
	# Ecrit une commande cmd sur le périphérique et retourne les 
	# informations renvoyées par le périphérique.
	# Le processus est bloquant.
	def commande cmd
		self.ecrire cmd
		self.lire
	end
end

# Classe permettant la communication série sans attendre la réponce (non bloquant)
class SerieThread < Serie

	# Initialisation à partir d'un périphérique et d'une vitesse de 
	# connexion
	def initialize(peripherique = "/dev/ttyUSB0", vitesse = 57600)
		super(peripherique,vitesse)
	end
	
	# Ecrit directement sur le port série
	def ecrire action
		@sp.write action + "\r\n"
	end
	
	# Lance un thread d'écoute du port série. A chaque nouvelle ligne, le
	# retour est envoyé à la fonction callback pour l'analyse.
	def demarrer
		@thread = Thread.new {
			retour = ""
			caractere = ""
			while true
				caractere = @sp.getc
				if caractere != nil
					retour << caractere.chr
					if caractere == 10
						retour = retour.split("\r\n").first
						callback retour
						retour = ""
					end
				end
			end
		}
	end
	
	# Fonction callback
	def callback retour
		puts retour
	end
	
	# Arrête le thread de lecture
	def arreter
		@thread.exit
	end
end
