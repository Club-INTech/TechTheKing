# encoding: utf-8

# Ce fichiers contient l'objet ListeEvenements ou l'on inscrit tous les évènements qui peuvent survenir
# Et l'objet GestionEvenement qui s'arrange pour vérifier régulièrement si un évènement est survenu

require "Log"
require 'GestionEvenements'

class ListeEvenements < GestionEvenements

	# un évènement tout con qui va interompre la stratégie ?
        def ev_arret
        end

end

