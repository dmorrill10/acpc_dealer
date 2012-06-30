require File.expand_path("../acpc_dealer/version", __FILE__)

require File.expand_path("../hand_evaluator", __FILE__)

module AcpcDealer
  GAME_DEFINITION_FILE_NAMES = lambda do
    path_to_project_acpc_server_directory = File.expand_path('../../ext/project_acpc_server', __FILE__)
    {
      holdem_limit_2p_reverse_blinds: "#{path_to_project_acpc_server_directory}/holdem.limit.2p.reverse_blinds.game",
      holdem_no_limit_2p_reverse_blinds: "#{path_to_project_acpc_server_directory}/holdem.nolimit.2p.reverse_blinds.game",
      holdem_limit_3p_reverse_blinds: "#{path_to_project_acpc_server_directory}/holdem.limit.3p.game",
      holdem_no_limit_3p_reverse_blinds: "#{path_to_project_acpc_server_directory}/holdem.nolimit.3p.game"
    }
  end.call
end
