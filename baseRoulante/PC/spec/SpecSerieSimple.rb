require "rspec"

require "SerieSimple"

describe "Serie Simple" do

  before(:each) do
    @serie_physique = mock(SerialPort)
    SerialPort.stub(:new).and_return(@serie_physique)
  end

  it "peut allumer la led du jumper" do
    @serie_physique.should_receive(:write).with("bonjour\r\n")
    
    interface = Serie.new
    interface.ecrire "bonjour"  
  end
  
end