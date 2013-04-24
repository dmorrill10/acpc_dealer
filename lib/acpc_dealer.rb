require File.expand_path('../hand_evaluator', __FILE__)
require File.expand_path("../acpc_dealer/dealer_runner", __FILE__)

module AcpcDealer
  VENDOR_DIRECTORY = File.expand_path('../../vendor', __FILE__)
  DEALER_DIRECTORY = File.join(VENDOR_DIRECTORY, 'project_acpc_server')
  GAME_DEF_DIRECTORY = File.join(VENDOR_DIRECTORY, 'game_defs')
  GAME_DEFINITION_FILE_PATHS =
  {
    2 =>
    {
      limit: "#{DEALER_DIRECTORY}/holdem.limit.2p.reverse_blinds.game",
      nolimit: "#{DEALER_DIRECTORY}/holdem.nolimit.2p.reverse_blinds.game",
      nolimit100b: File.join(GAME_DEF_DIRECTORY, 'game_def.holdem.2p.no_limit.100BB.reverse_blinds')
    },
    3 =>
    {
      limit: "#{DEALER_DIRECTORY}/holdem.limit.3p.game",
      nolimit: "#{DEALER_DIRECTORY}/holdem.nolimit.3p.game"
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
      nolimit: "#{DEALER_DIRECTORY}/example_player.nolimit.3p.sh"
    }
  }
end
