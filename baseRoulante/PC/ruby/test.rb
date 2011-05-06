require 'libChessUp'

Socket = LibChessUp::Socket.Instance(42000);
Socket.ouvrirThread();
while(true);
end
