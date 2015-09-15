require 'version'
require 'hand_evaluator'
require "acpc_dealer/dealer_runner"
require 'date'

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
      kuhn_sf1: "#{DEALER_DIRECTORY}/kuhn_3p_equilibrium_player.sf1.sh",
      kuhn_sf2: "#{DEALER_DIRECTORY}/kuhn_3p_equilibrium_player.sf2.sh",
      kuhn_sf3: "#{DEALER_DIRECTORY}/kuhn_3p_equilibrium_player.sf3.sh"
    }
  }

  ConnectionInformation = Struct.new(:port_number, :host_name) do
    def initialize(port_number, host_name = 'localhost')
      super port_number, host_name
    end
  end

  def self.game_def_label(number_of_players, betting_type_key)
    "#{number_of_players}P-#{betting_type_key}"
  end

  def self.default_match_name(players, game_def, seed)
    "#{players.join('-')}.#{game_def}.r#{seed}.#{date}"
  end

  private

  def self.date
    "#{Date.today.strftime('%b%-d_%Y').downcase}"
  end
end
