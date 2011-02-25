require "rspec"

require "InterfaceActionneurs"

describe "Interface Asservissement" do

  before(:each) do
    @interface = InterfaceActionneurs.new("mock_tty")
  end

  it "peut allumer la led du jumper" do
    @interface.should_receive(:commande).with("b").and_return(1)
    @interface.allumerLed.should == 1
  end
  
end