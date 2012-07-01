require File.expand_path('../hand_evaluator', __FILE__)
require File.expand_path("../acpc_dealer/dealer_runner", __FILE__)

module AcpcDealer
  DEALER_DIRECTORY = File.expand_path('../../vendor/project_acpc_server', __FILE__)
  GAME_DEFINITION_FILE_PATHS =
  {
    two_player:
    {
      limit: "#{DEALER_DIRECTORY}/holdem.limit.2p.reverse_blinds.game",
      no_limit: "#{DEALER_DIRECTORY}/holdem.nolimit.2p.reverse_blinds.game"
    },
    three_player:
    {
      limit: "#{DEALER_DIRECTORY}/holdem.limit.3p.game",
      no_limit: "#{DEALER_DIRECTORY}/holdem.nolimit.3p.game"
    }
  }
  DEALER_PATH = "#{DEALER_DIRECTORY}/dealer"
  EXAMPLE_PLAYERS =
  {
    two_player:
    {
      limit: "#{DEALER_DIRECTORY}/example_player.limit.2p.sh",
      no_limit: "#{DEALER_DIRECTORY}/example_player.nolimit.2p.sh"
    },
    three_player:
    {
      limit: "#{DEALER_DIRECTORY}/example_player.limit.3p.sh",
      no_limit: "#{DEALER_DIRECTORY}/example_player.nolimit.3p.sh"
    }
  }
end
