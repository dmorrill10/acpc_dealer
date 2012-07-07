require File.expand_path('../hand_evaluator', __FILE__)
require File.expand_path("../acpc_dealer/dealer_runner", __FILE__)

module AcpcDealer
  DEALER_DIRECTORY = File.expand_path('../../vendor/project_acpc_server', __FILE__)
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
