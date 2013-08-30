require 'hand_evaluator'
require "acpc_dealer/dealer_runner"

module AcpcDealer
  VENDOR_DIRECTORY = File.expand_path('../../vendor', __FILE__)
  DEALER_DIRECTORY = File.join(VENDOR_DIRECTORY, 'project_acpc_server')
  GAME_DEFINITION_FILE_PATHS =
  {
    2 =>
    {
      limit: "#{DEALER_DIRECTORY}/holdem.limit.2p.reverse_blinds.game",
      nolimit: "#{DEALER_DIRECTORY}/holdem.nolimit.2p.reverse_blinds.game"
    },
    3 =>
    {
      limit: "#{DEALER_DIRECTORY}/holdem.limit.3p.game",
      nolimit: "#{DEALER_DIRECTORY}/holdem.nolimit.3p.game",
      kuhn: "#{DEALER_DIRECTORY}/kuhn.limit.3p.game"
    }
  }
  DEALER_PATH = "#{DEALER_DIRECTORY}/dealer"
  EXAMPLE_PLAYERS =
  {
    2 =>
    {
      limit: "#{DEALER_DIRECTORY}/example_player.limit.2p.sh",
      nolimit: "#{DEALER_DIRECTORY}/example_player.nolimit.2p.sh"
    },
    3 =>
    {
      limit: "#{DEALER_DIRECTORY}/example_player.limit.3p.sh",
      nolimit: "#{DEALER_DIRECTORY}/example_player.nolimit.3p.sh",
      kuhn: "#{DEALER_DIRECTORY}/kuhn_3p_equilibrium_player.sf1.sh"
    }
  }

  ConnectionInformation = Struct.new(:port_number, :host_name) do
    def initialize(port_number, host_name = 'localhost')
      super port_number, host_name
    end
  end
end
