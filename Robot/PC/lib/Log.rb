# Ce fichier définit la classe de log
# Son utilisation est fortement recommandée lorsque vous programmer

require 'logger'
require 'singleton'

# Cette classe permet d'afficher les logs soit sur la sortie des erreurs,
# soit dans un fichier avec un certain niveau de log.

class Logger
    include Singleton
    @@old_initialize = Logger.instance_method :initialize

    def initialize
        # @@old_initialize.bind(self).call("log/" + Time.now.strftime("%m.%d.%Y-%Hh%M"))
        @@old_initialize.bind(self).call(STDERR)
    end  
end

